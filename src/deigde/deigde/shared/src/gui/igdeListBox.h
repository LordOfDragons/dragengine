/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _IGDELISTBOX_H_
#define _IGDELISTBOX_H_

#include <stddef.h>

#include "igdeWidget.h"
#include "model/igdeListItem.h"
#include "model/igdeListItemSorter.h"

#include <dragengine/common/collection/decIntSet.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>

class igdeListBoxListener;
class igdeIcon;
class igdeListItem;

/**
 * \brief IGDE UI ListBox.
 */
class DE_DLL_EXPORT igdeListBox : public igdeWidget{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeListBox> Ref;
	
	
	/** \brief Selection mode. */
	enum eSelectionMode{
		/** \brief Single select. */
		esmSingle,
		
		/** \brief Multiple selection. */
		esmMultiple
	};
	
	
	
private:
	bool pEnabled;
	decObjectList pItems;
	eSelectionMode pSelectionMode;
	int pSelection;
	igdeListItemSorter::Ref pSorter;
	int pRows;
	decString pDescription;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listbox. */
	igdeListBox(igdeEnvironment &environment, int rows, const char *description = "");
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeListBox();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief ListBox is enabled. */
	inline bool GetEnabled() const{return pEnabled;}
	
	/** \brief Set if button is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Visible rows. */
	inline int GetRows() const{return pRows;}
	
	/** \brief Set visible rows. */
	void SetRows(int rows);
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{return pDescription;}
	
	/** \brief Set description shown in tool tips. */
	void SetDescription(const char *description);
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Number of items. */
	int GetItemCount() const;
	
	/** \brief Item at index. */
	igdeListItem *GetItemAt(int index) const;
	
	/** \brief Item with data or NULL if not found. */
	igdeListItem *GetItemWithData(void *data) const;
	
	/** \brief Item is present. */
	bool HasItem(igdeListItem *item) const;
	
	/** \brief Item with text is present. */
	bool HasItem(const char *item) const;
	
	/** \brief Item with data is present. */
	bool HasItemWithData(void *data) const;
	
	/** \brief Index of item or -1 if absent. */
	int IndexOfItem(igdeListItem *item) const;
	
	/** \brief Index of item with text or -1 if absent. */
	int IndexOfItem(const char *item) const;
	
	/** \brief Index of item with data or -1 if absent. */
	int IndexOfItemWithData(void *data) const;
	
	/** \brief Add item. */
	void AddItem(igdeListItem *item);
	
	/** \brief Add item of type igdeListItem with text. */
	void AddItem(const char *text, igdeIcon *icon = NULL, void *data = NULL);
	
	void AddItem(igdeListItem::Ref &item, const char *text, igdeIcon *icon = NULL, void *data = NULL);
	
	/** \brief Insert item at index. */
	void InsertItem(int index, igdeListItem *item);
	
	/** \brief Insert item of type igdeListItem with text at index. */
	void InsertItem(int index, const char *text, igdeIcon *icon = NULL, void *data = NULL);
	
	void InsertItem(igdeListItem::Ref &item, int index, const char *text,
		igdeIcon *icon = NULL, void *data = NULL);
	
	/** \brief Move item. */
	void MoveItem(int fromIndex, int toIndex);
	
	/** \brief Remove item from index. */
	void RemoveItem(int index);
	
	/** \brief Remove all items. */
	void RemoveAllItems();
	
	/** \brief Item modified. */
	void ItemChangedAt(int index);
	
	
	
	/** \brief Sorter or NULL. */
	inline igdeListItemSorter *GetSorter() const{return pSorter;}
	
	/** \brief Set sorter or NULL. */
	void SetSorter(igdeListItemSorter *sorter);
	
	/** \brief Set default sorter sorting items ascending lexicographically by their text. */
	void SetDefaultSorter();
	
	/** \brief Sort items using sorter if present. */
	void SortItems();
	
	
	
	/** \brief Selection mode. */
	inline eSelectionMode GetSelectionMode() const{return pSelectionMode;}
	
	/** \brief Set selection mode. */
	void SetSelectionMode(eSelectionMode mode);
	
	/** \brief Index of selected item or -1. */
	inline int GetSelection() const{return pSelection;}
	
	/** \brief Selected item or nullptr. */
	igdeListItem *GetSelectedItem() const;
	
	/** \brief Selected item data or nullptr. */
	void *GetSelectedItemData() const;
	
	/** \brief Set index of selected item or -1. */
	void SetSelection(int selection);
	
	/**
	 * \brief Set selected item matching data.
	 * 
	 * Short-cut for calling SetSelection(IndexOfItemWithData(data)).
	 */
	void SetSelectionWithData(void *data);
	
	/** \brief Add item to the selection range if absent. */
	void SelectItem(int index);
	
	/** \brief Select all items. */
	void SelectAllItems();
	
	/** \brief Remove item from selection range if present. */
	void DeselectItem(int index);
	
	/** \brief Deselect all items. */
	void DeselectAllItems();
	
	/** \brief Make item visible. */
	void MakeItemVisible(int index);
	
	/** \brief Make selection visible if any selection is present. */
	void MakeSelectionVisible();
	
	/** \brief Show context menu at position. */
	void ShowContextMenu(const decPoint &position);
	
	
	
	/** \brief Add listener. */
	void AddListener(igdeListBoxListener *listener);
	
	/** \brief Remove listener. */
	void RemoveListener(igdeListBoxListener *listener);
	
	/** \brief Notify listeners selection changed. */
	virtual void NotifySelectionChanged();
	
	/** \brief Notify listeners item selected. */
	virtual void NotifyItemSelected(int index);
	
	/** \brief Notify listeners item deselected. */
	virtual void NotifyItemDeselected(int index);
	
	/** \brief Notify listeners double clicked on item. */
	virtual void NotifyDoubleClickItem(int index);
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
	virtual void OnItemAdded(int index);
	
	/** \brief Item removed. */
	virtual void OnItemRemoved(int index);
	
	/** \brief All items removed. */
	virtual void OnAllItemsRemoved();
	
	/** \brief Item changed. */
	virtual void OnItemChanged(int index);
	
	/** \brief Item moved. */
	virtual void OnItemMoved(int fromIndex, int toIndex);
	
	/** \brief Items sorted. */
	virtual void OnItemsSorted();
	
	/** \brief Selection changed. */
	virtual void OnSelectionChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Rows changed. */
	virtual void OnRowsChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Selection mode changed. */
	virtual void OnSelectionModeChanged();
	/*@}*/
};

#endif
