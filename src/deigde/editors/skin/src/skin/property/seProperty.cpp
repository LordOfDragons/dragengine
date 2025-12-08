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

#include "seProperty.h"
#include "node/sePropertyNodeGroup.h"
#include "../seSkin.h"
#include "../texture/seTexture.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoManager.h>

#include <deigde/engine/textureProperties/igdeTextureProperty.h>
#include <deigde/engine/textureProperties/igdeTexturePropertyList.h>



// Class seProperty
/////////////////////

// Constructor, destructor
////////////////////////////

seProperty::seProperty(deEngine *engine, const char *name) :
pEngine(engine),

pTexture(NULL),

pName(name),
pValueType(evtColor),

pValue(0.0f),

pColor(1.0f, 1.0f, 1.0f),

pVideoSharedTime(true),

pNodeGroup(NULL),
pEngNodeGroup(NULL),
pActiveNodeGroup(NULL),
pNodeSelection(*this),
pActiveNodeLayer(0),
pNodeTileX(false),
pNodeTileY(false),
pNodeBitCount(8),

pSelected(false),
pActive(false)
{
	if(!engine){
		DETHROW(deeInvalidParam);
	}
	
	pNodeGroup.TakeOverWith*engine);
	pNodeGroup->SetProperty(this);
}

seProperty::seProperty(const seProperty &property) :
pEngine(property.pEngine),

pTexture(NULL),

pName(property.pName),
pValueType(property.pValueType),
pRenderableName(property.pRenderableName),
pBoneName(property.pBoneName),

pValue(property.pValue),

pColor(property.pColor),

pPathImage(property.pPathImage),
pEngImage(property.pEngImage),

pPathVideo(property.pPathVideo),
pEngVideo(property.pEngVideo),
pVideoSharedTime(property.pVideoSharedTime),

pNodeGroup(NULL),
pEngNodeGroup(NULL),
pActiveNodeGroup(NULL),
pNodeSelection(*this),
pActiveNodeLayer(0),
pNodeColor(property.pNodeColor),
pNodeTileX(property.pNodeTileX),
pNodeTileY(property.pNodeTileY),
pNodeBitCount(property.pNodeBitCount),

pSelected(false),
pActive(false)
{
	pMappedComponents[0] = property.pMappedComponents[0];
	pMappedComponents[1] = property.pMappedComponents[1];
	pMappedComponents[2] = property.pMappedComponents[2];
	pMappedComponents[3] = property.pMappedComponents[3];
	
	pNodeGroup.TakeOverWith*property.pNodeGroup);
	pNodeGroup->SetProperty(this);
}

seProperty::~seProperty(){
	pNodeSelection.RemoveAll();
	if(pNodeGroup){
		pNodeGroup->SetProperty(NULL);
	}
}



// Management
///////////////

void seProperty::SetTexture(seTexture *texture){
	if(pTexture == texture){
		return;
	}
	
	pTexture = texture;
	UpdateResources();
}

void seProperty::SetName(const char *name){
	pName = name;
}

void seProperty::SetRenderableName(const char *name){
	if(pRenderableName.Equals(name)){
		return;
	}
	
	pRenderableName = name;
	NotifyChanged();
}

void seProperty::SetBoneName(const char *name){
	if(pBoneName.Equals(name)){
		return;
	}
	
	pBoneName = name;
	NotifyChanged();
}

void seProperty::SetValueType(eValueTypes type){
	if(pValueType == type){
		return;
	}
	
	pValueType = type;
	NotifyChanged();
}

void seProperty::SetValue(float value){
	if(fabsf(value - pValue) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pValue = value;
	NotifyChanged();
}

void seProperty::SetColor(const decColor &color){
	if(color.IsEqualTo(pColor)){
		return;
	}
	
	pColor = color;
	NotifyChanged();
}

void seProperty::SetImagePath(const char *imagePath){
	if(pPathImage.Equals(imagePath)){
		return;
	}
	
	pPathImage = imagePath;
	UpdateImage();
	NotifyChanged();
}

void seProperty::UpdateImage(){
	deImage::Ref image;
	
	if(!pPathImage.IsEmpty() && pTexture && pTexture->GetSkin()){
		try{
			image.TakeOver(pEngine->GetImageManager()->LoadImage(
				pPathImage, pTexture->GetSkin()->GetDirectoryPath()));
			
		}catch(const deException &e){
			pTexture->GetSkin()->GetLogger()->LogException("Skin Editor", e);
		}
	}
	
	pEngImage = image;
}

void seProperty::SetVideoPath(const char *videoPath){
	if(pPathVideo.Equals(videoPath)){
		return;
	}
	
	pPathVideo = videoPath;
	UpdateVideo();
	NotifyChanged();
}

void seProperty::UpdateVideo(){
	deVideo::Ref video;
	
	if(!pPathVideo.IsEmpty() && pTexture && pTexture->GetSkin()){
		try{
			video.TakeOver(pEngine->GetVideoManager()->LoadVideo(
				pPathVideo, pTexture->GetSkin()->GetDirectoryPath(), false));
			
		}catch(const deException &e){
			pTexture->GetSkin()->GetLogger()->LogException("Skin Editor", e);
		}
	}
	
	pEngVideo = video;
}

void seProperty::SetVideoSharedTime(bool shareTime){
	if(pVideoSharedTime == shareTime){
		return;
	}
	
	pVideoSharedTime = shareTime;
	NotifyChanged();
}



const seMapped::Ref &seProperty::GetMappedComponent(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index <= 3)
	
	return pMappedComponents[index];
}

