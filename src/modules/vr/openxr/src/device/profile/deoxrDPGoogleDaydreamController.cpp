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

#include "deoxrDPGoogleDaydreamController.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPGoogleDaydreamController
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPGoogleDaydreamController::deoxrDPGoogleDaydreamController( deoxrInstance &instance ) :
deoxrDeviceProfile( instance,
	deoxrPath( instance, "/interaction_profiles/google/daydream_controller" ),
	"Google Daydream Controller" )
{
}

deoxrDPGoogleDaydreamController::~deoxrDPGoogleDaydreamController(){
}



// Management
///////////////

void deoxrDPGoogleDaydreamController::SuggestBindings(){
	// Valid for user paths:
	// - /user/hand/left
	// - /user/hand/right
	// 
	// Supported component paths:
	// - /input/select/click
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/click
	// - /input/trackpad/touch
	// - /input/grip/pose
	// - /input/aim/pose
	
	const int bindingCount = 7 * 2;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePathList[ 2 ] = { "/user/hand/left", "/user/hand/right" };
	int i;
	
	for( i=0; i<2; i++ ){
		const decString &basePath = basePathList[ i ];
		
		pAdd( b, deVROpenXR::eiaPose, basePath + "/input/aim/pose" );
		
		pAdd( b, deVROpenXR::eiaTriggerPress, basePath + "/input/select/click" );
		pAdd( b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/select/click" );
		
		pAdd( b, deVROpenXR::eiaTrackpadAnalog, basePath + "/input/trackpad/x" );
		pAdd( b, deVROpenXR::eiaTrackpadAnalog, basePath + "/input/trackpad/y" );
		pAdd( b, deVROpenXR::eiaTrackpadPress, basePath + "/input/trackpad/click" );
		pAdd( b, deVROpenXR::eiaTrackpadTouch, basePath + "/input/trackpad/touch" );
	}
	
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
}
