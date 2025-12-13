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

#include "igdeXMLGameDefinition.h"
#include "igdeGameDefinition.h"
#include "igdeGDCategory.h"
#include "igdeTagManager.h"
#include "class/igdeGDClass.h"
#include "class/igdeGDClassInherit.h"
#include "class/igdeGDClassManager.h"
#include "class/igdeGDCamera.h"
#include "class/billboard/igdeGDCBillboard.h"
#include "class/component/igdeGDCComponent.h"
#include "class/component/igdeGDCCTexture.h"
#include "class/envmapprobe/igdeGDCEnvMapProbe.h"
#include "class/snappoint/igdeGDCSnapPoint.h"
#include "class/particleemitter/igdeGDCParticleEmitter.h"
#include "class/forcefield/igdeGDCForceField.h"
#include "class/light/igdeGDCLight.h"
#include "class/speaker/igdeGDCSpeaker.h"
#include "class/navspace/igdeGDCNavigationSpace.h"
#include "class/navblocker/igdeGDCNavigationBlocker.h"
#include "class/world/igdeGDCWorld.h"
#include "particleemitter/igdeGDParticleEmitter.h"
#include "property/igdeGDProperty.h"
#include "skin/igdeGDSkin.h"
#include "skin/igdeGDSkinManager.h"
#include "sky/igdeGDSky.h"
#include "sky/igdeGDSkyController.h"
#include "sky/igdeGDSkyManager.h"
#include "../codec/igdeCodecPropertyString.h"
#include "../engine/igdeEngineController.h"
#include "../environment/igdeEnvironment.h"
#include "../gui/igdeMainWindow.h"
#include "../gui/filedialog/igdeFilePatternList.h"
#include "../gui/filedialog/igdeFilePattern.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>



// Definitions
////////////////

#define LOGGING_NAME "XMLGameDefinition"



// Class igdeXMLGameDefinition
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeXMLGameDefinition::igdeXMLGameDefinition(igdeEnvironment &environment, deLogger *logger) :
igdeBaseXML(logger, LOGGING_NAME),
pEnvironment(environment){
}

igdeXMLGameDefinition::~igdeXMLGameDefinition(){
}



// Loading
////////////

void igdeXMLGameDefinition::Load(decBaseFileReader &file, igdeGameDefinition &gamedef){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::New());
	
	// parse xml
	decXmlParser(GetLogger()).ParseXml(&file, xmlDoc);
	
	// some cleanup visiting
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	// parse xml tree
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root){
		DETHROW(deeInvalidParam);
	}
	if(strcmp(root->GetName(), "gameDefinition") != 0){
		if(strcmp(root->GetName(), "gameDef") != 0){ // old version
			DETHROW(deeInvalidParam);
		}
	}
	
	// parse game definition
	pParseGameDefinition(*root, gamedef);
	
	// preparation work
	gamedef.UpdateTags();
	gamedef.ResolveInheritClasses();
}



// Private Functions
//////////////////////

