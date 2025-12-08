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
#include "deSky.h"
#include "deSkyManager.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// class deSkyManager
/////////////////////////////

// constructor, destructor
deSkyManager::deSkyManager(deEngine *engine) : deResourceManager(engine, ertSky){
	SetLoggingName("sky");
}
deSkyManager::~deSkyManager(){
	ReleaseLeakingResources();
}

// management
int deSkyManager::GetSkyCount() const{
	return pSkies.GetCount();
}

deSky *deSkyManager::GetRootSky() const{
	return (deSky*)pSkies.GetRoot();
}

deSky *deSkyManager::CreateSky(){
	deSky::Ref sky = NULL;
	// create and add sky
	try{
		sky.TakeOver(new deSky(this));
		if(!sky) DETHROW(deeOutOfMemory);
		GetGraphicSystem()->LoadSky(sky);
		pSkies.Add(sky);
	}catch(const deException &){
		throw;
	}
	// finished
	return sky;
}



void deSkyManager::ReleaseLeakingResources(){
	if(GetSkyCount() > 0){
		LogWarnFormat("%i leaking skies", GetSkyCount());
		pSkies.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSkyManager::SystemGraphicLoad(){
	deSky::Ref sky = (deSky*)pSkies.GetRoot();
	
	while(sky){
		if(!sky->GetPeerGraphic()){
			GetGraphicSystem()->LoadSky(sky);
		}
		sky = (deSky*)sky->GetLLManagerNext();
	}
}

void deSkyManager::SystemGraphicUnload(){
	deSky::Ref sky = (deSky*)pSkies.GetRoot();
	
	while(sky){
		sky->SetPeerGraphic(NULL);
		sky = (deSky*)sky->GetLLManagerNext();
	}
}

void deSkyManager::RemoveResource(deResource *resource){
	pSkies.RemoveIfPresent(resource);
}
