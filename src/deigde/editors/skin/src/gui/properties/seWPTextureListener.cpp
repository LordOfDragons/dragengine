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

#include "seWPTexture.h"
#include "seWPTextureListener.h"
#include "../../skin/seSkin.h"
#include "../../skin/texture/seTexture.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seWPTextureListener
//////////////////////////////

// Constructor, destructor
////////////////////////////

seWPTextureListener::seWPTextureListener(seWPTexture &panel) :
pPanel(panel){
}

seWPTextureListener::~seWPTextureListener(){
}



// Management
///////////////

void seWPTextureListener::MappedStructureChanged(seSkin *skin){
	if(skin != pPanel.GetSkin()){
		return;
	}
	
	pPanel.UpdatePropertyMappedTargetList();
}

void seWPTextureListener::MappedNameChanged(seSkin *skin, seMapped*){
	MappedStructureChanged(skin);
}

void seWPTextureListener::TextureStructureChanged(seSkin *skin){
	if(skin != pPanel.GetSkin()){
		return;
	}
	
	pPanel.UpdateTextureList();
}

void seWPTextureListener::TextureChanged(seSkin *skin, seTexture *texture){
	if(skin != pPanel.GetSkin() || ! texture->GetActive()){
		return;
	}
	
	pPanel.UpdateTexture();
}

void seWPTextureListener::TextureNameChanged(seSkin *skin, seTexture*){
	if(skin != pPanel.GetSkin()){
		return;
	}
	
	pPanel.UpdateTextureList();
}

void seWPTextureListener::ActiveTextureChanged(seSkin *skin){
	if(skin != pPanel.GetSkin()){
		return;
	}
	
	pPanel.UpdateTexture();
}



void seWPTextureListener::PropertyStructureChanged(seSkin *skin, seTexture *texture){
	if(skin != pPanel.GetSkin() || ! texture->GetActive()){
		return;
	}
	
	pPanel.UpdatePropertyList();
}

void seWPTextureListener::PropertyChanged(seSkin *skin, seTexture *texture, seProperty *property){
	if(skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive()){
		return;
	}
	
	pPanel.ShowPropertyPanel();
	pPanel.UpdateProperty();
}

void seWPTextureListener::ActivePropertyChanged(seSkin *skin, seTexture *texture){
	if(skin != pPanel.GetSkin() || ! texture->GetActive()){
		return;
	}
	
	pPanel.ShowPropertyPanel();
	pPanel.SelectActiveProperty();
}

void seWPTextureListener::PropertyNodeChanged(seSkin* skin, seTexture* texture,
seProperty* property, sePropertyNode* node){
	if(skin != pPanel.GetSkin() || ! texture->GetActive() || ! property->GetActive()
	|| node != property->GetNodeGroup()){
		return;
	}
	
	pPanel.UpdateProperty();
}
