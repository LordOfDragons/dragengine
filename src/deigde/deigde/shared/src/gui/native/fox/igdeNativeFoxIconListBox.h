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
	
	/** \brief Create native widget. */
	static igdeNativeFoxIconListBox* CreateNativeWidget( igdeIconListBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
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
	virtual void UpdateMinimumSize();
	virtual void Focus();
	virtual void MakeItemVisible( int index );
	virtual void RemoveAllItems();
	
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
	long onListCommand( FXObject*, FXSelector, void* );
	long onListChanged( FXObject*, FXSelector, void* );
	long onListSelected( FXObject*, FXSelector, void* );
	long onListDeselected( FXObject*, FXSelector, void* );
	long onListHeaderChanged( FXObject*, FXSelector, void* );
	long onListHeaderClicked( FXObject*, FXSelector, void* );
	long onListRightMouseDown( FXObject*, FXSelector, void* );
	long onListRightMouseUp( FXObject*, FXSelector, void* );
	long onListDoubleClicked( FXObject*, FXSelector, void* );
	
	long onResizerDrag( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxIconListBox igdeNativeIconListBox;

#endif
