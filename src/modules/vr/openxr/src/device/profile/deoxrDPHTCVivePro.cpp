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

#include "deoxrDPHTCVivePro.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPHTCVivePro
////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPHTCVivePro::deoxrDPHTCVivePro( deoxrInstance &instance ) :
deoxrDeviceProfile( instance,
	deoxrPath( instance, "/interaction_profiles/htc/vive_pro" ),
	"HTC VIVE Pro HMD" )
{
}

deoxrDPHTCVivePro::~deoxrDPHTCVivePro(){
}



// Management
///////////////

void deoxrDPHTCVivePro::SuggestBindings(){
	// Valid for user paths:
	// - /user/head
	// 
	// Supported component paths:
	// - /input/system/click (may not be available for application use)
	// - /input/volume_up/click
	// - /input/volume_down/click
	// - /input/mute_mic/click
	
	const int bindingCount = 1;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	
	const decString basePath( "/user/head" );
	
	pAdd( b, deVROpenXR::eiaButtonPrimaryPress, basePath + "/input/system/click" );
	
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
}
