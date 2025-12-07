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

#ifndef _DEBPCOLLIDERCOMPONENT_H_
#define _DEBPCOLLIDERCOMPONENT_H_

#include "debpCollider.h"
#include "../shape/debpShapeList.h"
#include "../debpCollisionObject.h"
#include "../forcefield/debpForceFieldFluctuation.h"
#include "debpBulletShape.h"

class btGhostObject;
class btCollisionObject;
class debpColliderBone;
class debpColliderBones;
class debpComponent;
class debpPhysicsBody;
class debpSweepCollisionTest;
class deColliderComponent;
class deColliderConstraint;
class deRigBone;



/**
 * @brief Bullet Collider Component.
 * Provides collision handling using a Component as collision shape.
 */
class debpColliderComponent : public debpCollider{
public:
	/** Test mode enumerations. */
	enum eTestModes{
		/** Test against rig shape. */
		etmRigShape,
		/** Test against bone shape. */
		etmBoneShape,
		/** Test against static model using the model octree. */
		etmModelStatic,
		/** Test against dynamic model. */
		etmModelDynamic,
		/** No test. Used if the component is missing. */
		etmNone
	};
	
private:
	deColliderComponent &pColliderComponent;
	debpComponent *pLinkedComponent;
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	decVector pLinVelo;
	decVector pAngVelo;
	decVector pGravity;
	decVector pPredictDisp;
	decVector pPredictVelo;
	decVector pPredictRotation;
	bool pHasGravity;
	bool pHasLinVelo;
	bool pHasAngVelo;
	bool pDirtyBones;
	bool pDirtyAttachments;
	bool pDirtyShapes;
	bool pDirtySweepTest;
	bool pDirtyStaticTest;
	bool pPreventUpdate;
	bool pPreventAttNotify;
	bool pResetKinematicInterpolation;
	debpColliderBones *pBones;
	eTestModes pTestMode;
	debpPhysicsBody *pSimplePhyBody;
	
	debpShapeList pRigShapes;
	decDVector pRigOffset;
	bool pHasRigOffset;
	
	decVector pRelMoveDisplacement;
	decQuaternion pRelMoveRotation;
	decDMatrix pRelMoveMatrix;
	decQuaternion pRelMoveMatrixRot;
	bool pDirtyRelMoveMatrix;
	
	debpSweepCollisionTest *pSweepCollisionTest;
	
	btGhostObject *pStaticCollisionTest;
	debpBulletShape::Ref pStaticCollisionTestShape;
	debpCollisionObject pStaticCollisionTestObject;
	
	debpForceFieldFluctuation pForceFieldFluctuation;
	
	bool pDirtyDebugChecks;
	
public:
	// constructor, destructor
	debpColliderComponent(dePhysicsBullet *bullet, deColliderComponent &collider);
	~debpColliderComponent();
	
	// physics management
	/** Retrieves the collider component. */
	inline deColliderComponent &GetColliderComponent() const{ return pColliderComponent; }
	/** Retrieves the linked component. */
	inline debpComponent *GetLinkedComponent() const{ return pLinkedComponent; }
	/** Sets the linked component or NULL to clear the link. */
	void SetLinkedComponent(debpComponent *component);
	
	// information
	/** Retrieves the predicted displacement. */
	inline const decVector &GetPredictedDisplacement() const{ return pPredictDisp; }
	/** Retrieves the predicted rotation. */
	inline const decVector &GetPredictedRotation() const{ return pPredictRotation; }
	/** Retrieves the test mode. */
	inline eTestModes GetTestMode() const{ return pTestMode; }
	
	/** \brief Mark bones dirty. */
	void DirtyBones();
	
	/** \brief Mark attachments dirty. */
	void DirtyAttachments();
	
	/** \brief Parent world changed. */
	void SetParentWorld(debpWorld *parentWorld) override;
	
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
	
	bool GetRigidBodyDeactivated() const override;
	
	/** \brief Apply gravity to linear velocity for kinematic simulation. */
	void ApplyGravity(float elapsed);
	
	/** \brief Apply force fields to linear and angular velocity for dynamic somulation. */
	void ApplyForceFields(float elapsed);
	
	/** Predict the displacement. */
	void PredictDisplacement(float elapsed);
	/** Interpolate position using the predicted displacement. */
	void InterpolatePosition(float percentage);
	/** Applies the predicted displacement to the position. */
	void ApplyDisplacement();
	/** Predict rotation. */
	void PredictRotation(float elapsed);
	/** Interpolate rotation using predicted rotation. */
	void InterpolateRotation(float percentage);
	/** Applies the predicted rotation to the orientation. */
	void ApplyRotation();
	
	/** Prepares the collision detection. */
	void PrepareDetection(float elapsed) override;
	/** Finished the collision detection updating the collider and send notifications. */
	void FinishDetection() override;
	
	/** \brief Calculate auto collision detection re-register value. */
	bool CalcAutoColDetPrepare() override;
	
	/** \brief Calculate auto collision detection re-register value. */
	bool CalcAutoColDetFinish() override;
	
