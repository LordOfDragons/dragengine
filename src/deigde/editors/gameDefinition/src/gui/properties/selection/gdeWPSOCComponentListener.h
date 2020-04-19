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

#ifndef _GDEWPSOCCOMPONENTLISTENER_H_
#define _GDEWPSOCCOMPONENTLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSOCComponent;



/**
 * \brief Object class component panel listener.
 */
class gdeWPSOCComponentListener : public gdeGameDefinitionListener{
private:
	gdeWPSOCComponent &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSOCComponentListener( gdeWPSOCComponent &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSOCComponentListener();
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
	
	/** \brief Object texture property name changed. */
	virtual void OCTexturePropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object texture properties changed. */
	virtual void OCTexturePropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Active object class changed. */
	virtual void ActiveObjectClassChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Object class components changed. */
	virtual void OCComponentsChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class component changed. */
	virtual void OCComponentChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** \brief Object class component active texture changed. */
	virtual void OCComponentActiveTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component );
	
	/** \brief Object class component texture changed. */
	virtual void OCComponentTextureChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** \brief Object class component texture name changed. */
	virtual void OCComponentTextureNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** \brief Object class component texture properties changed. */
	virtual void OCComponentTexturePropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCComponent *component, gdeOCComponentTexture *texture );
	
	/** \brief Active object class component changed. */
	virtual void ActiveOCComponentChanged( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
