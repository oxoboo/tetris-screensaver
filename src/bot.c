/*
 * Copyright (c) 2024-2025 Oxoboo
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include "bot.h"
#include "errorvalues.h"

bot_t bot_new(void) {
    return (bot_t) {
        .holes = 0,
        .line_deps_cells = 0,
        .dest_orient_index = 0,
        .dest_x = 0,
    };
}

/* Count empty cells that are under at least one filled cell. */
uint32_t count_holes(const matrix_t* matrix) {
    uint32_t holes = 0;
    bool top_reached;
    for (size_t c = 0; c < matrix->cols; ++c) {
        top_reached = false;
        for (size_t r = 0; r < matrix->rows; ++r) {
            if (!top_reached) {
                if (matrix->table[r][c] != TYPE_NONE) {
                    top_reached = true;
                }
            } else {
                if (matrix->table[r][c] == TYPE_NONE) {
                    ++holes;
                }
            }
        }
    }
    return holes;
}

/*
 * Count empty cells that can only be filled with a line piece. Any gap that is exactly one cell
 * wide and at least three cells deep will have at least one of these cells.
 *
 * This function needs a matrix at least two columns wide. This function will return 0 if the
 * matrix is less than two columns wide.
 */
uint32_t count_line_dep_cells(const matrix_t* matrix, bool ignore_rightmost_col) {
    if (matrix->cols < 2) {
        return 0;
    }
    uint32_t cells = 0;
    size_t r;
    /* left-most column */
    for (r = 0; r < matrix->rows; ++r) {
        if (matrix->table[r][1] != TYPE_NONE) {
            break;
        }
    }
    r += 2;
    for ( ; r < matrix->rows; ++r) {
        if (matrix->table[r][0] == TYPE_NONE) {
            ++cells;
        } else {
            break;
        }
    }
    /* between left and right-most column */
    for (size_t c = 1; c < matrix->cols - 1; ++c) {
        for (r = 0; r < matrix->rows; ++r) {
            if (matrix->table[r][c - 1] != TYPE_NONE && matrix->table[r][c + 1] != TYPE_NONE) {
                break;
            }
        }
        r += 2;
        for ( ; r < matrix->rows; ++r) {
            if (matrix->table[r][c] == TYPE_NONE) {
                ++cells;
            } else {
                break;
            }
        }
    }
    /* right-most column */
    if (!ignore_rightmost_col) {
        for (r = 0; r < matrix->rows; ++r) {
            if (matrix->table[r][matrix->cols - 2] != TYPE_NONE) {
                break;
            }
        }
        r += 2;
        for ( ; r < matrix->rows; ++r) {
            if (matrix->table[r][matrix->cols - 1] == TYPE_NONE) {
                ++cells;
            } else {
                break;
            }
        }
    }
    return cells;
}

/* Find the standard deviation of the heights of each column of the stack. */
double stack_deviation(const matrix_t* matrix) {
    uint32_t cols = matrix->cols;
    uint32_t heights[cols];
    for (size_t c = 0; c < cols; ++c) {
        heights[c] = 0;
        for (size_t r = 0; r < matrix->rows; ++r) {
            if (matrix->table[r][c] != TYPE_NONE) {
                heights[c] = matrix->rows - r;
                break;
            }
        }
    }
    uint32_t sum = 0;
    for (size_t i = 0; i < cols; ++i) {
        sum += heights[i];
    }
    double mean = (double)sum / cols;
    double sum_squares = 0;
    for (size_t i = 0; i < cols; ++i) {
        sum_squares += pow(fabs(heights[i] - mean), 2);
    }
    return sqrt(sum_squares / (cols - 1));
}

uint32_t get_stack_height(const matrix_t* matrix) {
    for (size_t r = 0; r < matrix->rows; ++r) {
        for (size_t c = 0; c < matrix->cols; ++c) {
            if (matrix->table[r][c] != TYPE_NONE) {
                return (matrix->rows - r);
            }
        }
    }
    return 0;
}

