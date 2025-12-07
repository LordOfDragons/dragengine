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

#include "peeLoadSaveEmitter.h"
#include "peeLoadSaveSystem.h"
#include "../emitter/peeEmitter.h"
#include "../emitter/peeController.h"
#include "../emitter/peeParameter.h"
#include "../emitter/peeType.h"
#include "../gui/peeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Particle Emitter Editor"

struct sParameter{
	deParticleEmitterType::eParameters parameter;
	const char *name;
};

static const sParameter vParameters[31] = {
	{deParticleEmitterType::epTimeToLive, "timeToLive"},
	{deParticleEmitterType::epInterval, "interval"},
	{deParticleEmitterType::epParticleCount, "particleCount"},
	{deParticleEmitterType::epCastAngleX, "castAngleX"},
	{deParticleEmitterType::epCastAngleY, "castAngleY"},
	{deParticleEmitterType::epSize, "size"},
	{deParticleEmitterType::epRed, "red"},
	{deParticleEmitterType::epGreen, "green"},
	{deParticleEmitterType::epBlue, "blue"},
	{deParticleEmitterType::epTransparency, "transparency"},
	{deParticleEmitterType::epEmissivity, "emissivity"},
	{deParticleEmitterType::epMass, "mass"},
	{deParticleEmitterType::epRotation, "rotation"},
	{deParticleEmitterType::epLinearVelocity, "linearVelocity"},
	{deParticleEmitterType::epAngularVelocity, "angularVelocity"},
	{deParticleEmitterType::epBrownMotion, "brownMotion"},
	{deParticleEmitterType::epDamping, "damping"},
	{deParticleEmitterType::epDrag, "drag"},
	{deParticleEmitterType::epGravityX, "gravityX"},
	{deParticleEmitterType::epGravityY, "gravityY"},
	{deParticleEmitterType::epGravityZ, "gravityZ"},
	{deParticleEmitterType::epLocalGravity, "localGravity"},
	{deParticleEmitterType::epForceFieldDirect, "forceFieldDirect"},
	{deParticleEmitterType::epForceFieldSurface, "forceFieldSurface"},
	{deParticleEmitterType::epForceFieldMass, "forceFieldMass"},
	{deParticleEmitterType::epForceFieldSpeed, "forceFieldSpeed"},
	{deParticleEmitterType::epElasticity, "elasticity"},
	{deParticleEmitterType::epRoughness, "roughness"},
	{deParticleEmitterType::epEmitDirection, "emitDirection"},
	{deParticleEmitterType::epBeamStart, "beamStart"},
	{deParticleEmitterType::epBeamEnd, "beamEnd"}
};

static bool ParameterForName(deParticleEmitterType::eParameters &parameter, const char *name){
	int i;
	for(i=0; i<31; i++){
		if(strcmp(vParameters[i].name, name) == 0){
			parameter = vParameters[i].parameter;
			return true;
		}
	}
	return false;
}



struct sController{
	deParticleEmitterType::eEmitControllers controller;
	const char *name;
};

static const sController vControllers[4] = {
	{deParticleEmitterType::eecLifetime, "lifetime"},
	{deParticleEmitterType::eecMass, "mass"},
	{deParticleEmitterType::eecLinearVelocity, "linearVelocity"},
	{deParticleEmitterType::eecAngularVelocity, "angularVelocity"}
};

static bool ControllerForName(deParticleEmitterType::eEmitControllers &controller, const char *name){
	int i;
	for(i=0; i<4; i++){
		if(strcmp(vControllers[i].name, name) == 0){
			controller = vControllers[i].controller;
			return true;
		}
	}
	return false;
}



// Class peeLoadSaveEmitter
/////////////////////////////

// Constructor, destructor
////////////////////////////

peeLoadSaveEmitter::peeLoadSaveEmitter(peeLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource) : igdeBaseXML(logger, loggerSource){
	pLSSys = lssys;
	
	pName = "Particle Emitter Definition";
	pPattern = ".depemit";
}



