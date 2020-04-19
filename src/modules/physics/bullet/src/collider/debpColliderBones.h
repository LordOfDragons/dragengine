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
 * \brief Collider bones.
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
	/** \brief Create bones list. */
	debpColliderBones( debpCollider &collider, deColliderComponent *engColliderComponent );
	
	/** \brief Create bones list. */
	debpColliderBones( debpCollider &collider, deColliderRig *engColliderRig );
	
	/** \brief Clean up bones list. */
	~debpColliderBones();
	/*@}*/
	
	
	
	/** \name Bones */
	/*@{*/
	/** \brief Number of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/** \brief Bone at index which can be NULL. */
	debpColliderBone *GetBoneAt( int index ) const;
	
	/** \brief Bone at index is not NULL. */
	bool HasBoneAt( int index ) const;
	
	/** \brief Root bone index or -1 if there is none. */
	inline int GetRootBoneIndex() const{ return pRootBone; }
	
	/** \brief Root bone or NULL if there is none. */
	debpColliderBone *GetRootBone() const;
	
	/** \brief Number of physics bones. */
	inline int GetBonePhysicsCount() const{ return pBonePhysicsCount; }
	
	/** \brief hysics Physics bone at index. */
	debpColliderBone &GetBonePhysicsAt( int index ) const;
	
	/** \brief One or more bones have constraints. */
	bool HasConstraints() const;
	
	/** \brief One or more bones are breakable. */
	bool HasBreakableConstraints() const;
	
	/** \brief Get component or NULL. */
	deComponent *GetComponent() const;
	
	/** \brief Get rig or NULL. */
	deRig *GetRig() const;
	
	
	
	/**
	 * \brief Two bones can collider with eath other.
	 * 
	 * Two bones can collider if they are not the same, they both have physics bodies and
	 * are not direct parent and child except not having any active constraints between them.
	 */
	bool CanBonesCollide( int bone1, int bone2 ) const;
	
	/** \brief Set dynamic world for all bones and constraints attached to them. */
	void SetDynamicWorld( debpCollisionWorld *dynWorld );
	
	/**
	 * \brief Update collider state from physics body state.
	 * \returns \em true if bones changed.
	 */
	bool UpdateFromBody();
	
	/** \brief Activate physics body of dirty bones. */
	void ActivateDirtyPhysicsBodies();
	
	/** \brief Set gravity for all physics bodies. */
	void SetGravity( const decVector &gravity );
	
	/** \brief Apply force field. */
	void ApplyForceField( const debpForceField &forceField, float fluctStrength, float fluctDirection );
	
	/** \brief Prepare bones for collision detection. */
	void PrepareForDetection( float elapsed );
	
	/** \brief Prepare bone constraints for collision simulation step. */
	void PrepareConstraintsForStep();
	
	/** \brief Check if bone constraint have broken. */
	void CheckConstraintsBroke();
	
	/** \brief Update AABBs. */
	void UpdatePhyBodyAABBs();
	
	/** \brief Update bone shapes. */
	void UpdateShapes();
	
	/** \brief Update bone shapes. */
	void UpdateShapes( const decDMatrix &transformation );
	
	
	
	/** \brief Create debug drawers. */
	void CreateDebugDrawers();
	
	/** \brief Destroy debug drawers. */
	void DestroyDebugDrawers();
	
	/** \brief Update debug drawers. */
	void UpdateDebugDrawersShape();
	
	/** \brief Update bone debug drawers if present. */
	void UpdateDebugDrawers();
	
	
	
	/** \brief Enable bone constraint. */
	void EnableConstraint( int bone, int constraint, bool enable );
	
	/** \brief Apply impulse to all bones. */
	void ApplyImpuls( const decVector &impuls );
	
	void ApplyImpulsAt( const decVector &impuls, const decVector &position );
	
	void ApplyTorqueImpuls( const decVector &torqueImpuls );
	
	void ApplyForce( const decVector &force );
	
	void ApplyForceAt( const decVector &force, const decVector &point );
	
	void ApplyTorque( const decVector &torque );
	
	
	
	/** \brief Set linear velocity of all bones. */
	void LinearVelocityChanged( const decVector &velocity );
	
	/** \brief Set angular velocity of all bones. */
	void AngularVelocityChanged( const decVector &velocity );
	
	/** \brief Enable all physics bodies. */
	void EnablePhysicsBodies( bool enabled );
	
	/** \brief Update physics type of physics bodies. */
	void UpdatePhysicsType( deCollider::eResponseType responseType );
	
	/** \brief Update physics type of bone physics body if present. */
	void UpdatePhysicsType( deCollider::eResponseType responseType, int bone );
	
	/** \brief Collision filter or ignore colliders changed. */
	void CollisionFilteringChanged();
	
	
	
	/**
	 * \brief Point is inside bone shapes.
	 * 
	 * Call UpdateShapes before using PointInside.
	 */
	bool PointInside( const decDVector &point );
	
	/**
	 * \brief Calculate shape extends.
	 * 
	 * Call UpdateShapes before using PointInside.
	 */
	void CalcShapeExtends( decDVector &minExtend, decDVector &maxExtend );
	
	
	
	/** \brief Update static collision tests. */
	void UpdateStaticCollisionTests();
	
	
	
private:
	void pCleanUp();
	void pCreateBones();
	void pSetBoneShape( int index, deRigBone &bone, decVector &scale );
	void pCreateConstraints( const deRig &rig );
	void pPreparePhyBones();
};

#endif
