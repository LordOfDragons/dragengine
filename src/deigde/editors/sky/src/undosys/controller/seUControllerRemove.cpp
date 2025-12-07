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

#include "seUControllerRemove.h"
#include "../../sky/seSky.h"
#include "../../sky/controller/seController.h"
#include "../../sky/link/seLink.h"
#include "../../sky/layer/seLayer.h"

#include <dragengine/common/exceptions.h>



// Class seUControllerRemove
//////////////////////////////

// Constructor, destructor
////////////////////////////

seUControllerRemove::seUControllerRemove(seController *controller) :
pSky(nullptr),
pController(nullptr),
pIndex(0),
pLinks(nullptr),
pLinkCount(0)
{
	DEASSERT_NOTNULL(controller)
	
	seSky * const sky = controller->GetSky();
	DEASSERT_NOTNULL(sky)
	
	SetShortInfo("Remove Controller");
	
	pIndex = sky->GetControllers().IndexOf(controller);
	DEASSERT_TRUE(pIndex != -1)
	
	const int usageCount = sky->CountControllerUsage(controller);
	if(usageCount > 0){
		const seLayerList &layers = sky->GetLayers();
		const int layerCount = layers.GetCount();
		int i, j, k;
		
		try{
			pLinks = new seLink*[usageCount];
			
			for(i=0; i<layerCount; i++){
				seLayer * const layer = layers.GetAt(i);
				
				for(j=deSkyLayer::etOffsetX; j<=deSkyLayer::etAmbientIntensity; j++){
					const deSkyLayer::eTargets target = (deSkyLayer::eTargets)j;
					const seLinkList &links = layer->GetTarget(target).GetLinks();
					
					const int linkCount = links.GetCount();
					for(k=0; k<linkCount; k++){
						seLink * const link = links.GetAt(k);
						if(link->GetController() != controller){
							continue;
						}
						
						pLinks[pLinkCount] = link;
						link->AddReference();
						pLinkCount++;
					}
				}
			}
			
		}catch(const deException &){
			pCleanUp();
			throw;
		}
	}
	
	pSky = sky;
	sky->AddReference();
	
	pController = controller;
	controller->AddReference();
}

seUControllerRemove::~seUControllerRemove(){
	pCleanUp();
}



// Management
///////////////

void seUControllerRemove::Undo(){
	pSky->InsertControllerAt(pController, pIndex);
	
	int i;
	for(i=0; i<pLinkCount; i++){
		pLinks[i]->SetController(pController);
	}
}

void seUControllerRemove::Redo(){
	pSky->RemoveController(pController);
	
	int i;
	for(i=0; i<pLinkCount; i++){
		pLinks[i]->SetController(nullptr);
	}
}



// Private Functions
//////////////////////

void seUControllerRemove::pCleanUp(){
	int i;
	for(i=0; i<pLinkCount; i++){
		pLinks[i]->FreeReference();
	}
	
	if(pController){
		pController->FreeReference();
	}
	if(pSky){
		pSky->FreeReference();
	}
}
