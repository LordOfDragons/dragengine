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

#ifndef _DEBPCOLLIDER_H_
#define _DEBPCOLLIDER_H_

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/physics/deBasePhysicsCollider.h>

class debpColliderAttachment;
class debpColliderCollisionTest;
class debpColliderComponent;
class debpColliderConstraint;
class debpColliderRig;
class debpColliderVolume;
class debpCollisionWorld;
class debpWorld;

class deCollider;
class deColliderCollisionTest;
class deDebugDrawer;
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
	
	decPointerList pCollisionTests;
	
	decPointerSet pAttachedToList;
	decPointerSet pTrackingTouchSensors;
	
	int pIndex;
	bool pIsMoving;
	bool pDirtyMatrix;
	bool pDirtyOctree;
	bool pRequiresUpdate;
	bool pUseKinematicSim;
	bool pIsPrepared;
	
	bool pMarked;
	
	int pColDetPrepareIndex;
	bool pAutoColDetPrepare;
	int pColDetFinishIndex;
	bool pAutoColDetFinish;
	int pPPCTColliderIndex;
	int pUpdateOctreeIndex;
	
	deDebugDrawer *pDebugDrawer;
	deDebugDrawerShape *pDDSShape;
	
	
	
public:
	// constructor, destructor
	debpCollider( dePhysicsBullet *bullet, deCollider &collider, int type );
	~debpCollider();
	
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
	
	/** \brief Requires update. */
	inline bool GetRequiresUpdate() const{ return pRequiresUpdate; }
	
	/** \brief Requires update. */
	void RequiresUpdate();
	
	/** \brief Clear requires update. */
	void ClearRequiresUpdate();
	
	inline bool GetIsMoving() const{ return pIsMoving; }
	void SetIsMoving( bool isMoving );
	const decDMatrix &GetMatrix();
	const decDMatrix &GetInverseMatrix();
	const decDMatrix &GetMatrixNormal();
	void SetIndex( int index );
	virtual void SetParentWorld( debpWorld *parentWorld );
	
	/** Parent world changed. */
	void ParentWorldChanged();
	
	void SetAddToOctree( bool doAdd );
	void SetExtends( const decDVector &minExtend, const decDVector &maxExtend );
	void MarkMatrixDirty();
	void MarkDirtyOctree();
	
	/** \brief Two collider can collide. */
	bool Collides( const debpCollider &collider ) const;
	
	/** \brief Two collider can not collide. */
	bool CollidesNot( const debpCollider &collider ) const;
	
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
	virtual void DetectCustomCollision( float elapsed );
	
	/** Prepares the collision detection. */
	virtual void PrepareDetection( float elapsed );
	/** Finished the collision detection updating the collider and send notifications. */
	virtual void FinishDetection();
	
	/** \brief Updates the collision object aabbs if dirty. */
	virtual void UpdateCollisionObjectAABBs();
	
	/** Retrieves the dynamics world or NULL. */
	debpCollisionWorld *GetDynamicsWorld() const;
	
	/** Retrieves the marked flag. */
	inline bool GetMarked() const{ return pMarked; }
	/** Sets the marked flag. */
	inline void SetMarked( bool marked ){ pMarked = marked; }
	
	/** Determines if this collider has to be simulated using kinematics. */
	inline bool GetUseKinematicSimulation() const{ return pUseKinematicSim; }
	/** Sets if this collider has to be simulated using kinematics. */
	void SetUseKinematicSimulation( bool useKinematicSimulation );
	
	/** Updates the octree position. */
	virtual void UpdateOctreePosition();
	
	/** Updates shapes with the current matirx. */
	virtual void UpdateShapes();
	/** Updates shapes using a transformation matrix. */
	virtual void UpdateShapesWithMatrix( const decDMatrix &transformation );
	/** Retrieves the minimum extend of the shapes. */
	inline const decDVector &GetShapeMinimumExtend() const{ return pShapeMinExtend; }
	/** Retrieves the maximum extend of the shapes. */
	inline const decDVector &GetShapeMaximumExtend() const{ return pShapeMaxExtend; }
	/** Sets the extends of the shapes. */
	void SetShapeExtends( const decDVector &minExtend, const decDVector &maxExtend );
	
	/**
	 * \brief Prepare constraints for next detection step.
	 * \details Required to deal with advanced features like joint frictions.
	 */
	void PrepareConstraintsForStep();
	
	/** \brief Check if collider constraints broke and notify the scripting module if required. */
	void CheckColliderConstraintsBroke();
	
	/** \brief Process collider collision tests. */
	void ProcessColliderCollisionTests();
	
	
	
	/** \brief Register for prepare collision detection if not registered yet. */
	void RegisterColDetPrepare();
	
	/** \brief Unregister for prepare collision detection if registered. */
	void UnregisterColDetPrepare();
	
	/** \brief Prepare collision detection index or -1 if not registered. */
	inline int GetColDetPrepareIndex() const{ return pColDetPrepareIndex; }
	
	/** \brief Set prepare collision detection index or -1 if not registered. */
	void SetColDetPrepareIndex( int index );
	
	/** \brief Automatically re-registered for collision detection prepare. */
	inline bool GetAutoColDetPrepare() const{ return pAutoColDetPrepare; }
	
	/** \brief Set automatically re-registered for collision detection prepare. */
	void SetAutoColDetPrepare( bool autoColDetPrepare );
	
	/** \brief Calculate auto collision detection re-register value. */
	virtual bool CalcAutoColDetPrepare();
	
	
	
	/** \brief Register for finish collision detection if not registered yet. */
	void RegisterColDetFinish();
	
	/** \brief Unregister for finish collision detection if registered. */
	void UnregisterColDetFinish();
	
	/** \brief Finish collision detection index or -1 if not registered. */
	inline int GetColDetFinishIndex() const{ return pColDetFinishIndex; }
	
	/** \brief Set finish collision detection index or -1 if not registered. */
	void SetColDetFinishIndex( int index );
	
	/** \brief Automatically re-registered for collision detection finish. */
	inline bool GetAutoColDetFinish() const{ return pAutoColDetFinish; }
	
	/** \brief Set automatically re-registered for collision detection finish. */
	void SetAutoColDetFinish( bool autoColDetFinish );
	
	/** \brief Calculate auto collision detection re-register value. */
	virtual bool CalcAutoColDetFinish();
	
	
	
	/** \brief Register for post physics collision processing not registered yet. */
	void RegisterPPCProcessing();
	
	/** \brief Unregister for post physics collision processing if registered. */
	void UnregisterPPCProcessing();
	
	/** \brief Post physics collision processing index or -1 if not registered. */
	inline int GetPPCProcessingIndex() const{ return pPPCTColliderIndex; }
	
	/** \brief Set post physics collision processing index or -1 if not registered. */
	void SetPPCProcessingIndex( int index );
	
	
	
	/** \brief Register for update octree processing not registered yet. */
	void RegisterUpdateOctree();
	
	/** \brief Unregister for update octree processing if registered. */
	void UnregisterUpdateOctree();
	
	/** \brief Update octree processing index or -1 if not registered. */
	inline int GetUpdateOctreeIndex() const{ return pUpdateOctreeIndex; }
	
	/** \brief Set update octree processing index or -1 if not registered. */
	void SetUpdateOctreeIndex( int index );
	
	
	
	/** @name Attachments */
	/*@{*/
	/** \brief Retrieves the number of attachments. */
	inline int GetAttachmentCount() const{ return pAttachmentCount; }
	/** \brief Retrieves an attachment. */
	debpColliderAttachment *GetAttachmentAt( int index ) const;
	
	/** Retrieves the list of colliders this collider is attached to. */
	inline decPointerSet &GetAttachedToList(){ return pAttachedToList; }
	inline const decPointerSet &GetAttachedToList() const{ return pAttachedToList; }
	
	/** \brief List of touch sensors tracking this collider. */
	inline decPointerSet &GetTrackingTouchSensors(){ return pTrackingTouchSensors; }
	inline const decPointerSet &GetTrackingTouchSensors() const{ return pTrackingTouchSensors; }
	/*@}*/
	
	
	
	/** \name Constraints */
	/*@{*/
	/** Retrieves the number of constraints. */
	inline int GetConstraintCount() const{ return pConstraintCount; }
	/** Retrieves the constraint at the given index. */
	debpColliderConstraint *GetConstraintAt( int index ) const;
	/*@}*/
	
	
	
	/** \name Post physics collision tests */
	/*@{*/
	/** \brief Number of post physics collision tests. */
	int GetCollisionTestCount() const;
	
	/** \brief Post physics collision test at index. */
	debpColliderCollisionTest *GetCollisionTestAt( int index ) const;
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Debug drawer or \em NULL if not activated .*/
	inline deDebugDrawer *GetDebugDrawer() const{ return pDebugDrawer; }
	
	/** \brief Debug drawer shape or \em NULL if not ativated. */
	inline deDebugDrawerShape *GetDDSShape() const{ return pDDSShape; }
	
	/** \brief Update debug drawer if developer mode is enabled. */
	virtual void UpdateDebugDrawer();
	
	/**
	 * \brief Update debug drawer shape shape.
	 * \details Called after creating debug drawer or if the collider subclass requires an update.
	 */
	virtual void UpdateDDSShape();
	/*@}*/
	
	
	
	/** @name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Scale changed. */
	virtual void ScaleChanged();
	
	/** \brief Position or orientation changed. */
	virtual void GeometryChanged();
	
	/** Linear velocity changed. */
	virtual void LinearVelocityChanged();
	/** Angular velocity changed. */
	virtual void AngularVelocityChanged();
	/** Enabled changed. */
	virtual void EnabledChanged();
	/** Gravity changed. */
	virtual void GravityChanged();
	/** Properties like mass changed. */
	virtual void PropertiesChanged();
	/** Response type changed. */
	virtual void ResponseTypeChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Ignore colliders changed. */
	virtual void IgnoreCollidersChanged();
	
	/** \brief Force field factor changed. */
	virtual void ForceFieldChanged();
	
	/** Attachment added. */
	virtual void AttachmentAdded( int index, deColliderAttachment *attachment );
	/** Attachment changed. */
	virtual void AttachmentChanged( int index, deColliderAttachment *attachment );
	/** Attachment removed. */
	virtual void AttachmentRemoved( int index, deColliderAttachment *attachment );
	/** All attachments removed. */
	virtual void AllAttachmentsRemoved();
	
	/** Constraint added. */
	virtual void ConstraintAdded( int index, deColliderConstraint *attachment );
	/** Constraint changed. */
	virtual void ConstraintChanged( int index, deColliderConstraint *attachment );
	/** Constraint removed. */
	virtual void ConstraintRemoved( int index, deColliderConstraint *attachment );
	/** All attachments removed. */
	virtual void AllConstraintsRemoved();
	
	
	
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
	/*@}*/
	
	
	
protected:
	inline bool GetIsPrepared() const{ return pIsPrepared; }
	
private:
	void pCleanUp();
	void pRemoveFromAllTrackingTouchSensors();
};

#endif
