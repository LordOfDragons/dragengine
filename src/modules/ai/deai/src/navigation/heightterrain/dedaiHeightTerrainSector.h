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

#ifndef _DEDAIHEIGHTTERRAINSECTOR_H_
#define _DEDAIHEIGHTTERRAINSECTOR_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>

class dedaiHeightTerrain;
class dedaiHeightTerrainNavSpace;
class dedaiSpace;

class deHeightTerrainSector;



/**
 * \brief Navigation space peer.
 */
class dedaiHeightTerrainSector : public deObject{
private:
	dedaiHeightTerrain &pHeightTerrain;
	const deHeightTerrainSector &pSector;
	
	decDVector pPosition;
	float *pHeights;
	
	decObjectOrderedSet pNavSpaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sector. */
	dedaiHeightTerrainSector( dedaiHeightTerrain &heightTerrain, const deHeightTerrainSector &sector );
	
protected:
	/** \brief Clean up sector. */
	virtual ~dedaiHeightTerrainSector();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Height terrain. */
	inline dedaiHeightTerrain &GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** \brief Sector. */
	inline const deHeightTerrainSector &GetSector() const{ return pSector; }
	
	
	
	/** \brief World position of sector center. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Heights. */
	inline const float *GetHeights() const{ return pHeights; }
	/*@}*/
	
	
	
	/** \name Navigation spaces */
	/*@{*/
	/** \brief Number of navigation spaces. */
	int GetNavSpaceCount() const;
	
	/** \brief Navigation space at index. */
	dedaiHeightTerrainNavSpace *GetNavSpaceAt( int index ) const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Parent world changed. */
	void ParentWorldChanged();
	
	/** \brief Sector changed. */
	void SectorChanged();
	
	/** \brief Sector heights changed. */
	void HeightChanged( const decPoint &from, const decPoint &to );
	
	/** \brief Navigation space added. */
	void NavSpaceAdded( deHeightTerrainNavSpace *navspace );
	
	/** \brief Navigation space removed. */
	void NavSpaceRemoved( int index );
	
	/** \brief All navigation spaces removed. */
	void AllNavSpacesRemoved();
	
	/** \brief Navigation space layer changed. */
	void NavSpaceLayerChanged( int index );
	
	/** \brief Navigation space type changed. */
	void NavSpaceTypeChanged( int index );
	
	/** \brief Navigation space snapping parameters changed. */
	void NavSpaceSnappingChanged( int index );
	
	/** \brief Navigation space layout changed. */
	void NavSpaceLayoutChanged( int index );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pUpdateHeights();
	void pUpdateHeights( const decPoint &from, const decPoint &to );
};

#endif
