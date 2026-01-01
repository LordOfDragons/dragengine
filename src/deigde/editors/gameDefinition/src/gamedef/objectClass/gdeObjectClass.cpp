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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdeObjectClass.h"
#include "billboard/gdeOCBillboard.h"
#include "camera/gdeOCCamera.h"
#include "component/gdeOCComponent.h"
#include "component/gdeOCComponentTexture.h"
#include "envmapprobe/gdeOCEnvMapProbe.h"
#include "inherit/gdeOCInherit.h"
#include "light/gdeOCLight.h"
#include "navblocker/gdeOCNavigationBlocker.h"
#include "navspace/gdeOCNavigationSpace.h"
#include "particleemitter/gdeOCParticleEmitter.h"
#include "forceField/gdeOCForceField.h"
#include "snappoint/gdeOCSnapPoint.h"
#include "speaker/gdeOCSpeaker.h"
#include "world/gdeOCWorld.h"
#include "../gdeGameDefinition.h"
#include "../property/gdeProperty.h"

#include <deigde/gamedefinition/class/igdeGDClass.h>

#include <dragengine/common/exceptions.h>



// Class gdeObjectClass
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeObjectClass::gdeObjectClass(const char *name) :
pGameDefinition(nullptr),
pName(name),
pScaleMode(esmUniform),

pIsGhost(false),
pCanInstantiate(true),
pIsAttachableBehavior(false),
pInheritSubObjects(igdeGDClass::FilterSubObjectsAll){
}

gdeObjectClass::gdeObjectClass(const gdeObjectClass &objectClass) :
pGameDefinition(nullptr),
pName(objectClass.pName),
pDescription(objectClass.pDescription),
pScaleMode(objectClass.pScaleMode),

pPropertyValues(objectClass.pPropertyValues),

pCategory(objectClass.pCategory),
pHideTags(objectClass.pHideTags),
pPartialHideTags(objectClass.pPartialHideTags),

pIsGhost(objectClass.pIsGhost),
pCanInstantiate(objectClass.pCanInstantiate),
pIsAttachableBehavior(objectClass.pIsAttachableBehavior),
pInheritSubObjects(objectClass.pInheritSubObjects)
{
	objectClass.pProperties.Visit([&](const gdeProperty &p){
		pProperties.Add(gdeProperty::Ref::New(p));
	});
	
	objectClass.pTextureProperties.Visit([&](const gdeProperty &p){
		pTextureProperties.Add(gdeProperty::Ref::New(p));
	});
	
	objectClass.pInherits.Visit([&](const gdeOCInherit &p){
		pInherits.Add(gdeOCInherit::Ref::New(p));
	});
	
	objectClass.pBillboards.Visit([&](const gdeOCBillboard &p){
		pBillboards.Add(gdeOCBillboard::Ref::New(p));
	});
	
	objectClass.pCameras.Visit([&](const gdeOCCamera &p){
		pCameras.Add(gdeOCCamera::Ref::New(p));
	});
	
	objectClass.pComponents.Visit([&](const gdeOCComponent &p){
		pComponents.Add(gdeOCComponent::Ref::New(p));
	});
	
	objectClass.pEnvMapProbes.Visit([&](const gdeOCEnvMapProbe &p){
		pEnvMapProbes.Add(gdeOCEnvMapProbe::Ref::New(p));
	});
	
	objectClass.pLights.Visit([&](const gdeOCLight &p){
		pLights.Add(gdeOCLight::Ref::New(p));
	});
	
	objectClass.pNavigationBlockers.Visit([&](const gdeOCNavigationBlocker &p){
		pNavigationBlockers.Add(gdeOCNavigationBlocker::Ref::New(p));
	});
	
	objectClass.pNavigationSpaces.Visit([&](const gdeOCNavigationSpace &p){
		pNavigationSpaces.Add(gdeOCNavigationSpace::Ref::New(p));
	});
	
	objectClass.pParticleEmitters.Visit([&](const gdeOCParticleEmitter &p){
		pParticleEmitters.Add(gdeOCParticleEmitter::Ref::New(p));
	});
	
	objectClass.pForceFields.Visit([&](const gdeOCForceField &p){
		pForceFields.Add(gdeOCForceField::Ref::New(p));
	});
	
	objectClass.pSnapPoints.Visit([&](const gdeOCSnapPoint &p){
		pSnapPoints.Add(gdeOCSnapPoint::Ref::New(p));
	});
	
	objectClass.pSpeakers.Visit([&](const gdeOCSpeaker &p){
		pSpeakers.Add(gdeOCSpeaker::Ref::New(p));
	});
	
	objectClass.pWorlds.Visit([&](const gdeOCWorld &p){
		pWorlds.Add(gdeOCWorld::Ref::New(p));
	});
	
	objectClass.pTextures.Visit([&](const gdeOCComponentTexture &p){
		pTextures.Add(gdeOCComponentTexture::Ref::New(p));
	});
}

