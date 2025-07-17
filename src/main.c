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

#include <stdio.h>
#include <time.h>
#include "SDL.h"
#include "matrix.h"
#include "graphics.h"
#include "bot.h"
#include "errorvalues.h"

enum {
    SCREEN_DEFAULT_WIDTH = 1280,
    SCREEN_DEFAULT_HEIGHT = 720,
};

enum {
    LINES_CLEARED_TETRIS = 4,
    LINES_PER_PALLETE = 10,
};

/* values are in milliseconds */
enum {
    TIME_DAS = 267,
    TIME_ARR = 100,
    TIME_ARE = 167,
    TIME_DROP = 32,
    TIME_CLEAR = 300,
    TIME_RESET0 = 1500, /* Time before falling curtain animation. */
    TIME_RESET1 = 1500, /* Time during falling curtain animation. */
    TIME_RESET2 = 500, /* Time while curtian is down. */
    TIME_RESET3 = 1500, /* Time during rising curtain animation. */
};

enum {
    BOT_DELAY_AFTER_SPAWN = 600,
    BOT_DELAY_AFTER_MOVEMENT = 400,
    BOT_DELAY_AFTER_ROTATION = 200,
};

/*
 * Initiate the SDL library, set up the game, and call srand. Return 0 on success or a non-zero
 * value on error.
 */
int32_t init(SDL_Window** window, uint32_t win_flags,
             SDL_Renderer** renderer, graphics_t** graphics, matrix_t** matrix) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return ERROR_SDL_INIT;
    }
    *window = SDL_CreateWindow(
        "NES Tetris Screensaver",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_DEFAULT_WIDTH,
        SCREEN_DEFAULT_HEIGHT,
        win_flags
    );
    if (!window) {
        return ERROR_SDL_WINDOW;
    }
    *renderer = SDL_CreateRenderer(
        *window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!(*renderer)) {
        return ERROR_SDL_RENDERER;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
        return ERROR_SDL_HINT;
    }
    if (SDL_SetRenderDrawColor(*renderer, REND_GRAY, REND_GRAY, REND_GRAY, 0xFF) < 0) {
        return ERROR_SDL_SET_RENDER_DRAW;
    };
    *matrix = matrix_new(MATRIX_ROWS, MATRIX_COLS, MATRIX_HIDDEN_ROWS);
    if (!(*matrix)) {
        return ERROR_MATRIX;
    }
    *graphics = graphics_new(*renderer, *matrix);
    if (!(*graphics)) {
        return ERROR_GRAPHICS;
    }
    srand(time(NULL));
    SDL_ShowCursor(SDL_DISABLE);
    return 0;
}

/*
 * Return whether any user input has been made. `ignore_mouse_motion` may be set to `true` to
 * prevent the program from immediately closing after it is opened.
 */
bool event_quit(uint32_t type, bool ignore_mouse_motion, bool debug_mode) {
    bool quit = false;
    if (debug_mode) {
        quit = type == SDL_QUIT;
    } else {
        quit = (
            type == SDL_QUIT
            || type == SDL_KEYDOWN
            || (type == SDL_MOUSEMOTION && !ignore_mouse_motion)
            || type == SDL_MOUSEBUTTONDOWN
        );
    }
    return quit;
}

void are_loop(SDL_Renderer* renderer, graphics_t* graphics, const matrix_t* matrix,
              piece_t** piece, SDL_Event* event, bool* quit, bool debug_mode) {
    uint64_t time_end = SDL_GetTicks64() + TIME_ARE;
    while (!(*quit) && time_end > SDL_GetTicks64()) {
        while (SDL_PollEvent(event) != 0) {
            *quit = *quit || event_quit(event->type, 0, debug_mode);
        }
        SDL_RenderClear(renderer);
        graphics_clear_gray(graphics, matrix);
        graphics_matrix(graphics, matrix);
        graphics_piece(graphics, *piece, matrix);
        graphics_render(renderer, graphics);
    }
}

