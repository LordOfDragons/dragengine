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

#ifndef _IGDENATIVENULLLISTBOX_H_
#define _IGDENATIVENULLLISTBOX_H_

class igdeListBox;


/**
 * Null list box.
 */
class igdeNativeNullListBox{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullListBox();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullListBox();
	
	/** \brief Create native widget. */
	static igdeNativeNullListBox* CreateNativeWidget( igdeListBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void BuildList();
	virtual void UpdateItem( int index );
	virtual void UpdateStyles();
	virtual void UpdateSelection();
	virtual void Focus();
	virtual void MakeItemVisible( int index );
	virtual void InsertItem( int index );
	virtual void RemoveItem( int index );
	virtual void RemoveAllItems();
	virtual void MoveItem( int fromIndex, int toIndex );
	virtual void UpdateEnabled();
	virtual void UpdateRowCount();
	virtual void UpdateDescription();
	/*@}*/
};

typedef igdeNativeNullListBox igdeNativeListBox;

#endif
