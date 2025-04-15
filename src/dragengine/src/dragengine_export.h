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

#ifndef _DRAGENGINE_EXPORT_H_
#define _DRAGENGINE_EXPORT_H_

#include "dragengine_configuration.h"

#ifdef OS_W32_VS
	#ifdef DE_COMPILE_DLL
		#define DE_DLL_EXPORT __declspec(dllexport)
	#else
		#define DE_DLL_EXPORT __declspec(dllimport)
	#endif

	// disable warning about "class X needs to have dll-interface to be used by clients of class Y"
	#pragma warning( disable: 4251 )
	
#else
	#define DE_DLL_EXPORT
#endif

#ifdef OS_ANDROID
	#define DE_FUNC_USED __attribute__((used,optnone,weak))
#else
	#define DE_FUNC_USED
#endif

#endif
