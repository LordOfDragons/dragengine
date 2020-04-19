/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEWPTTREEITEM_H_
#define _CEWPTTREEITEM_H_

#include <deigde/gui/model/igdeTreeItem.h>

class ceWPTTreeItemModel;
class igdeTreeList;
class igdeMenuCascade;


/**
 * \brief Tree item supporting ceWPTTreeItemModel.
 * 
 * Keeps reference to assigned model if present.
 */
class ceWPTTreeItem : public igdeTreeItem{
private:
	igdeTreeList *pTreeList;
	ceWPTTreeItemModel *pModel;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item. */
	ceWPTTreeItem( igdeTreeList *treeList );
	
protected:
	/** \brief Clean up tree item. */
	virtual ~ceWPTTreeItem();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Tree list. */
	inline igdeTreeList &GetTreeList() const{ return *pTreeList; }
	
	/** \brief Model or \em NULL. */
	inline ceWPTTreeItemModel *GetModel() const{ return pModel; }
	
	/** \brief Set model or \em NULL. */
	void SetModel( ceWPTTreeItemModel *model );
	
	
	
	/** \brief Add item with model. */
	void AddItem( ceWPTTreeItemModel *model );
	
	/** \brief Insert item with model at position. */
	void InsertItem( ceWPTTreeItemModel *model, int position );
	
	/**
	 * \brief Remove item for model.
	 * 
	 * Clears model from removed item if present.
	 */
	void RemoveItem( ceWPTTreeItemModel *model );
	
	/**
	 * \brief Remove all child items.
	 * 
	 * Clears model from all removed children down to the deepest level.
	 */
	void RemoveAllItems();
	
	/** \brief Sort items. */
	void SortItems();
	
	/** \brief Set item as current item and make it visible. */
	void SetAsCurrentItem();
	
	
	
	/** \brief User selected item. */
	void OnSelected();
	
	/** \brief Build context menu. */
	void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief User expanded item. */
	void OnExpanded();
	
	/** \brief User collapsed item. */
	void OnCollapsed();
	/*@}*/
};

#endif
 
