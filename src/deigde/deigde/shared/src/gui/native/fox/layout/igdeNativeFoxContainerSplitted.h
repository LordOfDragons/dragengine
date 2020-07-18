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

#ifndef _IGDENATIVEFOXCONTAINERSPLITTED_H_
#define _IGDENATIVEFOXCONTAINERSPLITTED_H_

#include "../foxtoolkit.h"

class igdeContainerSplitted;


/**
 * FOX Native button.
 */
class igdeNativeFoxContainerSplitted : public FXSplitter{
	FXDECLARE( igdeNativeFoxContainerSplitted )
	
protected:
	igdeNativeFoxContainerSplitted();
	
public:
	enum eFoxIDs{
		ID_SELF = FXButton::ID_LAST,
	};
	
private:
	igdeContainerSplitted *pOwner;
	bool pNoUpdate;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxContainerSplitted( igdeContainerSplitted &owner, FXComposite *parent, int layoutFlags );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxContainerSplitted();
	
	/** \brief Create native widget. */
	static igdeNativeFoxContainerSplitted* CreateNativeWidget( igdeContainerSplitted &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateSplitValue();
	
	static int SplitterFlags( const igdeContainerSplitted &owner );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onResize( FXObject *sender, FXSelector selector, void *data );
	long onCommand( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

typedef igdeNativeFoxContainerSplitted igdeNativeContainerSplitted;

#endif
