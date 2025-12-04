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

#ifndef _IGDENATIVEFOXSPINTEXTFIELD_H_
#define _IGDENATIVEFOXSPINTEXTFIELD_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFont.h"

class igdeSpinTextField;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native spinTextField.
 */
class igdeNativeFoxSpinTextField : public FXSpinner{
	FXDECLARE( igdeNativeFoxSpinTextField )
	
protected:
	igdeNativeFoxSpinTextField();
	
public:
	enum eFoxIDs{
		ID_SELF = FXSpinner::ID_LAST,
	};
	
private:
	igdeSpinTextField *pOwner;
	igdeFont::Ref pFont;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxSpinTextField( igdeSpinTextField &owner, FXComposite *parent,
		int layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxSpinTextField();
	
	/** \brief Create native widget. */
	static igdeNativeFoxSpinTextField* CreateNativeWidget( igdeSpinTextField &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void Focus();
	virtual void UpdateRange();
	virtual void UpdateEnabled();
	virtual void UpdateValue();
	virtual void UpdateDescription();
	
	static int SpinTextFieldFlags( const igdeSpinTextField &owner );
	static igdeFont *SpinTextFieldFont( const igdeSpinTextField &owner, const igdeGuiTheme &guitheme );
	static int SpinTextFieldPadLeft( const igdeGuiTheme &guitheme );
	static int SpinTextFieldPadRight( const igdeGuiTheme &guitheme );
	static int SpinTextFieldPadTop( const igdeGuiTheme &guitheme );
	static int SpinTextFieldPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxSpinTextField igdeNativeSpinTextField;

#endif