void seProperty::SetMappedComponent(int index, seMapped *mapped){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index <= 3)
	
	pMappedComponents[index] = mapped;
	NotifyChanged();
}



void seProperty::SetNodeGroup(sePropertyNodeGroup *nodeGroup){
	if(!nodeGroup || nodeGroup->GetProperty()){
		DETHROW(deeInvalidParam);
	}
	
	pNodeSelection.RemoveAll();
	
	pNodeGroup->SetProperty(NULL);
	
	pNodeGroup = nodeGroup;
	
	nodeGroup->AddReference();
	nodeGroup->SetProperty(this);
	
	nodeGroup->NotifyChanged();
	nodeGroup->NotifyStructreChanged();
	
	UpdateEngineNodeGroup();
	NotifyChanged();
}

void seProperty::UpdateEngineNodeGroup(){
	// TODO
}

void seProperty::SetActiveNodeGroup(sePropertyNodeGroup *node){
	if(pActiveNodeGroup == node){
		return;
	}
	
	const bool selectionChanged = pNodeSelection.GetSelected().GetCount() > 0;
	const bool activeNodeChanged = pNodeSelection.HasActive();
	
	pNodeSelection.RemoveAll();
	
	if(pActiveNodeGroup){
		pActiveNodeGroup->SetActiveGroup(false);
	}
	
	pActiveNodeGroup = node;
	
	if(node){
		node->AddReference();
		node->SetActiveGroup(true);
	}
	
	if(pTexture && pTexture->GetSkin()){
		seSkin &skin = *pTexture->GetSkin();
		if(selectionChanged){
			skin.NotifyPropertyNodeSelectionChanged(pTexture, this);
		}
		if(activeNodeChanged){
			skin.NotifyPropertyActiveNodeChanged(pTexture, this);
		}
		skin.NotifyPropertyActiveNodeGroupChanged(pTexture, this);
	}
}

void seProperty::SetActiveNodeLayer(int layer){
	if(pActiveNodeLayer == layer){
		return;
	}
	
	pActiveNodeLayer = layer;
	
	if(pTexture && pTexture->GetSkin()){
		pTexture->GetSkin()->NotifyPropertyActiveNodeLayerChanged(pTexture, this);
	}
}

void seProperty::SetNodeColor(const decColor &color){
	if(color.IsEqualTo(pNodeColor)){
		return;
	}
	
	pNodeColor = color;
	NotifyChanged();
}

void seProperty::SetNodeTileX(bool tileX){
	if(pNodeTileX == tileX){
		return;
	}
	
	pNodeTileX = tileX;
	NotifyChanged();
}

void seProperty::SetNodeTileY(bool tileY){
	if(pNodeTileY == tileY){
		return;
	}
	
	pNodeTileY = tileY;
	NotifyChanged();
}

void seProperty::SetNodeBitCount(int bitCount){
	if(pNodeBitCount == bitCount){
		return;
	}
	
	switch(bitCount){
	case 8:
	case 16:
	case 32:
		pNodeBitCount = bitCount;
		NotifyChanged();
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}



void seProperty::SetSelected(bool selected){
	pSelected = selected;
}

void seProperty::SetActive(bool active){
	pActive = active;
}

void seProperty::NotifyChanged(){
	if(!pTexture){
		return;
	}
	
	pTexture->InvalidateEngineSkin();
	
	if(pTexture->GetSkin()){
		pTexture->GetSkin()->NotifyPropertyChanged(pTexture, this);
	}
}

void seProperty::UpdateResources(){
	UpdateImage();
	UpdateVideo();
	pNodeGroup->UpdateResources();
}

void seProperty::InitDefaults(const igdeTexturePropertyList &knownPropertyList){
	const igdeTextureProperty * const knownProperty = knownPropertyList.GetNamed(pName);
	if(!knownProperty){
		return;
	}
	
	SetValue(knownProperty->GetDefaultValue());
	SetColor(knownProperty->GetDefaultColor());
	
	if(knownProperty->GetComponentCount() == 1){
		SetValueType(seProperty::evtValue);
		SetNodeColor(decColor(knownProperty->GetDefaultValue(),
			knownProperty->GetDefaultValue(), knownProperty->GetDefaultValue()));
		
	}else{
		SetValueType(seProperty::evtColor);
		SetNodeColor(knownProperty->GetDefaultColor());
	}
}
