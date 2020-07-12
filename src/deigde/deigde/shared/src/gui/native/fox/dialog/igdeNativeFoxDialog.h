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

#ifndef _IGDENATIVEFOXDIALOG_H_
#define _IGDENATIVEFOXDIALOG_H_

#include "../foxtoolkit.h"

class igdeDialog;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native dialog.
 */
class igdeNativeFoxDialog : public FXDialogBox{
	FXDECLARE( igdeNativeFoxDialog )
	
protected:
	igdeNativeFoxDialog();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDialogBox::ID_LAST,
	};
	
private:
	igdeDialog *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxDialog( igdeDialog &owner, FXWindow *parent, const igdeGuiTheme &guitheme );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxDialog();
	
	/** \brief Create native widget. */
	static igdeNativeFoxDialog* CreateNativeWidget( igdeDialog &owner, igdeWidget *ownerOwner );
	
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
	virtual void ShowDialog();
	
	virtual void CloseDialog( bool accepted );
	
	static int DialogFlags( const igdeDialog &owner );
	
	static int DialogPadLeft( const igdeGuiTheme &guitheme );
	static int DialogPadRight( const igdeGuiTheme &guitheme );
	static int DialogPadTop( const igdeGuiTheme &guitheme );
	static int DialogPadBottom( const igdeGuiTheme &guitheme );
	static int DialogPadContent( const igdeGuiTheme &guitheme );
	static int DialogPadButtons( const igdeGuiTheme &guitheme );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onClose( FXObject*, FXSelector, void* );
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	long onFrameUpdate( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxDialog igdeNativeDialog;

#endif
