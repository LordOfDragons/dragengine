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

#ifndef _IGDENATIVEFOXSWITCHER_H_
#define _IGDENATIVEFOXSWITCHER_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeSwitcher;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native switcher.
 */
class igdeNativeFoxSwitcher : public FXSwitcher{
	FXDECLARE( igdeNativeFoxSwitcher )
	
protected:
	igdeNativeFoxSwitcher();
	
public:
	enum eFoxIDs{
		ID_SELF = FXSwitcher::ID_LAST,
	};
	
private:
	igdeSwitcher *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxSwitcher( igdeSwitcher &owner, FXComposite *parent, int layoutFlags );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxSwitcher();
	
	/** \brief Create native widget. */
	static igdeNativeFoxSwitcher* CreateNativeWidget( igdeSwitcher &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateCurrent();
	/*@}*/
};

typedef igdeNativeFoxSwitcher igdeNativeSwitcher;

#endif
