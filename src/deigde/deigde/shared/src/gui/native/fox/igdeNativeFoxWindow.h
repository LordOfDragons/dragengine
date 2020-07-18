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

#ifndef _IGDENATIVEFOXWINDOW_H_
#define _IGDENATIVEFOXWINDOW_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeWindow;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native window.
 */
class igdeNativeFoxWindow : public FXTopWindow{
	FXDECLARE( igdeNativeFoxWindow )
	
protected:
	igdeNativeFoxWindow();
	
public:
	enum eFoxIDs{
		ID_SELF = FXTopWindow::ID_LAST,
	};
	
private:
	igdeWindow *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxWindow( igdeWindow &owner, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxWindow();
	
	/** \brief Create native widget. */
	static igdeNativeFoxWindow* CreateNativeWidget( igdeWindow &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateEnabled();
	virtual void UpdatePosition();
	virtual void UpdateIcon();
	virtual void UpdateTitle();
	virtual void UpdateSize();
	
	static int WindowFlags( const igdeWindow &owner );
	static int WindowPadLeft( const igdeGuiTheme &guitheme );
	static int WindowPadRight( const igdeGuiTheme &guitheme );
	static int WindowPadTop( const igdeGuiTheme &guitheme );
	static int WindowPadBottom( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onConfigure( FXObject *sender, FXSelector selector, void *data );
	long onClose( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

typedef igdeNativeFoxWindow igdeNativeWindow;

#endif