/* This animation loop is meant to be played when 1-3 lines are cleared. */
void clear_loop1(SDL_Renderer* renderer, graphics_t* graphics,
                 const matrix_t* matrix, SDL_Event* event, bool* quit, bool debug_mode) {
    uint64_t time_start = SDL_GetTicks64();
    uint64_t time_end = SDL_GetTicks64() + TIME_CLEAR;
    while (!(*quit) && time_end > SDL_GetTicks64()) {
        while (SDL_PollEvent(event) != 0) {
            *quit = *quit || event_quit(event->type, 0, debug_mode);
        }
        uint64_t time_from_start = SDL_GetTicks64() - time_start;
        SDL_RenderClear(renderer);
        graphics_clear_gray(graphics, matrix);
        graphics_anim_clear(graphics, matrix, time_from_start, TIME_CLEAR);
        graphics_render(renderer, graphics);
    }
}

/* This animation loop is meant to be played when 4 lines are cleared. */
void clear_loop2(SDL_Renderer* renderer, graphics_t* graphics,
                 const matrix_t* matrix, SDL_Event* event, bool* quit, bool debug_mode) {
    uint64_t time_start = SDL_GetTicks64();
    uint64_t time_end = SDL_GetTicks64() + TIME_CLEAR;
    uint32_t interval = TIME_CLEAR / FLASH_STATES;
    uint64_t time_flash_states[FLASH_STATES];
    for (size_t i = 0; i < FLASH_STATES; ++i) {
        time_flash_states[i] = interval * i;
    }

    uint32_t index_flash = 0;
    bool state_flash = false;
    while (!(*quit) && time_end > SDL_GetTicks64()) {
        while (SDL_PollEvent(event) != 0) {
            *quit = *quit || event_quit(event->type, 0, debug_mode);
        }
        uint64_t time_from_start = SDL_GetTicks64() - time_start;
        if (index_flash < FLASH_STATES && time_from_start > time_flash_states[index_flash]) {
            state_flash = !state_flash;
            if (state_flash) {
                SDL_SetRenderDrawColor(renderer, FLASH_GRAY, FLASH_GRAY, FLASH_GRAY, 0xFF);
            } else {
                SDL_SetRenderDrawColor(renderer, REND_GRAY, REND_GRAY, REND_GRAY, 0xFF);
            }
            ++index_flash;
        }

        SDL_RenderClear(renderer);
        graphics_clear(graphics, matrix);
        graphics_anim_clear(graphics, matrix, time_from_start, TIME_CLEAR);
        graphics_render(renderer, graphics);
    }
    SDL_SetRenderDrawColor(renderer, REND_GRAY, REND_GRAY, REND_GRAY, 0xFF);
}

/* Play the falling curtain animation. */
void reset_loop1(SDL_Renderer* renderer, graphics_t* graphics, const matrix_t* matrix,
                 piece_t** piece, SDL_Event* event, bool* quit, bool debug_mode) {
    uint64_t time_start = SDL_GetTicks64();
    uint64_t time_end = time_start + TIME_RESET0;
    while (!(*quit) && time_end > SDL_GetTicks64()) {
        while (SDL_PollEvent(event) != 0) {
            *quit = *quit || event_quit(event->type, 0, debug_mode);
        }
        SDL_RenderClear(renderer);
        graphics_clear_gray(graphics, matrix);
        graphics_matrix(graphics, matrix);
        graphics_piece(graphics, *piece, matrix);
        graphics_render(renderer, graphics);
    }

    time_start = SDL_GetTicks64();
    time_end = time_start + TIME_RESET1;
    while (!(*quit) && time_end > SDL_GetTicks64()) {
        while (SDL_PollEvent(event) != 0) {
            *quit = *quit || event_quit(event->type, 0, debug_mode);
        }
        uint64_t time_from_start = SDL_GetTicks64() - time_start;
        SDL_RenderClear(renderer);
        graphics_clear_gray(graphics, matrix);
        graphics_matrix(graphics, matrix);
        graphics_piece(graphics, *piece, matrix);
        graphics_curtain1(graphics, matrix, time_from_start, TIME_RESET1);
        graphics_render(renderer, graphics);
    }
}

