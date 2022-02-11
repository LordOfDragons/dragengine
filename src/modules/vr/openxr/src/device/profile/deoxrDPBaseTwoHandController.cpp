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

#include "deoxrDPBaseTwoHandController.h"
#include "../deoxrDeviceAxis.h"
#include "../deoxrDeviceButton.h"
#include "../deoxrDeviceComponent.h"
#include "../deoxrDeviceFeedback.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPBaseTwoHandController
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPBaseTwoHandController::deoxrDPBaseTwoHandController(
	deoxrInstance &instance, const deoxrPath &path, const char *name ) :
deoxrDeviceProfile( instance, path, name ){
}

deoxrDPBaseTwoHandController::~deoxrDPBaseTwoHandController(){
	pRemoveDevice( true );
	pRemoveDevice( false );
}



// Management
///////////////

void deoxrDPBaseTwoHandController::CheckAttached(){
	const bool enabled = pProfileEnabled();
	
	if( enabled && pMatchesProfile( GetInstance().GetPathHandLeft() ) ){
		pAddDevice( true );
		
	}else{
		pRemoveDevice( true );
	}
	
	if( enabled && pMatchesProfile( GetInstance().GetPathHandRight() ) ){
		pAddDevice( false );
		
	}else{
		pRemoveDevice( false );
	}
}

void deoxrDPBaseTwoHandController::SuggestBindings(){
	if( pProfileEnabled() ){
		pSuggestBindings();
	}
}

void deoxrDPBaseTwoHandController::ClearActions(){
	pRemoveDevice( true );
	pRemoveDevice( false );
}



// Private Functions
//////////////////////

bool deoxrDPBaseTwoHandController::pProfileEnabled() const{
	return true;
}

void deoxrDPBaseTwoHandController::pRemoveDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( ! device ){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	oxr.GetDevices().Remove( device );
	device = nullptr;
}
