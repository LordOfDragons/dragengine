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

#ifndef _IGDENATIVENULLCOMBOBOX_H_
#define _IGDENATIVENULLCOMBOBOX_H_

class igdeComboBox;
class igdeListItem;


/**
 * FOX Native comboBox.
 */
class igdeNativeNullComboBox{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullComboBox();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullComboBox();
	
	/** \brief Create native widget. */
	static igdeNativeNullComboBox* CreateNativeWidget( igdeComboBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void BuildList();
	virtual void UpdateItem( int index );
	virtual void SyncSelection( bool changing );
	virtual void OnInvalidValueChanged();
	virtual void UpdateText();
	virtual void InsertItem( int index, const igdeListItem &item );
	virtual void RemoveItem( int index );
	virtual void RemoveAllItems();
	virtual void MoveItem( int fromIndex, int toIndex );
	virtual void Focus();
	virtual void UpdateRowCount();
	virtual void UpdateEnabled();
	virtual void UpdateEditable();
	virtual void UpdateDescription();
	/*@}*/
};

typedef igdeNativeNullComboBox igdeNativeComboBox;

#endif
