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

#include "igdeGDClass.h"
#include "igdeGDClassInherit.h"
#include "igdeGDClassManager.h"
#include "igdeGDCamera.h"
#include "billboard/igdeGDCBillboard.h"
#include "component/igdeGDCComponent.h"
#include "component/igdeGDCCTexture.h"
#include "snappoint/igdeGDCSnapPoint.h"
#include "particleemitter/igdeGDCParticleEmitter.h"
#include "forcefield/igdeGDCForceField.h"
#include "envmapprobe/igdeGDCEnvMapProbe.h"
#include "light/igdeGDCLight.h"
#include "speaker/igdeGDCSpeaker.h"
#include "navspace/igdeGDCNavigationSpace.h"
#include "navblocker/igdeGDCNavigationBlocker.h"
#include "world/igdeGDCWorld.h"
#include "../igdeTagManager.h"
#include "../property/igdeGDProperty.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/resources/image/deImage.h>



// Class igdeGDClass
//////////////////////

// Constructor, destructor
////////////////////////////

igdeGDClass::igdeGDClass(const char *name){
	if(!name || strlen(name) == 0){
		DETHROW(deeInvalidParam);
	}
	
	pName = name;
	pScaleMode = esmUniform;
	
	pCamera = nullptr;
	pHasCamera = false;
	pIsGhost = false;
	pCanInstantiate = true;
	pIsAttachableBehavior = false;
	pInheritSubObjects = FilterSubObjectsAll;
	
	try{
		pCamera = new igdeGDCamera;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

igdeGDClass::igdeGDClass(const igdeGDClass &gdclass){
	pCamera = nullptr;
	
	try{
		pName = gdclass.pName;
		pDescription = gdclass.pDescription;
		pScaleMode = gdclass.pScaleMode;
		
		if(gdclass.pCamera){
			pCamera = new igdeGDCamera(*gdclass.pCamera);
		}
		pHasCamera = gdclass.pHasCamera;
		pIsGhost = gdclass.pIsGhost;
		pCanInstantiate = gdclass.pCanInstantiate;
		pIsAttachableBehavior = gdclass.pIsAttachableBehavior;
		pInheritSubObjects = gdclass.pInheritSubObjects;
		
		pCategory = gdclass.pCategory;
		
		gdclass.pListProperties.Visit([&](const igdeGDProperty &p){
			pListProperties.Add(igdeGDProperty::Ref::New(p));
		});
		
		pPropertyValues = gdclass.pPropertyValues;
		
		gdclass.pTextureProperties.Visit([&](const igdeGDProperty &p){
			pTextureProperties.Add(igdeGDProperty::Ref::New(p));
		});
		
		gdclass.pListBillboards.Visit([&](const igdeGDCBillboard &s){
			pListBillboards.Add(igdeGDCBillboard::Ref::New(s));
		});
		
		gdclass.pListComponents.Visit([&](const igdeGDCComponent &s){
			pListComponents.Add(igdeGDCComponent::Ref::New(s));
		});
		
		gdclass.pListLights.Visit([&](const igdeGDCLight &s){
			pListLights.Add(igdeGDCLight::Ref::New(s));
		});
		
		gdclass.pSnapPoints.Visit([&](const igdeGDCSnapPoint &s){
			pSnapPoints.Add(igdeGDCSnapPoint::Ref::New(s));
		});
		
		gdclass.pListParticleEmitters.Visit([&](const igdeGDCParticleEmitter &s){
			pListParticleEmitters.Add(igdeGDCParticleEmitter::Ref::New(s));
		});
		
		gdclass.pListForceFields.Visit([&](const igdeGDCForceField &s){
			pListForceFields.Add(igdeGDCForceField::Ref::New(s));
		});
		
		gdclass.pListEnvMapProbes.Visit([&](const igdeGDCEnvMapProbe &s){
			pListEnvMapProbes.Add(igdeGDCEnvMapProbe::Ref::New(s));
		});
		
		gdclass.pListSpeakers.Visit([&](const igdeGDCSpeaker &s){
			pListSpeakers.Add(igdeGDCSpeaker::Ref::New(s));
		});
		
		gdclass.pListNavigationSpaces.Visit([&](const igdeGDCNavigationSpace &s){
			pListNavigationSpaces.Add(igdeGDCNavigationSpace::Ref::New(s));
		});
		
		gdclass.pListNavigationBlockers.Visit([&](const igdeGDCNavigationBlocker &s){
			pListNavigationBlockers.Add(igdeGDCNavigationBlocker::Ref::New(s));
		});
		
		gdclass.pListWorlds.Visit([&](const igdeGDCWorld &s){
			pListWorlds.Add(igdeGDCWorld::Ref::New(s));
		});
		
		pHideTags = gdclass.pHideTags;
		pPartialHideTags = gdclass.pPartialHideTags;
		
		gdclass.pInheritClasses.Visit([&](const igdeGDClassInherit &s){
			pInheritClasses.Add(igdeGDClassInherit::Ref::New(s));
		});
		
		pDefaultInheritPropertyPrefix = gdclass.pDefaultInheritPropertyPrefix;
		pPathEClass = gdclass.pPathEClass;
		gdclass.pComponentTextures.Visit([&](const igdeGDCCTexture &s){
			pComponentTextures.Add(igdeGDCCTexture::Ref::New(s));
		});
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

igdeGDClass::~igdeGDClass(){
	pCleanUp();
}



// Management
///////////////

void igdeGDClass::SetDescription(const char *description){
	pDescription = description;
}

void igdeGDClass::SetScaleMode(eScaleModes mode){
	pScaleMode = mode;
}

void igdeGDClass::SetHasCamera(bool hasCamera){
	pHasCamera = hasCamera;
}

void igdeGDClass::SetIsGhost(bool isGhost){
	pIsGhost = isGhost;
}

void igdeGDClass::SetCanInstantiate(bool canInstantiate){
	pCanInstantiate = canInstantiate;
}

void igdeGDClass::SetIsAttachableBehavior(bool isAttachableBehavior){
	pIsAttachableBehavior = isAttachableBehavior;
}

void igdeGDClass::SetCategory(const char *category){
	if(!category){
		DETHROW(deeInvalidParam);
	}
	
	pCategory = category;
}

void igdeGDClass::SetInheritSubObjects(int filter){
	pInheritSubObjects = filter;
}



void igdeGDClass::AddInheritClass(igdeGDClassInherit *inheritClass){
	DEASSERT_NOTNULL(inheritClass)
	pInheritClasses.AddOrThrow(inheritClass);
}

void igdeGDClass::ResolveInheritClasses(const igdeGDClassManager &classManager){
	pInheritClasses.Visit([&](igdeGDClassInherit &inherit){
		inherit.ResolveClass(classManager);
		inherit.AddAutoPrefixedPropertiesTo(*this);
	});
}

void igdeGDClass::RemoveAllInheritClasses(){
	pInheritClasses.RemoveAll();
}

bool igdeGDClass::Inherits(igdeGDClass *gdclass) const{
	if(!gdclass || gdclass == this){
		return false;
	}
	
	return pInheritClasses.HasMatching([&](const igdeGDClassInherit &inherit){
		igdeGDClass * const inheritClass = inherit.GetClass();
		return inheritClass == gdclass || inheritClass->Inherits(gdclass);
	});
}



void igdeGDClass::Check(){
}



void igdeGDClass::SetPreviewImage(deImage *image){
	pPreviewImage = image;
}

void igdeGDClass::SetDefaultInheritPropertyPrefix(const decString &prefix){
	pDefaultInheritPropertyPrefix = prefix;
}

void igdeGDClass::SetPathEClass(const decString &pathEClass){
	pPathEClass = pathEClass;
}

void igdeGDClass::GetDeepComponentTextures(igdeGDCCTexture::List &list) const{
	pComponentTextures.Visit([&](const igdeGDCCTexture &t){
		const decString &name = t.GetName();
		if(!list.HasNamed(name)){
			list.Add(igdeGDCCTexture::Ref::New(t));
		}
	});
	
	pInheritClasses.Visit([&](const igdeGDClassInherit &inherit){
		if(inherit.GetClass()){
			inherit.GetClass()->GetDeepComponentTextures(list);
		}
	});
}



// Properties
///////////////

void igdeGDClass::AddProperty(igdeGDProperty *property){
	DEASSERT_NOTNULL(property)
	pListProperties.AddOrThrow(property);
}

void igdeGDClass::RemoveProperty(igdeGDProperty *property){
	pListProperties.RemoveOrThrow(property);
}

void igdeGDClass::RemoveAllProperties(){
	pListProperties.RemoveAll();
}

void igdeGDClass::SetPropertyValues(const decStringDictionary &values){
	pPropertyValues = values;
}

igdeGDProperty *igdeGDClass::GetPropertyNamed(const char *name) const{
	const igdeGDClass *gdclass = this;
	igdeGDProperty::Ref property(gdclass->pListProperties.FindNamed(name));
	if(property){
		return property;
	}
	
	const int nameLen = (int)strlen(name);
	igdeGDProperty *result = nullptr;
	pInheritClasses.HasMatching([&](const igdeGDClassInherit &inherit){
		if(result || !inherit.GetClass()){
			return false;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if(nameLen < prefixLen || strncmp(name, prefix, prefixLen) != 0){
			return false;
		}
		
		result = inherit.GetClass()->GetPropertyNamed(name + prefixLen);
		return result != nullptr;
	});
	return result;
}

bool igdeGDClass::GetDefaultPropertyValue(const char *name, decString &value) const{
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	const decString *checkValue;
	if(pPropertyValues.GetAt(name, checkValue)){
		value = *checkValue;
		return true;
	}
	
	const igdeGDProperty::Ref property(pListProperties.FindNamed(name));
	if(property){
		value = property->GetDefaultValue();
		return true;
	}
	
	const int nameLen = (int)strlen(name);
	return pInheritClasses.HasMatching([&](const igdeGDClassInherit &inherit){
		if(!inherit.GetClass()){
			return false;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if(nameLen < prefixLen || strncmp(name, prefix, prefixLen) != 0){
			return false;
		}
		
		return inherit.GetClass()->GetDefaultPropertyValue(name + prefixLen, value);
	});
}

bool igdeGDClass::HasDefaultPropertyValue(const char *name) const{
	decString value;
	return GetDefaultPropertyValue(name, value);
}

void igdeGDClass::AddPropertyNames(decStringSet &set, bool inherited) const{
	pListProperties.Visit([&](const igdeGDProperty &p){
		set.Add(p.GetName());
	});
	
	if(!inherited){
		return;
	}
	
	pInheritClasses.Visit([&](const igdeGDClassInherit &inherit){
		if(inherit.GetClass()){
			inherit.GetClass()->pAddPropertyNames(set, inherit.GetPropertyPrefix());
		}
	});
}



// Texture properties
///////////////////////

void igdeGDClass::SetTextureProperties(const igdeGDProperty::List &properties){
	pTextureProperties = properties;
}

void igdeGDClass::AddTextureProperty(igdeGDProperty *property){
	DEASSERT_NOTNULL(property)
	pTextureProperties.AddOrThrow(property);
}

void igdeGDClass::RemoveTextureProperty(igdeGDProperty *property){
	pTextureProperties.RemoveOrThrow(property);
}

void igdeGDClass::RemoveAllTextureProperties(){
	pTextureProperties.RemoveAll();
}

igdeGDProperty *igdeGDClass::GetTexturePropertyNamed(const char *name) const{
	const igdeGDClass *gdclass = this;
	igdeGDProperty::Ref property(gdclass->pTextureProperties.FindNamed(name));
	if(property){
		return property;
	}
	
	const int nameLen = (int)strlen(name);
	
	pInheritClasses.HasMatching([&](const igdeGDClassInherit &inherit){
		if(!inherit.GetClass()){
			return false;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if(nameLen < prefixLen || strncmp(name, prefix, prefixLen) != 0){
			return false;
		}
		
		property = inherit.GetClass()->GetTexturePropertyNamed(name + prefixLen);
		return property != nullptr;
	});
	
	return property;
}

bool igdeGDClass::GetDefaultTexturePropertyValue(const char *name, decString &value) const{
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	const igdeGDProperty::Ref property(pTextureProperties.FindNamed(name));
	if(property){
		value = property->GetDefaultValue();
		return true;
	}
	
	const int nameLen = (int)strlen(name);
	return pInheritClasses.HasMatching([&](const igdeGDClassInherit &inherit){
		if(!inherit.GetClass()){
			return false;
		}
		
		const decString &prefix = inherit.GetPropertyPrefix();
		const int prefixLen = prefix.GetLength();
		if(nameLen < prefixLen || strncmp(name, prefix, prefixLen) != 0){
			return false;
		}
		
		return inherit.GetClass()->GetDefaultTexturePropertyValue(name + prefixLen, value);
	});
}

bool igdeGDClass::HasDefaultTexturePropertyValue(const char *name) const{
	decString value;
	return GetDefaultTexturePropertyValue(name, value);
}

void igdeGDClass::AddTexturePropertyNames(decStringSet &set, bool inherited) const{
	const int propertyCount = pTextureProperties.GetCount();
	int i;
	for(i=0; i<propertyCount; i++){
		set.Add(pTextureProperties.GetAt(i)->GetName());
	}
	
	if(!inherited){
		return;
	}
	
	pInheritClasses.Visit([&](const igdeGDClassInherit &inherit){
		if(inherit.GetClass()){
			inherit.GetClass()->pAddTexturePropertyNames(set, inherit.GetPropertyPrefix());
		}
	});
}



// Components
///////////////

void igdeGDClass::AddComponent(igdeGDCComponent *component){
	DEASSERT_NOTNULL(component)
	pListComponents.AddOrThrow(component);
}

void igdeGDClass::RemoveComponent(igdeGDCComponent *component){
	pListComponents.RemoveOrThrow(component);
}

void igdeGDClass::RemoveAllComponents(){
	pListComponents.RemoveAll();
}

bool igdeGDClass::HasComponentLinkedProperty(const char *name) const{
	const int count = pListComponents.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(pListComponents.GetAt(i)->HasPropertyWithName(name)){
			return true;
		}
	}
	
	return false;
}
const igdeGDCComponent::List igdeGDClass::GetComponentsWithLinkedProperty(const char *name) const{
	return pListComponents.Collect([&](const igdeGDCComponent &c){
		return c.HasPropertyWithName(name);
	});
}

const igdeGDClass::IndexList igdeGDClass::GetComponentIndicesWithLinkedProperty(const char *name) const{
	const int count = pListComponents.GetCount();
	IndexList list;
	int i;
	
	for(i=0; i<count; i++){
		if(pListComponents.GetAt(i)->HasPropertyWithName(name)){
			list.Add(i);
		}
	}
	
	return list;
}



// Lights
///////////

void igdeGDClass::AddLight(igdeGDCLight *light){
	DEASSERT_NOTNULL(light)
	pListLights.AddOrThrow(light);
}

void igdeGDClass::RemoveLight(igdeGDCLight *light){
	pListLights.RemoveOrThrow(light);
	// notify;
}

void igdeGDClass::RemoveAllLights(){
	pListLights.RemoveAll();
	// notify;
}

bool igdeGDClass::HasLightLinkedProperty(const char *name) const{
	const int count = pListLights.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(pListLights.GetAt(i)->HasPropertyWithName(name)){
			return true;
		}
	}
	
	return false;
}
const igdeGDCLight::List igdeGDClass::GetLightWithLinkedProperty(const char *name) const{
	return pListLights.Collect([&](const igdeGDCLight &c){
		return c.HasPropertyWithName(name);
	});
}

const igdeGDClass::IndexList igdeGDClass::GetLightIndicesWithLinkedProperty(const char *name) const{
	const int count = pListLights.GetCount();
	IndexList list;
	int i;
	
	for(i=0; i<count; i++){
		if(pListLights.GetAt(i)->HasPropertyWithName(name)){
			list.Add(i);
		}
	}
	
	return list;
}



// Billboards
///////////

void igdeGDClass::AddBillboard(igdeGDCBillboard *billboard){
	DEASSERT_NOTNULL(billboard)
	pListBillboards.AddOrThrow(billboard);
}

void igdeGDClass::RemoveBillboard(igdeGDCBillboard *billboard){
	pListBillboards.RemoveOrThrow(billboard);
	// notify;
}

void igdeGDClass::RemoveAllBillboards(){
	pListBillboards.RemoveAll();
	// notify;
}

bool igdeGDClass::HasBillboardLinkedProperty(const char *name) const{
	const int count = pListBillboards.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(pListBillboards.GetAt(i)->HasPropertyWithName(name)){
			return true;
		}
	}
	
	return false;
}
const igdeGDCBillboard::List igdeGDClass::GetBillboardWithLinkedProperty(const char *name) const{
	return pListBillboards.Collect([&](const igdeGDCBillboard &b){
		return b.HasPropertyWithName(name);
	});
}

const igdeGDClass::IndexList igdeGDClass::GetBillboardIndicesWithLinkedProperty(const char *name) const{
	const int count = pListBillboards.GetCount();
	IndexList list;
	int i;
	
	for(i=0; i<count; i++){
		if(pListBillboards.GetAt(i)->HasPropertyWithName(name)){
			list.Add(i);
		}
	}
	
	return list;
}



// Speakers
/////////////

void igdeGDClass::AddSpeaker(igdeGDCSpeaker *speaker){
	DEASSERT_NOTNULL(speaker)
	pListSpeakers.AddOrThrow(speaker);
}

void igdeGDClass::RemoveSpeaker(igdeGDCSpeaker *speaker){
	pListSpeakers.RemoveOrThrow(speaker);
}

void igdeGDClass::RemoveAllSpeakers(){
	pListSpeakers.RemoveAll();
}

bool igdeGDClass::HasSpeakerLinkedProperty(const char *name) const{
	const int count = pListSpeakers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(pListSpeakers.GetAt(i)->HasPropertyWithName(name)){
			return true;
		}
	}
	
	return false;
}
const igdeGDCSpeaker::List igdeGDClass::GetSpeakerWithLinkedProperty(const char *name) const{
	return pListSpeakers.Collect([&](const igdeGDCSpeaker &s){
		return s.HasPropertyWithName(name);
	});
}

const igdeGDClass::IndexList igdeGDClass::GetSpeakerIndicesWithLinkedProperty(const char *name) const{
	const int count = pListSpeakers.GetCount();
	IndexList list;
	int i;
	
	for(i=0; i<count; i++){
		if(pListSpeakers.GetAt(i)->HasPropertyWithName(name)){
			list.Add(i);
		}
	}
	
	return list;
}



// Navigation spaces
//////////////////////

void igdeGDClass::AddNavigationSpace(igdeGDCNavigationSpace *navSpace){
	DEASSERT_NOTNULL(navSpace)
	pListNavigationSpaces.AddOrThrow(navSpace);
}

void igdeGDClass::RemoveNavigationSpace(igdeGDCNavigationSpace *navSpace){
	pListNavigationSpaces.RemoveOrThrow(navSpace);
}

void igdeGDClass::RemoveAllNavigationSpaces(){
	pListNavigationSpaces.RemoveAll();
}

bool igdeGDClass::HasNavSpaceLinkedProperty(const char *name) const{
	const int count = pListNavigationSpaces.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(pListNavigationSpaces.GetAt(i)->HasPropertyWithName(name)){
			return true;
		}
	}
	
	return false;
}
const igdeGDCNavigationSpace::List igdeGDClass::GetNavSpaceWithLinkedProperty(const char *name) const{
	return pListNavigationSpaces.Collect([&](const igdeGDCNavigationSpace &ns){
		return ns.HasPropertyWithName(name);
	});
}

const igdeGDClass::IndexList igdeGDClass::GetNavSpaceIndicesWithLinkedProperty(const char *name) const{
	const int count = pListNavigationSpaces.GetCount();
	IndexList list;
	int i;
	
	for(i=0; i<count; i++){
		if(pListNavigationSpaces.GetAt(i)->HasPropertyWithName(name)){
			list.Add(i);
		}
	}
	
	return list;
}



// Navigation blockers
//////////////////////

void igdeGDClass::AddNavigationBlocker(igdeGDCNavigationBlocker *blocker){
	DEASSERT_NOTNULL(blocker)
	pListNavigationBlockers.AddOrThrow(blocker);
}

void igdeGDClass::RemoveNavigationBlocker(igdeGDCNavigationBlocker *blocker){
	pListNavigationBlockers.RemoveOrThrow(blocker);
}

void igdeGDClass::RemoveAllNavigationBlockers(){
	pListNavigationBlockers.RemoveAll();
}

bool igdeGDClass::HasNavBlockerLinkedProperty(const char *name) const{
	const int count = pListNavigationBlockers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(pListNavigationBlockers.GetAt(i)->HasPropertyWithName(name)){
			return true;
		}
	}
	
	return false;
}
const igdeGDCNavigationBlocker::List igdeGDClass::GetNavBlockerWithLinkedProperty(const char *name) const{
	return pListNavigationBlockers.Collect([&](const igdeGDCNavigationBlocker &nb){
		return nb.HasPropertyWithName(name);
	});
}

const igdeGDClass::IndexList igdeGDClass::GetNavBlockerIndicesWithLinkedProperty(const char *name) const{
	const int count = pListNavigationBlockers.GetCount();
	IndexList list;
	int i;
	
	for(i=0; i<count; i++){
		if(pListNavigationBlockers.GetAt(i)->HasPropertyWithName(name)){
			list.Add(i);
		}
	}
	
	return list;
}



// Worlds
///////////

void igdeGDClass::AddWorld(igdeGDCWorld *world){
	DEASSERT_NOTNULL(world)
	pListWorlds.AddOrThrow(world);
}

void igdeGDClass::RemoveWorld(igdeGDCWorld *world){
	pListWorlds.RemoveOrThrow(world);
}

void igdeGDClass::RemoveAllWorlds(){
	pListWorlds.RemoveAll();
}

bool igdeGDClass::HasWorldLinkedProperty(const char *name) const{
	const int count = pListWorlds.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(pListWorlds.GetAt(i)->HasPropertyWithName(name)){
			return true;
		}
	}
	
	return false;
}
const igdeGDCWorld::List igdeGDClass::GetWorldsWithLinkedProperty(const char *name) const{
	return pListWorlds.Collect([&](const igdeGDCWorld &w){
		return w.HasPropertyWithName(name);
	});
}

