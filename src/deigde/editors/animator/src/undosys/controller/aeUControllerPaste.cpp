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

#include "aeUControllerPaste.h"
#include "../../animator/controller/aeController.h"

#include <dragengine/common/exceptions.h>



// Class aeUControllerPaste
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeUControllerPaste::aeUControllerPaste(aeAnimator *animator, const aeController::List &controllerList) :
pAnimator(animator)
{
	DEASSERT_NOTNULL(animator)
	const int controllerCount = controllerList.GetCount();
	DEASSERT_TRUE(controllerCount > 0)
	
	int i;
	for(i=0; i<controllerCount; i++){
		pControllerList.Add(aeController::Ref::New(*controllerList.GetAt(i)));
	}
	
	pIndex = animator->GetControllers().GetCount();
}

aeUControllerPaste::~aeUControllerPaste(){
}



// Management
///////////////

void aeUControllerPaste::Undo(){
	pControllerList.Visit([&](aeController *controller){
		pAnimator->RemoveController(controller);
	});
}

void aeUControllerPaste::Redo(){
	const aeController::List &controllers = pAnimator->GetControllers();
	
	pControllerList.VisitIndexed([&](int i, aeController *controller){
		decString name(controller->GetName());
		int number = 2;
		while(controllers.HasMatching([&](aeController *c){
			return c->GetName() == name;
		})){
			name.Format("%s #%d", controller->GetName().GetString(), number++);
		}
		controller->SetName(name);
		
		pAnimator->InsertControllerAt(controller, pIndex + i);
	});
	
	pAnimator->SetActiveController(pControllerList.Last());
}
