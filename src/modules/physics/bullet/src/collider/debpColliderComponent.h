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

class btGhostObject;
class debpBulletShape;
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
	debpBulletShape *pStaticCollisionTestShape;
	debpCollisionObject pStaticCollisionTestObject;
	
	debpForceFieldFluctuation pForceFieldFluctuation;
	
	bool pDirtyDebugChecks;
	
public:
	// constructor, destructor
	debpColliderComponent( dePhysicsBullet *bullet, deColliderComponent &collider );
	~debpColliderComponent();
	
	// physics management
	/** Retrieves the collider component. */
	inline deColliderComponent &GetColliderComponent() const{ return pColliderComponent; }
	/** Retrieves the linked component. */
	inline debpComponent *GetLinkedComponent() const{ return pLinkedComponent; }
	/** Sets the linked component or NULL to clear the link. */
	void SetLinkedComponent( debpComponent *component );
	
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
	
	/** Detect collision for a custom collision step. */
	virtual void DetectCustomCollision( float elapsed );
	
	virtual bool GetRigidBodyDeactivated() const;
	
	/** \brief Apply gravity to linear velocity for kinematic simulation. */
	void ApplyGravity( float elapsed );
	
	/** \brief Apply force fields to linear and angular velocity for dynamic somulation. */
	void ApplyForceFields( float elapsed );
	
	/** Predict the displacement. */
	void PredictDisplacement( float elapsed );
	/** Interpolate position using the predicted displacement. */
	void InterpolatePosition( float percentage );
	/** Applies the predicted displacement to the position. */
	void ApplyDisplacement();
	/** Predict rotation. */
	void PredictRotation( float elapsed );
	/** Interpolate rotation using predicted rotation. */
	void InterpolateRotation( float percentage );
	/** Applies the predicted rotation to the orientation. */
	void ApplyRotation();
	
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
	
	/** Retrieves the list of rig shapes. */
	inline debpShapeList &GetRigShapes(){ return pRigShapes; }
	inline const debpShapeList &GetRigShapes() const{ return pRigShapes; }
	
	/** Updates the octree position. */
	virtual void UpdateOctreePosition();
	
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
	
	
	
	/** \brief Force field fluctuation. */
	inline const debpForceFieldFluctuation &GetForceFieldFluctuation() const{ return pForceFieldFluctuation; }
	
	
	
	/** \brief Simple physics body or \em NULL. */
	inline debpPhysicsBody *GetSimplePhysicsBody() const{ return pSimplePhyBody; }
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
	
	/** \brief Applies an impuls at the center mass point of the given bone. */
	virtual void ApplyBoneImpuls( int bone, const decVector &impuls );
	/** \brief Applies an impuls relative to the bone position. */
	virtual void ApplyBoneImpulsAt( int bone, const decVector &impuls, const decVector &position );
	/** \brief Applies a torque impuls force at the center mass point of the given bone. */
	virtual void ApplyBoneTorqueImpuls( int bone, const decVector &torqueImpuls );
	/** \brief Applies a force at the center mass point of the given bone. */
	virtual void ApplyBoneForce( int bone, const decVector &force );
	/** \brief Applies a force relative to the bone position. */
	virtual void ApplyBoneForceAt( int bone, const decVector &force, const decVector &position );
	/** \brief Applies a torque force at the center mass point of the given bone. */
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
	
	/** \brief Set up weight based attachment parameters. */
	virtual void InitWeightAttachment( deColliderAttachment &attachment, int face );
	
	
	
	/** \brief Constraint added. */
	virtual void ConstraintAdded( int index, deColliderConstraint *attachment );
	
	/** \brief Constraint changed. */
	virtual void ConstraintChanged( int index, deColliderConstraint *attachment );
	
	
	
	/** \brief Component changed. */
	virtual void ComponentChanged();
	
	
	
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
	void pUpdateAttachments( bool force );
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
