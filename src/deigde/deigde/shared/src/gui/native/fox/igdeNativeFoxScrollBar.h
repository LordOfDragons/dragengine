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

#ifndef _IGDENATIVEFOXSCROLLBAR_H_
#define _IGDENATIVEFOXSCROLLBAR_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeScrollBar;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native scroll bar.
 */
class igdeNativeFoxScrollBar : public FXScrollBar{
	FXDECLARE( igdeNativeFoxScrollBar )
	
protected:
	igdeNativeFoxScrollBar();
	
public:
	enum eFoxIDs{
		ID_SELF = FXScrollBar::ID_LAST,
	};
	
private:
	igdeScrollBar *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxScrollBar( igdeScrollBar &owner, FXComposite *parent, int layoutFlags );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxScrollBar();
	
	/** \brief Create native widget. */
	static igdeNativeFoxScrollBar* CreateNativeWidget( igdeScrollBar &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual FXbool canFocus() const;
	
	virtual void UpdateRange();
	virtual void UpdateValue();
	virtual void UpdateEnabled();
	
	static int ScrollBarFlags( const igdeScrollBar &owner );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxScrollBar igdeNativeScrollBar;

#endif
