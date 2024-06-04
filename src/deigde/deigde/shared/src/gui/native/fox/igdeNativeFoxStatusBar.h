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

#ifndef _IGDENATIVEFOXSTATUSBAR_H_
#define _IGDENATIVEFOXSTATUSBAR_H_

#include "foxtoolkit.h"


class igdeStatusBar;
class igdeGuiTheme;
class igdeFont;


/**
 * \brief FOX toolkit Native Text Area.
 */
class igdeNativeFoxStatusBar : public FXStatusBar{
	FXDECLARE( igdeNativeFoxStatusBar )
protected:
	   igdeNativeFoxStatusBar();
	
public:
	enum eFoxIDs{
		ID_SELF = FXStatusBar::ID_LAST,
		ID_LAST
	};
	
private:
	igdeStatusBar *pOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text widget. */
	igdeNativeFoxStatusBar( igdeStatusBar &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up text widget. */
	virtual ~igdeNativeFoxStatusBar();
	
	/** \brief Create native widget. */
	static igdeNativeFoxStatusBar* CreateNativeWidget( igdeStatusBar &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateText();
	
	static int StatusBarFlags( const igdeStatusBar &owner );
	static igdeFont *StatusBarFont( const igdeStatusBar &owner, const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onResize( FXObject*, FXSelector, void* );
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxStatusBar igdeNativeStatusBar;

#endif
