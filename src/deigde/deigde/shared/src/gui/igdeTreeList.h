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

#ifndef _IGDETREELIST_H_
#define _IGDETREELIST_H_

#include <stddef.h>

#include "igdeWidget.h"
#include "event/igdeTreeListListener.h"
#include "model/igdeTreeItem.h"
#include "model/igdeTreeItemSorter.h"

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>

class igdeIcon;
class igdeTreeItem;

/**
 * \brief IGDE UI TreeList.
 */
class DE_DLL_EXPORT igdeTreeList : public igdeWidget{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTreeList> Ref;
	
	
private:
	bool pEnabled;
	igdeTreeItem::Ref pFirstChild;
	igdeTreeItem::Ref pSelection;
	igdeTreeItemSorter::Ref pSorter;
	int pRows;
	decString pDescription;
	
	decTObjectOrderedSet<igdeTreeListListener> pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listbox. */
	igdeTreeList(igdeEnvironment &environment, int rows, const char *description = "");
	
	
	
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
	void SetEnabled(bool enabled);
	
	/** \brief Visible rows. */
	inline int GetRows() const{ return pRows; }
	
	/** \brief Set visible rows. */
	void SetRows(int rows);
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription(const char *description);
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief First child item. */
	inline const igdeTreeItem::Ref &GetFirstChild() const{ return pFirstChild; }
	
	/** \brief Last child item. */
	igdeTreeItem *GetLastChild() const;
	
	/** \brief Number of children. */
	int GetChildrenCount() const;
	
	/** \brief Item with data or nullptr if not found. */
	igdeTreeItem *GetItemWithData(void *data) const;
	
	/** \brief Item with data starting at parent or nullptr if not found. */
	igdeTreeItem *GetItemWithData(igdeTreeItem *parent, void *data) const;
	
	/** \brief Item is present. */
	bool HasItem(igdeTreeItem *item) const;
	
	/** \brief Item with text is present. */
	bool HasItem(const char *item) const;
	
	/** \brief Item with data is present. */
	bool HasItemWithData(void *data) const;
	
	/** \brief Add item as last child of parent. */
	void AppendItem(igdeTreeItem *parent, igdeTreeItem *item);
	
	/** \brief Add item of type igdeTreeItem with text as last child of parent. */
	igdeTreeItem *AppendItem(igdeTreeItem *parent, const char *text,
		igdeIcon *icon = nullptr, void *data = nullptr);
	
	void AppendItem(igdeTreeItem *parent, igdeTreeItem::Ref &item, const char *text,
		igdeIcon *icon = nullptr, void *data = nullptr);
	
	/** \brief Insert item before another. */
	void InsertItemBefore(igdeTreeItem *beforeItem, igdeTreeItem *item);
	
	/** \brief Insert item of type igdeTreeItem with text at index. */
	igdeTreeItem *InsertItemBefore(igdeTreeItem *beforeItem, const char *text,
		igdeIcon *icon = nullptr, void *data = nullptr);
	
	void InsertItemBefore(igdeTreeItem *beforeItem, igdeTreeItem::Ref &item,
		const char *text, igdeIcon *icon = nullptr, void *data = nullptr);
	
	/** \brief Insert item after another. */
	void InsertItemAfter(igdeTreeItem *afterItem, igdeTreeItem *item);
	
	/** \brief Insert item of type igdeTreeItem with text at index. */
	igdeTreeItem *InsertItemAfter(igdeTreeItem *afterItem, const char *text,
		igdeIcon *icon = nullptr, void *data = nullptr);
	
	void InsertItemAfter(igdeTreeItem *afterItem, igdeTreeItem::Ref &item,
		const char *text, igdeIcon *icon = nullptr, void *data = nullptr);
	
	/** \brief Move item before another. */
	void MoveItemBefore(igdeTreeItem *item, igdeTreeItem *beforeItem);
	
	/** \brief Move item after another. */
	void MoveItemAfter(igdeTreeItem *item, igdeTreeItem *afterItem);
	
	/** \brief Remove item from index. */
	void RemoveItem(igdeTreeItem *item);
	
