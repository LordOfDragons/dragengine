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

#ifndef _IGDENATIVEFOXMENUCASCADE_H_
#define _IGDENATIVEFOXMENUCASCADE_H_

#include "../foxtoolkit.h"

class igdeMenuCascade;
class igdeWindow;


/**
 * FOX native widget.
 */
class igdeNativeFoxMenuCascade{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	static void* CreateNativeWidget( igdeMenuCascade &owner );
	
	/** \brief Post create native widget. */
	static void PostCreateNativeWidget( igdeMenuCascade &owner, void *native );
	
	/** \brief Destroy native widget. */
	static void DestroyNativeWidget( igdeMenuCascade &owner, void *native );
	
	/** \brief Container native widget. */
	static void *GetNativeContainer( const igdeMenuCascade &owner, void *native );
	
	/** \brief Update title. */
	static void UpdateTitle( const igdeMenuCascade &owner, void *native );
	
	/** \brief Update description. */
	static void UpdateDescription( const igdeMenuCascade &owner, void *native );
	
	/** \brief Update icon. */
	static void UpdateIcon( const igdeMenuCascade &owner, void *native );
	
	/** \brief Update enabled. */
	static void UpdateEnabled( const igdeMenuCascade &owner, void *native );
	
	/** \brief Create native popup widget. */
	static void *CreateNativePopup( igdeMenuCascade &owner, igdeWidget &widgetOwner );
	
	/** \brief Create native popup widget. */
	static void PostCreateNativePopup( igdeMenuCascade &owner, void *native );
	
	/** \brief Show popup window with menu. */
	static void ShowPopupWindow( igdeMenuCascade &owner, igdeWidget &widgetOwner, const decPoint &position );
	
	/** \brief Destroy native popup widget. */
	static void DestroyNativePopup( igdeMenuCascade &owner, void *native );
	/*@}*/
};

typedef igdeNativeFoxMenuCascade igdeNativeMenuCascade;

#endif
