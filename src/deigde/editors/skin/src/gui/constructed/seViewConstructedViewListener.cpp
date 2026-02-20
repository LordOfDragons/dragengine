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

#include "seViewConstructedView.h"
#include "seViewConstructedViewListener.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"



// Class seViewConstructedViewListener
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seViewConstructedViewListener::seViewConstructedViewListener(seViewConstructedView &view) :
pView(view){
}

seViewConstructedViewListener::~seViewConstructedViewListener(){
}



// Management
///////////////

void seViewConstructedViewListener::ActiveTextureChanged(seSkin *skin){
	if(pView.GetSkin() != skin){
		return;
	}
	
	pView.UpdateConstructedCanvas();
	pView.RecreateContentCanvas();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyChanged(seSkin *skin, seTexture *texture, seProperty *property){
	if(pView.GetSkin() != skin || !texture->GetActive() || !property->GetActive()
	|| property->GetValueType() != seProperty::evtConstructed){
		return;
	}
	
	pView.UpdateConstructedCanvas();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::ActivePropertyChanged(seSkin *skin, seTexture *texture){
	if(pView.GetSkin() != skin || !texture->GetActive()){
		return;
	}
	
	pView.UpdateConstructedCanvas();
	pView.RecreateContentCanvas();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyNodeStructureChanged(seSkin *skin,
seTexture *texture, seProperty *property){
	if(pView.GetSkin() != skin || !texture->GetActive() || !property->GetActive()){
		return;
	}
	
	pView.RecreateContentCanvas();
	pView.UpdateGroupDarkening();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyNodeChanged(seSkin *skin, seTexture *texture,
seProperty *property, sePropertyNode *node){
	if(pView.GetSkin() != skin || !texture->GetActive() || !property->GetActive()){
		return;
	}
	
	if(node == property->GetNodeGroup()){
		pView.UpdateConstructedCanvas();
		
	}else{
		pView.UpdateContentCanvasParams();
	}
	
	pView.UpdateGroupDarkening();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyActiveNodeChanged(seSkin *skin,
seTexture *texture, seProperty *property){
	if(pView.GetSkin() != skin || !texture->GetActive() || !property->GetActive()){
		return;
	}
	
	pView.UpdateGroupDarkening();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyNodeSelectionChanged(seSkin *skin,
seTexture *texture, seProperty *property){
	if(pView.GetSkin() != skin || !texture->GetActive() || !property->GetActive()){
		return;
	}
	
	pView.UpdateGroupDarkening();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyActiveNodeGroupChanged(seSkin *skin,
seTexture *texture, seProperty *property){
	if(pView.GetSkin() != skin || !texture->GetActive() || !property->GetActive()){
		return;
	}
	
	pView.UpdateGroupDarkening();
	pView.UpdateMarkers();
}

void seViewConstructedViewListener::PropertyActiveNodeLayerChanged(seSkin *skin,
seTexture *texture, seProperty *property){
	if(pView.GetSkin() != skin || !texture->GetActive() || !property->GetActive()){
		return;
	}
	
	pView.UpdateConstructedCanvas();
	pView.RecreateContentCanvas();
	pView.UpdateMarkers();
}