const igdeGDClass::IndexList igdeGDClass::GetWorldIndicesWithLinkedProperty(const char *name) const{
	const int count = pListWorlds.GetCount();
	IndexList list;
	int i;
	
	for(i=0; i<count; i++){
		if(pListWorlds.GetAt(i)->HasPropertyWithName(name)){
			list.Add(i);
		}
	}
	
	return list;
}




// SnapPoints
////////////

void igdeGDClass::AddSnapPoint(igdeGDCSnapPoint *snappoint){
	DEASSERT_NOTNULL(snappoint)
	pSnapPoints.AddOrThrow(snappoint);
}

void igdeGDClass::RemoveSnapPoint(igdeGDCSnapPoint *snappoint){
	pSnapPoints.RemoveOrThrow(snappoint);
}

void igdeGDClass::RemoveAllSnapPoints(){
	pSnapPoints.RemoveAll();
}



// Particle Emitters
//////////////////////

void igdeGDClass::AddParticleEmitter(igdeGDCParticleEmitter *emitter){
	DEASSERT_NOTNULL(emitter)
	pListParticleEmitters.AddOrThrow(emitter);
}

void igdeGDClass::RemoveParticleEmitter(igdeGDCParticleEmitter *emitter){
	pListParticleEmitters.RemoveOrThrow(emitter);
	// notify;
}

