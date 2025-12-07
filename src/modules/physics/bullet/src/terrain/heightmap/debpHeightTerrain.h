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
	debpHeightTerrain(dePhysicsBullet *bullet, deHeightTerrain *heightTerrain);
	/** Cleans up the peer. */
	virtual ~debpHeightTerrain();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline dePhysicsBullet *GetBullet() const{return pBullet;}
	/** Retrieves the height terrain. */
	inline deHeightTerrain *GetHeightTerrain() const{return pHeightTerrain;}
	
	/** Retrieves the parent world. */
	inline debpWorld *GetParentWorld() const{return pParentWorld;}
	/** Sets the parent world. */
	void SetParentWorld(debpWorld *parentWorld);
	
	/** Retrieves the number of sectors. */
	inline int GetSectorCount() const{return pSectorCount;}
	/** Retrieves the sector at the given index. */
	debpHTSector *GetSectorAt(int index) const;
	/** Retrieves the sector with the given coordinates or NULL if not found. */
	debpHTSector *GetSectorWith(int x, int y) const;
	
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
	virtual void HeightChanged(const decPoint &fromSector, const decPoint &fromCoordinates,
		const decPoint &toSector, const decPoint &toCoordinates);
	
	/** Sector has been added. */
	virtual void SectorAdded(deHeightTerrainSector *sector);
	/** Sector has been removed. */
	virtual void SectorRemoved(int index);
	/** All sectors have been removed. */
	virtual void AllSectorsRemoved();
	/** Sector changed. */
	virtual void SectorChanged(int index);
	
	/** Decal has been added. */
	virtual void DecalAdded(int sector, deDecal *decal);
	/** Decal has been removed. */
	virtual void DecalRemoved(int sector, deDecal *decal);
	/** All decals have been removed. */
	virtual void AllDecalsRemoved(int sector);
	/*@}*/
	
	/** @name Collision Detection */
	/*@{*/
	/**
	 * Stores all decals in contact with the given surface point into the provided
	 * list. The point is best taken from an earlier collision test and is in world
	 * coordinates.
	 */
	virtual void FindDecalsAt(const decDVector &point, deDecalList &list);
	/**
	 * Stores all decals in contact with the given shape into the provided list.
	 */
	virtual void FindDecalsTouching(const decShape &shape, deDecalList &list);
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
