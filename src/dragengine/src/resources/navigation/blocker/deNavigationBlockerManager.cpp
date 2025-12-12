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

#include "deNavigationBlocker.h"
#include "deNavigationBlockerManager.h"

#include "../../../deEngine.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deAISystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/ai/deBaseAINavigationBlocker.h"
#include "../../../common/exceptions.h"



// Class deNavigationBlockerManager
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationBlockerManager::deNavigationBlockerManager(deEngine *engine) : deResourceManager(engine, ertNavigationBlocker){
	SetLoggingName("navigation blocker");
}

deNavigationBlockerManager::~deNavigationBlockerManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deNavigationBlockerManager::GetNavigationBlockerCount() const{
	return pBlockers.GetCount();
}

deNavigationBlocker *deNavigationBlockerManager::GetRootNavigationBlocker() const{
	return (deNavigationBlocker*)pBlockers.GetRoot();
}

deNavigationBlocker::Ref deNavigationBlockerManager::CreateNavigationBlocker(){
	const deNavigationBlocker::Ref blocker(deNavigationBlocker::Ref::New(this));
	GetAISystem()->LoadNavigationBlocker(blocker);
	pBlockers.Add(blocker);
	return blocker;
}



void deNavigationBlockerManager::ReleaseLeakingResources(){
	const int count = GetNavigationBlockerCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking navigation blockers", count);
		pBlockers.RemoveAll(); // we do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deNavigationBlockerManager::SystemAILoad(){
	deNavigationBlocker *blocker = (deNavigationBlocker*)pBlockers.GetRoot();
	deAISystem &aisys = *GetAISystem();
	
	while(blocker){
		if(!blocker->GetPeerAI()){
			aisys.LoadNavigationBlocker(blocker);
		}
		blocker = (deNavigationBlocker*)blocker->GetLLManagerNext();
	}
}

void deNavigationBlockerManager::SystemAIUnload(){
	deNavigationBlocker *blocker = (deNavigationBlocker*)pBlockers.GetRoot();
	
	while(blocker){
		blocker->SetPeerAI(NULL);
		blocker = (deNavigationBlocker*)blocker->GetLLManagerNext();
	}
}

void deNavigationBlockerManager::RemoveResource(deResource *resource){
	pBlockers.RemoveIfPresent(resource);
}
