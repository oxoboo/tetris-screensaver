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
#include <string.h>
#include "matrix.h"

enum {
    LINE_ORIENTS = 2,
    LINE_ROWS = 4,
    LINE_COLS = 4,
    O_ORIENTS = 1,
    O_ROWS = 4,
    O_COLS = 4,
    J_ORIENTS = 4,
    J_ROWS = 3,
    J_COLS = 3,
    L_ORIENTS = 4,
    L_ROWS = 3,
    L_COLS = 3,
    S_ORIENTS = 2,
    S_ROWS = 3,
    S_COLS = 3,
    T_ORIENTS = 4,
    T_ROWS = 3,
    T_COLS = 3,
    Z_ORIENTS = 2,
    Z_ROWS = 3,
    Z_COLS = 3,
};

const uint8_t line_table[LINE_ORIENTS][LINE_ROWS][LINE_COLS] = {
    {
        { TYPE_NONE, TYPE_NONE, TYPE_NONE, TYPE_NONE },
        { TYPE_NONE, TYPE_NONE, TYPE_NONE, TYPE_NONE },
        { TYPE_LINE, TYPE_LINE, TYPE_LINE, TYPE_LINE },
        { TYPE_NONE, TYPE_NONE, TYPE_NONE, TYPE_NONE },
    },
    {
        { TYPE_NONE, TYPE_NONE, TYPE_LINE, TYPE_NONE },
        { TYPE_NONE, TYPE_NONE, TYPE_LINE, TYPE_NONE },
        { TYPE_NONE, TYPE_NONE, TYPE_LINE, TYPE_NONE },
        { TYPE_NONE, TYPE_NONE, TYPE_LINE, TYPE_NONE },
    },
};

const uint8_t o_table[O_ORIENTS][O_ROWS][O_COLS] = {
    {
        { TYPE_NONE, TYPE_NONE, TYPE_NONE, TYPE_NONE },
        { TYPE_NONE, TYPE_O, TYPE_O, TYPE_NONE },
        { TYPE_NONE, TYPE_O, TYPE_O, TYPE_NONE },
        { TYPE_NONE, TYPE_NONE, TYPE_NONE, TYPE_NONE },
    },
};

const uint8_t j_table[J_ORIENTS][J_ROWS][J_COLS] = {
    {
        { TYPE_NONE, TYPE_NONE, TYPE_NONE },
        { TYPE_J, TYPE_J, TYPE_J },
        { TYPE_NONE, TYPE_NONE, TYPE_J },
    },
    {
        { TYPE_NONE, TYPE_J, TYPE_NONE },
        { TYPE_NONE, TYPE_J, TYPE_NONE },
        { TYPE_J, TYPE_J, TYPE_NONE },
    },
    {
        { TYPE_J, TYPE_NONE, TYPE_NONE },
        { TYPE_J, TYPE_J, TYPE_J },
        { TYPE_NONE, TYPE_NONE, TYPE_NONE },
    },
    {
        { TYPE_NONE, TYPE_J, TYPE_J },
        { TYPE_NONE, TYPE_J, TYPE_NONE },
        { TYPE_NONE, TYPE_J, TYPE_NONE },
    },
};

const uint8_t l_table[L_ORIENTS][L_ROWS][L_COLS] = {
    {
        { TYPE_NONE, TYPE_NONE, TYPE_NONE },
        { TYPE_L, TYPE_L, TYPE_L },
        { TYPE_L, TYPE_NONE, TYPE_NONE },
    },
    {
        { TYPE_L, TYPE_L, TYPE_NONE },
        { TYPE_NONE, TYPE_L, TYPE_NONE },
        { TYPE_NONE, TYPE_L, TYPE_NONE },
    },
    {
        { TYPE_NONE, TYPE_NONE, TYPE_L },
        { TYPE_L, TYPE_L, TYPE_L },
        { TYPE_NONE, TYPE_NONE, TYPE_NONE },
    },
    {
        { TYPE_NONE, TYPE_L, TYPE_NONE },
        { TYPE_NONE, TYPE_L, TYPE_NONE },
        { TYPE_NONE, TYPE_L, TYPE_L },
    },
};

