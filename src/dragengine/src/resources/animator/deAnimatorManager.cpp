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
#include "deAnimator.h"
#include "deAnimatorManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/animator/deBaseAnimatorModule.h"
#include "../../systems/deAnimatorSystem.h"
#include "../../deEngine.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../common/exceptions.h"


// class deAnimatorManager
////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorManager::deAnimatorManager(deEngine *engine) : deResourceManager(engine,ertAnimator){
	SetLoggingName("animator");
}

deAnimatorManager::~deAnimatorManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deAnimatorManager::GetAnimatorCount() const{
	return pAnimators.GetCount();
}

deAnimator *deAnimatorManager::GetRootAnimator() const{
	return (deAnimator*)pAnimators.GetRoot();
}

deAnimator *deAnimatorManager::CreateAnimator(){
	deAnimator::Ref animator = NULL;
	// load skin
	try{
		// create and add animator
		animator.TakeOver(new deAnimator(this));
		if(!animator) DETHROW(deeOutOfMemory);
		GetAnimatorSystem()->LoadAnimator(animator);
		// add animator
		pAnimators.Add(animator);
	}catch(const deException &){
		throw;
	}
	// finished
	return animator;
}



void deAnimatorManager::ReleaseLeakingResources(){
	int count = GetAnimatorCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking animators", count);
		pAnimators.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deAnimatorManager::SystemAnimatorLoad(){
	deAnimator::Ref animator = (deAnimator*)pAnimators.GetRoot();
	deAnimatorSystem &aniSys = *GetAnimatorSystem();
	
	while(animator){
		if(!animator->GetPeerAnimator()){
			aniSys.LoadAnimator(animator);
		}
		
		animator = (deAnimator*)animator->GetLLManagerNext();
	}
}

void deAnimatorManager::SystemAnimatorUnload(){
	deAnimator::Ref animator = (deAnimator*)pAnimators.GetRoot();
	
	while(animator){
		animator->SetPeerAnimator(NULL);
		animator = (deAnimator*)animator->GetLLManagerNext();
	}
}

void deAnimatorManager::RemoveResource(deResource *resource){
	pAnimators.RemoveIfPresent(resource);
}
