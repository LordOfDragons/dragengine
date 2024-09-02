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

#ifndef _GDKINCLUDE_H_
#define _GDKINCLUDE_H_

#include <Windows.h>

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
#undef CreateService

// these conflict with type save min/max implementation in decMath
#undef min
#undef max

// GDK includes
#include <XGameRuntimeInit.h>
#include <XAsync.h>
#include <XUser.h>
#include <XGameErr.h>
#include <xsapi-c/types_c.h>
#include <xsapi-c/xbox_live_global_c.h>
#include <xsapi-c/xbox_live_context_c.h>
#include <xsapi-c/achievements_manager_c.h>

#endif
