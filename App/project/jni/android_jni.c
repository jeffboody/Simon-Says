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

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "a3d/a3d_GL.h"

#define LOG_TAG "SimonSays"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

typedef struct
{
	GLsizei w;
	GLsizei h;
} simon_says_t;

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

	self->w = 0;
	self->h = 0;
	return self;
}

void simon_says_delete(simon_says_t** _self)
{
	// *_self can be null
	assert(_self);

	simon_says_t* self = *_self;
	if(self)
	{
		LOGD("debug");
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static simon_says_t* simon_says = NULL;

/***********************************************************
* public                                                   *
***********************************************************/

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeCreate(JNIEnv* env)
{
	assert(env);
	LOGD("debug");

	if(simon_says != NULL)
	{
		LOGE("renderer already exists");
		return;
	}

	if(a3d_GL_load() == 0)
	{
		LOGE("a3d_GL_load failed");
		return;
	}

	simon_says = simon_says_new("/data/data/com.jeffboody.SimonSays/files/whitrabt.texgz");
}

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeDestroy(JNIEnv* env)
{
	assert(env);
	LOGD("debug");

	if(simon_says)
	{
		simon_says_delete(&simon_says);
		a3d_GL_unload();
	}
}

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeChangeSurface(JNIEnv* env, jobject  obj, jint w, jint h)
{
	assert(env);
	LOGD("debug");

	if(simon_says)
	{
		simon_says_resize(simon_says, w, h);
	}
}

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeDraw(JNIEnv* env)
{
	assert(env);
	LOGD("debug");

	if(simon_says)
	{
		a3d_GL_frame_begin();
		simon_says_draw(simon_says);
		a3d_GL_frame_end();
	}
}

JNIEXPORT int JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeClientVersion(JNIEnv* env)
{
	assert(env);
	LOGD("debug");
	return 1;
}
