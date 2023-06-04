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
#include <string.h>

#include "deoxrDPEyeGazeInteraction.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class deoxrDPEyeGazeInteraction
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPEyeGazeInteraction::deoxrDPEyeGazeInteraction( deoxrInstance &instance ) :
deoxrDeviceProfile( instance,
	deoxrPath( instance, "/interaction_profiles/ext/eye_gaze_interaction" ),
	"Eye Gaze Interaction" ){
}

deoxrDPEyeGazeInteraction::~deoxrDPEyeGazeInteraction(){
}



// Management
///////////////

void deoxrDPEyeGazeInteraction::CheckAttached(){
	if( GetInstance().SupportsExtension( deoxrInstance::extEXTEyeGazeInteraction ) ){
		pAddDevice();
		
	}else{
		pRemoveDevice();
	}
}

void deoxrDPEyeGazeInteraction::SuggestBindings(){
	if( ! GetInstance().SupportsExtension( deoxrInstance::extEXTEyeGazeInteraction ) ){
		return;
	}
	
	// Valid for user paths:
	// - /user/eyes_ext
	// 
	// Supported component paths:
	// - /input/gaze_ext/pose
	
	const int bindingCount = 1;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	pAdd( b, deVROpenXR::eiaPose, "/user/eyes_ext/input/gaze_ext/pose" );
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
}

void deoxrDPEyeGazeInteraction::ClearActions(){
	pRemoveDevice();
}



// Private Functions
//////////////////////

void deoxrDPEyeGazeInteraction::pAddDevice() {
	if( pDevice ){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	pDevice.TakeOver( new deoxrDevice( oxr, *this ) );
	
	decString id;
	
	pDevice->SetType( deInputDevice::edtVREyeTracker );
	pDevice->SetName( "Eye Tracker" );
	id.Format( "%segi", OXR_DEVID_PREFIX );
	
	pDevice->SetActionPose( oxr.GetAction( deVROpenXR::eiaPose ) );
	pDevice->SetID( id );
	
	pDevice->SetSpacePose( deoxrSpace::Ref::New( new deoxrSpace( *pGetSession(),
		pDevice->GetActionPose(), pDevice->GetSubactionPath(), GetDeviceRotation() ) ) );
	
	GetInstance().GetOxr().GetDevices().Add( pDevice );
}

void deoxrDPEyeGazeInteraction::pRemoveDevice() {
	if( ! pDevice ){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	oxr.GetDevices().Remove( pDevice );
	pDevice = nullptr;
}