void igdeXMLGameDefinition::pParseGameDefinition(const decXmlElementTag &root, igdeGameDefinition &gamedef){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "identifier"){
			gamedef.SetID(GetCDataString(*tag));
			
		}else if(tagName == "description"){
			gamedef.SetDescription(ReadMultilineString(*tag));
			
		}else if(tagName == "basePath"){
			decString filename(GetCDataString(*tag));
			
			filename.ReplaceString("%{DE_SHARE_PATH}", pEnvironment.GetEngineController()
				->GetEngine()->GetOS()->GetPathShare().GetString());
			
			gamedef.SetBasePath(filename);
			
		}else if(tagName == "vfsPath"){
			gamedef.SetVFSPath(GetCDataString(*tag));
			
		}else if(tagName == "scriptModule"){
			gamedef.SetScriptModule(GetCDataString(*tag));
			
		}else if(strcmp(tag->GetName(), "baseGameDefinition") == 0){
			gamedef.GetBaseGameDefinitionIDList().Add(GetCDataString(*tag));
			
		}else if(tagName == "defaultClass"){
			gamedef.GetClassManager()->SetDefaultClassName(GetCDataString(*tag));
			
		}else if(tagName == "defaultSkin"){
			gamedef.GetSkinManager()->SetDefaultSkinPath(GetCDataString(*tag));
			
		}else if(tagName == "defaultSky"){
			gamedef.GetSkyManager()->SetDefaultPath(GetCDataString(*tag));
			
		}else if(tagName == "findPathClasses"){
			const char * const cdata = GetCDataString(*tag);
			decStringList &autoPath = gamedef.GetClassManager()->GetAutoFindPath();
			if(cdata && !autoPath.Has(cdata)){
				autoPath.Add(cdata);
			}
			
		}else if(tagName == "findPathSkins"){
			const char * const cdata = GetCDataString(*tag);
			decStringList &autoPath = gamedef.GetSkinManager()->GetAutoFindPath();
			if(cdata && !autoPath.Has(cdata)){
				autoPath.Add(cdata);
			}
			
		}else if(tagName == "findPathSkies"){
			const char * const cdata = GetCDataString(*tag);
			decStringList &autoPath = gamedef.GetSkyManager()->GetAutoFindPath();
			if(cdata && !autoPath.Has(cdata)){
				autoPath.Add(cdata);
			}
			
		}else if(tagName == "class"){
			pParseClass(*tag, gamedef);
			
		}else if(tagName == "skin"){
			pParseSkin(*tag, gamedef);
			
		}else if(tagName == "sky"){
			pParseSky(*tag, gamedef);
			
		}else if(tagName == "particleEmitter"){
			pParseParticleEmitter(*tag, gamedef);
			
		}else if(tagName == "categories"){
			pParseGDCategories(*tag, gamedef);
			
		}else if(tagName == "decalProperty"){
			try{
				const igdeGDProperty::Ref property(igdeGDProperty::Ref::New(
					GetAttributeString(*tag, "name")));
				pParseProperty(*tag, property);
				
				gamedef.GetListDecalProperties().Add(property);
			}catch(const deException &e){
				LogErrorExceptionTag(*tag, e);
			}
			
		}else if(tagName == "property"){
			try{
				const igdeGDProperty::Ref property(igdeGDProperty::Ref::New(
					GetAttributeString(*tag, "name")));
				pParseProperty(*tag, property);
				
				gamedef.GetListWorldProperties().Add(property);
			}catch(const deException &e){
				LogErrorExceptionTag(*tag, e);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void igdeXMLGameDefinition::pParseClass(const decXmlElementTag &root, igdeGameDefinition &gamedef){
	const igdeGDClass::Ref gdClass(igdeGDClass::Ref::New(GetAttributeString(root, "name")));
	decStringDictionary propertyValues;
	const char *scaleMode;
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "description"){
			gdClass->SetDescription(ReadMultilineString(*tag));
			
		}else if(tagName == "base"){
			// deprecated
			
		}else if(tagName == "size"){
			// deprecated
			
		}else if(tagName == "property"){
			try{
				const igdeGDProperty::Ref property(igdeGDProperty::Ref::New(
					GetAttributeString(*tag, "name")));
				pParseProperty(*tag, property);
				
				gdClass->AddProperty(property);
			}catch(const deException &){
				throw;
			}
			
		}else if(tagName == "propertyValue"){
			propertyValues.SetAt(GetAttributeString(*tag, "name"),
				ReadMultilineString(*tag));
			
		}else if(tagName == "textureProperty"){
			try{
				const igdeGDProperty::Ref property(igdeGDProperty::Ref::New(
					GetAttributeString(*tag, "name")));
				pParseProperty(*tag, property);
				
				gdClass->AddTextureProperty(property);
			}catch(const deException &e){
				LogErrorExceptionTag(*tag, e);
			}
			
		}else if(tagName == "scaleMode"){
			scaleMode = GetCDataString(*tag);
			
			if(strcmp(scaleMode, "fixed") == 0){
				gdClass->SetScaleMode(igdeGDClass::esmFixed);
				
			}else if(strcmp(scaleMode, "uniform") == 0){
				gdClass->SetScaleMode(igdeGDClass::esmUniform);
				
			}else if(strcmp(scaleMode, "free") == 0){
				gdClass->SetScaleMode(igdeGDClass::esmFree);
				
			}else{
				LogWarnUnknownValue(*tag, scaleMode);
				//gdClass->SetScaleMode( igdeGDClass::esmFixed );
			}
			
		}else if(tagName == "component"){
			pParseClassComponent(*tag, gdClass);
			
		}else if(tagName == "billboard"){
			pParseClassBillboard(*tag, gdClass);
			
		}else if(tagName == "camera"){
			pParseCamera(*tag, gdClass->GetCamera());
			gdClass->SetHasCamera(true);
			
		}else if(tagName == "light"){
			pParseClassLight(*tag, gdClass);
			
		}else if(tagName == "snapPoint"){
			pParseClassSnapPoint(*tag, gdClass);
			
		}else if(tagName == "particleEmitter"){
			pParseClassParticleEmitter(*tag, gdClass);
			
		}else if(tagName == "forceField"){
			pParseClassForceField(*tag, gdClass);
			
		}else if(tagName == "environmentMapProbe"){
			pParseClassEnvMapProbe(*tag, gdClass);
			
		}else if(tagName == "speaker"){
			pParseClassSpeaker(*tag, gdClass);
			
		}else if(tagName == "navigationSpace"){
			pParseClassNavigationSpace(*tag, gdClass);
			
		}else if(tagName == "navigationBlocker"){
			pParseClassNavigationBlocker(*tag, gdClass);
			
		}else if(tagName == "world"){
			pParseClassWorld(*tag, gdClass);
			
		}else if(tagName == "category"){
			gdClass->SetCategory(GetCDataString(*tag));
			
		}else if(tagName == "ghost"){
			gdClass->SetIsGhost(GetCDataBool(*tag));
			
		}else if(tagName == "canInstantiate"
		/* backwards compatibility */ || tagName == "canInstanciate" ){
			gdClass->SetCanInstantiate(GetCDataBool(*tag));
			
		}else if(tagName == "attachableBehavior"){
			gdClass->SetIsAttachableBehavior(GetCDataBool(*tag));
			
		}else if(tagName == "replaceSubObjects"){
			const decStringList keys(decString(GetCDataString(*tag)).Split(','));
			int j, filter = igdeGDClass::FilterSubObjectsAll;
			const int keyCount = keys.GetCount();
			
			for(j=0; j<keyCount; j++){
				const decString &key = keys.GetAt(j);
				
				if(key == "billboards"){
					filter &= ~igdeGDClass::efsoBillboards;
					
				}else if(key == "components"){
					filter &= ~igdeGDClass::efsoComponents;
					
				}else if(key == "lights"){
					filter &= ~igdeGDClass::efsoLights;
					
				}else if(key == "snapPoints"){
					filter &= ~igdeGDClass::efsoSnapPoints;
					
				}else if(key == "particleEmitters"){
					filter &= ~igdeGDClass::efsoParticleEmitters;
					
				}else if(key == "forceFields"){
					filter &= ~igdeGDClass::efsoForceFields;
					
				}else if(key == "envMapProbes"){
					filter &= ~igdeGDClass::efsoEnvMapProbes;
					
				}else if(key == "speakers"){
					filter &= ~igdeGDClass::efsoSpeakers;
					
				}else if(key == "navigationSpaces"){
					filter &= ~igdeGDClass::efsoNavigationSpaces;
					
				}else if(key == "navigationBlockers"){
					filter &= ~igdeGDClass::efsoNavigationBlockers;
					
				}else if(key == "worlds"){
					filter &= ~igdeGDClass::efsoWorlds;
				}
			}
			
			gdClass->SetInheritSubObjects(filter);
			
		}else if(tagName == "inherit"){
			pParseClassInherit(*tag, gdClass);
			
		}else if(tagName == "defaultInheritPropertyPrefix"){
			gdClass->SetDefaultInheritPropertyPrefix(GetCDataString(*tag));
			
		}else if(tagName == "hideTag"){
			gdClass->GetHideTags().AddTag(GetCDataString(*tag));
			
		}else if(tagName == "partialHideTag"){
			gdClass->GetPartialHideTags().AddTag(GetCDataString(*tag));
			
		}else if(tagName == "texture"){
			pParseClassTexture(*tag, gdClass);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdClass->SetPropertyValues(propertyValues);
	gdClass->Check();
	gamedef.GetClassManager()->Add(gdClass);
}

void igdeXMLGameDefinition::pParseClassComponent(const decXmlElementTag &root, igdeGDClass &gdclass){
	const int elementCount = root.GetElementCount();
	const igdeGDCComponent::Ref component(igdeGDCComponent::Ref::New());
	const char *value;
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const  tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "model"){
			component->SetModelPath(GetCDataString(*tag));
			
		}else if(tagName == "skin"){
			component->SetSkinPath(GetCDataString(*tag));
			
		}else if(tagName == "rig"){
			component->SetRigPath(GetCDataString(*tag));
			
		}else if(tagName == "animator"){
			component->SetAnimatorPath(GetCDataString(*tag));
			
		}else if(tagName == "occlusionMesh"){
			component->SetOcclusionMeshPath(GetCDataString(*tag));
			
		}else if(tagName == "audioModel"){
			component->SetAudioModelPath(GetCDataString(*tag));
			
		}else if(tagName == "playbackController"){
			component->SetPlaybackController(GetCDataString(*tag));
			
		}else if(tagName == "noScaling"){
			component->SetDoNotScale(GetCDataBool(*tag));
			
		}else if(tagName == "static"){
			component->SetStatic(GetCDataBool(*tag));
			
		}else if(tagName == "partialHide"){
			component->SetPartialHide(GetCDataBool(*tag));
			
		}else if(tagName == "attachTarget"){
			component->SetAttachTarget(GetCDataBool(*tag));
			
		}else if(tagName == "colliderResponseType"){
			value = GetCDataString(*tag);
			
			if(strcmp(value, "static") == 0){
				component->SetColliderResponseType(deCollider::ertStatic);
				
			}else if(strcmp(value, "kinematic") == 0){
				component->SetColliderResponseType(deCollider::ertKinematic);
				
			}else if(strcmp(value, "dynamic") == 0){
				component->SetColliderResponseType(deCollider::ertDynamic);
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else if(tagName == "renderEnvMap"){
			component->SetRenderEnvMap(GetCDataBool(*tag));
			
		}else if(tagName == "affectsAudio"){
			component->SetAffectsAudio(GetCDataBool(*tag));
			
		}else if(tagName == "lightShadowIgnore"){
			component->SetLightShadowIgnore(GetCDataBool(*tag));
			
		}else if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			component->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector rotation;
			ReadVector(*tag, rotation);
			component->SetOrientation(decQuaternion::CreateFromEuler(rotation * DEG2RAD));
			
		}else if(tagName == "bone"){
			component->SetBoneName(GetCDataString(*tag));
			
		}else if(tagName == "texture"){
			pParseClassComponentTexture(*tag, gdclass, *component);
			
		}else if(tagName == "link"){
			value = GetAttributeString(*tag, "target");
			
			if(strcmp(value, "model") == 0){
				component->SetPropertyName(igdeGDCComponent::epModel, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "skin") == 0){
				component->SetPropertyName(igdeGDCComponent::epSkin, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "rig") == 0){
				component->SetPropertyName(igdeGDCComponent::epRig, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "animator") == 0){
				component->SetPropertyName(igdeGDCComponent::epAnimator, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "playbackController") == 0){
				component->SetPropertyName(igdeGDCComponent::epPlaybackController, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "occlusionMesh") == 0){
				component->SetPropertyName(igdeGDCComponent::epOcclusionMesh, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "audioModel") == 0){
				component->SetPropertyName(igdeGDCComponent::epAudioModel, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "renderEnvMap") == 0){
				component->SetPropertyName(igdeGDCComponent::epRenderEnvMap, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "affectsAudio") == 0){
				component->SetPropertyName(igdeGDCComponent::epAffectsAudio, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachPosition") == 0){
				component->SetPropertyName(igdeGDCComponent::epAttachPosition, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachRotation") == 0){
				component->SetPropertyName(igdeGDCComponent::epAttachRotation, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "lightShadowIgnore") == 0){
				component->SetPropertyName(igdeGDCComponent::epLightShadowIgnore, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "animation") == 0){
				component->SetPropertyName(igdeGDCComponent::epAnimation, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "move") == 0){
				component->SetPropertyName(igdeGDCComponent::epMove, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddComponent(component);
}

void igdeXMLGameDefinition::pParseClassBillboard(const decXmlElementTag &root, igdeGDClass &gdclass){
	const igdeGDCBillboard::Ref billboard(igdeGDCBillboard::Ref::New());
	const int elementCount = root.GetElementCount();
	const char *value;
	int i;
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "skin"){
			billboard->SetSkinPath(GetCDataString(*tag));
			
		}else if(tagName == "axis"){
			decVector axis(0.0f, 1.0f, 0.0f);
			ReadVector(*tag, axis);
			billboard->SetAxis(axis);
			
		}else if(tagName == "size"){
			decVector2 size(1.0f, 1.0f);
			ReadVector2(*tag, size);
			billboard->SetSize(size);
			
		}else if(tagName == "offset"){
			decVector2 offset;
			ReadVector2(*tag, offset);
			billboard->SetOffset(offset);
			
		}else if(tagName == "locked"){
			billboard->SetLocked(GetCDataBool(*tag));
			
		}else if(tagName == "spherical"){
			billboard->SetSpherical(GetCDataBool(*tag));
			
		}else if(tagName == "sizeFixedToScreen"){
			billboard->SetSizeFixedToScreen(GetCDataBool(*tag));
			
		}else if(tagName == "noScaling"){
			billboard->SetDoNotScale(GetCDataBool(*tag));
			
		}else if(tagName == "partialHide"){
			billboard->SetPartialHide(GetCDataBool(*tag));
			
		}else if(tagName == "renderEnvMap"){
			billboard->SetRenderEnvMap(GetCDataBool(*tag));
			
		}else if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			billboard->SetPosition(position);
			
		}else if(tagName == "bone"){
			billboard->SetBoneName(GetCDataString(*tag));
			
		}else if(tagName == "link"){
			value = GetAttributeString(*tag, "target");
			
			if(strcmp(value, "skin") == 0){
				billboard->SetPropertyName(igdeGDCBillboard::epSkin, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "axis") == 0){
				billboard->SetPropertyName(igdeGDCBillboard::epAxis, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "offset") == 0){
				billboard->SetPropertyName(igdeGDCBillboard::epOffset, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "locked") == 0){
				billboard->SetPropertyName(igdeGDCBillboard::epLocked, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "spherical") == 0){
				billboard->SetPropertyName(igdeGDCBillboard::epSpherical, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "renderEnvMap") == 0){
				billboard->SetPropertyName(igdeGDCBillboard::epRenderEnvMap, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachPosition") == 0){
				billboard->SetPropertyName(igdeGDCBillboard::epAttachPosition, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddBillboard(billboard);
}

void igdeXMLGameDefinition::pParseClassInherit(const decXmlElementTag &root, igdeGDClass &gdclass){
	const igdeGDClassInherit::Ref inherit(igdeGDClassInherit::Ref::New(
		GetAttributeString(root, "name")));
	
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "propertyPrefix"){
			inherit->SetPropertyPrefix(GetCDataString(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddInheritClass(inherit);
}

void igdeXMLGameDefinition::pParseClassComponentTexture(const decXmlElementTag &root,
igdeGDClass&, igdeGDCComponent &gdccomponent){
	const igdeGDCCTexture::Ref texture(igdeGDCCTexture::Ref::New());
	const int elementCount = root.GetElementCount();
	decStringDictionary properties;
	int e;
	
	texture->SetName(GetAttributeString(root, "name"));
	if(gdccomponent.GetTextureList().HasNamed(texture->GetName().GetString())){
		LogWarnGenericProblemValue(root, texture->GetName().GetString(), "A texture with this name exists already.");
	}
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "skin"){
			texture->SetPathSkin(GetCDataString(*tag));
			
		}else if(tagName == "offset"){
			decVector2 offset;
			ReadVector2(*tag, offset);
			texture->SetOffset(offset);
			
		}else if(tagName == "scale"){
			decVector2 scale(1.0f, 1.0f);
			ReadVector2(*tag, scale);
			texture->SetScale(scale);
			
		}else if(tagName == "rotate"){
			texture->SetRotation(GetCDataFloat(*tag) * DEG2RAD);
			
		}else if(tagName == "tint"){
			decColor color;
			ReadColor(*tag, color);
			texture->SetColorTint(color);
			
		}else if(tagName == "property"){
			properties.SetAt(GetAttributeString(*tag, "name"),
				ReadMultilineString(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	texture->SetProperties(properties);
	
	gdccomponent.GetTextureList().Add(texture);
}

void igdeXMLGameDefinition::pParseClassLight(const decXmlElementTag &root, igdeGDClass &gdclass){
	const igdeGDCLight::Ref gdcLight(igdeGDCLight::Ref::New());
	const char *value;
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			gdcLight->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector rotation;
			ReadVector(*tag, rotation);
			gdcLight->SetOrientation(decQuaternion::CreateFromEuler(rotation * DEG2RAD));
			
		}else if(tagName == "bone"){
			gdcLight->SetBoneName(GetCDataString(*tag));
			
		}else if(tagName == "color"){
			gdcLight->SetColor(decColor(
				GetAttributeFloat(*tag, "r"),
				GetAttributeFloat(*tag, "g"),
				GetAttributeFloat(*tag, "b")));
			
		}else if(tagName == "type"){
			value = GetCDataString(*tag);
			
			if(strcmp(value, "point") == 0){
				gdcLight->SetType(deLight::eltPoint);
				
			}else if(strcmp(value, "spot") == 0){
				gdcLight->SetType(deLight::eltSpot);
				
			}else if(strcmp(value, "projector") == 0){
				gdcLight->SetType(deLight::eltProjector);
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else if(tagName == "intensity"){
			gdcLight->SetIntensity(GetCDataFloat(*tag));
			
		}else if(tagName == "range"){
			gdcLight->SetRange(GetCDataFloat(*tag));
			
		}else if(tagName == "ambientRatio"){
			gdcLight->SetAmbientRatio(GetCDataFloat(*tag));
			
		}else if(tagName == "halfIntensityDistance"){
			gdcLight->SetHalfIntensityDistance(GetCDataFloat(*tag));
			
		}else if(tagName == "spotAngle"){
			gdcLight->SetSpotAngle(GetCDataFloat(*tag));
			
		}else if(tagName == "spotRatio"){
			gdcLight->SetSpotRatio(GetCDataFloat(*tag));
			
		}else if(tagName == "spotSmoothness"){
			gdcLight->SetSpotSmoothness(GetCDataFloat(*tag));
			
		}else if(tagName == "spotExponent"){
			gdcLight->SetSpotExponent(GetCDataFloat(*tag));
			
		}else if(tagName == "activated"){
			gdcLight->SetActivated(GetCDataBool(*tag));
			
		}else if(tagName == "castShadows"){
			gdcLight->SetCastShadows(GetCDataBool(*tag));
			
		}else if(tagName == "lightSkin"){
			gdcLight->SetLightSkinPath(GetCDataString(*tag));
			
		}else if(tagName == "hintLightImportance"){
			gdcLight->SetHintLightImportance(GetCDataInt(*tag));
			
		}else if(tagName == "hintShadowImportance"){
			gdcLight->SetHintShadowImportance(GetCDataInt(*tag));
			
		}else if(tagName == "hintMovement"){
			value = GetCDataString(*tag);
			
			if(strcmp(value, "static") == 0){
				gdcLight->SetHintMovement(deLight::emhStationary);
				
			}else if(strcmp(value, "jittering") == 0){
				gdcLight->SetHintMovement(deLight::emhJittering);
				
			}else if(strcmp(value, "dynamic") == 0){
				gdcLight->SetHintMovement(deLight::emhDynamic);
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else if(tagName == "hintParameter"){
			value = GetCDataString(*tag);
			
			if(strcmp(value, "static") == 0){
				gdcLight->SetHintParameter(deLight::ephStatic);
				
			}else if(strcmp(value, "activation") == 0){
				gdcLight->SetHintParameter(deLight::ephActivation);
				
			}else if(strcmp(value, "flicker") == 0){
				gdcLight->SetHintParameter(deLight::ephFlicker);
				
			}else if(strcmp(value, "dynamic") == 0){
				gdcLight->SetHintParameter(deLight::ephDynamic);
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else if(tagName == "link"){
			value = GetAttributeString(*tag, "target");
			
			if(strcmp(value, "type") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epType, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "color") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epColor, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "intensity") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epIntensity, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "range") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epRange, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "ambientRatio") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epAmbientRatio, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "halfIntensityDistance") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epHalfIntDist, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "spotAngle") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epSpotAngle, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "spotRatio") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epSpotRatio, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "spotSmoothness") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epSpotSmoothness, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "spotExponent") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epSpotExponent, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "lightSkin") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epLightSkin, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "activated") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epActivated, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "castShadows") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epCastShadows, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "hintLightImportance") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epHintLightImportance, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "hintShadowImportance") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epHintShadowImportance, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachPosition") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epAttachPosition, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachRotation") == 0){
				gdcLight->SetPropertyName(igdeGDCLight::epAttachRotation, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else if(tagName == "trigger"){
			value = GetAttributeString(*tag, "target");
			
			if(strcmp(value, "activated") == 0){
				gdcLight->SetTriggerName(igdeGDCLight::etActivated, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddLight(gdcLight);
}

void igdeXMLGameDefinition::pParseClassSnapPoint(const decXmlElementTag &root,
igdeGDClass &gdclass){
	const igdeGDCSnapPoint::Ref snappoint(igdeGDCSnapPoint::Ref::New());
	const int count = root.GetElementCount();
	int i;
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "name"){
			snappoint->SetName(GetCDataString(*tag));
			
		}else if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			snappoint->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector rotation;
			ReadVector(*tag, rotation);
			snappoint->SetRotation(rotation);
			
		}else if(tagName == "snapDistance"){
			snappoint->SetSnapDistance(GetCDataFloat(*tag));
			
		}else if(tagName == "snapToRotation"){
			snappoint->SetSnapToRotation(GetCDataBool(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddSnapPoint(snappoint);
}

void igdeXMLGameDefinition::pParseClassParticleEmitter(const decXmlElementTag &root, igdeGDClass &gdclass){
	const igdeGDCParticleEmitter::Ref emitter(igdeGDCParticleEmitter::Ref::New());
	const int elementCount = root.GetElementCount();
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "path"){
			emitter->SetPath(GetCDataString(*tag));
			
		}else if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			emitter->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			emitter->SetOrientation(decQuaternion::CreateFromEuler(orientation * DEG2RAD));
			
		}else if(tagName == "bone"){
			emitter->SetBoneName(GetCDataString(*tag));
			
		}else if(tagName == "casting"){
			emitter->SetCasting(GetCDataBool(*tag));
			
		}else if(tagName == "link"){
			const char * const value = GetAttributeString(*tag, "target");
			
			if(strcmp(value, "path") == 0){
				emitter->SetPropertyName(igdeGDCParticleEmitter::epPath, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "casting") == 0){
				emitter->SetPropertyName(igdeGDCParticleEmitter::epCasting, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachPosition") == 0){
				emitter->SetPropertyName(igdeGDCParticleEmitter::epAttachPosition, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachRotation") == 0){
				emitter->SetPropertyName(igdeGDCParticleEmitter::epAttachRotation, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else if(tagName == "trigger"){
			const char * const value = GetAttributeString(*tag, "target");
			
			if(strcmp(value, "casting") == 0){
				emitter->SetTriggerName(igdeGDCParticleEmitter::etCasting, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddParticleEmitter(emitter);
}

void igdeXMLGameDefinition::pParseClassForceField(const decXmlElementTag &root, igdeGDClass &gdclass){
	const igdeGDCForceField::Ref field(igdeGDCForceField::Ref::New());
	const int elementCount = root.GetElementCount();
	igdeCodecPropertyString codec;
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			field->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			field->SetOrientation(decQuaternion::CreateFromEuler(orientation * DEG2RAD));
			
		}else if(tagName == "bone"){
			field->SetBoneName(GetCDataString(*tag));
			
		}else if(tagName == "influenceArea"){
			decShapeList list;
			codec.DecodeShapeList(GetCDataString(*tag), list);
			field->SetInfluenceArea(list);
			
		}else if(tagName == "radius"){
			field->SetRadius(GetCDataFloat(*tag));
			
		}else if(tagName == "exponent"){
			field->SetExponent(GetCDataFloat(*tag));
			
		}else if(tagName == "fieldType"){
			const decString cdata(GetCDataString(*tag));
			
			if(cdata == "radial"){
				field->SetFieldType(deForceField::eftRadial);
				
			}else if(cdata == "linear"){
				field->SetFieldType(deForceField::eftLinear);
				
			}else if(cdata == "vortex"){
				field->SetFieldType(deForceField::eftVortex);
				
			}else{
				LogWarnUnknownValue(*tag, cdata);
			}
			
		}else if(tagName == "applicationType"){
			const decString cdata(GetCDataString(*tag));
			
			if(cdata == "direct"){
				field->SetApplicationType(deForceField::eatDirect);
				
			}else if(cdata == "surface"){
				field->SetApplicationType(deForceField::eatSurface);
				
			}else if(cdata == "mass"){
				field->SetApplicationType(deForceField::eatMass);
				
			}else if(cdata == "speed"){
				field->SetApplicationType(deForceField::eatSpeed);
				
			}else{
				LogWarnUnknownValue(*tag, cdata);
			}
			
		}else if(tagName == "direction"){
			decVector direction;
			ReadVector(*tag, direction);
			field->SetDirection(direction);
			
		}else if(tagName == "force"){
			field->SetForce(GetCDataFloat(*tag));
			
		}else if(tagName == "fluctuationDirection"){
			field->SetFluctuationDirection(GetCDataFloat(*tag));
			
		}else if(tagName == "fluctuationForce"){
			field->SetFluctuationForce(GetCDataFloat(*tag));
			
		}else if(tagName == "shape"){
			decShapeList list;
			codec.DecodeShapeList(GetCDataString(*tag), list);
			field->SetShape(list);
			
		}else if(tagName == "enabled"){
			field->SetEnabled(GetCDataBool(*tag));
			
		}else if(tagName == "link"){
			const char * const property = GetAttributeString(*tag, "property");
			const decString value(GetAttributeString(*tag, "target"));
			
			if(value == "influenceArea"){
				field->SetPropertyName(igdeGDCForceField::epInfluenceArea, property);
				
			}else if(value == "radius"){
				field->SetPropertyName(igdeGDCForceField::epRadius, property);
				
			}else if(value == "exponent"){
				field->SetPropertyName(igdeGDCForceField::epExponent, property);
				
			}else if(value == "fieldType"){
				field->SetPropertyName(igdeGDCForceField::epFieldType, property);
				
			}else if(value == "applicationType"){
				field->SetPropertyName(igdeGDCForceField::epApplicationType, property);
				
			}else if(value == "direction"){
				field->SetPropertyName(igdeGDCForceField::epDirection, property);
				
			}else if(value == "force"){
				field->SetPropertyName(igdeGDCForceField::epForce, property);
				
			}else if(value == "fluctuationDirection"){
				field->SetPropertyName(igdeGDCForceField::epFluctuationDirection, property);
				
			}else if(value == "fluctuationForce"){
				field->SetPropertyName(igdeGDCForceField::epFluctuationForce, property);
				
			}else if(value == "shape"){
				field->SetPropertyName(igdeGDCForceField::epShape, property);
				
			}else if(value == "enabled"){
				field->SetPropertyName(igdeGDCForceField::epEnabled, property);
				
			}else if(value == "attachPosition"){
				field->SetPropertyName(igdeGDCForceField::epAttachPosition, property);
				
			}else if(value == "attachRotation"){
				field->SetPropertyName(igdeGDCForceField::epAttachRotation, property);
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else if(tagName == "trigger"){
			const char * const property = GetAttributeString(*tag, "property");
			const decString value(GetAttributeString(*tag, "target"));
			
			if(value == "enabled"){
				field->SetTriggerName(igdeGDCForceField::etEnabled, property);
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddForceField(field);
}

void igdeXMLGameDefinition::pParseClassEnvMapProbe(const decXmlElementTag &root, igdeGDClass &gdclass){
	const igdeGDCEnvMapProbe::Ref envMapProbe(igdeGDCEnvMapProbe::Ref::New());
	const int elementCount = root.GetElementCount();
	igdeCodecPropertyString codec;
	decShapeList shapeList;
	const char *value;
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			envMapProbe->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			envMapProbe->SetOrientation(decQuaternion::CreateFromEuler(orientation * DEG2RAD));
			
		}else if(tagName == "scaling"){
			decVector scaling(1.0f, 1.0f, 1.0f);
			ReadVector(*tag, scaling);
			envMapProbe->SetScaling(scaling);
			
		}else if(tagName == "influenceArea"){
			codec.DecodeShapeList(GetCDataString(*tag), shapeList);
			envMapProbe->SetShapeListInfluence(shapeList);
			shapeList.RemoveAll();
			
		}else if(tagName == "reflectionShape"){
			codec.DecodeShapeList(GetCDataString(*tag), shapeList);
			
			if(shapeList.GetCount() == 0){
				envMapProbe->SetShapeReflection(nullptr);
				
			}else{
				envMapProbe->SetShapeReflection(shapeList.GetAt(0)->Copy());
			}
			
			shapeList.RemoveAll();
			
		}else if(tagName == "reflectionMask"){
			codec.DecodeShapeList(GetCDataString(*tag), shapeList);
			envMapProbe->SetShapeListReflectionMask(shapeList);
			shapeList.RemoveAll();
			
		}else if(tagName == "influenceBorderSize"){
			envMapProbe->SetInfluenceBorderSize(GetCDataFloat(*tag));
			
		}else if(tagName == "influencePriority"){
			envMapProbe->SetInfluencePriority(GetCDataInt(*tag));
			
		}else if(tagName == "link"){
			value = GetAttributeString(*tag, "target");
			
			if(strcmp(value, "influenceArea") == 0){
				envMapProbe->SetPropertyName(igdeGDCEnvMapProbe::epInfluenceArea, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "influenceBorderSize") == 0){
				envMapProbe->SetPropertyName(igdeGDCEnvMapProbe::epInfluenceBorderSize, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "influencePriority") == 0){
				envMapProbe->SetPropertyName(igdeGDCEnvMapProbe::epInfluencePriority, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "reflectionShape") == 0){
				envMapProbe->SetPropertyName(igdeGDCEnvMapProbe::epReflectionShape, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "reflectionMask") == 0){
				envMapProbe->SetPropertyName(igdeGDCEnvMapProbe::epReflectionMask, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachPosition") == 0){
				envMapProbe->SetPropertyName(igdeGDCEnvMapProbe::epAttachPosition, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachRotation") == 0){
				envMapProbe->SetPropertyName(igdeGDCEnvMapProbe::epAttachRotation, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddEnvironmentMapProbe(envMapProbe);
}

void igdeXMLGameDefinition::pParseClassSpeaker(const decXmlElementTag &root, igdeGDClass &gdclass){
	const igdeGDCSpeaker::Ref gdcSpeaker(igdeGDCSpeaker::Ref::New());
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "sound"){
			gdcSpeaker->SetPathSound(GetCDataString(*tag));
			
		}else if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			gdcSpeaker->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			gdcSpeaker->SetOrientation(decQuaternion::CreateFromEuler(orientation * DEG2RAD));
			
		}else if(tagName == "bone"){
			gdcSpeaker->SetBoneName(GetCDataString(*tag));
			
		}else if(tagName == "looping"){
			gdcSpeaker->SetLooping(GetCDataBool(*tag));
			
		}else if(tagName == "playing"){
			gdcSpeaker->SetPlaying(GetCDataBool(*tag));
			
		}else if(tagName == "volume"){
			gdcSpeaker->SetVolume(GetCDataFloat(*tag));
			
		}else if(tagName == "range"){
			gdcSpeaker->SetRange(GetCDataFloat(*tag));
			
		}else if(tagName == "rollOff"){
			gdcSpeaker->SetRollOff(GetCDataFloat(*tag));
			
		}else if(tagName == "distanceOffset"){
			gdcSpeaker->SetDistanceOffset(GetCDataFloat(*tag));
			
		}else if(tagName == "playSpeed"){
			gdcSpeaker->SetPlaySpeed(GetCDataFloat(*tag));
			
		}else if(tagName == "link"){
			const char * const value = GetAttributeString(*tag, "target");
			
			if(strcmp(value, "sound") == 0){
				gdcSpeaker->SetPropertyName(igdeGDCSpeaker::epSound, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "looping") == 0){
				gdcSpeaker->SetPropertyName(igdeGDCSpeaker::epLooping, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "playing") == 0){
				gdcSpeaker->SetPropertyName(igdeGDCSpeaker::epPlaying, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "volume") == 0){
				gdcSpeaker->SetPropertyName(igdeGDCSpeaker::epVolume, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "range") == 0){
				gdcSpeaker->SetPropertyName(igdeGDCSpeaker::epRange, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "rollOff") == 0){
				gdcSpeaker->SetPropertyName(igdeGDCSpeaker::epRollOff, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "distanceOffset") == 0){
				gdcSpeaker->SetPropertyName(igdeGDCSpeaker::epDistanceOffset, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "playSpeed") == 0){
				gdcSpeaker->SetPropertyName(igdeGDCSpeaker::epPlaySpeed, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachPosition") == 0){
				gdcSpeaker->SetPropertyName(igdeGDCSpeaker::epAttachPosition, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachRotation") == 0){
				gdcSpeaker->SetPropertyName(igdeGDCSpeaker::epAttachRotation, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else if(tagName == "trigger"){
			const char * const value = GetAttributeString(*tag, "target");
			
			if(strcmp(value, "playing") == 0){
				gdcSpeaker->SetTriggerName(igdeGDCSpeaker::etPlaying, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "muted") == 0){
				gdcSpeaker->SetTriggerName(igdeGDCSpeaker::etMuted, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddSpeaker(gdcSpeaker);
}

void igdeXMLGameDefinition::pParseClassNavigationSpace(const decXmlElementTag &root, igdeGDClass &gdclass){
	const igdeGDCNavigationSpace::Ref gdcNavSpace(igdeGDCNavigationSpace::Ref::New());
	igdeCodecPropertyString codec;
	decShapeList shapeList;
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "path"){
			gdcNavSpace->SetPath(GetCDataString(*tag));
			
		}else if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			gdcNavSpace->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			gdcNavSpace->SetOrientation(decQuaternion::CreateFromEuler(orientation * DEG2RAD));
			
		}else if(tagName == "bone"){
			gdcNavSpace->SetBoneName(GetCDataString(*tag));
			
		}else if(tagName == "layer"){
			gdcNavSpace->SetLayer(GetCDataInt(*tag));
			
		}else if(tagName == "type"){
			const char * const value = GetCDataString(*tag);
			
			if(strcmp(value, "grid") == 0){
				gdcNavSpace->SetType(deNavigationSpace::estGrid);
				
			}else if(strcmp(value, "mesh") == 0){
				gdcNavSpace->SetType(deNavigationSpace::estMesh);
				
			}else if(strcmp(value, "volume") == 0){
				gdcNavSpace->SetType(deNavigationSpace::estVolume);
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else if(tagName == "blockingPriority"){
			gdcNavSpace->SetBlockingPriority(GetCDataInt(*tag));
			
		}else if(tagName == "blockerShape"){
			codec.DecodeShapeList(GetCDataString(*tag), shapeList);
			gdcNavSpace->SetBlockerShapeList(shapeList);
			shapeList.RemoveAll();
			
		}else if(tagName == "snapDistance"){
			gdcNavSpace->SetSnapDistance(GetCDataFloat(*tag));
			
		}else if(tagName == "snapAngle"){
			gdcNavSpace->SetSnapAngle(GetCDataFloat(*tag));
			
		}else if(tagName == "link"){
			const char * const value = GetAttributeString(*tag, "target");
			
			if(strcmp(value, "path") == 0){
				gdcNavSpace->SetPropertyName(igdeGDCNavigationSpace::epPath, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "blockerShape") == 0){
				gdcNavSpace->SetPropertyName(igdeGDCNavigationSpace::epBlockerShape, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "layer") == 0){
				gdcNavSpace->SetPropertyName(igdeGDCNavigationSpace::epLayer, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "blockingPriority") == 0){
				gdcNavSpace->SetPropertyName(igdeGDCNavigationSpace::epBlockingPriority, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "snapDistance") == 0){
				gdcNavSpace->SetPropertyName(igdeGDCNavigationSpace::epSnapDistance, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "snapAngle") == 0){
				gdcNavSpace->SetPropertyName(igdeGDCNavigationSpace::epSnapAngle, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachPosition") == 0){
				gdcNavSpace->SetPropertyName(igdeGDCNavigationSpace::epAttachPosition, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachRotation") == 0){
				gdcNavSpace->SetPropertyName(igdeGDCNavigationSpace::epAttachRotation, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddNavigationSpace(gdcNavSpace);
}

void igdeXMLGameDefinition::pParseClassNavigationBlocker(const decXmlElementTag &root, igdeGDClass &gdclass){
	const igdeGDCNavigationBlocker::Ref gdcNavBlocker(igdeGDCNavigationBlocker::Ref::New());
	igdeCodecPropertyString codec;
	decShapeList shapeList;
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			gdcNavBlocker->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			gdcNavBlocker->SetOrientation(decQuaternion::CreateFromEuler(orientation * DEG2RAD));
			
		}else if(tagName == "scaling"){
			decVector scaling(1.0f, 1.0f, 1.0f);
			ReadVector(*tag, scaling);
			gdcNavBlocker->SetScaling(scaling);
			
		}else if(tagName == "bone"){
			gdcNavBlocker->SetBoneName(GetCDataString(*tag));
			
		}else if(tagName == "enabled"){
			gdcNavBlocker->SetEnabled(GetCDataBool(*tag));
			
		}else if(tagName == "layer"){
			gdcNavBlocker->SetLayer(GetCDataInt(*tag));
			
		}else if(tagName == "type"){
			const char * const value = GetCDataString(*tag);
			
			if(strcmp(value, "grid") == 0){
				gdcNavBlocker->SetType(deNavigationSpace::estGrid);
				
			}else if(strcmp(value, "mesh") == 0){
				gdcNavBlocker->SetType(deNavigationSpace::estMesh);
				
			}else if(strcmp(value, "volume") == 0){
				gdcNavBlocker->SetType(deNavigationSpace::estVolume);
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else if(tagName == "blockingPriority"){
			gdcNavBlocker->SetBlockingPriority(GetCDataInt(*tag));
			
		}else if(tagName == "shape"){
			codec.DecodeShapeList(GetCDataString(*tag), shapeList);
			gdcNavBlocker->SetShapeList(shapeList);
			shapeList.RemoveAll();
			
		}else if(tagName == "link"){
			const char * const value = GetAttributeString(*tag, "target");
			
			if(strcmp(value, "enabled") == 0){
				gdcNavBlocker->SetPropertyName(igdeGDCNavigationBlocker::epEnabled, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "shape") == 0){
				gdcNavBlocker->SetPropertyName(igdeGDCNavigationBlocker::epShape, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "layer") == 0){
				gdcNavBlocker->SetPropertyName(igdeGDCNavigationBlocker::epLayer, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "blockingPriority") == 0){
				gdcNavBlocker->SetPropertyName(igdeGDCNavigationBlocker::epBlockingPriority, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachPosition") == 0){
				gdcNavBlocker->SetPropertyName(igdeGDCNavigationBlocker::epAttachPosition, GetAttributeString(*tag, "property"));
				
			}else if(strcmp(value, "attachRotation") == 0){
				gdcNavBlocker->SetPropertyName(igdeGDCNavigationBlocker::epAttachRotation, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddNavigationBlocker(gdcNavBlocker);
}

void igdeXMLGameDefinition::pParseClassWorld(const decXmlElementTag &root, igdeGDClass &gdclass){
	const igdeGDCWorld::Ref gdcWorld(igdeGDCWorld::Ref::New());
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "path"){
			gdcWorld->SetPath(GetCDataString(*tag));
			
		}else if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			gdcWorld->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			gdcWorld->SetOrientation(decQuaternion::CreateFromEuler(orientation * DEG2RAD));
			
		}else if(tagName == "link"){
			const decString &value = GetAttributeString(*tag, "target");
			
			if(value == "path"){
				gdcWorld->SetPropertyName(igdeGDCWorld::epPath, GetAttributeString(*tag, "property"));
				
			}else if(value == "position"){
				gdcWorld->SetPropertyName(igdeGDCWorld::epPosition, GetAttributeString(*tag, "property"));
				
			}else if(value == "rotation"){
				gdcWorld->SetPropertyName(igdeGDCWorld::epRotation, GetAttributeString(*tag, "property"));
				
			}else{
				LogWarnUnknownValue(*tag, value);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	gdclass.AddWorld(gdcWorld);
}

void igdeXMLGameDefinition::pParseClassTexture(const decXmlElementTag &root, igdeGDClass &gdclass){
	const igdeGDCCTexture::Ref texture(igdeGDCCTexture::Ref::New());
	const int elementCount = root.GetElementCount();
	decStringDictionary properties;
	int i;
	
	texture->SetName(GetAttributeString(root, "name"));
	if(gdclass.GetComponentTextures().HasNamed(texture->GetName())){
		LogWarnGenericProblemValue(root, texture->GetName().GetString(), "A texture with this name exists already.");
	}
	
	for(i=0; i<elementCount; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "skin"){
			texture->SetPathSkin(GetCDataString(*tag));
			
		}else if(tagName == "offset"){
			decVector2 offset;
			ReadVector2(*tag, offset);
			texture->SetOffset(offset);
			
		}else if(tagName == "scale"){
			decVector2 scale(1.0f, 1.0f);
			ReadVector2(*tag, scale);
			texture->SetScale(scale);
			
		}else if(tagName == "rotate"){
			texture->SetRotation(GetCDataFloat(*tag) * DEG2RAD);
			
		}else if(tagName == "tint"){
			decColor color;
			ReadColor(*tag, color);
			texture->SetColorTint(color);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	texture->SetProperties(properties);
	
	gdclass.GetComponentTextures().Add(texture);
}

void igdeXMLGameDefinition::pParseProperty(const decXmlElementTag &root, igdeGDProperty &property){
	const char *type;
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "description"){
			property.SetDescription(ReadMultilineString(*tag));
			
		}else if(tagName == "type"){
			type = GetCDataString(*tag);
			
			if(strcmp(type, "string") == 0){
				property.SetType(igdeGDProperty::eptString);
				
			}else if(strcmp(type, "integer") == 0){
				property.SetType(igdeGDProperty::eptInteger);
				
			}else if(strcmp(type, "point2") == 0){
				property.SetType(igdeGDProperty::eptPoint2);
				
			}else if(strcmp(type, "point3") == 0){
				property.SetType(igdeGDProperty::eptPoint3);
				
			}else if(strcmp(type, "float") == 0){
				property.SetType(igdeGDProperty::eptFloat);
				
			}else if(strcmp(type, "vector2") == 0){
				property.SetType(igdeGDProperty::eptVector2);
				
			}else if(strcmp(type, "vector3") == 0){
				property.SetType(igdeGDProperty::eptVector3);
				
			}else if(strcmp(type, "color") == 0){
				property.SetType(igdeGDProperty::eptColor);
				
			}else if(strcmp(type, "boolean") == 0){
				property.SetType(igdeGDProperty::eptBoolean);
				
			}else if(strcmp(type, "range") == 0){
				property.SetType(igdeGDProperty::eptRange);
				
			}else if(strcmp(type, "selection") == 0){
				property.SetType(igdeGDProperty::eptSelect);
				
			}else if(strcmp(type, "list") == 0){
				property.SetType(igdeGDProperty::eptList);
				
			}else if(strcmp(type, "path") == 0){
				property.SetType(igdeGDProperty::eptPath);
				
			}else if(strcmp(type, "triggerExpression") == 0){
				property.SetType(igdeGDProperty::eptTriggerExpression);
				
			}else if(strcmp(type, "triggerTarget") == 0){
				property.SetType(igdeGDProperty::eptTriggerTarget);
				
			}else if(strcmp(type, "shape") == 0){
				property.SetType(igdeGDProperty::eptShape);
				
			}else if(strcmp(type, "shapeList") == 0){
				property.SetType(igdeGDProperty::eptShapeList);
				
			}else if(strcmp(type, "identifier") == 0){
				property.SetType(igdeGDProperty::eptIdentifier);
				
			}else{
				property.SetType(igdeGDProperty::eptString);
				LogWarnUnknownValue(*tag, type);
			}
			
		}else if(tagName == "range"){
			property.SetMinimumValue(GetAttributeFloat(*tag, "min"));
			property.SetMaximumValue(GetAttributeFloat(*tag, "max"));
			
		}else if(tagName == "default"){
			property.SetDefaultValue(ReadMultilineString(*tag));
			
		}else if(tagName == "option"){
			property.GetOptions().Add(ReadMultilineString(*tag));
			
		}else if(tagName == "pathPatternType"){
			type = GetCDataString(*tag);
			
			if(strcmp(type, "all") == 0){
				property.SetPathPatternType(igdeGDProperty::epptAll);
				
			}else if(strcmp(type, "model") == 0){
				property.SetPathPatternType(igdeGDProperty::epptModel);
				
			}else if(strcmp(type, "skin") == 0){
				property.SetPathPatternType(igdeGDProperty::epptSkin);
				
			}else if(strcmp(type, "rig") == 0){
				property.SetPathPatternType(igdeGDProperty::epptRig);
				
			}else if(strcmp(type, "animation") == 0){
				property.SetPathPatternType(igdeGDProperty::epptAnimation);
				
			}else if(strcmp(type, "animator") == 0){
				property.SetPathPatternType(igdeGDProperty::epptAnimator);
				
			}else if(strcmp(type, "image") == 0){
				property.SetPathPatternType(igdeGDProperty::epptImage);
				
			}else if(strcmp(type, "occlusionMesh") == 0){
				property.SetPathPatternType(igdeGDProperty::epptOcclusionMesh);
				
			}else if(strcmp(type, "audioModel") == 0){
				property.SetPathPatternType(igdeGDProperty::epptAudioModel);
				
			}else if(strcmp(type, "navigationSpace") == 0){
				property.SetPathPatternType(igdeGDProperty::epptNavigationSpace);
				
			}else if(strcmp(type, "particleEmitter") == 0){
				property.SetPathPatternType(igdeGDProperty::epptParticleEmitter);
				
			}else if(strcmp(type, "sound") == 0){
				property.SetPathPatternType(igdeGDProperty::epptSound);
				
			}else if(strcmp(type, "synthesizer") == 0){
				property.SetPathPatternType(igdeGDProperty::epptSynthesizer);
				
			}else if(strcmp(type, "video") == 0){
				property.SetPathPatternType(igdeGDProperty::epptVideo);
				
			}else if(strcmp(type, "font") == 0){
				property.SetPathPatternType(igdeGDProperty::epptFont);
				
			}else if(strcmp(type, "sky") == 0){
				property.SetPathPatternType(igdeGDProperty::epptSky);
				
			}else if(strcmp(type, "camera") == 0){
				property.SetPathPatternType(igdeGDProperty::epptCamera);
				
			}else if(strcmp(type, "world") == 0){
				property.SetPathPatternType(igdeGDProperty::epptWorld);
				
			}else if(strcmp(type, "custom") == 0){
				property.SetPathPatternType(igdeGDProperty::epptCustom);
				
			}else{
				property.SetPathPatternType(igdeGDProperty::epptAll);
				LogWarnUnknownValue(*tag, type);
			}
			
		}else if(tagName == "customPathPattern"){
			pParseCustomFilePatternList(*tag, property.GetCustomPathPattern());
			
		}else if(tagName == "idGroup"){
			property.SetIdentifierGroup(GetCDataString(*tag));
				
		}else if(tagName == "idUsage"){
			property.SetIdentifierUsage(GetCDataBool(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void igdeXMLGameDefinition::pParseCustomFilePatternList(const decXmlElementTag &root, igdeFilePatternList &list){
	igdeFilePattern *pattern = nullptr;
	int e;
	
	for(e=0; e<root.GetElementCount(); e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "add"){
			try{
				pattern = new igdeFilePattern(GetAttributeString(*tag, "name"),
					GetAttributeString(*tag, "pattern"), GetAttributeString(*tag, "default"));
				
				list.AddFilePattern(pattern);
				pattern = nullptr;
				
			}catch(const deException &){
				if(pattern){
					delete pattern;
				}
				pattern = nullptr;
				throw;
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void igdeXMLGameDefinition::pParseCamera(const decXmlElementTag &root, igdeGDCamera *camera){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "position"){
			decVector position;
			ReadVector(*tag, position);
			camera->SetPosition(position);
			
		}else if(tagName == "orientation"){
			decVector rotation;
			ReadVector(*tag, rotation);
			camera->SetOrientation(decQuaternion::CreateFromEuler(rotation * DEG2RAD));
			
		}else if(tagName == "fov"){
			camera->SetFov(GetCDataFloat(*tag));
			
		}else if(tagName == "fovRatio"){
			camera->SetFovRatio(GetCDataFloat(*tag));
			
		}else if(tagName == "imageDistance"){
			camera->SetImageDistance(GetCDataFloat(*tag));
			
		}else if(tagName == "viewDistance"){
			camera->SetViewDistance(GetCDataFloat(*tag));
			
		}else if(tagName == "propName"){
			camera->SetPropName(GetCDataString(*tag));
			
		}else if(tagName == "propPosition"){
			camera->SetPropPosition(GetCDataString(*tag));
			
		}else if(tagName == "propRotation"){
			camera->SetPropRotation(GetCDataString(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void igdeXMLGameDefinition::pParseParticleEmitter(const decXmlElementTag &root, igdeGameDefinition &gamedef){
	igdeGDParticleEmitterManager &manager = gamedef.GetParticleEmitterManager();
	const int elementCount = root.GetElementCount();
	const char *path = nullptr;
	const char *name = nullptr;
	int e;
	
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "path"){
			path = GetCDataString(*tag);
			
		}else if(tagName == "name"){
			name = GetCDataString(*tag);
		}
	}
	
	if(!path){
		LogErrorMissingTag(root, "path");
	}
	if(!name){
		LogErrorMissingTag(root, "name");
	}
	
	if(manager.GetEmitterList().HasWithPath(path)){
		LogErrorGenericProblemValue(root, path, "A particle emitter with this path exists already.");
	}
	if(manager.GetEmitterList().HasWithName(name)){
		LogErrorGenericProblemValue(root, name, "A particle emitter with this name exists already.");
	}
	
	const igdeGDParticleEmitter::Ref emitter(igdeGDParticleEmitter::Ref::New(path, name));
	for(e=0; e<elementCount; e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "path"){
			// done before: skip it
			
		}else if(tagName == "name"){
			// done before: skip it
			
		}else if(tagName == "description"){
			emitter->SetDescription(ReadMultilineString(*tag));
			
		}else if(tagName == "category"){
			emitter->SetCategory(GetCDataString(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	manager.AddEmitter(emitter);
}

void igdeXMLGameDefinition::pParseSkin(const decXmlElementTag &root, igdeGameDefinition &gamedef){
	igdeGDSkinManager &skinManager = *gamedef.GetSkinManager();
	const char *strPath = nullptr;
	const char *strName = nullptr;
	igdeGDSkin *skin = nullptr;
	int e;
	
	// first we have to look for the important tags which are required to construct a new skin.
	for(e=0; e<root.GetElementCount(); e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "path"){
			strPath = GetCDataString(*tag);
			
		}else if(tagName == "name"){
			strName = GetCDataString(*tag);
		}
	}
	
	if(!strPath){
		LogErrorMissingTag(root, "path");
	}
	if(!strName){
		LogErrorMissingTag(root, "name");
	}
	
	if(skinManager.HasSkinWithPath(strPath)){
		LogErrorGenericProblemValue(root, strPath, "A skin with this path exists already.");
	}
	if(skinManager.HasSkinWithName(strName)){
		LogErrorGenericProblemValue(root, strName, "A skin with this name exists already.");
	}
	
	skin = new igdeGDSkin(strPath, strName);
	
	skinManager.AddSkin(skin);
	
	// now we read all the other tags.
	for(e=0; e<root.GetElementCount(); e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "path"){
			// done before: skip it
			
		}else if(tagName == "name"){
			// done before: skip it
			
		}else if(tagName == "description"){
			skin->SetDescription(ReadMultilineString(*tag));
			
		}else if(tagName == "category"){
			skin->SetCategory(GetCDataString(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void igdeXMLGameDefinition::pParseSky(const decXmlElementTag &root, igdeGameDefinition &gamedef){
	igdeGDSkyManager &skyManager = *gamedef.GetSkyManager();
	const char *strPath = nullptr;
	const char *strName = nullptr;
	int e;
	
	// first we have to look for the important tags which are required to construct a new sky.
	for(e=0; e<root.GetElementCount(); e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "path"){
			strPath = GetCDataString(*tag);
			
		}else if(tagName == "name"){
			strName = GetCDataString(*tag);
		}
	}
	
	if(!strPath){
		LogErrorMissingTag(root, "path");
	}
	if(!strName){
		LogErrorMissingTag(root, "name");
	}
	
	if(skyManager.GetSkyList().HasWithPath(strPath)){
		LogErrorGenericProblemValue(root, strPath, "A sky with this path exists already.");
	}
	if(skyManager.GetSkyList().HasWithName(strName)){
		LogErrorGenericProblemValue(root, strName, "A sky with this name exists already.");
	}
	
	const igdeGDSky::Ref sky(igdeGDSky::Ref::New(strPath, strName));
	skyManager.AddSky(sky);
	
	// now we read all the other tags.
	for(e=0; e<root.GetElementCount(); e++){
		const decXmlElementTag * const tag = root.GetElementIfTag(e);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "path"){
			// done before: skip it
			
		}else if(tagName == "name"){
			// done before: skip it
			
		}else if(tagName == "description"){
			sky->SetDescription(ReadMultilineString(*tag));
			
		}else if(tagName == "category"){
			sky->SetCategory(GetCDataString(*tag));
			
		}else if(tagName == "controller"){
			sky->AddController(igdeGDSkyController::Ref::New(
				GetAttributeString(*tag, "name"), GetCDataFloat(*tag)));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void igdeXMLGameDefinition::pParseGDCategories(const decXmlElementTag &root, igdeGameDefinition &gamedef){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "classes"){
			pParseRootCategory(*tag, gamedef.GetClassManager()->GetCategories());
			
		}else if(tagName == "skins"){
			pParseRootCategory(*tag, gamedef.GetSkinManager()->GetCategories());
			
		}else if(tagName == "skies"){
			pParseRootCategory(*tag, gamedef.GetSkyManager()->GetCategories());
			
		}else if(tagName == "particleEmitters"){
			pParseRootCategory(*tag, gamedef.GetParticleEmitterManager().GetCategories());
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void igdeXMLGameDefinition::pParseRootCategory(const decXmlElementTag &root, igdeGDCategory *parent){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "description"){
			parent->SetDescription(ReadMultilineString(*tag));
			
		}else if(tagName == "category"){
			pParseCategory(*tag, parent);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void igdeXMLGameDefinition::pParseCategory(const decXmlElementTag &root, igdeGDCategory *parent){
	igdeGDCategory::Ref category;
	int i;
	
	// first we have to look for the important name tag which is required to construct and add a new category.
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "name"){
			if(category){
				continue;
			}
			
			const char * const categoryName = GetCDataString(*tag);
			if(parent->HasCategoryNamed(categoryName)){
				LogErrorGenericProblemValue(*tag, categoryName, "Category with this name exists already");
			}
			
			category = igdeGDCategory::Ref::New(categoryName);
		}
	}
	if(!category){
		LogErrorMissingTag(root, "name");
	}
	
	parent->AddCategory(category);
	
	// now we read all the other tags.
	decStringSet autoCategorizePattern;
	
	for(i=0; i<root.GetElementCount(); i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if(tagName == "name"){
			// we did this already so we can skip it safely.
			
		}else if(tagName == "description"){
			category->SetDescription(ReadMultilineString(*tag));
			
		}else if(tag->GetName() == "autoCategorize"){
			autoCategorizePattern.Add(GetCDataString(*tag));
			
		}else if(tag->GetName() == "hidden"){
			category->SetHidden(GetCDataBool(*tag));
			
		}else if(tagName == "category"){
			pParseCategory(*tag, category);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	category->SetAutoCategorizePattern(autoCategorizePattern);
}
