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

#include "seLoadSaveSky.h"
#include "seLoadSaveSystem.h"
#include "../gui/seWindowMain.h"
#include "../sky/seSky.h"
#include "../sky/body/seBody.h"
#include "../sky/controller/seController.h"
#include "../sky/controller/seControllerTarget.h"
#include "../sky/layer/seLayer.h"
#include "../sky/link/seLink.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>



// Class seLoadSaveSky
/////////////////////////

// Constructor, destructor
////////////////////////////

seLoadSaveSky::seLoadSaveSky(seLoadSaveSystem &lssys, deLogger *logger, const char *source) :
igdeBaseXML(logger, source),
pLSSys(lssys),
pName("Sky Definition"),
pPattern(".desky"){
}



// Loading and saving
///////////////////////

void seLoadSaveSky::LoadSky(seLoadSaveSystem &lssys, seSky &sky, decBaseFileReader &reader){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	decXmlParser(lssys.GetWindowMain().GetEnvironment().GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || root->GetName() != "sky"){
		DETHROW(deeInvalidParam);
	}
	
	pReadSky(*root, sky);
}

void seLoadSaveSky::SaveSky(seLoadSaveSystem &lssys, const seSky &sky, decBaseFileWriter &writer){
	decXmlWriter xmlWriter(&writer);
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteSky(xmlWriter, sky);
}



// Private Functions
//////////////////////

void seLoadSaveSky::pWriteSky(decXmlWriter &writer, const seSky &sky){
	int i;
	
	writer.WriteOpeningTag("sky", false, true);
	
	WriteColor(writer, "bgColor", sky.GetBgColor());
	
	const seControllerList &controllers = sky.GetControllers();
	const int controllerCount = controllers.GetCount();
	if(controllerCount > 0){
		writer.WriteNewline();
		for(i=0; i<controllerCount; i++){
			pWriteController(writer, *controllers.GetAt(i));
		}
	}
	
	const seLinkList &links = sky.GetLinks();
	const int linkCount = links.GetCount();
	if(linkCount > 0){
		writer.WriteNewline();
		for(i=0; i<linkCount; i++){
			pWriteLink(writer, *links.GetAt(i));
		}
	}
	
	const seLayerList &layers = sky.GetLayers();
	const int layerCount = layers.GetCount();
	if(layerCount > 0){
		writer.WriteNewline();
		for(i=0; i<layerCount; i++){
			pWriteLayer(writer, *layers.GetAt(i));
		}
	}
	
	writer.WriteClosingTag("sky", true);
}
	
void seLoadSaveSky::pWriteController(decXmlWriter &writer, const seController &controller){
	writer.WriteOpeningTag("controller", false, true);
	
	writer.WriteDataTagString("name", controller.GetName());
	
	writer.WriteOpeningTagStart("limits");
	writer.WriteAttributeFloat("min", controller.GetMinimumValue());
	writer.WriteAttributeFloat("max", controller.GetMaximumValue());
	writer.WriteOpeningTagEnd(true);
	
	if(controller.GetFrozen()){
		writer.WriteDataTagBool("frozen", true);
	}
	if(controller.GetClamp()){
		writer.WriteDataTagBool("clamp", true);
	}
	
	writer.WriteClosingTag("controller", true);
}

void seLoadSaveSky::pWriteLink(decXmlWriter &writer, const seLink &link){
	writer.WriteOpeningTag("link", false, true);
	
	writer.WriteDataTagString("name", link.GetName());
	
	const seController * const controller = link.GetController();
	if(controller){
		writer.WriteDataTagInt("controller", controller->GetIndex());
	}
	
	WriteCurveBezier(writer, "curve", link.GetCurve());
	
	if(link.GetRepeat() > 1){
		writer.WriteDataTagInt("repeat", link.GetRepeat());
	}
	
	writer.WriteClosingTag("link", true);
}