	/**
	 * \brief Prepare constraints for next detection step.
	 * \details Required to deal with advanced features like joint frictions.
	 */
	void PrepareRigConstraintsForStep();
	
	/** \brief Check if rig constraints broke and notify the scripting module if required. */
	void CheckRigConstraintsBroke();
	
	/** \brief Updates the collision object aabbs if dirty. */
	void UpdateCollisionObjectAABBs() override;
	
	/** Retrieves the list of rig shapes. */
	inline debpShapeList &GetRigShapes(){ return pRigShapes; }
	inline const debpShapeList &GetRigShapes() const{ return pRigShapes; }
	
	/** Updates the octree position. */
	void UpdateOctreePosition() override;
	
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
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** \brief Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	
	
	/** \brief Force field fluctuation. */
	inline const debpForceFieldFluctuation &GetForceFieldFluctuation() const{ return pForceFieldFluctuation; }
	
	
	
	/** \brief Simple physics body or \em NULL. */
	inline debpPhysicsBody *GetSimplePhysicsBody() const{ return pSimplePhyBody; }
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Update debug drawer if developer mode is enabled. */
	void UpdateDebugDrawer() override;
	
	/**
	 * \brief Update debug drawer shape shape.
	 * \details Called after creating debug drawer or if the collider subclass requires an update.
	 */
	void UpdateDDSShape() override;
	/*@}*/
	
	
	
	/** \name Actions */
	/*@{*/
	/** \brief Enable or disable a component or rigged collider bone constraint. */
	void EnableBoneConstraint(int bone, int constraint, bool enable) override;
	/**
	 * \brief Replace a component or rigged collider bone constraint.
	 * \details The provided rig constraint only serves as source to copy the
	 *          new parameters. It has to be freed by the called afterwards.
	 */
	void ReplaceBoneConstraint(int bone, int constraint, const deRigConstraint &replacement) override;
	/*@}*/
	
	/** @name Forces */
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
	void ApplyForceAt(const decVector &force, const decVector &position) override;
	/** \brief Applies a torque force at the center mass point. */
	void ApplyTorque(const decVector &torque) override;
	
	/** \brief Applies an impuls at the center mass point of the given bone. */
	void ApplyBoneImpuls(int bone, const decVector &impuls) override;
	/** \brief Applies an impuls relative to the bone position. */
	void ApplyBoneImpulsAt(int bone, const decVector &impuls, const decVector &position) override;
	/** \brief Applies a torque impuls force at the center mass point of the given bone. */
	void ApplyBoneTorqueImpuls(int bone, const decVector &torqueImpuls) override;
	/** \brief Applies a force at the center mass point of the given bone. */
	void ApplyBoneForce(int bone, const decVector &force) override;
	/** \brief Applies a force relative to the bone position. */
	void ApplyBoneForceAt(int bone, const decVector &force, const decVector &position) override;
	/** \brief Applies a torque force at the center mass point of the given bone. */
	void ApplyBoneTorque(int bone, const decVector &torque) override;
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
	
	/** \brief Rig changed. */
	void RigChanged() override;
	
	
	
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
	
	/** \brief Set up weight based attachment parameters. */
	void InitWeightAttachment(deColliderAttachment &attachment, int face) override;
	
	
	
	/** \brief Constraint added. */
	void ConstraintAdded(int index, deColliderConstraint *attachment) override;
	
	/** \brief Constraint changed. */
	void ConstraintChanged(int index, deColliderConstraint *attachment) override;
	
	
	
	/** \brief Component changed. */
	void ComponentChanged() override;
	
	
	
	/** \brief Bone position changed. */
	void BonePositionChanged(int index) override;
	
	/** \brief Bone orientation changed. */
	void BoneOrientationChanged(int index) override;
	
	/** \brief Bone linear velocity changed. */
	void BoneLinearVelocityChanged(int index) override;
	
	/** \brief Bone angular velocity changed. */
	void BoneAngularVelocityChanged(int index) override;
	
	/** \brief Bone properties changed. */
	void BonePropertiesChanged(int index) override;
	
	/** \brief Bone dynamic changed. */
	void BoneDynamicChanged(int index) override;
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
	
	
	
	// internal notifications
	/** \brief Attached component rig changed. */
	void ComponentRigChanged();
	
	/** \brief Attached component shape changed. */
	void ComponentShapeChanged();
	
	/** \brief Attached component mesh dirty. */
	void ComponentMeshDirty();
	
	/** \brief Bones or NULL. */
	inline debpColliderBones *GetBones() const{ return pBones; }
	
	bool IsSimpleShape() const;
	
private:
	void pCleanUp();
	void pUpdateBones();
	void pUpdateAttachments(bool force);
	void pApplyAccumRelMoveMatrices();
	void pUpdateIsMoving();
	
	/** \brief Updates the sweep collision test object. */
	void pUpdateSweepCollisionTest();
	/** \brief Updates the static collision test object. */
	void pUpdateStaticCollisionTest();
	
	/** \brief Check debug checks for performance problems. */
	void pCheckDebugChecks();
	
	debpBulletShape *pCreateBPShape();
	
	bool pPrepreStaticCollisionTestPos();
};

#endif
