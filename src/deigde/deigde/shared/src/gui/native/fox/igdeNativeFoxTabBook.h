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

#ifndef _IGDENATIVEFOXTABBOOK_H_
#define _IGDENATIVEFOXTABBOOK_H_

#include "foxtoolkit.h"
#include "../../igdeTabBook.h"
#include "../../resources/igdeFontReference.h"

class igdeTabBook;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;
class igdeNativeFoxAutoScroller;


/**
 * FOX Native tabBook.
 */
class igdeNativeFoxTabBook : public FXVerticalFrame{
	FXDECLARE( igdeNativeFoxTabBook )
	
protected:
	igdeNativeFoxTabBook();
	
public:
	enum eFoxIDs{
		ID_SELF = FXVerticalFrame::ID_LAST,
		ID_HEADER,
		ID_AUTO_SCROLLER,
// 		ID_BTN_SCROLL_HEADER_LEFT,
// 		ID_BTN_SCROLL_HEADER_RIGHT
	};
	
private:
	igdeTabBook *pOwner;
// 	FXArrowButton *pBtnScrollHeadersLeft;
// 	FXArrowButton *pBtnScrollHeadersRight;
	igdeNativeFoxAutoScroller *pAutoScroller;
	FXHorizontalFrame *pHeaders;
	FXSwitcher *pSwitcher;
	igdeFontReference pFont;
	int pPadLeft;
	int pPadRight;
	int pPadTop;
	int pPadBottom;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxTabBook( igdeTabBook &owner, FXComposite *parent, FXComposite *windowParent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxTabBook();
	
	/** \brief Create native widget. */
	static igdeNativeFoxTabBook* CreateNativeWidget( igdeTabBook &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline FXSwitcher *GetSwitcher() const{ return pSwitcher; }
	
	virtual void AddHeader( const igdeTabBook::cHeader &header );
	virtual void RemoveHeader( int index );
	virtual void RemoveAllHeaders();
	virtual void ChangePanel( int index );
	virtual void *GetNativeContainer();
	
	static igdeFont *TabBookFont( const igdeTabBook &owner, const igdeGuiTheme &guitheme );
	static int TabBookPadLeft( const igdeGuiTheme &guitheme );
	static int TabBookPadRight( const igdeGuiTheme &guitheme );
	static int TabBookPadTop( const igdeGuiTheme &guitheme );
	static int TabBookPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onHeaderCommand( FXObject*, FXSelector, void* );
	long onHeaderUpdate( FXObject*, FXSelector, void* );
	long onHeaderMouseWheel( FXObject*, FXSelector, void* );
	long onHeaderMouseMoved( FXObject*, FXSelector, void* );
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxTabBook igdeNativeTabBook;

#endif
