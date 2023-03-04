/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOGLGL_H_
#define _DEOGLGL_H_

#include <dragengine/dragengine_configuration.h>

#ifdef ANDROID
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

#elif defined OS_W32_VS
	// required by visual studio
	#include "include_windows.h"
	struct __GLsync;
	typedef struct __GLsync *GLsync;
	#include <GL/gl.h>

#else
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
