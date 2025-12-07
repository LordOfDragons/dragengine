/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEOXRBASICS_H_
#define _DEOXRBASICS_H_

#include <dragengine/dragengine_configuration.h>

#ifdef INTERNAL_XR_LOADER
#define XR_NO_PROTOTYPES
#endif

#ifdef OS_ANDROID
	#include <jni.h>
	
	typedef void* EGLDisplay;
	typedef void* EGLConfig;
	typedef void* EGLContext;
	typedef unsigned int EGLenum;
	
	#define XR_USE_PLATFORM_ANDROID
	#define XR_USE_GRAPHICS_API_OPENGL_ES
	
#elif defined OS_UNIX
// 	#include <X11/Xlib.h>
// 	#include <X11/Xutil.h>
	typedef struct _XDisplay Display;
	typedef unsigned long GLXDrawable;
	typedef void *GLXFBConfig;
	typedef void *GLXContext;
	
	#define XR_USE_PLATFORM_XLIB
	#define XR_USE_GRAPHICS_API_OPENGL

#elif defined OS_W32
	#include <dragengine/app/include_windows.h>
	typedef struct HDC__ *HDC;
	typedef struct HGLRC__ *HGLRC;
	
	#define XR_USE_PLATFORM_WIN32
	#define XR_USE_GRAPHICS_API_OPENGL
#endif

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

void deoxrDebugCheckCommand(XrResult result, const char *file, int line);
#define OXR_CHECK(cmd) deoxrDebugCheckCommand(cmd, __FILE__, __LINE__)

#endif
