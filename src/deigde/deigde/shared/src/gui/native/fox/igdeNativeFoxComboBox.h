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

#ifndef _IGDENATIVEFOXCOMBOBOX_H_
#define _IGDENATIVEFOXCOMBOBOX_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeComboBox;
class igdeEnvironment;
class igdeGuiTheme;
class igdeListItem;
class igdeWidget;


/**
 * FOX Native comboBox.
 */
class igdeNativeFoxComboBox : public FXComboBox{
	FXDECLARE( igdeNativeFoxComboBox )
	
protected:
	igdeNativeFoxComboBox();
	
public:
	enum eFoxIDs{
		ID_SELF = FXComboBox::ID_LAST,
	};
	
private:
	igdeComboBox *pOwner;
	igdeFontReference pFont;
	FXColor pOrgBackColor;
	FXColor pInvalidBackColor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxComboBox( igdeComboBox &owner, FXComposite *parent, int layoutFlags,
		const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxComboBox();
	
	/** \brief Create native widget. */
	static igdeNativeFoxComboBox* CreateNativeWidget( igdeComboBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void BuildList();
	virtual void UpdateItem( int index );
	virtual void SyncSelection( bool changing );
	virtual void OnInvalidValueChanged();
	virtual void UpdateText();
	virtual void InsertItem( int index, const igdeListItem &item );
	virtual void RemoveItem( int index );
	virtual void RemoveAllItems();
	virtual void MoveItem( int fromIndex, int toIndex );
	virtual void Focus();
	virtual void UpdateRowCount();
	virtual void UpdateEnabled();
	virtual void UpdateEditable();
	virtual void UpdateDescription();
	
	static int ComboBoxFlags( const igdeComboBox &owner );
	static igdeFont *ComboBoxFont( const igdeComboBox &owner, const igdeGuiTheme &guitheme );
	static int ComboBoxPadLeft( const igdeGuiTheme &guitheme );
	static int ComboBoxPadRight( const igdeGuiTheme &guitheme );
	static int ComboBoxPadTop( const igdeGuiTheme &guitheme );
	static int ComboBoxPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand( FXObject*, FXSelector, void* );
	long onChanged( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxComboBox igdeNativeComboBox;

#endif
