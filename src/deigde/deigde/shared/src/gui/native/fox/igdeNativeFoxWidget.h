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
