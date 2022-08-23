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

#ifndef _DEBPCOLLIDERBONES_H_
#define _DEBPCOLLIDERBONES_H_

#include "../debpCollisionObject.h"

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/resources/collider/deCollider.h>

class debpColliderBone;
class debpCollider;
class debpForceField;

class deColliderComponent;
class deColliderRig;
class deComponent;
class deRigBone;
class deRig;



/**
 * Collider bones.
 * 
 * Stores information about physics simulated bones. The bones list can contain NULL values
 * if the respective bone is not physically simulated. The physics bone list contains links
 * to all physically simulated bones only. This can be used to quickly iterate only bones
 * that are physically simulated.
 * 
 * Stores also a list of component or rig bones require update calculations. 
 */
class debpColliderBones{
private:
	debpCollider &pCollider;
	deColliderRig *pEngColliderRig;
	deColliderComponent *pEngColliderComponent;
	
	debpColliderBone **pBones;
	int pBoneCount;
	
	debpColliderBone **pBonesPhysics;
	int pBonePhysicsCount;
	
	int pRootBone;
	float pRigMass;
	float pRigInvMass;
	
	decPointerList pConstraints;
	
	
	
public:
	/** \name Constructors, destructors */
	/*@{*/
	/** Create bones list. */
	debpColliderBones( debpCollider &collider, deColliderComponent *engColliderComponent );
	
	/** Create bones list. */
	debpColliderBones( debpCollider &collider, deColliderRig *engColliderRig );
	
	/** Clean up bones list. */
	~debpColliderBones();
	/*@}*/
	
	
	
	/** \name Bones */
	/*@{*/
	/** Number of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/** Bone at index which can be NULL. */
	debpColliderBone *GetBoneAt( int index ) const;
	
	/** Bone at index is not NULL. */
	bool HasBoneAt( int index ) const;
	
	/** Root bone index or -1 if there is none. */
	inline int GetRootBoneIndex() const{ return pRootBone; }
	
	/** Root bone or NULL if there is none. */
	debpColliderBone *GetRootBone() const;
	
	/** Number of physics bones. */
	inline int GetBonePhysicsCount() const{ return pBonePhysicsCount; }
	
	/** hysics Physics bone at index. */
	debpColliderBone &GetBonePhysicsAt( int index ) const;
	
	/** One or more bones have constraints. */
	bool HasConstraints() const;
	
	/** One or more bones are breakable. */
	bool HasBreakableConstraints() const;
	
	/** Get component or NULL. */
	deComponent *GetComponent() const;
	
	/** Get rig or NULL. */
	deRig *GetRig() const;
	
	
	
	/**
	 * Two bones can collider with eath other.
	 * 
	 * Two bones can collider if they are not the same, they both have physics bodies and
	 * are not direct parent and child except not having any active constraints between them.
	 */
	bool CanBonesCollide( int bone1, int bone2 ) const;
	
	/** Set dynamic world for all bones and constraints attached to them. */
	void SetDynamicWorld( debpCollisionWorld *dynWorld );
	
	/**
	 * Update collider state from physics body state.
	 * \returns \em true if bones changed.
	 */
	bool UpdateFromBody();
	
	/** Set all bones dirty. */
	void SetAllBonesDirty();
	
	/** Activate physics body of dirty bones. */
	void ActivateDirtyPhysicsBodies();
	
	/** Set gravity for all physics bodies. */
	void SetGravity( const decVector &gravity );
	
	/** Apply force field. */
	void ApplyForceField( const debpForceField &forceField, float fluctStrength, float fluctDirection );
	
	/** Prepare bones for collision detection. */
	void PrepareForDetection( float elapsed );
	
	/** Prepare bone constraints for collision simulation step. */
	void PrepareConstraintsForStep();
	
	/** Check if bone constraint have broken. */
	void CheckConstraintsBroke();
	
	/** Update AABBs. */
	void UpdatePhyBodyAABBs();
	
	/** Update bone shapes. */
	void UpdateShapes();
	
	/** Update bone shapes. */
	void UpdateShapes( const decDMatrix &transformation );
	
	
	
	/** Create debug drawers. */
	void CreateDebugDrawers();
	
	/** Destroy debug drawers. */
	void DestroyDebugDrawers();
	
	/** Update debug drawers. */
	void UpdateDebugDrawersShape();
	
	/** Update bone debug drawers if present. */
	void UpdateDebugDrawers();
	
	
	
	/** Enable bone constraint. */
	void EnableConstraint( int bone, int constraint, bool enable );
	
	/** Apply impulse to all bones. */
	void ApplyImpuls( const decVector &impuls );
	
	void ApplyImpulsAt( const decVector &impuls, const decVector &position );
	
	void ApplyTorqueImpuls( const decVector &torqueImpuls );
	
	void ApplyForce( const decVector &force );
	
	void ApplyForceAt( const decVector &force, const decVector &point );
	
	void ApplyTorque( const decVector &torque );
	
	
	
	/** Set linear velocity of all bones. */
	void LinearVelocityChanged( const decVector &velocity );
	
	/** Set angular velocity of all bones. */
	void AngularVelocityChanged( const decVector &velocity );
	
	/** Enable all physics bodies. */
	void EnablePhysicsBodies( bool enabled );
	
	/** Update physics type of physics bodies. */
	void UpdatePhysicsType( deCollider::eResponseType responseType );
	
	/** Update physics type of bone physics body if present. */
	void UpdatePhysicsType( deCollider::eResponseType responseType, int bone );
	
	/** Collision filter or ignore colliders changed. */
	void CollisionFilteringChanged();
	
	
	
	/**
	 * Point is inside bone shapes.
	 * 
	 * Call UpdateShapes before using PointInside.
	 */
	bool PointInside( const decDVector &point );
	
	/**
	 * Calculate shape extends.
	 * 
	 * Call UpdateShapes before using PointInside.
	 */
	void CalcShapeExtends( decDVector &minExtend, decDVector &maxExtend );
	
	
	
	/** Update static collision tests. */
	void UpdateStaticCollisionTests();
	
	
	
private:
	void pCleanUp();
	void pCreateBones();
	void pSetBoneShape( int index, deRigBone &bone, decVector &scale );
	void pCreateConstraints( const deRig &rig );
	void pPreparePhyBones();
};

#endif
