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
#include <string.h>
#include "graphics.h"

#define BLOCK_WIDTH  8
#define BLOCK_HEIGHT 8
#define NUM_PALLETES 10

#define PIXEL_FORMAT    SDL_PIXELFORMAT_RGBA8888
#define DEFAULT_ALPHA   0xBF
#define BG_GRAY         0x00
#define BYTES_PER_PIXEL 4

enum color_index {
    INDEX_RED,
    INDEX_GREEN,
    INDEX_BLUE,
};

enum color_value {
    COLOR_B,
    COLOR_1,
    COLOR_2,
    COLOR_W,
};

typedef struct {
    uint8_t color1[3];
    uint8_t color2[3];
} pallete_t;

const pallete_t pallete[NUM_PALLETES] = {
    { .color1 = { 0x00, 0x58, 0xF8 }, .color2 = { 0x3C, 0xBC, 0xFC } },
    { .color1 = { 0x00, 0xA8, 0x00 }, .color2 = { 0xB8, 0xF8, 0x18 } },
    { .color1 = { 0xD8, 0x00, 0xCC }, .color2 = { 0xF8, 0x78, 0xF8 } },
    { .color1 = { 0x00, 0x58, 0xF8 }, .color2 = { 0x58, 0xD8, 0x54 } },
    { .color1 = { 0xE4, 0x00, 0x58 }, .color2 = { 0x58, 0xF8, 0x98 } },
    { .color1 = { 0x58, 0xF8, 0x98 }, .color2 = { 0x68, 0x88, 0xFC } },
    { .color1 = { 0xF8, 0x38, 0x00 }, .color2 = { 0x7C, 0x7C, 0x7C } },
    { .color1 = { 0x68, 0x44, 0xFC }, .color2 = { 0xA8, 0x00, 0x20 } },
    { .color1 = { 0x00, 0x58, 0xF8 }, .color2 = { 0xF8, 0x38, 0x00 } },
    { .color1 = { 0xF8, 0x38, 0x00 }, .color2 = { 0xFC, 0xA0, 0x44 } },
};

const uint8_t pixels_block1[BLOCK_HEIGHT][BLOCK_WIDTH] = {
    { COLOR_W, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_B },
    { COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B },
};

const uint8_t pixels_block2[BLOCK_HEIGHT][BLOCK_WIDTH] = {
    { COLOR_W, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_B },
    { COLOR_2, COLOR_W, COLOR_W, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_B },
    { COLOR_2, COLOR_W, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_B },
    { COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_B },
    { COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_B },
    { COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_B },
    { COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_B },
    { COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B },
};

const uint8_t pixels_block3[BLOCK_HEIGHT][BLOCK_WIDTH] = {
    { COLOR_W, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_W, COLOR_W, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_W, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_B },
    { COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_B },
    { COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B },
};

const uint8_t colors_curtain[BLOCK_HEIGHT][BLOCK_WIDTH] = {
    { COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2 },
    { COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2, COLOR_2 },
    { COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W },
    { COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W },
    { COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W, COLOR_W },
    { COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1 },
    { COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1, COLOR_1 },
    { COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B, COLOR_B },
};

/*
 * Convert a tetromino's values to the values of its corresponding NES Tetris counterpart. Return
 * a 0, 1, or 2. If `type` is an invalid value, return a 0 anyway.
 */
uint8_t type_to_nes_type(uint8_t type) {
    switch (type) {
        case TYPE_LINE:
            return 0;
        case TYPE_O:
            return 0;
        case TYPE_J:
            return 1;
        case TYPE_L:
            return 2;
        case TYPE_S:
            return 1;
        case TYPE_T:
            return 0;
        case TYPE_Z:
            return 2;
        default:
            return 0;
    }
}

/* Give a random pallete value. srand() should be called prior to calling this function. */
uint32_t rand_pallete_value() {
    return rand() % NUM_PALLETES;
}

/* Create a new graphics struct. Uses RGBA pixel format. Return NULL on failure. */
graphics_t* graphics_new(SDL_Renderer* renderer, const matrix_t* matrix) {
    graphics_t* graphics = malloc(sizeof(graphics_t));
    if (!graphics) {
        return NULL;
    }
    graphics->texture = SDL_CreateTexture(
        renderer,
        PIXEL_FORMAT,
        SDL_TEXTUREACCESS_STREAMING,
        graphics_texture_width(matrix),
        graphics_texture_height(matrix)
    );
    SDL_SetTextureBlendMode(graphics->texture, SDL_BLENDMODE_BLEND);
    graphics->size = sizeof(uint8_t) * graphics_texture_size(matrix) * BYTES_PER_PIXEL;
    graphics->pixels = malloc(graphics->size);
    if (!graphics->pixels) {
        free(graphics);
        return NULL;
    }
    graphics->pallete_value = 0;
    return graphics;
}

