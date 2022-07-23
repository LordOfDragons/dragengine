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

#ifndef _IGDENATIVEFOXSTATUSBAR_H_
#define _IGDENATIVEFOXSTATUSBAR_H_

#include "foxtoolkit.h"


class igdeStatusBar;
class igdeGuiTheme;
class igdeFont;


/**
 * \brief FOX toolkit Native Text Area.
 */
class igdeNativeFoxStatusBar : public FXStatusBar{
	FXDECLARE( igdeNativeFoxStatusBar )
protected:
	   igdeNativeFoxStatusBar();
	
public:
	enum eFoxIDs{
		ID_SELF = FXStatusBar::ID_LAST,
		ID_LAST
	};
	
private:
	igdeStatusBar *pOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text widget. */
	igdeNativeFoxStatusBar( igdeStatusBar &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up text widget. */
	virtual ~igdeNativeFoxStatusBar();
	
	/** \brief Create native widget. */
	static igdeNativeFoxStatusBar* CreateNativeWidget( igdeStatusBar &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateText();
	
	static int StatusBarFlags( const igdeStatusBar &owner );
	static igdeFont *StatusBarFont( const igdeStatusBar &owner, const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onResize( FXObject*, FXSelector, void* );
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxStatusBar igdeNativeStatusBar;

#endif
