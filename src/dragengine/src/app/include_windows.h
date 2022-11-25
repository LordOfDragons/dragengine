/* 
 * Drag[en]gine Game Engine
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

#ifndef _DE_INCLUDE_WINDOWS_H_
#define _DE_INCLUDE_WINDOWS_H_

#include "../dragengine_configuration.h"

#ifdef OS_W32

#ifndef OS_W32_VS
	// this is required to fix problem with function related to Read-Write-Lock not found
	// (InitializeSRWLock, ...)
	#ifdef _WIN32_WINNT
		#undef _WIN32_WINNT
	#endif
	#define _WIN32_WINNT _WIN32_WINNT_WIN7
#endif

// required to use SetProcessDpiAwarenessContext
#ifdef WINVER
	#if WINVER < 0x0605
		#undef WINVER
		#define WINVER 0x0605
	#endif
#else
	#define WINVER 0x0605
#endif

#include <windows.h>


#ifdef OS_W32_UWP
	// for getting timeval
	#include <WinSock2.h>
#endif


// windows has the stupid idea of using macros to hack its own incompetence. this results in
// tons of strange compiler errors. undo here all the crap that is in our way. windows really
// should be shot to the moon with only a one-way ticket U_U
#undef CreateFont
#undef CreateImage
#undef CreateFile
#undef DeleteFile
#undef DrawText
#undef FindResource
#undef GetObject
#undef LoadFont
#undef LoadImage
#undef RemoveProp
#undef GetMessage
#undef GetProp


#ifdef OS_W32_VS
	// these conflict with type save min/max implementation in decMath
	#undef min
	#undef max
	
#else
	#undef near
	#undef far

#endif // OS_W32_VS

#endif // OS_W32

#endif // _DE_INCLUDE_WINDOWS_H_
