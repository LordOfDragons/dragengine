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
#include "deDebugDrawer.h"
#include "deDebugDrawerManager.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// class deDebugDrawerManager
///////////////////////////////

// constructor, destructor
////////////////////////////

deDebugDrawerManager::deDebugDrawerManager(deEngine *engine) : deResourceManager(engine, ertDebugDrawer){
	SetLoggingName("debug drawer");
}

deDebugDrawerManager::~deDebugDrawerManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deDebugDrawerManager::GetDebugDrawerCount() const{
	return pDebugDrawers.GetCount();
}

deDebugDrawer *deDebugDrawerManager::GetRootDebugDrawer() const{
	return (deDebugDrawer*)pDebugDrawers.GetRoot();
}

deDebugDrawer *deDebugDrawerManager::CreateDebugDrawer(){
	deDebugDrawer::Ref debugDrawer = NULL;
	// create and add debug drawer
	try{
		debugDrawer.TakeOver(new deDebugDrawer(this));
		if(!debugDrawer) DETHROW(deeOutOfMemory);
		GetGraphicSystem()->LoadDebugDrawer(debugDrawer);
		pDebugDrawers.Add(debugDrawer);
	}catch(const deException &){
		throw;
	}
	// finished
	return debugDrawer;
}



void deDebugDrawerManager::ReleaseLeakingResources(){
	if(GetDebugDrawerCount() > 0){
		LogWarnFormat("%i leaking debug drawers", GetDebugDrawerCount());
		pDebugDrawers.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deDebugDrawerManager::SystemGraphicLoad(){
	deDebugDrawer::Ref debugDrawer = (deDebugDrawer*)pDebugDrawers.GetRoot();
	
	while(debugDrawer){
		if(!debugDrawer->GetPeerGraphic()){
			GetGraphicSystem()->LoadDebugDrawer(debugDrawer);
		}
		
		debugDrawer = (deDebugDrawer*)debugDrawer->GetLLManagerNext();
	}
}

void deDebugDrawerManager::SystemGraphicUnload(){
	deDebugDrawer::Ref debugDrawer = (deDebugDrawer*)pDebugDrawers.GetRoot();
	
	while(debugDrawer){
		debugDrawer->SetPeerGraphic(NULL);
		debugDrawer = (deDebugDrawer*)debugDrawer->GetLLManagerNext();
	}
}

void deDebugDrawerManager::RemoveResource(deResource *resource){
	pDebugDrawers.RemoveIfPresent(resource);
}
