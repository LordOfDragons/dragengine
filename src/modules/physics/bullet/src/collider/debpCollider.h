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

#ifndef _DEBPCOLLIDER_H_
#define _DEBPCOLLIDER_H_

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/systems/modules/physics/deBasePhysicsCollider.h>

class debpColliderAttachment;
class debpColliderCollisionTest;
class debpColliderComponent;
class debpColliderConstraint;
class debpColliderRig;
class debpColliderVolume;
class debpCollisionWorld;
class debpWorld;
class debpTouchSensor;
class debpCollisionTest;

class deCollider;
class deColliderCollisionTest;
class deDebugDrawerShape;
class dePhysicsBullet;

class btDynamicsWorld;



/**
 * @brief Bullet Collider.
 * Provides the base implementation of a collider.
 */
class debpCollider : public deBasePhysicsCollider{
public:
	/** Collider types. */
	enum eColliderTypes{
		ectVolume,
		ectComponent,
		ectRig
	};
	
private:
	int pType;
	
	dePhysicsBullet *pBullet;
	debpWorld *pParentWorld;
	deCollider &pCollider;
	decDVector pMinExtend, pMaxExtend;
	decDVector pShapeMinExtend, pShapeMaxExtend;
	decDMatrix pMatrix, pInvMatrix, pMatrixNormal;
	
	debpColliderAttachment **pAttachments;
	int pAttachmentCount;
	int pAttachmentSize;
	
	debpColliderConstraint **pConstraints;
	int pConstraintCount;
	int pConstraintSize;
	
	decTList<debpColliderCollisionTest*> pCollisionTests;
	
	decTOrderedSet<debpCollider*> pAttachedToList;
	decTOrderedSet<debpTouchSensor*> pTrackingTouchSensors;
	
	int pIndex;
	bool pIsMoving;
	bool pDirtyMatrix;
	bool pDirtyOctree;
	bool pRequiresUpdate;
	bool pUseKinematicSim;
	bool pIsPrepared;
	
	bool pMarked;
	bool pTouchSensorMarked;
	
	int pColDetPrepareIndex;
	bool pAutoColDetPrepare;
	int pColDetFinishIndex;
	bool pAutoColDetFinish;
	int pPPCTColliderIndex;
	int pUpdateOctreeIndex;
	
	deDebugDrawer::Ref pDebugDrawer;
	deDebugDrawerShape *pDDSShape;
	
	
	
public:
	// constructor, destructor
	debpCollider(dePhysicsBullet *bullet, deCollider &collider, int type);
	~debpCollider() override;
	
	// physics management
	/** Retrieves the type. */
	inline int GetType() const{ return pType; }
	/** Determines if this is a collider volume. */
	inline bool IsVolume() const{ return pType == ectVolume; }
	/** Determines if this is a collider component. */
	inline bool IsComponent() const{ return pType == ectComponent; }
	/** Determines if this is a collider rigged. */
	inline bool IsRigged() const{ return pType == ectRig; }
	/** Cast to collider volume. Throws an exception if this is not a collider volume. */
	debpColliderVolume *CastToVolume() const;
	/** Cast to collider component. Throws an exception if this is not a collider component. */
	debpColliderComponent *CastToComponent() const;
	/** Cast to collider rigged. Throws an exception if this is not a collider rigged. */
	debpColliderRig *CastToRigged() const;
	
	/** Retrieves the module. */
	inline dePhysicsBullet *GetBullet() const{ return pBullet; }
	
	inline deCollider &GetCollider() const{ return pCollider; }
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	inline debpWorld *GetParentWorld() const{ return pParentWorld; }
	inline int GetIndex() const{ return pIndex; }
	
	/** Requires update. */
	inline bool GetRequiresUpdate() const{ return pRequiresUpdate; }
	
	/** Requires update. */
	void RequiresUpdate();
	
	/** Clear requires update. */
	void ClearRequiresUpdate();
	
	inline bool GetIsMoving() const{ return pIsMoving; }
	void SetIsMoving(bool isMoving);
	const decDMatrix &GetMatrix();
	const decDMatrix &GetInverseMatrix();
	const decDMatrix &GetMatrixNormal();
	void SetIndex(int index);
	virtual void SetParentWorld(debpWorld *parentWorld);
	
	/** Parent world changed. */
	void ParentWorldChanged();
	
	void SetAddToOctree(bool doAdd);
	void SetExtends(const decDVector &minExtend, const decDVector &maxExtend);
	void MarkMatrixDirty();
	void MarkDirtyOctree();
	
	/** Two collider can collide. */
	bool Collides(const debpCollider &collider) const;
	
	/** Two collider can not collide. */
	bool CollidesNot(const debpCollider &collider) const;
	
