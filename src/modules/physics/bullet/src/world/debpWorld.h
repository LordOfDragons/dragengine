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

#ifndef _DEBPWORLD_H_
#define _DEBPWORLD_H_

#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/systems/modules/physics/deBasePhysicsWorld.h>

class deWorld;

class dePhysicsBullet;
class debpHeightTerrain;
class debpCollisionWorld;
class debpGhostPairCallback;
class debpPhysicsBody;
class debpOverlapFilterCallback;
class debpUnstuckCollider;
class debpCollider;
class debpSharedCollisionFiltering;
class debpConstraintSolver;

class btDynamicsWorld;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btCollisionConfiguration;
struct btSoftBodyWorldInfo;
class btSoftBodySolver;


/**
 * Physics world peer.
 */
class debpWorld : public deBasePhysicsWorld{
private:
	dePhysicsBullet &pBullet;
	deWorld &pWorld;
	deCollisionInfo::Ref pColInfo;
	debpUnstuckCollider *pUnstuckCollider;
	
	debpHeightTerrain *pHeightTerrain;
	
	debpSharedCollisionFiltering *pSharedCollisionFiltering;
	
	debpCollisionWorld *pDynWorld;
	btBroadphaseInterface *pBroadPhase;
	btCollisionDispatcher *pColDisp;
	debpConstraintSolver *pConstraintSolver;
	btSoftBodySolver *pSoftBodySolver;
	btCollisionConfiguration *pColConfig;
	btSoftBodyWorldInfo	*pSoftBodyWorldInfo;
	debpOverlapFilterCallback *pOverlapFilterCallback;
	debpGhostPairCallback *pGhostPairCallback;
	
	bool pDirtyOctree;
	bool pDirtyDynWorldAABB;
	
	float pLeftOverTime;
	
	deCollider *pDetNextCollider;
	deTouchSensor *pDetNextTouchSensor;
	
	debpCollider **pColDetPrepareColliders;
	int pColDetPrepareColliderCount;
	int pColDetPrepareColliderSize;
	int pColDetPrepareColliderProcessCount;
	
	debpCollider **pColDetFinishColliders;
	int pColDetFinishColliderCount;
	int pColDetFinishColliderSize;
	
	debpCollider **pPPCTColliders;
	int pPPCTColliderCount;
	int pPPCTColliderSize;
	
	debpCollider **pUpdateOctreeColliders;
	int pUpdateOctreeColliderCount;
	int pUpdateOctreeColliderSize;
	
	int pSimMaxSubStep;
	float pSimTimeStep;
	
	decVector pGravity;
	float pDynCollisionVelocityThreshold;
	
	bool pProcessingPhysics;
	
	decTimer pPerfTimer;
	decTimer pPerfTimer2;
	
	decLayerMask pLastDebugShowCategory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create world peer. */
	debpWorld(dePhysicsBullet &bullet, deWorld &world);
	
	/** Clean up world peer. */
	virtual ~debpWorld();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline dePhysicsBullet &GetBullet() const{ return pBullet; }
	
	/** Engine world resource. */
	inline deWorld &GetWorld() const{ return pWorld; }
	
	/** Shared collision information. */
	inline const deCollisionInfo::Ref &GetCollisionInfo() const{ return pColInfo; }
	
	/** Unstuck collider. */
	inline debpUnstuckCollider *GetUnstuckCollider() const{ return pUnstuckCollider; }
	
	
	
	/** Maximum sub steps for dynamic simulation. */
	inline int GetSimulationMaxSubSteps() const{ return pSimMaxSubStep; }
	
	/** Fixed time step for dynamic simulation. */
	inline float GetSimulationTimeStep() const{ return pSimTimeStep; }
	
	
	
	/** Gravity. */
	inline const decVector &GetGravity() const{ return pGravity; }
	
	/**
	 * Dynamic collision velocity threshold.
	 * \see debpCollisionWorld::CheckDynamicCollisions.
	 */
	inline float GetDynamicCollisionVelocityThreshold() const{ return pDynCollisionVelocityThreshold; }
	
	/** In progress of processing physics. */
	inline bool GetProcessingPhysics() const{ return pProcessingPhysics; }
	
	
	
	/** Update octrees if dirty. */
	void UpdateOctrees();
	
	/**
	 * Mark octrees dirty.
	 * \details Used by world elements to request an octree update the next time.
	 */
	void MarkOctreeDirty();
	
	/** Update dynamic world aabbs. */
	void UpdateDynWorldAABBs();
	
	
	
	/** Add collider for prepare collision detection. */
	void pColDetPrepareColliderAdd(debpCollider *collider);
	
	/** Remove collider for prepare collision detection. */
	void pColDetPrepareColliderRemove(debpCollider *collider);
	
	
	
	/** Add collider for finish collision detection. */
	void pColDetFinishColliderAdd(debpCollider *collider);
	
	/** Remove collider for finish collision detection. */
	void pColDetFinishColliderRemove(debpCollider *collider);
	
	
	
	/** Add collider for post physics collision test processing. */
	void pPPCTColliderAdd(debpCollider *collider);
	
	/** Remove collider for post physics collision test processing. */
	void pPPCTColliderRemove(debpCollider *collider);
	
	
	
	/** Add collider for update octree processing. */
	void pUpdateOctreeColliderAdd(debpCollider *collider);
	
	/** Remove collider for update octree processing. */
	void pUpdateOctreeColliderRemove(debpCollider *collider);
	
	
	
	/** Shared collision filteringNULL. */
	inline debpSharedCollisionFiltering &GetSharedCollisionFiltering() const{ return *pSharedCollisionFiltering; }
	
	
	
