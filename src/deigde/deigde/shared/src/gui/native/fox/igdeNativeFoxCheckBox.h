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

#ifndef _IGDENATIVEFOXCHECKBOX_H_
#define _IGDENATIVEFOXCHECKBOX_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

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
	igdeFontReference pFont;
	
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
