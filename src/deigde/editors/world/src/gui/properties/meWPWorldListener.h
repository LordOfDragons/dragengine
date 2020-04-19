/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEWPWORLDLISTENER_H_
#define _MEWPWORLDLISTENER_H_

#include "../../world/meWorldNotifier.h"

class meWPWorld;



/**
 * \brief World panel listener
 */
class meWPWorldListener : public meWorldNotifier{
private:
	meWPWorld &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWPWorldListener( meWPWorld &panel );
	
	/** \brief Clean up listener. */
	virtual ~meWPWorldListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Lighting parameters changed. */
	virtual void LightingChanged( meWorld *world );
	
	/** \brief Path find test changed. */
	virtual void PathFindTestChanged( meWorld *world );
	
	/** \brief Sector count changed. */
	virtual void SectorCountChanged( meWorld *world );
	
	/** \brief Sector size changed. */
	virtual void SectorSizeChanged( meWorld *world );
	
	/** \brief World properties changed. */
	virtual void PropertiesChanged( meWorld *world );
	
	/** \brief World active property changed. */
	virtual void ActivePropertyChanged( meWorld *world );
	
	/** \brief Object properties changed. */
	virtual void ObjectPropertiesChanged( meWorld *world, meObject *object );
	
	/** \brief Object texture properties changed. */
	virtual void ObjectTexturePropertiesChanged( meWorld *world, meObject *object, meObjectTexture *texture );
	/*@}*/
};

#endif
