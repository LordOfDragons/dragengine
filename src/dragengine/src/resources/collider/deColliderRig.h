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

#ifndef _DECOLLIDERRIG_H_
#define _DECOLLIDERRIG_H_

#include "deCollider.h"
#include "../rig/deRigReference.h"

class deColliderBone;
class deRigConstraint;


/**
 * \brief Rig collider.
 *
 * Colliders are used by the physics module to provide collision
 * detection and optional collision response. After the collision
 * detection stage is done each Collider contains the new state
 * of motion. The user can then update the game state depending
 * on the changes. The scripting peer receives all relevant events
 * during a collision detection run. The Collider is defined by a
 * Rig object. The Rig object contains a list of bones. Each bone
 * is defined by a Collision Volume and additional physical
 * properties. Each bone is considered to be one single collision
 * body able to collide with all other bones in the rig except the
 * direct parent bone.
 */
class DE_DLL_EXPORT deColliderRig : public deCollider{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deColliderRig> Ref;
	
	
	
private:
	deRigReference pRig;
	
	deColliderBone **pBones;
	int pBoneCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rig collider. */
	deColliderRig( deColliderManager *manager );
	
protected:
	/**
	 * \brief Clean up rig collider.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deColliderRig();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rig or NULL if not set. */
	inline deRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig or NULL if not set. */
	void SetRig( deRig *rig );
	
	
	
	/** \brief Number of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/**
	 * \brief Bone at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetBoneCount().
	 */
	deColliderBone &GetBoneAt( int index ) const;
	
	
	
	/**
	 * \brief Copy bone states from another rig collider bones if set.
	 * 
	 * The rigs do not have to match. Only matching bones are copied.
	 */
	void CopyStatesFromColliderRig( const deColliderRig &collider );
	
	/**
	 * \brief Copy a bone state from a collider rig bone if set.
	 * 
	 * The rigs do not have to match. Only if matching the bone state is copied.
	 */
	void CopyStateFromColliderRig( int bone, const deColliderRig &collider );
	
	/**
	 * \brief Copy a bone state from a collider rig bone if set.
	 * 
	 * The rigs do not have to match. Allows to copy staes between arbitrary bones.
	 */
	void CopyStateFromColliderRig( int boneFrom, int boneTo, const deColliderRig &collider );
	
	/** \brief Enable or disable a bone constraint. */
	void EnableBoneConstraint( int bone, int constraint, bool enable );
	
	/**
	 * \brief Replace a bone constraint.
	 * 
	 * The provided rig constraint only serves as source to copy the
	 * new parameters. It has to be freed by the called afterwards.
	 */
	void ReplaceBoneConstraint( int bone, int constraint, const deRigConstraint &replacement );
	/*@}*/
	
	
	
	/** \name Force and Impuls */
	/*@{*/
	/**
	 * \brief Apply impuls at center mass point of bone.
	 * 
	 * This affects linear and angular velocity. This is the best way way to apply impacts.
	 * 
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetBoneCount().
	 */
	void ApplyBoneImpuls( int bone, const decVector &impuls );
	
	/**
	 * \brief Apply impuls relative to bone position.
	 * 
	 * This affects linear and angular velocity. This is the best way way to apply impacts.
	 * 
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetBoneCount().
	 */
	void ApplyBoneImpulsAt( int bone, const decVector &impuls, const decVector &point );
	
	/**
	 * \brief Apply torque impuls at center mass point of bone.
	 * 
	 * This affects angular velocity. This is the best way way to apply impacts.
	 * 
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetBoneCount().
	 */
	void ApplyBoneTorqueImpuls( int bone, const decVector &torqueImpuls );
	
	/**
	 * \brief Apply force at center mass point of bone.
	 * 
	 * This affects the total force and torque applied during the next debpWorld::DetectCollision only.
	 * Useful to apply continuous forces on an object. Has to be called every frame update.
	 * 
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetBoneCount().
	 */
	void ApplyBoneForce( int bone, const decVector &force );
	
	/**
	 * \brief Apply force relative to bone position.
	 * 
	 * This affects the total force and torque applied during the next debpWorld::DetectCollision only.
	 * Useful to apply continuous forces on an object. Has to be called every frame update.
	 * 
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetBoneCount().
	 */
	void ApplyBoneForceAt( int bone, const decVector &force, const decVector &point );
	
	/**
	 * \brief Apply torque force at center mass point of bone.
	 * 
	 * This affects the total torque applied during the next debpWorld::DetectCollision only.
	 * Useful to apply continuous forces on an object. Has to be called every frame update.
	 * 
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetBoneCount().
	 */
	void ApplyBoneTorque( int bone, const decVector &torque );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit the collider. */
	virtual void Visit( deColliderVisitor &visitor );
	/*@}*/
};

#endif
