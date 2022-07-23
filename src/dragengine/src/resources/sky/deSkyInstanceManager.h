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

#ifndef _DESKYINSTANCEMANAGER_H_
#define _DESKYINSTANCEMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deSkyInstance;


/**
 * \brief Sky instance resource manager.
 */
class DE_DLL_EXPORT deSkyInstanceManager : public deResourceManager{
private:
	deResourceList pInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky instance resource manager. */
	deSkyInstanceManager( deEngine *engine );
	
	/** \brief Clean up sky instance resource manager and report leaking resources. */
	virtual ~deSkyInstanceManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of sky instance resource. */
	int GetSkyInstanceCount() const;
	
	/** \brief Root sky instance resource for iteration purpose. */
	deSkyInstance *GetRootSkyInstance() const;
	
	/** \brief Create sky instance. */
	deSkyInstance *CreateSkyInstance();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic system peers of all stored resources have to be created. */
	virtual void SystemGraphicLoad();
	
	/** \brief Graphic system peers of all stored resources have to be freed. */
	virtual void SystemGraphicUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * 
	 * \warning For internal use only. Never call these function on your own.
	 */
	/*@{*/
	/** \warning For internal use only. Never call this function on your own. */
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
