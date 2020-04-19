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
class igdeComboBoxFilter : public igdeComboBox{
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
