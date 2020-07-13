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

#ifndef _IGDENATIVEFOXMENUCOMMAND_H_
#define _IGDENATIVEFOXMENUCOMMAND_H_

#include "../foxtoolkit.h"

class igdeMenuCommand;


/**
 * FOX native widget.
 */
class igdeNativeFoxMenuCommand : public FXMenuCommand{
	FXDECLARE( igdeNativeFoxMenuCommand )
	
protected:
	igdeNativeFoxMenuCommand();
	
public:
	enum eFoxIDs{
		ID_SELF = FXMenuCommand::ID_LAST,
	};
	
private:
	igdeMenuCommand *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxMenuCommand( igdeMenuCommand &owner, FXComposite *parent );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxMenuCommand();
	
	/** \brief Create native widget. */
	static igdeNativeFoxMenuCommand* CreateNativeWidget( igdeMenuCommand &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateText();
	virtual void UpdateDescription();
	virtual void UpdateHotKey();
	virtual void UpdateIcon();
	virtual void UpdateEnabled();
	
	static FXString BuildConstrText( igdeMenuCommand &owner );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	long onUpdate( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

typedef igdeNativeFoxMenuCommand igdeNativeMenuCommand;

#endif
