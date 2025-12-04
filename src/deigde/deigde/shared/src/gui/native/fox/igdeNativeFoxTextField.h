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

#ifndef _IGDENATIVEFOXTEXTFIELD_H_
#define _IGDENATIVEFOXTEXTFIELD_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFont.h"

class igdeTextField;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native textField.
 */
class igdeNativeFoxTextField : public FXTextField{
	FXDECLARE( igdeNativeFoxTextField )
	
protected:
	igdeNativeFoxTextField();
	
public:
	enum eFoxIDs{
		ID_SELF = FXTextField::ID_LAST,
	};
	
private:
	igdeTextField *pOwner;
	igdeFont::Ref pFont;
	FXColor pOrgBackColor;
	FXColor pInvalidBackColor;
	bool pPreventKeyPressHook;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxTextField( igdeTextField &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxTextField();
	
	/** \brief Create native widget. */
	static igdeNativeFoxTextField* CreateNativeWidget( igdeTextField &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void Focus();
	virtual void OnInvalidValueChanged();
	virtual void UpdateText();
	virtual void UpdateEnabled();
	virtual void UpdateDescription();
	virtual void UpdateEditable();
	virtual int GetCursorPosition() const;
	virtual void SetCursorPosition( int position );
	
	static int TextFieldFlags( const igdeTextField &owner );
	static igdeFont *TextFieldFont( const igdeTextField &owner, const igdeGuiTheme &guitheme );
	static int TextFieldPadLeft( const igdeGuiTheme &guitheme );
	static int TextFieldPadRight( const igdeGuiTheme &guitheme );
	static int TextFieldPadTop( const igdeGuiTheme &guitheme );
	static int TextFieldPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand( FXObject*, FXSelector, void* );
	long onChanged( FXObject*, FXSelector, void* );
	long onKeyPress( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxTextField igdeNativeTextField;

#endif
