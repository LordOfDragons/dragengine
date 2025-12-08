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

#include "deAnimatorInstance.h"
#include "deAnimatorInstanceManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/animator/deBaseAnimatorModule.h"
#include "../../systems/deAnimatorSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// Class deAnimatorInstanceManager
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorInstanceManager::deAnimatorInstanceManager(deEngine *engine) :
deResourceManager(engine, ertAnimatorInstance){
	SetLoggingName("animator instance");
}

deAnimatorInstanceManager::~deAnimatorInstanceManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deAnimatorInstanceManager::GetAnimatorInstanceCount() const{
	return pInstances.GetCount();
}

deAnimatorInstance *deAnimatorInstanceManager::GetRootAnimatorInstance() const{
	return (deAnimatorInstance*)pInstances.GetRoot();
}

deAnimatorInstance::Ref deAnimatorInstanceManager::CreateAnimatorInstance(){
	deAnimatorInstance::Ref instance = NULL;
	
	try{
		instance.TakeOver(new deAnimatorInstance(this));
		if(!instance) DETHROW(deeOutOfMemory);
		
		GetAnimatorSystem()->LoadAnimatorInstance(instance);
		
		pInstances.Add(instance);
		
	}catch(const deException &){
		throw;
	}
	
	return instance;
}



void deAnimatorInstanceManager::ReleaseLeakingResources(){
	const int count = GetAnimatorInstanceCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking animator instances", count);
		pInstances.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deAnimatorInstanceManager::SystemAnimatorLoad(){
	deAnimatorInstance::Ref instance = (deAnimatorInstance*)pInstances.GetRoot();
	deAnimatorSystem &aniSys = *GetAnimatorSystem();
	
	while(instance){
		if(!instance->GetPeerAnimator()){
			aniSys.LoadAnimatorInstance(instance);
		}
		
		instance = (deAnimatorInstance*)instance->GetLLManagerNext();
	}
}

void deAnimatorInstanceManager::SystemAnimatorUnload(){
	deAnimatorInstance::Ref instance = (deAnimatorInstance*)pInstances.GetRoot();
	
	while(instance){
		instance->SetPeerAnimator(NULL);
		instance = (deAnimatorInstance*)instance->GetLLManagerNext();
	}
}



void deAnimatorInstanceManager::RemoveResource(deResource *resource){
	pInstances.RemoveIfPresent(resource);
}