gdeObjectClass::~gdeObjectClass(){
	pCleanUp();
}



// Management
///////////////

void gdeObjectClass::SetGameDefinition(gdeGameDefinition *gamedef){
	pGameDefinition = gamedef;
}



void gdeObjectClass::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	
	if(pGameDefinition){
		pGameDefinition->NotifyObjectClassNameChanged(this);
	}
}

void gdeObjectClass::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	
	if(pGameDefinition){
		pGameDefinition->NotifyObjectClassChanged(this);
	}
}



void gdeObjectClass::SetScaleMode(eScaleModes mode){
	if(mode < esmFixed || mode > esmFree){
		DETHROW(deeInvalidParam);
	}
	
	if(pScaleMode == mode){
		return;
	}
	
	pScaleMode = mode;
	
	if(pGameDefinition){
		pGameDefinition->NotifyObjectClassChanged(this);
	}
}



void gdeObjectClass::SetCategory(const char *category){
	if(pCategory == category){
		return;
	}
	
	pCategory = category;
	
	if(pGameDefinition){
		pGameDefinition->NotifyObjectClassChanged(this);
	}
}

void gdeObjectClass::SetHideTags(const decStringSet &tags){
	if(pHideTags == tags){
		return;
	}
	
	pHideTags = tags;
	
	if(pGameDefinition){
		pGameDefinition->NotifyObjectClassChanged(this);
	}
}

void gdeObjectClass::SetPartialHideTags(const decStringSet &tags){
	if(pPartialHideTags == tags){
		return;
	}
	
	pPartialHideTags = tags;
	
	if(pGameDefinition){
		pGameDefinition->NotifyObjectClassChanged(this);
	}
}




void gdeObjectClass::NotifyPropertyChanged(gdeProperty *property){
	if(pGameDefinition){
		pGameDefinition->NotifyOCPropertyChanged(this, property);
	}
}

void gdeObjectClass::NotifyPropertyNameChanged(gdeProperty *property){
	if(pGameDefinition){
		pGameDefinition->NotifyOCPropertyNameChanged(this, property);
	}
}

void gdeObjectClass::NotifyPropertiesChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCPropertiesChanged(this);
	}
}

void gdeObjectClass::NotifyPropertyValuesChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCPropertyValuesChanged(this);
	}
}

void gdeObjectClass::NotifyTexturePropertyChanged(gdeProperty *property){
	if(pGameDefinition){
		pGameDefinition->NotifyOCTexturePropertyChanged(this, property);
	}
}

void gdeObjectClass::NotifyTexturePropertyNameChanged(gdeProperty *property){
	if(pGameDefinition){
		pGameDefinition->NotifyOCTexturePropertyNameChanged(this, property);
	}
}

void gdeObjectClass::NotifyTexturePropertiesChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCTexturePropertiesChanged(this);
	}
}

