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

#ifndef _IGDENATIVEFOXSCROLLBAR_H_
#define _IGDENATIVEFOXSCROLLBAR_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFont::Ref.h"

class igdeScrollBar;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native scroll bar.
 */
class igdeNativeFoxScrollBar : public FXScrollBar{
	FXDECLARE( igdeNativeFoxScrollBar )
	
protected:
	igdeNativeFoxScrollBar();
	
public:
	enum eFoxIDs{
		ID_SELF = FXScrollBar::ID_LAST,
	};
	
private:
	igdeScrollBar *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxScrollBar( igdeScrollBar &owner, FXComposite *parent, int layoutFlags );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxScrollBar();
	
	/** \brief Create native widget. */
	static igdeNativeFoxScrollBar* CreateNativeWidget( igdeScrollBar &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXbool canFocus() const;
	
	virtual void UpdateRange();
	virtual void UpdateValue();
	virtual void UpdateEnabled();
	
	static int ScrollBarFlags( const igdeScrollBar &owner );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxScrollBar igdeNativeScrollBar;

#endif
