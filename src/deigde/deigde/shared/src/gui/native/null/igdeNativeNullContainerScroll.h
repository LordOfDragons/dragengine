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

#ifndef _IGDENATIVENULLCONTAINERSCROLL_H_
#define _IGDENATIVENULLCONTAINERSCROLL_H_

class igdeContainerScroll;


/**
 * Null button.
 */
class igdeNativeNullContainerScroll{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullContainerScroll();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullContainerScroll();
	
	/** \brief Create native widget. */
	static igdeNativeNullContainerScroll* CreateNativeWidget( igdeContainerScroll &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
};

typedef igdeNativeNullContainerScroll igdeNativeContainerScroll;

#endif
