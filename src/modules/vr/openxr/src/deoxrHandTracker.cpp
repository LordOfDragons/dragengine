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
#include "deoxrHandTracker.h"
#include "deoxrSession.h"
#include "deoxrUtils.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrHandTracker
//////////////////////

deoxrHandTracker::deoxrHandTracker( deoxrSession &session, XrHandEXT hand ) :
pSession( session ),
pHand( hand ),
pHandTracker( XR_NULL_HANDLE ),
pJointLocations( nullptr ),
pJointVelocities( nullptr ),
pJointCount( 0 ),
pPoseBones( nullptr ),
pPoseBoneCount( 0 ),
pMapBoneXrToDe( nullptr )
{
	deoxrInstance &instance = session.GetSystem().GetInstance();
	
	try{
		// create hand tracker
		XrHandTrackerCreateInfoEXT createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.type = XR_TYPE_HAND_TRACKER_CREATE_INFO_EXT;
		createInfo.hand = hand;
		createInfo.handJointSet = XR_HAND_JOINT_SET_DEFAULT_EXT;
		
		OXR_CHECK( instance.xrCreateHandTrackerEXT( session.GetSession(), &createInfo, &pHandTracker ) );
		
		// create arrays to store joint locations and velocities
		pJointLocations = new XrHandJointLocationEXT[ XR_HAND_JOINT_COUNT_EXT ];
		memset( pJointLocations, 0, sizeof( XrHandJointLocationEXT ) * XR_HAND_JOINT_COUNT_EXT );
		
		pJointVelocities = new XrHandJointVelocityEXT[ XR_HAND_JOINT_COUNT_EXT ];
		memset( pJointVelocities, 0, sizeof( XrHandJointVelocityEXT ) * XR_HAND_JOINT_COUNT_EXT );
		
		for( pJointCount=0; pJointCount<XR_HAND_JOINT_COUNT_EXT; pJointCount++ ){
			pJointLocations[ pJointCount ].pose.orientation.w = 1.0f;
			pJointLocations[ pJointCount ].radius = 0.01f;
		}
		
		// initialize structures used to fetch joints
		memset( &pLocateInfo, 0, sizeof( pLocateInfo ) );
		pLocateInfo.type = XR_TYPE_HAND_JOINTS_LOCATE_INFO_EXT;
		pLocateInfo.baseSpace = pSession.GetSpace()->GetSpace();
		
		memset( &pLocations, 0, sizeof( pLocations ) );
		pLocations.type = XR_TYPE_HAND_JOINT_LOCATIONS_EXT;
		pLocations.jointCount = pJointCount;
		pLocations.jointLocations = pJointLocations;
		
		memset( &pVelocities, 0, sizeof( pVelocities ) );
		pVelocities.type = XR_TYPE_HAND_JOINT_VELOCITIES_EXT;
		pVelocities.jointCount = pJointCount;
		pVelocities.jointVelocities = pJointVelocities;
		pLocations.next = &pVelocities;
		
		const void **next = &pLocateInfo.next;
		
		if( instance.SupportsExtension( deoxrInstance::extEXTHandJointsMotionRange ) ){
			memset( &pMotionRange, 0, sizeof( pMotionRange ) );
			pMotionRange.type = XR_TYPE_HAND_JOINTS_MOTION_RANGE_INFO_EXT;
			pMotionRange.handJointsMotionRange = XR_HAND_JOINTS_MOTION_RANGE_UNOBSTRUCTED_EXT;
			
			*next = &pMotionRange;
			next = &pMotionRange.next;
		}
		
		// create input bones arrays to store data to be send to user
		pPoseBones = new deInputDevicePose[ deInputDevice::HandBoneCount ];
		pPoseBoneCount = deInputDevice::HandBoneCount;
		
		// create bone mapping table.
		// 
		// not used:
		// XR_HAND_JOINT_PALM_EXT
		// XR_HAND_JOINT_THUMB_TIP_EXT
		// XR_HAND_JOINT_INDEX_TIP_EXT
		// XR_HAND_JOINT_MIDDLE_TIP_EXT
		// XR_HAND_JOINT_RING_TIP_EXT
		// XR_HAND_JOINT_LITTLE_TIP_EXT
		
		pMapBoneXrToDe = new sBoneMapping[ 20 ];
		
		pSetBoneMapping( 0, deInputDevice::ehbWrist, XR_HAND_JOINT_WRIST_EXT );
		
		pSetBoneMapping( 1, deInputDevice::ehbThumb0, XR_HAND_JOINT_THUMB_METACARPAL_EXT );
		pSetBoneMapping( 2, deInputDevice::ehbThumb1, XR_HAND_JOINT_THUMB_PROXIMAL_EXT );
		pSetBoneMapping( 3, deInputDevice::ehbThumb2, XR_HAND_JOINT_THUMB_DISTAL_EXT );
		
		pSetBoneMapping( 4, deInputDevice::ehbIndex0, XR_HAND_JOINT_INDEX_METACARPAL_EXT );
		pSetBoneMapping( 5, deInputDevice::ehbIndex1, XR_HAND_JOINT_INDEX_PROXIMAL_EXT );
		pSetBoneMapping( 6, deInputDevice::ehbIndex2, XR_HAND_JOINT_INDEX_INTERMEDIATE_EXT );
		pSetBoneMapping( 7, deInputDevice::ehbIndex3, XR_HAND_JOINT_INDEX_DISTAL_EXT );
		
		pSetBoneMapping( 8, deInputDevice::ehbMiddle0, XR_HAND_JOINT_MIDDLE_METACARPAL_EXT );
		pSetBoneMapping( 9, deInputDevice::ehbMiddle1, XR_HAND_JOINT_MIDDLE_PROXIMAL_EXT );
		pSetBoneMapping( 10, deInputDevice::ehbMiddle2, XR_HAND_JOINT_MIDDLE_INTERMEDIATE_EXT );
		pSetBoneMapping( 11, deInputDevice::ehbMiddle3, XR_HAND_JOINT_MIDDLE_DISTAL_EXT );
		
		pSetBoneMapping( 12, deInputDevice::ehbRing0, XR_HAND_JOINT_RING_METACARPAL_EXT );
		pSetBoneMapping( 13, deInputDevice::ehbRing1, XR_HAND_JOINT_RING_PROXIMAL_EXT );
		pSetBoneMapping( 14, deInputDevice::ehbRing2, XR_HAND_JOINT_RING_INTERMEDIATE_EXT );
		pSetBoneMapping( 15, deInputDevice::ehbRing3, XR_HAND_JOINT_RING_DISTAL_EXT );
		
		pSetBoneMapping( 16, deInputDevice::ehbPinky0, XR_HAND_JOINT_LITTLE_METACARPAL_EXT );
		pSetBoneMapping( 17, deInputDevice::ehbPinky1, XR_HAND_JOINT_LITTLE_PROXIMAL_EXT );
		pSetBoneMapping( 18, deInputDevice::ehbPinky2, XR_HAND_JOINT_LITTLE_INTERMEDIATE_EXT );
		pSetBoneMapping( 19, deInputDevice::ehbPinky3, XR_HAND_JOINT_LITTLE_DISTAL_EXT );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrHandTracker::~deoxrHandTracker(){
	pCleanUp();
}



// Management
///////////////

void deoxrHandTracker::Locate(){
	const deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	pLocateInfo.time = pSession.GetPredictedDisplayTime();
	
	if( ! XR_SUCCEEDED( instance.xrLocateHandJointsEXT( pHandTracker, &pLocateInfo, &pLocations ) )
	|| ! pLocations.isActive ){
// 		deVROpenXR &oxr = pSession.GetSystem().GetInstance().GetOxr();
// 		oxr.LogInfoFormat("FAIL %d", pLocations.isActive);
		return;
	}
	
	int i;
	for( i=0; i<pPoseBoneCount; i++ ){
		const sBoneMapping &mapping = pMapBoneXrToDe[ i ];
		
		mapping.bone->SetPosition( deoxrUtils::Convert( mapping.location->pose.position ) );
		mapping.bone->SetOrientation( deoxrUtils::Convert( mapping.location->pose.orientation ) );
		
		mapping.bone->SetLinearVelocity( deoxrUtils::Convert( mapping.velocity->linearVelocity ) );
		mapping.bone->SetAngularVelocity( deoxrUtils::ConvertEuler( mapping.velocity->angularVelocity ) );
	}
}

deInputDevicePose &deoxrHandTracker::GetPoseBoneAt( int index ){
	if( index < 0 || index >= pPoseBoneCount ){
		DETHROW( deeInvalidParam );
	}
	return pPoseBones[ index ];
}

const deInputDevicePose &deoxrHandTracker::GetPoseBoneAt( int index ) const{
	if( index < 0 || index >= pPoseBoneCount ){
		DETHROW( deeInvalidParam );
	}
	return pPoseBones[ index ];
}

void deoxrHandTracker::LogPoseBones( const char *prefix ) const{
	deVROpenXR &oxr = pSession.GetSystem().GetInstance().GetOxr();
	int i;
	
	for( i=0; i<pPoseBoneCount; i++ ){
		const deInputDevicePose &pose = pPoseBones[ i ];
		const decVector &p = pose.GetPosition();
		const decVector o( pose.GetOrientation().GetEulerAngles() * RAD2DEG );
		const decVector &lv = pose.GetLinearVelocity();
		const decVector av( pose.GetAngularVelocity() * RAD2DEG );
		oxr.LogInfoFormat( "%sBone %02d: pos=(%.3f,%.3f,%.3f) rot=(% 3f,% 3f,% 3f) lv=(% 3.1f,% 3.1f,% 3.1f) av=(% 3f,% 3f,% 3f)",
			prefix, i, p.x, p.y, p.z, o.x, o.y, o.z, lv.x, lv.y, lv.z, av.x, av.y, av.z);
	}
}



// Private Functions
//////////////////////

void deoxrHandTracker::pCleanUp(){
	if( pHandTracker ){
		pSession.GetSystem().GetInstance().xrDestroyHandTrackerEXT( pHandTracker );
	}
	
	if( pMapBoneXrToDe ){
		delete [] pMapBoneXrToDe;
	}
	
	if( pPoseBones ){
		delete [] pPoseBones;
	}
	
	if( pJointLocations ){
		delete [] pJointLocations;
	}
	if( pJointVelocities ){
		delete [] pJointVelocities;
	}
}

void deoxrHandTracker::pSetBoneMapping( int index, int from, int to ){
	pMapBoneXrToDe[ index ].location = pJointLocations + from;
	pMapBoneXrToDe[ index ].velocity = pJointVelocities + from;
	pMapBoneXrToDe[ index ].bone = pPoseBones + to;
}
