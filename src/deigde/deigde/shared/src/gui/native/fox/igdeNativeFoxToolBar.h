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

#ifndef _IGDENATIVEFOXTOOLBAR_H_
#define _IGDENATIVEFOXTOOLBAR_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeToolBar;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native toolBar.
 * 
 * This is done complicated here because of a peculiarity of the FOX toolkit. the toolbar
 * can be floating and docket. the floating window has to be delete always since we are
 * responsible for the reference. the toolbar we have to delete also *BUT ONLY* if not
 * parented to the floating window. this forces us to do some nasty book-keeping to
 * avoid crashes and other strange problems.
 * 
 * A solution jumping quick to mind is the floating window is declared the native widget
 * while the toolbar is declared the native container. this is not correct since the
 * toolbar widget should be the toolbar and not an imaginary window. this causes segfaults
 * while deleting since IGDE UI expects the native widget to be parented to the container
 * not an arbitrary other widget.
 * 
 * So no matter what the FXToolBar has to be the native widget. so the question is how
 * to solve the mess with the floating window. the biggest issue here is deleting the
 * toolbar widget and in the destructor being required to delete also the shell window.
 * 
 * The solution used is to move the deletion into a method of the native class. this
 * method has to delete the floating toolbar (which has to be done always) and if the
 * toolbar itself has to be deleted too.
 */
class igdeNativeFoxToolBar : public FXToolBar{
	FXDECLARE( igdeNativeFoxToolBar )
	
protected:
	igdeNativeFoxToolBar();
	
public:
	enum eFoxIDs{
		ID_SELF = FXToolBar::ID_LAST,
	};
	
private:
	igdeToolBar *pOwner;
	FXToolBarShell *pWetDock;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxToolBar( igdeToolBar &owner, FXComposite *parent,
		FXComposite *windowParent, int layoutFlags );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxToolBar();
	
	/** \brief Create native widget. */
	static igdeNativeFoxToolBar* CreateNativeWidget( igdeToolBar &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
};

typedef igdeNativeFoxToolBar igdeNativeToolBar;

#endif
