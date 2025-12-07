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

#ifndef _DEARIKWORKSTATE_H_
#define _DEARIKWORKSTATE_H_

#include <dragengine/common/math/decMath.h>

class dearBoneState;
class deRigBone;



/**
 * Working state of bone for inverse kinematic solving.
 */
class dearIKWorkState{
public:
	/** Axis Types. */
	enum eAxisTypes{
		/** Locked. */
		eatLocked,
		
		/** Limited. */
		eatLimited,
		
		/** Free. */
		eatFree
	};
	
	
	
private:
	int pBoneStateIndex;
	decMatrix pGlobalMatrix;
	decMatrix pInvGlobalMatrix;
	eAxisTypes pAxisTypeX;
	eAxisTypes pAxisTypeY;
	eAxisTypes pAxisTypeZ;
	bool pHasLimits;
	decVector pDampening;
	bool pHasDampening;
	decVector pLimitLower;
	decVector pLimitUpper;
	decQuaternion pLimitZeroQuat;
	decQuaternion pLimitZeroQuatInv;
	decVector pLockedRotation;
	float pWeight;
	float pLength;
	decVector pEndPosition;
	decQuaternion pRigLocalRot;
	decQuaternion pInvRigLocalRot;
	decQuaternion pLastGlobalOrientation;
	decQuaternion pInvLastGlobalOrientation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create bone state. */
	dearIKWorkState();
	
	/** Clean up bone state. */
	~dearIKWorkState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Bone state index. */
	inline int GetBoneStateIndex() const{return pBoneStateIndex;}
	
	/** Set bone state index. */
	void SetBoneStateIndex(int index);
	
	/** Global matrix. */
	inline const decMatrix &GetGlobalMatrix() const{return pGlobalMatrix;}
	
	/** Set global matrix. */
	void SetGlobalMatrix(const decMatrix &matrix);
	
	/** Inverse global matrix. */
	inline const decMatrix &GetInverseGlobalMatrix() const{return pInvGlobalMatrix;}
	
	/** Set inverse global matrix. */
	void SetInverseGlobalMatrix(const decMatrix &matrix);
	
	/** Transform global matrix and optionally update inverse. */
	void TransformGlobalMatrix(const decMatrix &matrix, bool updateInverse);
	
	
	
	/** X axis type. */
	inline eAxisTypes GetAxisTypeX() const{return pAxisTypeX;}
	
	/** Set X axis type. */
	void SetAxisTypeX(eAxisTypes type);
	
	/** Y axis type. */
	inline eAxisTypes GetAxisTypeY() const{return pAxisTypeY;}
	
	/** Set Y axis type. */
	void SetAxisTypeY(eAxisTypes type);
	
	/** Z axis type. */
	inline eAxisTypes GetAxisTypeZ() const{return pAxisTypeZ;}
	
	/** Set Z axis type. */
	void SetAxisTypeZ(eAxisTypes type);
	
	/** Any limits in effect. */
	inline bool GetHasLimits() const{return pHasLimits;}
	
	/** Set if any limits are in effect. */
	void SetHasLimits(bool hasLimits);
	
	/** Dampening. */
	inline const decVector &GetDampening() const{return pDampening;}
	
	/** Set dampening. */
	void SetDampening(const decVector &resistance);
	
	/** Has dampening. */
	inline bool GetHasDampening() const{return pHasDampening;}
	
	/** Lower limit. */
	inline const decVector &GetLimitLower() const{return pLimitLower;}
	
	/** Upper limit. */
	inline const decVector &GetLimitUpper() const{return pLimitUpper;}
	
	/** Limit center quaternion. */
	inline const decQuaternion &GetLimitZeroQuat() const{return  pLimitZeroQuat;}
	
	/** Inverse limit center quaternion. */
	inline const decQuaternion &GetLimitZeroQuatInv() const{return pLimitZeroQuatInv;}
	
	/** Update limits from rig bone. */
	void UpdateLimits(const deRigBone &bone);
	
	/** Locked rotation. */
	inline const decVector &GetLockedRotation() const{return pLockedRotation;}
	
	/** Set locked rotation. */
	void SetLockedRotation(const decVector &rotation);
	
	/** Weight. */
	inline float GetWeight() const{return pWeight;}
	
	/** Set weight. */
	void SetWeight(float weight);
	
	/** Length. */
	inline float GetLength() const{return pLength;}
	
	/** Set length. */
	void SetLength(float length);
	
	/** End position. */
	const decVector &GetEndPosition() const{return pEndPosition;}
	
	/** Set end position. */
	void SetEndPosition(const decVector &position);
	
	/** End position in component space. */
	decVector GetGlobalEnd() const;
	
	/** Rig local rotation. */
	inline const decQuaternion &GetRigLocalRotation() const{return pRigLocalRot;}
	
	/** Inverse rig local rotation. */
	inline const decQuaternion &GetInverseRigLocalRotation() const{return pInvRigLocalRot;}
	
	/** Set rig local rotation. */
	void SetRigLocalRotation(const decQuaternion &rotation);
	
	/** Last global orientation. */
	inline const decQuaternion &GetLastGlobalOrientation() const{return pLastGlobalOrientation;}
	
	/** Inverse last global orientation. */
	inline const decQuaternion &GetInvLastGlobalOrientation() const{return pInvLastGlobalOrientation;}
	
	/** Set last global orientation. */
	void SetLastGlobalOrientation(const decQuaternion &orientation);
	/*@}*/
};

#endif
