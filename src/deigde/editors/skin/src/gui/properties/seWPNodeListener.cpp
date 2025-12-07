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

#include "seWPNode.h"
#include "seWPNodeListener.h"
#include "../../skin/seSkin.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNode.h"

#include <dragengine/common/exceptions.h>



// Class seWPNodeListener
///////////////////////////

// Constructor, destructor
////////////////////////////

seWPNodeListener::seWPNodeListener(seWPNode &panel) :
pPanel(panel){
}

seWPNodeListener::~seWPNodeListener(){
}



// Management
///////////////

void seWPNodeListener::MappedStructureChanged(seSkin *skin){
	if(skin != pPanel.GetSkin()){
		return;
	}
	
	pPanel.UpdateMappedTargetList();
}

void seWPNodeListener::MappedNameChanged(seSkin *skin, seMapped*){
	MappedStructureChanged(skin);
}



void seWPNodeListener::ActiveTextureChanged(seSkin *skin){
	if(skin != pPanel.GetSkin()){
		return;
	}
	
	pPanel.UpdateMappedTypeList();
	pPanel.UpdateNode();
}

void seWPNodeListener::PropertyChanged(seSkin *skin, seTexture *texture, seProperty *property){
	if(skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive()){
		return;
	}
	
	pPanel.ShowNodePanel();
	pPanel.UpdateNode();
}

void seWPNodeListener::ActivePropertyChanged(seSkin *skin, seTexture *texture){
	if(skin != pPanel.GetSkin() || ! texture->GetActive()){
		return;
	}
	
	pPanel.UpdateOutline();
	pPanel.ShowNodePanel();
	pPanel.UpdateMappedTypeList();
	pPanel.UpdateNode();
}

void seWPNodeListener::PropertyNodeChanged(seSkin *skin, seTexture *texture,
seProperty *property, sePropertyNode*){
	if(skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive()){
		return;
	}
	
	pPanel.UpdateOutline();
	pPanel.UpdateNode();
}

void seWPNodeListener::PropertyNodeStructureChanged(seSkin *skin, seTexture *texture,
seProperty *property){
	if(skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive()){
		return;
	}
	
	pPanel.UpdateOutline();
	pPanel.OutlinerSelectActive();
}

void seWPNodeListener::PropertyNodeSelectionChanged(seSkin *skin, seTexture *texture,
seProperty *property){
	if(skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive()){
		return;
	}
	
	pPanel.UpdateNode();
	pPanel.ShowNodePanel();
}

void seWPNodeListener::PropertyActiveNodeChanged(seSkin *skin, seTexture *texture,
seProperty *property){
	if(skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive()){
		return;
	}
	
	pPanel.OutlinerSelectActive();
	pPanel.UpdateMappedTypeList();
	pPanel.UpdateNode();
	pPanel.ShowNodePanel();
}
