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

#include "deoxrDPSamsungOdysseyController.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>


// Class deoxrDPSamsungOdysseyController
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPSamsungOdysseyController::deoxrDPSamsungOdysseyController( deoxrInstance &instance ) :
deoxrDPMicrosoftMixedRealityMotionController( instance,
	deoxrPath( instance, "/interaction_profiles/oculus/touch_controller" ),
	"Samsung Odyssey Controller" )
{
}

deoxrDPSamsungOdysseyController::~deoxrDPSamsungOdysseyController(){
}


// Protected Functions
////////////////////////

bool deoxrDPSamsungOdysseyController::pProfileEnabled() const{
	return GetInstance().SupportsExtension( deoxrInstance::extEXTSamsungOdysseyController );
}

const char *deoxrDPSamsungOdysseyController::pDeviceIdPrefix() const{
	return "soc_";
}
