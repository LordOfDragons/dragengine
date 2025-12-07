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

#ifndef _DEDAIHEIGHTTERRAIN_H_
#define _DEDAIHEIGHTTERRAIN_H_

#include <dragengine/systems/modules/ai/deBaseAIHeightTerrain.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>

class dedaiWorld;
class deDEAIModule;
class dedaiHeightTerrainSector;

class deHeightTerrain;



/**
 * \brief Height terrain peer.
 */
class dedaiHeightTerrain : public deBaseAIHeightTerrain{
private:
	deDEAIModule &pDEAI;
	const deHeightTerrain &pHeightTerrain;
	
	decObjectOrderedSet pSectors;
	dedaiWorld *pParentWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedaiHeightTerrain(deDEAIModule &deai, const deHeightTerrain &heightTerrain);
	
	/** \brief Clean up peer. */
	virtual ~dedaiHeightTerrain();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief AI module. */
	inline deDEAIModule &GetDEAI() const{return pDEAI;}
	
	/** \brief Height terrain. */
	inline const deHeightTerrain &GetHeightTerrain() const{return pHeightTerrain;}
	
	
	
	/** \brief Parent world or \em NULL. */
	inline dedaiWorld *GetParentWorld() const{return pParentWorld;}
	
	/** \brief Set parent world or \em NULL. */
	void SetParentWorld(dedaiWorld *world);
	/*@}*/
	
	
	
	/** \name Sectors */
	/*@{*/
	/** \brief Count of sectors. */
	int GetSectorCount() const;
	
	/** \brief Sector at index. */
	dedaiHeightTerrainSector *GetSectorAt(int index) const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Parameter changes. */
	virtual void ParametersChanged();
	
	/** \brief Sector added. */
	virtual void SectorAdded(deHeightTerrainSector *sector);
	
	/** \brief Sector removed. */
	virtual void SectorRemoved(int index);
	
	/** \brief All sector removed. */
	virtual void AllSectorsRemoved();
	
	/** \brief Sector changed. */
	virtual void SectorChanged(int index);
	
	
	
	/** \brief Sector heights changed. */
	virtual void HeightChanged(const decPoint &fromSector, const decPoint &fromCoordinates,
		const decPoint &toSector, const decPoint &toCoordinates);
	
	
	
	/** \brief Navigation space added. */
	virtual void NavSpaceAdded(int sector, deHeightTerrainNavSpace *navspace);
	
	/** \brief Navigation space removed. */
	virtual void NavSpaceRemoved(int sector, int index);
	
	/** \brief All navigation spaces removed. */
	virtual void AllNavSpacesRemoved(int sector);
	
	/** \brief Navigation space layer changed. */
	virtual void NavSpaceLayerChanged(int sector, int index);
	
	/** \brief Navigation space type changed. */
	virtual void NavSpaceTypeChanged(int sector, int index);
	
	/** \brief Navigation space snapping parameters changed. */
	virtual void NavSpaceSnappingChanged(int sector, int index);
	
	/** \brief Navigation space layout changed. */
	virtual void NavSpaceLayoutChanged(int sector, int index);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