void seLoadSaveSky::pWriteLayer(decXmlWriter &writer, const seLayer &layer){
	writer.WriteOpeningTag("layer", false, true);
	
	writer.WriteDataTagString("name", layer.GetName().GetString());
	
	const decVector &offset = layer.GetOffset();
	if(!offset.IsZero()){
		WriteVector(writer, "offset", offset);
	}
	
	const decVector &orientation = layer.GetOrientation();
	if(!orientation.IsZero()){
		WriteVector(writer, "orientation", orientation);
	}
	
	const decColor &color = layer.GetColor();
	if(!color.IsEqualTo(decColor(1.0f, 1.0f, 1.0f))){
		WriteColor(writer, "color", color);
	}
	
	writer.WriteDataTagFloat("intensity", layer.GetIntensity());
	
	if(fabsf(layer.GetTransparency() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("transparency", layer.GetTransparency());
	}
	
	if(layer.GetMuliplyBySkyLight()){
		writer.WriteDataTagBool("mulBySkyLight", true);
	}
	if(layer.GetMuliplyBySkyColor()){
		writer.WriteDataTagBool("mulBySkyColor", true);
	}
	
	if(!layer.GetSkinPath().IsEmpty()){
		writer.WriteDataTagString("skin", layer.GetSkinPath());
	}
	
	if(!layer.GetLightOrientation().IsZero()){
		WriteVector(writer, "lightOrientation", layer.GetLightOrientation());
	}
	
	if(!layer.GetLightColor().IsEqualTo(decColor(1.0f, 1.0f, 1.0f))){
		WriteColor(writer, "lightColor", layer.GetLightColor());
	}
	
	if(fabsf(layer.GetLightIntensity()) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("lightIntensity", layer.GetLightIntensity());
	}
	
	if(fabsf(layer.GetAmbientIntensity()) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("ambientIntensity", layer.GetAmbientIntensity());
	}
	
	const seBodyList &bodies = layer.GetBodies();
	int bodyCount = bodies.GetCount();
	int i;
	
	if(bodyCount > 0){
		for(i=0; i<bodyCount; i++){
			pWriteBody(writer, *bodies.GetAt(i));
		}
	}
	
	for(i=deSkyLayer::etOffsetX; i<=deSkyLayer::etAmbientIntensity; i++){
		pWriteTarget(writer, layer, (deSkyLayer::eTargets)i);
	}
	
	writer.WriteClosingTag("layer", true);
}

void seLoadSaveSky::pWriteTarget(decXmlWriter &writer, const seLayer &layer,
deSkyLayer::eTargets target){
	const seControllerTarget &ctarget = layer.GetTarget(target);
	const seLinkList &links = ctarget.GetLinks();
	int linkCount = links.GetCount();
	if(linkCount == 0){
		return;
	}
	
	writer.WriteOpeningTagStart("target");
	
	switch(target){
	case deSkyLayer::etOffsetX:
		writer.WriteAttributeString("type", "offsetX");
		break;
		
	case deSkyLayer::etOffsetY:
		writer.WriteAttributeString("type", "offsetY");
		break;
		
	case deSkyLayer::etOffsetZ:
		writer.WriteAttributeString("type", "offsetZ");
		break;
		
	case deSkyLayer::etOrientationX:
		writer.WriteAttributeString("type", "orientationX");
		break;
		
	case deSkyLayer::etOrientationY:
		writer.WriteAttributeString("type", "orientationY");
		break;
		
	case deSkyLayer::etOrientationZ:
		writer.WriteAttributeString("type", "orientationZ");
		break;
		
	case deSkyLayer::etRotationX:
		writer.WriteAttributeString("type", "rotationX");
		break;
		
	case deSkyLayer::etRotationY:
		writer.WriteAttributeString("type", "rotationY");
		break;
		
	case deSkyLayer::etRotationZ:
		writer.WriteAttributeString("type", "rotationZ");
		break;
		
	case deSkyLayer::etColorR:
		writer.WriteAttributeString("type", "colorR");
		break;
		
	case deSkyLayer::etColorG:
		writer.WriteAttributeString("type", "colorG");
		break;
		
	case deSkyLayer::etColorB:
		writer.WriteAttributeString("type", "colorB");
		break;
		
	case deSkyLayer::etIntensity:
		writer.WriteAttributeString("type", "intensity");
		break;
		
	case deSkyLayer::etTransparency:
		writer.WriteAttributeString("type", "transparency");
		break;
		
	case deSkyLayer::etLightColorR:
		writer.WriteAttributeString("type", "lightColorR");
		break;
		
	case deSkyLayer::etLightColorG:
		writer.WriteAttributeString("type", "lightColorG");
		break;
		
	case deSkyLayer::etLightColorB:
		writer.WriteAttributeString("type", "lightColorB");
		break;
		
	case deSkyLayer::etLightIntensity:
		writer.WriteAttributeString("type", "lightIntensity");
		break;
		
	case deSkyLayer::etAmbientIntensity:
		writer.WriteAttributeString("type", "ambientIntensity");
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	writer.WriteOpeningTagEnd();
	
	int i;
	for(i=0; i<linkCount; i++){
		writer.WriteDataTagInt("link", links.GetAt(i)->GetIndex());
	}
	
	writer.WriteClosingTag("target", true);
}

void seLoadSaveSky::pWriteBody(decXmlWriter &writer, const seBody &body){
	writer.WriteOpeningTag("body", false, true);
	
	const decVector &orientation = body.GetOrientation();
	if(!orientation.IsZero()){
		WriteVector(writer, "orientation", orientation);
	}
	
	const decVector2 &size = body.GetSize();
	WriteVector2(writer, "size", size);
	
	const decColor &color = body.GetColor();
	if(!color.IsEqualTo(decColor(1.0f, 1.0f, 1.0f))){
		WriteColor(writer, "color", color);
	}
	
	if(!body.GetSkinPath().IsEmpty()){
		writer.WriteDataTagString("skin", body.GetSkinPath().GetString());
	}
	
	writer.WriteClosingTag("body", true);
}



void seLoadSaveSky::pReadSky(const decXmlElementTag &root, seSky &sky){
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "bgColor"){
			decColor color;
			ReadColor(*tag, color);
			sky.SetBgColor(color);
			
		}else if(tagName == "controller"){
			pReadController(*tag, sky);
			
		}else if(tagName == "link"){
			pReadLink(*tag, sky);
			
		}else if(tagName == "layer"){
			pReadLayer(*tag, sky);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void seLoadSaveSky::pReadController(const decXmlElementTag &root, seSky &sky){
	const int elementCount = root.GetElementCount();
	seController *controller = NULL;
	int i;
	
	try{
		controller = new seController;
		
		for(i=0; i<elementCount; i++){
			decXmlElementTag * const tag = root.GetElementIfTag(i);
			if(!tag){
				continue;
			}
			
			const decString &tagName = tag->GetName();
			
			if(tagName == "name"){
				controller->SetName(GetCDataString(*tag));
				
			}else if(tagName == "clamp"){
				controller->SetClamp(GetCDataBool(*tag));
				
			}else if(tagName == "frozen"){
				controller->SetFrozen(GetCDataBool(*tag));
				
			}else if(tagName == "limits"){
				controller->SetMinimumValue(GetAttributeFloat(*tag, "min"));
				controller->SetMaximumValue(GetAttributeFloat(*tag, "max"));
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
		
		sky.AddController(controller);
		controller->FreeReference();
		
	}catch(const deException &){
		throw;
	}
}

void seLoadSaveSky::pReadLink(const decXmlElementTag &root, seSky &sky){
	const seLink::Ref link(seLink::Ref::NewWith());
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "name"){
			link->SetName(GetCDataString(*tag));
			
		}else if(tagName == "controller"){
			const int index = GetCDataInt(*tag);
			if(index != -1){
				link->SetController(sky.GetControllers().GetAt(index));
			}
			
		}else if(tagName == "repeat"){
			link->SetRepeat(GetCDataInt(*tag));
			
		}else if(tagName == "curve"){
			ReadCurveBezier(*tag, link->GetCurve());
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	sky.AddLink(link);
}

void seLoadSaveSky::pReadLayer(const decXmlElementTag &root, seSky &sky){
	const seLayer::Ref layer(seLayer::Ref::NewWith(*sky.GetEnvironment()));
	const int elementCount = root.GetElementCount();
	int i;
	
	sky.AddLayer(layer); // ensures relative path can be properly resolved
	
	for(i=0; i<elementCount; i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "name"){
			layer->SetName(GetCDataString(*tag));
			
		}else if(tagName == "offset"){
			decVector offset;
			ReadVector(*tag, offset);
			layer->SetOffset(offset);
			
		}else if(tagName == "orientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			layer->SetOrientation(orientation);
			
		}else if(tagName == "color"){
			decColor color(1.0f, 1.0f, 1.0f);
			ReadColor(*tag, color);
			layer->SetColor(color);
			
		}else if(tagName == "intensity"){
			layer->SetIntensity(GetCDataFloat(*tag));
			
		}else if(tagName == "transparency"){
			layer->SetTransparency(GetCDataFloat(*tag));
			
		}else if(tagName == "mulBySkyLight"){
			layer->SetMultiplyBySkyLight(GetCDataBool(*tag));
			
		}else if(tagName == "mulBySkyColor"){
			layer->SetMultiplyBySkyColor(GetCDataBool(*tag));
			
		}else if(tagName == "skin"){
			layer->SetSkinPath(GetCDataString(*tag));
			
		}else if(tagName == "lightOrientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			layer->SetLightOrientation(orientation);
			
		}else if(tagName == "lightColor"){
			decColor color(1.0f, 1.0f, 1.0f);
			ReadColor(*tag, color);
			layer->SetLightColor(color);
			
		}else if(tagName == "lightIntensity"){
			layer->SetLightIntensity(GetCDataFloat(*tag));
			
		}else if(tagName == "ambientIntensity"){
			layer->SetAmbientIntensity(GetCDataFloat(*tag));
			
		}else if(tagName == "body"){
			pReadBody(*tag, sky, layer);
			
		}else if(tagName == "target"){
			pReadTarget(*tag, sky, layer);
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void seLoadSaveSky::pReadTarget(const decXmlElementTag &root, seSky &sky, seLayer &layer){
	const decString &type = GetAttributeString(root, "type");
	seControllerTarget *target = NULL;
	
	if(type == "offsetX"){
		target = &layer.GetTarget(deSkyLayer::etOffsetX);
		
	}else if(type == "offsetY"){
		target = &layer.GetTarget(deSkyLayer::etOffsetY);
		
	}else if(type == "offsetZ"){
		target = &layer.GetTarget(deSkyLayer::etOffsetZ);
		
	}else if(type == "orientationX"){
		target = &layer.GetTarget(deSkyLayer::etOrientationX);
		
	}else if(type == "orientationY"){
		target = &layer.GetTarget(deSkyLayer::etOrientationY);
		
	}else if(type == "orientationZ"){
		target = &layer.GetTarget(deSkyLayer::etOrientationZ);
		
	}else if(type == "rotationX"){
		target = &layer.GetTarget(deSkyLayer::etRotationX);
		
	}else if(type == "rotationY"){
		target = &layer.GetTarget(deSkyLayer::etRotationY);
		
	}else if(type == "rotationZ"){
		target = &layer.GetTarget(deSkyLayer::etRotationZ);
		
	}else if(type == "colorR"){
		target = &layer.GetTarget(deSkyLayer::etColorR);
		
	}else if(type == "colorG"){
		target = &layer.GetTarget(deSkyLayer::etColorG);
		
	}else if(type == "colorB"){
		target = &layer.GetTarget(deSkyLayer::etColorB);
		
	}else if(type == "intensity"){
		target = &layer.GetTarget(deSkyLayer::etIntensity);
		
	}else if(type == "transparency"){
		target = &layer.GetTarget(deSkyLayer::etTransparency);
		
	}else if(type == "lightColorR"){
		target = &layer.GetTarget(deSkyLayer::etLightColorR);
		
	}else if(type == "lightColorG"){
		target = &layer.GetTarget(deSkyLayer::etLightColorG);
		
	}else if(type == "lightColorB"){
		target = &layer.GetTarget(deSkyLayer::etLightColorB);
		
	}else if(type == "lightIntensity"){
		target = &layer.GetTarget(deSkyLayer::etLightIntensity);
		
	}else if(type == "ambientIntensity"){
		target = &layer.GetTarget(deSkyLayer::etAmbientIntensity);
		
	}else{
		return;
	}
	
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "link"){
			seLink *link = sky.GetLinks().GetAt(GetCDataInt(*tag));
			if(!target->GetLinks().Has(link)){
				target->AddLink(link);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void seLoadSaveSky::pReadBody(const decXmlElementTag &root, seSky &sky, seLayer &layer){
	const seBody::Ref body(seBody::Ref::NewWith(sky.GetEngine()));
	const int elementCount = root.GetElementCount();
	int i;
	
	layer.AddBody(body); // ensures relative path can be properly resolved
	
	for(i=0; i<elementCount; i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if(tagName == "orientation"){
			decVector orientation;
			ReadVector(*tag, orientation);
			body->SetOrientation(orientation);
			
		}else if(tagName == "size"){
			decVector2 size(5.0f, 5.0f);
			ReadVector2(*tag, size);
			body->SetSize(size);
			
		}else if(tagName == "color"){
			decColor color(1.0f, 1.0f, 1.0f);
			ReadColor(*tag, color);
			body->SetColor(color);
			
		}else if(tagName == "skin"){
			body->SetSkinPath(GetCDataString(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}
