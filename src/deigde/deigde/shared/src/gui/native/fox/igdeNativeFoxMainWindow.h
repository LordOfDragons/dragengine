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

#ifndef _IGDENATIVEFOXMAINWINDOW_H_
#define _IGDENATIVEFOXMAINWINDOW_H_

#include "foxtoolkit.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/math/decMath.h>


class igdeMainWindow;


/**
 * \brief FOX toolkit native main window.
 */
class igdeNativeFoxMainWindow : public FXMainWindow{
	FXDECLARE( igdeNativeFoxMainWindow )
protected:
	igdeNativeFoxMainWindow();
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create container. */
	igdeNativeFoxMainWindow( igdeMainWindow &owner );
	
	/** \brief Clean up container. */
	virtual ~igdeNativeFoxMainWindow();
	
	/** \brief Create native widget. */
	static igdeNativeFoxMainWindow* CreateNativeWidget( igdeMainWindow &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void create();
	virtual void destroy();
	
	/** \brief System color. */
	decColor GetSystemColor( igdeEnvironment::eSystemColors color ) const;
	
	/** \brief Update window state. */
	void UpdateWindowState();
	
	/** \brief Update enabled state. */
	virtual void UpdateEnabled();
	
	/** \brief Update position. */
	virtual void UpdatePosition();
	
	/** \brief Update icon. */
	virtual void UpdateIcon();
	
	/** \brief Update window title. */
	virtual void UpdateTitle();
	
	/** \brief Update size. */
	virtual void UpdateSize();
	
	/** \brief Set window state to owner. */
	void SetWindowState();
	
	/** \brief Get main app font config. */
	static void GetAppFontConfig( igdeFont::sConfiguration &config );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onConfigure( FXObject *sender, FXSelector selector, void *data );
	long onMap( FXObject *sender, FXSelector selector, void *data );
	long onClose( FXObject *sender, FXSelector selector, void *data );
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
	long onMinimized( FXObject *sender, FXSelector selector, void *data );
	long onRestored( FXObject *sender, FXSelector selector, void *data );
	long onMaximized( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
	
private:
	igdeMainWindow *pOwner;
};

typedef igdeNativeFoxMainWindow igdeNativeMainWindow;

#endif
