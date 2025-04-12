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

#ifndef _DEOGLGL_H_
#define _DEOGLGL_H_

#include <dragengine/dragengine_configuration.h>

#ifdef WITH_OPENGLES
	#undef __glext_h_
	#define __glext_h_ 1
	
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES2/gl2platform.h>
	#include <GLES3/gl3platform.h>
	
	#undef __glext_h_
	
#elif defined OS_MACOS
	#include <stddef.h>
	typedef ptrdiff_t GLsizeiptr;
	typedef ptrdiff_t GLintptr;
	typedef char GLchar;
	
	#undef __glext_h_
	#define __glext_h_ 1
	#define GL_VERSION_1_2 1
	#define GL_VERSION_1_3 1
	#define GL_VERSION_1_5 1
	#define GL_VERSION_2_0 1
	#define GL_VERSION_2_1 1
	
	#include <OpenGL/gl.h>
	
	#undef GL_VERSION_2_1
	#undef GL_VERSION_2_0
	#undef GL_VERSION_1_5
	#undef GL_VERSION_1_3
	#undef GL_VERSION_1_2
	#undef __glext_h_

#else
	#ifdef OS_W32_VS
		#include "include_windows.h"
	#endif
	
	// there are tons of broken gl.h and glext.h out in the wild. we have to prevent them from
	// messing everything up since we have our own glext.h which actually deserves the name.
	// for this we define tokens where platforms are known containing incorrect definitions
	#undef __glext_h_
	#define __glext_h_ 1
	
	#include <GL/gl.h>
	
	#undef __glext_h_
#endif

// include the extension results as well as our own glext.h
#include "extensions/deoglExtResult.h"

#endif
