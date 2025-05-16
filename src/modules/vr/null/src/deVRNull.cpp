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

#include <stdio.h>
#include <stdlib.h>

#include "deVRNull.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevicePose.h>
#include <dragengine/systems/deVRSystem.h>


#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *NullVRCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif
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

bool deVRNull::IsRuntimeRunning(){
	return false;
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

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class denvrModuleInternal : public deInternalModule{
public:
	denvrModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("NullVR");
		SetDescription("No VR Support.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtVR);
		SetDirectoryName("null");
		SetPriority(0);
		SetIsFallback(true);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(NullVRCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *denvrRegisterInternalModule(deModuleSystem *system){
	return new denvrModuleInternal(system);
}
#endif
