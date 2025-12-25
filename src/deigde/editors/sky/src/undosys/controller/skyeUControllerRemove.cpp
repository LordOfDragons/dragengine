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

#include "skyeUControllerRemove.h"
#include "../../sky/skyeSky.h"
#include "../../sky/controller/skyeController.h"
#include "../../sky/layer/skyeLayer.h"

#include <dragengine/common/exceptions.h>



// Class skyeUControllerRemove
//////////////////////////////

// Constructor, destructor
////////////////////////////

skyeUControllerRemove::skyeUControllerRemove(skyeController *controller) :
pIndex(0)
{
	DEASSERT_NOTNULL(controller)
	
	skyeSky * const sky = controller->GetSky();
	DEASSERT_NOTNULL(sky)
	
	SetShortInfo("Remove Controller");
	
	pIndex = sky->GetControllers().IndexOf(controller);
	DEASSERT_TRUE(pIndex != -1)
	
	const int usageCount = sky->CountControllerUsage(controller);
	if(usageCount > 0){
		sky->GetLayers().Visit([&](const skyeLayer &layer){
			int i;
			for(i=deSkyLayer::etOffsetX; i<=deSkyLayer::etAmbientIntensity; i++){
				const deSkyLayer::eTargets target = (deSkyLayer::eTargets)i;
				layer.GetTarget(target).GetLinks().Visit([&](skyeLink *link){
					if(link->GetController() == controller){
						pLinks.Add(link);
					}
				});
			}
		});
	}
	
	pSky = sky;
	
	pController = controller;
}

skyeUControllerRemove::~skyeUControllerRemove(){
}



// Management
///////////////

void skyeUControllerRemove::Undo(){
	pSky->InsertControllerAt(pController, pIndex);
	pLinks.Visit([&](skyeLink &link){
		link.SetController(pController);
	});
}

void skyeUControllerRemove::Redo(){
	pSky->RemoveController(pController);
	pLinks.Visit([&](skyeLink &link){
		link.SetController(nullptr);
	});
}