/* Play the rising curtain animation. */
void reset_loop2(SDL_Renderer* renderer, graphics_t* graphics, const matrix_t* matrix,
                 piece_t** piece, SDL_Event* event, bool* quit, bool debug_mode) {
    uint64_t time_start = SDL_GetTicks64();
    uint64_t time_end = time_start + TIME_RESET2;
    while (!(*quit) && time_end > SDL_GetTicks64()) {
        while (SDL_PollEvent(event) != 0) {
            *quit = *quit || event_quit(event->type, 0, debug_mode);
        }
        SDL_RenderClear(renderer);
        graphics_curtain2(graphics, matrix);
        graphics_render(renderer, graphics);
    }

    time_start = SDL_GetTicks64();
    time_end = time_start + TIME_RESET3;
    while (!(*quit) && time_end > SDL_GetTicks64()) {
        while (SDL_PollEvent(event) != 0) {
            *quit = *quit || event_quit(event->type, 0, debug_mode);
        }
        uint64_t time_from_start = SDL_GetTicks64() - time_start;
        SDL_RenderClear(renderer);
        graphics_clear_gray(graphics, matrix);
        graphics_piece(graphics, *piece, matrix);
        graphics_curtain3(graphics, matrix, time_from_start, TIME_RESET3);
        graphics_render(renderer, graphics);
    }
}

