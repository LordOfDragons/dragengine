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

#ifndef _DEARCOMPONENTBONESTATE_H_
#define _DEARCOMPONENTBONESTATE_H_

#include <dragengine/common/math/decMath.h>



/**
 * Component bone state.
 */
class dearComponentBoneState{
private:
	decVector pPosition;
	decQuaternion pRotation;
	decVector pScale;
	
	dearComponentBoneState *pParent;
	decMatrix pOriginalMatrix;
	decMatrix pMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create component bone state. */
	dearComponentBoneState();
	
	/** Clean up component bone state. */
	~dearComponentBoneState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Position. */
	inline const decVector &GetPosition() const{return pPosition;}
	
	/** Set position. */
	void SetPosition(const decVector &position);
	
	/** Rotation . */
	inline const decQuaternion &GetRotation() const{return pRotation;}
	
	/** Set rotation. */
	void SetRotation(const decQuaternion &rotation);
	
	/** Scale. */
	inline const decVector &GetScale() const{return pScale;}
	
	/** Set scale. */
	void SetScale(const decVector &scale);
	
	
	
	/** Parent state used to prepare the matrices or \em NULL. */
	inline dearComponentBoneState *GetParent() const{return pParent;}
	
	/** Set parent state used to prepare the matrices or \em NULL. */
	void SetParent(dearComponentBoneState *parent);
	
	/** Original matrix required to prepate matrix. */
	inline const decMatrix &GetOriginalMatrix() const{return pOriginalMatrix;}
	
	/** Set original matrix required to prepare matrix. */
	void SetOriginalMatrix(const decMatrix &matrix);
	
	/** Prepared matrix. */
	inline const decMatrix &GetMatrix() const{return pMatrix;}
	
	/** Set prepared matrix. */
	void SetMatrix(const decMatrix &matrix);
	/*@}*/
};

#endif
