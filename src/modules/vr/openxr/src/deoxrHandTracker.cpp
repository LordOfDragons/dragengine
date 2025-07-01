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

#include <stdlib.h>
#include <string.h>

#include "deVROpenXR.h"
#include "deoxrHandTracker.h"
#include "deoxrSession.h"
#include "deoxrUtils.h"
#include "device/deoxrDevice.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// structs
////////////

void deoxrHandTracker::sFingerBending::Init(deInputDevice::eHandBones pbase,
deInputDevice::eHandBones ptip, float angle0, float angle1){
	base = pbase;
	tip = ptip;
	
	if(angle1 > angle0){
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


void deoxrHandTracker::sFingerSpreading::Init(deInputDevice::eHandBones pbone1,
deInputDevice::eHandBones pbone2, float angle0, float angle1){
	bone1 = pbone1;
	bone2 = pbone2;
	
	angleLower = angle0 * DEG2RAD;
	angleUpper = angle1 * DEG2RAD;
	mapValueLower = 0.0f;
	mapValueUpper = 1.0f;
	
	value = 0.0f;
}


void deoxrHandTracker::sFixBrokenRotationBone::Init(
deInputDevice::eHandBones pbase, deInputDevice::eHandBones ptarget){
	base = pbase;
	target = ptarget;
}


// class deoxrHandTracker
///////////////////////////

deoxrHandTracker::deoxrHandTracker( deoxrSession &session, deoxrDevice &device, XrHandEXT hand ) :
pSession( session ),
pDevice( device ),
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
	memset(pBendFinger, 0, sizeof(sFingerBending) * FingerBendingCount);
	memset(pSpreadFinger, 0, sizeof(sFingerSpreading) * SpreadFingerCount);
	memset(pFixBrokenRotationBone, 0, sizeof(sFixBrokenRotationBone) * FoxBrokenRotationBoneCount);
	
	/*
	// these values allow for the fully closed hand only if squeezing the grip
	pBendFinger[ 0 ].Init( deInputDevice::ehbThumb0, deInputDevice::ehbThumb1, -15.0f, -55.0f );
	pBendFinger[ 1 ].Init( deInputDevice::ehbIndex0, deInputDevice::ehbIndex1, -15.0f, -95.0f );
	pBendFinger[ 2 ].Init( deInputDevice::ehbMiddle0, deInputDevice::ehbMiddle1, -15.0f, -94.0f );
	pBendFinger[ 3 ].Init( deInputDevice::ehbRing0, deInputDevice::ehbRing1, -15.0f, -92.0f );
	pBendFinger[ 4 ].Init( deInputDevice::ehbPinky0, deInputDevice::ehbPinky1, -15.0f, -90.0f );
	*/
	
	// these values allow for the fully closed hand without squeezing the grip
	pBendFinger[0].Init(deInputDevice::ehbThumb0, deInputDevice::ehbThumb1, -15.0f, -55.0f);
	pBendFinger[1].Init(deInputDevice::ehbIndex1, deInputDevice::ehbIndex2, -15.0f, -94.0f);
	pBendFinger[2].Init(deInputDevice::ehbMiddle1, deInputDevice::ehbMiddle2, -15.0f, -88.0f);
	pBendFinger[3].Init(deInputDevice::ehbRing1, deInputDevice::ehbRing2, -15.0f, -80.0f);
	pBendFinger[4].Init(deInputDevice::ehbPinky1, deInputDevice::ehbPinky2, -15.0f, -72.0f);
	
	pSpreadFinger[0].Init(deInputDevice::ehbThumb1, deInputDevice::ehbIndex1, 0, 90);
	pSpreadFinger[1].Init(deInputDevice::ehbIndex1, deInputDevice::ehbMiddle1, 0, 30);
	pSpreadFinger[2].Init(deInputDevice::ehbMiddle1, deInputDevice::ehbRing1, 0, 30);
	pSpreadFinger[3].Init(deInputDevice::ehbRing1, deInputDevice::ehbPinky1, 0, 30);
	
	pFixBrokenRotationBone[0].Init(deInputDevice::ehbThumb0, deInputDevice::ehbThumb1);
	pFixBrokenRotationBone[1].Init(deInputDevice::ehbThumb1, deInputDevice::ehbThumb2);
	pFixBrokenRotationBone[2].Init(deInputDevice::ehbIndex0, deInputDevice::ehbIndex1);
	pFixBrokenRotationBone[3].Init(deInputDevice::ehbIndex1, deInputDevice::ehbIndex2);
	pFixBrokenRotationBone[4].Init(deInputDevice::ehbIndex2, deInputDevice::ehbIndex3);
	pFixBrokenRotationBone[5].Init(deInputDevice::ehbMiddle0, deInputDevice::ehbMiddle1);
	pFixBrokenRotationBone[6].Init(deInputDevice::ehbMiddle1, deInputDevice::ehbMiddle2);
	pFixBrokenRotationBone[7].Init(deInputDevice::ehbMiddle2, deInputDevice::ehbMiddle3);
	pFixBrokenRotationBone[8].Init(deInputDevice::ehbRing0, deInputDevice::ehbRing1);
	pFixBrokenRotationBone[9].Init(deInputDevice::ehbRing1, deInputDevice::ehbRing2);
	pFixBrokenRotationBone[10].Init(deInputDevice::ehbRing2, deInputDevice::ehbRing3);
	pFixBrokenRotationBone[11].Init(deInputDevice::ehbPinky0, deInputDevice::ehbPinky1);
	pFixBrokenRotationBone[12].Init(deInputDevice::ehbPinky1, deInputDevice::ehbPinky2);
	pFixBrokenRotationBone[13].Init(deInputDevice::ehbPinky2, deInputDevice::ehbPinky3);
	
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
		
		// initialize structures used to fetch joints. we use always session space
		// not device space if present due to bugs in vr drivers causing hand tracking
		// to be always reported in world space instead of the space we tell it to use
		memset( &pLocateInfo, 0, sizeof( pLocateInfo ) );
		pLocateInfo.type = XR_TYPE_HAND_JOINTS_LOCATE_INFO_EXT;
		pLocateInfo.baseSpace = session.GetSpace()->GetSpace();
		
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

#define FLAGS_POS_VALID (XR_SPACE_LOCATION_POSITION_TRACKED_BIT | XR_SPACE_LOCATION_POSITION_VALID_BIT)
#define FLAGS_ROT_VALID (XR_SPACE_LOCATION_ORIENTATION_TRACKED_BIT | XR_SPACE_LOCATION_ORIENTATION_VALID_BIT)
#define FLAGS_LVELO_VALID XR_SPACE_VELOCITY_LINEAR_VALID_BIT
#define FLAGS_AVELO_VALID XR_SPACE_VELOCITY_ANGULAR_VALID_BIT

void deoxrHandTracker::Locate(){
	const deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	pLocateInfo.time = pSession.GetPredictedDisplayTime();
	
	if( ! XR_SUCCEEDED( instance.xrLocateHandJointsEXT( pHandTracker, &pLocateInfo, &pLocations ) )
	|| ! pLocations.isActive ){
		return;
	}
	
	decMatrix invDeviceMatrix;
	int i;
	
	if(pDevice.GetSpacePose()){
		const deInputDevicePose &pose = pDevice.GetDirectDevicePose();
		invDeviceMatrix = decMatrix::CreateWorld(pose.GetPosition(), pose.GetOrientation()).QuickInvert();
	}
	
	for(i=0; i<pMapBoneXrToDeCount; i++){
		const sBoneMapping &mapping = pMapBoneXrToDe[i];
		
		const decMatrix matrix(decMatrix::CreateWorld(
			deoxrUtils::Convert(mapping.location->pose.position),
			deoxrUtils::Convert(mapping.location->pose.orientation)) * invDeviceMatrix);
		
		if((mapping.location->locationFlags & FLAGS_POS_VALID) == FLAGS_POS_VALID){
			mapping.bone->SetPosition(matrix.GetPosition());
		}
		
		if((mapping.location->locationFlags & FLAGS_ROT_VALID) == FLAGS_ROT_VALID){
			mapping.bone->SetOrientation(matrix.ToQuaternion());
		}
		
		if((mapping.velocity->velocityFlags & FLAGS_LVELO_VALID) == FLAGS_LVELO_VALID){
			mapping.bone->SetLinearVelocity(invDeviceMatrix.TransformNormal(
				deoxrUtils::Convert(mapping.velocity->linearVelocity)));
		}
		
		if((mapping.velocity->velocityFlags & FLAGS_AVELO_VALID) == FLAGS_AVELO_VALID){
			mapping.bone->SetAngularVelocity(invDeviceMatrix.TransformNormal(
				deoxrUtils::ConvertEuler(mapping.velocity->angularVelocity)));
		}
		
		/*{
		const decVector r(mapping.bone->GetOrientation().GetEulerAngles() * RAD2DEG);
		instance.GetOxr().LogInfoFormat("BoneMap %02d[%02d]: r=(%f,%f,%f) f=(%d,%d)",
			mapping.constDe, mapping.constOxr, r.x, r.y, r.z,
			(int)mapping.location->locationFlags, (int)mapping.velocity->velocityFlags);
		}*/
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
	
	pFixBrokenBoneRotations();
	pCalcFingerBending();
	pCalcFingerSpreading();
}

deInputDevicePose &deoxrHandTracker::GetPoseBoneAt(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pPoseBoneCount)
	
	return pPoseBones[index];
}

const deInputDevicePose &deoxrHandTracker::GetPoseBoneAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pPoseBoneCount)
	
	return pPoseBones[index];
}

