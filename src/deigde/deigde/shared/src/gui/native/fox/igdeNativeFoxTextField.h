/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDENATIVEFOXTEXTFIELD_H_
#define _IGDENATIVEFOXTEXTFIELD_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

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
	igdeFontReference pFont;
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
