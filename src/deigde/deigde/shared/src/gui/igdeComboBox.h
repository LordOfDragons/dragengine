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

#ifndef _IGDECOMBOBOX_H_
#define _IGDECOMBOBOX_H_

#include <stddef.h>

#include "igdeWidget.h"
#include "model/igdeListItemSorter.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>


class igdeComboBoxListener;
class igdeIcon;
class igdeListItem;


/**
 * \brief IGDE UI ComboBox.
 * 
 * \todo igdeComboBox has to be a subclass of igdeTextField once done.
 */
class DE_DLL_EXPORT igdeComboBox : public igdeWidget{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeComboBox> Ref;
	
	
private:
	bool pEnabled;
	decObjectList pItems;
	int pSelection;
	decString pText;
	igdeListItemSorter::Ref pSorter;
	int pColumns;
	int pRows;
	bool pEditable;
	decString pDescription;
	bool pInvalidValue;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create combobox. */
	igdeComboBox(igdeEnvironment &environment, int columns, const char *description = "");
	
	igdeComboBox(igdeEnvironment &environment, int columns, bool editable,
		const char *description = "");
	
	igdeComboBox(igdeEnvironment &environment, int columns, int rows,
		const char *description = "");
	
	igdeComboBox(igdeEnvironment &environment, int columns, int rows, bool editable,
		const char *description = "");
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeComboBox();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief ComboBox is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Visible columns in edit field. */
	inline int GetColumns() const{ return pColumns; }
	
	/** \brief Visible rows in drop down list. */
	inline int GetRows() const{ return pRows; }
	
	/** \brief Set number of visible rows in drop down list. */
	void SetRows(int rows);
	
	/** \brief Text is editable. */
	inline bool GetEditable() const{ return pEditable; }
	
	/** \brief Set if text is editable. */
	void SetEditable(bool editable);
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription(const char *description);
	
	/** \brief Mark widget as having an invalue value. */
	inline bool GetInvalidValue() const{ return pInvalidValue; }
	
	/** \brief Set to mark widget as having an invalid value. */
	void SetInvalidValue(bool invalidValue);
	
	/** \brief Focus widget. */
	void Focus();
	
	
	
	/** \brief Number of items. */
	int GetItemCount() const;
	
	/** \brief Item at index. */
	igdeListItem *GetItemAt(int index) const;
	
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
	igdeListItem *AddItem(const char *text, igdeIcon *icon = NULL, void *data = NULL);
	
	/** \brief Insert item at index. */
	void InsertItem(int index, igdeListItem *item);
	
	/** \brief Insert item of type igdeListItem with text at index. */
	igdeListItem *InsertItem(int index, const char *text, igdeIcon *icon = NULL, void *data = NULL);
	
	/** \brief Move item. */
	void MoveItem(int fromIndex, int toIndex);
	
	/** \brief Remove item from index. */
	void RemoveItem(int index);
	
	/** \brief Remove all items. */
	void RemoveAllItems();
	
	/** \brief Item modified. */
	void ItemChangedAt(int index);
	
	
	
	/** \brief Sorter or NULL. */
	inline igdeListItemSorter *GetSorter() const{ return pSorter; }
	
	/** \brief Set sorter or NULL. */
	void SetSorter(igdeListItemSorter *sorter);
	
	/** \brief Set default sorter sorting items ascending lexicographically by their text. */
	void SetDefaultSorter();
	
	/** \brief Sort items using sorter if present. */
	void SortItems();
	
	
	
	/** \brief Index of selected item or -1. */
	inline int GetSelection() const{ return pSelection; }
	
	/** \brief Selected item or nullptr. */
	igdeListItem *GetSelectedItem() const;
	
	/** \brief Selected item date or nullptr. */
	void *GetSelectedItemData() const;
	
	/** \brief Set index of selected item or -1. */
	void SetSelection(int selection);
	
	/**
	 * \brief Set selected item matching data.
	 * 
	 * Short-cut for calling SetSelection(IndexOfItemWithData(data)).
	 */
	void SetSelectionWithData(void *data);
	
	
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText(const char *text, bool changing = false);
	
	/** \brief Clear text. */
	void ClearText();
	
	
	
	/** \brief Add listener. */
	void AddListener(igdeComboBoxListener *listener);
	
	/** \brief Remove listener. */
	void RemoveListener(igdeComboBoxListener *listener);
	
	/** \brief Notify listeners text changed. */
	virtual void NotifyTextChanged();
	
	/** \brief Notify listeners text is changing. */
	virtual void NotifyTextChanging();
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
	
	/** \brief Text changed. */
	virtual void OnTextChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Rows changed. */
	virtual void OnRowsChanged();
	
	/** \brief Editable changed. */
	virtual void OnEditableChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Invalid value changed. */
	virtual void OnInvalidValueChanged();
	
	/** \brief Request focus. */
	virtual void OnRequestFocus();
	/*@}*/
};

#endif