void igdeGDClass::RemoveAllParticleEmitters(){
	pListParticleEmitters.RemoveAll();
	// notify;
}



// Force Fields
/////////////////

void igdeGDClass::AddForceField(igdeGDCForceField *field){
	DEASSERT_NOTNULL(field)
	pListForceFields.AddOrThrow(field);
}

void igdeGDClass::RemoveForceField(igdeGDCForceField *field){
	pListForceFields.RemoveOrThrow(field);
	// notify;
}

void igdeGDClass::RemoveAllForceFields(){
	pListForceFields.RemoveAll();
	// notify;
}



// Environment Map Probes
///////////////////////////

void igdeGDClass::AddEnvironmentMapProbe(igdeGDCEnvMapProbe *probe){
	DEASSERT_NOTNULL(probe)
	pListEnvMapProbes.AddOrThrow(probe);
}

void igdeGDClass::RemoveEnvironmentMapProbe(igdeGDCEnvMapProbe *probe){
	pListEnvMapProbes.RemoveOrThrow(probe);
	// notify;
}

void igdeGDClass::RemoveAllEnvironmentMapProbes(){
	pListEnvMapProbes.RemoveAll();
	// notify;
}

bool igdeGDClass::HasEnvMapProbesLinkedProperty(const char *name) const{
	const int count = pListEnvMapProbes.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(pListEnvMapProbes.GetAt(i)->HasPropertyWithName(name)){
			return true;
		}
	}
	
	return false;
}
const igdeGDCEnvMapProbe::List igdeGDClass::GetEnvMapProbesWithLinkedProperty(const char *name) const{
	return pListEnvMapProbes.Collect([&](const igdeGDCEnvMapProbe &e){
		return e.HasPropertyWithName(name);
	});
}

