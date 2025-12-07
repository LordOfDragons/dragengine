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

#include "meObjectTexture.h"
#include "../meObject.h"
#include "../../meWorld.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>


 
// Class meObjectTexture
//////////////////////////

// Constructor, destructor
////////////////////////////

meObjectTexture::meObjectTexture(igdeEnvironment *environment, const char *name) :
pEnvironment(environment),

pObject(NULL),

pName(name),
pEngSkin(NULL),
pTexCoordScaling(1.0f, 1.0f),
pTexCoordRotation(0.0f),

pColorTint(1.0f, 1.0f, 1.0f),

pDynamicSkin(NULL)
{
	if(!environment){
		DETHROW(deeInvalidParam);
	}
}

meObjectTexture::meObjectTexture(const meObjectTexture &texture) :
pEnvironment(texture.pEnvironment),

pObject(NULL),

pName(texture.pName),
pSkinPath(texture.pSkinPath),
pEngSkin(NULL),
pTexCoordOffset(texture.pTexCoordOffset),
pTexCoordScaling(texture.pTexCoordScaling),
pTexCoordRotation(texture.pTexCoordRotation),

pColorTint(texture.pColorTint),

pProperties(texture.pProperties),
pActiveProperty(texture.pActiveProperty),

pDynamicSkin(NULL)
{
	try{
		pEngSkin = texture.GetEngineSkin();
		if(pEngSkin){
			pEngSkin->AddReference();
		}
		
		UpdateDynamicSkin();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meObjectTexture::~meObjectTexture(){
	pCleanUp();
}



// Management
///////////////

void meObjectTexture::SetObject(meObject *object){
	pObject = object;
}



void meObjectTexture::SetSkinPath(const char *skinPath){
	if(pSkinPath == skinPath){
		return;
	}
	
	pSkinPath = skinPath;
	LoadSkin();
	NotifyChanged();
}

void meObjectTexture::LoadSkin(){
	deSkin *engSkin = NULL;
	
	if(!pSkinPath.IsEmpty()){
		try{
			engSkin = pEnvironment->GetEngineController()->GetEngine()->
				GetSkinManager()->LoadSkin(pSkinPath.GetString(), "/");
			
		}catch(const deException &){
			engSkin = pEnvironment->GetStockSkin(igdeEnvironment::essError);
			engSkin->AddReference();
		}
	}
	
	if(pEngSkin){
		pEngSkin->FreeReference();
	}
	pEngSkin = engSkin;
	
	UpdateDynamicSkin();
	
	if(pObject){
		pObject->UpdateComponentTextures();
	}
}



void meObjectTexture::SetTexCoordOffset(const decVector2 &offset){
	if(offset.IsEqualTo(pTexCoordOffset)){
		return;
	}
	
	pTexCoordOffset = offset;
	
	if(pObject){
		pObject->UpdateComponentTextures();
	}
	NotifyChanged();
}

void meObjectTexture::SetTexCoordScaling(const decVector2 &scaling){
	if(scaling.IsEqualTo(pTexCoordScaling)){
		return;
	}
	
	pTexCoordScaling = scaling;
	
	if(pObject){
		pObject->UpdateComponentTextures();
	}
	NotifyChanged();
}

void meObjectTexture::SetTexCoordRotation(float rotation){
	if(fabsf(rotation - pTexCoordRotation) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pTexCoordRotation = rotation;
	
	if(pObject){
		pObject->UpdateComponentTextures();
	}
	NotifyChanged();
}



void meObjectTexture::SetColorTint(const decColor &color){
	if(color.IsEqualTo(pColorTint)){
		return;
	}
	
	pColorTint = color;
	
	UpdateDynamicSkin();
	
	if(pObject){
		pObject->UpdateComponentTextures();
	}
	NotifyChanged();
}



void meObjectTexture::UpdateDynamicSkin(){
	const bool hasTint = !decColor(1.0f, 1.0f, 1.0f).IsEqualTo(pColorTint);
	bool requiresDynamicSkin = false;
	
	if(hasTint){
		requiresDynamicSkin = true;
	}
	
	if(requiresDynamicSkin){
		if(!pDynamicSkin){
			pDynamicSkin = pEnvironment->GetEngineController()->GetEngine()->GetDynamicSkinManager()->CreateDynamicSkin();
		}
		
		pDynamicSkin->RemoveAllRenderables();
		
		if(!hasTint){
			return;
		}
		
		deDSRenderableColor * const renderable = new deDSRenderableColor("tint");
		renderable->SetColor(pColorTint);
		pDynamicSkin->AddRenderable(renderable);
		
	}else{
		if(pDynamicSkin){
			pDynamicSkin->FreeReference();
			pDynamicSkin = NULL;
		}
	}
}



void meObjectTexture::NotifyChanged(){
	if(!pObject){
		return;
	}
	
	meWorld * const world = pObject->GetWorld();
	if(!world){
		return;
	}
	
	world->SetChanged(true);
	world->NotifyObjectTextureChanged(pObject, this);
}

void meObjectTexture::NotifyPropertiesChanged(){
	if(!pObject){
		return;
	}
	
	meWorld * const world = pObject->GetWorld();
	if(!world){
		return;
	}
	
	world->SetChanged(true);
	world->NotifyObjectTexturePropertiesChanged(pObject, this);
}

void meObjectTexture::NotifyActivePropertyChanged(){
	if(!pObject){
		return;
	}
	
	meWorld * const world = pObject->GetWorld();
	if(!world){
		return;
	}
	
	world->SetChanged(true);
	world->NotifyObjectTextureActivePropertyChanged(pObject, this);
}



// Properties
///////////////

void meObjectTexture::SetProperty(const char *key, const char *value){
	bool activeChanged = false;
	
	pProperties.SetAt(key, value);
	
	if(pActiveProperty.IsEmpty()){
		pActiveProperty = key;
		activeChanged = true;
	}
	
	//UpdateProperties();
	
	NotifyPropertiesChanged();
	if(activeChanged){
		NotifyActivePropertyChanged();
	}
}

void meObjectTexture::SetProperties(const decStringDictionary &properties){
	pProperties = properties;
	
	if(pProperties.GetCount() == 0){
		pActiveProperty = "";
		
	}else{
		pActiveProperty = pProperties.GetAt(pProperties.GetKeys().GetAt(0));
	}
	
	//UpdateProperties();
	
	NotifyPropertiesChanged();
	NotifyActivePropertyChanged();
}

void meObjectTexture::RemoveProperty(const char *key){
	if(!pProperties.Has(key)){
		return;
	}
	
	bool activeChanged = false;
	
	pProperties.Remove(key);
	
	if(pActiveProperty == key){
		if(pProperties.GetCount() == 0){
			pActiveProperty = "";
			
		}else{
			pActiveProperty = pProperties.GetAt(pProperties.GetKeys().GetAt(0));
		}
		activeChanged = true;
	}
	
	//UpdateProperties();
	
	NotifyPropertiesChanged();
	if(activeChanged){
		NotifyActivePropertyChanged();
	}
}

void meObjectTexture::RemoveAllProperties(){
	if(pProperties.GetCount() == 0){
		return;
	}
	
	pProperties.RemoveAll();
	pActiveProperty = "";
	
	//UpdateProperties();
	
	NotifyPropertiesChanged();
	NotifyActivePropertyChanged();
}

void meObjectTexture::SetActiveProperty(const char *property){
	if(pActiveProperty == property){
		return;
	}
	
	pActiveProperty = property;
	
	NotifyActivePropertyChanged();
}



// Private Functions
//////////////////////

void meObjectTexture::pCleanUp(){
	if(pDynamicSkin){
		pDynamicSkin->FreeReference();
	}
	if(pEngSkin){
		pEngSkin->FreeReference();
	}
}
