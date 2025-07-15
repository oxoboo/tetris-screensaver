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

piece_t* piece_new(const matrix_t* matrix, uint8_t type) {
    uint32_t orientations;
    uint32_t rows;
    uint32_t cols;
    switch (type) {
        case TYPE_LINE:
            rows = 4;
            cols = 4;
            orientations = 2;
            break;
        case TYPE_O:
            rows = 4;
            cols = 4;
            orientations = 1;
            break;
        case TYPE_J:
            rows = 3;
            cols = 3;
            orientations = 4;
            break;
        case TYPE_L:
            rows = 3;
            cols = 3;
            orientations = 4;
            break;
        case TYPE_S:
            rows = 3;
            cols = 3;
            orientations = 2;
            break;
        case TYPE_T:
            rows = 3;
            cols = 3;
            orientations = 4;
            break;
        case TYPE_Z:
            rows = 3;
            cols = 3;
            orientations = 2;
            break;
        default:
            return NULL;
    }

    uint8_t*** table = malloc(orientations * sizeof(uint8_t**));
    if (!table) {
        return NULL;
    }
    for (size_t i = 0; i < orientations; ++i) {
        table[i] = malloc(rows * sizeof(uint8_t*));
        if (!table[i]) {
            for (size_t i2 = 0; i2 < i; ++i2) {
                free(table[i2]);
            }
            free(table);
            return NULL;
        }
        for (size_t r = 0; r < rows; ++r) {
            table[i][r] = malloc(cols * sizeof(uint8_t));
            if (!table[i][r]) {
                for (size_t r2 = 0; r2 < r; ++r2) {
                    free(table[i][r2]);
                }
                free(table[i]);
                for (size_t i2 = 0; i2 < i; ++i2) {
                    for (size_t r2 = 0; r2 < rows; ++r2) {
                        free(table[i2][r2]);
                    }
                    free(table[i2]);
                }
                free(table);
                return NULL;
            }
            for (size_t c = 0; c < cols; ++c) {
                table[i][r][c] = TYPE_NONE;
            }
        }
    }

    switch (type) {
        case TYPE_LINE:
            /* orientation 1 */
            memset(table[0][2], TYPE_LINE, 4);
            /* orientation 2 */
            table[1][0][2] = TYPE_LINE;
            table[1][1][2] = TYPE_LINE;
            table[1][2][2] = TYPE_LINE;
            table[1][3][2] = TYPE_LINE;
            break;
        case TYPE_O:
            /* orientation 1 */
            table[0][1][1] = TYPE_O;
            table[0][1][2] = TYPE_O;
            table[0][2][1] = TYPE_O;
            table[0][2][2] = TYPE_O;
            break;
        case TYPE_J:
            /* orientation 1 */
            table[0][1][0] = TYPE_J;
            table[0][1][1] = TYPE_J;
            table[0][1][2] = TYPE_J;
            table[0][2][2] = TYPE_J;
            /* orientation 2 */
            table[1][0][1] = TYPE_J;
            table[1][1][1] = TYPE_J;
            table[1][2][1] = TYPE_J;
            table[1][2][0] = TYPE_J;
            /* orientation 3 */
            table[2][0][0] = TYPE_J;
            table[2][1][0] = TYPE_J;
            table[2][1][1] = TYPE_J;
            table[2][1][2] = TYPE_J;
            /* orientation 4 */
            table[3][0][1] = TYPE_J;
            table[3][0][2] = TYPE_J;
            table[3][1][1] = TYPE_J;
            table[3][2][1] = TYPE_J;
            break;
        case TYPE_L:
            /* orientation 1 */
            table[0][1][0] = TYPE_L;
            table[0][1][1] = TYPE_L;
            table[0][1][2] = TYPE_L;
            table[0][2][0] = TYPE_L;
            /* orientation 2 */
            table[1][0][0] = TYPE_L;
            table[1][0][1] = TYPE_L;
            table[1][1][1] = TYPE_L;
            table[1][2][1] = TYPE_L;
            /* orientation 3 */
            table[2][0][2] = TYPE_L;
            table[2][1][0] = TYPE_L;
            table[2][1][1] = TYPE_L;
            table[2][1][2] = TYPE_L;
            /* orientation 4 */
            table[3][0][1] = TYPE_L;
            table[3][1][1] = TYPE_L;
            table[3][2][1] = TYPE_L;
            table[3][2][2] = TYPE_L;
            break;
        case TYPE_S:
            /* orientation 1 */
            table[0][1][1] = TYPE_S;
            table[0][1][2] = TYPE_S;
            table[0][2][0] = TYPE_S;
            table[0][2][1] = TYPE_S;
            /* orientation 2 */
            table[1][0][1] = TYPE_S;
            table[1][1][1] = TYPE_S;
            table[1][1][2] = TYPE_S;
            table[1][2][2] = TYPE_S;
            break;
        case TYPE_T:
            /* orientation 1 */
            table[0][1][0] = TYPE_T;
            table[0][1][1] = TYPE_T;
            table[0][1][2] = TYPE_T;
            table[0][2][1] = TYPE_T;
            /* orientation 2 */
            table[1][0][1] = TYPE_T;
            table[1][1][0] = TYPE_T;
            table[1][1][1] = TYPE_T;
            table[1][2][1] = TYPE_T;
            /* orientation 3 */
            table[2][0][1] = TYPE_T;
            table[2][1][0] = TYPE_T;
            table[2][1][1] = TYPE_T;
            table[2][1][2] = TYPE_T;
            /* orientation 4 */
            table[3][0][1] = TYPE_T;
            table[3][1][1] = TYPE_T;
            table[3][1][2] = TYPE_T;
            table[3][2][1] = TYPE_T;
            break;
        case TYPE_Z:
            /* orientation 1 */
            table[0][1][0] = TYPE_Z;
            table[0][1][1] = TYPE_Z;
            table[0][2][1] = TYPE_Z;
            table[0][2][2] = TYPE_Z;
            /* orientation 2 */
            table[1][0][2] = TYPE_Z;
            table[1][1][1] = TYPE_Z;
            table[1][1][2] = TYPE_Z;
            table[1][2][1] = TYPE_Z;
            break;
        default:
            return NULL;

    }

    piece_t* piece = malloc(sizeof(piece_t));
    if (!piece) {
        return NULL;
    }
    piece->table = table;
    piece->orientations = orientations;
    piece->rows = rows;
    piece->cols = cols;
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
    for (int32_t r = 0; r < (int64_t)piece->rows; ++r) {
        for (int32_t c = 0; c < (int64_t)piece->cols; ++c) {
            if (piece->table[piece->orient_index][r][c] != TYPE_NONE) {
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
    for (size_t r = 0; r < piece->rows; ++r) {
        for (size_t c = 0; c < piece->cols; ++c) {
            uint8_t type = piece->table[piece->orient_index][r][c];
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
    if (!piece) {
        return;
    }
    for (size_t i = 0; i < piece->orientations; ++i) {
        for (size_t r = 0; r < piece->rows; ++r) {
            if (piece->table[i][r]) {
                free(piece->table[i][r]);
            }
        }
        if (piece->table[i]) {
            free(piece->table[i]);
        }
    }
    if (piece->table) {
        free(piece->table);
    }
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
    return (
        (row < 0)
        || (col < 0)
        || (row + 1 > (int64_t)matrix->rows)
        || (col + 1 > (int64_t)matrix->cols)
    );
}

/*
 * Copy the table from one matrix to another. Return whether the dimensions of the matrices
 * match.
 */
bool matrix_copy_table(matrix_t* dest, const matrix_t* src) {
    if (dest->rows != src->rows
        || dest->hidden_rows != src->hidden_rows
        || dest->cols != src->cols) {
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
