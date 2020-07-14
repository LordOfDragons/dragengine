/* 
 * Drag[en]gine IGDE
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

decPoint igdeNativeFoxWidget::WidgetToScreen( const igdeWidget &widget, const decPoint &position ){
	if( ! widget.GetNativeWidget() ){
		return position;
	}
	
	/*
	// FOX bug prevents this from working. the code below does work. enought right now
	const FXWindow * widget = ( const FXWindow * )pNativeWidget;
	decPoint p( position );
	
	while( widget ){
		p.x += widget->getX();
		p.y += widget->getY();
		widget = widget->getParent();
	}
	
	return p;
	*/
	
	const FXWindow &native = *( ( const FXWindow * )widget.GetNativeWidget() );
	FXint tox, toy;
	native.translateCoordinatesTo( tox, toy, native.getRoot(), position.x, position.y );
	return decPoint( tox, toy );
}

decPoint igdeNativeFoxWidget::GetCursorPosition( const igdeWidget &widget ){
	if( ! widget.GetNativeWidget() ){
		return decPoint();
	}
	
	const FXWindow &native = *( ( const FXWindow * )widget.GetNativeWidget() );
	FXint x, y;
	FXuint buttons;
	if( native.getCursorPosition( x, y, buttons) ){
		return decPoint( x, y );
	}
	
	return decPoint();
}

decPoint igdeNativeFoxWidget::GetSize( const igdeWidget &widget ){
	if( ! widget.GetNativeWidget() ){
		return decPoint();
	}
	
	const FXWindow &native = *( ( const FXWindow * )widget.GetNativeWidget() );
	return decPoint( native.getWidth(), native.getHeight() );
}

void igdeNativeFoxWidget::UpdateVisible( const igdeWidget &widget ){
	FXWindow * const native = ( FXWindow* )widget.GetNativeWidget();
	if( ! native ){
		return;
	}
	
	if( widget.GetVisible() ){
		native->show();
		
	}else{
		native->hide();
	}
	
	native->recalc();
}

bool igdeNativeFoxWidget::HasNativeParent( const igdeWidget &widget ){
	FXWindow * const native = ( FXWindow* )widget.GetNativeWidget();
	return native && native->getParent() != NULL;
}

#ifdef OS_UNIX
Window igdeNativeFoxWidget::NativeWidgetID( const igdeWidget &widget ){
	FXWindow * const native = ( FXWindow* )widget.GetNativeWidget();
	if( ! native ){
		return 0;
	}
	return ( Window )native->id();
}
#endif
#ifdef OS_W32
HWND igdeNativeFoxWidget::NativeWidgetID( const igdeWidget &widget ){
	FXWindow * const native = ( FXWindow* )widget.GetNativeWidget();
	if( ! native ){
		return INVALID_HANDLE_VALUE;
	}
	return ( HWND )native->id();
}
#endif

#ifdef OS_UNIX
Window igdeNativeFoxWidget::NativeWidgetParentID( const igdeWidget &widget ){
	FXWindow * const native = ( FXWindow* )widget.GetNativeWidget();
	if( ! native || ! native->getParent() ){
		return 0;
	}
	return ( Window )native->getParent()->id();
}
#endif
#ifdef OS_W32
HWND igdeNativeFoxWidget::NativeWidgetParentID( const igdeWidget &widget ){
	FXWindow * const native = ( FXWindow* )widget.GetNativeWidget();
	if( ! native || ! native->getParent() ){
		return INVALID_HANDLE_VALUE;
	}
	return ( HWND )native->getParent()->id();
}
#endif

void igdeNativeFoxWidget::DetachNativeWindow( const igdeWidget &widget ){
	FXWindow * const native = ( FXWindow* )widget.GetNativeWidget();
	if( native ){
		native->detach();
	}
}

#ifdef OS_UNIX
Display *igdeNativeFoxWidget::GetDisplayConnection(){
	return ( Display* )FXApp::instance()->getDisplay();
}
#endif

#endif
