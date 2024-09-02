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

#ifndef _DEOXRUTILS_H_
#define _DEOXRUTILS_H_

#include "deoxrBasics.h"

#include <dragengine/common/math/decMath.h>



/**
 * Oxr utils.
 * 
 * \note Coordinate Systems.
 * 
 * OpenXR uses X=right, Y=up and Z=backwards and is a right handed system.
 */
class deoxrUtils{
private:
	deoxrUtils();
	~deoxrUtils();
	
public:
	/** \name Utils */
	/*@{*/
	/** Convert XrVector3f to decVector. */
	static decVector Convert( const XrVector3f &vector );
	
	/** Convert euler XrVector3f to decVector. */
	static decVector ConvertEuler( const XrVector3f &vector );
	
	/** Convert XrQuaternionf to decQuaternion. */
	static decQuaternion Convert( const XrQuaternionf &quaternion );
	
	/** Convert decQuaternion to XrQuaternionf. */
	static void Convert( const decQuaternion &quaternion, XrQuaternionf &xrquat );
	
	/** Convert decVector to XrQuaternionf. */
	static void Convert( const decVector &rotation, XrQuaternionf &xrquat );
	
	/** Convert XrPosef to decMatrix. */
	static decMatrix Convert( const XrPosef &pose );
};

#endif