// Loading and saving
///////////////////////

void peeLoadSaveEmitter::LoadEmitter(peeLoadSaveSystem&, peeEmitter &emitter, decBaseFileReader &reader){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(! root || strcmp(root->GetName(), "particleEmitter") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pReadEmitter(*root, emitter);
}

void peeLoadSaveEmitter::SaveEmitter(peeLoadSaveSystem&, const peeEmitter &emitter, decBaseFileWriter &writer){
	decXmlWriter xmlWriter(&writer);
	xmlWriter.WriteXMLDeclaration();
	pWriteEmitter(xmlWriter, emitter);
}



// Private Functions
//////////////////////

void peeLoadSaveEmitter::pWriteEmitter(decXmlWriter &writer, const peeEmitter &emitter){
	const peeControllerList &controllerList = emitter.GetControllers();
	const int controllerCount = controllerList.GetCount();
	const peeTypeList &typeList = emitter.GetTypeList();
	const int typeCount = typeList.GetCount();
	int i;
	
	writer.WriteOpeningTag("particleEmitter", false, true);
	
	for(i=0; i<controllerCount; i++){
		pWriteController(writer, *controllerList.GetAt(i));
	}
	
	writer.WriteDataTagFloat("burstLifetime", emitter.GetBurstLifetime());
	writer.WriteDataTagBool("emitBurst", emitter.GetEmitBurst());
	
	if(typeCount > 0){
		writer.WriteNewline();
		for(i=0; i<typeCount; i++){
			pWriteType(writer, emitter, *typeList.GetAt(i));
		}
	}
	
	writer.WriteClosingTag("particleEmitter", true);
}

void peeLoadSaveEmitter::pWriteController(decXmlWriter &writer, const peeController &controller){
	writer.WriteOpeningTag("controller", false, true);
	
	writer.WriteDataTagString("name", controller.GetName().GetString());
	
	writer.WriteOpeningTagStart("limits");
	writer.WriteAttributeFloat("lower", controller.GetLower());
	writer.WriteAttributeFloat("upper", controller.GetUpper());
	writer.WriteOpeningTagEnd(true);
	
	writer.WriteDataTagBool("frozen", controller.GetFrozen());
	writer.WriteDataTagBool("clamp", controller.GetClamp());
	writer.WriteDataTagBool("linkToTime", controller.GetLinkToTime());
	
	writer.WriteClosingTag("controller", true);
}

void peeLoadSaveEmitter::pWriteType(decXmlWriter &writer, const peeEmitter &emitter, const peeType &type){
	int i;
	
	writer.WriteOpeningTag("type", false, true);
	
	writer.WriteDataTagString("name", type.GetName());
	writer.WriteDataTagString("skin", type.GetSkinPath());
	
	if(! type.GetModelPath().IsEmpty()){
		writer.WriteDataTagString("model", type.GetModelPath());
	}
	if(! type.GetModelSkinPath().IsEmpty()){
		writer.WriteDataTagString("modelSkin", type.GetModelSkinPath());
	}
	
	switch(type.GetCastFrom()){
	case deParticleEmitterType::ecfVertex:
		writer.WriteDataTagString("castFrom", "vertex");
		break;
		
	case deParticleEmitterType::ecfFace:
		writer.WriteDataTagString("castFrom", "face");
		break;
		
	case deParticleEmitterType::ecfVolume:
		writer.WriteDataTagString("castFrom", "volume");
		break;
	}
	
	switch(type.GetSimulationType()){
	case deParticleEmitterType::estParticle:
		writer.WriteDataTagString("simulationType", "particle");
		break;
		
	case deParticleEmitterType::estRibbon:
		writer.WriteDataTagString("simulationType", "ribbon");
		break;
		
	case deParticleEmitterType::estBeam:
		writer.WriteDataTagString("simulationType", "beam");
		break;
	}
	
	writer.WriteDataTagBool("intervalAsDistance", type.GetIntervalAsDistance());
	
	if(! type.GetPathTrailEmitter().IsEmpty()){
		writer.WriteDataTagString("trailEmitter", type.GetPathTrailEmitter());
	}
	
	for(i=0; i<4; i++){
		if(type.GetTrailController(vControllers[i].controller).IsEmpty()){
			continue;
		}
		writer.WriteOpeningTagStart("trailController");
		writer.WriteAttributeString("id", vControllers[i].name);
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(type.GetTrailController(vControllers[i].controller));
		writer.WriteClosingTag("trailController", false);
	}
	writer.WriteDataTagFloat("physicsSize", type.GetPhysicsSize());
	
	switch(type.GetCollisionResponse()){
	case deParticleEmitterType::ecrDestroy:
		writer.WriteDataTagString("collisionResponse", "destroy");
		break;
		
	case deParticleEmitterType::ecrPhysical:
		writer.WriteDataTagString("collisionResponse", "physical");
		break;
		
	case deParticleEmitterType::ecrCustom:
		writer.WriteDataTagString("collisionResponse", "custom");
		break;
	}
	
	if(! type.GetPathCollisionEmitter().IsEmpty()){
		writer.WriteDataTagString("collisionEmitter", type.GetPathCollisionEmitter());
	}
	writer.WriteDataTagFloat("emitMinImpulse", type.GetEmitMinImpulse());
	for(i=0; i<4; i++){
		if(type.GetEmitController(vControllers[i].controller).IsEmpty()){
			continue;
		}
		writer.WriteOpeningTagStart("emitController");
		writer.WriteAttributeString("id", vControllers[i].name);
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(type.GetEmitController(vControllers[i].controller));
		writer.WriteClosingTag("emitController", false);
	}
	
	writer.WriteNewline();
	for(i=0; i<31; i++){
		pWriteParameter(writer, emitter, type, vParameters[i].parameter, vParameters[i].name);
	}
	
	writer.WriteClosingTag("type", true);
}

void peeLoadSaveEmitter::pWriteParameter(decXmlWriter &writer, const peeEmitter &emitter,
const peeType &type, deParticleEmitterType::eParameters parameterType, const char *name){
	const peeParameter &parameter = *type.GetParameterAt(parameterType);
	const peeControllerList &controllerList = emitter.GetControllers();
	
	writer.WriteOpeningTagStart("parameter");
	writer.WriteAttributeString("id", name);
	writer.WriteOpeningTagEnd();
	
	writer.WriteDataTagFloat("value", parameter.GetValue());
	if(parameter.GetSpread() > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("spread", parameter.GetSpread());
	}
	
	const int controllerValue = controllerList.IndexOf(parameter.GetControllerValue());
	if(controllerValue != -1){
		writer.WriteDataTagInt("controllerValue", controllerValue);
	}
	
	const int controllerSpread = controllerList.IndexOf(parameter.GetControllerSpread());
	if(controllerSpread != -1){
		writer.WriteDataTagInt("controllerSpread", controllerSpread);
	}
	
	if(parameter.GetCurveValue().GetPointCount() != 0){
		WriteCurveBezier(writer, "curveValue", parameter.GetCurveValue());
	}
	if(parameter.GetCurveSpread().GetPointCount() != 0){
		WriteCurveBezier(writer, "curveSpread", parameter.GetCurveSpread());
	}
	if(parameter.GetCurveProgress().GetPointCount() != 0){
		WriteCurveBezier(writer, "curveProgress", parameter.GetCurveProgress());
	}
	if(parameter.GetCurveBeam().GetPointCount() != 0){
		WriteCurveBezier(writer, "curveBeam", parameter.GetCurveBeam());
	}
	
	writer.WriteClosingTag("parameter", true);
}



void peeLoadSaveEmitter::pReadEmitter(const decXmlElementTag &root, peeEmitter &emitter){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "controller") == 0){
				pReadController(*tag, emitter);
				
			}else if(strcmp(tag->GetName(), "burstLifetime") == 0){
				emitter.SetBurstLifetime(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "emitBurst") == 0){
				emitter.SetEmitBurst(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "type") == 0){
				pReadType(*tag, emitter);
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void peeLoadSaveEmitter::pReadController(const decXmlElementTag &root, peeEmitter &emitter){
	const int elementCount = root.GetElementCount();
	peeController *controller = NULL;
	const decXmlElementTag *tag;
	int e;
	
	try{
		controller = new peeController;
		
		for(e=0; e<elementCount; e++){
			tag = root.GetElementIfTag(e);
			
			if(tag){
				if(strcmp(tag->GetName(), "name") == 0){
					controller->SetName(GetCDataString(*tag));
					
				}else if(strcmp(tag->GetName(), "clamp") == 0){
					controller->SetClamp(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "frozen") == 0){
					controller->SetFrozen(GetCDataBool(*tag));
					
				}else if(strcmp(tag->GetName(), "lower") == 0){ // deprecated
					controller->SetLower(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "upper") == 0){ // deprecated
					controller->SetUpper(GetCDataFloat(*tag));
					
				}else if(strcmp(tag->GetName(), "limits") == 0){
					controller->SetLower(GetAttributeFloat(*tag, "lower"));
					controller->SetUpper(GetAttributeFloat(*tag, "upper"));
				
				}else if(strcmp(tag->GetName(), "linkToTime") == 0){
					controller->SetLinkToTime(GetCDataBool(*tag));
					
				}else{
					LogWarnUnknownTag(root, *tag);
				}
			}
		}
		
		emitter.AddController(controller);
		controller->FreeReference();
		
	}catch(const deException &){
		if(controller){
			controller->FreeReference();
		}
		throw;
	}
}

void peeLoadSaveEmitter::pReadType(const decXmlElementTag &root, peeEmitter &emitter){
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	const char *identifier;
	peeType *type = NULL;
	int i;
	
	try{
		type = new peeType(emitter.GetEngine());
		
		for(i=0; i<elementCount; i++){
			tag = root.GetElementIfTag(i);
			if(! tag){
				continue;
			}
			
			if(strcmp(tag->GetName(), "name") == 0){
				type->SetName(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "skin") == 0){
				type->SetSkinPath(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "model") == 0){
				type->SetModelPath(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "modelSkin") == 0){
				type->SetModelSkinPath(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "castFrom") == 0){
				identifier = GetCDataString(*tag);
				
				if(strcmp(identifier, "vertex") == 0){
					type->SetCastFrom(deParticleEmitterType::ecfVertex);
					
				}else if(strcmp(identifier, "face") == 0){
					type->SetCastFrom(deParticleEmitterType::ecfFace);
					
				}else if(strcmp(identifier, "volume") == 0){
					type->SetCastFrom(deParticleEmitterType::ecfVolume);
					
				}else{
					LogWarnUnknownValue(*tag, identifier);
				}
				
			}else if(strcmp(tag->GetName(), "simulationType") == 0){
				identifier = GetCDataString(*tag);
				
				if(strcmp(identifier, "particle") == 0){
					type->SetSimulationType(deParticleEmitterType::estParticle);
					
				}else if(strcmp(identifier, "ribbon") == 0){
					type->SetSimulationType(deParticleEmitterType::estRibbon);
					
				}else if(strcmp(identifier, "beam") == 0){
					type->SetSimulationType(deParticleEmitterType::estBeam);
					
				}else{
					LogWarnUnknownValue(*tag, identifier);
				}
				
			}else if(strcmp(tag->GetName(), "intervalAsDistance") == 0){
				type->SetIntervalAsDistance(GetCDataBool(*tag));
				
			}else if(strcmp(tag->GetName(), "trailEmitter") == 0){
				type->SetPathTrailEmitter(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "trailController") == 0){
				identifier = GetAttributeString(*tag, "id");
				deParticleEmitterType::eEmitControllers controller;
				if(! ControllerForName(controller, identifier)){
					LogErrorUnknownValue(*tag, identifier);
					continue;
				}
				type->SetTrailController(controller, GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "physicsSize") == 0){
				type->SetPhysicsSize(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "collisionResponse") == 0){
				identifier = GetCDataString(*tag);
				
				if(strcmp(identifier, "destroy") == 0){
					type->SetCollisionResponse(deParticleEmitterType::ecrDestroy);
					
				}else if(strcmp(identifier, "physical") == 0){
					type->SetCollisionResponse(deParticleEmitterType::ecrPhysical);
					
				}else if(strcmp(identifier, "custom") == 0){
					type->SetCollisionResponse(deParticleEmitterType::ecrCustom);
					
				}else{
					LogWarnUnknownValue(*tag, identifier);
				}
				
			}else if(strcmp(tag->GetName(), "collisionEmitter") == 0){
				type->SetPathCollisionEmitter(GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "emitMinImpulse") == 0){
				type->SetEmitMinImpulse(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "emitController") == 0){
				identifier = GetAttributeString(*tag, "id");
				deParticleEmitterType::eEmitControllers controller;
				if(! ControllerForName(controller, identifier)){
					LogErrorUnknownValue(*tag, identifier);
					continue;
				}
				type->SetEmitController(controller, GetCDataString(*tag));
				
			}else if(strcmp(tag->GetName(), "parameter") == 0){
				pReadParameter(*tag, emitter, *type);
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
		
		emitter.AddType(type);
		type->FreeReference();
		
	}catch(const deException &){
		if(type){
			type->FreeReference();
		}
		throw;
	}
}

void peeLoadSaveEmitter::pReadParameter(const decXmlElementTag &root, peeEmitter &emitter, peeType &type){
	const peeControllerList &controllerList = emitter.GetControllers();
	const char *id = GetAttributeString(root, "id");
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	decCurveBezier curve;
	int e, index;
	
	deParticleEmitterType::eParameters parameterEnum;
	if(! ParameterForName(parameterEnum, id)){
		LogErrorUnknownValue(root, id);
		DETHROW(deeInvalidParam);
	}
	peeParameter &parameter = *type.GetParameterAt(parameterEnum);
	
	for(e=0; e<elementCount; e++){
		tag = root.GetElementIfTag(e);
		
		if(tag){
			if(strcmp(tag->GetName(), "value") == 0){
				parameter.SetValue(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "spread") == 0){
				parameter.SetSpread(GetCDataFloat(*tag));
				
			}else if(strcmp(tag->GetName(), "controllerValue") == 0){
				index = GetCDataInt(*tag);
				
				if(index == -1){
					parameter.SetControllerValue(NULL);
					
				}else{
					parameter.SetControllerValue(controllerList.GetAt(index));
				}
				
			}else if(strcmp(tag->GetName(), "controllerSpread") == 0){
				index = GetCDataInt(*tag);
				
				if(index == -1){
					parameter.SetControllerSpread(NULL);
					
				}else{
					parameter.SetControllerSpread(controllerList.GetAt(index));
				}
				
			}else if(strcmp(tag->GetName(), "curveValue") == 0){
				curve.RemoveAllPoints();
				ReadCurveBezier(*tag, curve);
				parameter.SetCurveValue(curve);
				
			}else if(strcmp(tag->GetName(), "curveSpread") == 0){
				curve.RemoveAllPoints();
				ReadCurveBezier(*tag, curve);
				parameter.SetCurveSpread(curve);
				
			}else if(strcmp(tag->GetName(), "curveProgress") == 0){
				curve.RemoveAllPoints();
				ReadCurveBezier(*tag, curve);
				parameter.SetCurveProgress(curve);
				
			}else if(strcmp(tag->GetName(), "curveBeam") == 0){
				curve.RemoveAllPoints();
				ReadCurveBezier(*tag, curve);
				parameter.SetCurveBeam(curve);
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}