/* Count filled cells in a column. */
uint32_t count_in_col(const matrix_t* matrix, uint32_t col) {
    uint32_t num = 0;
    for (size_t r = 0; r < matrix->rows; ++r) {
        if (matrix->table[r][col] != TYPE_NONE) {
            ++num;
        };
    }
    return num;
}

/*
 * Return a piece that satisfies certain conditions:
 * + The piece can be placed without making more holes in the stack.
 * + The piece can be placed without creating more line dependencies.
 * + The piece can be placed without making the stack too high.
 *
 * If there is no piece that satisfies these conditions, then return a random piece. This function
 * may test the tetriminoes in random order. Because this function calls rand(), srand() should be
 * called at least once before calling this function.
 *
 * See documentation on bot_find_place.
 */
piece_t* bot_next_piece(bot_t* bot, const matrix_t* matrix, int32_t* err_value) {
    uint8_t bag[NUM_PIECES] = {
        TYPE_LINE,
        TYPE_O,
        TYPE_J,
        TYPE_L,
        TYPE_S,
        TYPE_T,
        TYPE_Z,
    };
    /* Shuffle the bag. */
    uint8_t bag_randomized[NUM_PIECES];
    for (size_t i = 0; i < NUM_PIECES; ++i) {
        uint32_t index = rand() % (NUM_PIECES - i);
        bag_randomized[i] = bag[index];
        bag[index] = bag[NUM_PIECES - i - 1];
    }
    uint32_t init_holes = count_holes(matrix);
    uint32_t init_line_deps = count_line_dep_cells(matrix, true);
    uint32_t stack_height_limit;
    if (matrix->rows > 4) {
        stack_height_limit = matrix->rows - matrix->hidden_rows - 4;
    } else {
        stack_height_limit = matrix->rows - matrix->hidden_rows;
    }
    int32_t tmp_err_value = 0;
    uint8_t type = rand() % NUM_PIECES + 1; /* default value */
    for (size_t i = 0; i < NUM_PIECES; ++i) {
        tmp_err_value = bot_find_place(bot, matrix, bag_randomized[i]);
        if (tmp_err_value < 0) {
            *err_value = tmp_err_value;
            return NULL;
        }
        bool has_hole = bot->holes > init_holes;
        bool has_line_dep = bot->line_deps_cells > init_line_deps;
        bool stack_too_high = bot->stack_height > stack_height_limit;
        if (!has_hole && !has_line_dep && !stack_too_high) {
            type = bag_randomized[i];
            break;
        }
    }

    piece_t* piece = piece_new(matrix, type);
    if (piece == NULL) {
        *err_value = ERROR_PIECE;
    }
    return piece;
}

/*
 * Find the "best" placement (from spawn).
 *
 * The algorithm will normally prioritize the following from most to least important:
 * 1. Avoid making gaps in the stack.
 * 2. Avoid creating line piece dependencies.
 * 3. Leave the rightmost column as empty as possible unless there is at least one hole in the
 *    stack.
 * 4. Stack as flat as possible.
 *
 * The point of the bot is to play humanely enough under certain conditions. This algorithm is
 * pretty simple. It cannot tuck in pieces under overhangs, nor can it do t-spins, or look at a
 * "next" queue. To prevent the bot from making inevitable mistakes, it should be given certain
 * pieces at certain times (cheat).
 *
 * This function will allocate memory. Return 0 on success or a non-zero value on failure.
 */
