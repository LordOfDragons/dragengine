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

#include "deNavigator.h"
#include "deNavigatorManager.h"

#include "../../../deEngine.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deAISystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/ai/deBaseAINavigator.h"
#include "../../../common/exceptions.h"



// Class deNavigatorManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigatorManager::deNavigatorManager(deEngine *engine) : deResourceManager(engine, ertNavigator){
	SetLoggingName("navigator");
}

deNavigatorManager::~deNavigatorManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deNavigatorManager::GetNavigatorCount() const{
	return pNavigators.GetCount();
}

deNavigator *deNavigatorManager::GetRootNavigator() const{
	return (deNavigator*)pNavigators.GetRoot();
}

deNavigator *deNavigatorManager::CreateNavigator(){
	deNavigator *navigator = NULL;
	
	try{
		navigator = new deNavigator(this);
		if(!navigator){
			DETHROW(deeOutOfMemory);
		}
		
		GetAISystem()->LoadNavigator(navigator);
		
		pNavigators.Add(navigator);
		
	}catch(const deException &){
		if(navigator){
			navigator->FreeReference();
		}
		throw;
	}
	
	return navigator;
}



void deNavigatorManager::ReleaseLeakingResources(){
	const int count = GetNavigatorCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking navigators", count);
		pNavigators.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deNavigatorManager::SystemAILoad(){
	deNavigator *navigator = (deNavigator*)pNavigators.GetRoot();
	deAISystem &aisys = *GetAISystem();
	
	while(navigator){
		if(!navigator->GetPeerAI()){
			aisys.LoadNavigator(navigator);
		}
		
		navigator = (deNavigator*)navigator->GetLLManagerNext();
	}
}

void deNavigatorManager::SystemAIUnload(){
	deNavigator *navigator = (deNavigator*)pNavigators.GetRoot();
	
	while(navigator){
		navigator->SetPeerAI(NULL);
		navigator = (deNavigator*)navigator->GetLLManagerNext();
	}
}

void deNavigatorManager::RemoveResource(deResource *resource){
	pNavigators.RemoveIfPresent(resource);
}
