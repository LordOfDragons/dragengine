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

#ifndef _DEBPCLOSESTCONVEXRESULTCALLBACK_H_
#define _DEBPCLOSESTCONVEXRESULTCALLBACK_H_

#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"

#include <dragengine/common/math/decMath.h>

class debpHeightTerrain;
class deCollisionInfo;
class debpHTSector;
class debpCollider;
class debpColliderVolume;
class debpColliderComponent;

class deBaseScriptingCollider;
class deCollider;
class deCollisionInfo;
class debpDCollisionVolume;



/**
 * \brief Modification of the \ref btCollisionWorld::ClosestConvexResultCallback struct for
 *        collision testing with bullet specific collision tests in btCollisionWorld.
 */
class debpClosestConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback{
private:
	debpCollider *pCollider;
	debpColliderVolume *pColliderVolume;
	debpColliderComponent *pColliderComponent;
	decDVector pMoveDirection;
	decVector pRotation;
	
	debpHTSector *pHitHTSector;
	deCollider *pHitCollider;
	int pHitBone;
	int pHitShape;
	int pHitFace;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new result callback. */
	debpClosestConvexResultCallback();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Sets the collider to test. */
	void SetTestCollider( debpCollider *bpCollider );
	/** \brief Sets the collider to test. */
	void SetTestCollider( debpCollider *bpCollider, const decDVector &displacement );
	
	/** \brief Determine if the move direction is not zero. */
	bool MoveDirIsNotZero() const;
	
	/** \brief Determine if the move direction is zero. */
	bool MoveDirIsZero() const;
	
	/** \brief Determine if the rotation is not zero. */
	bool RotationNotZero() const;
	
	/** \brief Determine if the rotation is zero. */
	bool RotationZero() const;
	
	/** \brief Retrieves the move direction. */
	inline const decDVector &GetMoveDirection() const{ return pMoveDirection; }
	/** \brief Determines if a collision has been found for the current test collider. */
	inline bool HasLocalCollision() const{ return hasHit(); }
	/** \brief Retrieves the hit distance. */
	inline float GetHitDistance() const{ return ( float )m_closestHitFraction; }
	
	/** \brief Reset the visitor. */
	void Reset();
	
	/** \brief Updates a collision info object with the found results. */
	void GetResult( deCollisionInfo &collisionInfo ) const;
	/*@}*/
	
	/** \name Bullet */
	/*@{*/
	/** \brief Determines if a collision is possible. */
	virtual bool needsCollision( btBroadphaseProxy *proxy0 ) const;
	/** \brief Adds a result. This fills in the parameters for later delivery to the collision info. */
	virtual btScalar addSingleResult( btCollisionWorld::LocalConvexResult &convexResult, bool normalInWorldSpace );
	/*@}*/
};

#endif
