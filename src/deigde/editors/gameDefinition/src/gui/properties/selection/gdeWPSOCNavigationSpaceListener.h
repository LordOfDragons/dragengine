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

#ifndef _GDEWPSOCNAVIGATIONSPACELISTENER_H_
#define _GDEWPSOCNAVIGATIONSPACELISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSOCNavigationSpace;



/**
 * \brief Object class navigation space panel listener.
 */
class gdeWPSOCNavigationSpaceListener : public gdeGameDefinitionListener{
private:
	gdeWPSOCNavigationSpace &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSOCNavigationSpaceListener( gdeWPSOCNavigationSpace &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSOCNavigationSpaceListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object property name changed. */
	virtual void OCPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object properties changed. */
	virtual void OCPropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Active object class changed. */
	virtual void ActiveObjectClassChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Object class navigation spaces changed. */
	virtual void OCNavigationSpacesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class navigation space changed. */
	virtual void OCNavigationSpaceChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace );
	
	/** \brief Active object class navigation space changed. */
	virtual void ActiveOCNavigationSpaceChanged( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
