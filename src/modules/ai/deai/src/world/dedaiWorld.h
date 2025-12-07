/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	dedaiWorld(deDEAIModule &deai, deWorld &world);
	
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
	dedaiLayer *GetLayer(int layer);
	
	
	
	/** \brief Update developer mode information if enabled. */
	void CheckDeveloperMode();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Update world. */
	virtual void Update(float elapsed);
	
	/** \brief Navigation space has been added. */
	virtual void NavigationSpaceAdded(deNavigationSpace *navspace);
	
	/** \brief Navigation space has been removed. */
	virtual void NavigationSpaceRemoved(deNavigationSpace *navspace);
	
	/** \brief All navigation spaces have been removed. */
	virtual void AllNavigationSpacesRemoved();
	
	/** \brief Navigation blocker has been added. */
	virtual void NavigationBlockerAdded(deNavigationBlocker *blocker);
	
	/** \brief Navigation blocker has been removed. */
	virtual void NavigationBlockerRemoved(deNavigationBlocker *blocker);
	
	/** \brief All navigation blockers have been removed. */
	virtual void AllNavigationBlockersRemoved();
	
	/** \brief Navigator has been added. */
	virtual void NavigatorAdded(deNavigator *navigator);
	
	/** \brief Navigator has been removed. */
	virtual void NavigatorRemoved(deNavigator *navigator);
	
	/** \brief Remove all navigators. */
	virtual void AllNavigatorsRemoved();
	
	/** \brief Height Terrain changed. */
	virtual void HeightTerrainChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
