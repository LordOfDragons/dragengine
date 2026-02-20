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

#include "deHeightTerrain.h"
#include "../../deResourceManager.h"
#include "../../deResourceList.h"

class deEngine;


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
	~deHeightTerrainManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of height terrains. */
	int GetHeightTerrainCount() const;
	
	/** \brief Root height terrain resource for iteration purpose. */
	deHeightTerrain *GetRootHeightTerrain() const;
	
	/** \brief Create height terrain. */
	deHeightTerrain::Ref CreateHeightTerrain(float sectorSize, int heightImageSize);
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Graphic System Peers of all stored resources have to be created. */
	void SystemGraphicLoad() override;
	
	/** \brief Graphic System Peers of all stored resources have to be freed. */
	void SystemGraphicUnload() override;
	
	/** \brief Physics System Peers of all stored resources have to be created. */
	void SystemPhysicsLoad() override;
	
	/** \brief Physics System Peers of all stored resources have to be freed. */
	void SystemPhysicsUnload() override;
	
	/** \brief AI System Peers of all stored resources have to be created. */
	void SystemAILoad() override;
	
	/** \brief AI System Peers of all stored resources have to be freed. */
	void SystemAIUnload() override;
	
	/** \brief Audio system peers of all stored resources have to be created. */
	void SystemAudioLoad() override;
	
	/** \brief Audio system peers of all stored resources have to be freed. */
	void SystemAudioUnload() override;
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * 
	 * For use by resource objects only. Do not call directly.
	 */
	/*@{*/
	void RemoveResource(deResource *resource) override;
	/*@}*/
};

#endif
