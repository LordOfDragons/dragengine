/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEWINDOWMAINLISTENER_H_
#define _SEWINDOWMAINLISTENER_H_

#include "../skin/seSkinListener.h"

class seWindowMain;



/**
 * \brief Window Main Listener.
 */
class seWindowMainListener : public seSkinListener{
private:
	seWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWindowMainListener( seWindowMain &window );
	
	/** \brief Clean up listener. */
	~seWindowMainListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged( seSkin *skin );
	
	/** \brief Undo changed. */
	virtual void UndoChanged( seSkin *skin );
	
	/** \brief Textures have been added or removed. */
	virtual void TextureStructureChanged ( seSkin *skin );
	
	/** \brief Properties have been added or removed. */
	virtual void PropertyStructureChanged ( seSkin *skin, seTexture *texture );
	/*@}*/
};

#endif
