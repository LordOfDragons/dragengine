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
#include <string.h>
#include <stdlib.h>

#include "gdeUOCPropertyValuesFromSubObjects.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/gdeObjectClass.h"
#include "../../gamedef/objectClass/component/gdeOCComponent.h"
#include "../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"
#include "../../gamedef/objectClass/light/gdeOCLight.h"
#include "../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"
#include "../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"
#include "../../gamedef/objectClass/speaker/gdeOCSpeaker.h"
#include "../../gamedef/objectClass/world/gdeOCWorld.h"
#include "../../gamedef/property/gdeProperty.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>

#include <deigde/codec/igdeCodecPropertyString.h>



static void UpdateComponent(decStringDictionary &values,
igdeCodecPropertyString codec, const gdeOCComponent &component){
	if(component.IsPropertySet(gdeOCComponent::epModel)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epModel),
			component.GetModelPath());
	}
	
	if(component.IsPropertySet(gdeOCComponent::epSkin)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epSkin),
			component.GetSkinPath());
	}
	
	if(component.IsPropertySet(gdeOCComponent::epRig)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epRig),
			component.GetRigPath());
	}
	
	if(component.IsPropertySet(gdeOCComponent::epAnimator)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epAnimator),
			component.GetAnimatorPath());
	}
	
	if(component.IsPropertySet(gdeOCComponent::epPlaybackController)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epPlaybackController),
			component.GetPlaybackController());
	}
	
	if(component.IsPropertySet(gdeOCComponent::epOcclusionMesh)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epOcclusionMesh),
			component.GetOcclusionMeshPath());
	}
	
	if(component.IsPropertySet(gdeOCComponent::epAudioModel)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epAudioModel),
			component.GetAudioModelPath());
	}
	
	if(component.IsPropertySet(gdeOCComponent::epRenderEnvMap)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epRenderEnvMap),
			component.GetRenderEnvMap() ? "1" : "0");
	}
	
	if(component.IsPropertySet(gdeOCComponent::epAffectsAudio)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epAffectsAudio),
			component.GetAffectsAudio() ? "1" : "0");
	}
	
	if(component.IsPropertySet(gdeOCComponent::epLightShadowIgnore)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epLightShadowIgnore),
			component.GetLightShadowIgnore() ? "1" : "0");
	}
	
	if(component.IsPropertySet(gdeOCComponent::epAttachPosition)){
		decString string;
		codec.EncodeVector(component.GetPosition(), string);
		values.SetAt(component.GetPropertyName(gdeOCComponent::epAttachPosition), string);
	}
	
	if(component.IsPropertySet(gdeOCComponent::epAttachRotation)){
		decString string;
		codec.EncodeVector(component.GetRotation(), string);
		values.SetAt(component.GetPropertyName(gdeOCComponent::epAttachRotation), string);
	}
	
	if(component.IsPropertySet(gdeOCComponent::epAnimation)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epAnimation),
			component.GetAnimationPath());
	}
	
	if(component.IsPropertySet(gdeOCComponent::epMove)){
		values.SetAt(component.GetPropertyName(gdeOCComponent::epMove),
			component.GetMove());
	}
}

