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

#include "meUHTVLinkCut.h"
#include "../../../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../../../world/heightterrain/rules/meHTVRSlot.h"
#include "../../../../../world/heightterrain/rules/meHTVRLink.h"
#include "../../../../../world/heightterrain/rules/meHTVRule.h"

#include <dragengine/common/exceptions.h>

// Class meUHTVLinkCut
////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVLinkCut::meUHTVLinkCut(meHTVegetationLayer *vlayer){
	if(!vlayer) DETHROW(deeInvalidParam);
	
	pVLayer = NULL;
	pLinks = NULL;
	pLinkCount = 0;
	
	SetShortInfo("Cut Vegetation Layer Links");
	SetMemoryConsumption(sizeof(meUHTVLinkCut));
	
	pVLayer = vlayer;
	vlayer->AddReference();
}

meUHTVLinkCut::~meUHTVLinkCut(){
	if(pLinks){
		while(pLinkCount > 0){
			pLinkCount--;
			pLinks[pLinkCount]->FreeReference();
		}
		delete [] pLinks;
	}
	

}

// Management
///////////////

void meUHTVLinkCut::AddLinkToCut(meHTVRLink *link){
	if(!link) DETHROW(deeInvalidParam);
	
	meHTVRLink **newArray = new meHTVRLink*[pLinkCount + 1];
	if(!newArray) DETHROW(deeOutOfMemory);
	if(pLinks){
		memcpy(newArray, pLinks, sizeof(meHTVRLink*) * pLinkCount);
		delete [] pLinks;
	}
	pLinks = newArray;
	pLinks[pLinkCount++] = link;
	
	link->AddReference();
}

void meUHTVLinkCut::Undo(){
	meHTVRule *ruleSource, *ruleDestination;
	int l, slotSource, slotDestination;
	
	for(l=0; l<pLinkCount; l++){
		ruleSource = pLinks[l]->GetSourceRule();
		ruleDestination = pLinks[l]->GetDestinationRule();
		slotSource = pLinks[l]->GetSourceSlot();
		slotDestination = pLinks[l]->GetDestinationSlot();
		
		ruleSource->GetSlotAt(slotSource).AddLink(pLinks[l]);
		ruleDestination->GetSlotAt(slotDestination).AddLink(pLinks[l]);
		
		pVLayer->AddLink(pLinks[l]);
	}
}

void meUHTVLinkCut::Redo(){
	meHTVRule *ruleSource, *ruleDestination;
	int l, slotSource, slotDestination;
	
	for(l=0; l<pLinkCount; l++){
		ruleSource = pLinks[l]->GetSourceRule();
		ruleDestination = pLinks[l]->GetDestinationRule();
		slotSource = pLinks[l]->GetSourceSlot();
		slotDestination = pLinks[l]->GetDestinationSlot();
		
		ruleSource->GetSlotAt(slotSource).RemoveLink(pLinks[l]);
		ruleDestination->GetSlotAt(slotDestination).RemoveLink(pLinks[l]);
		
		pVLayer->RemoveLink(pLinks[l]);
	}
}
