/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>

#include "deInputDevicePose.h"



// Class deInputDevicePose
/////////////////////////////////

deInputDevicePose::deInputDevicePose(){
}

deInputDevicePose::deInputDevicePose( const deInputDevicePose &pose ) :
pPosition( pose.pPosition ),
pOrientation( pose.pOrientation ),
pLinearVelocity( pose.pLinearVelocity ),
pAngularVelocity( pose.pAngularVelocity ){
}

deInputDevicePose::~deInputDevicePose(){
}



// Device information
///////////////////////

void deInputDevicePose::SetPosition( const decVector &position ){
	pPosition = position;
}

void deInputDevicePose::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void deInputDevicePose::SetLinearVelocity( const decVector &velocity ){
	pLinearVelocity = velocity;
}

void deInputDevicePose::SetAngularVelocity( const decVector &velocity ){
	pAngularVelocity = velocity;
}



// Operators
//////////////

deInputDevicePose &deInputDevicePose::operator=( const deInputDevicePose &pose ){
	pPosition = pose.pPosition;
	pOrientation = pose.pOrientation;
	pLinearVelocity = pose.pLinearVelocity;
	pAngularVelocity = pose.pAngularVelocity;
	return *this;
}
