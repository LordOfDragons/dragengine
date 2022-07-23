/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECOLLIDER_H_
#define _DECOLLIDER_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decCollisionFilter.h"
#include "../../common/collection/decObjectSet.h"
#include "../../common/collection/decObjectOrderedSet.h"

class deBasePhysicsCollider;
class deBaseScriptingCollider;
class deColliderAttachment;
class deColliderCollisionTest;
class deColliderConstraint;
class deColliderManager;
class deColliderVisitor;
class deWorld;


/**
 * \brief World Collider Object.
 *
 * Colliders are used by the physics module to provide collision
 * detection and optional collision response. After the collision
 * detection stage is done each Collider contains the new state
 * of motion. The user can then update the game state depending
 * on the changes. The scripting peer receives all relevant events
 * during a collision detection run. A Collider can be defined as
 * a couple of different types.
 *
 * The Collider object itself can not be constructed. Create a new
 * Collider object using one of deColliderVolume, deColliderRig
 * or deColliderComponent classes.
 *
 * \par Collision Response Types
 * The Collision Response Type determines how a collision is
 * handeled once detected.
 *
 * ertNone causes no response to be send and
 * hence does not notify you about a collision. This is used for
 * static objects that only act as blockers or objects that do not
 * wish to generate collision notification because they are activated
 * by other objects.
 *
 * ertPhysics causes a physical response to be calculated by the
 * Physics Module. The Scripting Module is notified using its peer
 * about the collision. You are responsible there to provide
 * information about the surfaces getting in contact to allow the
 * Physics Module to calculate a proper physical reaction to the
 * collision.
 *
 * ertCustom is used to generate your own collision responses. Upon
 * collision the Scripting Module is notified. In the
 * CollisionResponse function you are responsible for specifying
 * the effect of the collision on the object.
 */
class DE_DLL_EXPORT deCollider : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCollider> Ref;
	
	
	
public:
	/** \brief Collision Response Types. */
	enum eResponseType{
		/** \brief Static, same as no response. */
		ertStatic,
		
		/** \brief Dynamic, same as physics response. */
		ertDynamic,
		
		/** \brief Kinematic, same as custom response. */
		ertKinematic
	};
	
	
	
private:
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	decVector pLinearVelocity;
	decVector pAngularVelocity;
	decVector pGravity;
	float pMass;
	eResponseType pResponseType;
	bool pEnabled;
	bool pUseLocalGravity;
	
	float pForceFieldDirect;
	float pForceFieldSurface;
	float pForceFieldMass;
	float pForceFieldSpeed;
	
	deColliderAttachment **pAttachments;
	int pAttachmentCount;
	int pAttachmentSize;
	
	deColliderConstraint **pConstraints;
	int pConstraintCount;
	int pConstraintSize;
	
	decObjectOrderedSet pCollisionTests;
	
	decCollisionFilter pCollisionFilter;
	decObjectSet pIgnoreColliders;
	
	deBasePhysicsCollider *pPeerPhysics;
	deBaseScriptingCollider *pPeerScripting;
	
	deWorld *pParentWorld;
	deCollider *pLLWorldPrev;
	deCollider *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create collider. */
	deCollider( deColliderManager *manager );
	