	/** Create physics body if not existing already. */
	virtual void CreateBody();
	/** Destroy physics body if existing. */
	virtual void DestroyBody();
	/** Update collider state from physics body state. */
	virtual void UpdateFromBody();
	/** Update extends if required. */
	virtual void UpdateExtends();
	/** Update matrices if required. */
	virtual void UpdateMatrix();
	/** Prepare for a simulation step. */
	virtual void PrepareForStep();
	/** Detect collision for a custom collision step. */
	virtual void DetectCustomCollision(float elapsed);
	
	/** Prepares the collision detection. */
	virtual void PrepareDetection(float elapsed);
	/** Finished the collision detection updating the collider and send notifications. */
	virtual void FinishDetection();
	
	/** Updates the collision object aabbs if dirty. */
	virtual void UpdateCollisionObjectAABBs();
	
	/** Retrieves the dynamics world or NULL. */
	debpCollisionWorld *GetDynamicsWorld() const;
	
	/** Retrieves the marked flag. */
	inline bool GetMarked() const{ return pMarked; }
	/** Sets the marked flag. */
	inline void SetMarked(bool marked){pMarked = marked;}
	
	inline bool GetTouchSensorMarked() const{ return pTouchSensorMarked; }
	inline void SetTouchSensorMarked(bool marked){pTouchSensorMarked = marked;}
	
	/** Determines if this collider has to be simulated using kinematics. */
	inline bool GetUseKinematicSimulation() const{ return pUseKinematicSim; }
	/** Sets if this collider has to be simulated using kinematics. */
	void SetUseKinematicSimulation(bool useKinematicSimulation);
	
	/** Updates the octree position. */
	virtual void UpdateOctreePosition();
	
	/** Updates shapes with the current matirx. */
	virtual void UpdateShapes();
	/** Updates shapes using a transformation matrix. */
	virtual void UpdateShapesWithMatrix(const decDMatrix &transformation);
	/** Retrieves the minimum extend of the shapes. */
	inline const decDVector &GetShapeMinimumExtend() const{ return pShapeMinExtend; }
	/** Retrieves the maximum extend of the shapes. */
	inline const decDVector &GetShapeMaximumExtend() const{ return pShapeMaxExtend; }
	/** Sets the extends of the shapes. */
	void SetShapeExtends(const decDVector &minExtend, const decDVector &maxExtend);
	
	/**
	 * Prepare constraints for next detection step.
	 * \details Required to deal with advanced features like joint frictions.
	 */
	void PrepareConstraintsForStep();
	
	/** Check if collider constraints broke and notify the scripting module if required. */
	void CheckColliderConstraintsBroke();
	
	/** Process collider collision tests. */
	void ProcessColliderCollisionTests();
	
	
	
	/** Register for prepare collision detection if not registered yet. */
	void RegisterColDetPrepare();
	
	/** Unregister for prepare collision detection if registered. */
	void UnregisterColDetPrepare();
	
	/** Prepare collision detection index or -1 if not registered. */
	inline int GetColDetPrepareIndex() const{ return pColDetPrepareIndex; }
	
	/** Set prepare collision detection index or -1 if not registered. */
	void SetColDetPrepareIndex(int index);
	
	/** Automatically re-registered for collision detection prepare. */
	inline bool GetAutoColDetPrepare() const{ return pAutoColDetPrepare; }
	
	/** Set automatically re-registered for collision detection prepare. */
	void SetAutoColDetPrepare(bool autoColDetPrepare);
	
	/** Calculate auto collision detection re-register value. */
	virtual bool CalcAutoColDetPrepare();
	
	
	
	/** Register for finish collision detection if not registered yet. */
	void RegisterColDetFinish();
	
	/** Unregister for finish collision detection if registered. */
	void UnregisterColDetFinish();
	
	/** Finish collision detection index or -1 if not registered. */
	inline int GetColDetFinishIndex() const{ return pColDetFinishIndex; }
	
	/** Set finish collision detection index or -1 if not registered. */
	void SetColDetFinishIndex(int index);
	
	/** Automatically re-registered for collision detection finish. */
	inline bool GetAutoColDetFinish() const{ return pAutoColDetFinish; }
	
	/** Set automatically re-registered for collision detection finish. */
	void SetAutoColDetFinish(bool autoColDetFinish);
	
	/** Calculate auto collision detection re-register value. */
	virtual bool CalcAutoColDetFinish();
	
	
	
	/** Register for post physics collision processing not registered yet. */
	void RegisterPPCProcessing();
	
	/** Unregister for post physics collision processing if registered. */
	void UnregisterPPCProcessing();
	
	/** Post physics collision processing index or -1 if not registered. */
	inline int GetPPCProcessingIndex() const{ return pPPCTColliderIndex; }
	
	/** Set post physics collision processing index or -1 if not registered. */
	void SetPPCProcessingIndex(int index);
	
	
	
	/** Register for update octree processing not registered yet. */
	void RegisterUpdateOctree();
	
	/** Unregister for update octree processing if registered. */
	void UnregisterUpdateOctree();
	
	/** Update octree processing index or -1 if not registered. */
	inline int GetUpdateOctreeIndex() const{ return pUpdateOctreeIndex; }
	