uint32_t graphics_texture_width(const matrix_t* matrix) {
    return matrix->cols * BLOCK_WIDTH;
}

uint32_t graphics_texture_height(const matrix_t* matrix) {
    return ((matrix->rows - matrix->hidden_rows) * BLOCK_HEIGHT);
}

uint32_t graphics_texture_size(const matrix_t* matrix) {
    return (graphics_texture_width(matrix) * graphics_texture_height(matrix));
}

void graphics_cell(graphics_t* graphics, const matrix_t* matrix,
                   uint8_t type, uint32_t row, uint32_t col) {
    uint32_t p_index = graphics->pallete_value % NUM_PALLETES;
    const uint8_t (*pixels)[BLOCK_HEIGHT][BLOCK_WIDTH];
    switch (type_to_nes_type(type)) {
        case 0:
            pixels = &pixels_block1;
            break;
        case 1:
            pixels = &pixels_block2;
            break;
        case 2:
            pixels = &pixels_block3;
            break;
        default:
            pixels = &pixels_block1;
            break;
    }

    uint32_t pixels_per_row = matrix->cols * BLOCK_WIDTH * BLOCK_HEIGHT;
    for (size_t y = 0; y < BLOCK_HEIGHT; ++y) {
        for (size_t x = 0; x < BLOCK_WIDTH; ++x) {
            uint32_t index = ((pixels_per_row * (row - matrix->hidden_rows))
                + (col * BLOCK_WIDTH)
                + (matrix->cols * BLOCK_WIDTH * y) + x)
                * BYTES_PER_PIXEL;
            switch ((*pixels)[y][x]) {
                case COLOR_1:
                    graphics->pixels[index] = DEFAULT_ALPHA;
                    graphics->pixels[index + 1] = pallete[p_index].color1[INDEX_BLUE];
                    graphics->pixels[index + 2] = pallete[p_index].color1[INDEX_GREEN];
                    graphics->pixels[index + 3] = pallete[p_index].color1[INDEX_RED];
                    break;
                case COLOR_2:
                    graphics->pixels[index] = DEFAULT_ALPHA;
                    graphics->pixels[index + 1] = pallete[p_index].color2[INDEX_BLUE];
                    graphics->pixels[index + 2] = pallete[p_index].color2[INDEX_GREEN];
                    graphics->pixels[index + 3] = pallete[p_index].color2[INDEX_RED];
                    break;
                case COLOR_W:
                    graphics->pixels[index] = DEFAULT_ALPHA;
                    graphics->pixels[index + 1] = 0xFF;
                    graphics->pixels[index + 2] = 0xFF;
                    graphics->pixels[index + 3] = 0xFF;
                    break;
                case COLOR_B:
                    graphics->pixels[index] = DEFAULT_ALPHA;
                    graphics->pixels[index + 1] = 0x00;
                    graphics->pixels[index + 2] = 0x00;
                    graphics->pixels[index + 3] = 0x00;
                    break;
                default:
                    break;
            }
        }
    }
}

