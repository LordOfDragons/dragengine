/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _RECLAPPLYPUSH_H_
#define _RECLAPPLYPUSH_H_

#include "reCLHitList.h"

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>

class decLayerMask;
class deBasePhysicsWorld;
class reRig;
class deCollider;



/**
 * \brief Collider lister applying a push to the closest hit shape in the simulation collider.
 */
class reCLApplyPush : public deBaseScriptingCollider{
private:
	reRig *pRig;
	
	decDVector pRayPosition;
	decDVector pRayDirection;
	
	decVector pPush;
	
	deCollider *pCollider;
	
	deCollider *pClosestCollider;
	int pClosestBone;
	float pClosestDistance;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision listener. */
	reCLApplyPush( reRig *rig );
	/** Cleans up the collision listener. */
	virtual ~reCLApplyPush();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the ray position. */
	inline const decDVector &GetRayPosition() const{ return pRayPosition; }
	/** \brief Retrieves the ray direction. */
	inline const decDVector &GetRayDirection() const{ return pRayDirection; }
	
	/** Sets the ray. */
	void SetRay( const decDVector &position, const decVector &direction );
	/** Sets the push. */
	void SetPush( const decVector &push );
	/** Sets the collider. */
	void SetCollider( deCollider *collider );
	
	/** \brief Reset visitor. */
	void Reset();
	/** \brief Apply push to the closest shape if any. */
	void ApplyPush();
	
	/**
	 * \brief Helper method applying push to the closest shape if any in the world.
	 * \details Calls Reset(), then RayHits on the world with the provided
	 *          layer maks and then ApplyPush().
	 */
	void ApplyPushIn( deBasePhysicsWorld &phyWorld, const decLayerMask &layerMask );
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/**
	 * Determines the response for a given collision. If you specified ertCustom
	 * you have to update the info object with the response to the collision. In
	 * all other cases you do must not modify the info object.
	 */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	/**
	 * Determines if this collider can be hit by the given collider.
	 */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	/**
	 * Notifies the scripts that the properties of this collider have changed and
	 * that the attached element has to update. This is usually called after the
	 * collision detection but can also be called multiple times.
	 */
	virtual void ColliderChanged( deCollider *owner );
};

#endif