const uint8_t s_table[S_ORIENTS][S_ROWS][S_COLS] = {
    {
        { TYPE_NONE, TYPE_NONE, TYPE_NONE },
        { TYPE_NONE, TYPE_S, TYPE_S },
        { TYPE_S, TYPE_S, TYPE_NONE },
    },
    {
        { TYPE_NONE, TYPE_S, TYPE_NONE },
        { TYPE_NONE, TYPE_S, TYPE_S },
        { TYPE_NONE, TYPE_NONE, TYPE_S },
    },
};

const uint8_t t_table[T_ORIENTS][T_ROWS][T_COLS] = {
    {
        { TYPE_NONE, TYPE_NONE, TYPE_NONE },
        { TYPE_T, TYPE_T, TYPE_T },
        { TYPE_NONE, TYPE_T, TYPE_NONE },
    },
    {
        { TYPE_NONE, TYPE_T, TYPE_NONE },
        { TYPE_T, TYPE_T, TYPE_NONE },
        { TYPE_NONE, TYPE_T, TYPE_NONE },
    },
    {
        { TYPE_NONE, TYPE_T, TYPE_NONE },
        { TYPE_T, TYPE_T, TYPE_T },
        { TYPE_NONE, TYPE_NONE, TYPE_NONE },
    },
    {
        { TYPE_NONE, TYPE_T, TYPE_NONE },
        { TYPE_NONE, TYPE_T, TYPE_T },
        { TYPE_NONE, TYPE_T, TYPE_NONE },
    },
};

const uint8_t z_table[Z_ORIENTS][Z_ROWS][Z_COLS] = {
    {
        { TYPE_NONE, TYPE_NONE, TYPE_NONE },
        { TYPE_Z, TYPE_Z, TYPE_NONE },
        { TYPE_NONE, TYPE_Z, TYPE_Z },
    },
    {
        { TYPE_NONE, TYPE_NONE, TYPE_Z },
        { TYPE_NONE, TYPE_Z, TYPE_Z },
        { TYPE_NONE, TYPE_Z, TYPE_NONE },
    },
};

piece_t* piece_new(const matrix_t* matrix, uint8_t type) {
    piece_t* piece = malloc(sizeof(piece_t));
    if (!piece) {
        return NULL;
    }
    switch (type) {
        case TYPE_LINE:
            piece->table = line_table;
            piece->orientations = LINE_ORIENTS;
            piece->rows = LINE_ROWS;
            piece->cols = LINE_COLS;
            break;
        case TYPE_O:
            piece->table = o_table;
            piece->orientations = O_ORIENTS;
            piece->rows = O_ROWS;
            piece->cols = O_COLS;
            break;
        case TYPE_J:
            piece->table = j_table;
            piece->orientations = J_ORIENTS;
            piece->rows = J_ROWS;
            piece->cols = J_COLS;
            break;
        case TYPE_L:
            piece->table = l_table;
            piece->orientations = L_ORIENTS;
            piece->rows = L_ROWS;
            piece->cols = L_COLS;
            break;
        case TYPE_S:
            piece->table = s_table;
            piece->orientations = S_ORIENTS;
            piece->rows = S_ROWS;
            piece->cols = S_COLS;
            break;
        case TYPE_T:
            piece->table = t_table;
            piece->orientations = T_ORIENTS;
            piece->rows = T_ROWS;
            piece->cols = T_COLS;
            break;
        case TYPE_Z:
            piece->table = z_table;
            piece->orientations = Z_ORIENTS;
            piece->rows = Z_ROWS;
            piece->cols = Z_COLS;
            break;
        default:
            return NULL;
    }
    piece->orient_index = 0;
    piece->x = matrix->cols / 2 - piece->cols / 2;
    piece->y = matrix->hidden_rows - 1;
    piece->type = type;
    return piece;
}

