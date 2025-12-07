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

#ifdef IGDE_TOOLKIT_NULL

#include "igdeNativeNullWidget.h"


// Class igdeNativeNullWidget
///////////////////////////////

// Management
///////////////

decPoint igdeNativeNullWidget::WidgetToScreen(const igdeWidget &, const decPoint &position){
	return position;
}

decPoint igdeNativeNullWidget::GetCursorPosition(const igdeWidget &){
	return decPoint();
}

decPoint igdeNativeNullWidget::GetSize(const igdeWidget &){
	return decPoint(256, 256);
}

void igdeNativeNullWidget::UpdateVisible(const igdeWidget &){
}

bool igdeNativeNullWidget::HasNativeParent(const igdeWidget &){
	return false;
}

#if defined OS_UNIX && defined HAS_LIB_X11
Window igdeNativeNullWidget::NativeWidgetID(const igdeWidget &){
	return 0;
}
#endif
#ifdef OS_W32
HWND igdeNativeNullWidget::NativeWidgetID(const igdeWidget &){
	return NULL;
}
#endif

#if defined OS_UNIX && defined HAS_LIB_X11
Window igdeNativeNullWidget::NativeWidgetParentID(const igdeWidget &){
	return 0;
}
#endif
#ifdef OS_W32
HWND igdeNativeNullWidget::NativeWidgetParentID(const igdeWidget &){
	return NULL;
}
#endif

void igdeNativeNullWidget::DetachNativeWindow(const igdeWidget &){
}

#if defined OS_UNIX && defined HAS_LIB_X11
Display *igdeNativeNullWidget::GetDisplayConnection(){
	return nullptr;
}
#endif

#endif
