/* 
 * Drag[en]gine OpenXR VR Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deVROpenXR.h"
#include "deoxrLoader.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicCamera.h>
#include <dragengine/threading/deMutexGuard.h>


#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *OpenXRCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry Function
///////////////////

deBaseModule *OpenXRCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deVROpenXR( *loadableModule );
		
	}catch( const deException & ){
		return nullptr;
	}
}



// Class deVROpenXR
/////////////////////

// Constructor, destructor
////////////////////////////

deVROpenXR::deVROpenXR( deLoadableModule &loadableModule ) :
deBaseVRModule( loadableModule ),
pLoader( nullptr )
{
}

deVROpenXR::~deVROpenXR(){
	CleanUp();
}



// Management
///////////////



// Module Management
//////////////////////

bool deVROpenXR::Init(){
	const bool enableDebug = true;
	
	try{
		pLoader = new deoxrLoader( *this );
		pInstance.TakeOver( new deoxrInstance( *this, enableDebug ) );
		
	}catch( const deException &e ){
		LogException( e );
		pInstance = nullptr;
		if( pLoader ){
			delete pLoader;
			pLoader = nullptr;
		}
	}
	
	LogInfoFormat( "Runtime Installed: %s", pInstance ? "Yes" : "No" );
	
	if( pLoader ){
		LogInfoFormat( "Runtime Config File: %s", pLoader->GetRuntimeConfigFile().GetString() );
		LogInfoFormat( "Runtime Library: %s", pLoader->GetRuntimeLibraryPath().GetString() );
	}
	
// 	LogInfoFormat( "HMD Present: %s", vr::VR_IsHmdPresent() ? "Yes" : "No" );
	return true;
}

void deVROpenXR::CleanUp(){
	StopRuntime();
	SetCamera( nullptr );
	
	pInstance = nullptr;
	if( pLoader ){
		delete pLoader;
		pLoader = nullptr;
	}
}



// Runtime
////////////

bool deVROpenXR::RuntimeUsable(){
	return pInstance;
}

void deVROpenXR::StartRuntime(){
	if( ! pInstance ){
		DETHROW_INFO( deeInvalidAction, "runtime not found" );
	}
	
	LogInfo( "Start Runtime" );
	
	try{
		
	}catch( const deException &e ){
		LogException( e );
		StopRuntime();
		throw;
	}
}

void deVROpenXR::StopRuntime(){
	LogInfo( "Shutdown runtime" );
}

void deVROpenXR::SetCamera( deCamera *camera ){
	if( pCamera == camera ){
		return;
	}
	
	if( pCamera && pCamera->GetPeerGraphic() ){
		pCamera->GetPeerGraphic()->VRResignedFromHMD();
	}
	
	pCamera = camera;
	
	if( camera && camera->GetPeerGraphic() ){
		camera->GetPeerGraphic()->VRAssignedToHMD();
	}
}



// Devices
////////////

int deVROpenXR::GetDeviceCount(){
	return 0;
}

deInputDevice *deVROpenXR::GetDeviceAt( int index ){
	DETHROW( deeInvalidParam );
}

int deVROpenXR::IndexOfDeviceWithID( const char *id ){
	return -1;
}


int deVROpenXR::IndexOfButtonWithID( int device, const char *id ){
	return -1;
}

int deVROpenXR::IndexOfAxisWithID( int device, const char *id ){
	return -1;
}

int deVROpenXR::IndexOfFeedbackWithID( int device, const char *id ){
	return -1;
}

int deVROpenXR::IndexOfComponentWithID( int device, const char *id ){
	return -1;
}

bool deVROpenXR::GetButtonPressed( int device, int button ){
	DETHROW( deeInvalidParam );
}

bool deVROpenXR::GetButtonTouched( int device, int button ){
	DETHROW( deeInvalidParam );
}

float deVROpenXR::GetAxisValue( int device, int axis ){
	DETHROW( deeInvalidParam );
}

float deVROpenXR::GetFeedbackValue( int device, int feedback ){
	DETHROW( deeInvalidParam );
}

void deVROpenXR::SetFeedbackValue( int device, int feedback, float value ){
	DETHROW( deeInvalidParam );
}

void deVROpenXR::GetDevicePose( int device, deInputDevicePose &pose ){
	DETHROW( deeInvalidParam );
}

void deVROpenXR::GetDeviceBonePose( int device, int bone, bool withController, deInputDevicePose &pose ){
	DETHROW( deeInvalidParam );
}



// Events
///////////

void deVROpenXR::ProcessEvents(){
}



// Graphic Module use only
////////////////////////////

decPoint deVROpenXR::GetRenderSize(){
	uint32_t width, height;
	width = 1024; height = 1024;
	return decPoint( ( int )width, ( int )height );
}

void deVROpenXR::GetProjectionParameters( eEye eye, float &left, float &right, float &top, float &bottom ){
	left = -1; right = 1; top = -1; bottom = 1;
}

decMatrix deVROpenXR::GetMatrixViewEye( eEye eye ){
	return decMatrix();
}

deModel *deVROpenXR::GetHiddenArea( eEye eye ){
	return nullptr;
}

deImage *deVROpenXR::GetDistortionMap( eEye eye ){
	return nullptr;
}

void deVROpenXR::BeginFrame(){
}

void deVROpenXR::SubmitOpenGLTexture2D( eEye eye, void *texture, const decVector2 &tcFrom,
const decVector2 &tcTo, bool distortionApplied ){
}

void deVROpenXR::EndFrame(){
}