/*
 * Create a new piece. Because this function calls rand(), srand() should be called at least once
 * before calling this function.
 */
piece_t* piece_new_rand(const matrix_t* matrix) {
    return piece_new(matrix, rand() % NUM_PIECES + 1);
}

/* Return whether the piece either collides with the stack or is out of bounds of the matrix. */
bool piece_collides(const piece_t* piece, const matrix_t* matrix) {
    const uint8_t (*table)[piece->orientations][piece->rows][piece->cols] = (const uint8_t(*)[piece->orientations][piece->rows][piece->cols])piece->table;
    for (int32_t r = 0; r < (int64_t)piece->rows; ++r) {
        for (int32_t c = 0; c < (int64_t)piece->cols; ++c) {
            if ((*table)[piece->orient_index][r][c] != TYPE_NONE) {
                /* check collision with the playfield */
                if (matrix_out_bounds(matrix, piece->y + r, piece->x + c)) {
                    return true;
                } else {
                    /* check collision with the stack (if there is one) */
                    if (matrix->table[piece->y + r][piece->x + c] != TYPE_NONE) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/* Move the piece down by one unit. Return whether the piece was able to be moved. */
bool piece_move_down(piece_t* piece, const matrix_t* matrix) {
    ++piece->y;
    bool collides = piece_collides(piece, matrix);
    if (collides) {
        --piece->y;
    }
    return !collides;
}

/* Move the piece to the left by one unit. Return whether the piece was able to be moved. */
bool piece_move_left(piece_t* piece, const matrix_t* matrix) {
    --piece->x;
    bool collides = piece_collides(piece, matrix);
    if (collides) {
        ++piece->x;
    }
    return !collides;
}

/* Move the piece to the right by one unit. Return whether the piece was able to be moved. */
bool piece_move_right(piece_t* piece, const matrix_t* matrix) {
    ++piece->x;
    bool collides = piece_collides(piece, matrix);
    if (collides) {
        --piece->x;
    }
    return !collides;
}

/*
 * Change the orientation of the piece as if the piece was rotated counterclockwise. Return whether
 * the orientation of the piece was able to be changed.
 */
bool piece_rotate_ccw(piece_t* piece, const matrix_t* matrix) {
    if (piece->orient_index == 0) {
        piece->orient_index = piece->orientations;
    }
    --piece->orient_index;

    bool collides = piece_collides(piece, matrix);
    if (collides) {
        ++piece->orient_index;
        if (piece->orient_index >= piece->orientations) {
            piece->orient_index = 0;
        }
    }
    return !collides;
}

/*
 * Change the orientation of the piece as if the piece was rotated clockwise. Return whether the
 * orientation of the piece was able to be changed.
 */
bool piece_rotate_cw(piece_t* piece, const matrix_t* matrix) {
    ++piece->orient_index;
    if (piece->orient_index >= piece->orientations) {
        piece->orient_index = 0;
    }

    bool collides = piece_collides(piece, matrix);
    if (collides) {
        if (piece->orient_index == 0) {
            piece->orient_index = piece->orientations;
        }
        --piece->orient_index;
    }
    return !collides;
}

void piece_place(const piece_t* piece, matrix_t* matrix) {
    const uint8_t (*table)[piece->orientations][piece->rows][piece->cols] = (const uint8_t(*)[piece->orientations][piece->rows][piece->cols])piece->table;
    for (size_t r = 0; r < piece->rows; ++r) {
        for (size_t c = 0; c < piece->cols; ++c) {
            uint8_t type = (*table)[piece->orient_index][r][c];
            if (type != TYPE_NONE) {
                /* copy piece to matrix */
                if (!matrix_out_bounds(matrix, piece->y + r, piece->x + c)) {
                    matrix->table[piece->y + r][piece->x + c] = type;
                }
            }
        }
    }
}

void piece_free(piece_t* piece) {
    free(piece);
}

/*
 * Create a new matrix. The matrix should be at least four columns wide for the game to work
 * properly. Return NULL on failure.
 */
matrix_t* matrix_new(uint32_t rows, uint32_t cols, uint32_t hidden_rows) {
    if (rows < hidden_rows) {
        return NULL;
    }
    matrix_t* matrix = malloc(sizeof(matrix_t));
    if (!matrix) {
        return NULL;
    }
    matrix->table = malloc(rows * sizeof(uint8_t*));
    if (!matrix->table) {
        free(matrix);
        return NULL;
    }
    for (size_t r = 0; r < rows; ++r) {
        matrix->table[r] = malloc(cols * sizeof(uint8_t));
        if (!matrix->table[r]) {
            for (size_t r2 = 0; r2 < r; ++r2) {
                free(matrix->table[r2]);
            }
            free(matrix->table);
            free(matrix);
            return NULL;
        }
        for (size_t c = 0; c < cols; ++c) {
            matrix->table[r][c] = TYPE_NONE;
        }
    }
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->hidden_rows = hidden_rows;
    return matrix;
}

bool matrix_row_full(const matrix_t* matrix, const uint32_t row) {
    bool is_row_full = true;
    for (size_t c = 0; c < matrix->cols; ++c) {
        uint8_t type = matrix->table[row][c];
        if (type == TYPE_NONE) {
            is_row_full = false;
            break;
        }
    }
    return is_row_full;
}

bool matrix_out_bounds(const matrix_t* matrix, int32_t row, int32_t col) {
    return ((row < 0) || (col < 0) || (row + 1 > (int64_t)matrix->rows) || (col + 1 > (int64_t)matrix->cols));
}

/*
 * Copy the table from one matrix to another. Return whether the dimensions of the matrices
 * match.
 */
bool matrix_copy_table(matrix_t* dest, const matrix_t* src) {
    if (dest->rows != src->rows || dest->hidden_rows != src->hidden_rows || dest->cols != src->cols) {
        return false;
    }
    for (size_t r = 0; r < src->rows; ++r) {
        for (size_t c = 0; c < src->cols; ++c) {
            dest->table[r][c] = src->table[r][c];
        }
    }
    return true;
}

void matrix_clear(matrix_t* matrix) {
    for (size_t r = 0; r < matrix->rows; ++r) {
        memset(matrix->table[r], 0, matrix->cols);
    }
}

/* Shift the stack down by one row, starting from a given row. */
void matrix_shift_stack(matrix_t* matrix, uint32_t from_row) {
    for (size_t r = from_row; r > 0; --r) {
        for (size_t c = 0; c < matrix->cols; ++c) {
            matrix->table[r][c] = matrix->table[r - 1][c];
        }
    }
    for (size_t c = 0; c < matrix->cols; ++c) {
        matrix->table[0][c] = TYPE_NONE;
    }
}

/*
 * Clear filled rows and shift stack down. If there are no filled rows, then the matrix will not
 * be affected.
 */
void matrix_clean(matrix_t* matrix) {
    for (size_t r = 0; r < matrix->rows; ++r) {
        if (matrix_row_full(matrix, r)) {
            for (size_t c = 0; c < matrix->cols; ++c) {
                matrix->table[r][c] = TYPE_NONE;
            }
            matrix_shift_stack(matrix, r);
        }
    }
}

void matrix_free(matrix_t* matrix) {
    if (!matrix) {
        return;
    }
    for (size_t r = 0; r < matrix->rows; ++r) {
        if (matrix->table[r]) {
            free(matrix->table[r]);
        }
    }
    if (matrix->table) {
        free(matrix->table);
    }
    free(matrix);
}
