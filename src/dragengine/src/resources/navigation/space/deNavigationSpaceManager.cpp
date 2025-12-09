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

#include "deNavigationSpace.h"
#include "deNavigationSpaceManager.h"

#include "../../../deEngine.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deAISystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/ai/deBaseAINavigationSpace.h"
#include "../../../common/exceptions.h"



// Class deNavigationSpaceManager
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationSpaceManager::deNavigationSpaceManager(deEngine *engine) : deResourceManager(engine, ertNavigationSpace){
	SetLoggingName("navigation space");
}

deNavigationSpaceManager::~deNavigationSpaceManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deNavigationSpaceManager::GetNavigationSpaceCount() const{
	return pNavSpaces.GetCount();
}

deNavigationSpace *deNavigationSpaceManager::GetRootNavigationSpace() const{
	return (deNavigationSpace*)pNavSpaces.GetRoot();
}

deNavigationSpace::Ref deNavigationSpaceManager::CreateNavigationSpace(){
	const deNavigationSpace::Ref navspace(deNavigationSpace::Ref::NewWith(this));
	GetAISystem()->LoadNavigationSpace(navspace);
	pNavSpaces.Add(navspace);
	return navspace;
}



void deNavigationSpaceManager::ReleaseLeakingResources(){
	const int count = GetNavigationSpaceCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking navigation spaces", count);
		pNavSpaces.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deNavigationSpaceManager::SystemAILoad(){
	deNavigationSpace *navspace = (deNavigationSpace*)pNavSpaces.GetRoot();
	deAISystem &aisys = *GetAISystem();
	
	while(navspace){
		if(!navspace->GetPeerAI()){
			aisys.LoadNavigationSpace(navspace);
		}
		
		navspace = (deNavigationSpace*)navspace->GetLLManagerNext();
	}
}

void deNavigationSpaceManager::SystemAIUnload(){
	deNavigationSpace *navspace = (deNavigationSpace*)pNavSpaces.GetRoot();
	
	while(navspace){
		navspace->SetPeerAI(NULL);
		navspace = (deNavigationSpace*)navspace->GetLLManagerNext();
	}
}

void deNavigationSpaceManager::RemoveResource(deResource *resource){
	pNavSpaces.RemoveIfPresent(resource);
}
