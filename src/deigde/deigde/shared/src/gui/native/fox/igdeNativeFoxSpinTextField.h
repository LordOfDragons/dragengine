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

#ifndef _IGDENATIVEFOXSPINTEXTFIELD_H_
#define _IGDENATIVEFOXSPINTEXTFIELD_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

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
	igdeFontReference pFont;
	
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
