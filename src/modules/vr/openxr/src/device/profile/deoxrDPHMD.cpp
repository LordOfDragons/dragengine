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

#include "deoxrDPHMD.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPHMD
/////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPHMD::deoxrDPHMD( deoxrInstance &instance ) :
deoxrDeviceProfile( instance, deoxrPath( instance, "/interaction_profiles/dragengine/hmd" ), "HMD" ){
}

deoxrDPHMD::~deoxrDPHMD(){
	pRemoveDevice();
}



// Management
///////////////

void deoxrDPHMD::CheckAttached(){
	deoxrSession * const session = GetInstance().GetOxr().GetSession();
	
	if( session ){
		pAddDevice();
		
	}else{
		pRemoveDevice();
	}
}

void deoxrDPHMD::SuggestBindings(){
}



// Protected
//////////////

void deoxrDPHMD::pAddDevice(){
	if( pDevice ){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	pDevice.TakeOver( new deoxrDevice( oxr, *this ) );
	pDevice->SetType( deInputDevice::edtVRHMD );
	pDevice->SetName( "HMD" );
	
	decString id;
	id.Format( "%shmd", OXR_DEVID_PREFIX );
	pDevice->SetID( id );
	
	oxr.GetDevices().Add( pDevice );
}

void deoxrDPHMD::pRemoveDevice(){
	if( ! pDevice ){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	oxr.GetDevices().Remove( pDevice );
	pDevice = nullptr;
}
