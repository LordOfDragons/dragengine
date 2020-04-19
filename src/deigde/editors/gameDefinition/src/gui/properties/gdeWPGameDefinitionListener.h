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

#ifndef _GDEWPGAMEDEFINITIONLISTENER_H_
#define _GDEWPGAMEDEFINITIONLISTENER_H_

#include "../../gamedef/gdeGameDefinitionListener.h"

class gdeWPGameDefinition;



/**
 * \brief Properties window game definition panel listener.
 */
class gdeWPGameDefinitionListener : public gdeGameDefinitionListener{
private:
	gdeWPGameDefinition &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPGameDefinitionListener( gdeWPGameDefinition &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPGameDefinitionListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Game definition changed. */
	virtual void GameDefinitionChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Base path changed. */
	virtual void BasePathChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Base game definitions changed. */
	virtual void BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief World properties changed. */
	virtual void WorldPropertiesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief World changed. */
	virtual void WorldPropertyChanged( gdeGameDefinition *gameDefinition,
		gdeProperty *property );
	
	/** \brief World name changed. */
	virtual void WorldPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeProperty *property );
	
	
	
	/** \brief Decal properties changed. */
	virtual void DecalPropertiesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Decal changed. */
	virtual void DecalPropertyChanged( gdeGameDefinition *gameDefinition,
		gdeProperty *property );
	
	/** \brief Decal name changed. */
	virtual void DecalPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeProperty *property );
	
	
	
	/** \brief Auto find path object classes changed. */
	virtual void AutoFindPathObjectClassesChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Auto find path skins changed. */
	virtual void AutoFindPathSkinsChanged( gdeGameDefinition *gameDefinition );
	
	/** \brief Auto find path skies changed. */
	virtual void AutoFindPathSkiesChanged( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
