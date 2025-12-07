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
class DE_DLL_EXPORT deComponentBone{
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
	void SetPosition(const decVector &position);
	
	/** \brief Rotation relative to the rig bone coordinate system. */
	inline const decQuaternion &GetRotation() const{ return pRotation; }
	
	/** \brief Set rotation relative to the rig bone coordinate system. */
	void SetRotation(const decQuaternion &rotation);
	
	/** \brief Scaling relative to the rig bone coordinate system. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set scaling relative to the rig bone coordinate system. */
	void SetScale(const decVector &scale);
	
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
	void SetMatrix(const decMatrix &matrix);
	
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
	void SetOriginalMatrix(const decVector &position, const decVector &rotation);
	
	/** \brief Parent bone index. */
	inline int GetParentBone() const{ return pParentBone; }
	
	/** \brief Parent bone index. */
	void SetParentBone(int parentBone);
	/*@}*/
};

#endif
