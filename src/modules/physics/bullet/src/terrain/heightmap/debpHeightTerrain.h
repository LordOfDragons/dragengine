/* 
 * Drag[en]gine Bullet Physics Module
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

// include only once
#ifndef _DEBPHEIGHTTERRAIN_H_
#define _DEBPHEIGHTTERRAIN_H_

// includes
#include "dragengine/common/math/decMath.h"
#include "dragengine/systems/modules/physics/deBasePhysicsHeightTerrain.h"

// predefinitions
class debpWorld;
class debpHTSector;
class dePhysicsBullet;

class deHeightTerrain;



/**
 * @brief Height Terrain.
 *
 * Peer for the height terrain resource.
 */
class debpHeightTerrain : public deBasePhysicsHeightTerrain{
private:
	dePhysicsBullet *pBullet;
	deHeightTerrain *pHeightTerrain;
	
	debpWorld *pParentWorld;
	
	debpHTSector **pSectors;
	int pSectorCount;
	int pSectorSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	debpHeightTerrain( dePhysicsBullet *bullet, deHeightTerrain *heightTerrain );
	/** Cleans up the peer. */
	virtual ~debpHeightTerrain();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline dePhysicsBullet *GetBullet() const{ return pBullet; }
	/** Retrieves the height terrain. */
	inline deHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** Retrieves the parent world. */
	inline debpWorld *GetParentWorld() const{ return pParentWorld; }
	/** Sets the parent world. */
	void SetParentWorld( debpWorld *parentWorld );
	
	/** Retrieves the number of sectors. */
	inline int GetSectorCount() const{ return pSectorCount; }
	/** Retrieves the sector at the given index. */
	debpHTSector *GetSectorAt( int index ) const;
	/** Retrieves the sector with the given coordinates or NULL if not found. */
	debpHTSector *GetSectorWith( int x, int y ) const;
	
	/** Update. */
	void Update();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Parameter changes. */
	virtual void ParametersChanged();
	
	/** Layer mask changed. */
	virtual void CollisionFilterChanged();
	/** Sector heights changed. */
	virtual void HeightChanged( const decPoint &fromSector, const decPoint &fromCoordinates,
		const decPoint &toSector, const decPoint &toCoordinates );
	
	/** Sector has been added. */
	virtual void SectorAdded( deHeightTerrainSector *sector );
	/** Sector has been removed. */
	virtual void SectorRemoved( int index );
	/** All sectors have been removed. */
	virtual void AllSectorsRemoved();
	/** Sector changed. */
	virtual void SectorChanged( int index );
	
	/** Decal has been added. */
	virtual void DecalAdded( int sector, deDecal *decal );
	/** Decal has been removed. */
	virtual void DecalRemoved( int sector, deDecal *decal );
	/** All decals have been removed. */
	virtual void AllDecalsRemoved( int sector );
	/*@}*/
	
	/** @name Collision Detection */
	/*@{*/
	/**
	 * Stores all decals in contact with the given surface point into the provided
	 * list. The point is best taken from an earlier collision test and is in world
	 * coordinates.
	 */
	virtual void FindDecalsAt( const decDVector &point, deDecalList &list );
	/**
	 * Stores all decals in contact with the given shape into the provided list.
	 */
	virtual void FindDecalsTouching( const decShape &shape, deDecalList &list );
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
