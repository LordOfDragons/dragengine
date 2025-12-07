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

#ifndef _DEHEIGHTTERRAINMANAGER_H_
#define _DEHEIGHTTERRAINMANAGER_H_

#include "../../deResourceManager.h"
#include "../../deResourceList.h"

class deEngine;
class deHeightTerrain;


/**
 * \brief Height terrain manager.
 */
class DE_DLL_EXPORT deHeightTerrainManager : public deResourceManager{
private:
	deResourceList pTerrains;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resource manager. */
	deHeightTerrainManager(deEngine *engine);
	
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
	deHeightTerrain *CreateHeightTerrain(float sectorSize, int heightImageSize);
	
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
	void RemoveResource(deResource *resource);
	/*@}*/
};

#endif
