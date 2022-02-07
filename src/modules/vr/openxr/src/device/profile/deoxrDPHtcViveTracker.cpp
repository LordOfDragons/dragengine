/* 
 * Drag[en]gine OpenXR VR Module
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

#include "deoxrDPHtcViveTracker.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPHtcViveTracker
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPHtcViveTracker::deoxrDPHtcViveTracker( deoxrInstance &instance ) :
deoxrDeviceProfile( instance,
	deoxrPath( instance, "/interaction_profiles/htc/vive_tracker_htcx" ),
	"HTC VIVE Tracker" )
{
}

deoxrDPHtcViveTracker::~deoxrDPHtcViveTracker(){
}



// Management
///////////////

void deoxrDPHtcViveTracker::SuggestBindings(){
	if( ! GetInstance().SupportsExtension( deoxrInstance::extHTCXViveTrackerInteraction ) ){
		return;
	}
	
	// Valid for top level user path:
	// - VIVE tracker persistent path (unspecified format, enumerate)
	// - /user/vive_tracker_htcx/role/<role-type> (find by assigned role)
	//   - XR_NULL_PATH
	//   - handheld_object
	//   - left_foot
	//   - right_foot
	//   - left_shoulder
	//   - right_shoulder
	//   - left_elbow
	//   - right_elbow
	//   - left_knee
	//   - right_knee
	//   - waist
	//   - chest
	//   - camera
	//   - keyboard
	// 
	// Supported component paths:
	// - /input/system/click (may not be available for application use)
	// - /input/menu/click
	// - /input/trigger/click
	// - /input/squeeze/click
	// - /input/trigger/value
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/click
	// - /input/trackpad/touch
	// - /input/grip/pose
	// - /output/haptic
	// 
	// Enumeration support:
	// typedef struct XrViveTrackerPathsHTCX {
	//    XrStructureType    type;
	//    void*              next;
	//    XrPath             persistentPath;
	//    XrPath             rolePath;
	// } XrViveTrackerPathsHTCX;
	// 
	// xrEnumerateViveTrackerPathsHTCX(
	//    XrInstance                                  instance,
	//    uint32_t                                    pathCapacityInput, /* use 0 to get required count in pathCountOutput */
	//    uint32_t*                                   pathCountOutput,
	//    XrViveTrackerPathsHTCX*                     paths);
	
	/*
	const int bindingCount = 5 * 2;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[ 2 ] = { "/user/hand/left", "/user/hand/right" };
	int i;
	
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, deVROpenXR::eiaPose, basePath + "/input/aim/pose" );
		
		pAdd( b, deVROpenXR::eiaTriggerPress, basePath + "/input/select/click" );
		pAdd( b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/select/click" );
		
		pAdd( b, deVROpenXR::eiaButtonPrimaryPress, basePath + "/input/menu/click" );
		
		pAdd( b, deVROpenXR::eiaGripHaptic, basePath + "/output/haptic" );
	}
	
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
	*/
}