const igdeGDClass::IndexList igdeGDClass::GetEnvMapProbesIndicesWithLinkedProperty(const char *name) const{
	const int count = pListEnvMapProbes.GetCount();
	IndexList list;
	int i;
	
	for(i=0; i<count; i++){
		if(pListEnvMapProbes.GetAt(i)->HasPropertyWithName(name)){
			list.Add(i);
		}
	}
	
	return list;
}



// Private Functions
//////////////////////

void igdeGDClass::pCleanUp(){
	RemoveAllSpeakers();
	RemoveAllNavigationBlockers();
	RemoveAllNavigationSpaces();
	RemoveAllEnvironmentMapProbes();
	RemoveAllParticleEmitters();
	RemoveAllForceFields();
	RemoveAllSnapPoints();
	RemoveAllLights();
	RemoveAllBillboards();
	RemoveAllComponents();
	RemoveAllWorlds();
	if(pCamera){
		delete pCamera;
	}
}
void igdeGDClass::pAddPropertyNames(decStringSet &set, const decString &prefix) const{
	pListProperties.Visit([&](const igdeGDProperty &p){
		set.Add(prefix + p.GetName());
	});
	
	pInheritClasses.Visit([&](const igdeGDClassInherit &inherit){
		if(inherit.GetClass()){
			inherit.GetClass()->pAddPropertyNames(set, prefix + inherit.GetPropertyPrefix());
		}
	});
}

void igdeGDClass::pAddTexturePropertyNames(decStringSet &set, const decString &prefix) const{
	pTextureProperties.Visit([&](const igdeGDProperty &p){
		set.Add(prefix + p.GetName());
	});
	
	pInheritClasses.Visit([&](const igdeGDClassInherit &inherit){
		if(inherit.GetClass()){
			inherit.GetClass()->pAddTexturePropertyNames(set, prefix + inherit.GetPropertyPrefix());
		}
	});
}
