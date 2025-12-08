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

#include "seTexture.h"
#include "seTextureSkinBuilder.h"
#include "../seSkin.h"
#include "../property/seProperty.h"
#include "../dynamicskin/seDynamicSkin.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkin.h>



// Class seTexture
////////////////////

// Constructor, destructor
////////////////////////////

seTexture::seTexture(deEngine *engine, const char *name) :
pEngine(engine),
pEngSkin(NULL),
pSkin(NULL),
pName(name),
pActiveProperty(NULL),
pTexCoordScaling(1.0f, 1.0f),
pTexCoordRotation(0.0f),
pSelected(false),
pActive(false),
pDirtySkin(true)
{
	if(!engine){
		DETHROW(deeInvalidParam);
	}
}

seTexture::seTexture(const seTexture &texture) :
pEngine(texture.pEngine),
pEngSkin(NULL),
pSkin(NULL),
pName(texture.pName),
pActiveProperty(NULL),
pTexCoordOffset(texture.pTexCoordOffset),
pTexCoordScaling(texture.pTexCoordScaling),
pTexCoordRotation(texture.pTexCoordRotation),
pSelected(false),
pActive(false),
pDirtySkin(true)
{
	const int propertyCount = texture.pPropertyList.GetCount();
	seProperty *property = NULL;
	int i;
	
	try{
		for(i=0; i<propertyCount; i++){
			property = new seProperty(*texture.pPropertyList.GetAt(i));
			AddProperty(property);
			property->FreeReference();
			property = NULL;
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

seTexture::~seTexture(){
	pCleanUp();
}



// Management
///////////////

void seTexture::SetSkin(seSkin *skin){
	if(pSkin == skin){
		return;
	}
	
	pSkin = skin;
	UpdateResources();
}



void seTexture::SetName(const char *name){
	if(!name) DETHROW(deeInvalidParam);
	
	if(!pName.Equals(name)){
		pName = name;
		NotifyNameChanged();
	}
}

void seTexture::SetSelected(bool selected){
	pSelected = selected;
}

void seTexture::SetActive(bool active){
	pActive = active;
}



void seTexture::NotifyChanged(){
	InvalidateEngineSkin();
	if(pSkin){
		pSkin->NotifyTextureChanged(this);
	}
}

void seTexture::NotifyNameChanged(){
	InvalidateEngineSkin();
	if(pSkin){
		pSkin->NotifyTextureNameChanged(this);
	}
}



void seTexture::InvalidateEngineSkin(){
	pDirtySkin = true;
}

void seTexture::UpdateEngineSkin(){
	if(!pDirtySkin || !pSkin->GetEnableSkinUpdate()){
		return;
	}
	
	// free the old skin
		pEngSkin = NULL;
	}
	
	// build new skin. for this we use an empty name which creates an unnamed skin.
	// named skins can only exist once but unnamed skins can exist multiple times.
	seTextureSkinBuilder builder(*pSkin, *this);
	
	pEngSkin.TakeOver(GetEngine()->GetSkinManager()->CreateSkin("", builder));
	
	// update component and light with the new skin
	AssignSkinToComponentTexture();
	AssignSkinToLight();
	
	// no more dirty
	pDirtySkin = false;
}

void seTexture::AssignSkinToComponentTexture(){
	if(!pSkin || !pSkin->GetEngineComponent()){
		return;
	}
	
	deComponent &engComponent = *pSkin->GetEngineComponent();
	if(!engComponent.GetModel()){
		return;
	}
	
	const int index = engComponent.GetModel()->IndexOfTextureNamed(pName);
	if(index == -1){
		return;
	}
	
	deComponentTexture &engComponentTexture = engComponent.GetTextureAt(index);
	engComponentTexture.SetSkin(pEngSkin);
	engComponentTexture.SetTexture(0);
	engComponentTexture.SetTransform(pCalcTexCoordTransform());
	engComponentTexture.SetDynamicSkin(pSkin->GetDynamicSkin().GetEngineDynamicSkin());
	engComponent.NotifyTextureChanged(index);
	
	pUpdateTexCoordTransform();
}

void seTexture::AssignSkinToLight(){
	if(!pSkin || !pSkin->GetEngineComponent()){
		return;
	}
	if(pSkin->GetTextureList().GetAt(0) != this){
		return;
	}
	
	pSkin->GetEngineLight()->SetLightSkin(pEngSkin);
}



void seTexture::UpdateResources(){
	const int count = pPropertyList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pPropertyList.GetAt(i)->UpdateResources();
	}
}



// Properties
///////////////

void seTexture::AddProperty(seProperty *property){
	pPropertyList.Add(property);
	property->SetTexture(this);
	
	InvalidateEngineSkin();
	if(pSkin){
		pSkin->NotifyPropertyStructureChanged(this);
	}
	
	if(!pActiveProperty){
		SetActiveProperty(property);
	}
}

void seTexture::RemoveProperty(seProperty *property){
	if(!property || property->GetTexture() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(property->GetActive()){
		if(pPropertyList.GetCount() > 1){
			seProperty *activeProperty = pPropertyList.GetAt(0);
			
			if(activeProperty == property){
				activeProperty = pPropertyList.GetAt(1);
			}
			
			SetActiveProperty(activeProperty);
			
		}else{
			SetActiveProperty(NULL);
		}
	}
	
	property->SetTexture(NULL);
	pPropertyList.Remove(property);
	
	InvalidateEngineSkin();
	if(pSkin){
		pSkin->NotifyPropertyStructureChanged(this);
	}
}

void seTexture::RemoveAllProperties(){
	const int count = pPropertyList.GetCount();
	int p;
	
	SetActiveProperty(NULL);
	
	for(p=0; p<count; p++){
		pPropertyList.GetAt(p)->SetTexture(NULL);
	}
	pPropertyList.RemoveAll();
	
	InvalidateEngineSkin();
	if(pSkin){
		pSkin->NotifyPropertyStructureChanged(this);
	}
}

bool seTexture::HasActiveProperty() const{
	return pActiveProperty != nullptr;
}

void seTexture::SetActiveProperty(seProperty *property){
	if(property == pActiveProperty){
		return;
	}
	
	if(pActiveProperty){
		pActiveProperty->SetActive(false);
	}
	
	pActiveProperty = property;
	
	if(property){
		property->AddReference();
		property->SetActive(true);
	}
	
	if(pSkin){
		pSkin->NotifyActivePropertyChanged(this);
	}
}



// Preview Parameters
///////////////////////

void seTexture::SetTexCoordOffset(const decVector2 &offset){
	if(offset.IsEqualTo(pTexCoordOffset)){
		return;
	}
	
	pTexCoordOffset = offset;
	pUpdateTexCoordTransform();
}

void seTexture::SetTexCoordScaling(const decVector2 &scaling){
	if(scaling.IsEqualTo(pTexCoordScaling)){
		return;
	}
	
	pTexCoordScaling = scaling;
	pUpdateTexCoordTransform();
}

void seTexture::SetTexCoordRotation(float rotation){
	if(fabsf(rotation - pTexCoordRotation) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pTexCoordRotation = rotation;
	pUpdateTexCoordTransform();
}



// Private Functions
//////////////////////

void seTexture::pCleanUp(){
	RemoveAllProperties();
	
}

void seTexture::pUpdateTexCoordTransform(){
	if(!pSkin){
		return;
	}
	if(!pSkin->GetEngineComponent()){
		return;
	}
	
	deComponent &engComponent = *pSkin->GetEngineComponent();
	if(!engComponent.GetModel()){
		return;
	}
	
	const int index = engComponent.GetModel()->IndexOfTextureNamed(pName);
	if(index == -1){
		return;
	}
	
	deComponentTexture &engComponentTexture = engComponent.GetTextureAt(index);
	engComponentTexture.SetTransform(pCalcTexCoordTransform());
	engComponent.NotifyTextureChanged(index);
}

decTexMatrix2 seTexture::pCalcTexCoordTransform() const{
	decVector2 scaling(pTexCoordScaling);
	if(scaling.x == 0.0f){
		scaling.x = 1.0f;
	}
	if(scaling.y == 0.0f){
		scaling.y = 1.0f;
	}
	
	return decTexMatrix2::CreateScale(scaling) *
		decTexMatrix2::CreateRotation(pTexCoordRotation * DEG2RAD) *
		decTexMatrix2::CreateTranslation(pTexCoordOffset);
}
