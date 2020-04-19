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

#ifndef _DECOMPONENTBONE_H_
#define _DECOMPONENTBONE_H_

#include "../../common/math/decMath.h"


/**
 * \brief Scene component bone.
 * 
 * Holds the state of a rig or component bone
 * \todo
 * - Remove weight matrix as the graphic module calculates it itself
 */
class deComponentBone{
private:
	int pParentBone;
	
	decVector pPosition;
	decQuaternion pRotation;
	decVector pScale;
	
	decMatrix pOrgMatrix;
	decMatrix pInvOrgMatrix;
	
	decMatrix pMatrix;
	decMatrix pInvMatrix;
	bool pDirtyInvMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new component bone. */
	deComponentBone();
	
	/** \brief Clean up component bone. */
	~deComponentBone();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position relative to the rig bone coordinate system. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position relative to the rig bone coordinate system. */
	void SetPosition( const decVector &position );
	
	/** \brief Rotation relative to the rig bone coordinate system. */
	inline const decQuaternion &GetRotation() const{ return pRotation; }
	
	/** \brief Set rotation relative to the rig bone coordinate system. */
	void SetRotation( const decQuaternion &rotation );
	
	/** \brief Scaling relative to the rig bone coordinate system. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set scaling relative to the rig bone coordinate system. */
	void SetScale( const decVector &scale );
	
	/**
	 * \brief Bone matrix relative to the component coordinate system.
	 * 
	 * Valid only after PrepareBones has been called on the parent component.
	 */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/**
	 * \brief Bone matrix relative to the component coordinate system.
	 * 
	 * Marks the inverse matrix dirty causing the inverse matrix to be
	 * calculated the next time GetInverseMatrix is called.
	 */
	void SetMatrix( const decMatrix &matrix );
	
	/**
	 * \brief Inverse bone matrix relative to the component coordinate system.
	 * 
	 * Calculates the inverse matrix from the matrix if dirty. Valid only after
	 * PrepareBones has been called on the parent component.
	 */
	const decMatrix &GetInverseMatrix();
	
	/** \brief Original matrix relative to the rig bone parent. */
	inline const decMatrix &GetOriginalMatrix() const{ return pOrgMatrix; }
	
	/** \brief Inverse original matrix relative to the rig bone parent. */
	inline const decMatrix &GetInverseOriginalMatrix() const{ return pInvOrgMatrix; }
	
	/** \brief Set the original matrix and original inverse matrix. */
	void SetOriginalMatrix( const decVector &position, const decVector &rotation );
	
	/** \brief Parent bone index. */
	inline int GetParentBone() const{ return pParentBone; }
	
	/** \brief Parent bone index. */
	void SetParentBone( int parentBone );
	/*@}*/
};

#endif