protected:
	/**
	 * \brief Clean up collider.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deCollider();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Central mass point position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set central mass point position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/**
	 * \brief Set position and orientation at the same time.
	 * 
	 * Colliders are the only resources supporting attachments. Using SetPosition() and
	 * SetOrientation() causes attachments to be repositions immediately. If many resources
	 * are attached to the collider (perhaps even nested) this can reduce performance since
	 * attachments are potentially repositioned twice. This call ensures attachments are
	 * only repositioned once improving performance.
	 */
	void SetGeometry( const decDVector &position, const decQuaternion &orientation );
	
	void SetGeometry( const decDVector &position, const decQuaternion &orientation,
		const decVector &scale );
	
	/** \brief Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/**
	 * \brief Set scale.
	 * 
	 * For deColliderComponent the scaling is used from the set deComponent
	 * in the first place. The collider scaling is applied on top of it.
	 * Hence if you use deColliderComponent with scaled components you
	 * do not have to set the scaling on the deColliderComponent. For
	 * deColliderVolume and deColliderRig using scaling is required.
	 */
	void SetScale( const decVector &scale );
	
	/** \brief Linear velocity in m/s. */
	inline const decVector &GetLinearVelocity() const{ return pLinearVelocity; }
	
	/** \brief Set linear velocity in m/s. */
	void SetLinearVelocity( const decVector &linVelo );
	
	/** \brief Angular velocity in degree/s. */
	inline const decVector &GetAngularVelocity() const{ return pAngularVelocity; }
	
	/** \brief Set angular velocity in degree/s. */
	void SetAngularVelocity( const decVector &angVelo );
	
	/** \brief Mass in kg. */
	inline float GetMass() const{ return pMass; }
	
	/** \brief Set mass in kg. */
	void SetMass( float mass );
	
	/** \brief Gravity in m/s^2. */
	inline const decVector &GetGravity() const{ return pGravity; }
	
	/** \brief Set gravity in m/s^2. */
	void SetGravity( const decVector &gravity );
	
	/**
	 * \brief Collider is enabled.
	 * 
	 * Only enabled colliders take part in collision detection and response.
	 * Disabled colliders though still update attachments and can be used
	 * in manual collision detection.
	 */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/**
	 * \brief Set if collider is enabled.
	 * 
	 * Only enabled colliders take part in collision detection and response.
	 * Disabled colliders though still update attachments and can be used
	 * in manual collision detection.
	 */
	void SetEnabled( bool enabled );
	
	/** \brief Collision response type. */
	inline eResponseType GetResponseType() const{ return pResponseType; }
	
	/** \brief Set Collision response type. */
	void SetResponseType( eResponseType responseType );
	
	/** \brief Use local gravity instead of the world gravity. */
	inline bool GetUseLocalGravity() const{ return pUseLocalGravity; }
	
	/** \brief Set if local gravity is used instead of the world gravity. */
	void SetUseLocalGravity( bool useLocalGravity );
	
	
	
	/** \brief Factor for direct type force fields. */
	inline float GetForceFieldDirect() const{ return pForceFieldDirect; }
	
	/** \brief Set factor for direct type force fields. */
	void SetForceFieldDirect( float factor );
	
	/** \brief Factor for surface type force fields. */
	inline float GetForceFieldSurface() const{ return pForceFieldSurface; }
	
	/** \brief Set factor for surface type force fields. */
	void SetForceFieldSurface( float factor );
	
	/** \brief Factor for mass type force fields. */
	inline float GetForceFieldMass() const{ return pForceFieldMass; }
	
	/** \brief Set factor for mass type force fields. */
	void SetForceFieldMass( float factor );
	
	/** \brief Factor for speed type force fields. */
	inline float GetForceFieldSpeed() const{ return pForceFieldSpeed; }
	
	/** \brief Set factor for speed type force fields. */
	void SetForceFieldSpeed( float factor );
	
	
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter( const decCollisionFilter &filter );
	/*@}*/
	
	
	
	/** \name Force and Impuls */
	/*@{*/
	/**
	 * \brief Apply impuls at the center mass point.
	 * 
	 * This affects linear and angular velocity. This is the best way way to apply impacts.
	 */
	void ApplyImpuls( const decVector &impuls );
	
	/**
	 * \brief Apply impuls relative to the collider position.
	 * 
	 * This affects linear and angular velocity. This is the best way way to apply impacts.
	 */
	void ApplyImpulsAt( const decVector &impuls, const decVector &point );
	
	/**
	 * \brief Apply torque impuls at the center mass point.
	 * 
	 * This affects angular velocity. This is the best way way to apply impacts.
	 */
	void ApplyTorqueImpuls( const decVector &torqueImpuls );
	
	/**
	 * \brief Apply force at the center mass point.
	 * 
	 * This affects the total force and torque applied during the next debpWorld::DetectCollision only.
	 * Useful to apply continuous forces on an object. Has to be called every frame update.
	 */
	void ApplyForce( const decVector &force );
	
	/**
	 * \brief Apply force relative to the collider position.
	 * 
	 * This affects the total force and torque applied during the next debpWorld::DetectCollision only.
	 * Useful to apply continuous forces on an object. Has to be called every frame update.
	 */
	void ApplyForceAt( const decVector &force, const decVector &point );
	
	/**
	 * \brief Apply torque force at the center mass point.
	 * 
	 * This affects the total torque applied during the next debpWorld::DetectCollision only.
	 * Useful to apply continuous forces on an object. Has to be called every frame update.
	 */
	void ApplyTorque( const decVector &torque );
	/*@}*/
	
	
	
	/** \name Attachments */
	/*@{*/
	/** \brief Number of attachments. */
	inline int GetAttachmentCount() const{ return pAttachmentCount; }
	
	/**
	 * \brief Attachment at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetAttachmentCount().
	 */
	deColliderAttachment *GetAttachmentAt( int index ) const;
	
	/** \brief Resource is attached. */
	bool HasAttachmentWith( deResource *resource ) const;
	
	/** \brief Attachment with resource or NULL if not attached. */
	deColliderAttachment *GetAttachmentWith( deResource *resource ) const;
	
	/**
	 * \brief Add attachment.
	 * \throws deeInvalidParam Resource is already attached.
	 */
	void AddAttachment( deColliderAttachment *attachment );
	
	/**
	 * \brief Remove attachment.
	 * \throws deeInvalidParam \em attachment is absent.
	 */
	void RemoveAttachment( deColliderAttachment *attachment );
	
	/** \brief Remove all attachments. */
	void RemoveAllAttachments();
	
	/**
	 * \brief Notify peers attachment changed.
	 * 
	 * You have to call this explicitly if you changed attachments.
	 */
	void NotifyAttachmentChanged( int index );
	
	/**
	 * \brief Force update of all attachments.
	 * 
	 * You have to call this if want to reset colliders and components to a
	 * predefined state like after warping or resetting objects. This requires
	 * an attachment update outside the regular physics simulation run by the
	 * physics module to prevent exploding behavior.
	 */
	void AttachmentsForceUpdate();
	/*@}*/
	
	
	
	/** \name Constraints */
	/*@{*/
	/** \brief Number of constraints. */
	inline int GetConstraintCount() const{ return pConstraintCount; }
	
	/**
	 * \brief Constraint at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetConstraintCount().
	 */
	deColliderConstraint *GetConstraintAt( int index ) const;
	
	/** \brief Index of constraint or -1 if absent. */
	int IndexOfConstraint( deColliderConstraint *constraint ) const;
	
	/** \brief Constraint is present. */
	bool HasConstraint( deColliderConstraint *constraint ) const;
	
	/** \brief Add constraint. */
	void AddConstraint( deColliderConstraint *constraint );
	
	/**
	 * \brief Remove constraint.
	 * \throws deeInvalidParam \em constraint is absent.
	 */
	void RemoveConstraint( deColliderConstraint *constraint );
	
	/** \brief Remove all constraints. */
	void RemoveAllConstraints();
	
	/**
	 * \brief Notify peers  constraint has changed.
	 * 
	 * You have to call this explicitly if you changed something on an constraint.
	 */
	void NotifyConstraintChanged( int index );
	/*@}*/
	
	
	
	/** \name Ignore colliders */
	/*@{*/
	/** \brief Number of colliders to ignore. */
	int GetIgnoreColliderCount() const;
	
	/**
	 * \brief Collider to ignore at index.
	 * \throws deeInvalidParam \em index is less than 0.
	 * \throws deeInvalidParam \em index is greater or equal than GetIgnoreColliderCount()-1.
	 */
	deCollider *GetIgnoreColliderAt( int index ) const;
	
	/** \brief Collider to ignore is present. */
	bool HasIgnoreCollider( deCollider *collider ) const;
	
	/**
	 * \brief Add collider to ignore.
	 * \throws deeInvalidParam \em collider is present.
	 */
	void AddIgnoreCollider( deCollider *collider );
	
	/**
	 * \brief Remove collider to ignore.
	 * \throws deeInvalidParam \em collider is absent.
	 */
	void RemoveIgnoreCollider( deCollider *collider );
	
	/** \brief Remove all colliders to ignore. */
	void RemoveAllIgnoreColliders();
	/*@}*/
	
	
	
	/** \name Post physics collision tests */
	/*@{*/
	/** \brief Number of post physics collision tests. */
	int GetCollisionTestCount() const;
	
	/**
	 * \brief Post physics collision test at index.
	 * \throws deeInvalidParam \em index is less than 0.
	 * \throws deeInvalidParam \em index is greater or equal than GetCollisionTestCount()-1.
	 */
	deColliderCollisionTest *GetCollisionTestAt( int index ) const;
	
	/**
	 * \brief Index of post physics collision test or -1 if not found.
	 */
	int IndexOfCollisionTest( deColliderCollisionTest *collisionTest ) const;
	
	/**
	 * \brief Post physics collision test is present.
	 */
	bool HasCollisionTest( deColliderCollisionTest *collisionTest ) const;
	
	/** \brief Adds post physics collision test. */
	void AddCollisionTest( deColliderCollisionTest *collisionTest );
	
	/**
	 * \brief Remove post physics collision test.
	 * \throws deeInvalidParam \em collisionTest is absent.
	 */
	void RemoveCollisionTest( deColliderCollisionTest *collisionTest );
	
	/** \brief Remove all post physics collision tests. */
	void RemoveAllCollisionTests();
	
	/**
	 * \brief Notify peers a post physics collision test changed.
	 * 
	 * You have to call this explicitly if you changed post physics collision tests.
	 * 
	 * \throws deeInvalidParam \em index is less than 0 or greater than or equal to GetCollisionTestCount().
	 */
	void NotifyCollisionTestChanged( int index );
	
	/**
	 * \brief Notify peers a post physics collision test enabled or disabled.
	 * 
	 * You have to call this explicitly if you changed post physics collision tests enabled state.
	 * 
	 * \throws deeInvalidParam \em index is less than 0 or greater than or equal to GetCollisionTestCount().
	 */
	void NotifyCollisionTestEnableChanged( int index );
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/** \brief Test if a point is located inside the collider. */
	bool PointInside( const decDVector &point );
	
	/**
	 * \brief Test ray for collision with the collider.
	 * 
	 * For each collision deBaseScriptingCollider::CollisionResponse() is called. To
	 * stop testing set deCollisionInfo::SetStopTesting() to true. The distance parameter
	 * in the collision response represents the actual distance to the ray origin along
	 * the ray direction.
	 */
	void RayHits( const decDVector &rayOrigin, const decVector &rayDirection, deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test collider for collision with collider.
	 * 
	 * For each collision deBaseScriptingCollider::CollisionResponse() of the listener
	 * set in the tested collider is called. To stop testing set deCollisionInfo::SetStopTesting()
	 * to true.
	 */
	void ColliderHits( deCollider *collider, deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test moving collider for collision with collider.
	 * 
	 * For each collision deBaseScriptingCollider::CollisionResponse() of the listener
	 * set in the tested collider is called. To stop testing set deCollisionInfo::SetStopTesting()
	 * to true.
	 */
	void ColliderMoveHits( deCollider *collider, const decVector &displacement, deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test rotating collider for collision with collider.
	 * 
	 * For each collision deBaseScriptingCollider::CollisionResponse() of the listener
	 * set in the tested collider is called. To stop testing set deCollisionInfo::SetStopTesting()
	 * to true.
	 */
	void ColliderRotateHits( deCollider *collider, const decVector &rotation, deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test moving and rotating collider for collision with collider.
	 * 
	 * For each collision deBaseScriptingCollider::CollisionResponse() of the listener
	 * set in the tested collider is called. To stop testing set deCollisionInfo::SetStopTesting()
	 * to true.
	 */
	void ColliderMoveRotateHits( deCollider *collider, const decVector &displacement,
		const decVector &rotation, deBaseScriptingCollider *listener );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Physics system peer. */
	inline deBasePhysicsCollider *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer. */
	void SetPeerPhysics( deBasePhysicsCollider *peer );
	
	/** \brief Scripting system peer. */
	inline deBaseScriptingCollider *GetPeerScripting() const{ return pPeerScripting; }
	
	/** \brief Set scripting system peer. */
	void SetPeerScripting( deBaseScriptingCollider *peer );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit collider. */
	virtual void Visit( deColliderVisitor &visitor );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous collider in the parent world linked list. */
	inline deCollider *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next collider in the parent world linked list. */
	void SetLLWorldPrev( deCollider *collider );
	
	/** \brief Next collider in the parent world linked list. */
	inline deCollider *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next collider in the parent world linked list. */
	void SetLLWorldNext( deCollider *collider );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	int pFindAttachment( deColliderAttachment *attachment );

protected:
	// for deCollisionVolume
	void pNotifyCollisionVolumeChanged();
	
	// for deCollisionRigged
	void pNotifyRigChanged();
	
	// for deCollisionComponent
	void pNotifyComponentChanged();
	
	// for deColliderBone
	friend class deColliderBone;
	void pNotifyBonePositionChanged( int index );
	void pNotifyBoneOrientationChanged( int index );
	void pNotifyBoneLinearVelocityChanged( int index );
	void pNotifyBoneAngularVelocityChanged( int index );
	void pNotifyBonePropertiesChanged( int index );
	void pNotifyBoneDynamicChanged( int index );
};

#endif