float deoxrHandTracker::GetBendFingerAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < FingerBendingCount)
	
	return pBendFinger[index].value;
}

float deoxrHandTracker::GetSpreadFingerAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < SpreadFingerCount)
	
	return pSpreadFinger[index].value;
}

void deoxrHandTracker::LogPoseBones(const char *prefix) const{
	deVROpenXR &oxr = pSession.GetSystem().GetInstance().GetOxr();
	int i;
	
	for(i=0; i<pPoseBoneCount; i++){
		const deInputDevicePose &pose = pPoseBones[i];
		const decVector &p = pose.GetPosition();
		const decVector o(pose.GetOrientation().GetEulerAngles() * RAD2DEG);
		const decVector &lv = pose.GetLinearVelocity();
		const decVector av(pose.GetAngularVelocity() * RAD2DEG);
		oxr.LogInfoFormat( "%sBone %02d: pos=(%.3f,%.3f,%.3f) rot=(% 3f,% 3f,% 3f)"
			" lv=(% 3.1f,% 3.1f,% 3.1f) av=(% 3f,% 3f,% 3f)",
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
	sBoneMapping &m = pMapBoneXrToDe[index];
	m.constOxr = from;
	m.constDe = to;
	m.location = pJointLocations + from;
	m.velocity = pJointVelocities + from;
	m.bone = pPoseBones + to;
}

void deoxrHandTracker::pFixBrokenBoneRotations(){
	int i;
	for(i=0; i<FoxBrokenRotationBoneCount; i++){
		sFixBrokenRotationBone &fbrb = pFixBrokenRotationBone[i];
		const decVector &target = pPoseBones[fbrb.target].GetPosition();
		const decVector &base = pPoseBones[fbrb.base].GetPosition();
		const decVector viewExpected(target - base);
		if(viewExpected.IsZero()){
			continue;
		}
		
		const decVector up(decMatrix::CreateFromQuaternion(
			pPoseBones[fbrb.base].GetOrientation()).TransformUp());
		
		pPoseBones[fbrb.base].SetOrientation(decMatrix::CreateVU(viewExpected, up).ToQuaternion());
	}
}

void deoxrHandTracker::pCalcFingerBending(){
	int i;
	for(i=0; i<FingerBendingCount; i++){
		sFingerBending &fb = pBendFinger[i];
		const decVector view(decMatrix::CreateFromQuaternion(pPoseBones[fb.tip].GetOrientation()
			* pPoseBones[fb.base].GetOrientation().Conjugate()).TransformView());
		const float angle = atan2f(view.y, view.z);
		fb.value = decMath::linearStep(angle, fb.angleLower, fb.angleUpper,
			fb.mapValueLower, fb.mapValueUpper);
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
	for(i=0; i<SpreadFingerCount; i++){
		sFingerSpreading &fs = pSpreadFinger[i];
		const float angle = pPoseBones[fs.bone1].GetOrientation().AngleBetween(
			pPoseBones[fs.bone2].GetOrientation());
		fs.value = decMath::linearStep(angle, fs.angleLower, fs.angleUpper,
			fs.mapValueLower, fs.mapValueUpper);
		//fs.value = 0.0f;
	}
}
