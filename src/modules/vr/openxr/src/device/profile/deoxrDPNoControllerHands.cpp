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

#include "deoxrDPNoControllerHands.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDPNoControllerHands
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPNoControllerHands::deoxrDPNoControllerHands( deoxrInstance &instance ) :
deoxrDeviceProfile( instance, deoxrPath(), "No Controller Hands" )
{
}

deoxrDPNoControllerHands::~deoxrDPNoControllerHands(){
	pRemoveDevice( true );
	pRemoveDevice( false );
}



// Management
///////////////

void deoxrDPNoControllerHands::CheckAttached(){
	bool useHandsLeft = false;
	bool useHandsRight = false;
	
	if( GetInstance().SupportsExtension( deoxrInstance::extEXTHandTracking )
	&& GetInstance().GetOxr().GetSystem()->GetSupportsHandTracking() ){
		// use hands if no device has been added covering hands
		useHandsLeft = true;
		useHandsRight = true;
		
		const deoxrDeviceManager &devices = GetInstance().GetOxr().GetDevices();
		const int count = devices.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			deoxrDevice * const device = devices.GetAt( i );
			if( pDeviceLeft == device || pDeviceRight == device ){
				continue;
			}
			
			switch( device->GetType() ){
			case deInputDevice::edtVRLeftHand:
				useHandsLeft = false;
				break;
				
			case deInputDevice::edtVRRightHand:
				useHandsRight = false;
				break;
				
			default:
				break;
			}
		}
	}
	
	if( useHandsLeft ){
		pAddDevice( true );
		
	}else{
		pRemoveDevice( true );
	}
	
	if( useHandsRight ){
		pAddDevice( false );
		
	}else{
		pRemoveDevice( false );
	}
}

void deoxrDPNoControllerHands::SuggestBindings(){
}

void deoxrDPNoControllerHands::ClearActions(){
	pRemoveDevice( true );
	pRemoveDevice( false );
}



// Private Functions
//////////////////////

void deoxrDPNoControllerHands::pAddDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( device ){
		return;
	}
	
	device.TakeOver( new deoxrDevice( GetInstance().GetOxr(), *this ) );
	
	decString id;
	
	if( left ){
		device->SetType( deInputDevice::edtVRLeftHand );
		device->SetName( "Left Hand" );
		id.Format( "%snch_l", OXR_DEVID_PREFIX );
		device->SetSubactionPath( GetInstance().GetPathHandLeft() );
		device->SetMatrixWristToDevice( decMatrix::CreateTranslation( 0.03f, 0.06f, 0.14f ) );
		
	}else{
		device->SetType( deInputDevice::edtVRRightHand );
		device->SetName( "Right Hand" );
		id.Format( "%snch_r", OXR_DEVID_PREFIX );
		device->SetSubactionPath( GetInstance().GetPathHandRight() );
		device->SetMatrixWristToDevice( decMatrix::CreateTranslation( -0.03f, 0.06f, 0.14f ) );
	}
	
	device->SetID( id );
	
	pAddHandTracker( device, left );
	
	GetInstance().GetOxr().GetDevices().Add( device );
}

void deoxrDPNoControllerHands::pRemoveDevice( bool left ){
	deoxrDevice::Ref &device = left ? pDeviceLeft : pDeviceRight;
	if( ! device ){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	oxr.GetDevices().Remove( device );
	device = nullptr;
}
