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

#include "deoxrUtils.h"

#include <dragengine/common/exceptions.h>


// class deoxrUtils
/////////////////////

deoxrUtils::deoxrUtils(){}
deoxrUtils::~deoxrUtils(){}

// Utils
//////////

decVector deoxrUtils::Convert( const XrVector3f &vector ){
	return decVector( vector.x, vector.y, -vector.z );
}

decVector deoxrUtils::ConvertEuler( const XrVector3f &vector ){
	return decVector( vector.x, vector.y, -vector.z );
}

decQuaternion deoxrUtils::Convert( const XrQuaternionf &quaternion ){
	//return decQuaternion( quaternion.x, quaternion.y, -quaternion.z, -quaternion.w );
	// axis axis' = (axis.x, axis.y, -axis.z)
	// handedness switch: axis'' = -axis' = (-axis.x, -axis.y, axis.z)  
	return decQuaternion( -quaternion.x, -quaternion.y, quaternion.z, quaternion.w );
}

void deoxrUtils::Convert( const decQuaternion &quaternion, XrQuaternionf &xrquat ){
	/*
	xrquat.x = quaternion.x;
	xrquat.y = quaternion.y;
	xrquat.z = -quaternion.z;
	xrquat.w = -quaternion.w;
	*/
	xrquat.x = -quaternion.x;
	xrquat.y = -quaternion.y;
	xrquat.z = quaternion.z;
	xrquat.w = quaternion.w;
}

void deoxrUtils::Convert( const decVector &rotation, XrQuaternionf &xrquat ){
	Convert( decQuaternion::CreateFromEuler( rotation ), xrquat );
}

decMatrix deoxrUtils::Convert( const XrPosef &pose ){
	return decMatrix::CreateWorld( Convert( pose.position ), Convert( pose.orientation ) );
}