void graphics_curtain(graphics_t* graphics, const matrix_t* matrix, uint32_t row) {
    uint32_t p_index = graphics->pallete_value % NUM_PALLETES;
    uint32_t pixels_per_row = matrix->cols * BLOCK_WIDTH * BLOCK_HEIGHT;
    for (size_t col = 0; col < matrix->cols; ++col) {
        for (size_t y = 0; y < BLOCK_HEIGHT; ++y) {
            for (size_t x = 0; x < BLOCK_WIDTH; ++x) {
                uint32_t index = ((pixels_per_row * (row - matrix->hidden_rows))
                    + (col * BLOCK_WIDTH)
                    + (matrix->cols * BLOCK_WIDTH * y) + x)
                    * BYTES_PER_PIXEL;
                switch (colors_curtain[y][x]) {
                    case COLOR_1:
                        graphics->pixels[index] = DEFAULT_ALPHA;
                        graphics->pixels[index + 1] = pallete[p_index].color1[INDEX_BLUE];
                        graphics->pixels[index + 2] = pallete[p_index].color1[INDEX_GREEN];
                        graphics->pixels[index + 3] = pallete[p_index].color1[INDEX_RED];
                        break;
                    case COLOR_2:
                        graphics->pixels[index] = DEFAULT_ALPHA;
                        graphics->pixels[index + 1] = pallete[p_index].color2[INDEX_BLUE];
                        graphics->pixels[index + 2] = pallete[p_index].color2[INDEX_GREEN];
                        graphics->pixels[index + 3] = pallete[p_index].color2[INDEX_RED];
                        break;
                    case COLOR_W:
                        graphics->pixels[index] = DEFAULT_ALPHA;
                        graphics->pixels[index + 1] = 0xFF;
                        graphics->pixels[index + 2] = 0xFF;
                        graphics->pixels[index + 3] = 0xFF;
                        break;
                    case COLOR_B:
                        graphics->pixels[index] = DEFAULT_ALPHA;
                        graphics->pixels[index + 1] = 0x00;
                        graphics->pixels[index + 2] = 0x00;
                        graphics->pixels[index + 3] = 0x00;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

/* Fill pixel data with a gray (or black) color. */
void graphics_clear_gray(graphics_t* graphics, const matrix_t* matrix) {
    uint32_t size = graphics_texture_size(matrix);
    for (size_t i = 0; i < size; ++i) {
        graphics->pixels[i * BYTES_PER_PIXEL] = DEFAULT_ALPHA;
        graphics->pixels[i * BYTES_PER_PIXEL + 1] = BG_GRAY;
        graphics->pixels[i * BYTES_PER_PIXEL + 2] = BG_GRAY;
        graphics->pixels[i * BYTES_PER_PIXEL + 3] = BG_GRAY;
    }
}

/* Fill pixel data with a fully transparent color. */
void graphics_clear(graphics_t* graphics, const matrix_t* matrix) {
    uint32_t size = graphics_texture_size(matrix);
    for (size_t i = 0; i < size; ++i) {
        graphics->pixels[i * BYTES_PER_PIXEL] = 0x00;
        graphics->pixels[i * BYTES_PER_PIXEL + 1] = 0x00;
        graphics->pixels[i * BYTES_PER_PIXEL + 2] = 0x00;
        graphics->pixels[i * BYTES_PER_PIXEL + 3] = 0x00;
    }
}

void graphics_matrix(graphics_t* graphics, const matrix_t* matrix) {
    for (size_t r = matrix->hidden_rows; r < matrix->rows; ++r) {
        for (size_t c = 0; c < matrix->cols; ++c) {
            uint8_t type = matrix->table[r][c];
            if (type == TYPE_NONE) {
                continue;
            }
            graphics_cell(graphics, matrix, type, r, c);
        }
    }
}

void graphics_piece(graphics_t* graphics, const piece_t* piece, const matrix_t* matrix) {
    const uint8_t (*table)[piece->orientations][piece->rows][piece->cols] = (const uint8_t(*)[piece->orientations][piece->rows][piece->cols])piece->table;
    for (size_t r = 0; r < piece->rows; ++r) {
        for (size_t c = 0; c < piece->cols; ++c) {
            uint8_t type = (*table)[piece->orient_index][r][c];
            if (type == TYPE_NONE || piece->y + r < matrix->hidden_rows) {
                continue;
            }
            graphics_cell(graphics, matrix, type, piece->y + r, piece->x + c);
        }
    }
}

/* Matrix clearing lines animation. */
void graphics_anim_clear(graphics_t* graphics, const matrix_t* matrix,
                         uint64_t time, uint64_t duration) {
    for (size_t r = matrix->hidden_rows; r < matrix->rows; ++r) {
        if (matrix_row_full(matrix, r)) {
            uint32_t each_side = (matrix->cols / 2)
                - ((double)time / duration)
                * (matrix->cols / 2);
            for (size_t c = 0; c < each_side; ++c) {
                uint8_t type = matrix->table[r][c];
                if (type == TYPE_NONE) {
                    continue;
                }
                graphics_cell(graphics, matrix, type, r, c);
            }
            for (size_t c = matrix->cols - 1; c > matrix->cols - each_side - 1; --c) {
                uint8_t type = matrix->table[r][c];
                if (type == TYPE_NONE) {
                    continue;
                }
                graphics_cell(graphics, matrix, type, r, c);
            }
        } else {
            for (size_t c = 0; c < matrix->cols; ++c) {
                uint8_t type = matrix->table[r][c];
                if (type == TYPE_NONE) {
                    continue;
                }
                graphics_cell(graphics, matrix, type, r, c);
            }
        }
    }
}

/* "Falling" curtain animation. */
void graphics_curtain1(graphics_t* graphics, const matrix_t* matrix,
                       uint64_t time, uint64_t duration) {
    uint32_t rows = ((double)time / duration) * (matrix->rows - matrix->hidden_rows);
    for (size_t r = matrix->hidden_rows; r < matrix->hidden_rows + rows; ++r) {
        graphics_curtain(graphics, matrix, r);
    }
}

/* Curtain-in-place animation. */
void graphics_curtain2(graphics_t* graphics, const matrix_t* matrix) {
    for (size_t r = matrix->hidden_rows; r < matrix->rows; ++r) {
        graphics_curtain(graphics, matrix, r);
    }
}

/* "Rising" curtain animation. */
void graphics_curtain3(graphics_t* graphics, const matrix_t* matrix,
                       uint64_t time, uint64_t duration) {
    uint32_t rows = (matrix->rows - matrix->hidden_rows)
        - ((double)time / duration) * (matrix->rows - matrix->hidden_rows);
    for (size_t r = matrix->hidden_rows; r < rows + matrix->hidden_rows; ++r) {
        graphics_curtain(graphics, matrix, r);
    }
}

/* Scale the game to fill as much of the screen as possible without distortion. */
void graphics_render(SDL_Renderer* renderer, graphics_t* graphics) {
    int32_t screen_width;
    int32_t screen_height;
    SDL_GetRendererOutputSize(renderer, &screen_width, &screen_height);
    int32_t texture_width;
    int32_t texture_height;
    SDL_QueryTexture(graphics->texture, NULL, NULL, &texture_width, &texture_height);

    uint32_t scale;
    if ((double)screen_width / texture_width * texture_height >= screen_height) {
        scale = screen_height / texture_height;
    } else {
        scale = screen_width / texture_width;
    }

    uint32_t render_width = texture_width * scale;
    uint32_t render_height = texture_height * scale;
    SDL_Rect rect;
    rect.x = screen_width / 2 - render_width / 2;
    rect.y = screen_height / 2 - render_height / 2;
    rect.w = render_width;
    rect.h = render_height;

    void* pixels = NULL;
    int32_t pitch;
    SDL_LockTexture(graphics->texture, NULL, &pixels, &pitch);
    memcpy(pixels, graphics->pixels, pitch * texture_height);
    SDL_UnlockTexture(graphics->texture);
    SDL_RenderCopy(renderer, graphics->texture, NULL, &rect);
    SDL_RenderPresent(renderer);
}

/* Proportionally stretch the game view to fill as much of the screen as possible. */
void graphics_render_stretch(SDL_Renderer* renderer, graphics_t* graphics) {
    int32_t screen_width;
    int32_t screen_height;
    SDL_GetRendererOutputSize(renderer, &screen_width, &screen_height);
    int32_t texture_width;
    int32_t texture_height;
    SDL_QueryTexture(graphics->texture, NULL, NULL, &texture_width, &texture_height);

    SDL_Rect rect;
    if ((double)screen_width / texture_width * texture_height >= screen_height) {
        uint32_t render_width = (double)screen_height * texture_width / texture_height;
        rect.x = screen_width / 2 - render_width / 2;
        rect.y = 0;
        rect.w = render_width;
        rect.h = screen_height;
    } else {
        uint32_t render_height = (double)screen_width * texture_height / texture_width;
        rect.x = 0;
        rect.y = screen_height / 2 - render_height / 2;
        rect.w = screen_width;
        rect.h = render_height;
    }

    void* pixels = NULL;
    int32_t pitch;
    SDL_LockTexture(graphics->texture, NULL, &pixels, &pitch);
    memcpy(pixels, graphics->pixels, pitch * texture_height);
    SDL_UnlockTexture(graphics->texture);
    SDL_RenderCopy(renderer, graphics->texture, NULL, &rect);
    SDL_RenderPresent(renderer);
}

void graphics_free(graphics_t* graphics) {
    if (!graphics) {
        return;
    }
    if (graphics->texture) {
        SDL_DestroyTexture(graphics->texture);
    }
    if (graphics->pixels) {
        free(graphics->pixels);
    }
    free(graphics);
}
