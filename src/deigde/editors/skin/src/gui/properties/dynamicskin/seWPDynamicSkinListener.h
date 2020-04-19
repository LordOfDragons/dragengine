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

#ifndef _SEWPDYNAMICSKINLISTENER_H_
#define _SEWPDYNAMICSKINLISTENER_H_

#include "../../../skin/seSkinListener.h"

class seWPDynamicSkin;



/**
 * \brief Texture panel listener.
 */
class seWPDynamicSkinListener : public seSkinListener{
private:
	seWPDynamicSkin &pPanel;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPDynamicSkinListener( seWPDynamicSkin &panel );
	
	/** \brief Clean up listener. */
	virtual ~seWPDynamicSkinListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dynamic skin renderables have been added or removed. */
	virtual void DynamicSkinRenderableStructureChanged ( seSkin *skin );
	
	/** \brief A dynamic skin renderable changed. */
	virtual void DynamicSkinRenderableChanged( seSkin *skin, seDynamicSkinRenderable *renderable );
	
	/** \brief A dynamic skin renderable name changed. */
	virtual void DynamicSkinRenderableNameChanged( seSkin *skin, seDynamicSkinRenderable *renderable );
	
	/** \brief Active dynamic skin renderable changed. */
	virtual void DynamicSkinActiveRenderableChanged( seSkin *skin );
	/*@}*/
};

#endif