int32_t bot_find_place(bot_t* bot, const matrix_t* matrix, uint8_t piece_type) {
    matrix_t* tmp_matrix = matrix_new(matrix->rows, matrix->cols, matrix->hidden_rows);
    if (tmp_matrix == NULL) {
        return ERROR_MATRIX;
    }
    piece_t* tmp_piece = piece_new(tmp_matrix, piece_type);
    if (tmp_piece == NULL) {
        return ERROR_PIECE;
    }
    if (!matrix_copy_table(tmp_matrix, matrix)) {
        return ERROR_MATRIX_DIM_MISMATCH;
    }
    double lowest_dev = DBL_MAX;
    uint32_t least_holes = UINT32_MAX;
    uint32_t least_line_dep_cells = UINT32_MAX;
    uint32_t least_in_rightmost_col = UINT32_MAX;
    int32_t init_x = tmp_piece->x;
    int32_t init_y = tmp_piece->y;
    /* brute force, evaluate each x-coordinate and each rotation */
    for (size_t i = 0; i < tmp_piece->orientations; ++i) {
        tmp_piece->orient_index = i;
        tmp_piece->x = init_x;
        tmp_piece->y = init_y;
        bool collides = false;
        while (!collides) {
            collides = !piece_move_left(tmp_piece, tmp_matrix);
        }
        int32_t left_range = tmp_piece->x;
        tmp_piece->x = init_x;
        collides = 0;
        while (!collides) {
            collides = !piece_move_right(tmp_piece, tmp_matrix);
        }
        int32_t right_range = tmp_piece->x;
        for (int32_t x = left_range; x <= right_range; ++x) {
            matrix_copy_table(tmp_matrix, matrix);
            tmp_piece->x = x;
            tmp_piece->y = init_y;
            collides = 0;
            while (!collides) {
                collides = !piece_move_down(tmp_piece, tmp_matrix);
            }
            piece_place(tmp_piece, tmp_matrix);
            matrix_clean(tmp_matrix);
            /* evaluate the placement */
            bool overwrite = false;
            double dev = stack_deviation(tmp_matrix);
            uint32_t holes = count_holes(tmp_matrix);
            uint32_t line_dep_cells = count_line_dep_cells(tmp_matrix, true);
            uint32_t in_rightmost_col = count_in_col(tmp_matrix, tmp_matrix->cols - 1);
            overwrite = holes < least_holes;
            if (holes == least_holes) {
                overwrite = line_dep_cells < least_line_dep_cells;
                if (line_dep_cells == least_line_dep_cells) {
                    overwrite = in_rightmost_col < least_in_rightmost_col && holes == 0;
                    if (in_rightmost_col == least_in_rightmost_col || holes > 0) {
                        overwrite = dev < lowest_dev;
                        if (dev == lowest_dev) {
                            overwrite = in_rightmost_col < least_in_rightmost_col;
                        }
                    }
                }
            }

            if (overwrite) {
                lowest_dev = dev;
                least_holes = holes;
                least_line_dep_cells = line_dep_cells;
                least_in_rightmost_col = in_rightmost_col;
                bot->holes = holes;
                bot->line_deps_cells = line_dep_cells;
                bot->stack_height = get_stack_height(tmp_matrix);
                bot->dest_orient_index = i;
                bot->dest_x = x;
            }
        }
    }
    matrix_free(tmp_matrix);
    piece_free(tmp_piece);
    return 0;
}

/*
 * Update an array of inputs that will be made at a certain moment. The piece will always be moved
 * in the following order:
 * 1. Rotate the piece to the desired orientation.
 * 2. Move the piece to the desired columns.
 * 3. Move the piece down until it is placed on the stack.
 *
 * Before calling this function, pass `bot` to bot_find_piece for every piece the game spawns,
 * or use bot_next_piece.
 */
void bot_update_inputs(bot_t* bot, bool inputs[BOT_NUM_INPUTS], const piece_t* piece) {
    for (size_t i = 0; i < BOT_NUM_INPUTS; ++i) {
        inputs[i] = 0;
    }
    int64_t x = piece->orient_index;
    int64_t d = bot->dest_orient_index;
    if (x != d) {
        int64_t n = piece->orientations;
        bool is_cw_optimal = true;
        if (d > x) {
            is_cw_optimal = llabs(d - x) <= llabs(x + (n - d));
        } else if (x > d) {
            is_cw_optimal = llabs(d - x) >= llabs(x - (n + d));
        }
        if (is_cw_optimal) {
            inputs[BOT_INPUT_CW] = 1;
        } else {
            inputs[BOT_INPUT_CCW] = 1;
        }
    } else if (piece->x < bot->dest_x) {
        inputs[BOT_INPUT_RIGHT] = 1;
    } else if (piece->x > bot->dest_x) {
        inputs[BOT_INPUT_LEFT] = 1;
    } else if (piece->x == bot->dest_x) {
        inputs[BOT_INPUT_DOWN] = 1;
    }
}
