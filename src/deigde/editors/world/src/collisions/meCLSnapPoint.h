/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MECLSNAPPOINT_H_
#define _MECLSNAPPOINT_H_

#include "../world/object/meObjectList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class meWorld;
class meObject;
class meObjectSnapPoint;
class igdeGDCSnapPoint;
class decShapeList;



/**
 * \brief Closest Snap Point Visitor.
 *
 * Collision listener storing the closest hit snap point for an object. Snap points matching
 * the object to test are ignored to avoid self-snapping. The object center and each snap
 * point of the object are tested against the found snap points. The pair with the closest
 * distance is chosen.
 */
class meCLSnapPoint : public deBaseScriptingCollider{
private:
	meWorld &pWorld;
	
	meObject *pObject;
	
	float pDistance;
	igdeGDCSnapPoint *pSourceSnapPoint;
	meObject *pTargetObject;
	const igdeGDCSnapPoint *pTargetSnapPoint;
	
	meObjectList pIgnoreObjects;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	meCLSnapPoint( meWorld &world, meObject *object );
	
	/** \brief Clean up visitor. */
	virtual ~meCLSnapPoint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief World. */
	inline meWorld &GetWorld() const{ return pWorld; }
	
	/** \brief Object. */
	inline meObject *GetObject() const{ return pObject; }
	
	/** \brief Snapping is enabled due to gui parameters. */
	bool IsSnappingEnabled() const;
	
	/**
	 * \brief Build testing shape list for collider.
	 * 
	 * Creates a box shape enclosing the object center and all snap points in the object
	 * in world space. Use this as shape for collider using this listener. The shape is
	 * relative to the object center. Thus set the collider position and orientation to
	 * match the object position and orientation.
	 */
	void CalcBoundingBoxShape( decShapeList &list );
	
	
	
	/** \brief Add object to ignore. */
	void AddIgnoreObject( meObject *object );
	
	/** \brief Reset visitor. */
	void Reset();
	
	/** \brief Distance between source point and target snap point. */
	inline float GetDistance() const{ return pDistance; }
	
	/** \brief Source snap point or \em NULL to use object position. */
	inline igdeGDCSnapPoint *GetSourceSnapPoint() const{ return pSourceSnapPoint; }
	
	/** \brief Target object or \em NULL if not set. */
	inline meObject *GetTargetObject() const{ return pTargetObject; }
	
	/** \brief Target snap point or \em NULL if not set. */
	inline const igdeGDCSnapPoint *GetTargetSnapPoint() const{ return pTargetSnapPoint; }
	
	
	
	/** \brief Matrix required to apply snapping in world space. */
	decDMatrix CalcCorrectionMatrix() const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Collision response. */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	
	/** \brief Can hit collider. */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	
	/** \brief Collider changed which is not used. */
	virtual void ColliderChanged( deCollider *owner );
	/*@}*/
};

#endif
