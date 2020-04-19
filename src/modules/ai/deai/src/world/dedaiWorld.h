/* 
 * Drag[en]gine AI Module
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

#ifndef _DEDAIWORLD_H_
#define _DEDAIWORLD_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/systems/modules/ai/deBaseAIWorld.h>

class dedaiLayer;
class dedaiHeightTerrain;
class deNavigationBlocker;
class deDEAIModule;
class deWorld;
class dedaiSpaceMeshFace;
class dedaiSpaceGridVertex;



/**
 * \brief World peer.
 */
class dedaiWorld : public deBaseAIWorld{
private:
	deDEAIModule &pDEAI;
	deWorld &pWorld;
	
	dedaiHeightTerrain *pHeightTerrain;
	
	decObjectList pLayers;
	
	unsigned int pDevModeUpdateTracker;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedaiWorld( deDEAIModule &deai, deWorld &world );
	
	/** \brief Clean up peer. */
	virtual ~dedaiWorld();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief AI module. */
	inline deDEAIModule &GetDEAI() const{ return pDEAI; }
	
	/** \brief World resource. */
	inline deWorld &GetWorld() const{ return pWorld; }
	
	
	
	/** \brief Height terrain or \em NULL. */
	inline dedaiHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	
	
	/**
	 * \brief Layer with layer number.
	 * \details If not present it is created. Layers live as long as the world lives.
	 *          It is not necessary to add/free references.
	 */
	dedaiLayer *GetLayer( int layer );
	
	
	
	/** \brief Update developer mode information if enabled. */
	void CheckDeveloperMode();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Update world. */
	virtual void Update( float elapsed );
	
	/** \brief Navigation space has been added. */
	virtual void NavigationSpaceAdded( deNavigationSpace *navspace );
	
	/** \brief Navigation space has been removed. */
	virtual void NavigationSpaceRemoved( deNavigationSpace *navspace );
	
	/** \brief All navigation spaces have been removed. */
	virtual void AllNavigationSpacesRemoved();
	
	/** \brief Navigation blocker has been added. */
	virtual void NavigationBlockerAdded( deNavigationBlocker *blocker );
	
	/** \brief Navigation blocker has been removed. */
	virtual void NavigationBlockerRemoved( deNavigationBlocker *blocker );
	
	/** \brief All navigation blockers have been removed. */
	virtual void AllNavigationBlockersRemoved();
	
	/** \brief Navigator has been added. */
	virtual void NavigatorAdded( deNavigator *navigator );
	
	/** \brief Navigator has been removed. */
	virtual void NavigatorRemoved( deNavigator *navigator );
	
	/** \brief Remove all navigators. */
	virtual void AllNavigatorsRemoved();
	
	/** \brief Height Terrain changed. */
	virtual void HeightTerrainChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
