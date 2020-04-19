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

#ifndef _SEWPNODELISTENER_H_
#define _SEWPNODELISTENER_H_

#include "../../skin/seSkinListener.h"

class seWPNode;



/**
 * \brief Node panel listener.
 */
class seWPNodeListener : public seSkinListener{
private:
	seWPNode &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPNodeListener( seWPNode &panel );
	
	/** \brief Clean up listener. */
	virtual ~seWPNodeListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active texture changed. */
	virtual void ActiveTextureChanged( seSkin *skin );
	
	/** \brief Property changed. */
	virtual void PropertyChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** \brief Active property changed. */
	virtual void ActivePropertyChanged( seSkin *skin, seTexture *texture );
	
	/** \brief Property node changed. */
	virtual void PropertyNodeChanged( seSkin *skin, seTexture *texture, seProperty *property, sePropertyNode *node );
	
	/** \brief Property node structre changed. */
	virtual void PropertyNodeStructureChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** \brief Property selected nodes changed. */
	virtual void PropertyNodeSelectionChanged( seSkin *skin, seTexture *texture, seProperty *property );
	
	/** \brief Active property node changed. */
	virtual void PropertyActiveNodeChanged( seSkin *skin, seTexture *texture, seProperty *property );
	/*@}*/
};

#endif
