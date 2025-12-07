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

#ifndef _DEBPCOLLIDERRIGGED_H_
#define _DEBPCOLLIDERRIGGED_H_

#include "debpCollider.h"
#include "../shape/debpShapeList.h"

class deColliderConstraint;
class deColliderRig;
class deRigBone;

class debpColliderBones;
class debpColliderBone;
class debpPhysicsBody;
class debpRig;



/**
 * @brief Bullet Collider Rigged.
 */
class debpColliderRig : public debpCollider{
public:
	/** Test mode enumerations. */
	enum eTestModes{
		/** Test against rig shape. */
		etmRigShape,
		/** Test against bone shape. */
		etmBoneShape,
		etmNone
	};
	
private:
	deColliderRig &pColliderRig;
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	decVector pLinVelo;
	decVector pAngVelo;
	decVector pGravity;
	decVector pPredictDisp;
	decVector pPredictVelo;
	bool pHasGravity;
	bool pHasLinVelo;
	bool pHasAngVelo;
	bool pDirtyBones;
	bool pDirtyAttachments;
	bool pDirtyShapes;
	bool pPreventUpdate;
	bool pPreventAttNotify;
	eTestModes pTestMode;
	debpPhysicsBody *pSimplePhyBody;
	debpColliderBones *pBones;
	
	debpRig *pRig;
	debpShapeList pRigShapes;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rigged collider. */
	debpColliderRig(dePhysicsBullet *bullet, deColliderRig &collider);
	/** Cleans up the rigged collider. */
	~debpColliderRig() override;
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the rigged collider. */
	inline deColliderRig &GetColliderRig() const{return pColliderRig;}
	
	/** Retrieves the test mode. */
	inline eTestModes GetTestMode() const{return pTestMode;}
	
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
	
	/** Retrieves the rig or NULL if not set. */
	inline debpRig *GetRig() const{return pRig;}
	/** Retrieves the list of rig shapes. */
	inline debpShapeList &GetRigShapes(){return pRigShapes;}
	inline const debpShapeList &GetRigShapes() const{return pRigShapes;}
	
	/** Updates shapes with the current matrix. */
	void UpdateShapes() override;
	/** Updates shapes using a transformation matrix. */
	void UpdateShapesWithMatrix(const decDMatrix &transformation) override;
	
	/** Determines if the shape is simple. */
	bool IsSimpleShape() const;
	
	/** \brief Simple physics body or \em NULL. */
	inline debpPhysicsBody *GetSimplePhysicsBody() const{return pSimplePhyBody;}
	
	/** \brief Bones or NULL. */
	inline debpColliderBones *GetBones() const{return pBones;}
	
	/** Prepare for static collsion test. Returns true if ready or false if not usable. */
	bool PrepareStaticCollisionTest() override;
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
	/** Applies a force at the center mass point. */
	void ApplyForce(const decVector &force) override;
	/** Applies a force relative to the collider position. */
	void ApplyForceAt(const decVector &force, const decVector &point) override;
	/** Applies a torque force at the center mass point. */
	void ApplyTorque(const decVector &torque) override;
	
	/** Applies a force at the center mass point of the given bone. */
	void ApplyBoneForce(int bone, const decVector &force) override;
	/** Applies a force relative to the bone position. */
	void ApplyBoneForceAt(int bone, const decVector &force, const decVector &point) override;
	/** Applies a torque force at the center mass point of the given bone. */
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
	
	
	
	/** \brief Constraint added. */
	void ConstraintAdded(int index, deColliderConstraint *constraint) override;
	
	/** \brief Constraint changed. */
	void ConstraintChanged(int index, deColliderConstraint *constraint) override;
	/*@}*/
	
private:
	void pUpdateShapeExtends();
	void pCleanUp();
	void pUpdateBones();
	void pUpdateAttachments(bool force);
	void pUpdateIsMoving();
};

#endif
