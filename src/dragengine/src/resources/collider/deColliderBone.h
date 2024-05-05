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