static void UpdateEnvMapProbe(decStringDictionary &values,
igdeCodecPropertyString codec, const gdeOCEnvMapProbe &envMapProbe){
	decString string;
	
	if(envMapProbe.IsPropertySet(gdeOCEnvMapProbe::epInfluenceArea)){
		codec.EncodeShapeList(envMapProbe.GetShapeListInfluence(), string);
		values.SetAt(envMapProbe.GetPropertyName(gdeOCEnvMapProbe::epInfluenceArea), string);
	}
	
	if(envMapProbe.IsPropertySet(gdeOCEnvMapProbe::epInfluenceBorderSize)){
		string.Format("%.3g", envMapProbe.GetInfluenceBorderSize());
		values.SetAt(envMapProbe.GetPropertyName(gdeOCEnvMapProbe::epInfluenceBorderSize), string);
	}
	
	if(envMapProbe.IsPropertySet(gdeOCEnvMapProbe::epInfluencePriority)){
		string.Format("%d", envMapProbe.GetInfluencePriority());
		values.SetAt(envMapProbe.GetPropertyName(gdeOCEnvMapProbe::epInfluencePriority), string);
	}
	
	if(envMapProbe.IsPropertySet(gdeOCEnvMapProbe::epReflectionShape)){
		decShapeList list;
		if(envMapProbe.GetShapeReflection()){
			list.Add(envMapProbe.GetShapeReflection()->Copy());
		}
		codec.EncodeShapeList(list, string);
		values.SetAt(envMapProbe.GetPropertyName(gdeOCEnvMapProbe::epReflectionShape), string);
	}
	
	if(envMapProbe.IsPropertySet(gdeOCEnvMapProbe::epReflectionMask)){
		codec.EncodeShapeList(envMapProbe.GetShapeListReflectionMask(), string);
		values.SetAt(envMapProbe.GetPropertyName(gdeOCEnvMapProbe::epReflectionMask), string);
	}
	
	if(envMapProbe.IsPropertySet(gdeOCEnvMapProbe::epAttachPosition)){
		codec.EncodeVector(envMapProbe.GetPosition(), string);
		values.SetAt(envMapProbe.GetPropertyName(gdeOCEnvMapProbe::epAttachPosition), string);
	}
	
	if(envMapProbe.IsPropertySet(gdeOCEnvMapProbe::epAttachRotation)){
		codec.EncodeVector(envMapProbe.GetRotation(), string);
		values.SetAt(envMapProbe.GetPropertyName(gdeOCEnvMapProbe::epAttachRotation), string);
	}
}

static void UpdateLight(decStringDictionary &values,
igdeCodecPropertyString codec, const gdeOCLight &light){
	decString string;
	
	if(light.IsPropertySet(gdeOCLight::epType)){
		switch(light.GetType()){
		case deLight::eltPoint:
			values.SetAt(light.GetPropertyName(gdeOCLight::epType), "point");
			break;
			
		case deLight::eltSpot:
			values.SetAt(light.GetPropertyName(gdeOCLight::epType), "spot");
			break;
			
		case deLight::eltProjector:
			values.SetAt(light.GetPropertyName(gdeOCLight::epType), "projector");
			break;
			
		default:
			values.SetAt(light.GetPropertyName(gdeOCLight::epType), "point");
		}
	}
	
	if(light.IsPropertySet(gdeOCLight::epColor)){
		codec.EncodeColor3(light.GetColor(), string);
		values.SetAt(light.GetPropertyName(gdeOCLight::epColor), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epIntensity)){
		string.Format("%.3g", light.GetIntensity());
		values.SetAt(light.GetPropertyName(gdeOCLight::epIntensity), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epAmbientRatio)){
		string.Format("%.3g", light.GetAmbientRatio());
		values.SetAt(light.GetPropertyName(gdeOCLight::epAmbientRatio), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epRange)){
		string.Format("%.3g", light.GetRange());
		values.SetAt(light.GetPropertyName(gdeOCLight::epRange), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epHalfIntDist)){
		string.Format("%.3g", light.GetHalfIntensityDistance());
		values.SetAt(light.GetPropertyName(gdeOCLight::epHalfIntDist), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epSpotAngle)){
		string.Format("%.3g", light.GetSpotAngle());
		values.SetAt(light.GetPropertyName(gdeOCLight::epSpotAngle), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epSpotRatio)){
		string.Format("%.3g", light.GetSpotRatio());
		values.SetAt(light.GetPropertyName(gdeOCLight::epSpotRatio), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epSpotSmoothness)){
		string.Format("%.3g", light.GetSpotSmoothness());
		values.SetAt(light.GetPropertyName(gdeOCLight::epSpotSmoothness), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epSpotExponent)){
		string.Format("%.3g", light.GetSpotExponent());
		values.SetAt(light.GetPropertyName(gdeOCLight::epSpotExponent), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epLightSkin)){
		values.SetAt(light.GetPropertyName(gdeOCLight::epLightSkin), light.GetLightSkinPath());
	}
	
	if(light.IsPropertySet(gdeOCLight::epActivated)){
		values.SetAt(light.GetPropertyName(gdeOCLight::epActivated),
			light.GetActivated() ? "1" : "0");
	}
	
	if(light.IsPropertySet(gdeOCLight::epCastShadows)){
		values.SetAt(light.GetPropertyName(gdeOCLight::epCastShadows),
			light.GetCastShadows() ? "1" : "0");
	}
	
	if(light.IsPropertySet(gdeOCLight::epHintLightImportance)){
		string.Format("%d", light.GetHintLightImportance());
		values.SetAt(light.GetPropertyName(gdeOCLight::epHintLightImportance), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epHintShadowImportance)){
		string.Format("%d", light.GetHintShadowImportance());
		values.SetAt(light.GetPropertyName(gdeOCLight::epHintShadowImportance), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epAttachPosition)){
		codec.EncodeVector(light.GetPosition(), string);
		values.SetAt(light.GetPropertyName(gdeOCLight::epAttachPosition), string);
	}
	
	if(light.IsPropertySet(gdeOCLight::epAttachRotation)){
		codec.EncodeVector(light.GetRotation(), string);
		values.SetAt(light.GetPropertyName(gdeOCLight::epAttachRotation), string);
	}
}

