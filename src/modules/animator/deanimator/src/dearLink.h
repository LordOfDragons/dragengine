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

#ifndef _DEARLINK_H_
#define _DEARLINK_H_

#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/deAnimatorLink.h>

class decIntList;
class dearAnimatorInstance;
class deAnimatorLink;
class dearControllerStates;


/**
 * Animator link.
 */
class dearLink{
private:
	const dearAnimatorInstance &pInstance;
	deAnimatorLink pLink;
	const decCurveBezierEvaluator pEvaluator;
	int pBoneIndex;
	int pVPSIndex;
	const bool pWrapY;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create link. */
	dearLink( dearAnimatorInstance &instance, const deAnimatorLink &link,
		const decIntList &controllerMapping );
	
	/** Clean up link. */
	~dearLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Animator link. */
	inline const deAnimatorLink &GetLink() const{ return pLink; }
	
	/** Link has valid controller. */
	bool HasController() const;
	
	/** Controller index relative to parent animator instance. */
	int GetController() const;
	
	/** Link has valid bone. */
	bool HasBone() const;
	
	/** Link has valid vertex position set. */
	bool HasVPS() const;
	
	
	
	/** Value of link. */
	float GetValue( float defaultValue ) const;
	
	/** Vector of link. */
	void GetVector( decVector &vector ) const;
	
	/** Quaternion of link. */
	void GetQuaternion( decQuaternion &quaternion ) const;
	/*@}*/
};

#endif
