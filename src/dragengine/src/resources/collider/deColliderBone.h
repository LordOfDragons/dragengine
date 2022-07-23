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

#ifndef _DECOLLIDERBONE_H_
#define _DECOLLIDERBONE_H_

#include "../../common/math/decMath.h"
#include "../deResource.h"

class deCollider;


/**
 * \brief Scene Collider Bone Object.
 *
 * Defines the motion state of a bone in a collider of the rigged
 * or component type.
 */
class DE_DLL_EXPORT deColliderBone{
private:
	deCollider *pCollider;
	int pIndex;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pLinearVelocity;
	decVector pAngularVelocity;
	
	decDMatrix pMatrix, pInvMatrix;
	
	float pMass;
	bool pDynamic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new collider bone object with the given parent collider. */
	deColliderBone( deCollider *collider, int index );
	
	/** \brief Clean up collider bone object. */
	~deColliderBone();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent collider. */
	inline deCollider *GetCollider() const{ return pCollider; }
	
	/** \brief Bone index inside the parent collider. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Position relative to parent bone or collider if there is no parent bone. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position relative to parent bone or collider if there is no parent bone. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Linear velocity in m/s. */
	inline const decVector &GetLinearVelocity() const{ return pLinearVelocity; }
	
	/** \brief Set linear velocity in m/s. */
	void SetLinearVelocity( const decVector &linVelo );
	
	/** \brief Angular velocity in degree/s. */
	inline const decVector &GetAngularVelocity() const{ return pAngularVelocity; }
	
	/** \brief Set angular velocity in degree/s. */
	void SetAngularVelocity( const decVector &angVelo );
	
	/** \brief Matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Inverse matrix. */
	inline const decDMatrix &GetInverseMatrix() const{ return pInvMatrix; }
	
	/** \brief Updates the matrix from the current position and orientation. */
	void UpdateMatrix();
	
	/** \brief Mass factor. */
	inline float GetMass() const{ return pMass; }
	
	/** \brief Set mass factor. */
	void SetMass( float mass );
	
	/** \brief Bone is dynamic. */
	inline bool GetDynamic() const{ return pDynamic; }
	
	/** \brief Sets of the bone is dynamic. */
	void SetDynamic( bool dynamic );
	/*@}*/
};

#endif
