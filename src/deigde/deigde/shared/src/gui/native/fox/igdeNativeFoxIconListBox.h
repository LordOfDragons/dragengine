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

#ifndef _IGDENATIVEFOXICONLISTBOX_H_
#define _IGDENATIVEFOXICONLISTBOX_H_

#include <dragengine/common/math/decMath.h>

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"


class igdeListItem;
class igdeIconListBox;
class igdeGuiTheme;
class igdeNativeFoxResizer;


/**
 * \brief FOX toolkit Native icon list box.
 */
class igdeNativeFoxIconListBox : public FXVerticalFrame{
	FXDECLARE( igdeNativeFoxIconListBox )
protected:
	   igdeNativeFoxIconListBox();
	
public:
	enum eFoxIDs{
		ID_LISTBOX = FXVerticalFrame::ID_LAST,
		ID_HEADER,
		ID_RESIZER,
		ID_LAST
	};
	
private:
	igdeIconListBox *pOwner;
	igdeFontReference pFont;
	FXIconList *pListBox;
	igdeNativeFoxResizer *pResizer;
	decPoint pDefaultSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxIconListBox( igdeIconListBox &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxIconListBox();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List box. */
	inline FXIconList *GetListBox() const{ return pListBox; }
	
	/** \brief Resizer or \em NULL. */
	inline igdeNativeFoxResizer *GetResizser() const{ return pResizer; }
	
	
	
	virtual void BuildHeader();
	virtual void BuildList();
	virtual void InsertItem( int index, igdeListItem &item );
	virtual void UpdateItem( int index );
	virtual void MoveItem( int fromIndex, int toIndex );
	virtual void RemoveItem( int index );
	virtual void UpdateSelection();
	virtual void UpdateHeader();
	virtual void UpdateStyles();
	virtual void UpdateDescription();
	virtual void UpdateEnabled();
	virtual void Focus();
	
	static int IconListBoxFlagsBorder( const igdeIconListBox &owner );
	static int IconListBoxFlags( const igdeIconListBox &owner );
	static igdeFont *IconListBoxFont( const igdeIconListBox &owner, const igdeGuiTheme &guitheme );
	static int IconListBoxPadLeft( const igdeGuiTheme &guitheme );
	static int IconListBoxPadRight( const igdeGuiTheme &guitheme );
	static int IconListBoxPadTop( const igdeGuiTheme &guitheme );
	static int IconListBoxPadBottom( const igdeGuiTheme &guitheme );
	
	virtual FXint getDefaultWidth();
	virtual FXint getDefaultHeight();
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onListCommand( FXObject *sender, FXSelector selector, void *data );
	long onListChanged( FXObject *sender, FXSelector selector, void *data );
	long onListSelected( FXObject *sender, FXSelector selector, void *data );
	long onListDeselected( FXObject *sender, FXSelector selector, void *data );
	long onListHeaderChanged( FXObject *sender, FXSelector selector, void *data );
	long onListHeaderClicked( FXObject *sender, FXSelector selector, void *data );
	long onListRightMouseDown( FXObject *sender, FXSelector selector, void *data );
	long onListRightMouseUp( FXObject *sender, FXSelector selector, void *data );
	long onListDoubleClicked( FXObject *sender, FXSelector selector, void *data );
	
	long onResizerDrag( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

#endif
