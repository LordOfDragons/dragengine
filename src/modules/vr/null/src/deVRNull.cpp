/* 
 * Drag[en]gine Null VR Module
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

#include <stdio.h>
#include <stdlib.h>

#include "deVRNull.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevicePose.h>
#include <dragengine/systems/deVRSystem.h>


#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *NullVRCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry Function
///////////////////

deBaseModule *NullVRCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deVRNull( *loadableModule );
		
	}catch( const deException & ){
		return nullptr;
	}
}



// Class deVRNull
///////////////////

// Constructor, destructor
////////////////////////////

deVRNull::deVRNull( deLoadableModule &loadableModule ) :
deBaseVRModule( loadableModule ){
}

deVRNull::~deVRNull(){
}



// Management
///////////////

bool deVRNull::Init(){
	return true;
}

void deVRNull::CleanUp(){
}



// Runtime
////////////

bool deVRNull::RuntimeUsable(){
	return false;
}

void deVRNull::StartRuntime(){
	DETHROW_INFO( deeInvalidAction, "VR not supported" );
}

void deVRNull::StopRuntime(){
}

void deVRNull::SetCamera( deCamera* ){
}



// Devices
////////////

int deVRNull::GetDeviceCount(){
	return 0;
}

deInputDevice *deVRNull::GetDeviceAt( int ){
	DETHROW( deeInvalidParam );
}

int deVRNull::IndexOfDeviceWithID( const char* ){
	return -1;
}


int deVRNull::IndexOfButtonWithID( int, const char* ){
	return -1;
}

int deVRNull::IndexOfAxisWithID( int, const char* ){
	return -1;
}

int deVRNull::IndexOfFeedbackWithID( int, const char* ){
	return -1;
}

int deVRNull::IndexOfComponentWithID( int, const char* ){
	return -1;
}

bool deVRNull::GetButtonPressed( int, int ){
	DETHROW( deeInvalidParam );
}

bool deVRNull::GetButtonTouched( int, int ){
	DETHROW( deeInvalidParam );
}

float deVRNull::GetAxisValue( int, int ){
	DETHROW( deeInvalidParam );
}

float deVRNull::GetFeedbackValue( int, int ){
	DETHROW( deeInvalidParam );
}

void deVRNull::SetFeedbackValue( int, int, float ){
}

void deVRNull::GetDevicePose( int, deInputDevicePose &pose ){
	DETHROW( deeInvalidParam );
}

void deVRNull::GetDeviceBonePose( int, int, bool, deInputDevicePose &pose ){
	DETHROW( deeInvalidParam );
}



// Events
///////////

void deVRNull::ProcessEvents(){
}

decPoint deVRNull::GetRenderSize(){
	DETHROW( deeInvalidParam );
}

void deVRNull::GetProjectionParameters( eEye, float&, float&, float&, float& ){
	DETHROW( deeInvalidParam );
}

decMatrix deVRNull::GetMatrixViewEye( eEye ){
	DETHROW( deeInvalidParam );
}

deModel *deVRNull::GetHiddenArea( eEye ){
	return nullptr;
}

deImage *deVRNull::GetDistortionMap( eEye ){
	return nullptr;
}

void deVRNull::StartBeginFrame(){
}

void deVRNull::WaitBeginFrameFinished(){
}

void deVRNull::SubmitOpenGLTexture2D( eEye, void*, const decVector2 &, const decVector2 &, bool ){
}

void deVRNull::EndFrame(){
}
