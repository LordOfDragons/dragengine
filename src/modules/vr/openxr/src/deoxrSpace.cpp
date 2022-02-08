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

#include <stdlib.h>
#include <string.h>

#include "deVROpenXR.h"
#include "deoxrSpace.h"
#include "deoxrSession.h"
#include "deoxrUtils.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrSpace
//////////////////////

deoxrSpace::deoxrSpace( deoxrSession &session, XrReferenceSpaceType type ) :
pSession( session ),
pType( type ),
pSpace( XR_NULL_HANDLE )
{
	try{
		XrReferenceSpaceCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO;
		createInfo.referenceSpaceType = type;
		createInfo.poseInReferenceSpace.orientation.w = 1.0f;
		
		OXR_CHECK( session.GetSystem().GetInstance().GetOxr(),
			session.GetSystem().GetInstance().xrCreateReferenceSpace(
				session.GetSession(), &createInfo, &pSpace ) );
		
		// xrGetReferenceSpaceBoundsRect : get chaperone
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrSpace::~deoxrSpace(){
	pCleanUp();
}



// Management
///////////////

void deoxrSpace::LocateSpace( const deoxrSpace &space, XrTime time,
decVector &position, decQuaternion &orientation ) const{
	const deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	XrSpaceLocation location;
	memset( &location, 0, sizeof( location ) );
	location.type = XR_TYPE_SPACE_LOCATION;
	
	//OXR_CHECK( instance.GetOxr(), instance.xrLocateSpace( pSpace, space.pSpace, time, &location ) );
	if( ! XR_SUCCEEDED( instance.xrLocateSpace( pSpace, space.pSpace, time, &location ) ) ){
		return;
	}
	
	if( ( location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT ) != 0 ){
		position = deoxrUtils::Convert( location.pose.position );
	}
	
	if( ( location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT ) != 0 ){
		orientation = deoxrUtils::Convert( location.pose.orientation );
	}
}

void deoxrSpace::LocateSpace( const deoxrSpace &space, XrTime time, decVector &position,
decQuaternion &orientation, decVector &linearVelocity, decVector &angularVelocity ) const{
	const deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	XrSpaceVelocity velocity;
	memset( &velocity, 0, sizeof( velocity ) );
	velocity.type = XR_TYPE_SPACE_VELOCITY;
	
	XrSpaceLocation location;
	memset( &location, 0, sizeof( location ) );
	location.type = XR_TYPE_SPACE_LOCATION;
	location.pose.orientation.w = 1.0f;
	location.next = &velocity;
	
	//OXR_CHECK( instance.GetOxr(), instance.xrLocateSpace( pSpace, space.pSpace, time, &location ) );
	if( ! XR_SUCCEEDED( instance.xrLocateSpace( pSpace, space.pSpace, time, &location ) ) ){
		return;
	}
	
	if( ( location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT ) != 0 ){
		position = deoxrUtils::Convert( location.pose.position );
	}
	
	if( ( location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT ) != 0 ){
		orientation = deoxrUtils::Convert( location.pose.orientation );
	}
	
	if( ( velocity.velocityFlags & XR_SPACE_VELOCITY_LINEAR_VALID_BIT ) != 0 ){
		linearVelocity = deoxrUtils::Convert( velocity.linearVelocity );
	}
	
	if( ( velocity.velocityFlags & XR_SPACE_VELOCITY_ANGULAR_VALID_BIT ) != 0 ){
		angularVelocity = deoxrUtils::Convert( velocity.angularVelocity );
	}
}



// Private Functions
//////////////////////

void deoxrSpace::pCleanUp(){
}
