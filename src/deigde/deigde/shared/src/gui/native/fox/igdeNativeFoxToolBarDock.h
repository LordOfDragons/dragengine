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

#ifndef _IGDENATIVEFOXTOOLBARDOCK_H_
#define _IGDENATIVEFOXTOOLBARDOCK_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeToolBarDock;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native toolBarDock.
 */
class igdeNativeFoxToolBarDock : public FXDockSite{
	FXDECLARE( igdeNativeFoxToolBarDock )
	
protected:
	igdeNativeFoxToolBarDock();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDockSite::ID_LAST,
	};
	
private:
	igdeToolBarDock *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxToolBarDock( igdeToolBarDock &owner, FXComposite *parent );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxToolBarDock();
	
	/** \brief Create native widget. */
	static igdeNativeFoxToolBarDock* CreateNativeWidget( igdeToolBarDock &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	static int LayoutFlags( const igdeToolBarDock &owner );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxToolBarDock igdeNativeToolBarDock;

#endif
