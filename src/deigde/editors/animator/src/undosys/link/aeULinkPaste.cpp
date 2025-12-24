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

aeULinkPaste::aeULinkPaste(aeAnimator *animator, const aeLink::List &linkList) :
pAnimator(animator)
{
	DEASSERT_NOTNULL(animator)
	DEASSERT_TRUE(linkList.IsNotEmpty())
	
	linkList.Visit([&](const aeLink &link){
		pLinkList.Add(aeLink::Ref::New(link));
	});
}

aeULinkPaste::~aeULinkPaste(){
}



// Management
///////////////

void aeULinkPaste::Undo(){
	// remove the links
	pLinkList.Visit([&](aeLink *link){
		pAnimator->RemoveLink(link);
	});
	
	// remove controller added in a prior redo
	pRemoveControllerList.Visit([&](aeController *controller){
		pAnimator->RemoveController(controller);
	});
	pRemoveControllerList.RemoveAll();
}

void aeULinkPaste::Redo(){
	const aeController::List &controllers = pAnimator->GetControllers();
	
	pRemoveControllerList.RemoveAll();
	
	pLinkList.Visit([&](aeLink *link){
		// check if controllers exist in the animator. if not add them and mark them to
		// remove during undo. controllers are matched by name. if controller has been
		// added modify all links. this should be safe since only pasted links not present
		// in the animator can be potentially modified
		aeController * const controller = link->GetController();
		if(controller && !controllers.Has(controller)){
			aeController * const sameNameController = controllers.FindOrDefault([&](const aeController &c){
				return c.GetName() == controller->GetName();
			});
			
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
	});
	
	pAnimator->SetActiveLink(pLinkList.Last());
}
