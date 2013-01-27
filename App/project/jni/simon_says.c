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

#include <stdlib.h>
#include <assert.h>
#include "a3d/a3d_GL.h"
#include "simon_says.h"

#define LOG_TAG "SimonSays"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

static GLfloat BOX[] =
{
	0.0f, 0.0f, -1.0f,   // 0
	0.0f, 1.0f, -1.0f,   // 1
	1.0f, 1.0f, -1.0f,   // 2
	1.0f, 0.0f, -1.0f,   // 3
};

static void drawbox(float top, float left,
                    float bottom, float right,
                    float r, float g, float b,
                    int filled)
{
	LOGD("top=%f, left=%f, bottom=%f, right=%f, r=%f, g=%f, b=%f",
	     top, left, bottom, right, r, g, b);

	BOX[0]  = left;
	BOX[1]  = top;
	BOX[3]  = left;
	BOX[4]  = bottom;
	BOX[6]  = right;
	BOX[7]  = bottom;
	BOX[9]  = right;
	BOX[10] = top;

	glColor4f(r, g, b, 1.0f);
	glVertexPointer(3, GL_FLOAT, 0, BOX);
	if(filled)
	{
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	else
	{
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

/***********************************************************
* public                                                   *
***********************************************************/

simon_says_t* simon_says_new(const char* font)
{
	LOGD("debug");

	LOGI("GL vendor     : %s", glGetString(GL_VENDOR));
	LOGI("GL renderer   : %s", glGetString(GL_RENDERER));
	LOGI("GL version    : %s", glGetString(GL_VERSION));
	LOGI("GL extensions : %s", glGetString(GL_EXTENSIONS));

	simon_says_t* self = (simon_says_t*) malloc(sizeof(simon_says_t));
	if(self == NULL)
	{
		LOGE("malloc failed");
		return NULL;
	}

	self->font = a3d_texfont_new(font);
	if(self->font == NULL)
		goto fail_font;

	self->message = a3d_texstring_new(self->font, 32, 48, A3D_TEXSTRING_TOP_CENTER, 1.0f, 1.0f, 0.235f, 1.0f);
	if(self->message == NULL)
		goto fail_message;
	a3d_texstring_printf(self->message, "Welcome to Simon Says");

	self->w = 0;
	self->h = 0;

	self->led_a = 1.0f;
	self->led_b = 1.0f;
	self->led_c = 1.0f;
	self->led_d = 1.0f;

	self->button_a = 0;
	self->button_b = 0;
	self->button_c = 0;
	self->button_d = 0;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnableClientState(GL_VERTEX_ARRAY);

	// success
	return self;

	// failure
	fail_message:
		a3d_texfont_delete(&self->font);
	fail_font:
		free(self);
	return NULL;
}

void simon_says_delete(simon_says_t** _self)
{
	// *_self can be null
	assert(_self);

	simon_says_t* self = *_self;
	if(self)
	{
		LOGD("debug");
		a3d_texstring_delete(&self->message);
		a3d_texfont_delete(&self->font);
		free(self);
		*_self = NULL;
	}
}

void simon_says_resize(simon_says_t* self, int w, int h)
{
	assert(self);
	LOGI("%ix%i", w, h);

	self->w = w;
	self->h = h;
	glViewport(0, 0, w, h);
}

void simon_says_draw(simon_says_t* self)
{
	assert(self);
	LOGD("debug");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0.0f, self->w, self->h, 0.0f, 0.0f, 2.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// compute simon says y-coords
	float yt = 0.10f * self->h;
	float yc = 0.45f * self->h;
	float yb = 0.80f * self->h;

	// compute simon says size
	float size = yc - yt;

	// compute simon says x-coords
	float xc = 0.50f * self->w;
	float xl = xc - size;
	float xr = xc + size;

	// font coords
	float fy = 0.85f * self->h;
	float fx = xc;

	glClear(GL_COLOR_BUFFER_BIT);

	// draw squares for red, green, blue, yellow
	drawbox(yt, xl, yc, xc,
            self->led_a, 0.0f, 0.0f,
            1);
	drawbox(yt, xc, yc, xr,
            0.0f, self->led_b, 0.0f,
            1);
	drawbox(yc, xl, yb, xc,
            0.0f, 0.0f, self->led_c,
            1);
	drawbox(yc, xc, yb, xr,
            self->led_d, self->led_d, 0.0f,
            1);

	// draw message
	a3d_texstring_draw(self->message, fx, fy, self->w, self->h);

	A3D_GL_GETERROR();
}

void simon_says_led(simon_says_t* self, int a, int b, int c, int d)
{
	assert(self);
	LOGD("debug a=%i, b=%i, c=%i, d=%i", a, b, c, d);

	self->led_a = a ? 1.0f : 0.5f;
	self->led_b = b ? 1.0f : 0.5f;
	self->led_c = c ? 1.0f : 0.5f;
	self->led_d = d ? 1.0f : 0.5f;
}

void simon_says_button(simon_says_t* self, int a, int b, int c, int d)
{
	assert(self);
	LOGD("debug a=%i, b=%i, c=%i, d=%i", a, b, c, d);

	self->button_a = a;
	self->button_b = b;
	self->button_c = c;
	self->button_d = d;
}

void simon_says_message(simon_says_t* self, const char* message)
{
	assert(self);
	LOGD("debug message=%s", message);

	a3d_texstring_printf(self->message, "%s", message);
}