void gdeObjectClass::NotifyInheritChanged(gdeOCInherit *inherit){
	if(pGameDefinition){
		pGameDefinition->NotifyOCInheritChanged(this, inherit);
	}
}

void gdeObjectClass::NotifyInheritsChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCInheritsChanged(this);
	}
}

void gdeObjectClass::SetDefaultInheritPropertyPrefix(const char *propertyName){
	if(pDefaultInheritPropertyPrefix == propertyName){
		return;
	}
	
	pDefaultInheritPropertyPrefix = propertyName;
	
	if(pGameDefinition){
		pGameDefinition->NotifyObjectClassChanged(this);
	}
}

bool gdeObjectClass::InheritsFrom(const gdeObjectClass *objectClass) const{
	const int count = pInherits.GetCount();
	int i;
	for(i=0; i<count; i++){
		const gdeOCInherit &inherit = *pInherits.GetAt(i);
		const gdeObjectClass * const inheritOC = pGameDefinition->FindObjectClass(inherit.GetName());
		if(inheritOC && inheritOC->IsOrInheritsFrom(objectClass)){
			return true;
		}
	}
	return false;
}

bool gdeObjectClass::IsOrInheritsFrom(const gdeObjectClass *objectClass) const{
	return this == objectClass || InheritsFrom(objectClass);
}

bool gdeObjectClass::IsOrInheritsFrom(const char *name) const{
	const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass(name);
	return objectClass && IsOrInheritsFrom(objectClass);
}


void gdeObjectClass::NotifyBillboardsChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCBillboardsChanged(this);
	}
}

void gdeObjectClass::NotifyBillboardChanged(gdeOCBillboard *billboard){
	if(pGameDefinition){
		pGameDefinition->NotifyOCBillboardChanged(this, billboard);
	}
}

void gdeObjectClass::NotifyCamerasChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCCamerasChanged(this);
	}
}

void gdeObjectClass::NotifyCameraChanged(gdeOCCamera *camera){
	if(pGameDefinition){
		pGameDefinition->NotifyOCCameraChanged(this, camera);
	}
}

void gdeObjectClass::NotifyComponentsChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCComponentsChanged(this);
	}
}

void gdeObjectClass::NotifyComponentChanged(gdeOCComponent *component){
	if(pGameDefinition){
		pGameDefinition->NotifyOCComponentChanged(this, component);
	}
}

void gdeObjectClass::NotifyComponentTextureChanged(gdeOCComponent *component,
gdeOCComponentTexture *texture){
	if(pGameDefinition){
		pGameDefinition->NotifyOCComponentTextureChanged(this, component, texture);
	}
}

void gdeObjectClass::NotifyComponentTextureNameChanged(gdeOCComponent *component,
gdeOCComponentTexture *texture){
	if(pGameDefinition){
		pGameDefinition->NotifyOCComponentTextureNameChanged(this, component, texture);
	}
}

void gdeObjectClass::NotifyComponentTexturePropertiesChanged(gdeOCComponent *component,
gdeOCComponentTexture *texture){
	if(pGameDefinition){
		pGameDefinition->NotifyOCComponentTexturePropertiesChanged(this, component, texture);
	}
}

void gdeObjectClass::NotifyEnvMapProbesChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCEnvMapProbesChanged(this);
	}
}

void gdeObjectClass::NotifyEnvMapProbeChanged(gdeOCEnvMapProbe *envMapProbe){
	if(pGameDefinition){
		pGameDefinition->NotifyOCEnvMapProbeChanged(this, envMapProbe);
	}
}

void gdeObjectClass::NotifyLightsChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCLightsChanged(this);
	}
}

void gdeObjectClass::NotifyLightChanged(gdeOCLight *light){
	if(pGameDefinition){
		pGameDefinition->NotifyOCLightChanged(this, light);
	}
}

void gdeObjectClass::NotifyNavigationBlockersChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCNavigationBlockersChanged(this);
	}
}

