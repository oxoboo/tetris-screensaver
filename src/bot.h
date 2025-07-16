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

#if !defined(BOT_H)

#include <stdbool.h>
#include "matrix.h"

typedef struct {
    bool left;
    bool right;
    bool down;
    bool ccw;
    bool cw;
} inputs_t;

void inputs_clear(inputs_t* inputs);

typedef struct {
    uint32_t holes;
    uint32_t line_deps_cells;
    uint32_t stack_height;
    uint32_t dest_orient_index;
    int32_t dest_x;
} bot_t;

bot_t    bot_new(void);
piece_t* bot_next_piece(bot_t* bot, const matrix_t* matrix, int32_t* err_value);
int32_t  bot_find_place(bot_t* bot, const matrix_t* matrix, uint8_t piece_type);
void     bot_update_inputs(bot_t* bot, inputs_t* inputs, const piece_t* piece);

#endif /* BOT_H */
