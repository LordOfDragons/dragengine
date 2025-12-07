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

#ifndef _DEBPCOLLISIONWORLD_H_
#define _DEBPCOLLISIONWORLD_H_

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "../coldet/debpBulletShapeCollision.h"

#include <dragengine/common/utils/decTimer.h>

class debpDelayedOperation;
class debpWorld;
class debpConstraintSolver;



/**
 * \brief Bullet btSoftMultiBodyDynamicsWorld with some additions.
 */
class debpCollisionWorld : public btSoftRigidDynamicsWorld{
private:
	debpWorld &pWorld;
	debpDelayedOperation *pDelayedOperation;
	decTimer pPerfTimer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new collision world. */
	debpCollisionWorld(debpWorld &world, btDispatcher *dispatcher, btBroadphaseInterface *pairCache,
		debpConstraintSolver *constraintSolver, btCollisionConfiguration *collisionConfiguration,
		btSoftBodySolver *softBodySolver);
	/** \brief Cleans up the collision world. */
	virtual ~debpCollisionWorld();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the world. */
	inline debpWorld &GetWorld(){ return pWorld; }
	inline const debpWorld &GetWorld() const{ return pWorld; }
	/** \brief Retrieves the delayed operation object. */
	inline debpDelayedOperation &GetDelayedOperation(){ return *pDelayedOperation; }
	inline const debpDelayedOperation &GetDelayedOperation() const{ return *pDelayedOperation; }
	
	/** \brief Mark all collision objects AABB valid. */
	void MarkAllAABBValid();
	/** \brief Update all dirty collision objects AABBs. */
	void UpdateDirtyAABBs();
	
// 	virtual void updateAabbs();
	
	/** \brief Check for dynamic collisions after a simulation step. */
	void CheckDynamicCollisions(btScalar timeStep);
	
	/**
	 * \brief Script callback safe ray testing.
	 * 
	 * performs a raycast on all objects in the btCollisionWorld, and calls the resultCallback
	 * This allows for several queries: first hit, all hits, any hit, dependent on the value
	 * returned by the callback.
	 * 
	 * Protects btCollisionWorld::rayTest by locking delayed operations.
	 * 
	 * \note no code seems to be using this.
	 */
	void safeRayTest(const btVector3 &rayFromWorld, const btVector3 &rayToWorld,
		RayResultCallback &resultCallback) const;
	
	/**
	 * \brief Script callback safe convex sweep testing.
	 * 
	 * performs a swept convex cast on all objects in the btCollisionWorld, and calls the
	 * resultCallback. This allows for several queries: first hit, all hits, any hit, dependent
	 * on the value return by the callback.
	 * 
	 * Protects btCollisionWorld::convexSweepTest by locking delayed operations.
	 * 
	 * \note used by debpSweepCollisionTest.
	 */
	void safeConvexSweepTest(const btConvexShape *castShape, const btTransform &from,
		const btTransform &to, ConvexResultCallback &resultCallback,
		btScalar allowedCcdPenetration = (btScalar)0);
	
	/**
	 * \brief Script callback safe contact testing.
	 * 
	 * performs a discrete collision test between colObj against all objects in the
	 * btCollisionWorld, and calls the resultCallback. it reports one or more contact points
	 * for every overlapping object (including the one with deepest penetration).
	 * 
	 * Protects btCollisionWorld::contactTest by locking delayed operations.
	 * 
	 * \note no code seems to be using this.
	 */
	void safeContactTest(btCollisionObject *colObj, ContactResultCallback &resultCallback);
	
	/**
	 * \brief Script callback safe contact testing.
	 * 
	 * performs a discrete collision test between two collision objects and calls the
	 * resultCallback if overlap if detected. it reports one or more contact points
	 * (including the one with deepest penetration)
	 * 
	 * Protects btCollisionWorld::contactPairTest by locking delayed operations.
	 * 
	 * \note used by debpTouchSensor.
	 */
	void safeContactPairTest(btCollisionObject *colObjA, btCollisionObject *colObjB,
		ContactResultCallback& resultCallback);
	
	/**
	 * \brief Script callback safe contact testing.
	 * 
	 * Modified version of safeContactPairTest returning only true or false. This version
	 * avoids juggling contact points not required for a quick test.
	 * 
	 * \note used by debpTouchSensor.
	 */
	bool safeContactPairTest(btCollisionObject *colObjA, btCollisionObject *colObjB);
	
	
	