void gdeObjectClass::NotifyNavigationBlockerChanged(gdeOCNavigationBlocker *navblocker){
	if(pGameDefinition){
		pGameDefinition->NotifyOCNavigationBlockerChanged(this, navblocker);
	}
}

void gdeObjectClass::NotifyNavigationSpacesChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCNavigationSpacesChanged(this);
	}
}

void gdeObjectClass::NotifyNavigationSpaceChanged(gdeOCNavigationSpace *navspace){
	if(pGameDefinition){
		pGameDefinition->NotifyOCNavigationSpaceChanged(this, navspace);
	}
}

void gdeObjectClass::NotifyParticleEmittersChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCParticleEmittersChanged(this);
	}
}

void gdeObjectClass::NotifyParticleEmitterChanged(gdeOCParticleEmitter *emitter){
	if(pGameDefinition){
		pGameDefinition->NotifyOCParticleEmitterChanged(this, emitter);
	}
}

void gdeObjectClass::NotifyForceFieldsChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCForceFieldsChanged(this);
	}
}

void gdeObjectClass::NotifyForceFieldChanged(gdeOCForceField *field){
	if(pGameDefinition){
		pGameDefinition->NotifyOCForceFieldChanged(this, field);
	}
}

void gdeObjectClass::NotifySnapPointsChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCSnapPointsChanged(this);
	}
}

void gdeObjectClass::NotifySnapPointChanged(gdeOCSnapPoint *snappoint){
	if(pGameDefinition){
		pGameDefinition->NotifyOCSnapPointChanged(this, snappoint);
	}
}

void gdeObjectClass::NotifySnapPointNameChanged(gdeOCSnapPoint *snappoint){
	if(pGameDefinition){
		pGameDefinition->NotifyOCSnapPointNameChanged(this, snappoint);
	}
}

void gdeObjectClass::NotifySpeakersChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCSpeakersChanged(this);
	}
}

void gdeObjectClass::NotifySpeakerChanged(gdeOCSpeaker *speaker){
	if(pGameDefinition){
		pGameDefinition->NotifyOCSpeakerChanged(this, speaker);
	}
}

void gdeObjectClass::NotifyWorldsChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCWorldsChanged(this);
	}
}

void gdeObjectClass::NotifyWorldChanged(gdeOCWorld *world){
	if(pGameDefinition){
		pGameDefinition->NotifyOCWorldChanged(this, world);
	}
}



void gdeObjectClass::SetActiveTexture(gdeOCComponentTexture *texture){
	pActiveTexture = texture;
}

void gdeObjectClass::NotifyTexturesChanged(){
	if(pGameDefinition){
		pGameDefinition->NotifyOCTexturesChanged(this);
	}
}

void gdeObjectClass::NotifyTextureChanged(gdeOCComponentTexture *texture){
	if(pGameDefinition){
		pGameDefinition->NotifyOCTextureChanged(this, texture);
	}
}



void gdeObjectClass::SetIsGhost(bool isGhost){
	if(pIsGhost == isGhost){
		return;
	}
	
	pIsGhost = isGhost;
	
	if(pGameDefinition){
		pGameDefinition->NotifyObjectClassChanged(this);
	}
}

void gdeObjectClass::SetCanInstantiate(bool canInstantiate){
	if(pCanInstantiate == canInstantiate){
		return;
	}
	
	pCanInstantiate = canInstantiate;
	
	if(pGameDefinition){
		pGameDefinition->NotifyObjectClassChanged(this);
	}
}

void gdeObjectClass::SetInheritSubObjects(int filter){
	if(pInheritSubObjects == filter){
		return;
	}
	
	pInheritSubObjects = filter;
	
	if(pGameDefinition){
		pGameDefinition->NotifyObjectClassChanged(this);
	}
}

void gdeObjectClass::SetIsAttachableBehavior(bool isAttachableBehavior){
	if(pIsAttachableBehavior == isAttachableBehavior){
		return;
	}
	
	pIsAttachableBehavior = isAttachableBehavior;
	
	if(pGameDefinition){
		pGameDefinition->NotifyObjectClassChanged(this);
	}
}