	/** Set update octree processing index or -1 if not registered. */
	void SetUpdateOctreeIndex(int index);
	
	/** Prepare for static collsion test. Returns true if ready or false if not usable. */
	virtual bool PrepareStaticCollisionTest() = 0;
	/*@}*/
	
	
	
	/** @name Attachments */
	/*@{*/
	/** Retrieves the number of attachments. */
	inline int GetAttachmentCount() const{ return pAttachmentCount; }
	/** Retrieves an attachment. */
	debpColliderAttachment *GetAttachmentAt(int index) const;

	/** Retrieves the list of colliders this collider is attached to. */
	inline decTOrderedSet<debpCollider*> &GetAttachedToList(){ return pAttachedToList; }
	inline const decTOrderedSet<debpCollider*> &GetAttachedToList() const{ return pAttachedToList; }

	/** List of touch sensors tracking this collider. */
	inline decTOrderedSet<debpTouchSensor*> &GetTrackingTouchSensors(){ return pTrackingTouchSensors; }
	inline const decTOrderedSet<debpTouchSensor*> &GetTrackingTouchSensors() const{ return pTrackingTouchSensors; }
	/*@}*/



	/** \name Constraints */
	/*@{*/
	/** Retrieves the number of constraints. */
	inline int GetConstraintCount() const{ return pConstraintCount; }
	/** Retrieves the constraint at the given index. */
	debpColliderConstraint *GetConstraintAt(int index) const;
	/*@}*/
	
	
	
	/** \name Post physics collision tests */
	/*@{*/
	/** Number of post physics collision tests. */
	int GetCollisionTestCount() const;
	
	/** Post physics collision test at index. */
	debpColliderCollisionTest *GetCollisionTestAt(int index) const;
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** Debug drawer or \em NULL if not activated .*/
	inline const deDebugDrawer::Ref &GetDebugDrawer() const{ return pDebugDrawer; }
	
	/** Debug drawer shape or \em NULL if not ativated. */
	inline deDebugDrawerShape *GetDDSShape() const{ return pDDSShape; }
	
	/** Update debug drawer if developer mode is enabled. */
	virtual void UpdateDebugDrawer();
	
	/**
	 * Update debug drawer shape shape.
	 * \details Called after creating debug drawer or if the collider subclass requires an update.
	 */
	virtual void UpdateDDSShape();
	
	/** Rigid body deactivated state. */
	virtual bool GetRigidBodyDeactivated() const;
	/*@}*/
	
	
	
	/** @name Notifications */
	/*@{*/
	/** Position changed. */
	void PositionChanged() override;
	
	/** Orientation changed. */
	void OrientationChanged() override;
	
	/** Scale changed. */
	void ScaleChanged() override;
	
	/** Position or orientation changed. */
	void GeometryChanged() override;
	
	/** Linear velocity changed. */
	void LinearVelocityChanged() override;
	/** Angular velocity changed. */
	void AngularVelocityChanged() override;
	/** Enabled changed. */
	void EnabledChanged() override;
	/** Gravity changed. */
	void GravityChanged() override;
	/** Properties like mass changed. */
	void PropertiesChanged() override;
	/** Response type changed. */
	void ResponseTypeChanged() override;
	
	/** Collision filter changed. */
	void CollisionFilterChanged() override;
	
	/** Ignore colliders changed. */
	void IgnoreCollidersChanged() override;
	
	/** Force field factor changed. */
	void ForceFieldChanged() override;
	
	/** Attachment added. */
	void AttachmentAdded(int index, deColliderAttachment *attachment) override;
	/** Attachment changed. */
	void AttachmentChanged(int index, deColliderAttachment *attachment) override;
	/** Attachment removed. */
	void AttachmentRemoved(int index, deColliderAttachment *attachment) override;
	/** All attachments removed. */
	void AllAttachmentsRemoved() override;
	
	/** Constraint added. */
	void ConstraintAdded(int index, deColliderConstraint *attachment) override;
	/** Constraint changed. */
	void ConstraintChanged(int index, deColliderConstraint *attachment) override;
	/** Constraint removed. */
	void ConstraintRemoved(int index, deColliderConstraint *attachment) override;
	/** All attachments removed. */
	void AllConstraintsRemoved() override;
	
	
	
	/** Post physics collision test added. */
	void CollisionTestAdded(int index) override;
	
	/** Post physics collision test changed. */
	void CollisionTestChanged(int index) override;
	
	/** Post physics collision test enabled changed. */
	void CollisionTestEnabledChanged(int index) override;
	
	/** Post physics collision test removed. */
	void CollisionTestRemoved(int index) override;
	
	/** All post physics collision tests removed. */
	void AllCollisionTestsRemoved() override;
	/*@}*/
	
	
	
protected:
	inline bool GetIsPrepared() const{ return pIsPrepared; }
	
private:
	void pCleanUp();
	void pRemoveFromAllTrackingTouchSensors();
};

#endif
