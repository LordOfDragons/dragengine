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

#ifndef _DEBASEPHYSICSHEIGHTTERRAIN_H_
#define _DEBASEPHYSICSHEIGHTTERRAIN_H_

#include "../../../common/math/decMath.h"

class deHeightTerrain;
class deHeightTerrainSector;
class deDecal;
class deDecalList;
class decShape;


/**
 * \brief Physics System Height Terrain Peer.
 *
 * Peer for the height terrain resource used by the physics system.
 */
class DE_DLL_EXPORT deBasePhysicsHeightTerrain{
private:
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBasePhysicsHeightTerrain();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsHeightTerrain();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parameter changes. */
	virtual void ParametersChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Sector heights changed. */
	virtual void HeightChanged(const decPoint &fromSector, const decPoint &fromCoordinates,
		const decPoint &toSector, const decPoint &toCoordinates);
	
	/** \brief Sector has been added. */
	virtual void SectorAdded(deHeightTerrainSector *sector);
	
	/** \brief Sector has been removed. */
	virtual void SectorRemoved(int sector);
	
	/** \brief All sectors have been removed. */
	virtual void AllSectorsRemoved();
	
	/** \brief Sector changed. */
	virtual void SectorChanged(int sector);
	
	/** \brief Decal has been added. */
	virtual void DecalAdded(int sector, deDecal *decal);
	
	/** \brief Decal has been removed. */
	virtual void DecalRemoved(int sector, deDecal *decal);
	
	/** \brief All decals have been removed. */
	virtual void AllDecalsRemoved(int sector);
	/*@}*/
	
	
	
	/** \name Collision Detection */
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
};

#endif