bool gdeObjectClass::DeepGetNamedProperty(const char *name, const gdeObjectClass* &objectClass,
const gdeProperty* &property) const{
	property = pProperties.FindOrDefault([&](const gdeProperty::Ref &p){
		return p->GetName() == name;
	});
	if(property){
		objectClass = this;
		return true;
	}
	
	if(!pGameDefinition){
		return false;
	}
	
	const int nameLen = (int)strlen(name);
	const int count = pInherits.GetCount();
	int i;
	for(i=0; i<count; i++){
		const gdeOCInherit &inherit = *pInherits.GetAt(i);
		const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass(inherit.GetName());
		if(!ioc){
			continue;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if(nameLen < prefixLen || strncmp(name, prefix, prefixLen) != 0){
			continue;
		}
		
		if(ioc->DeepGetNamedProperty(name + prefixLen, objectClass, property)){
			return true;
		}
	}
	
	return false;
}

bool gdeObjectClass::NamedPropertyDefaultValue(const char *name, decString &value) const{
	const decString *propertyValue = nullptr;
	if(pPropertyValues.GetAt(name, propertyValue)){
		value = *propertyValue;
		return true;
	}
	
	const gdeProperty * const property = pProperties.FindOrDefault([&](const gdeProperty::Ref &p){
		return p->GetName() == name;
	});
	if(property){
		value = property->GetDefaultValue();
		return true;
	}
	
	const int nameLen = (int)strlen(name);
	const int count = pInherits.GetCount();
	int i;
	for(i=0; i<count; i++){
		const gdeOCInherit &inherit = *pInherits.GetAt(i);
		const gdeObjectClass * const ioc = pGameDefinition->FindObjectClass(inherit.GetName());
		if(!ioc){
			continue;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if(nameLen < prefixLen || strncmp(name, prefix, prefixLen) != 0){
			continue;
		}
		
		if(ioc->NamedPropertyDefaultValue(name + prefixLen, value)){
			return true;
		}
	}
	
	return false;
}

void gdeObjectClass::AddPropertyNamesTo(decStringSet &set, bool inherited) const{
	const int propertyCount = pProperties.GetCount();
	int i;
	for(i=0; i<propertyCount; i++){
		set.Add(pProperties.GetAt(i)->GetName());
	}
	
	if(!pGameDefinition || !inherited){
		return;
	}
	
	const int inheritCount = pInherits.GetCount();
	for(i=0; i<inheritCount; i++){
		const gdeOCInherit &inherit = *pInherits.GetAt(i);
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass(inherit.GetName());
		if(objectClass){
			objectClass->pAddPropertyNames(set, inherit.GetPropertyPrefix());
		}
	}
}

void gdeObjectClass::AddTexturePropertyNamesTo(decStringSet &set, bool inherited) const{
	const int propertyCount = pTextureProperties.GetCount();
	int i;
	for(i=0; i<propertyCount; i++){
		set.Add(pTextureProperties.GetAt(i)->GetName());
	}
	
	if(!pGameDefinition || !inherited){
		return;
	}
	
	const int inheritCount = pInherits.GetCount();
	for(i=0; i<inheritCount; i++){
		const gdeOCInherit &inherit = *pInherits.GetAt(i);
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass(inherit.GetName());
		if(objectClass){
			objectClass->pAddTexturePropertyNames(set, inherit.GetPropertyPrefix());
		}
	}
}

void gdeObjectClass::AddPropertiesTo(decStringDictionary &properties, bool inherited, bool includeValues) const{
	if(inherited){
		const int inheritCount = pInherits.GetCount();
		int i;
		for(i=0; i<inheritCount; i++){
			const gdeOCInherit &inherit = *pInherits.GetAt(i);
			const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass(inherit.GetName());
			if(objectClass){
				objectClass->pAddPropertiesTo(properties, inherit.GetPropertyPrefix());
			}
		}
	}
	
	const int propertyCount = pProperties.GetCount();
	int i;
	for(i=0; i<propertyCount; i++){
		const gdeProperty &property = *pProperties.GetAt(i);
		properties.SetAt(property.GetName(), property.GetDefaultValue());
	}
	
	if(includeValues){
		properties += pPropertyValues;
	}
}

void gdeObjectClass::GetDefinedUsedIDs(decStringSet &definedIDs, decStringSet &usedIDs) const{
	const int inheritCount = pInherits.GetCount();
	int i;
	for(i=0; i<inheritCount; i++){
		const gdeOCInherit &inherit = *pInherits.GetAt(i);
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass(inherit.GetName());
		if(objectClass){
			objectClass->GetDefinedUsedIDs(definedIDs, usedIDs);
		}
	}
	
	const int propertyCount = pProperties.GetCount();
	for(i=0; i<propertyCount; i++){
		const gdeProperty &property = *pProperties.GetAt(i);
		if(property.GetType() != gdeProperty::eptIdentifier || property.GetIdentifierGroup().IsEmpty()){
			continue;
		}
		
		if(property.GetIdentifierUsage()){
			definedIDs.Add(property.GetIdentifierGroup());
			
		}else{
			usedIDs.Add(property.GetIdentifierGroup());
		}
	}
}



// Private functions
//////////////////////

void gdeObjectClass::pCleanUp(){
	SetGameDefinition(nullptr);
}

void gdeObjectClass::pAddPropertyNames(decStringSet &set, const decString &prefix) const{
	const int propertyCount = pProperties.GetCount();
	int i;
	for(i=0; i<propertyCount; i++){
		set.Add(prefix + pProperties.GetAt(i)->GetName());
	}
	
	const int inheritCount = pInherits.GetCount();
	for(i=0; i<inheritCount; i++){
		const gdeOCInherit &inherit = *pInherits.GetAt(i);
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass(inherit.GetName());
		if(objectClass){
			objectClass->pAddPropertyNames(set, prefix + inherit.GetPropertyPrefix());
		}
	}
}

void gdeObjectClass::pAddTexturePropertyNames(decStringSet &set, const decString &prefix) const{
	const int propertyCount = pTextureProperties.GetCount();
	int i;
	for(i=0; i<propertyCount; i++){
		set.Add(prefix + pTextureProperties.GetAt(i)->GetName());
	}
	
	const int inheritCount = pInherits.GetCount();
	for(i=0; i<inheritCount; i++){
		const gdeOCInherit &inherit = *pInherits.GetAt(i);
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass(inherit.GetName());
		if(objectClass){
			objectClass->pAddTexturePropertyNames(set, prefix + inherit.GetPropertyPrefix());
		}
	}
}

void gdeObjectClass::pAddPropertiesTo(decStringDictionary &properties, const decString &prefix) const{
	const int inheritCount = pInherits.GetCount();
	int i;
	for(i=0; i<inheritCount; i++){
		const gdeOCInherit &inherit = *pInherits.GetAt(i);
		const gdeObjectClass * const objectClass = pGameDefinition->FindObjectClass(inherit.GetName());
		if(objectClass){
			objectClass->pAddPropertiesTo(properties, prefix + inherit.GetPropertyPrefix());
		}
	}
	
	const int propertyCount = pProperties.GetCount();
	for(i=0; i<propertyCount; i++){
		const gdeProperty &property = *pProperties.GetAt(i);
		properties.SetAt(prefix + property.GetName(), property.GetDefaultValue());
	}
	
	const decStringList keys(pPropertyValues.GetKeys());
	const int keyCount = keys.GetCount();
	for(i=0; i<keyCount; i++){
		const decString &key = keys.GetAt(i);
		properties.SetAt(prefix + key, pPropertyValues.GetAt(key));
	}
}
