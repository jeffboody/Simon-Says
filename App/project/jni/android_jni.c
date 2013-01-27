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

#include "simon_says.h"

#define LOG_TAG "SimonSays"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

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

JNIEXPORT void JNICALL Java_com_jeffboody_SimonSays_SimonSays_NativeLed(JNIEnv* env, jobject obj, jint a, jint b, jint c, jint d)
{
	assert(env);
	LOGD("debug a=%i, b=%i, c=%i, d=%i", a, b, c, d);

	if(simon_says)
	{
		simon_says_led(simon_says, a, b, c, d);
	}
}

JNIEXPORT void JNICALL Java_com_jeffboody_SimonSays_SimonSays_NativeButton(JNIEnv* env, jobject obj, jint a, jint b, jint c, jint d)
{
	assert(env);
	LOGD("debug a=%i, b=%i, c=%i, d=%i", a, b, c, d);

	if(simon_says)
	{
		simon_says_button(simon_says, a, b, c, d);
	}
}

JNIEXPORT void JNICALL Java_com_jeffboody_SimonSays_SimonSays_NativeMessage(JNIEnv* env, jobject obj, jstring message)
{
	assert(env);
	const char* m = (*env)->GetStringUTFChars(env, message, 0);
	LOGD("debug message=%s", m);

	if(simon_says)
	{
		simon_says_message(simon_says, m);
	}

	(*env)->ReleaseStringUTFChars(env, message, m);
}
