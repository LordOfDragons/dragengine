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

#include "skyeUTargetRemoveLink.h"
#include "../../sky/controller/skyeControllerTarget.h"
#include "../../sky/layer/skyeLayer.h"
#include "../../sky/link/skyeLink.h"

#include <dragengine/common/exceptions.h>



// Class skyeUTargetRemoveLink
//////////////////////////////

// Constructor, destructor
////////////////////////////

skyeUTargetRemoveLink::skyeUTargetRemoveLink(skyeLayer *layer,
deSkyLayer::eTargets target, skyeLink *link) :

pTarget(target)
{
	if(!layer || !link){
		DETHROW(deeInvalidParam);
	}
	if(!layer->GetTarget(target)->GetLinks().Has(link)){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Target Remove Link");
	
	pLayer = layer;
	pLink = link;
}

skyeUTargetRemoveLink::~skyeUTargetRemoveLink(){
}



// Management
///////////////

void skyeUTargetRemoveLink::Undo(){
	pLayer->GetTarget(pTarget)->AddLink(pLink);
	pLayer->NotifyTargetChanged(pTarget);
}

void skyeUTargetRemoveLink::Redo(){
	pLayer->GetTarget(pTarget)->RemoveLink(pLink);
	pLayer->NotifyTargetChanged(pTarget);
}
