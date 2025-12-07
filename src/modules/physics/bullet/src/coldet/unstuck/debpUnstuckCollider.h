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
	debpUnstuckCollider(debpWorld &world);
	/** Cleans up the unstuck collider object. */
	~debpUnstuckCollider();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the stuck collider. */
	inline debpCollider *GetStuckCollider() const{ return pStuckCollider; }
	/** Sets the stuck collider. */
	void SetStuckCollider(debpCollider *collider);
	/** Retrieves the displacement. */
	inline const decDVector &GetDisplacement() const{ return pDisplacement; }
	/** Sets the displacement. */
	void SetDisplacement(const decDVector &displacement);
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
	debpCollider *GetNearbyColliderAt(int index) const;
	/** Adds a nearby collider. */
	void AddNearbyCollider(debpCollider *collider);
	/** Removes all nearby colliders. */
	void RemoveAllNearbyColliders();
	
	/** Retrieves the number of blocker normals. */
	inline int GetBlockerNormalCount() const{ return pBlockerNormalCount; }
	/** Retrieves a blocker normal. */
	const decDVector &GetBlockerNormalAt(int index) const;
	/** Adds a blocker normal. */
	void AddBlockerNormal(const decDVector &normal);
	/** Removes all blocker normals. */
	void RemoveAllBlockerNormals();
	/*@}*/
};

#endif
