/*
 * Copyright (c) 2024 Oxoboo
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

#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_PIECES 7

#define MATRIX_ROWS        22
#define MATRIX_COLS        10
#define MATRIX_HIDDEN_ROWS 2

enum {
    TYPE_NONE,
    TYPE_LINE,
    TYPE_O,
    TYPE_J,
    TYPE_L,
    TYPE_S,
    TYPE_T,
    TYPE_Z,
};

typedef struct {
    uint8_t*** table; /* table[orientation][row][col] */
    uint32_t orientations;
    uint32_t rows;
    uint32_t cols;
    uint32_t orient_index;
    int32_t x;
    int32_t y;
    uint8_t type;
} piece_t;

typedef struct {
    uint8_t** table; /* table[row][col] */
    uint32_t rows;
    uint32_t hidden_rows;
    uint32_t cols;
} matrix_t;

piece_t* piece_new(const matrix_t* matrix, uint8_t type);
piece_t* piece_new_rand(const matrix_t* matrix);
bool     piece_collides(const piece_t* piece, const matrix_t* matrix);
bool     piece_move_down(piece_t* piece, const matrix_t* matrix);
bool     piece_move_left(piece_t* piece, const matrix_t* matrix);
bool     piece_move_right(piece_t* piece, const matrix_t* matrix);
bool     piece_rotate_ccw(piece_t* piece, const matrix_t* matrix);
bool     piece_rotate_cw(piece_t* piece, const matrix_t* matrix);
void     piece_place(const piece_t* piece, matrix_t* matrix);
void     piece_free(piece_t*);

matrix_t* matrix_new(uint32_t rows, uint32_t cols, uint32_t hidden_rows);
bool      matrix_row_full(const matrix_t* matrix, uint32_t row);
bool      matrix_out_bounds(const matrix_t* matrix, int32_t row, int32_t col);
bool      matrix_copy_table(matrix_t* dest, const matrix_t* src);
void      matrix_clear(matrix_t* matrix);
void      matrix_shift_stack(matrix_t* matrix, uint32_t from_row);
void      matrix_clean(matrix_t* matrix);
void      matrix_free(matrix_t* matrix);

#endif /* GAME_H */