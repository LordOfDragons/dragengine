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

#include "aeULinkPaste.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/link/aeLink.h"

#include <dragengine/common/exceptions.h>



// Class aeULinkPaste
///////////////////////

// Constructor, destructor
////////////////////////////

aeULinkPaste::aeULinkPaste(aeAnimator *animator, const aeLinkList &linkList) :
pAnimator(animator)
{
	DEASSERT_NOTNULL(animator)
	const int linkCount = linkList.GetCount();
	DEASSERT_TRUE(linkCount > 0)
	
	int i;
	for(i=0; i<linkCount; i++){
		pLinkList.Add(aeLink::Ref::NewWith(*linkList.GetAt(i)));
	}
}

aeULinkPaste::~aeULinkPaste(){
}



// Management
///////////////

void aeULinkPaste::Undo(){
	const int linkCount = pLinkList.GetCount();
	int i;
	
	// remove the links
	for(i=0; i<linkCount; i++){
		pAnimator->RemoveLink(pLinkList.GetAt(i));
	}
	
	// remove controller added in a prior redo
	const int controllerCount = pRemoveControllerList.GetCount();
	for(i=0; i<controllerCount; i++){
		pAnimator->RemoveController(pRemoveControllerList.GetAt(i));
	}
	pRemoveControllerList.RemoveAll();
}

void aeULinkPaste::Redo(){
	const aeControllerList &controllers = pAnimator->GetControllers();
	const int linkCount = pLinkList.GetCount();
	int i;
	
	pRemoveControllerList.RemoveAll();
	
	for(i=0; i<linkCount; i++){
		aeLink * const link = pLinkList.GetAt(i);
		
		// check if controllers exist in the animator. if not add them and mark them to
		// remove during undo. controllers are matched by name. if controller has been
		// added modify all links. this should be safe since only pasted links not present
		// in the animator can be potentially modified
		aeController * const controller = link->GetController();
		if(controller && !controllers.Has(controller)){
			aeController * const sameNameController = controllers.GetNamed(controller->GetName());
			
			if(sameNameController){
				link->SetController(sameNameController, false);
				
			}else{
				link->SetController(controller, false);
				pRemoveControllerList.Add(controller);
				pAnimator->AddController(controller);
			}
		}
		
		// insert link
		pAnimator->AddLink(link);
	}
	
	pAnimator->SetActiveLink(pLinkList.GetAt(linkCount - 1));
}
