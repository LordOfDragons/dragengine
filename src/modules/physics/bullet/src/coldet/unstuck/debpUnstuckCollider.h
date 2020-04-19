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

#ifndef _DEBPUNSTUCKCOLLIDER_H_
#define _DEBPUNSTUCKCOLLIDER_H_

#include <dragengine/common/math/decMath.h>

class debpCollider;
class debpWorld;



/**
 * @brief Unstuck Helper.
 * Provides support to free moving colliders from a stuck position. For this
 * a list of all colliders touching the stuck collider is stored together
 * with the collision parameters in the stuck situation. Using this information
 * an unstuck direction is calculated along which the collider can be pushed
 * to get out of the stuck position. If possible the direction is tried to
 * be perpendicular to the moving direction as this results in the least
 * noticeable movement.
 */
class debpUnstuckCollider{
private:
	debpWorld &pWorld;
	debpCollider *pStuckCollider;
	decDVector pDisplacement;
	
	debpCollider **pNearbyColliders;
	int pNearbyColliderCount;
	int pNearbyColliderSize;
	
	decDVector *pBlockerNormals;
	int pBlockerNormalCount;
	int pBlockerNormalSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new unstuck collider object. */
	debpUnstuckCollider( debpWorld &world );
	/** Cleans up the unstuck collider object. */
	~debpUnstuckCollider();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the stuck collider. */
	inline debpCollider *GetStuckCollider() const{ return pStuckCollider; }
	/** Sets the stuck collider. */
	void SetStuckCollider( debpCollider *collider );
	/** Retrieves the displacement. */
	inline const decDVector &GetDisplacement() const{ return pDisplacement; }
	/** Sets the displacement. */
	void SetDisplacement( const decDVector &displacement );
	/** Remove all colliders and blocker normals. */
	void Reset();
	/** Determine nearby colliders in the parent world. */
	void FindNearbyColliders();
	/** Determine blocker normals amongst all nearby colliders. */
	void FindBlockerNormals();
	/**
	 * Tries to unstuck the collider. If successful the collider is moved to the new
	 * location and true is returned. If unsuccessful the collider stays where it is
	 * and false is returned.
	 */
	bool UnstuckCollider();
	
	/** Retrieves the number of nearby colliders. */
	inline int GetNearbyColliderCount() const{ return pNearbyColliderCount; }
	/** Retrieves a nearby collider. */
	debpCollider *GetNearbyColliderAt( int index ) const;
	/** Adds a nearby collider. */
	void AddNearbyCollider( debpCollider *collider );
	/** Removes all nearby colliders. */
	void RemoveAllNearbyColliders();
	
	/** Retrieves the number of blocker normals. */
	inline int GetBlockerNormalCount() const{ return pBlockerNormalCount; }
	/** Retrieves a blocker normal. */
	const decDVector &GetBlockerNormalAt( int index ) const;
	/** Adds a blocker normal. */
	void AddBlockerNormal( const decDVector &normal );
	/** Removes all blocker normals. */
	void RemoveAllBlockerNormals();
	/*@}*/
};

#endif