static void UpdateNavBlocker(decStringDictionary &values,
igdeCodecPropertyString codec, const gdeOCNavigationBlocker &navblocker){
	decString string;
	
	if(navblocker.IsPropertySet(gdeOCNavigationBlocker::epEnabled)){
		values.SetAt(navblocker.GetPropertyName(gdeOCNavigationBlocker::epEnabled),
			navblocker.GetEnabled() ? "1" : "0");
	}
	
	if(navblocker.IsPropertySet(gdeOCNavigationBlocker::epShape)){
		codec.EncodeShapeList(navblocker.GetShapeList(), string);
		values.SetAt(navblocker.GetPropertyName(gdeOCNavigationBlocker::epShape), string);
	}
	
	if(navblocker.IsPropertySet(gdeOCNavigationBlocker::epLayer)){
		string.Format("%d", navblocker.GetLayer());
		values.SetAt(navblocker.GetPropertyName(gdeOCNavigationBlocker::epLayer), string);
	}
	
	if(navblocker.IsPropertySet(gdeOCNavigationBlocker::epBlockingPriority)){
		string.Format("%d", navblocker.GetBlockingPriority());
		values.SetAt(navblocker.GetPropertyName(gdeOCNavigationBlocker::epBlockingPriority), string);
	}
	
	if(navblocker.IsPropertySet(gdeOCNavigationBlocker::epAttachPosition)){
		codec.EncodeVector(navblocker.GetPosition(), string);
		values.SetAt(navblocker.GetPropertyName(gdeOCNavigationBlocker::epAttachPosition), string);
	}
	
	if(navblocker.IsPropertySet(gdeOCNavigationBlocker::epAttachRotation)){
		codec.EncodeVector(navblocker.GetRotation(), string);
		values.SetAt(navblocker.GetPropertyName(gdeOCNavigationBlocker::epAttachRotation), string);
	}
}

static void UpdateNavSpace(decStringDictionary &values,
igdeCodecPropertyString codec, const gdeOCNavigationSpace &navspace){
	decString string;
	
	if(navspace.IsPropertySet(gdeOCNavigationSpace::epPath)){
		values.SetAt(navspace.GetPropertyName(gdeOCNavigationSpace::epPath),
			navspace.GetPath());
	}
	
	if(navspace.IsPropertySet(gdeOCNavigationSpace::epBlockerShape)){
		codec.EncodeShapeList(navspace.GetBlockerShapeList(), string);
		values.SetAt(navspace.GetPropertyName(gdeOCNavigationSpace::epBlockerShape), string);
	}
	
	if(navspace.IsPropertySet(gdeOCNavigationSpace::epLayer)){
		string.Format("%d", navspace.GetLayer());
		values.SetAt(navspace.GetPropertyName(gdeOCNavigationSpace::epLayer), string);
	}
	
	if(navspace.IsPropertySet(gdeOCNavigationSpace::epBlockingPriority)){
		string.Format("%d", navspace.GetBlockingPriority());
		values.SetAt(navspace.GetPropertyName(gdeOCNavigationSpace::epBlockingPriority), string);
	}
	
	if(navspace.IsPropertySet(gdeOCNavigationSpace::epSnapDistance)){
		string.Format("%.3f", navspace.GetSnapDistance());
		values.SetAt(navspace.GetPropertyName(gdeOCNavigationSpace::epSnapDistance), string);
	}
	
	if(navspace.IsPropertySet(gdeOCNavigationSpace::epSnapAngle)){
		string.Format("%.3f", navspace.GetSnapAngle());
		values.SetAt(navspace.GetPropertyName(gdeOCNavigationSpace::epSnapAngle), string);
	}
	
	if(navspace.IsPropertySet(gdeOCNavigationSpace::epAttachPosition)){
		codec.EncodeVector(navspace.GetPosition(), string);
		values.SetAt(navspace.GetPropertyName(gdeOCNavigationSpace::epAttachPosition), string);
	}
	
	if(navspace.IsPropertySet(gdeOCNavigationSpace::epAttachRotation)){
		codec.EncodeVector(navspace.GetRotation(), string);
		values.SetAt(navspace.GetPropertyName(gdeOCNavigationSpace::epAttachRotation), string);
	}
}

