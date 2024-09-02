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
	const deoxrInstance &instance = GetInstance();
	bool useHandsLeft = false;
	bool useHandsRight = false;

	if( instance.SupportsExtension( deoxrInstance::extEXTHandTracking )
	&& instance.GetOxr().GetSystem()->GetSupportsHandTracking() ){
		// use hands if no device has been added covering hands
		bool hasLeft = false, hasRight = false;
		pHasHandDevices( pDeviceLeft, pDeviceRight, hasLeft, hasRight );

		useHandsLeft = not hasLeft;
		useHandsRight = not hasRight;
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

void deoxrDPNoControllerHands::RemoveDevice( deInputDevice::eDeviceTypes type ){
	if( type == deInputDevice::edtVRLeftHand && pDeviceLeft ){
		pRemoveDevice( true );
	}
	if( type == deInputDevice::edtVRRightHand && pDeviceRight ){
		pRemoveDevice( false );
	}
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
