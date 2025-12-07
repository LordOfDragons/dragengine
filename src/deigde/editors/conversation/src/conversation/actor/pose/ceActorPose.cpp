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

#include "ceActorPose.h"
#include "../controller/ceActorController.h"
#include "../gesture/ceActorGesture.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/loadsave/igdeLoadAnimator.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class ceActorPose
//////////////////////

// Constructor, destructor
////////////////////////////

ceActorPose::ceActorPose(igdeEnvironment &environment, const char *name) :
pEnvironment(environment),
pEngAnimator(NULL),
pName(name){
}

ceActorPose::ceActorPose(const ceActorPose &pose) :
pEnvironment(pose.pEnvironment),
pEngAnimator(NULL),
pName(pose.pName),
pPathAnimator(pose.pPathAnimator),
pControllerNames(pose.pControllerNames)
{
	// clone gestures
	const int gestureCount = pose.pGestures.GetCount();
	int i;
	
	for(i=0; i<gestureCount; i++){
		ceActorGesture *gesture = NULL;
		
		try{
			gesture = new ceActorGesture(*pose.pGestures.GetAt(i));
			pGestures.Add(gesture);
			gesture->FreeReference();
			
		}catch(const deException &){
			if(gesture){
				gesture->FreeReference();
			}
			throw;
		}
	}
	
	// take over animator
	pEngAnimator = pose.pEngAnimator;
	if(pEngAnimator){
		pEngAnimator->AddReference();
	}
	
	// clone controllers
	const int controllerCount = pose.pControllers.GetCount();
	for(i=0; i<controllerCount; i++){
		ceActorController *controller = NULL;
		
		try{
			controller = new ceActorController(*pose.pControllers.GetAt(i));
			pControllers.Add(controller);
			controller->FreeReference();
			
		}catch(const deException &){
			if(controller){
				controller->FreeReference();
			}
			throw;
		}
	}
}

ceActorPose::~ceActorPose(){
	if(pEngAnimator){
		pEngAnimator->SetRig(NULL);
		pEngAnimator->FreeReference();
	}
}



// Management
///////////////

void ceActorPose::SetName(const char *name){
	pName = name;
}

void ceActorPose::SetPathAnimator(const char *path){
	if(pPathAnimator.Equals(path)){
		return;
	}
	
	pPathAnimator = path;
	pLoadAnimator();
}



// Private Functions
//////////////////////

void ceActorPose::pLoadAnimator(){
	pControllerNames.RemoveAll();
	
	if(pPathAnimator.IsEmpty()){
		return;
	}
	
	// load animator
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	decBaseFileReader::Ref reader;
	deAnimator *animator = NULL;
	
	try{
		reader.TakeOver(engine.GetVirtualFileSystem()->OpenFileForReading(
			decPath::CreatePathUnix(pPathAnimator)));
		animator = engine.GetAnimatorManager()->CreateAnimator();
		
		igdeLoadAnimator(pEnvironment, pEnvironment.GetLogger(), LOGSOURCE).
			Load(pPathAnimator, *animator, reader);
		
		if(pEngAnimator){
			pEngAnimator->FreeReference();
		}
		pEngAnimator = animator;
		
	}catch(const deException &e){
		if(animator){
			animator->FreeReference();
		}
		pEnvironment.GetLogger()->LogException(LOGSOURCE, e);
		
		// ignore missing or broken animators. this can easily happen during development
		return;
	}
	
	// update controller name list
	const int count = animator->GetControllerCount();
	int i;
	
	for(i=0; i<count; i++){
		pControllerNames.Add(animator->GetControllerAt(i)->GetName());
	}
}
