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

#ifndef _IGDETREELIST_H_
#define _IGDETREELIST_H_

#include <stddef.h>

#include "igdeWidget.h"
#include "model/igdeTreeItemReference.h"
#include "model/igdeTreeItemSorterReference.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>


class igdeTreeListListener;
class igdeIcon;
class igdeTreeItem;
class igdeTreeItemReference;


/**
 * \brief IGDE UI TreeList.
 */
class igdeTreeList : public igdeWidget{
private:
	bool pEnabled;
	igdeTreeItemReference pFirstChild;
	igdeTreeItemReference pSelection;
	igdeTreeItemSorterReference pSorter;
	int pRows;
	decString pDescription;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listbox. */
	igdeTreeList( igdeEnvironment &environment, int rows, const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTreeList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief TreeList is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Visible rows. */
	inline int GetRows() const{ return pRows; }
	
	/** \brief Set visible rows. */
	void SetRows( int rows );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief First child item. */
	inline igdeTreeItem *GetFirstChild() const{ return pFirstChild; }
	
	/** \brief Last child item. */
	igdeTreeItem *GetLastChild() const;
	
	/** \brief Number of children. */
	int GetChildrenCount() const;
	
	/** \brief Item with data or NULL if not found. */
	igdeTreeItem *GetItemWithData( void *data ) const;
	
	/** \brief Item with data starting at parent or NULL if not found. */
	igdeTreeItem *GetItemWithData( igdeTreeItem *parent, void *data ) const;
	
	/** \brief Item is present. */
	bool HasItem( igdeTreeItem *item ) const;
	
	/** \brief Item with text is present. */
	bool HasItem( const char *item ) const;
	
	/** \brief Item with data is present. */
	bool HasItemWithData( void *data ) const;
	
	/** \brief Add item as last child of parent. */
	void AppendItem( igdeTreeItem *parent, igdeTreeItem *item );
	
	/** \brief Add item of type igdeTreeItem with text as last child of parent. */
	igdeTreeItem *AppendItem( igdeTreeItem *parent, const char *text,
		igdeIcon *icon = NULL, void *data = NULL );
	
	void AppendItem( igdeTreeItem *parent, igdeTreeItemReference &item, const char *text,
		igdeIcon *icon = NULL, void *data = NULL );
	
	/** \brief Insert item before another. */
	void InsertItemBefore( igdeTreeItem *beforeItem, igdeTreeItem *item );
	
	/** \brief Insert item of type igdeTreeItem with text at index. */
	igdeTreeItem *InsertItemBefore( igdeTreeItem *beforeItem, const char *text,
		igdeIcon *icon = NULL, void *data = NULL );
	
	void InsertItemBefore( igdeTreeItem *beforeItem, igdeTreeItemReference &item,
		const char *text, igdeIcon *icon = NULL, void *data = NULL );
	
	/** \brief Insert item after another. */
	void InsertItemAfter( igdeTreeItem *afterItem, igdeTreeItem *item );
	
	/** \brief Insert item of type igdeTreeItem with text at index. */
	igdeTreeItem *InsertItemAfter( igdeTreeItem *afterItem, const char *text,
		igdeIcon *icon = NULL, void *data = NULL );
	
	void InsertItemAfter( igdeTreeItem *afterItem, igdeTreeItemReference &item,
		const char *text, igdeIcon *icon = NULL, void *data = NULL );
	
	/** \brief Move item before another. */
	void MoveItemBefore( igdeTreeItem *item, igdeTreeItem *beforeItem );
	
	/** \brief Move item after another. */
	void MoveItemAfter( igdeTreeItem *item, igdeTreeItem *afterItem );
	
	/** \brief Remove item from index. */
	void RemoveItem( igdeTreeItem *item );
	
	/** \brief Remove all items. */
	void RemoveAllItems();
	
	/** \brief Remove all items underneath parent. */
	void RemoveAllItems( igdeTreeItem *parent );
	
	/** \brief Item modified. */
	void ItemChanged( igdeTreeItem *item );
	
	
	
	/** \brief Sorter or NULL. */
	inline igdeTreeItemSorter *GetSorter() const{ return pSorter; }
	
	/** \brief Set sorter or NULL. */
	void SetSorter( igdeTreeItemSorter *sorter );
	
	/** \brief Set default sorter sorting items ascending lexicographically by their text. */
	void SetDefaultSorter();
	
	/** \brief Sort children of item using sorter if present. */
	void SortItems( igdeTreeItem *item );
	
	/** \brief Sort all items using sorter if present. */
	void SortAllItems();
	
	
	
	/** \brief Selected item or NULL. */
	inline igdeTreeItem *GetSelection() const{ return pSelection; }
	
	/** \brief Set selected item or NULL. */
	void SetSelection( igdeTreeItem *selection );
	
	/**
	 * \brief Set selected item matching data.
	 * 
	 * Short-cut for calling SetSelection(IndexOfItemWithData(data)).
	 */
	void SetSelectionWithData( void *data );
	
	/** \brief Make item visible. */
	void MakeItemVisible( igdeTreeItem *item );
	
	/** \brief Make selection visible if any selection is present. */
	void MakeSelectionVisible();
	
	/** \brief Show context menu at position. */
	void ShowContextMenu( const decPoint &position );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeTreeListListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeTreeListListener *listener );
	
	/** \brief Notify listeners selection changed. */
	virtual void NotifySelectionChanged();
	
	/** \brief Notify listeners item expanded. */
	virtual void NotifyItemExpanded( igdeTreeItem *item );
	
	/** \brief Notify listeners item collapsed. */
	virtual void NotifyItemCollapsed( igdeTreeItem *item );
	
	/** \brief Notify listeners double clicked on item. */
	virtual void NotifyDoubleClickItem( igdeTreeItem *item );
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	
	
	
protected:
	/** \brief Item added. */
	virtual void OnItemAdded( igdeTreeItem *item );
	
	/** \brief Item removed. */
	virtual void OnItemRemoved( igdeTreeItem *item );
	
	/** \brief All items removed. */
	virtual void OnAllItemsRemoved( igdeTreeItem *parent );
	
	/** \brief Item changed. */
	virtual void OnItemChanged( igdeTreeItem *item );
	
	/** \brief Item moved. */
	virtual void OnItemMoved( igdeTreeItem *item );
	
	/** \brief Items sorted. */
	virtual void OnItemsSorted( igdeTreeItem *item );
	
	/** \brief Selection changed. */
	virtual void OnSelectionChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Rows changed. */
	virtual void OnRowsChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Item with data. */
	igdeTreeItem *pGetItemWithData( igdeTreeItem *parent, void *data ) const;
	
	/** \brief Item is present. */
	bool pHasItem( igdeTreeItem *parent, igdeTreeItem *item ) const;
	
	/** \brief Item with text is present. */
	bool pHasItem( igdeTreeItem *parent, const char *text ) const;
	
	/** \brief Item with data is present. */
	bool pHasItem( igdeTreeItem *parent, void *data ) const;
	
	/** \brief Remove item. */
	void pRemoveItem( igdeTreeItem *item );
	
	/** \brief Append item. */
	void pAppendItem( igdeTreeItem *parent, igdeTreeItem *item );
	
	/** \brief Insert item before. */
	void pInsertItemBefore(  igdeTreeItem *item, igdeTreeItem *beforeItem );
	
	/** \brief Insert item after. */
	void pInsertItemAfter(  igdeTreeItem *item, igdeTreeItem *afterItem );
	
	/** \brief Remove all items. */
	void pRemoveAllItems( igdeTreeItem *item );
	/*@}*/
};

#endif
