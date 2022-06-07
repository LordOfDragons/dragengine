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

#ifndef _IGDENATIVEFOXLISTBOX_H_
#define _IGDENATIVEFOXLISTBOX_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"


class igdeListBox;
class igdeGuiTheme;
class igdeNativeFoxResizer;


/**
 * \brief FOX toolkit Native list box.
 */
class igdeNativeFoxListBox : public FXVerticalFrame{
	FXDECLARE( igdeNativeFoxListBox )
protected:
	   igdeNativeFoxListBox();
	
public:
	enum eFoxIDs{
		ID_LISTBOX = FXVerticalFrame::ID_LAST,
		ID_RESIZER,
		ID_LAST
	};
	
private:
	igdeListBox *pOwner;
	igdeFontReference pFont;
	FXList *pListBox;
	igdeNativeFoxResizer *pResizer;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxListBox( igdeListBox &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxListBox();
	
	/** \brief Create native widget. */
	static igdeNativeFoxListBox* CreateNativeWidget( igdeListBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List box. */
	inline FXList *GetListBox() const{ return pListBox; }
	
	/** \brief Resizer or \em NULL. */
	inline igdeNativeFoxResizer *GetResizser() const{ return pResizer; }
	
	
	
	virtual void BuildList();
	virtual void UpdateItem( int index );
	virtual void UpdateStyles();
	virtual void UpdateSelection();
	virtual void Focus();
	virtual void MakeItemVisible( int index );
	virtual void InsertItem( int index );
	virtual void RemoveItem( int index );
	virtual void RemoveAllItems();
	virtual void MoveItem( int fromIndex, int toIndex );
	virtual void UpdateEnabled();
	virtual void UpdateRowCount();
	virtual void UpdateDescription();
	
	static int ListBoxFlags( const igdeListBox &owner );
	static igdeFont *ListBoxFont( const igdeListBox &owner, const igdeGuiTheme &guitheme );
	static int ListBoxPadLeft( const igdeGuiTheme &guitheme );
	static int ListBoxPadRight( const igdeGuiTheme &guitheme );
	static int ListBoxPadTop( const igdeGuiTheme &guitheme );
	static int ListBoxPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onListCommand( FXObject*, FXSelector, void* );
	long onListChanged( FXObject*, FXSelector, void* );
	long onListSelected( FXObject*, FXSelector, void* );
	long onListDeselected( FXObject*, FXSelector, void* );
	long onListRightMouseDown( FXObject*, FXSelector, void* );
	long onListRightMouseUp( FXObject*, FXSelector, void* );
	long onListDoubleClicked( FXObject*, FXSelector, void* );
	
	long onResizerDrag( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxListBox igdeNativeListBox;

#endif
