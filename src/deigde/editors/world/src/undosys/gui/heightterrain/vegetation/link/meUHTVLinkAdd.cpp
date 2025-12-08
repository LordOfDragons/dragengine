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

#include "meUHTVLinkAdd.h"
#include "../../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../../world/heightterrain/rules/meHTVRSlot.h"
#include "../../../../../world/heightterrain/rules/meHTVRLink.h"
#include "../../../../../world/heightterrain/rules/meHTVRule.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVLinkAdd
////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVLinkAdd::meUHTVLinkAdd(meHTVegetationLayer *vlayer, meHTVRLink *link){
	if(!vlayer || !link) DETHROW(deeInvalidParam);
	
	pVLayer = NULL;
	pLink = NULL;
	
	SetShortInfo("Add Vegetation Layer Link");
	SetMemoryConsumption(sizeof(meUHTVLinkAdd));
	
	pVLayer = vlayer;
	pLink = link;
}

meUHTVLinkAdd::~meUHTVLinkAdd(){
	if(pLink) pLink->FreeReference();
	if(pVLayer) pVLayer->FreeReference();
}



// Management
///////////////

void meUHTVLinkAdd::Undo(){
	meHTVRule *ruleSource = pLink->GetSourceRule();
	meHTVRule *ruleDestination = pLink->GetDestinationRule();
	int slotSource = pLink->GetSourceSlot();
	int slotDestination = pLink->GetDestinationSlot();
	
	ruleSource->GetSlotAt(slotSource).RemoveLink(pLink);
	ruleDestination->GetSlotAt(slotDestination).RemoveLink(pLink);
	
	pVLayer->RemoveLink(pLink);
}

void meUHTVLinkAdd::Redo(){
	meHTVRule *ruleSource = pLink->GetSourceRule();
	meHTVRule *ruleDestination = pLink->GetDestinationRule();
	int slotSource = pLink->GetSourceSlot();
	int slotDestination = pLink->GetDestinationSlot();
	
	ruleSource->GetSlotAt(slotSource).AddLink(pLink);
	ruleDestination->GetSlotAt(slotDestination).AddLink(pLink);
	
	pVLayer->AddLink(pLink);
}
