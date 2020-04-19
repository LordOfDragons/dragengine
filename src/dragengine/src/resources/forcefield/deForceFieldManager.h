/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEFORCEFIELDMANAGER_H_
#define _DEFORCEFIELDMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deForceField;


/**
 * \brief Force Field Manager.
 *
 * Manages a list of force fields.
 */
class deForceFieldManager : public deResourceManager{
private:
	deResourceList pFields;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new resource manager. */
	deForceFieldManager( deEngine *engine );
	
	/** \brief Clean up resource manager and reports leaking resources. */
	virtual ~deForceFieldManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of force fields. */
	int GetForceFieldCount() const;
	
	/** \brief Root force field resource for iteration purpose. */
	deForceField *GetRootForceField() const;
	
	/** \brief Create new force field. */
	deForceField *CreateForceField();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemPhysicsLoad();
	void SystemPhysicsUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
