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

#ifndef _IGDENATIVEFOXMENUOPTION_H_
#define _IGDENATIVEFOXMENUOPTION_H_

#include "../foxtoolkit.h"

class igdeMenuOption;


/**
 * FOX native widget.
 */
class igdeNativeFoxMenuOption : public FXMenuRadio{
	FXDECLARE( igdeNativeFoxMenuOption )
	
protected:
	igdeNativeFoxMenuOption();
	
public:
	enum eFoxIDs{
		ID_SELF = FXMenuRadio::ID_LAST,
	};
	
private:
	igdeMenuOption *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxMenuOption( igdeMenuOption &owner, FXComposite *parent );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxMenuOption();
	
	/** \brief Create native widget. */
	static igdeNativeFoxMenuOption* CreateNativeWidget( igdeMenuOption &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateSelected();
	
	static FXString BuildConstrText( igdeMenuOption &owner );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onMenuAction( FXObject*, FXSelector, void* );
	long updateMenuAction( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxMenuOption igdeNativeMenuOption;

#endif
