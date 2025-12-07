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
#include <string.h>
#include "deCamera.h"
#include "deCameraManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/deGraphicSystem.h"



// Class deCameraManager
//////////////////////////

// Constructor, destructor
////////////////////////////

deCameraManager::deCameraManager(deEngine *engine) : deResourceManager(engine, ertCamera){
	SetLoggingName("camera");
}

deCameraManager::~deCameraManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deCameraManager::GetCameraCount() const{
	return pCameras.GetCount();
}

deCamera *deCameraManager::GetRootCamera() const{
	return (deCamera*)pCameras.GetRoot();
}

deCamera *deCameraManager::CreateCamera(){
	deCamera *camera = NULL;
	
	// create and add camera
	try{
		camera = new deCamera(this);
		if(! camera) DETHROW(deeOutOfMemory);
		GetGraphicSystem()->LoadCamera(camera);
		pCameras.Add(camera);
		
	}catch(const deException &){
		if(camera){
			camera->FreeReference();
		}
		throw;
	}
	
	// finished
	return camera;
}



void deCameraManager::ReleaseLeakingResources(){
	if(GetCameraCount() > 0){
		LogWarnFormat("%i leaking cameras", GetCameraCount());
		pCameras.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////

void deCameraManager::SystemGraphicLoad(){
	deCamera *camera = (deCamera*)pCameras.GetRoot();
	
	while(camera){
		if(! camera->GetPeerGraphic()){
			GetGraphicSystem()->LoadCamera(camera);
		}
		
		camera = (deCamera*)camera->GetLLManagerNext();
	}
}

void deCameraManager::SystemGraphicUnload(){
	deCamera *camera = (deCamera*)pCameras.GetRoot();
	
	while(camera){
		camera->SetPeerGraphic(NULL);
		camera = (deCamera*)camera->GetLLManagerNext();
	}
}

void deCameraManager::RemoveResource(deResource *resource){
	pCameras.RemoveIfPresent(resource);
}