	/**
	 * \brief Performs a swept convex cast on all objects in the debpCollisionWorld calling resultCallback.
	 * 
	 * Allows for several queries: first hit, all hits, any hit, dependent on the value return by
	 * the callback. Shadows convexSweepTest in btCollisionWorld to use faster collision
	 * detection algorithms in certain cases. The shadowed call only works through debpCollisionWorld
	 * since convexSweepTest in btCollisionWorld is unfortunately static.
	 * 
	 * \note hides function btCollisionWorld::convexSweepTest which is not virtual.
	 * 
	 * \note used by debpParticleEmitterInstanceType
	 */
	void convexSweepTest(const btConvexShape *castShape, const btTransform &from, const btTransform &to,
	ConvexResultCallback &resultCallback,  btScalar allowedCcdPenetration = (btScalar)0.0);
	
	
	
	/**
	 * \brief Performs a collision detection query and calls the resultCallback.
	 * 
	 * Used internally by rayTest and convexSweepTest. Shadows objectQuerySingle in btCollisionWorld
	 * to use faster collision detection algorithms in certain cases. The shadowed call only works
	 * through debpCollisionWorld since objectQuerySingle in btCollisionWorld is unfortunately static.
	 * 
	 * \note hides function btCollisionWorld::objectQuerySingle which is not virtual.
	 * 
	 * \note used by debpSweepCollisionTest.
	 */
	void objectQuerySingle(const btConvexShape *castShape, const btTransform &convexFromTrans,
	const btTransform &convexToTrans, btCollisionObject *collisionObject,
	const btCollisionShape *collisionShape, const btTransform &colObjWorldTransform,
	ConvexResultCallback &resultCallback, btScalar allowedPenetration);
	
	
	
	/**
	 * \brief Test for collisions of a collider with the world.
	 * \details Modified version of btCollisionWorld::contactTest. Avoids using bullet collision algorithm
	 *          usage due to speed problems. The bullet collision algorithms used in btCollisionWorld::contactTest
	 *          try to calculate all contact points instead of exiting at the first collision found. This is
	 *          especially a problem with triangle meshes where every triangle is tested all the time. This
	 *          version uses dragengine collision testing while still taking advantage of bullet broadphase
	 *          optimizations. Except from the changed collision testing the method behaves the same as
	 *          btCollisionWorld::contactTest. For this reason the method is also implemented to shadow
	 *          btCollisionWorld::contactTest. This way the temporary hack is transparent and can be removed
	 *          or altered once bullet does this kind of collision testing fast.
	 * 
	 * \note hides function btCollisionWorld::contactTest which is not virtual.
	 * 
	 * \note used by debpCollisionWorld::SingleSweepCallback.
	 */
	void contactTest(btCollisionObject *colObj, ContactResultCallback &resultCallback);
	
	/** \brief Solve constraints. */
	virtual void solveConstraints(btContactSolverInfo &solverInfo);
	
#if 0
	/**
	 * rayTest performs a raycast on all objects in the btCollisionWorld, and calls the resultCallback
	 * This allows for several queries: first hit, all hits, any hit, dependent on the value returned by the callback.
	 * Modified to use own collision tests where possible.
	 */
	virtual void rayTest(const btVector3 &rayFromWorld, const btVector3 &rayToWorld, RayResultCallback &resultCallback) const;
	
	/**
	 * Modified version of btCollisionWorld::rayTestSingle. Uses own collision tests where possible. For all other
	 * cases uses the original btCollisionWorld::rayTestSingle method.
	 */
	void RayTestSingle(const btTransform &rayFromTrans, const btTransform &rayToTrans,
		btCollisionObject *collisionObject, const btCollisionShape *collisionShape,
		const btTransform &colObjWorldTransform, RayResultCallback &resultCallback) const;
#endif
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	int GetNumNonStaticRigidBodies() const;
	/*@}*/
	
protected:
	/**
	 * \brief Performs an single simulation step.
	 * \details Used by btSoftMultiBodyDynamicsWorld. Overloaded to lock the delayed operation while running.
	 */
	virtual void internalSingleStepSimulation(btScalar timeStep);
};

#endif
