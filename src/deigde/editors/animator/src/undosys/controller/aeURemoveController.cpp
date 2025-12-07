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
#include <string.h>
#include <stdlib.h>

#include "aeURemoveController.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/link/aeLink.h"

#include <dragengine/common/exceptions.h>



// Class aeURemoveController
//////////////////////////////

// Constructor, destructor
////////////////////////////

aeURemoveController::aeURemoveController(aeAnimator *animator, aeController *controller) :
pAnimator(NULL),
pController(NULL),
pIndex(-1)
{
	if(!animator || !controller){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Remove controller");
	
	pIndex = animator->GetControllers().IndexOf(controller);
	if(pIndex == -1){
		DETHROW(deeInvalidParam);
	}
	
	const aeLinkList &links = animator->GetLinks();
	const int count = links.GetCount();
	int i;
	for(i=0; i<count; i++){
		aeLink * const link = links.GetAt(i);
		if(link->GetController() == controller){
			pLinksUsingController.Add(link);
		}
	}
	
	pAnimator = animator;
	animator->AddReference();
	
	pController = controller;
	controller->AddReference();
}

aeURemoveController::~aeURemoveController(){
	pCleanUp();
}



// Management
///////////////

void aeURemoveController::Undo(){
	pAnimator->InsertControllerAt(pController, pIndex);
	
	const int count = pLinksUsingController.GetCount();
	int i;
	for(i=0; i<count; i++){
		pLinksUsingController.GetAt(i)->SetController(pController, false);
	}
}

void aeURemoveController::Redo(){
	const int count = pLinksUsingController.GetCount();
	int i;
	for(i=0; i<count; i++){
		pLinksUsingController.GetAt(i)->SetController(NULL, false);
	}
	
	pAnimator->RemoveController(pController);
}



// Private Functions
//////////////////////

void aeURemoveController::pCleanUp(){
}
