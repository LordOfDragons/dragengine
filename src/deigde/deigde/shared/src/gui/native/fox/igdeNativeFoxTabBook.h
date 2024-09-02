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
