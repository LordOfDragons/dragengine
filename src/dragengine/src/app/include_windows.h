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
#undef GetClassName


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
