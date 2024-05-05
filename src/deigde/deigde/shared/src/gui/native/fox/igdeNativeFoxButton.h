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

#ifndef _IGDENATIVEFOXBUTTON_H_
#define _IGDENATIVEFOXBUTTON_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeButton;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native button.
 */
class igdeNativeFoxButton : public FXButton{
	FXDECLARE( igdeNativeFoxButton )
	
protected:
	igdeNativeFoxButton();
	
public:
	enum eFoxIDs{
		ID_SELF = FXButton::ID_LAST,
	};
	
private:
	igdeButton *pOwner;
	igdeFontReference pFont;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxButton( igdeButton &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxButton();
	
	/** \brief Create native widget. */
	static igdeNativeFoxButton* CreateNativeWidget( igdeButton &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXbool canFocus() const;
	
	virtual void Focus();
	virtual void UpdateStyle();
	virtual void UpdateText();
	virtual void UpdateDescription();
	virtual void UpdateIcon();
	virtual void UpdateEnabled();
	
	static const char *ButtonText( const igdeButton &owner );
	static FXIcon *ButtonIcon( const igdeButton &owner );
	static int ButtonFlags( const igdeButton &owner );
	static igdeFont *ButtonFont( const igdeButton &owner, const igdeGuiTheme &guitheme );
	static int ButtonPadLeft( const igdeGuiTheme &guitheme );
	static int ButtonPadRight( const igdeGuiTheme &guitheme );
	static int ButtonPadTop( const igdeGuiTheme &guitheme );
	static int ButtonPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand( FXObject*, FXSelector, void* );
	long onUpdate( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxButton igdeNativeButton;

#endif
