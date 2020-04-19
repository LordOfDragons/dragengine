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

#ifndef _GDEWPSOCNAVIGATIONBLOCKERLISTENER_H_
#define _GDEWPSOCNAVIGATIONBLOCKERLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSOCNavigationBlocker;



/**
 * \brief Object class navigation blocker panel listener.
 */
class gdeWPSOCNavigationBlockerListener : public gdeGameDefinitionListener{
private:
	gdeWPSOCNavigationBlocker &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSOCNavigationBlockerListener( gdeWPSOCNavigationBlocker &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSOCNavigationBlockerListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Base path changed. */
	virtual void BasePathChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Base game definitions changed. */
	virtual void BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Object property name changed. */
	virtual void OCPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object properties changed. */
	virtual void OCPropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Active object class changed. */
	virtual void ActiveObjectClassChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Object class navigation blockers changed. */
	virtual void OCNavigationBlockersChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class navigation blocker changed. */
	virtual void OCNavigationBlockerChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker );
	
	/** \brief Active object class navigation blocker changed. */
	virtual void ActiveOCNavigationBlockerChanged( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
