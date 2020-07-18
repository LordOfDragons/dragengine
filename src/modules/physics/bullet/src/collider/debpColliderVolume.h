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
	debpColliderVolume( dePhysicsBullet *bullet, deColliderVolume &collider );
	~debpColliderVolume();
	// information
	inline const decVector &GetPredictedDisplacement() const{ return pPredictDisp; }
	inline const decVector &GetPredictedRotation() const{ return pPredictRot; }
	// physics management
	inline deColliderVolume &GetColliderVolume() const{ return pColliderVolume; }
	
	/** Retrieves the list of shapes. */
	inline debpShapeList &GetShapes(){ return pShapes; }
	inline const debpShapeList &GetShapes() const{ return pShapes; }
	
	/** Retrieves the physics body. */
	inline debpPhysicsBody *GetPhysicsBody() const{ return pPhyBody; }
	/** Determines if the collider has to be simulated using fake dynamics. */
	inline bool GetUseFakeDynamics() const{ return pUseFakeDynamics; }
	
	/** Retrieves the gravity. */
	inline const decVector &GetGravity() const{ return pGravity; }
	/** Retrieves the linear velocity. */
	inline const decVector &GetLinearVelocity() const{ return pLinVelo; }
	
	/** \brief Mark shapes dirty. */
	void DirtyBPShape();
	
	/** \brief Mark attachments dirty. */
	void DirtyAttachments();
	
	/** Applies the gravity to the linear velocity for kinematic simulation. */
	void ApplyGravity( float elapsed );
	/** Predict the displacement and rotation. */
	void PredictDisplacement( float elapsed );
	/** Interpolate position using the predicted displacement and rotation. */
	void InterpolatePosition( float percentage );
	/** Applies the predicted displacement and rotation to the position. */
	void ApplyDisplacement();
	/** Apply fake dynamics response. */
	void ApplyFakeDynamicResponse( deCollisionInfo &colinfo );
	
	/** Updates shapes with the current matirx. */
	virtual void UpdateShapes();
	/** Updates shapes using a transformation matrix. */
	virtual void UpdateShapesWithMatrix( const decDMatrix &transformation );
	/** Updates the shape extends. */
	void UpdateShapeExtends();
	
	/** \brief Retrieves the sweep collision test updating it if dirty. */
	debpSweepCollisionTest *GetSweepCollisionTest();
	/** \brief Retrieves the static collision test updating it if dirty. */
	btGhostObject *GetStaticCollisionTest();
	
	/** \brief Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** \brief Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
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
	/** Detect collision for a custom collision step. */
	virtual void DetectCustomCollision( float elapsed );
	
	/** Prepares the collision detection. */
	virtual void PrepareDetection( float elapsed );
	/** Finished the collision detection updating the collider and send notifications. */
	virtual void FinishDetection();
	
	/** \brief Updates the collision object aabbs if dirty. */
	virtual void UpdateCollisionObjectAABBs();
	
	
	
	/** \name Debugging */
	/*@{*/
	/**
	 * \brief Update debug drawer shape shape.
	 * \details Called after creating debug drawer or if the collider subclass requires an update.
	 */
	virtual void UpdateDDSShape();
	/*@}*/
	
	
	
	/** \name Force and Impuls */
	/*@{*/
	/** \brief Applies an impuls at the center mass point. */
	virtual void ApplyImpuls( const decVector &impuls );
	/** \brief Applies an impuls relative to the collider position. */
	virtual void ApplyImpulsAt( const decVector &impuls, const decVector &position );
	/** \brief Applies a torque impuls at the center mass point. */
	virtual void ApplyTorqueImpuls( const decVector &torqueImpuls );
	/** \brief Applies a force at the center mass point. */
	virtual void ApplyForce( const decVector &force );
	/** \brief Applies a force relative to the collider position. */
	virtual void ApplyForceAt( const decVector &force, const decVector &position );
	/** \brief Applies a torque force at the center mass point. */
	virtual void ApplyTorque( const decVector &torque );
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
	
	
	
	/** \brief Collision Volume changed. */
	virtual void CollisionVolumeChanged();
	
	
	
	/** \brief Constraint added. */
	virtual void ConstraintAdded( int index, deColliderConstraint *constraint );
	
	/** \brief Constraint changed. */
	virtual void ConstraintChanged( int index, deColliderConstraint *constraint );
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
	virtual void RayHits( const decDVector &rayOrigin, const decVector &rayDirection, deBaseScriptingCollider *listener );
	
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
	virtual void ColliderHits( deCollider *engCollider, deBaseScriptingCollider *listener );
	
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
	virtual void ColliderMoveHits( deCollider *engCollider, const decVector &displacement, deBaseScriptingCollider *listener );
	
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
	virtual void ColliderRotateHits( deCollider *engCollider, const decVector &rotation, deBaseScriptingCollider *listener );
	
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
	virtual void ColliderMoveRotateHits( deCollider *engCollider, const decVector &displacement,
		const decVector &rotation, deBaseScriptingCollider *listener );
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
	void pUpdateAttachments( bool force );
	void pUpdateIsMoving();
};

// end of include only once
#endif
