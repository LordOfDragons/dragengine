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

#ifndef _DEBASEPHASICSCOLLIDER_H_
#define _DEBASEPHASICSCOLLIDER_H_

#include "../../../common/math/decMath.h"

class deBaseScriptingCollider;
class deCollider;
class deColliderAttachment;
class deColliderConstraint;
class deRigConstraint;


/**
 * \brief Physics module collider peer.
 */
class DE_DLL_EXPORT deBasePhysicsCollider{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBasePhysicsCollider();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsCollider();
	/*@}*/
	
	
	
	/** \name Notifications */
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
	
	/** \brief Force field factor changed. */
	virtual void ForceFieldChanged();
	
	
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Ignore colliders changed. */
	virtual void IgnoreCollidersChanged();
	
	
	
	/** \brief Post physics collision test added. */
	virtual void CollisionTestAdded( int index );
	
	/** \brief Post physics collision test changed. */
	virtual void CollisionTestChanged( int index );
	
	/** \brief Post physics collision test enabled changed. */
	virtual void CollisionTestEnabledChanged( int index );
	
	/** \brief Post physics collision test removed. */
	virtual void CollisionTestRemoved( int index );
	
	/** \brief All post physics collision tests removed. */
	virtual void AllCollisionTestsRemoved();
	
	
	
	/** \brief Collision Volume changed. */
	virtual void CollisionVolumeChanged();
	
	/** \brief Rig changed. */
	virtual void RigChanged();

	/** \brief Component changed. */
	virtual void ComponentChanged();
	/*@}*/
	
	
	
	/** \name Bones */
	/*@{*/
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
	/*@}*/
	
	
	
	/** \name Force and Impuls */
	/*@{*/
	/** \brief Apply impuls at the center mass point. */
	virtual void ApplyImpuls( const decVector &impuls );
	
	/** \brief Apply impuls relative to the collider position. */
	virtual void ApplyImpulsAt( const decVector &impuls, const decVector &position );
	
	/** \brief Apply torque impuls at the center mass point. */
	virtual void ApplyTorqueImpuls( const decVector &torqueImpuls );
	
	/** \brief Apply force at the center mass point. */
	virtual void ApplyForce( const decVector &force );
	
	/** \brief Apply force relative to the collider position. */
	virtual void ApplyForceAt( const decVector &force, const decVector &position );
	
	/** \brief Apply torque force at the center mass point. */
	virtual void ApplyTorque( const decVector &torque );
	
	
	
	/** \brief Apply impuls at the center mass point of the given bone. */
	virtual void ApplyBoneImpuls( int bone, const decVector &impuls );
	
	/** \brief Apply impuls relative to the bone position. */
	virtual void ApplyBoneImpulsAt( int bone, const decVector &impuls, const decVector &position );
	
	/** \brief Apply torque impuls force at the center mass point of the given bone. */
	virtual void ApplyBoneTorqueImpuls( int bone, const decVector &torqueImpuls );
	
	/** \brief Apply force at the center mass point of the given bone. */
	virtual void ApplyBoneForce( int bone, const decVector &force );
	
	/** \brief Apply force relative to the bone position. */
	virtual void ApplyBoneForceAt( int bone, const decVector &force, const decVector &position );
	
	/** \brief Apply torque force at the center mass point of the given bone. */
	virtual void ApplyBoneTorque( int bone, const decVector &torque );
	/*@}*/
	
	
	
	/** \name Attachments */
	/*@{*/
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
	
	/**
	 * \brief Set up weight based attachment parameters.
	 * 
	 * Default implementation throws EInvalidParams.
	 */
	virtual void InitWeightAttachment( deColliderAttachment &attachment, int face );
	/*@}*/
	
	
	
	
	/** \name Constraints */
	/*@{*/
	/** \brief Constraint added. */
	virtual void ConstraintAdded( int index, deColliderConstraint *constraint );
	
	/** \brief Constraint changed. */
	virtual void ConstraintChanged( int index, deColliderConstraint *constraint );
	
	/** \brief Constraint removed. */
	virtual void ConstraintRemoved( int index, deColliderConstraint *constraint );
	
	/** \brief All constraints removed. */
	virtual void AllConstraintsRemoved();
	
	
	
	/** \brief Enable or disable a component or rigged collider bone constraint. */
	virtual void EnableBoneConstraint( int bone, int constraint, bool enable );
	
	/**
	 * \brief Replace a component or rigged collider bone constraint.
	 * 
	 * The provided rig constraint only serves as source to copy the
	 * new parameters. It has to be freed by the called afterwards.
	 */
	virtual void ReplaceBoneConstraint( int bone, int constraint, const deRigConstraint &replacement );
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/** \brief Test if a point is located inside the collider. */
	virtual bool PointInside( const decDVector &point );
	
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
	virtual void RayHits( const decDVector &rayOrigin, const decVector &rayDirection,
		deBaseScriptingCollider *listener );
	
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
	virtual void ColliderHits( deCollider *collider, deBaseScriptingCollider *listener );
	
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
	virtual void ColliderMoveHits( deCollider *collider, const decVector &displacement,
		deBaseScriptingCollider *listener );
	
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
	virtual void ColliderRotateHits( deCollider *collider, const decVector &rotation,
		deBaseScriptingCollider *listener );
	
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
	virtual void ColliderMoveRotateHits( deCollider *collider, const decVector &displacement,
		const decVector &rotation, deBaseScriptingCollider *listener );
	/*@}*/
};

#endif
