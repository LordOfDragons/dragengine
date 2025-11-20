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

#ifndef _MECLHITLISTENTRY_H_
#define _MECLHITLISTENTRY_H_

#include "../world/decal/meDecal.h"
#include "../world/object/meObject.h"
#include "../world/object/meObjectSnapPoint.h"
#include "../world/objectshape/meObjectShape.h"
#include "../world/navspace/meNavigationSpace.h"
#include "../world/terrain/meHeightTerrainSector.h"

#include <dragengine/common/math/decMath.h>


/**
 * \brief Hit list entry.
 */
class meCLHitListEntry : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meCLHitListEntry> Ref;
	
	
private:
	meObject::Ref pObject;
	meObjectShape::Ref pObjectShape;
	meDecal::Ref pDecal;
	meNavigationSpace::Ref pNavSpace;
	int pHTNavSpacePoint;
	meHeightTerrainSector::Ref pHTSector;
	meObjectSnapPoint::Ref pSnapPoint;
	
	float pDistance;
	decVector pNormal;
	
	
public:
	/** \name Constructors, Destructors */
	/*@{*/
	/** \brief Create hit list entry. */
	meCLHitListEntry();
	
protected:
	/** \brief Clean up hit list entry. */
	~meCLHitListEntry() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Clear entry. */
	void Clear();
	
	/** \brief Object or nullptr. */
	inline const meObject::Ref &GetObject() const{ return pObject; }
	
	/** \brief Set object or nullptr. */
	void SetObject(meObject *object);
	
	/** \brief Object shape or nullptr. */
	inline const meObjectShape::Ref &GetObjectShape() const{ return pObjectShape; }
	
	/** \brief Set object shape or nullptr. */
	void SetObjectShape(meObjectShape *objectShape);
	
	/** \brief Decal or nullptr. */
	inline const meDecal::Ref &GetDecal() const{ return pDecal; }
	
	/** \brief Set decal or nullptr. */
	void SetDecal(meDecal *decal);
	
	/** \brief Navigation space or nullptr. */
	inline const meNavigationSpace::Ref &GetNavigationSpace() const{ return pNavSpace; }
	
	/** \brief Set navigation space or nullptr. */
	void SetNavigationSpace(meNavigationSpace *navspace);
	
	/** \brief Height terrain navigation space point or -1. */
	inline int GetHTNavSpacePoint() const{ return pHTNavSpacePoint; }
	
	/** \brief Set height terrain navigation space point or -1. */
	void SetHTNavSpacePoint(int point);
	
	/** \brief Height terrain sector or nullptr. */
	inline const meHeightTerrainSector::Ref &GetHTSector() const{ return pHTSector; }
	
	/** \brief Set height terrain sector or nullptr. */
	void SetHTSector(meHeightTerrainSector *sector);
	
	/** \brief Snap point or nullptr. */
	inline const meObjectSnapPoint::Ref &GetSnapPoint() const{ return pSnapPoint; }
	
	/** \brief Set snap point or nullptr. */
	void SetSnapPoint(meObjectSnapPoint *snapPoint);
	
	
	/** \brief Distance. */
	inline float GetDistance() const{ return pDistance; }
	
	/** \brief Set distance. */
	void SetDistance(float distance);
	
	/** \brief Normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	
	/** \brief Set normal. */
	void SetNormal( const decVector &normal );
	
	
	/** \brief Sort decals. */
	void SortDecals();
	
	/**
	 * \brief Compare entry to another one.
	 * \returns 1 if entry is ordinally larger, -1 if smaller or 0 if equal.
	 */
	int CompareTo(const meCLHitListEntry &entry) const;
	
	/** Same element. */
	bool IsSame(const meCLHitListEntry &entry) const;
	/*@}*/
};

#endif
