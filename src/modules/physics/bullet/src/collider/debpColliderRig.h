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
	int pTestMode;
	debpPhysicsBody *pSimplePhyBody;
	debpColliderBones *pBones;
	
	debpRig *pRig;
	debpShapeList pRigShapes;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rigged collider. */
	debpColliderRig( dePhysicsBullet *bullet, deColliderRig &collider );
	/** Cleans up the rigged collider. */
	virtual ~debpColliderRig();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the rigged collider. */
	inline deColliderRig &GetColliderRig() const{ return pColliderRig; }
	
	/** Retrieves the test mode. */
	inline int GetTestMode() const{ return pTestMode; }
	
	/** \brief Mark bones dirty. */
	void DirtyBones();
	
	/** \brief Mark attachments dirty. */
	void DirtyAttachments();
	
	/** \brief Parent world changed. */
	virtual void SetParentWorld( debpWorld *parentWorld );
	
	/** Create physics body if not existing already. */
	virtual void CreateBody();
	/** Destroy physics body if existing. */
	virtual void DestroyBody();
	/** Update collider state from physics body state. */
	virtual void UpdateFromBody();
	/** Update extends if required. */
	virtual void UpdateExtends();
	/** Prepare for a simulation step. */
	virtual void PrepareForStep();
	
	/** Prepares the collision detection. */
	virtual void PrepareDetection( float elapsed );
	/** Finished the collision detection updating the collider and send notifications. */
	virtual void FinishDetection();
	
	/** \brief Calculate auto collision detection re-register value. */
	virtual bool CalcAutoColDetPrepare();
	
	/** \brief Calculate auto collision detection re-register value. */
	virtual bool CalcAutoColDetFinish();
	
	/**
	 * \brief Prepare constraints for next detection step.
	 * \details Required to deal with advanced features like joint frictions.
	 */
	void PrepareRigConstraintsForStep();
	
	/** \brief Check if rig constraints broke and notify the scripting module if required. */
	void CheckRigConstraintsBroke();
	
	/** \brief Updates the collision object aabbs if dirty. */
	virtual void UpdateCollisionObjectAABBs();
	
	/** Retrieves the rig or NULL if not set. */
	inline debpRig *GetRig() const{ return pRig; }
	/** Retrieves the list of rig shapes. */
	inline debpShapeList &GetRigShapes(){ return pRigShapes; }
	inline const debpShapeList &GetRigShapes() const{ return pRigShapes; }
	
	/** Updates shapes with the current matrix. */
	virtual void UpdateShapes();
	/** Updates shapes using a transformation matrix. */
	virtual void UpdateShapesWithMatrix( const decDMatrix &transformation );
	
	/** Determines if the shape is simple. */
	bool IsSimpleShape() const;
	
	/** \brief Simple physics body or \em NULL. */
	inline debpPhysicsBody *GetSimplePhysicsBody() const{ return pSimplePhyBody; }
	
	/** \brief Bones or NULL. */
	inline debpColliderBones *GetBones() const{ return pBones; }
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Update debug drawer if developer mode is enabled. */
	virtual void UpdateDebugDrawer();
	
	/**
	 * \brief Update debug drawer shape shape.
	 * \details Called after creating debug drawer or if the collider subclass requires an update.
	 */
	virtual void UpdateDDSShape();
	/*@}*/
	
	
	
	/** \name Actions */
	/*@{*/
	/** \brief Enable or disable a component or rigged collider bone constraint. */
	virtual void EnableBoneConstraint( int bone, int constraint, bool enable );
	/**
	 * \brief Replace a component or rigged collider bone constraint.
	 * \details The provided rig constraint only serves as source to copy the
	 *          new parameters. It has to be freed by the called afterwards.
	 */
	virtual void ReplaceBoneConstraint( int bone, int constraint, const deRigConstraint &replacement );
	/*@}*/
	
	/** @name Forces */
	/*@{*/
	/** Applies a force at the center mass point. */
	virtual void ApplyForce( const decVector &force );
	/** Applies a force relative to the collider position. */
	virtual void ApplyForceAt( const decVector &force, const decVector &point );
	/** Applies a torque force at the center mass point. */
	virtual void ApplyTorque( const decVector &torque );
	
	/** Applies a force at the center mass point of the given bone. */
	virtual void ApplyBoneForce( int bone, const decVector &force );
	/** Applies a force relative to the bone position. */
	virtual void ApplyBoneForceAt( int bone, const decVector &force, const decVector &point );
	/** Applies a torque force at the center mass point of the given bone. */
	virtual void ApplyBoneTorque( int bone, const decVector &torque );
	/*@}*/
	
	
	
	/** @name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Position or orientation changed. */
	virtual void GeometryChanged();
	
	/** \brief Scale changed. */
	virtual void ScaleChanged();
	
	/** \brief Linear velocity changed. */
	virtual void LinearVelocityChanged();
	
	/** \brief Angular velocity changed. */
	virtual void AngularVelocityChanged();
	
	/** \brief Enabled changed. */
	virtual void EnabledChanged();
	
	/** \brief Gravity changed. */
	virtual void GravityChanged();
	
	/** \brief Properties like mass changed. */
	virtual void PropertiesChanged();
	
	/** \brief Response type changed. */
	virtual void ResponseTypeChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Ignore colliders changed. */
	virtual void IgnoreCollidersChanged();
	
	/** \brief Rig changed. */
	virtual void RigChanged();
	
	
	
	/** \brief Attachment added. */
	virtual void AttachmentAdded( int index, deColliderAttachment *attachment );
	
	/** \brief Attachment changed. */
	virtual void AttachmentChanged( int index, deColliderAttachment *attachment );
	
	/** \brief Attachment removed. */
	virtual void AttachmentRemoved( int index, deColliderAttachment *attachment );
	
	/** \brief All attachments removed. */
	virtual void AllAttachmentsRemoved();
	
	/** \brief Force update of all attachments. */
	virtual void AttachmentsForceUpdate();
	
	
	
	/** \brief Bone position changed. */
	virtual void BonePositionChanged( int index );
	
	/** \brief Bone orientation changed. */
	virtual void BoneOrientationChanged( int index );
	
	/** \brief Bone linear velocity changed. */
	virtual void BoneLinearVelocityChanged( int index );
	
	/** \brief Bone angular velocity changed. */
	virtual void BoneAngularVelocityChanged( int index );
	
	/** \brief Bone properties changed. */
	virtual void BonePropertiesChanged( int index );
	
	/** \brief Bone dynamic changed. */
	virtual void BoneDynamicChanged( int index );
	
	
	
	/** \brief Constraint added. */
	virtual void ConstraintAdded( int index, deColliderConstraint *constraint );
	
	/** \brief Constraint changed. */
	virtual void ConstraintChanged( int index, deColliderConstraint *constraint );
	/*@}*/
	
private:
	void pUpdateShapeExtends();
	void pCleanUp();
	void pUpdateBones();
	void pUpdateAttachments( bool force );
	void pUpdateIsMoving();
};

#endif
