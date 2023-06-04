/* 
 * Drag[en]gine IGDE Animator Editor
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
#ifndef _AECLCLOSESTHIT_H_
#define _AECLCLOSESTHIT_H_

// includes
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>

// predefinitions



/**
 * Closest Hit Visitor.
 *
 * Stores the closest hit found while visitor collisions.
 */
class aeCLClosestHit : public deBaseScriptingCollider{
private:
	float pHitDistance;
	decVector pHitNormal;
	bool pHasHit;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	aeCLClosestHit();
	/** Cleans up the visitor. */
	virtual ~aeCLClosestHit();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Resets the visitor. */
	void Reset();
	
	/** Determines if an element has been hit. */
	inline bool GetHasHit() const{ return pHasHit; }
	/** Retrieves the hit distance. */
	inline float GetHitDistance() const{ return pHitDistance; }
	/** Retrieves the hit normal. */
	inline const decVector &GetHitNormal() const{ return pHitNormal; }
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/**
	 * Determines the response for a given collision. If you specified ertCustom
	 * you have to update the info object with the response to the collision. In
	 * all other cases you do must not modify the info object.
	 */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	/**
	 * Determines if a collider can be hit. If this peer is used with a collider
	 * then the script is asked to determine if the two collider can hit each
	 * other. If this peer is used in a ray test owner is NULL and the script
	 * is asked to determine if the ray can hit the collider.
	 * @param owner Collider this peer belongs to or NULL if a ray test is done.
	 * @param collider Collider to test.
	 * @return True if the owner/ray can hit the given collider.
	 */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	/**
	 * Notifies the scripts that the properties of this collider have changed and
	 * that the attached element has to update. This is usually called after the
	 * collision detection but can also be called multiple times.
	 */
	virtual void ColliderChanged( deCollider *owner );
	/*@}*/
};

// end of include only once
#endif
