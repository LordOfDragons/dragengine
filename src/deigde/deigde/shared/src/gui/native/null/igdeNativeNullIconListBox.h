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

#ifndef _IGDENATIVENULLICONLISTBOX_H_
#define _IGDENATIVENULLICONLISTBOX_H_

#include <dragengine/common/math/decMath.h>

class igdeListItem;
class igdeIconListBox;


/**
 * \brief Null icon list box.
 */
class igdeNativeNullIconListBox{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullIconListBox();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullIconListBox();
	
	/** \brief Create native widget. */
	static igdeNativeNullIconListBox* CreateNativeWidget( igdeIconListBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void BuildHeader();
	virtual void BuildList();
	virtual void InsertItem( int index, igdeListItem &item );
	virtual void UpdateItem( int index );
	virtual void MoveItem( int fromIndex, int toIndex );
	virtual void RemoveItem( int index );
	virtual void UpdateSelection();
	virtual void UpdateHeader();
	virtual void UpdateStyles();
	virtual void UpdateDescription();
	virtual void UpdateEnabled();
	virtual void UpdateMinimumSize();
	virtual void Focus();
	virtual void MakeItemVisible( int index );
	virtual void RemoveAllItems();
	/*@}*/
};

typedef igdeNativeNullIconListBox igdeNativeIconListBox;

#endif
