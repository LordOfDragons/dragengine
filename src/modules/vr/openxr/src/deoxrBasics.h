/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOXRBASICS_H_
#define _DEOXRBASICS_H_

#include <dragengine/dragengine_configuration.h>

#define XR_NO_PROTOTYPES
#define XR_USE_GRAPHICS_API_OPENGL

#ifdef OS_UNIX
// 	#include <X11/Xlib.h>
// 	#include <X11/Xutil.h>
	typedef struct _XDisplay Display;
	typedef unsigned long GLXDrawable;
	typedef void *GLXFBConfig;
	typedef void *GLXContext;
	
	#define XR_USE_PLATFORM_XLIB

#elif defined OS_W32
	#include <dragengine/app/include_windows.h>
	typedef struct HDC__ *HDC;
	typedef struct HGLRC__ *HGLRC;
	
	#define XR_USE_PLATFORM_WIN32
#endif

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

void deoxrDebugCheckCommand( XrResult result, const char *file, int line );
#define OXR_CHECK(cmd) deoxrDebugCheckCommand( cmd, __FILE__, __LINE__ )

#endif
