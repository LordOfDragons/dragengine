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

#ifndef _IGDECOMBOBOXFILTER_H_
#define _IGDECOMBOBOXFILTER_H_

#include "igdeComboBox.h"



/**
 * \brief Combo box with filtering support.
 * 
 * igdeComboBox extended with the ability to filter the list entries. For this an additional
 * igdeTextField is appended to the bottom of the pop up list enabling the user to enter a
 * filter string. The combo box list is altered to display only entries that contain the
 * filter string. By default the filtered combo box behaves like a conventional combo box.
 * To achieve the filtering behavior first fill the combo box with all the required entries
 * and optionally sorting them. Then call StoreFilterItems() to create a copy of the list to
 * be used as the original entries. This list contains references to the original igdeListItem
 * and not real copies of them. As soon as the filter string is changed the combo box content
 * is replaced with the entries from the copied list matching the filter string. If no copy
 * exists no filtering is done.
 */
class DE_DLL_EXPORT igdeComboBoxFilter : public igdeComboBox{
private:
	decObjectList pFilterItems;
	bool pFilterCaseInsensitive;
	decString pFilterString;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create combobox. */
	igdeComboBoxFilter( igdeEnvironment &environment, int columns, const char *description = "" );
	
	igdeComboBoxFilter( igdeEnvironment &environment, int columns, bool editable,
		const char *description = "" );
	
	igdeComboBoxFilter( igdeEnvironment &environment, int columns, int rows,
		const char *description = "" );
	
	igdeComboBoxFilter( igdeEnvironment &environment, int columns, int rows, bool editable,
		const char *description = "" );
	
	
	
protected:
	/** \brief Clean up filtered combo box. */
	virtual ~igdeComboBoxFilter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Store items for filtering. */
	void StoreFilterItems();
	
	/** \brief Release filter items. */
	void ReleaseFilterItems();
	
	/** \brief Replace items with filter items matching filter string. */
	void FilterItems();
	
	/** \brief Filter case insensitive. */
	inline bool GetFilterCaseInsensitive() const{ return pFilterCaseInsensitive; }
	
	/** \brief Set if filtering is case insensitive. */
	void SetFilterCaseInsentive( bool caseInsensitive );
	
	/** \brief Filter string. */
	inline const decString &GetFilterString() const{ return pFilterString; }
	
	/**
	 * \brief Set filter string.
	 * 
	 * Calls FilterItems() if the filter string changed.
	 */
	void SetFilterString( const char *filterString );
	
	/** \brief Number of stored filter items. */
	int GetFilterItemCount() const;
	
	/** \brief Stored filter item at index. */
	igdeListItem *GetFilterItemAt( int index ) const;
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
	
	/** \brief Filter string changed. */
	virtual void OnFilterStringChanged();
	
	/** \brief Invalid value changed. */
	virtual void OnInvalidValueChanged();
	
	/** \brief Request focus. */
	virtual void OnRequestFocus();
	/*@}*/
};

#endif
