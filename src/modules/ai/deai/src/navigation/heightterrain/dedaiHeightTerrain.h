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
	dedaiHeightTerrain( deDEAIModule &deai, const deHeightTerrain &heightTerrain );
	
	/** \brief Clean up peer. */
	virtual ~dedaiHeightTerrain();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief AI module. */
	inline deDEAIModule &GetDEAI() const{ return pDEAI; }
	
	/** \brief Height terrain. */
	inline const deHeightTerrain &GetHeightTerrain() const{ return pHeightTerrain; }
	
	
	
	/** \brief Parent world or \em NULL. */
	inline dedaiWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL. */
	void SetParentWorld( dedaiWorld *world );
	/*@}*/
	
	
	
	/** \name Sectors */
	/*@{*/
	/** \brief Count of sectors. */
	int GetSectorCount() const;
	
	/** \brief Sector at index. */
	dedaiHeightTerrainSector *GetSectorAt( int index ) const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Parameter changes. */
	virtual void ParametersChanged();
	
	/** \brief Sector added. */
	virtual void SectorAdded( deHeightTerrainSector *sector );
	
	/** \brief Sector removed. */
	virtual void SectorRemoved( int index );
	
	/** \brief All sector removed. */
	virtual void AllSectorsRemoved();
	
	/** \brief Sector changed. */
	virtual void SectorChanged( int index );
	
	
	
	/** \brief Sector heights changed. */
	virtual void HeightChanged( const decPoint &fromSector, const decPoint &fromCoordinates,
		const decPoint &toSector, const decPoint &toCoordinates );
	
	
	
	/** \brief Navigation space added. */
	virtual void NavSpaceAdded( int sector, deHeightTerrainNavSpace *navspace );
	
	/** \brief Navigation space removed. */
	virtual void NavSpaceRemoved( int sector, int index );
	
	/** \brief All navigation spaces removed. */
	virtual void AllNavSpacesRemoved( int sector );
	
	/** \brief Navigation space layer changed. */
	virtual void NavSpaceLayerChanged( int sector, int index );
	
	/** \brief Navigation space type changed. */
	virtual void NavSpaceTypeChanged( int sector, int index );
	
	/** \brief Navigation space snapping parameters changed. */
	virtual void NavSpaceSnappingChanged( int sector, int index );
	
	/** \brief Navigation space layout changed. */
	virtual void NavSpaceLayoutChanged( int sector, int index );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
