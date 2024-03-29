/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