static void UpdateSpeaker(decStringDictionary &values,
igdeCodecPropertyString codec, const gdeOCSpeaker &speaker){
	decString string;
	
	if(speaker.IsPropertySet(gdeOCSpeaker::epSound)){
		values.SetAt(speaker.GetPropertyName(gdeOCSpeaker::epSound),
			speaker.GetPathSound());
	}
	
	if(speaker.IsPropertySet(gdeOCSpeaker::epLooping)){
		values.SetAt(speaker.GetPropertyName(gdeOCSpeaker::epLooping),
			speaker.GetLooping() ? "1" : "0");
	}
	
	if(speaker.IsPropertySet(gdeOCSpeaker::epVolume)){
		string.Format("%.3f", speaker.GetVolume());
		values.SetAt(speaker.GetPropertyName(gdeOCSpeaker::epVolume), string);
	}
	
	if(speaker.IsPropertySet(gdeOCSpeaker::epRange)){
		string.Format("%.3f", speaker.GetRange());
		values.SetAt(speaker.GetPropertyName(gdeOCSpeaker::epRange), string);
	}
	
	if(speaker.IsPropertySet(gdeOCSpeaker::epRollOff)){
		string.Format("%.3f", speaker.GetRollOff());
		values.SetAt(speaker.GetPropertyName(gdeOCSpeaker::epRollOff), string);
	}
	
	if(speaker.IsPropertySet(gdeOCSpeaker::epDistanceOffset)){
		string.Format("%.1f", speaker.GetDistanceOffset());
		values.SetAt(speaker.GetPropertyName(gdeOCSpeaker::epDistanceOffset), string);
	}
	
	if(speaker.IsPropertySet(gdeOCSpeaker::epPlaySpeed)){
		string.Format("%.3f", speaker.GetPlaySpeed());
		values.SetAt(speaker.GetPropertyName(gdeOCSpeaker::epPlaySpeed), string);
	}
	
	if(speaker.IsPropertySet(gdeOCSpeaker::epPlaying)){
		values.SetAt(speaker.GetPropertyName(gdeOCSpeaker::epPlaying),
			speaker.GetPlaying() ? "1" : "0");
	}
	
	if(speaker.IsPropertySet(gdeOCSpeaker::epAttachPosition)){
		codec.EncodeVector(speaker.GetPosition(), string);
		values.SetAt(speaker.GetPropertyName(gdeOCSpeaker::epAttachPosition), string);
	}
	
	if(speaker.IsPropertySet(gdeOCSpeaker::epAttachRotation)){
		codec.EncodeVector(speaker.GetRotation(), string);
		values.SetAt(speaker.GetPropertyName(gdeOCSpeaker::epAttachRotation), string);
	}
}

