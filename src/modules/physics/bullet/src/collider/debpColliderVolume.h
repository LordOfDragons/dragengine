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

// include only once
#ifndef _DEBPCOLLIDERVOLUME_H_
#define _DEBPCOLLIDERVOLUME_H_

// includes
#include "debpCollider.h"
#include "../shape/debpShapeList.h"
#include "../debpCollisionObject.h"

class debpGhostObject;
class btGhostObject;
class debpSweepCollisionTest;
class deColliderVolume;
class debpMotionState;
class debpPhysicsBody;
class debpBulletShape;
class deCollisionInfo;



/**
 * @brief Bullet Collider Volume.
 * Provides collision handling using a set of collision
 * volumes considered as one collision volume.
 */
class debpColliderVolume : public debpCollider{
private:
	deColliderVolume &pColliderVolume;
	debpPhysicsBody *pPhyBody;
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	decVector pLinVelo;
	decVector pAngVelo;
	decVector pGravity;
	decVector pPredictDisp;
	decVector pPredictLinVelo;
	decVector pPredictRot;
	bool pHasGravity;
	bool pHasLinVelo;
	bool pHasAngVelo;
	bool pDirtyBPShape;
	bool pDirtyShapes;
	bool pDirtySweepTest;
	bool pDirtyStaticTest;
	bool pDirtyAttachments;
	bool pPreventUpdate;
	bool pPreventAttNotify;
	bool pUseFakeDynamics;
	
	debpShapeList pShapes;
	debpSweepCollisionTest *pSweepCollisionTest;
	
	btGhostObject *pStaticCollisionTest;
	debpBulletShape *pStaticCollisionTestShape;
	debpCollisionObject pStaticCollisionTestObject;
	
	debpGhostObject *pGhostKinematicMovement;
	
public:
	// constructor, destructor
	debpColliderVolume(dePhysicsBullet *bullet, deColliderVolume &collider);
	~debpColliderVolume();
	// information
	inline const decVector &GetPredictedDisplacement() const{return pPredictDisp;}
	inline const decVector &GetPredictedRotation() const{return pPredictRot;}
	// physics management
	inline deColliderVolume &GetColliderVolume() const{return pColliderVolume;}
	
	/** Retrieves the list of shapes. */
	inline debpShapeList &GetShapes(){return pShapes;}
	inline const debpShapeList &GetShapes() const{return pShapes;}
	
	/** Retrieves the physics body. */
	inline debpPhysicsBody *GetPhysicsBody() const{return pPhyBody;}
	/** Determines if the collider has to be simulated using fake dynamics. */
	inline bool GetUseFakeDynamics() const{return pUseFakeDynamics;}
	
	/** Retrieves the gravity. */
	inline const decVector &GetGravity() const{return pGravity;}
	/** Retrieves the linear velocity. */
	inline const decVector &GetLinearVelocity() const{return pLinVelo;}
	
	/** \brief Mark shapes dirty. */
	void DirtyBPShape();
	
	/** \brief Mark attachments dirty. */
	void DirtyAttachments();
	
	/** Applies the gravity to the linear velocity for kinematic simulation. */
	void ApplyGravity(float elapsed);
	/** Predict the displacement and rotation. */
	void PredictDisplacement(float elapsed);
	/** Interpolate position using the predicted displacement and rotation. */
	void InterpolatePosition(float percentage);
	/** Applies the predicted displacement and rotation to the position. */
	void ApplyDisplacement();
	/** Apply fake dynamics response. */
	void ApplyFakeDynamicResponse(deCollisionInfo &colinfo);
	
	/** Updates shapes with the current matirx. */
	void UpdateShapes() override;
	/** Updates shapes using a transformation matrix. */
	void UpdateShapesWithMatrix(const decDMatrix &transformation) override;
	/** Updates the shape extends. */
	void UpdateShapeExtends();
	
	/** \brief Sweep collision test updating it if dirty. */
	debpSweepCollisionTest *GetSweepCollisionTest();
	
	/** \brief Static collision test updating it if dirty or null if empty shape. */
	btCollisionObject *GetStaticCollisionTest();
	
