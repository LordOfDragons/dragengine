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

#ifndef _DEHEIGHTTERRAINMANAGER_H_
#define _DEHEIGHTTERRAINMANAGER_H_

#include "../../deResourceManager.h"
#include "../../deResourceList.h"

class deEngine;
class deHeightTerrain;


/**
 * \brief Height terrain manager.
 */
class deHeightTerrainManager : public deResourceManager{
private:
	deResourceList pTerrains;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resource manager. */
	deHeightTerrainManager( deEngine *engine );
	
	/** \brief Clean up resource manager and reports leaking resources. */
	virtual ~deHeightTerrainManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of height terrains. */
	int GetHeightTerrainCount() const;
	
	/** \brief Root height terrain resource for iteration purpose. */
	deHeightTerrain *GetRootHeightTerrain() const;
	
	/** \brief Create height terrain. */
	deHeightTerrain *CreateHeightTerrain( float sectorSize, int heightImageSize );
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic System Peers of all stored resources have to be created. */
	virtual void SystemGraphicLoad();
	
	/** \brief Graphic System Peers of all stored resources have to be freed. */
	virtual void SystemGraphicUnload();
	
	/** \brief Physics System Peers of all stored resources have to be created. */
	virtual void SystemPhysicsLoad();
	
	/** \brief Physics System Peers of all stored resources have to be freed. */
	virtual void SystemPhysicsUnload();
	
	/** \brief AI System Peers of all stored resources have to be created. */
	virtual void SystemAILoad();
	
	/** \brief AI System Peers of all stored resources have to be freed. */
	virtual void SystemAIUnload();
	
	/** \brief Audio system peers of all stored resources have to be created. */
	virtual void SystemAudioLoad();
	
	/** \brief Audio system peers of all stored resources have to be freed. */
	virtual void SystemAudioUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * 
	 * For use by resource objects only. Do not call directly.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