int32_t main_loop(SDL_Renderer* renderer, graphics_t* graphics,
                  matrix_t* matrix, piece_t** piece, bool debug_mode) {
    bot_t bot = bot_new();
    inputs_t inputs;
    inputs_clear(&inputs);
    SDL_Event event;
    uint64_t time_next_down = 0;
    uint64_t time_next_das = 0;
    uint64_t delay_bot_until = SDL_GetTicks64() + BOT_DELAY_AFTER_SPAWN;
    uint32_t lines_cleared = 0;
    uint32_t lines_next_pallete = LINES_PER_PALLETE;
    bool check_place_piece = false;
    bool ignore_mouse_motion = true;
    bool bot_force_drop = false;
    bool quit = false;

    int32_t err_value = 0;
    *piece = bot_next_piece(&bot, matrix, &err_value);
    if (err_value < 0) {
        return err_value;
    }
    while (!quit) {
        uint64_t ticks = SDL_GetTicks64();
        while (SDL_PollEvent(&event) != 0) {
            /*
             * SDL_MOUSEMOTION event happens when the application opens while the cursor is inside
             * window. To prevent the application from immediately closing, this event is ignored
             * on the first iteration of the main loop.
             */
            quit = quit || event_quit(event.type, ignore_mouse_motion, debug_mode);
        }
        ignore_mouse_motion = false;

        bool was_prev_input_move = inputs.left || inputs.right;
        bot_update_inputs(&bot, &inputs, *piece);
        /* delay the bot's input before dropping the piece */
        if (was_prev_input_move && inputs.down) {
            delay_bot_until = ticks + BOT_DELAY_AFTER_MOVEMENT;
        }

        if (ticks > delay_bot_until) {
            if (bot_force_drop) {
                inputs_clear(&inputs);
                inputs.down = true;
            }
            if (inputs.ccw) {
                bot_force_drop = !piece_rotate_ccw(*piece, matrix);
                delay_bot_until = ticks + BOT_DELAY_AFTER_ROTATION;
            }
            if (inputs.cw) {
                bot_force_drop = !piece_rotate_cw(*piece, matrix);
                delay_bot_until = ticks + BOT_DELAY_AFTER_ROTATION;
            }
            if (inputs.left) {
                if (ticks > time_next_das) {
                    bot_force_drop = !piece_move_left(*piece, matrix);
                    time_next_das = ticks + TIME_ARR;
                }
            }
            if (inputs.right) {
                if (ticks > time_next_das) {
                    bot_force_drop = !piece_move_right(*piece, matrix);
                    time_next_das = ticks + TIME_ARR;
                }
            }
            if (inputs.down) {
                if (ticks > time_next_down) {
                    check_place_piece = !piece_move_down(*piece, matrix);
                    time_next_down = ticks + TIME_DROP;
                }
            }
        }

        if (check_place_piece) {
            piece_place(*piece, matrix);
            are_loop(renderer, graphics, matrix, piece, &event, &quit, debug_mode);
            uint32_t filled_rows = 0;
            bool play_clear = false;
            for (size_t r = 0; r < matrix->rows; ++r) {
                if (matrix_row_full(matrix, r)) {
                    ++filled_rows;
                    ++lines_cleared;
                    play_clear = true;
                }
            }
            if (play_clear) {
                if (filled_rows >= LINES_CLEARED_TETRIS) {
                    clear_loop2(renderer, graphics, matrix, &event, &quit, debug_mode);
                } else {
                    clear_loop1(renderer, graphics, matrix, &event, &quit, debug_mode);
                }
                matrix_clean(matrix);
                if (lines_cleared >= lines_next_pallete) {
                    ++graphics->pallete_value;
                    lines_next_pallete += LINES_PER_PALLETE;
                }
            }
            piece_free(*piece);
            *piece = bot_next_piece(&bot, matrix, &err_value);
            if (err_value != 0) {
                return err_value;
            }
            /* If spawned piece collides with stack, play game over animation and reset game. */
            if (piece_collides(*piece, matrix)) {
                reset_loop1(renderer, graphics, matrix, piece, &event, &quit, debug_mode);
                matrix_clear(matrix);
                piece_free(*piece);
                *piece = bot_next_piece(&bot, matrix, &err_value);
                if (err_value != 0) {
                    return err_value;
                }
                reset_loop2(renderer, graphics, matrix, piece, &event, &quit, debug_mode);
            }
            bot_force_drop = 0;
            check_place_piece = 0;
            delay_bot_until = SDL_GetTicks64() + BOT_DELAY_AFTER_SPAWN;
        }

        SDL_RenderClear(renderer);
        graphics_clear_gray(graphics, matrix);
        graphics_matrix(graphics, matrix);
        graphics_piece(graphics, *piece, matrix);
        graphics_render(renderer, graphics);
    }
    return 0;
}

int32_t main(int32_t argc, char **argv) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    matrix_t* matrix = NULL;
    piece_t* piece = NULL;
    graphics_t* graphics = NULL;

    int32_t err_value = 0;
    bool init_attempted = false;
    bool debug = false;
    if (argc > 1) {
        if (strcmp(argv[1], "/s") == 0) {
            init_attempted = true;
            err_value = init(&window, SDL_WINDOW_FULLSCREEN_DESKTOP, &renderer, &graphics, &matrix);
        } else if (strcmp(argv[1], "/d") == 0) {
            init_attempted = true;
            debug = true;
            err_value = init(&window, SDL_WINDOW_RESIZABLE, &renderer, &graphics, &matrix);
        } else {
            err_value = ERROR_UNKNOWN_ARGUMENT;
        }
    } else {
        err_value = ERROR_FEW_ARGUMENTS;
    }
    if (init_attempted && err_value == 0) {
        graphics->pallete_value = rand_pallete_value();
        err_value = main_loop(renderer, graphics, matrix, &piece, debug);
    }
    if (err_value != 0) {
        printf("Error value: %d\n", err_value);
    }

    piece_free(piece);
    matrix_free(matrix);
    graphics_free(graphics);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    piece = NULL;
    matrix = NULL;
    graphics = NULL;
    renderer = NULL;
    window = NULL;
    SDL_Quit();
    return err_value;
}
