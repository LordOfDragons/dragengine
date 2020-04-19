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

#ifndef _GDEWPSOBJECTLISTENER_H_
#define _GDEWPSOBJECTLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSObjectClass;



/**
 * \brief Object class panel listener.
 */
class gdeWPSObjectClassListener : public gdeGameDefinitionListener{
private:
	gdeWPSObjectClass &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSObjectClassListener( gdeWPSObjectClass &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSObjectClassListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object class used tags changed. */
	virtual void ObjectClassUsedTagsChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Object class categories changed. */
	virtual void ObjectClassCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Base path changed. */
	virtual void BasePathChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Base game definitions changed. */
	virtual void BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Defined and used identifiers changed. */
	virtual void DefinedUsedIDsChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** Object class count or order changed. */
	virtual void ObjectClassStructureChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Object class changed. */
	virtual void ObjectClassChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object class name changed. */
	virtual void ObjectClassNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object property changed. */
	virtual void OCPropertyChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object property name changed. */
	virtual void OCPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object properties changed. */
	virtual void OCPropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object property values changed. */
	virtual void OCPropertyValuesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Object texture property changed. */
	virtual void OCTexturePropertyChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object texture property name changed. */
	virtual void OCTexturePropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object texture properties changed. */
	virtual void OCTexturePropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Active object class changed. */
	virtual void ActiveObjectClassChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Object class inherits changed. */
	virtual void OCInheritsChanged( gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass );
	
	/** \brief Object class inherit changed. */
	virtual void OCInheritChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCInherit *inherit );
	/*@}*/
};

#endif
