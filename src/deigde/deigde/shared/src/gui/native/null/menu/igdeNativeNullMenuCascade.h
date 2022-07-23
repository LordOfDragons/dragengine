/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _IGDENATIVENULLMENUCASCADE_H_
#define _IGDENATIVENULLMENUCASCADE_H_

#include <dragengine/common/math/decMath.h>

class igdeMenuCascade;
class igdeWindow;
class igdeWidget;


/**
 * FOX native widget.
 */
class igdeNativeNullMenuCascade{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	igdeNativeNullMenuCascade();
	virtual ~igdeNativeNullMenuCascade();
	
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

typedef igdeNativeNullMenuCascade igdeNativeMenuCascade;

#endif
