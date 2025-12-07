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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "foxtoolkit.h"
#include "igdeNativeFoxWidget.h"
#include "../../igdeWidget.h"

#include <dragengine/common/exceptions.h>



// Class igdeNativeFoxWidget
//////////////////////////////

// Management
///////////////

decPoint igdeNativeFoxWidget::WidgetToScreen(const igdeWidget &widget, const decPoint &position){
	if(! widget.GetNativeWidget()){
		return position;
	}
	
	/*
	// FOX bug prevents this from working. the code below does work. enought right now
	const FXWindow * widget = (const FXWindow *)pNativeWidget;
	decPoint p(position);
	
	while(widget){
		p.x += widget->getX();
		p.y += widget->getY();
		widget = widget->getParent();
	}
	
	return p;
	*/
	
	const FXWindow &native = *((const FXWindow *)widget.GetNativeWidget());
	FXint tox, toy;
	native.translateCoordinatesTo(tox, toy, native.getRoot(), position.x, position.y);
	return decPoint(tox, toy);
}

decPoint igdeNativeFoxWidget::GetCursorPosition(const igdeWidget &widget){
	if(! widget.GetNativeWidget()){
		return decPoint();
	}
	
	const FXWindow &native = *((const FXWindow *)widget.GetNativeWidget());
	FXint x, y;
	FXuint buttons;
	if(native.getCursorPosition(x, y, buttons)){
		return decPoint(x, y);
	}
	
	return decPoint();
}

decPoint igdeNativeFoxWidget::GetSize(const igdeWidget &widget){
	if(! widget.GetNativeWidget()){
		return decPoint();
	}
	
	const FXWindow &native = *((const FXWindow *)widget.GetNativeWidget());
	return decPoint(native.getWidth(), native.getHeight());
}

void igdeNativeFoxWidget::UpdateVisible(const igdeWidget &widget){
	FXWindow * const native = (FXWindow*)widget.GetNativeWidget();
	if(! native){
		return;
	}
	
	if(widget.GetVisible()){
		native->show();
		
	}else{
		native->hide();
	}
	
	native->recalc();
}

bool igdeNativeFoxWidget::HasNativeParent(const igdeWidget &widget){
	FXWindow * const native = (FXWindow*)widget.GetNativeWidget();
	return native && native->getParent() != NULL;
}

#ifdef OS_UNIX
Window igdeNativeFoxWidget::NativeWidgetID(const igdeWidget &widget){
	FXWindow * const native = (FXWindow*)widget.GetNativeWidget();
	if(! native){
		return 0;
	}
	return (Window)native->id();
}
#endif
#ifdef OS_W32
HWND igdeNativeFoxWidget::NativeWidgetID(const igdeWidget &widget){
	FXWindow * const native = (FXWindow*)widget.GetNativeWidget();
	if(! native){
		return NULL;
	}
	return (HWND)native->id();
}
#endif

#ifdef OS_UNIX
Window igdeNativeFoxWidget::NativeWidgetParentID(const igdeWidget &widget){
	FXWindow * const native = (FXWindow*)widget.GetNativeWidget();
	if(! native || ! native->getParent()){
		return 0;
	}
	return (Window)native->getParent()->id();
}
#endif
#ifdef OS_W32
HWND igdeNativeFoxWidget::NativeWidgetParentID(const igdeWidget &widget){
	FXWindow * const native = (FXWindow*)widget.GetNativeWidget();
	if(! native || ! native->getParent()){
		return NULL;
	}
	return (HWND)native->getParent()->id();
}
#endif

void igdeNativeFoxWidget::DetachNativeWindow(const igdeWidget &widget){
	FXWindow * const native = (FXWindow*)widget.GetNativeWidget();
	if(native){
		native->detach();
	}
}

#ifdef OS_UNIX
Display *igdeNativeFoxWidget::GetDisplayConnection(){
	return (Display*)FXApp::instance()->getDisplay();
}
#endif

#endif
