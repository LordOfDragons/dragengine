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
#include <dragengine/systems/modules/deBaseModule.h>


// struct void deoxrHandTracker::sFingerBending
/////////////////////////////////////////////////

void deoxrHandTracker::sFingerBending::Init( deInputDevice::eHandBones base,
deInputDevice::eHandBones tip, float angle0, float angle1 ){
	this->base = base;
	this->tip = tip;
	
	if( angle1 > angle0 ){
		angleLower = angle0 * DEG2RAD;
		angleUpper = angle1 * DEG2RAD;
		mapValueLower = 0.0f;
		mapValueUpper = 1.0f;
		
	}else{
		angleLower = angle1 * DEG2RAD;
		angleUpper = angle0 * DEG2RAD;
		mapValueLower = 1.0f;
		mapValueUpper = 0.0f;
	}
	
	value = 0.0f;
}



// class deoxrHandTracker
///////////////////////////

deoxrHandTracker::deoxrHandTracker( deoxrSession &session, XrHandEXT hand, deoxrSpace &space ) :
pSession( session ),
pHand( hand ),
pHandTracker( XR_NULL_HANDLE ),
pJointLocations( nullptr ),
pJointVelocities( nullptr ),
pJointCount( 0 ),
pPoseBones( nullptr ),
pPoseBoneCount( 0 ),
pMapBoneXrToDe( nullptr ),
pMapBoneXrToDeCount( 0 )
{
	memset( pBendFinger, 0, sizeof( sFingerBending ) * 5 );
	
	/*
	// these values allow for the fully closed hand only if squeezing the grip
	pBendFinger[ 0 ].Init( deInputDevice::ehbThumb0, deInputDevice::ehbThumb1, -15.0f, -55.0f );
	pBendFinger[ 1 ].Init( deInputDevice::ehbIndex0, deInputDevice::ehbIndex1, -15.0f, -95.0f );
	pBendFinger[ 2 ].Init( deInputDevice::ehbMiddle0, deInputDevice::ehbMiddle1, -15.0f, -94.0f );
	pBendFinger[ 3 ].Init( deInputDevice::ehbRing0, deInputDevice::ehbRing1, -15.0f, -92.0f );
	pBendFinger[ 4 ].Init( deInputDevice::ehbPinky0, deInputDevice::ehbPinky1, -15.0f, -90.0f );
	*/
	
	// these values allow for the fully closed hand without squeezing the grip
	pBendFinger[ 0 ].Init( deInputDevice::ehbThumb0, deInputDevice::ehbThumb1, -15.0f, -55.0f );
	pBendFinger[ 1 ].Init( deInputDevice::ehbIndex0, deInputDevice::ehbIndex1, -15.0f, -94.0f );
	pBendFinger[ 2 ].Init( deInputDevice::ehbMiddle0, deInputDevice::ehbMiddle1, -15.0f, -88.0f );
	pBendFinger[ 3 ].Init( deInputDevice::ehbRing0, deInputDevice::ehbRing1, -15.0f, -80.0f );
	pBendFinger[ 4 ].Init( deInputDevice::ehbPinky0, deInputDevice::ehbPinky1, -15.0f, -72.0f );
	
	int i;
	for( i=0; i<4; i++ ){
		pSpreadFinger[ i ] = 0.0f;
	}
	
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
		pLocateInfo.baseSpace = space.GetSpace();
		
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
		
		pMapBoneXrToDeCount = 20;
		
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
		return;
	}
	
	int i;
	for( i=0; i<pMapBoneXrToDeCount; i++ ){
		const sBoneMapping &mapping = pMapBoneXrToDe[ i ];
		
		mapping.bone->SetPosition( deoxrUtils::Convert( mapping.location->pose.position ) );
		mapping.bone->SetOrientation( deoxrUtils::Convert( mapping.location->pose.orientation ) );
		
		mapping.bone->SetLinearVelocity( deoxrUtils::Convert( mapping.velocity->linearVelocity ) );
		mapping.bone->SetAngularVelocity( deoxrUtils::ConvertEuler( mapping.velocity->angularVelocity ) );
	}
	
	/*{
		const decQuaternion &q1 = pPoseBones[deInputDevice::ehbWrist].GetOrientation();
		const decVector v1(decMatrix::CreateFromQuaternion(q1).TransformView());
		const decVector u1(decMatrix::CreateFromQuaternion(q1).TransformUp());
		instance.GetOxr().LogInfoFormat( "Wrist: q=(%f,%f,%f,%f) v=(%f,%f,%f) u=(%f,%f,%f)",
			q1.x, q1.y, q1.z, q1.w, v1.x, v1.y, v1.z, u1.x, u1.y, u1.z);
		
		const decQuaternion &q2 = pPoseBones[deInputDevice::ehbIndex3].GetOrientation();
		const decVector v2(decMatrix::CreateFromQuaternion(q2).TransformView());
		const decVector u2(decMatrix::CreateFromQuaternion(q2).TransformUp());
		instance.GetOxr().LogInfoFormat( "Tip: q=(%f,%f,%f,%f) v=(%f,%f,%f) u=(%f,%f,%f)",
			q2.x, q2.y, q2.z, q2.w, v2.x, v2.y, v2.z, u2.x, u2.y, u2.z);
	}*/
	
	pCalcFingerBending();
	pCalcFingerSpreading();
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

float deoxrHandTracker::GetBendFingerAt( int index ) const{
	if( index < 0 || index > 4 ){
		DETHROW( deeInvalidParam );
	}
	return pBendFinger[ index ].value;
}

float deoxrHandTracker::GetSpreadFingerAt( int index ) const{
	if( index < 0 || index > 3 ){
		DETHROW( deeInvalidParam );
	}
	return pSpreadFinger[ index ];
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

void deoxrHandTracker::pSetBoneMapping( int index, deInputDevice::eHandBones to, XrHandJointEXT from ){
	pMapBoneXrToDe[ index ].location = pJointLocations + from;
	pMapBoneXrToDe[ index ].velocity = pJointVelocities + from;
	pMapBoneXrToDe[ index ].bone = pPoseBones + to;
}

void deoxrHandTracker::pCalcFingerBending(){
	// using the second last finger orientation seems enough
	int i;
	for( i=0; i<5; i++ ){
		sFingerBending &fb = pBendFinger[ i ];
		const decVector view( decMatrix::CreateFromQuaternion( pPoseBones[ fb.tip ].GetOrientation()
			* pPoseBones[ fb.base ].GetOrientation().Conjugate() ).TransformView() );
		const float angle = atan2f( view.y, view.z );
		fb.value = decMath::linearStep( angle, fb.angleLower, fb.angleUpper, fb.mapValueLower, fb.mapValueUpper );
		/*
		if(i==2){
			pSession.GetSystem().GetInstance().GetOxr().LogInfoFormat(
				"Bend %d: angle=%f value=%f", i, angle * RAD2DEG, fb.value);
		}
		*/
	}
}

void deoxrHandTracker::pCalcFingerSpreading(){
	int i;
	for( i=0; i<4; i++ ){
		pSpreadFinger[ i ] = 0.0f;
	}
}