	/**
	 * \brief Prepared static collision or null if empty shape.
	 * 
	 * Same as GetStaticCollisionTest() with calling setWorldTransform().
	 */
	btCollisionObject *GetStaticCollisionTestPrepare();
	
	/** Prepare for static collsion test. Returns true if ready or false if not usable. */
	bool PrepareStaticCollisionTest() override;
	
	/** \brief Retrieves the position. */
	inline const decDVector &GetPosition() const{return pPosition;}
	/** \brief Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{return pOrientation;}
	
	/** Create physics body if not existing already. */
	void CreateBody() override;
	/** Destroy physics body if existing. */
	void DestroyBody() override;
	/** Update collider state from physics body state. */
	void UpdateFromBody() override;
	/** Update extends if required. */
	void UpdateExtends() override;
	/** Prepare for a simulation step. */
	void PrepareForStep() override;
	/** Detect collision for a custom collision step. */
	void DetectCustomCollision(float elapsed) override;
	
	/** Prepares the collision detection. */
	void PrepareDetection(float elapsed) override;
	/** Finished the collision detection updating the collider and send notifications. */
	void FinishDetection() override;
	
	/** \brief Updates the collision object aabbs if dirty. */
	void UpdateCollisionObjectAABBs() override;
	
	bool GetRigidBodyDeactivated() const override;
	
	
	/** \name Debugging */
	/*@{*/
	/**
	 * \brief Update debug drawer shape shape.
	 * \details Called after creating debug drawer or if the collider subclass requires an update.
	 */
	void UpdateDDSShape() override;
	/*@}*/
	
	
	
	/** \name Force and Impuls */
	/*@{*/
	/** \brief Applies an impuls at the center mass point. */
	void ApplyImpuls(const decVector &impuls) override;
	/** \brief Applies an impuls relative to the collider position. */
	void ApplyImpulsAt(const decVector &impuls, const decVector &position) override;
	/** \brief Applies a torque impuls at the center mass point. */
	void ApplyTorqueImpuls(const decVector &torqueImpuls) override;
	/** \brief Applies a force at the center mass point. */
	void ApplyForce(const decVector &force) override;
	/** \brief Applies a force relative to the collider position. */
	void ApplyForceAt(const decVector &force, const decVector &position)override;
	/** \brief Applies a torque force at the center mass point. */
	void ApplyTorque(const decVector &torque) override;
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/** \brief Position changed. */
	void PositionChanged() override;
	
	/** \brief Orientation changed. */
	void OrientationChanged() override;
	
	/** \brief Position or orientation changed. */
	void GeometryChanged() override;
	
	/** \brief Scale changed. */
	void ScaleChanged() override;
	
	/** \brief Linear velocity changed. */
	void LinearVelocityChanged() override;
	
	/** \brief Angular velocity changed. */
	void AngularVelocityChanged() override;
	
	/** \brief Enabled changed. */
	void EnabledChanged() override;
	
	/** \brief Gravity changed. */
	void GravityChanged() override;
	
	/** \brief Properties like mass changed. */
	void PropertiesChanged() override;
	
	/** \brief Response type changed. */
	void ResponseTypeChanged() override;
	
	/** \brief Collision filter changed. */
	void CollisionFilterChanged() override;
	
	/** \brief Ignore colliders changed. */
	void IgnoreCollidersChanged() override;
	
	
	
	/** \brief Attachment added. */
	void AttachmentAdded(int index, deColliderAttachment *attachment) override;
	
	/** \brief Attachment changed. */
	void AttachmentChanged(int index, deColliderAttachment *attachment) override;
	
	/** \brief Attachment removed. */
	void AttachmentRemoved(int index, deColliderAttachment *attachment) override;
	
	/** \brief All attachments removed. */
	void AllAttachmentsRemoved() override;
	
	/** \brief Force update of all attachments. */
	void AttachmentsForceUpdate() override;
	
	
	
	/** \brief Collision Volume changed. */
	void CollisionVolumeChanged() override;
	
	
	
