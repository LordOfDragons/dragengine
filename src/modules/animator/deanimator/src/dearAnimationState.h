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

// include only once
#ifndef _DEARANIMATIONSTATE_H_
#define _DEARANIMATIONSTATE_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefinitions



/**
 * Animation state class.
 * Stores the state of an animation frame.
 */
class dearAnimationState{
private:
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new bone state object. */
	dearAnimationState();
	/** Cleans up the bone state. */
	~dearAnimationState();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( const decVector &position );
	/** Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/** Sets the orientation. */
	void SetOrientation( const decQuaternion &orientation );
	/** Retrieves the size. */
	inline const decVector &GetSize() const{ return pSize; }
	/** Sets the size. */
	void SetSize( const decVector &size );
	/** Reset. */
	void Reset();
	/*@}*/
};

// end of include only once
#endif
