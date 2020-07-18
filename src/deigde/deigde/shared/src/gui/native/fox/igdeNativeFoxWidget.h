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

#ifndef _IGDENATIVEFOXWIDGET_H_
#define _IGDENATIVEFOXWIDGET_H_

#include <dragengine/dragengine_configuration.h>
#include <dragengine/common/math/decMath.h>

#if defined OS_UNIX && defined HAS_LIB_X11
#include <X11/Xlib.h>
#endif

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif

#ifdef OS_BEOS
	class BWindow;
#endif

class igdeWidget;


/**
 * \brief FOX Native widget.
 */
class igdeNativeFoxWidget{
public:
	/** \name Management */
	/*@{*/
	/** \brief Convert widget position to screen position. */
	static decPoint WidgetToScreen( const igdeWidget &widget, const decPoint &position );
	
	/** \brief Cursor position inside widget. */
	static decPoint GetCursorPosition( const igdeWidget &widget );
	
	/** \brief Widget size. */
	static decPoint GetSize( const igdeWidget &widget );
	
	/** \brief Update visibility. */
	static void UpdateVisible( const igdeWidget &widget );
	
	/** \brief Window has native parent. */
	static bool HasNativeParent( const igdeWidget &widget );
	
	/** \brief Get OS specific native widget handle. */
	#ifdef OS_UNIX
	static Window NativeWidgetID( const igdeWidget &widget );
	#endif
	#ifdef OS_W32
	static HWND NativeWidgetID( const igdeWidget &widget );
	#endif
	
	/** \brief Get OS specific native parent widget handle. */
	#ifdef OS_UNIX
	static Window NativeWidgetParentID( const igdeWidget &widget );
	#endif
	#ifdef OS_W32
	static HWND NativeWidgetParentID( const igdeWidget &widget );
	#endif
	
	/** \brief Detach native window. */
	static void DetachNativeWindow( const igdeWidget &widget );
	
	/** \brief Get display connection. */
	#ifdef OS_UNIX
	static Display *GetDisplayConnection();
	#endif
	/*@}*/
};

typedef igdeNativeFoxWidget igdeNativeWidget;

#endif
