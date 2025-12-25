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

#include "skyeULinkRemove.h"
#include "../../sky/skyeSky.h"
#include "../../sky/link/skyeLink.h"
#include "../../sky/layer/skyeLayer.h"

#include <dragengine/common/exceptions.h>



// Class skyeULinkRemove
////////////////////////

// Constructor, destructor
////////////////////////////

skyeULinkRemove::skyeULinkRemove(skyeLink *link){
	DEASSERT_NOTNULL(link)
	
	skyeSky * const sky = link->GetSky();
	DEASSERT_NOTNULL(sky)
	
	DEASSERT_TRUE(sky->GetLinks().IndexOf(link) != -1)
	
	SetShortInfo("Remove Link");
	
	const int targetCount = sky->CountLinkUsage(link);
	if(targetCount > 0){
		sky->GetLayers().Visit([&](skyeLayer *layer){
			int i;
			for(i=deSkyLayer::etOffsetX; i<=deSkyLayer::etAmbientIntensity; i++){
				const deSkyLayer::eTargets target = (deSkyLayer::eTargets)i;
				
				if(!layer->GetTarget(target).GetLinks().Has(link)){
					continue;
				}
				
				const cTarget::Ref utarget(cTarget::Ref::New());
				utarget->layer = layer;
				utarget->target = target;
				pTargets.Add(utarget);
			}
		});
	}
	
	pSky = sky;
	pLink = link;
}

skyeULinkRemove::~skyeULinkRemove(){
}



// Management
///////////////

void skyeULinkRemove::Undo(){
	pSky->AddLink(pLink);
	
	pTargets.Visit([&](cTarget &target){
		target.layer->GetTarget(target.target).AddLink(pLink);
		target.layer->NotifyTargetChanged(target.target);
	});
}

void skyeULinkRemove::Redo(){
	pTargets.Visit([&](cTarget &target){
		target.layer->GetTarget(target.target).RemoveLink(pLink);
		target.layer->NotifyTargetChanged(target.target);
	});
	
	pSky->RemoveLink(pLink);
}
