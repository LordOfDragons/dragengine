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

#ifndef _MEWPSDECALLISTENER_H_
#define _MEWPSDECALLISTENER_H_

#include "../../../../world/meWorldNotifier.h"

class meWPSDecal;



/**
 * \brief Decal panel listener.
 */
class meWPSDecalListener : public meWorldNotifier{
private:
	meWPSDecal &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWPSDecalListener( meWPSDecal &panel );
	
	/** \brief Clean up listener. */
	virtual ~meWPSDecalListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Decal selection changed. */
	virtual void DecalSelectionChanged( meWorld *world );
	
	/** \brief Decal changed. */
	virtual void DecalChanged( meWorld *world, meDecal *decal );
	
	/** \brief Decal geometry changed. */
	virtual void DecalGeometryChanged( meWorld *world, meDecal *decal );
	
	/** \brief Decal properties changed. */
	virtual void DecalPropertiesChanged( meWorld *world, meDecal *decal );
	
	/** \brief Decal active property changed. */
	virtual void DecalActivePropertyChanged( meWorld *world, meDecal *decal );
	/*@}*/
};

#endif
