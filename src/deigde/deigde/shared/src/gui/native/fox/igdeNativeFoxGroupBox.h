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

#ifndef _IGDENATIVEFOXGROUPBOX_H_
#define _IGDENATIVEFOXGROUPBOX_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeGroupBox;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native groupBox.
 */
class igdeNativeFoxGroupBox : public FXGroupBox{
	FXDECLARE( igdeNativeFoxGroupBox )
	
protected:
	igdeNativeFoxGroupBox();
	
public:
	enum eFoxIDs{
		ID_SELF = FXGroupBox::ID_LAST,
	};
	
private:
	igdeGroupBox *pOwner;
	int pUncollapsedFlags;
	int pCollapsedFlags;
	igdeFontReference pFont;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxGroupBox( igdeGroupBox &owner, FXComposite *parent, int layoutFlags,
		const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxGroupBox();
	
	/** \brief Create native widget. */
	static igdeNativeFoxGroupBox* CreateNativeWidget( igdeGroupBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXint getDefaultHeight();
	
	virtual void UpdateCollapsed();
	virtual void UpdateTitle();
	virtual void UpdateTitleAlignment();
	virtual void UpdateStretchLast();
	
	static int GroupBoxFlags( const igdeGroupBox &owner );
	static igdeFont *GroupBoxFont( const igdeGroupBox &owner, const igdeGuiTheme &guitheme );
	static int GroupBoxPadLeft( const igdeGuiTheme &guitheme );
	static int GroupBoxPadRight( const igdeGuiTheme &guitheme );
	static int GroupBoxPadTop( const igdeGuiTheme &guitheme );
	static int GroupBoxPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onLeftMouseDown( FXObject*, FXSelector, void* );
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxGroupBox igdeNativeGroupBox;

#endif
