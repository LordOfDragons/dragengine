/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALACOMPONENTBONE_H_
#define _DEOALACOMPONENTBONE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deComponentBone;


/**
 * \brief Component bone.
 */
class deoalAComponentBone{
private:
	int pIndex;
	decString pName;
	
	decVector pPosition;
	decQuaternion pRotation;
	decVector pScale;
	decMatrix pOriginalMatrix;
	decMatrix pRigInverseMatrix;
	deoalAComponentBone *pParent;
	
	decMatrix pMatrix;
	decMatrix pWeightMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create component bone. */
	deoalAComponentBone();
	
	/** \brief Clean up component bone. */
	~deoalAComponentBone();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex( int index );
	
	/** \brief Bone name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set bone name. */
	void SetName( const char *name );
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Rotation. */
	inline const decQuaternion &GetRotation() const{ return pRotation; }
	
	/** \brief Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set geometry. */
	void SetGeometry( const decVector &position, const decQuaternion &rotation, const decVector &scale );
	
	/** \brief Set geometry. */
	void SetGeometry( const deComponentBone &bone );
	
	/** \brief Original matrix (rig relative). */
	inline const decMatrix &GetOriginalMatrix() const{ return pOriginalMatrix; }
	
	/** \brief Set original matrix (rig relative). */
	void SetOriginalMatrix( const decMatrix &matrix );
	
	/** \brief Rig inverse matrix. */
	inline const decMatrix &GetRigInvMatrix() const{ return pRigInverseMatrix; }
	
	/** \brief Set rig inverse matrix. */
	void SetRigInverseMatrix( const decMatrix &matrix );
	
	/** \brief Parent bone. */
	inline deoalAComponentBone *GetParent() const{ return pParent; }
	
	/** \brief Set parent bone. */
	void SetParent( deoalAComponentBone *bone );
	
	/** \brief Matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Weight matrix. */
	inline const decMatrix &GetWeightMatrix() const{ return pWeightMatrix; }
	
	/** \brief Update matrix. */
	void UpdateMatrix();
	/*@}*/
};

#endif

