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
