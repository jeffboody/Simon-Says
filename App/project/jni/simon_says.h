/*
 * Copyright (c) 2013 Jeff Boody
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef simon_says_H
#define simon_says_H

#include "a3d/a3d_texfont.h"
#include "a3d/a3d_texstring.h"

typedef struct
{
	GLsizei w;
	GLsizei h;

	float led_r;
	float led_b;
	float led_g;
	float led_y;

	int button_r;
	int button_b;
	int button_g;
	int button_y;

	a3d_texfont_t*   font;
	a3d_texstring_t* message;
} simon_says_t;

simon_says_t* simon_says_new(const char* font);
void          simon_says_delete(simon_says_t** _self);
void          simon_says_resize(simon_says_t* self, int w, int h);
void          simon_says_draw(simon_says_t* self);
void          simon_says_led(simon_says_t* self, int r, int g, int b, int y);
void          simon_says_button(simon_says_t* self, int r, int g, int b, int y);
void          simon_says_message(simon_says_t* self, const char* message);

#endif
