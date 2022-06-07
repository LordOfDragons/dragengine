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

#ifndef _IGDENATIVENULLTREELIST_H_
#define _IGDENATIVENULLTREELIST_H_

class igdeTreeList;
class igdeTreeItem;


/**
 * Null tree list.
 */
class igdeNativeNullTreeList{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullTreeList();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullTreeList();
	
	/** \brief Create native widget. */
	static igdeNativeNullTreeList *CreateNativeWidget( igdeTreeList &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void BuildTree();
	virtual void UpdateItem( igdeTreeItem *item );
	virtual void MakeItemVisible( igdeTreeItem *item );
	virtual void CreateAndInsertItem( igdeTreeItem *item );
	virtual void CreateAndAppendItem( igdeTreeItem *item );
	virtual void RemoveItem( igdeTreeItem *item );
	virtual void RemoveAllItems( igdeTreeItem *parent );
	virtual void ItemMoved( igdeTreeItem *item );
	virtual void SelectItem( igdeTreeItem *item );
	virtual void ItemsSortedIn( igdeTreeItem *item );
	virtual void Focus();
	virtual void UpdateEnabled();
	virtual void UpdateRows();
	virtual void UpdateDescription();
	/*@}*/
};

typedef igdeNativeNullTreeList igdeNativeTreeList;

#endif
