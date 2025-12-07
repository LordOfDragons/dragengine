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

#ifndef _DEHEIGHTTERRAIN_H_
#define _DEHEIGHTTERRAIN_H_

#include "../../deResource.h"
#include "../../../common/math/decMath.h"
#include "../../../common/utils/decCollisionFilter.h"
#include "../../../common/collection/decPointerOrderedSet.h"
#include "../../../common/collection/decObjectOrderedSet.h"

class deHeightTerrainManager;
class deHeightTerrainSector;
class deDecalList;
class decShape;

class deBaseAudioHeightTerrain;
class deBaseGraphicHeightTerrain;
class deBasePhysicsHeightTerrain;
class deBaseAIHeightTerrain;


// definitions
/** 8-bit image value corresponding to 0 level. */
#define HT_8BIT_BASE		127
/** 16-bit image value corresponding to 0 level. */
#define HT_16BIT_BASE		32767

/** 8-bit scaling factor height to pixel. */
#define HT_8BIT_HTOP		255.0f
/** 16-bit scaling factor height to pixel. */
#define HT_16BIT_HTOP		65535.0f

/** 8-bit scaling factor pixel to height. */
#define HT_8BIT_PTOH		(1.0f / 255.0f)
/** 16-bit scaling factor pixel to height. */
#define HT_16BIT_PTOH		(1.0f / 65535.0f)


/**
 * \brief Height Terrain.
 *
 * Defines a height terrain.
 */
class DE_DLL_EXPORT deHeightTerrain : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deHeightTerrain> Ref;
	
	
private:
	float pSectorSize;
	int pSectorResolution;
	float pBaseHeight;
	float pHeightScaling;
	
	decPointerOrderedSet pSectors;
	decCollisionFilter pCollisionFilter;
	
	deBaseGraphicHeightTerrain *pPeerGraphic;
	deBasePhysicsHeightTerrain *pPeerPhysics;
	deBaseAudioHeightTerrain *pPeerAudio;
	deBaseAIHeightTerrain *pPeerAI;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create new height terrain.
	 * \param[in] manager Height terrain manager.
	 * \param[in] sectorSize Size in meters of sectors along the X and Z axis.
	 * \param[in] sectorResolution Resolution of sectors (width and height of height images).
	 */
	deHeightTerrain(deHeightTerrainManager *manager, float sectorSize, int sectorResolution);
	
protected:
	/**
	 * \brief Clean up height terrain.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deHeightTerrain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Size in meters of sectors along the X and Z axis.. */
	inline float GetSectorSize() const{ return pSectorSize; }
	
	/** \brief Resolution of sectors (width and height of height images). */
	inline int GetSectorResolution() const{ return pSectorResolution; }
	
	/** \brief Base height in meters. */
	inline float GetBaseHeight() const{ return pBaseHeight; }
	
	/** \brief Set base height in meters. */
	void SetBaseHeight(float height);
	
	/** \brief Height scaling. */
	inline float GetHeightScaling() const{ return pHeightScaling; }
	
	/** \brief Set height scaling. */
	void SetHeightScaling(float scaling);
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter(const decCollisionFilter &collisionFilter);
	
	/** \brief Notifies the peers that a sector heights changed. */
	void NotifyHeightChanged(const decPoint &fromSector, const decPoint &fromCoordinates,
		const decPoint &toSector, const decPoint &toCoordinates);
	/*@}*/
	
	
	
	/** \name Sectors */
	/*@{*/
	/** \brief Number of sectors. */
	int GetSectorCount() const;
	
	/** \brief Sector at the given index. */
	deHeightTerrainSector *GetSectorAt(int index) const;
	
	/** \brief Sector with the given coordinates or NULL if not existing. */
	deHeightTerrainSector *GetSectorWith(const decPoint &coordinates) const;
	
	/** \brief Adds a sector to the world. */
	void AddSector(deHeightTerrainSector *sector);
	
	/** \brief Removes the given sector from the world. */
	void RemoveSector(deHeightTerrainSector *sector);
	
	/** \brief Removes all sectors from the world. */
	void RemoveAllSectors();
	
	/** \brief Notifies the peers that a sector changed. */
	void NotifySectorChanged(int sector);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object. */
	inline deBaseGraphicHeightTerrain *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object. */
	void SetPeerGraphic(deBaseGraphicHeightTerrain *peer);
	
	/** \brief Physics system peer object. */
	inline deBasePhysicsHeightTerrain *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer object. */
	void SetPeerPhysics(deBasePhysicsHeightTerrain *peer);
	
	/** \brief Audio system peer or NULL. */
	inline deBaseAudioHeightTerrain *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set Audio system peer or NULL. */
	void SetPeerAudio(deBaseAudioHeightTerrain *peer);
	
	/** \brief AI system peer or NULL. */
	inline deBaseAIHeightTerrain *GetPeerAI() const{ return pPeerAI; }
	
	/** \brief Set AI system peer or NULL. */
	void SetPeerAI(deBaseAIHeightTerrain *peer);
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/**
	 * Stores all decals in contact with the given surface point into the provided
	 * list. The point is best taken from an earlier collision test and is in world
	 * coordinates.
	 * \param point Point on the surface to test.
	 * \param list List of decals to fill in. Not emptied before filling.
	 */
	void FindDecalsAt(const decDVector &point, deDecalList &list);
	
	/**
	 * Stores all decals in contact with the given shape into the provided list.
	 * \param shape Shape to test.
	 * \param list List of decals to fill in. Not emptied before filling.
	 */
	void FindDecalsTouching(const decShape &shape, deDecalList &list);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