	/** \brief Remove all items. */
	void RemoveAllItems();
	
	/** \brief Remove all items underneath parent. */
	void RemoveAllItems(igdeTreeItem *parent);
	
	/** \brief Item modified. */
	void ItemChanged(igdeTreeItem *item);
	
	
	
	/** \brief Sorter or nullptr. */
	inline const igdeTreeItemSorter::Ref &GetSorter() const{ return pSorter; }
	
	/** \brief Set sorter or nullptr. */
	void SetSorter(igdeTreeItemSorter *sorter);
	
	/** \brief Set default sorter sorting items ascending lexicographically by their text. */
	void SetDefaultSorter();
	
	/** \brief Sort children of item using sorter if present. */
	void SortItems(igdeTreeItem *item);
	
	/** \brief Sort all items using sorter if present. */
	void SortAllItems();
	
	
	
	/** \brief Selected item or nullptr. */
	inline const igdeTreeItem::Ref &GetSelection() const{ return pSelection; }
	
	/** \brief Set selected item or nullptr. */
	void SetSelection(igdeTreeItem *selection);
	
	/**
	 * \brief Set selected item matching data.
	 * 
	 * Short-cut for calling SetSelection(IndexOfItemWithData(data)).
	 */
	void SetSelectionWithData(void *data);
	
	/** \brief Make item visible. */
	void MakeItemVisible(igdeTreeItem *item);
	
	/** \brief Make selection visible if any selection is present. */
	void MakeSelectionVisible();
	
	/** \brief Show context menu at position. */
	void ShowContextMenu(const decPoint &position);
	
	
	
	/** \brief Add listener. */
	void AddListener(igdeTreeListListener *listener);
	
	/** \brief Remove listener. */
	void RemoveListener(igdeTreeListListener *listener);
	
	/** \brief Notify listeners selection changed. */
	virtual void NotifySelectionChanged();
	
	/** \brief Notify listeners item expanded. */
	virtual void NotifyItemExpanded(igdeTreeItem *item);
	
	/** \brief Notify listeners item collapsed. */
	virtual void NotifyItemCollapsed(igdeTreeItem *item);
	
	/** \brief Notify listeners double clicked on item. */
	virtual void NotifyDoubleClickItem(igdeTreeItem *item);
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
	virtual void OnItemAdded(igdeTreeItem *item);
	
	/** \brief Item removed. */
	virtual void OnItemRemoved(igdeTreeItem *item);
	
	/** \brief All items removed. */
	virtual void OnAllItemsRemoved(igdeTreeItem *parent);
	
	/** \brief Item changed. */
	virtual void OnItemChanged(igdeTreeItem *item);
	
	/** \brief Item moved. */
	virtual void OnItemMoved(igdeTreeItem *item);
	
	/** \brief Items sorted. */
	virtual void OnItemsSorted(igdeTreeItem *item);
	
	/** \brief Selection changed. */
	virtual void OnSelectionChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Rows changed. */
	virtual void OnRowsChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Item with data. */
	igdeTreeItem *pGetItemWithData(igdeTreeItem *parent, void *data) const;
	
	/** \brief Item is present. */
	bool pHasItem(igdeTreeItem *parent, igdeTreeItem *item) const;
	
	/** \brief Item with text is present. */
	bool pHasItem(igdeTreeItem *parent, const char *text) const;
	
	/** \brief Item with data is present. */
	bool pHasItem(igdeTreeItem *parent, void *data) const;
	
	/** \brief Remove item. */
	void pRemoveItem(igdeTreeItem *item);
	
	/** \brief Append item. */
	void pAppendItem(igdeTreeItem *parent, igdeTreeItem *item);
	
	/** \brief Insert item before. */
	void pInsertItemBefore(igdeTreeItem *item, igdeTreeItem *beforeItem);
	
	/** \brief Insert item after. */
	void pInsertItemAfter(igdeTreeItem *item, igdeTreeItem *afterItem);
	
	/** \brief Remove all items. */
	void pRemoveAllItems(igdeTreeItem *item);
	/*@}*/
};

#endif
