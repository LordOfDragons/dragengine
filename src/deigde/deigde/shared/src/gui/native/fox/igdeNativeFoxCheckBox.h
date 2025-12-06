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

#ifndef _IGDENATIVEFOXCHECKBOX_H_
#define _IGDENATIVEFOXCHECKBOX_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFont.h"

class igdeCheckBox;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native button.
 */
class igdeNativeFoxCheckBox : public FXCheckButton{
	FXDECLARE( igdeNativeFoxCheckBox )
	
protected:
	igdeNativeFoxCheckBox();
	
public:
	enum eFoxIDs{
		ID_SELF = FXCheckButton::ID_LAST,
	};
	
private:
	igdeCheckBox *pOwner;
	igdeFont::Ref pFont;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxCheckBox( igdeCheckBox &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxCheckBox();
	
	/** \brief Create native widget. */
	static igdeNativeFoxCheckBox* CreateNativeWidget( igdeCheckBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXbool canFocus() const;
	
	virtual void UpdateChecked();
	virtual void UpdateStyle();
	virtual void UpdateText();
	virtual void UpdateDescription();
	virtual void UpdateIcon();
	virtual void UpdateEnabled();
	
	static const char *CheckBoxText( const igdeCheckBox &owner );
	static FXIcon *CheckBoxIcon( const igdeCheckBox &owner );
	static int CheckBoxFlags( const igdeCheckBox &owner );
	static igdeFont *CheckBoxFont( const igdeCheckBox &owner, const igdeGuiTheme &guitheme );
	static int CheckBoxPadLeft( const igdeGuiTheme &guitheme );
	static int CheckBoxPadRight( const igdeGuiTheme &guitheme );
	static int CheckBoxPadTop( const igdeGuiTheme &guitheme );
	static int CheckBoxPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand( FXObject*, FXSelector, void* );
	long onUpdate( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxCheckBox igdeNativeCheckBox;

#endif
