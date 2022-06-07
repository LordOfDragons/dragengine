/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _IGDENATIVENULLMAINWINDOW_H_
#define _IGDENATIVENULLMAINWINDOW_H_

#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/math/decMath.h>

class igdeMainWindow;


/**
 * Null main window.
 */
class igdeNativeNullMainWindow{
private:
	igdeMainWindow &pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create container. */
	igdeNativeNullMainWindow( igdeMainWindow &owner );
	
	/** \brief Clean up container. */
	virtual ~igdeNativeNullMainWindow();
	
	/** \brief Create native widget. */
	static igdeNativeNullMainWindow* CreateNativeWidget( igdeMainWindow &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
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
};

typedef igdeNativeNullMainWindow igdeNativeMainWindow;

#endif
