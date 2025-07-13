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

#ifndef ERRORVALUES_H
#define ERRORVALUES_H

/* All error values listed in the enumerator must have a negative value. */
enum {
    ERROR_SDL_INIT = -14,
    ERROR_SDL_TEXTURE_FILTERING,
    ERROR_SDL_WINDOW,
    ERROR_SDL_RENDERER,
    ERROR_SDL_SET_RENDER_DRAW,
    ERROR_SDL_TEXTURE,
    ERROR_SDL_HINT,
    ERROR_MATRIX,
    ERROR_PIECE,
    ERROR_GRAPHICS,
    ERROR_BOT,
    ERROR_MATRIX_DIM_MISMATCH,
    ERROR_FEW_ARGUMENTS,
    ERROR_UNKNOWN_ARGUMENT,
};

#endif /* ERRORVALUES_H */
