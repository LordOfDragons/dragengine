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
#include "deoxrSpace.h"
#include "deoxrSession.h"
#include "deoxrUtils.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrSpace
//////////////////////

deoxrSpace::deoxrSpace(deoxrSession &session, XrReferenceSpaceType referenceType) :
pSession(session),
pSpace(XR_NULL_HANDLE)
{
	try{
		XrReferenceSpaceCreateInfo ci{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
		ci.referenceSpaceType = referenceType;
		ci.poseInReferenceSpace.orientation.w = 1.0f;
		
		OXR_CHECK(session.GetSystem().GetInstance().xrCreateReferenceSpace(
			session.GetSession(), &ci, &pSpace));
		
		// xrGetReferenceSpaceBoundsRect : get chaperone
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoxrSpace::deoxrSpace(deoxrSession &session, XrReferenceSpaceType referenceType,
	const XrPosef &pose) :
pSession(session),
pSpace(XR_NULL_HANDLE)
{
	try{
		XrReferenceSpaceCreateInfo ci{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
		ci.referenceSpaceType = referenceType;
		ci.poseInReferenceSpace = pose;
		
		OXR_CHECK(session.GetSystem().GetInstance().xrCreateReferenceSpace(
			session.GetSession(), &ci, &pSpace));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoxrSpace::deoxrSpace(deoxrSession &session, XrReferenceSpaceType referenceType,
const decMatrix &transform) :
pSession(session),
pSpace(XR_NULL_HANDLE)
{
	try{
		XrReferenceSpaceCreateInfo ci{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
		ci.referenceSpaceType = referenceType;
		deoxrUtils::Convert(transform, ci.poseInReferenceSpace);
		
		OXR_CHECK(session.GetSystem().GetInstance().xrCreateReferenceSpace(
			session.GetSession(), &ci, &pSpace));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoxrSpace::deoxrSpace(deoxrSession &session, const deoxrAction &action,
	const deoxrPath &subactionPath, const decVector &poseRotation) :
pSession(session),
pSpace(XR_NULL_HANDLE)
{
	try{
		XrActionSpaceCreateInfo ci{XR_TYPE_ACTION_SPACE_CREATE_INFO};
		ci.action = action.GetAction();
		ci.subactionPath = subactionPath;
		deoxrUtils::Convert(poseRotation, ci.poseInActionSpace.orientation);
		
		OXR_CHECK(session.GetSystem().GetInstance().xrCreateActionSpace(
			session.GetSession(), &ci, &pSpace));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoxrSpace::deoxrSpace(deoxrSession &session, const deoxrAction &action) :
pSession(session),
pSpace(XR_NULL_HANDLE)
{
	try{
		XrActionSpaceCreateInfo ci{XR_TYPE_ACTION_SPACE_CREATE_INFO};
		ci.action = action.GetAction();
		ci.poseInActionSpace.orientation.w = 1.0f;
		
		OXR_CHECK(session.GetSystem().GetInstance().xrCreateActionSpace(
			session.GetSession(), &ci, &pSpace));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoxrSpace::deoxrSpace(deoxrSession &session, XrSpace space) :
pSession(session),
pSpace(space)
{
	DEASSERT_FALSE(space == XR_NULL_HANDLE);
}

deoxrSpace::~deoxrSpace(){
	pCleanUp();
}



// Management
///////////////

int deoxrSpace::LocateSpace(const deoxrSpace &space, XrTime time,
decVector &position, decQuaternion &orientation) const{
	const deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	XrSpaceLocation location;
	memset(&location, 0, sizeof(location));
	location.type = XR_TYPE_SPACE_LOCATION;
	
	if(! XR_SUCCEEDED(instance.xrLocateSpace(pSpace, space.pSpace, time, &location))){
		return 0;
	}
	
	int validData = (int)eValidData::space;
	if((location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0){
		position = deoxrUtils::Convert(location.pose.position);
		validData |= (int)eValidData::position;
	}
	
	if((location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0){
		orientation = deoxrUtils::Convert(location.pose.orientation);
		validData |= (int)eValidData::orientation;
	}
	
	return validData;
}

int deoxrSpace::LocateSpace(const deoxrSpace &space, XrTime time, decVector &position,
decQuaternion &orientation, decVector &linearVelocity, decVector &angularVelocity) const{
	const deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	XrSpaceVelocity velocity;
	memset(&velocity, 0, sizeof(velocity));
	velocity.type = XR_TYPE_SPACE_VELOCITY;
	
	XrSpaceLocation location;
	memset(&location, 0, sizeof(location));
	location.type = XR_TYPE_SPACE_LOCATION;
	location.pose.orientation.w = 1.0f;
	location.next = &velocity;
	
	if(! XR_SUCCEEDED(instance.xrLocateSpace(pSpace, space.pSpace, time, &location))){
		return 0;
	}
	
	int validData = (int)eValidData::space;
	if((location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0){
		position = deoxrUtils::Convert(location.pose.position);
		validData |= (int)eValidData::position;
	}
	
	if((location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0){
		orientation = deoxrUtils::Convert(location.pose.orientation);
		validData |= (int)eValidData::orientation;
	}
	
	if((velocity.velocityFlags & XR_SPACE_VELOCITY_LINEAR_VALID_BIT) != 0){
		linearVelocity = deoxrUtils::Convert(velocity.linearVelocity);
		validData |= (int)eValidData::linearVelocity;
	}
	
	if((velocity.velocityFlags & XR_SPACE_VELOCITY_ANGULAR_VALID_BIT) != 0){
		angularVelocity = deoxrUtils::Convert(velocity.angularVelocity);
		validData |= (int)eValidData::angularVelocity;
	}
	
	return validData;
}

int deoxrSpace::LocateSpaceEye(XrTime time, decVector &position, decQuaternion &orientation,
	decVector &linearVelocity, decVector &angularVelocity) const{
	const deoxrInstance &instance = pSession.GetSystem().GetInstance();
	
	XrEyeGazeSampleTimeEXT eyeGazeSampleTime;
	memset(&eyeGazeSampleTime, 0, sizeof(eyeGazeSampleTime));
	eyeGazeSampleTime.type = XR_TYPE_EYE_GAZE_SAMPLE_TIME_EXT;
	
	XrSpaceVelocity velocity;
	memset(&velocity, 0, sizeof(velocity));
	velocity.type = XR_TYPE_SPACE_VELOCITY;
	velocity.next = &eyeGazeSampleTime;
	
	XrSpaceLocation location;
	memset(&location, 0, sizeof(location));
	location.type = XR_TYPE_SPACE_LOCATION;
	location.pose.orientation.w = 1.0f;
	location.next = &velocity;
	
	if(! XR_SUCCEEDED(instance.xrLocateSpace(pSpace, pSession.GetSpaceLocal()->pSpace, time, &location))){
		return 0;
	}
	
	int validData = (int)eValidData::space;
	
	// OpenXR does not specify this explicitely but from the data obtained by a VIVE
	// HMD the coordinate system used is relative to the eye camera not the head.
	// this thus requires a Y-rotation by 180 degrees to allow users to work with
	// coordinates in the same coordinate system as the HMD device uses
	const decQuaternion rotate(decQuaternion::CreateFromEulerY(180.0f * DEG2RAD));
	
	if((location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0){
		// const decVector converted( rotate * deoxrUtils::Convert( location.pose.position ) );

		// while blinking the eyes can not be tracked. VIVE does not clear the valid bit
		// which is incorrect since this causes the eyes to turn inside the head while
		// blinking. in the case of VIVE this situation returns (0,0,0) as position
		// and (0,0,0,-1) as orientation. if this is found we keep the last position
		// and orientation to not mess up the application
		if(location.pose.position.x == 0.0f && location.pose.position.y == 0.0f
		&& location.pose.position.z == 0.0f && location.pose.orientation.x == 0.0f
		&& location.pose.orientation.y == 0.0f && location.pose.orientation.z == 0.0f){
			return 0;
		}

		position = deoxrUtils::Convert(location.pose.position);
		validData |= (int)eValidData::position;
	}
	
	if((location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0){
		orientation = deoxrUtils::Convert(location.pose.orientation) * rotate;
		validData |= (int)eValidData::orientation;
	}
	
	if((velocity.velocityFlags & XR_SPACE_VELOCITY_LINEAR_VALID_BIT) != 0){
		linearVelocity = rotate * deoxrUtils::Convert(velocity.linearVelocity);
		validData |= (int)eValidData::linearVelocity;
	}
	
	if((velocity.velocityFlags & XR_SPACE_VELOCITY_ANGULAR_VALID_BIT) != 0){
		angularVelocity = deoxrUtils::Convert(velocity.angularVelocity);
		angularVelocity.y = -angularVelocity.y;
		angularVelocity.z = 0.0f;
		validData |= (int)eValidData::angularVelocity;
	}
	
	return validData;
}



// Private Functions
//////////////////////

void deoxrSpace::pCleanUp(){
	if(pSpace != XR_NULL_HANDLE){
		pSession.GetSystem().GetInstance().xrDestroySpace(pSpace);
	}
}
