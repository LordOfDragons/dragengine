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

#ifndef _IGDENATIVEFOXDIALOG_H_
#define _IGDENATIVEFOXDIALOG_H_

#include "../foxtoolkit.h"

class igdeDialog;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native dialog.
 */
class igdeNativeFoxDialog : public FXDialogBox{
	FXDECLARE( igdeNativeFoxDialog )
	
protected:
	igdeNativeFoxDialog();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDialogBox::ID_LAST,
	};
	
private:
	igdeDialog *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxDialog( igdeDialog &owner, FXWindow *parent, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxDialog();
	
	/** \brief Create native widget. */
	static igdeNativeFoxDialog* CreateNativeWidget( igdeDialog &owner, igdeWidget *ownerOwner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateEnabled();
	virtual void UpdatePosition();
	virtual void UpdateIcon();
	virtual void UpdateTitle();
	virtual void UpdateSize();
	virtual void ShowDialog();
	
	virtual void CloseDialog( bool accepted );
	
	static int DialogFlags( const igdeDialog &owner );
	
	static int DialogPadLeft( const igdeGuiTheme &guitheme );
	static int DialogPadRight( const igdeGuiTheme &guitheme );
	static int DialogPadTop( const igdeGuiTheme &guitheme );
	static int DialogPadBottom( const igdeGuiTheme &guitheme );
	static int DialogPadContent( const igdeGuiTheme &guitheme );
	static int DialogPadButtons( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onClose( FXObject*, FXSelector, void* );
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	long onFrameUpdate( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxDialog igdeNativeDialog;

#endif
