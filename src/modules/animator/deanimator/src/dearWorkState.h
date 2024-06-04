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

#ifndef _DEARWORKSTATE_H_
#define _DEARWORKSTATE_H_

#include <dragengine/common/math/decMath.h>

class dearBoneState;



/**
 * Working state of a bone for rules doing complex calculations on bones before they are applied
 * to the bone states. Stores only the global matrix as this one required during complex
 * calculations. Stores also a link to the bone state it is applied to in the end.
 */
class dearWorkState{
private:
	dearBoneState *pBoneState;
	decMatrix pGlobalMatrix;
	decMatrix pInvGlobalMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new bone state object. */
	dearWorkState();
	/** Cleans up the bone state. */
	~dearWorkState();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the linked bone state. */
	inline dearBoneState *GetBoneState() const{ return pBoneState; }
	/** Sets the linked bone state. */
	void SetBoneState( dearBoneState *boneState );
	/** Retrieves the global matrix. */
	inline const decMatrix &GetGlobalMatrix() const{ return pGlobalMatrix; }
	/** Sets the global matrix. */
	void SetGlobalMatrix( const decMatrix &matrix );
	/** Retrieves the inverse global matrix. */
	inline const decMatrix &GetInverseGlobalMatrix() const{ return pInvGlobalMatrix; }
	/** Sets the inverse global matrix. */
	void SetInverseGlobalMatrix( const decMatrix &matrix );
	/*@}*/
};

#endif
