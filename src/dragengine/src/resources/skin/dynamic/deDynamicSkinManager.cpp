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

#include "deDynamicSkin.h"
#include "deDynamicSkinManager.h"

#include "../../../deEngine.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deGraphicSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/graphic/deBaseGraphicDynamicSkin.h"
#include "../../../common/exceptions.h"



// Class deDynamicSkinManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deDynamicSkinManager::deDynamicSkinManager(deEngine *engine) : deResourceManager(engine, ertDynamicSkin){
	SetLoggingName("dynamic skin");
}

deDynamicSkinManager::~deDynamicSkinManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deDynamicSkinManager::GetDynamicSkinCount() const{
	return pSkins.GetCount();
}

deDynamicSkin *deDynamicSkinManager::GetRootDynamicSkin() const{
	return (deDynamicSkin*)pSkins.GetRoot();
}

deDynamicSkin::Ref deDynamicSkinManager::CreateDynamicSkin(){
	deDynamicSkin::Ref dynamicSkin = NULL;
	
	try{
		dynamicSkin.TakeOver(new deDynamicSkin(this));
		if(!dynamicSkin) DETHROW(deeOutOfMemory);
		
		GetGraphicSystem()->LoadDynamicSkin(dynamicSkin);
		
		pSkins.Add(dynamicSkin);
		
	}catch(const deException &){
		throw;
	}
	
	return dynamicSkin;
}



void deDynamicSkinManager::ReleaseLeakingResources(){
	int count = GetDynamicSkinCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking dynamic skins", count);
		pSkins.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deDynamicSkinManager::SystemGraphicLoad(){
	deDynamicSkin::Ref dynamicSkin = (deDynamicSkin*)pSkins.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while(dynamicSkin){
		if(!dynamicSkin->GetPeerGraphic()){
			grasys.LoadDynamicSkin(dynamicSkin);
		}
		
		dynamicSkin = (deDynamicSkin*)dynamicSkin->GetLLManagerNext();
	}
}

void deDynamicSkinManager::SystemGraphicUnload(){
	deDynamicSkin::Ref dynamicSkin = (deDynamicSkin*)pSkins.GetRoot();
	
	while(dynamicSkin){
		dynamicSkin->SetPeerGraphic(NULL);
		dynamicSkin = (deDynamicSkin*)dynamicSkin->GetLLManagerNext();
	}
}

void deDynamicSkinManager::RemoveResource(deResource *resource){
	pSkins.RemoveIfPresent(resource);
}