static void UpdateWorld(decStringDictionary &values, igdeCodecPropertyString codec, const gdeOCWorld &world){
	decString string;
	
	if(world.IsPropertySet(gdeOCWorld::epPath)){
		values.SetAt(world.GetPropertyName(gdeOCWorld::epPath), world.GetPath());
	}
	if(world.IsPropertySet(gdeOCWorld::epPosition)){
		codec.EncodeVector(world.GetPosition(), string);
		values.SetAt(world.GetPropertyName(gdeOCWorld::epPosition), string);
	}
	if(world.IsPropertySet(gdeOCWorld::epRotation)){
		codec.EncodeVector(world.GetRotation(), string);
		values.SetAt(world.GetPropertyName(gdeOCWorld::epRotation), string);
	}
}

static decStringDictionary BuildValues(const gdeObjectClass &objectClass){
	// build property values
	decStringDictionary newValues(objectClass.GetPropertyValues());
	igdeCodecPropertyString codec;
	int i, count;
	
	// components
	const gdeOCComponent::List &components = objectClass.GetComponents();
	count = components.GetCount();
	for(i=0; i<count; i++){
		UpdateComponent(newValues, codec, *components.GetAt(i));
	}
	
	// environment map probes
	const gdeOCEnvMapProbe::List &envMapProbes = objectClass.GetEnvMapProbes();
	count = envMapProbes.GetCount();
	for(i=0; i<count; i++){
		UpdateEnvMapProbe(newValues, codec, *envMapProbes.GetAt(i));
	}
	
	// lights
	const gdeOCLight::List &lights = objectClass.GetLights();
	count = lights.GetCount();
	for(i=0; i<count; i++){
		UpdateLight(newValues, codec, *lights.GetAt(i));
	}
	
	// navigation blockers
	const gdeOCNavigationBlocker::List &navBlockers = objectClass.GetNavigationBlockers();
	count = navBlockers.GetCount();
	for(i=0; i<count; i++){
		UpdateNavBlocker(newValues, codec, *navBlockers.GetAt(i));
	}
	
	// navigation spaces
	const gdeOCNavigationSpace::List &navSpaces = objectClass.GetNavigationSpaces();
	count = navSpaces.GetCount();
	for(i=0; i<count; i++){
		UpdateNavSpace(newValues, codec, *navSpaces.GetAt(i));
	}
	
	// speakers
	const gdeOCSpeaker::List &speakers = objectClass.GetSpeakers();
	count = speakers.GetCount();
	for(i=0; i<count; i++){
		UpdateSpeaker(newValues, codec, *speakers.GetAt(i));
	}
	
	// worlds
	const gdeOCWorld::List &worlds = objectClass.GetWorlds();
	count = worlds.GetCount();
	for(i=0; i<count; i++){
		UpdateWorld(newValues, codec, *worlds.GetAt(i));
	}
	
	// collect parent property values
	decStringDictionary parentValues;
	objectClass.AddPropertiesTo(parentValues, true, false);
	
	// final dictionary is the difference between the new and the parent dictionary.
	// this contains now only the parameters that actually changed
	decStringList names(newValues.GetKeys());
	const decString *parentValue;
	decStringDictionary values;
	
	count = names.GetCount();
	for(i=0; i<count; i++){
		const decString &name = names.GetAt(i);
		const decString &newValue = newValues.GetAt(name);
		
		if(parentValues.GetAt(name, parentValue) && *parentValue == newValue){
			continue; // same values as the parent has, skip it
		}
		
		values.SetAt(name, newValues.GetAt(name));
	}
	
	// finished
	return values;
}



// Class gdeUOCPropertyValuesFromSubObjects
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCPropertyValuesFromSubObjects::gdeUOCPropertyValuesFromSubObjects(
gdeObjectClass *objectClass, const decStringDictionary &newValues) :
gdeUOCSetPropertyValues(objectClass, newValues)
{
	SetShortInfo("Object class set property values from sub objects");
}

gdeUOCPropertyValuesFromSubObjects::~gdeUOCPropertyValuesFromSubObjects(){
}

gdeUOCPropertyValuesFromSubObjects::Ref gdeUOCPropertyValuesFromSubObjects::Create(
gdeObjectClass *objectClass){
	if(!objectClass){
		DETHROW(deeInvalidParam);
	}
	
	const decStringDictionary newValues(BuildValues(*objectClass));
	if(newValues.GetCount() > 0){ //&& newValues != objectClass->GetPropertyValues()){
		return Ref::New(objectClass, newValues);
		
	}else{
		return {};
	}
}
