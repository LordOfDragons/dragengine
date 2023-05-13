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

deoxrSpace::deoxrSpace( deoxrSession &session, XrReferenceSpaceType referenceType ) :
pSession( session ),
pSpace( XR_NULL_HANDLE )
{
	try{
		XrReferenceSpaceCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO;
		createInfo.referenceSpaceType = referenceType;
		createInfo.poseInReferenceSpace.orientation.w = 1.0f;
		
		OXR_CHECK( session.GetSystem().GetInstance().xrCreateReferenceSpace(
			session.GetSession(), &createInfo, &pSpace ) );
		
		// xrGetReferenceSpaceBoundsRect : get chaperone
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrSpace::deoxrSpace( deoxrSession &session, const deoxrAction &action,
	const deoxrPath &subactionPath, const decVector &poseRotation ) :
pSession( session ),
pSpace( XR_NULL_HANDLE )
{
	try{
		XrActionSpaceCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_ACTION_SPACE_CREATE_INFO;
		createInfo.action = action.GetAction();
		createInfo.subactionPath = subactionPath;
		deoxrUtils::Convert( poseRotation, createInfo.poseInActionSpace.orientation );
		
		OXR_CHECK( session.GetSystem().GetInstance().xrCreateActionSpace(
			session.GetSession(), &createInfo, &pSpace ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrSpace::deoxrSpace( deoxrSession &session, const deoxrAction &action ) :
pSession( session ),
pSpace( XR_NULL_HANDLE )
{
	try{
		XrActionSpaceCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_ACTION_SPACE_CREATE_INFO;
		createInfo.action = action.GetAction();
		createInfo.poseInActionSpace.orientation.w = 1.0f;
		
		OXR_CHECK( session.GetSystem().GetInstance().xrCreateActionSpace(
			session.GetSession(), &createInfo, &pSpace ) );
		
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

void deoxrSpace::LocateSpaceEye( XrTime time, decVector &position, decQuaternion &orientation,
	decVector &linearVelocity, decVector &angularVelocity ) const{
	const deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	XrEyeGazeSampleTimeEXT eyeGazeSampleTime;
	memset( &eyeGazeSampleTime, 0, sizeof( eyeGazeSampleTime ) );
	eyeGazeSampleTime.type = XR_TYPE_EYE_GAZE_SAMPLE_TIME_EXT;
	
	XrSpaceVelocity velocity;
	memset( &velocity, 0, sizeof( velocity ) );
	velocity.type = XR_TYPE_SPACE_VELOCITY;
	velocity.next = &eyeGazeSampleTime;
	
	XrSpaceLocation location;
	memset( &location, 0, sizeof( location ) );
	location.type = XR_TYPE_SPACE_LOCATION;
	location.pose.orientation.w = 1.0f;
	location.next = &velocity;
	
	if( ! XR_SUCCEEDED( instance.xrLocateSpace( pSpace, pSession.GetSpaceLocal()->pSpace, time, &location ) ) ){
		return;
	}
	
	if( ( location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT ) != 0 ){
		const decVector converted( deoxrUtils::Convert( location.pose.position ) );

		// while blinking the eyes can not be tracked. VIVE does not clear the valid bit
		// which is incorrect since this causes the eyes to turn inside the head while
		// blinking. in the case of VIVE this situation returns (0,0,0) as position
		// and (0,0,0,-1) as orientation. if this is found we keep the last position
		// and orientation to not mess up the application
		if( location.pose.position.x == 0.0f && location.pose.position.y == 0.0f
		&& location.pose.position.z == 0.0f && location.pose.orientation.x == 0.0f
		&& location.pose.orientation.y == 0.0f && location.pose.orientation.z == 0.0f ){
			return;
		}

		position = deoxrUtils::Convert( location.pose.position );
	}
	
	if( ( location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT ) != 0 ){
		orientation = deoxrUtils::Convert( location.pose.orientation )
			* decQuaternion::CreateFromEulerY( 180.0f * DEG2RAD );
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
