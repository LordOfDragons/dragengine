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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deLight.h"
#include "deLightManager.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// class deLightManager
/////////////////////////////

// constructor, destructor
deLightManager::deLightManager(deEngine *engine) : deResourceManager(engine, ertLight){
	SetLoggingName("light");
}
deLightManager::~deLightManager(){
	ReleaseLeakingResources();
}

// management
int deLightManager::GetLightCount() const{
	return pLights.GetCount();
}

deLight *deLightManager::GetRootLight() const{
	return (deLight*)pLights.GetRoot();
}

deLight *deLightManager::CreateLight(){
	deLight *light = NULL;
	// create and add light
	try{
		light = new deLight(this);
		if(!light) DETHROW(deeOutOfMemory);
		GetGraphicSystem()->LoadLight(light);
		pLights.Add(light);
	}catch(const deException &){
		if(light){
			light->FreeReference();
		}
		throw;
	}
	// finished
	return light;
}



void deLightManager::ReleaseLeakingResources(){
	if(GetLightCount() > 0){
		LogWarnFormat("%i leaking lights", GetLightCount());
		pLights.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deLightManager::SystemGraphicLoad(){
	deLight *light = (deLight*)pLights.GetRoot();
	
	while(light){
		if(!light->GetPeerGraphic()){
			GetGraphicSystem()->LoadLight(light);
		}
		
		light = (deLight*)light->GetLLManagerNext();
	}
}

void deLightManager::SystemGraphicUnload(){
	deLight *light = (deLight*)pLights.GetRoot();
	
	while(light){
		light->SetPeerGraphic(NULL);
		light = (deLight*)light->GetLLManagerNext();
	}
}

void deLightManager::RemoveResource(deResource *resource){
	pLights.RemoveIfPresent(resource);
}
