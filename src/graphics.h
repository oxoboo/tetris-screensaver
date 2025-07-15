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

#if !defined(GRAPHICS_H)

#include "SDL_render.h"
#include "matrix.h"

#define FLASH_GRAY   0x3F
#define FLASH_STATES 10
#define REND_GRAY    0x17

typedef struct {
    SDL_Texture* texture;
    uint8_t* pixels;
    uint32_t size;
    uint32_t pallete_value;
} graphics_t;

uint32_t rand_pallete_value();

graphics_t* graphics_new(SDL_Renderer* renderer, const matrix_t* matrix);
uint32_t    graphics_texture_width(const matrix_t* matrix);
uint32_t    graphics_texture_height(const matrix_t* matrix);
uint32_t    graphics_texture_size(const matrix_t* matrix);
void        graphics_clear_gray(graphics_t* graphics, const matrix_t* matrix);
void        graphics_clear(graphics_t* graphics, const matrix_t* matrix);
void        graphics_matrix(graphics_t* graphics, const matrix_t* matrix);
void        graphics_piece(graphics_t* graphics, const piece_t* piece, const matrix_t* matrix);
void        graphics_anim_clear(graphics_t* graphics, const matrix_t* matrix, uint64_t time, uint64_t duration);
void        graphics_curtain1(graphics_t* graphics, const matrix_t* matrix, uint64_t time, uint64_t duration);
void        graphics_curtain2(graphics_t* graphics, const matrix_t* matrix);
void        graphics_curtain3(graphics_t* graphics, const matrix_t* matrix, uint64_t time, uint64_t duration);
void        graphics_render(SDL_Renderer* renderer, graphics_t* graphics);
void        graphics_free(graphics_t* graphics);

#endif /* GRAPHICS_H */
