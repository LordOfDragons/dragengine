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

#ifndef _FOXTOOLKIT_H_
#define _FOXTOOLKIT_H_

#include <dragengine/dragengine_configuration.h>

#ifdef PI
#define FIX_FOX_BROKEN_PI
#undef PI
#endif

#include <fox-1.7/fx.h>
#include <fox-1.7/fx3d.h>
#include <fox-1.7/fxkeys.h>
#include <fox-1.7/FXPNGIcon.h>

#ifdef FIX_FOX_BROKEN_PI
#include <dragengine/common/math/decMath.h>
#endif

#define ICONLIST_HEADER_ID FXIconList::ID_HEADER

// anti-windows macros
#undef GetClassName
#undef GetObject
#undef LoadImage
#undef DeleteFile

#endif // _FOXTOOLKIT_H_
