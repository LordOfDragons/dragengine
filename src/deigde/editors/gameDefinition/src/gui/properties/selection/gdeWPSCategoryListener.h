/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEWPSCATEGORYLISTENER_H_
#define _GDEWPSCATEGORYLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSCategory;



/**
 * \brief Category panel listener.
 */
class gdeWPSCategoryListener : public gdeGameDefinitionListener{
private:
	gdeWPSCategory &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSCategoryListener( gdeWPSCategory &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSCategoryListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object class categories changed. */
	virtual void ObjectClassCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Skin categories changed. */
	virtual void SkinCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Sky categories changed. */
	virtual void SkyCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Particle emitter categories changed. */
	virtual void ParticleEmitterCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Active category changed. */
	virtual void ActiveCategoryChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** Object class count or order changed. */
	virtual void ObjectClassStructureChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Object class name changed. */
	virtual void ObjectClassNameChanged( gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass );
	/*@}*/
};

#endif
 
