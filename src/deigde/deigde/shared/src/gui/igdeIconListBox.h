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

#ifndef _IGDEICONLISTBOX_H_
#define _IGDEICONLISTBOX_H_

#include <stddef.h>

#include "igdeWidget.h"
#include "model/igdeListItemSorterReference.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>


class igdeIconListBoxListener;
class igdeIcon;
class igdeListHeader;
class igdeListItem;
class igdeListItemReference;


/**
 * \brief IGDE UI IconListBox.
 */
class igdeIconListBox : public igdeWidget{
public:
	/** \brief Selection mode. */
	enum eSelectionMode{
		/** \brief Single select. */
		esmSingle,
		
		/** \brief Multiple selection. */
		esmMultiple
	};
	
	/** \brief View mode. */
	enum eViewMode{
		/** \brief List mode similar to igdeListBox. */
		evmList,
		
		/** \brief Horizontal icon mode. */
		evmIconHorizontal,
		
		/** \brief Vertical icon mode. */
		evmIconVertical
	};
	
	
	
private:
	bool pEnabled;
	decObjectList pItems;
	eSelectionMode pSelectionMode;
	eViewMode pViewMode;
	int pSelection;
	igdeListItemSorterReference pSorter;
	int pRows;
	decString pDescription;
	decObjectOrderedSet pHeaders;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listbox. */
	igdeIconListBox( igdeEnvironment &environment, int rows, const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeIconListBox();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief IconListBox is enabled. */
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
	
	/** \brief View mode. */
	inline eViewMode GetViewMode() const{ return pViewMode; }
	
	/** \brief Set view mode. */
	void SetViewMode( eViewMode mode );
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Number of items. */
	int GetItemCount() const;
	
	/** \brief Item at index. */
	igdeListItem *GetItemAt( int index ) const;
	
	/** \brief Item is present. */
	bool HasItem( igdeListItem *item ) const;
	
	/** \brief Item with text is present. */
	bool HasItem( const char *item ) const;
	
	/** \brief Item with data is present. */
	bool HasItemWithData( void *data ) const;
	
	/** \brief Index of item or -1 if absent. */
	int IndexOfItem( igdeListItem *item ) const;
	
	/** \brief Index of item with text or -1 if absent. */
	int IndexOfItem( const char *item ) const;
	
	/** \brief Index of item with data or -1 if absent. */
	int IndexOfItemWithData( void *data ) const;
	
	/** \brief Add item. */
	void AddItem( igdeListItem *item );
	
	/** \brief Add item of type igdeListItem with text. */
	void AddItem( const char *text, igdeIcon *icon = NULL, void *data = NULL );
	
	void AddItem( const char *text, const decStringList &details,
		igdeIcon *icon = NULL, void *data = NULL );
	
	void AddItem( igdeListItemReference &item, const char *text, igdeIcon *icon = NULL,
		void *data = NULL );
	
	void AddItem( igdeListItemReference &item, const char *text, const decStringList &details,
		igdeIcon *icon = NULL, void *data = NULL );
	
	/** \brief Insert item at index. */
	void InsertItem( int index, igdeListItem *item );
	
	/** \brief Insert item of type igdeListItem with text at index. */
	void InsertItem( int index, const char *text, igdeIcon *icon = NULL, void *data = NULL );
	
	void InsertItem( int index, const char *text, const decStringList &details,
		igdeIcon *icon = NULL, void *data = NULL );
	
	void InsertItem( igdeListItemReference &item, int index, const char *text,
		igdeIcon *icon = NULL, void *data = NULL );
	
	void InsertItem( igdeListItemReference &item, int index, const char *text,
		const decStringList &details, igdeIcon *icon = NULL, void *data = NULL );
	
	/** \brief Move item. */
	void MoveItem( int fromIndex, int toIndex );
	
	/** \brief Remove item from index. */
	void RemoveItem( int index );
	
	/** \brief Remove all items. */
	void RemoveAllItems();
	
	/** \brief Item modified. */
	void ItemChangedAt( int index );
	
	
	
	/** \brief Sorter or NULL. */
	inline igdeListItemSorter *GetSorter() const{ return pSorter; }
	
	/** \brief Set sorter or NULL. */
	void SetSorter( igdeListItemSorter *sorter );
	
	/** \brief Set default sorter sorting items ascending lexicographically by their text. */
	void SetDefaultSorter();
	
	/** \brief Sort items using sorter if present. */
	void SortItems();
	
	
	
	/** \brief Selection mode. */
	inline eSelectionMode GetSelectionMode() const{ return pSelectionMode; }
	
	/** \brief Set selection mode. */
	void SetSelectionMode( eSelectionMode mode );
	
	/** \brief Index of selected item or -1. */
	inline int GetSelection() const{ return pSelection; }
	
	/** \brief Selected item or NULL. */
	igdeListItem *GetSelectedItem() const;
	
	/** \brief Selected item is not -1. */
	inline bool HasSelection() const{ return pSelection != -1; }
	
	/** \brief Set index of selected item or -1. */
	void SetSelection( int selection );
	
	/**
	 * \brief Set selected item matching data.
	 * 
	 * Short-cut for calling SetSelection(IndexOfItemWithData(data)).
	 */
	void SetSelectionWithData( void *data );
	
	/** \brief Add item to the selection range if absent. */
	void SelectItem( int index );
	
	/** \brief Select all items. */
	void SelectAllItems();
	
	/** \brief Remove item from selection range if present. */
	void DeselectItem( int index );
	
	/** \brief Deselect all items. */
	void DeselectAllItems();
	
	
	
	/** \brief Number of headers. */
	int GetHeaderCount() const;
	
	/** \brief Header at index. */
	igdeListHeader *GetHeaderAt( int index ) const;
	
	/** \brief Header is present. */
	bool HasHeader( igdeListHeader *header ) const;
	
	/** \brief Add header. */
	void AddHeader( igdeListHeader *header );
	
	/** \brief Remove header. */
	void RemoveHeader( igdeListHeader *header );
	
	/** \brief Remove all headers. */
	void RemoveAllHeaders();
	
	/** \brief Header modified. */
	void HeaderChangedAt( int index );
	
	/** \brief Notify listeners header clicked. */
	virtual void NotifyHeaderClicked( int index );
	
	
	
	/** \brief Ensure item is visible. */
	virtual void EnsureItemVisible( int index );
	
	/** \brief Ensure selected item is visible if selection is present. */
	void EnsureSelectedItemVisible();
	
	/** \brief Show context menu at position. */
	void ShowContextMenu( const decPoint &position );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeIconListBoxListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeIconListBoxListener *listener );
	
	/** \brief Notify listeners selection changed. */
	virtual void NotifySelectionChanged();
	
	/** \brief Notify listeners item selected. */
	virtual void NotifyItemSelected( int index );
	
	/** \brief Notify listeners item deselected. */
	virtual void NotifyItemDeselected( int index );
	
	/** \brief Notify listeners double clicked on item. */
	virtual void NotifyDoubleClickItem( int index );
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
	virtual void OnItemAdded( int index );
	
	/** \brief Item removed. */
	virtual void OnItemRemoved( int index );
	
	/** \brief All items removed. */
	virtual void OnAllItemsRemoved();
	
	/** \brief Item changed. */
	virtual void OnItemChanged( int index );
	
	/** \brief Item moved. */
	virtual void OnItemMoved( int fromIndex, int toIndex );
	
	/** \brief Items sorted. */
	virtual void OnItemsSorted();
	
	/** \brief Selection changed. */
	virtual void OnSelectionChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief View mode changed. */
	virtual void OnViewModeChanged();
	
	/** \brief Selection mode changed. */
	virtual void OnSelectionModeChanged();
	
	/** \brief Header changed. */
	virtual void OnHeaderChanged();
	/*@}*/
};

#endif
