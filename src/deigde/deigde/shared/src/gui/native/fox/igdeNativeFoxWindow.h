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

#ifndef _IGDENATIVEFOXWINDOW_H_
#define _IGDENATIVEFOXWINDOW_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFont.h"

class igdeWindow;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native window.
 */
class igdeNativeFoxWindow : public FXTopWindow{
	FXDECLARE(igdeNativeFoxWindow)
	
protected:
	igdeNativeFoxWindow();
	
public:
	enum eFoxIDs{
		ID_SELF = FXTopWindow::ID_LAST,
	};
	
private:
	igdeWindow *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxWindow(igdeWindow &owner, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxWindow();
	
	/** \brief Create native widget. */
	static igdeNativeFoxWindow* CreateNativeWidget(igdeWindow &owner);
	
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
	virtual void RaiseAndActivate();
	
	static int WindowFlags(const igdeWindow &owner);
	static int WindowPadLeft(const igdeGuiTheme &guitheme);
	static int WindowPadRight(const igdeGuiTheme &guitheme);
	static int WindowPadTop(const igdeGuiTheme &guitheme);
	static int WindowPadBottom(const igdeGuiTheme &guitheme);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onConfigure(FXObject*, FXSelector, void*);
	long onClose(FXObject*, FXSelector, void*);
	long onChildLayoutFlags(FXObject*, FXSelector, void*);
	/*@}*/
};

typedef igdeNativeFoxWindow igdeNativeWindow;

#endif
