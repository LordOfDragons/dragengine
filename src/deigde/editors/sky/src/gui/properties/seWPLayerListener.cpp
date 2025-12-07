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

#include "seWPLayer.h"
#include "seWPLayerListener.h"
#include "../../sky/seSky.h"
#include "../../sky/body/seBody.h"
#include "../../sky/layer/seLayer.h"
#include "../../sky/controller/seControllerTarget.h"

#include <dragengine/common/exceptions.h>



// Class seWPLayerListener
////////////////////////////

// Constructor, destructor
////////////////////////////

seWPLayerListener::seWPLayerListener(seWPLayer &panel) :
pPanel(panel){
}

seWPLayerListener::~seWPLayerListener(){
}



// Management
///////////////

void seWPLayerListener::LinkStructureChanged(seSky*){
	pPanel.UpdateLinkList();
}

void seWPLayerListener::LinkNameChanged(seSky*, seLink*){
	pPanel.UpdateLinkList();
}



void seWPLayerListener::LayerStructureChanged(seSky*){
	pPanel.UpdateLayerList();
}

void seWPLayerListener::LayerChanged(seSky*, seLayer *layer){
	if(layer->GetActive()){
		pPanel.UpdateLayer();
	}
}

void seWPLayerListener::LayerNameChanged(seSky*, seLayer*){
	pPanel.UpdateLayerList();
}

void seWPLayerListener::ActiveLayerChanged(seSky*){
	pPanel.SelectActiveLayer();
}



void seWPLayerListener::BodyStructureChanged(seSky*, seLayer *layer){
	if(layer->GetActive()){
		pPanel.UpdateBodyList();
	}
}

void seWPLayerListener::BodyChanged(seSky*, seLayer *layer, seBody *body){
	if(layer->GetActive() && body->GetActive()){
		pPanel.UpdateBody();
	}
}

void seWPLayerListener::ActiveBodyChanged(seSky*, seLayer *layer){
	if(layer->GetActive()){
		pPanel.SelectActiveBody();
	}
}



void seWPLayerListener::TargetChanged(seSky*, seLayer* layer, deSkyLayer::eTargets){
	if(layer->GetActive()){
		pPanel.UpdateTarget();
	}
}

void seWPLayerListener::ActiveTargetChanged(seSky*, seLayer *layer){
	if(layer->GetActive()){
		pPanel.SelectActiveTarget();
	}
}