	/** Height terrain or \em NULL. */
	inline debpHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** Dynamics world. */
	inline debpCollisionWorld *GetDynamicsWorld() const{ return pDynWorld; }
	
	
	
	/**
	 * Update dynamic parts of scene.
	 * \param elapsed Seconds elapsed since last update
	 */
	virtual void Update(float elapsed);
	/**
	 * Process physics simulation using the physics module.
	 * \details Apply collision detection on moving kinematic collider, physical simulation
	 *          on dynamic colliders, particle and prop field simulations as well as moving
	 *          colliders attached to other colliders.
	 */
	virtual void ProcessPhysics(float elapsed);
	/*@}*/
	
	
	
	/** @name Notifications */
	/*@{*/
	/** Size changed. */
	virtual void SizeChanged();
	/** Physics properties changed. */
	virtual void PhysicsChanged();
	
	/** Height Terrain changed. */
	virtual void HeightTerrainChanged();
	
	/** Component has been added. */
	virtual void ComponentAdded(deComponent *component);
	/** Component has been removed. */
	virtual void ComponentRemoved(deComponent *component);
	/** All components have been removed. */
	virtual void AllComponentsRemoved();
	
	/** Collider has been added. */
	virtual void ColliderAdded(deCollider *collider);
	/** Collider has been removed. */
	virtual void ColliderRemoved(deCollider *collider);
	/** All colliders have been removed. */
	virtual void AllCollidersRemoved();
	
	/** Touch sensor has been added. */
	virtual void TouchSensorAdded(deTouchSensor *touchSensor);
	/** Touch sensor has been removed. */
	virtual void TouchSensorRemoved(deTouchSensor *touchSensor);
	/** All touch sensors have been removed. */
	virtual void AllTouchSensorsRemoved();
	
	/** Prop field has been added. */
	virtual void PropFieldAdded(dePropField *propField);
	/** Prop field has been removed. */
	virtual void PropFieldRemoved(dePropField *propField);
	/** All prop fields have been removed. */
	virtual void AllPropFieldsRemoved();
	
	/** Force field has been added. */
	virtual void ForceFieldAdded(deForceField *forceField);
	/** Force field has been removed. */
	virtual void ForceFieldRemoved(deForceField *forceField);
	/** All force fields have been removed. */
	virtual void AllForceFieldsRemoved();
	
	/** Prop field has been added. */
	virtual void ParticleEmitterAdded(deParticleEmitterInstance *emitter);
	/** Prop field has been removed. */
	virtual void ParticleEmitterRemoved(deParticleEmitterInstance *emitter);
	/** All prop fields have been removed. */
	virtual void AllParticleEmittersRemoved();
	/*@}*/
	
	
	
	/** @name Collision Detection */
	/*@{*/
	/**
	 * Test a point for collision with colliders.
	 * 
	 * For each collision the collisionResponse function the given listener is called. To
	 * stop testing set StopTesting in the provided collision information object to true.
	 */
	virtual void PointHits(const decDVector &point, deBaseScriptingCollider *listener,
	const decCollisionFilter &collisionFilter);
	
	/**
	 * Tests a ray for collision with the element in the world.
	 * \details For each collision the collisionResponse function the given listener is called. To
	 *          stop testing set StopTesting in the provided collision information object to true.
	 */
	virtual void RayHits(const decDVector &rayOrigin, const decVector &rayDirection,
	deBaseScriptingCollider *listener, const decCollisionFilter &collisionFilter);
	
	/**
	 * Tests the collider for collision with world elements.
	 * \details For each collision the collisionResponse function of the listener is called. To
	 *          stop testing set StopTesting in the provided collision information object to true.
	 */
	virtual void ColliderHits(deCollider *collider, deBaseScriptingCollider *listener);
	
	/**
	 * Tests the moving collider for collision with world elements.
	 * \details For each collision starting with the earliest the collisionResponse function
	 *          of the listener is called. To stop testing set StopTesting in the provided
	 *          collision information object to true.
	 */
	virtual void ColliderMoveHits(deCollider *collider, const decVector &displacement,
	deBaseScriptingCollider *listener);
	
	/**
	 * Tests the rotating collider for collision with world elements.
	 * \details For each collision starting with the earliest the collisionResponse function
	 *          of the listener is called. To stop testing set StopTesting in the provided
	 *          collision information object to true.
	 */
	virtual void ColliderRotateHits(deCollider *collider, const decVector &rotation,
	deBaseScriptingCollider *listener);
	
	/**
	 * Tests the moving and rotating collider for collision with world elements.
	 * \details For each collision starting with the earliest the collisionResponse function
	 *          of the listener is called. To stop testing set StopTesting in the provided
	 *          collision information object to true.
	 */
	virtual void ColliderMoveRotateHits(deCollider *collider, const decVector &displacement,
	const decVector &rotation, deBaseScriptingCollider *listener);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pProcessPhysics(float elapsed);
	
	void pPrepareDetection(float elapsed);
	
	void pPrepareForStep();
// 	bool pStepPhysics();
	void pStepForceFields(float elapsed);
	
	void pPrepareParticleEmitters(float elapsed);
	void pStepParticleEmitters(float elapsed);
	
	void pUpdateFromBody();
	void pFinishDetection();
	
	/** Update collider post physics collision tests. */
	void pUpdatePostPhysicsCollisionTests();
	
	/**
	 * Make touch sensors notify their peers about touch changes accumulated during collision detection.
	 * \details This potentially modifies colliders including adding or removing them.
	 */
	void pApplyTouchSensorChanges();
};

#endif
