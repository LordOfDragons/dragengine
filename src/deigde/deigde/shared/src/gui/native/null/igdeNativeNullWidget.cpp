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

#ifdef IGDE_TOOLKIT_NULL

#include "igdeNativeNullWidget.h"


// Class igdeNativeNullWidget
///////////////////////////////

// Management
///////////////

decPoint igdeNativeNullWidget::WidgetToScreen( const igdeWidget &, const decPoint &position ){
	return position;
}

decPoint igdeNativeNullWidget::GetCursorPosition( const igdeWidget & ){
	return decPoint();
}

decPoint igdeNativeNullWidget::GetSize( const igdeWidget & ){
	return decPoint( 256, 256 );
}

void igdeNativeNullWidget::UpdateVisible( const igdeWidget & ){
}

bool igdeNativeNullWidget::HasNativeParent( const igdeWidget & ){
	return false;
}

#if defined OS_UNIX && defined HAS_LIB_X11
Window igdeNativeNullWidget::NativeWidgetID( const igdeWidget & ){
	return 0;
}
#endif
#ifdef OS_W32
HWND igdeNativeNullWidget::NativeWidgetID( const igdeWidget & ){
	return NULL;
}
#endif

#if defined OS_UNIX && defined HAS_LIB_X11
Window igdeNativeNullWidget::NativeWidgetParentID( const igdeWidget & ){
	return 0;
}
#endif
#ifdef OS_W32
HWND igdeNativeNullWidget::NativeWidgetParentID( const igdeWidget & ){
	return NULL;
}
#endif

void igdeNativeNullWidget::DetachNativeWindow( const igdeWidget & ){
}

#if defined OS_UNIX && defined HAS_LIB_X11
Display *igdeNativeNullWidget::GetDisplayConnection(){
	return nullptr;
}
#endif

#endif
