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

#ifndef _IGDENATIVEFOXTREELIST_H_
#define _IGDENATIVEFOXTREELIST_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"


class igdeTreeList;
class igdeTreeItem;
class igdeGuiTheme;
class igdeNativeFoxResizer;


/**
 * \brief FOX toolkit Native tree list.
 */
class igdeNativeFoxTreeList : public FXVerticalFrame{
	FXDECLARE( igdeNativeFoxTreeList )
protected:
	   igdeNativeFoxTreeList();
	
public:
	enum eFoxIDs{
		ID_TREELIST = FXVerticalFrame::ID_LAST,
		ID_RESIZER,
		ID_LAST
	};
	
private:
	igdeTreeList *pOwner;
	igdeFontReference pFont;
	FXTreeList *pTreeList;
	igdeNativeFoxResizer *pResizer;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxTreeList( igdeTreeList &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxTreeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List box. */
	inline FXTreeList *GetTreeList() const{ return pTreeList; }
	
	/** \brief Resizer or \em NULL. */
	inline igdeNativeFoxResizer *GetResizser() const{ return pResizer; }
	
	
	
	void BuildTree();
	void UpdateItem( igdeTreeItem *item );
	void MakeItemVisible( igdeTreeItem *item );
	void CreateAndInsertItem( igdeTreeItem *item );
	void CreateAndAppendItem( igdeTreeItem *item );
	void RemoveItem( igdeTreeItem *item );
	void RemoveAllItems( igdeTreeItem *parent );
	void ItemMoved( igdeTreeItem *item );
	void SelectItem( igdeTreeItem *item );
	void ItemsSortedIn( igdeTreeItem *item );
	virtual void Focus();
	
	static int TreeListFlags( const igdeTreeList &owner );
	static igdeFont *TreeListFont( const igdeTreeList &owner, const igdeGuiTheme &guitheme );
	static int TreeListPadLeft( const igdeGuiTheme &guitheme );
	static int TreeListPadRight( const igdeGuiTheme &guitheme );
	static int TreeListPadTop( const igdeGuiTheme &guitheme );
	static int TreeListPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onExpanded( FXObject *sender, FXSelector selector, void *data );
	long onCollapsed( FXObject *sender, FXSelector selector, void *data );
	
	long onListCommand( FXObject *sender, FXSelector selector, void *data );
	long onListChanged( FXObject *sender, FXSelector selector, void *data );
	long onListRightMouseDown( FXObject *sender, FXSelector selector, void *data );
	long onListRightMouseUp( FXObject *sender, FXSelector selector, void *data );
	long onListDoubleClicked( FXObject *sender, FXSelector selector, void *data );
	
	long onResizerDrag( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
	
	
	
private:
	void pDropItemsNativeWidget( igdeTreeItem *parent );
};

#endif
