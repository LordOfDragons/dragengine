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
	void SetIndex(int index);
	
	/** \brief Bone name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set bone name. */
	void SetName(const char *name);
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Rotation. */
	inline const decQuaternion &GetRotation() const{ return pRotation; }
	
	/** \brief Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set geometry. */
	void SetGeometry(const decVector &position, const decQuaternion &rotation, const decVector &scale);
	
	/** \brief Set geometry. */
	void SetGeometry(const deComponentBone &bone);
	
	/** \brief Original matrix (rig relative). */
	inline const decMatrix &GetOriginalMatrix() const{ return pOriginalMatrix; }
	
	/** \brief Set original matrix (rig relative). */
	void SetOriginalMatrix(const decMatrix &matrix);
	
	/** \brief Rig inverse matrix. */
	inline const decMatrix &GetRigInvMatrix() const{ return pRigInverseMatrix; }
	
	/** \brief Set rig inverse matrix. */
	void SetRigInverseMatrix(const decMatrix &matrix);
	
	/** \brief Parent bone. */
	inline deoalAComponentBone *GetParent() const{ return pParent; }
	
	/** \brief Set parent bone. */
	void SetParent(deoalAComponentBone *bone);
	
	/** \brief Matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Weight matrix. */
	inline const decMatrix &GetWeightMatrix() const{ return pWeightMatrix; }
	
	/** \brief Update matrix. */
	void UpdateMatrix();
	/*@}*/
};

#endif

