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

#include "seULinkRemove.h"
#include "../../sky/seSky.h"
#include "../../sky/link/seLink.h"
#include "../../sky/layer/seLayer.h"

#include <dragengine/common/exceptions.h>



// Class seULinkRemove
////////////////////////

// Constructor, destructor
////////////////////////////

seULinkRemove::seULinkRemove(seLink *link) :
pSky(NULL),
pLink(NULL),
pTargets(NULL),
pTargetCount(0)
{
	if(!link){
		DETHROW(deeInvalidParam);
	}
	
	seSky * const sky = link->GetSky();
	if(!sky){
		DETHROW(deeInvalidParam);
	}
	
	if(sky->GetLinks().IndexOf(link) == -1){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Remove Link");
	
	const int targetCount = sky->CountLinkUsage(link);
	if(targetCount > 0){
		const seLayerList &layers = sky->GetLayers();
		const int layerCount = layers.GetCount();
		int i, j;
		
		try{
			pTargets = new sTarget[targetCount];
			
			for(i=0; i<layerCount; i++){
				seLayer * const layer = layers.GetAt(i);
				
				for(j=deSkyLayer::etOffsetX; j<=deSkyLayer::etAmbientIntensity; j++){
					const deSkyLayer::eTargets target = (deSkyLayer::eTargets)j;
					
					if(!layer->GetTarget(target).GetLinks().Has(link)){
						continue;
					}
					
					pTargets[pTargetCount].layer = layer;
					pTargets[pTargetCount].target = target;
					layer->AddReference();
					pTargetCount++;
				}
			}
			
		}catch(const deException &){
			pCleanUp();
			throw;
		}
	}
	
	pSky = sky;
	sky->AddReference();
	
	pLink = link;
	link->AddReference();
}

seULinkRemove::~seULinkRemove(){
	pCleanUp();
}



// Management
///////////////

void seULinkRemove::Undo(){
	pSky->AddLink(pLink);
	
	int i;
	for(i=0; i<pTargetCount; i++){
		pTargets[i].layer->GetTarget(pTargets[i].target).AddLink(pLink);
		pTargets[i].layer->NotifyTargetChanged(pTargets[i].target);
	}
}

void seULinkRemove::Redo(){
	int i;
	for(i=0; i<pTargetCount; i++){
		pTargets[i].layer->GetTarget(pTargets[i].target).RemoveLink(pLink);
		pTargets[i].layer->NotifyTargetChanged(pTargets[i].target);
	}
	
	pSky->RemoveLink(pLink);
}



// Private Functions
//////////////////////

void seULinkRemove::pCleanUp(){
	int i;
	for(i=0; i<pTargetCount; i++){
		pTargets[i].layer->FreeReference();
	}
	
	if(pLink){
		pLink->FreeReference();
	}
	if(pSky){
		pSky->FreeReference();
	}
}
