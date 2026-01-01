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

#include <stdlib.h>
#include <string.h>

#include "skyeWPLayer.h"
#include "skyeWPLayerListener.h"
#include "../../sky/skyeSky.h"
#include "../../sky/body/skyeBody.h"
#include "../../sky/layer/skyeLayer.h"
#include "../../sky/controller/skyeControllerTarget.h"

#include <dragengine/common/exceptions.h>



// Class skyeWPLayerListener
////////////////////////////

// Constructor, destructor
////////////////////////////

skyeWPLayerListener::skyeWPLayerListener(skyeWPLayer &panel) :
pPanel(panel){
}

skyeWPLayerListener::~skyeWPLayerListener(){
}



// Management
///////////////

void skyeWPLayerListener::LinkStructureChanged(skyeSky*){
	pPanel.UpdateLinkList();
}

void skyeWPLayerListener::LinkNameChanged(skyeSky*, skyeLink*){
	pPanel.UpdateLinkList();
}



void skyeWPLayerListener::LayerStructureChanged(skyeSky*){
	pPanel.UpdateLayerList();
}

void skyeWPLayerListener::LayerChanged(skyeSky*, skyeLayer *layer){
	if(layer->GetActive()){
		pPanel.UpdateLayer();
	}
}

void skyeWPLayerListener::LayerNameChanged(skyeSky*, skyeLayer*){
	pPanel.UpdateLayerList();
}

void skyeWPLayerListener::ActiveLayerChanged(skyeSky*){
	pPanel.SelectActiveLayer();
}



void skyeWPLayerListener::BodyStructureChanged(skyeSky*, skyeLayer *layer){
	if(layer->GetActive()){
		pPanel.UpdateBodyList();
	}
}

void skyeWPLayerListener::BodyChanged(skyeSky*, skyeLayer *layer, skyeBody *body){
	if(layer->GetActive() && body->GetActive()){
		pPanel.UpdateBody();
	}
}

void skyeWPLayerListener::ActiveBodyChanged(skyeSky*, skyeLayer *layer){
	if(layer->GetActive()){
		pPanel.SelectActiveBody();
	}
}



void skyeWPLayerListener::TargetChanged(skyeSky*, skyeLayer* layer, deSkyLayer::eTargets){
	if(layer->GetActive()){
		pPanel.UpdateTarget();
	}
}

void skyeWPLayerListener::ActiveTargetChanged(skyeSky*, skyeLayer *layer){
	if(layer->GetActive()){
		pPanel.SelectActiveTarget();
	}
}