	/** \brief Constraint added. */
	void ConstraintAdded(int index, deColliderConstraint *constraint) override;
	
	/** \brief Constraint changed. */
	void ConstraintChanged(int index, deColliderConstraint *constraint) override;
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/** \brief Test if a point is located inside the collider. */
	bool PointInside(const decDVector &point) override;
	
	/**
	 * \brief Test ray for collision with the collider.
	 * 
	 * For each collision deBaseScriptingCollider::CollisionResponse() is called. To
	 * stop testing set deCollisionInfo::SetStopTesting() to true. The distance parameter
	 * in the collision response represents the actual distance to the ray origin along
	 * the ray direction.
	 * 
	 * Has to be overwritten by the physics system to provide this test. The default
	 * implementation does nothing at all.
	 */
	void RayHits(const decDVector &rayOrigin, const decVector &rayDirection, deBaseScriptingCollider *listener) override;
	
	/**
	 * \brief Test collider for collision with collider.
	 * 
	 * For each collision deBaseScriptingCollider::CollisionResponse() of the listener
	 * set in the tested collider is called. To stop testing set deCollisionInfo::SetStopTesting()
	 * to true.
	 * 
	 * Has to be overwritten by the physics system to provide this test. The default
	 * implementation does nothing at all.
	 */
	void ColliderHits(deCollider *engCollider, deBaseScriptingCollider *listener) override;
	
	/**
	 * \brief Test moving collider for collision with collider.
	 * 
	 * For each collision deBaseScriptingCollider::CollisionResponse() of the listener
	 * set in the tested collider is called. To stop testing set deCollisionInfo::SetStopTesting()
	 * to true.
	 * 
	 * Has to be overwritten by the physics system to provide this test. The default
	 * implementation does nothing at all.
	 */
	void ColliderMoveHits(deCollider *engCollider, const decVector &displacement, deBaseScriptingCollider *listener) override;
	
	/**
	 * \brief Test rotating collider for collision with collider.
	 * 
	 * For each collision deBaseScriptingCollider::CollisionResponse() of the listener
	 * set in the tested collider is called. To stop testing set deCollisionInfo::SetStopTesting()
	 * to true.
	 * 
	 * Has to be overwritten by the physics system to provide this test. The default
	 * implementation does nothing at all.
	 */
	void ColliderRotateHits(deCollider *engCollider, const decVector &rotation, deBaseScriptingCollider *listener) override;
	
	/**
	 * \brief Test moving and rotating collider for collision with collider.
	 * 
	 * For each collision deBaseScriptingCollider::CollisionResponse() of the listener
	 * set in the tested collider is called. To stop testing set deCollisionInfo::SetStopTesting()
	 * to true.
	 * 
	 * Has to be overwritten by the physics system to provide this test. The default
	 * implementation does nothing at all.
	 */
	void ColliderMoveRotateHits(deCollider *engCollider, const decVector &displacement,
		const decVector &rotation, deBaseScriptingCollider *listener) override;
	/*@}*/
	
private:
	void pCleanUp();
	
	void pRebuildShapeList();
	/** \brief Updates the sweep collision test object. */
	void pUpdateSweepCollisionTest();
	/** \brief Updates the static collision test object. */
	void pUpdateStaticCollisionTest();
	/**
	 * Determines if fake dynamics has to be used for dynamic colliders. This
	 * test checks if the bounding box of all shapes is not smaller in any of
	 * the box axes than a certain threshold. Down to this threshold Bullet
	 * can handle dynamic objects more or less well. Below the threshold
	 * tunneling is rather certain to happen. The fake dynamics prevents this
	 * by doing a kinematic simulation even if the collider itself is marked
	 * as being dynamic. This check only takes the original shape configuration
	 * into account. The collider matrix and the bone matrices have no influence.
	 */
	void pUpdateUseFakeDynamics();
	
	void pUpdateBPShape();
	debpBulletShape *pCreateBPShape();
	void pUpdateAttachments(bool force);
	void pUpdateIsMoving();
};

// end of include only once
#endif
