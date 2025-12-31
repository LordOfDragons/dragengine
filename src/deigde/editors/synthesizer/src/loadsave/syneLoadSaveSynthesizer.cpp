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

#include "syneLoadSaveSynthesizer.h"
#include "syneLoadSaveSystem.h"
#include "../gui/syneWindowMain.h"
#include "../synthesizer/syneSynthesizer.h"
#include "../synthesizer/controller/syneController.h"
#include "../synthesizer/controller/syneControllerTarget.h"
#include "../synthesizer/link/syneLink.h"
#include "../synthesizer/source/syneSourceSound.h"
#include "../synthesizer/source/syneSourceWave.h"
#include "../synthesizer/source/syneSourceChain.h"
#include "../synthesizer/source/syneSourceSynthesizer.h"
#include "../synthesizer/source/syneSourceGroup.h"
#include "../synthesizer/effect/syneEffect.h"
#include "../synthesizer/effect/syneEffectStretch.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerManager.h>
#include <dragengine/resources/synthesizer/deSynthesizerController.h>
#include <dragengine/resources/synthesizer/deSynthesizerControllerTarget.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>



// Definitions
////////////////

#define LOGSOURCE "Synthesizer Editor"



// Class syneLoadSaveSynthesizer
////////////////////////////////

// Constructor, destructor
////////////////////////////

syneLoadSaveSynthesizer::syneLoadSaveSynthesizer(syneLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource),
pLSSys(lssys),
pName("Drag[en]gine XML Synthesizer"),
pPattern(".desynth")
{
	if(!lssys){
		DETHROW(deeInvalidParam);
	}
}



// Management
///////////////

void syneLoadSaveSynthesizer::LoadSynthesizer(syneSynthesizer &synthesizer, decBaseFileReader &reader){
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::New());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || strcmp(root->GetName(), "synthesizer") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pReadSynthesizer(*root, synthesizer);
}

void syneLoadSaveSynthesizer::SaveSynthesizer(const syneSynthesizer &synthesizer, decBaseFileWriter &writer){
	decXmlWriter xmlWriter(&writer);
	
	xmlWriter.WriteXMLDeclaration();
	
	pWriteSynthesizer(xmlWriter, synthesizer);
}



// Private functions
//////////////////////

void syneLoadSaveSynthesizer::pWriteSynthesizer(decXmlWriter &writer, const syneSynthesizer &synthesizer){
	writer.WriteOpeningTag("synthesizer", false, true);
	
	synthesizer.GetControllers().VisitIndexed([&](int index, const syneController &controller){
		if(index > 0){
			writer.WriteNewline();
		}
		pWriteController(writer, controller);
	});
	
	synthesizer.GetLinks().Visit([&](const syneLink &link){
		writer.WriteNewline();
		pWriteLink(writer, synthesizer, link);
	});
	
	synthesizer.GetSources().Visit([&](const syneSource &source){
		writer.WriteNewline();
		pWriteSource(writer, synthesizer, source);
	});
	
	writer.WriteDataTagInt("channels", synthesizer.GetChannelCount());
	writer.WriteDataTagInt("sampleRate", synthesizer.GetSampleRate());
	writer.WriteDataTagInt("bytesPerSample", synthesizer.GetBytesPerSample());
	writer.WriteDataTagInt("sampleCount", synthesizer.GetSampleCount());
	
	writer.WriteClosingTag("synthesizer", true);
}

void syneLoadSaveSynthesizer::pWriteController(decXmlWriter &writer,
const syneController &controller){
	writer.WriteOpeningTag("controller");
	
	writer.WriteDataTagString("name", controller.GetName());
	
	writer.WriteOpeningTagStart("limits");
	writer.WriteAttributeFloat("min", controller.GetMinimumValue());
	writer.WriteAttributeFloat("max", controller.GetMaximumValue());
	writer.WriteOpeningTagEnd(true);
	
	if(!controller.GetClamp()){
		writer.WriteDataTagBool("clamp", false);
	}
	
	writer.WriteClosingTag("controller");
}

void syneLoadSaveSynthesizer::pWriteLink(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneLink &link){
	int controllerIndex = -1;
	
	if(link.GetController()){
		controllerIndex = synthesizer.GetControllers().IndexOf(link.GetController());
	}
	
	writer.WriteOpeningTag("link");
	
	writer.WriteDataTagString("name", link.GetName());
	writer.WriteDataTagInt("controller", controllerIndex);
	if(link.GetRepeat() != 1){
		writer.WriteDataTagInt("repeat", link.GetRepeat());
	}
	
	WriteCurveBezier(writer, "curve", link.GetCurve());
	
	writer.WriteClosingTag("link");
}

void syneLoadSaveSynthesizer::pWriteSourceCommon(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneSource &source){
	writer.WriteDataTagString("name", source.GetName());
	writer.WriteDataTagBool("enabled", source.GetEnabled());
	
	switch(source.GetMixMode()){
	case deSynthesizerSource::emmBlend:
		writer.WriteDataTagString("mixMode", "blend");
		break;
		
	case deSynthesizerSource::emmAdd:
		writer.WriteDataTagString("mixMode", "add");
		break;
	}
	
	if(fabsf(source.GetBlendFactor() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("blendFactor", source.GetBlendFactor());
	}
	if(fabsf(source.GetMinVolume() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("minVolume", source.GetMinVolume());
	}
	if(fabsf(source.GetMaxVolume() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("maxVolume", source.GetMaxVolume());
	}
	if(fabsf(source.GetMinPanning() - 0.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("minPanning", source.GetMinPanning());
	}
	if(fabsf(source.GetMaxPanning() - 0.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("maxPanning", source.GetMaxPanning());
	}
	
	pWriteControllerTarget(writer, synthesizer, source.GetTargetBlendFactor(), "blendFactor");
	pWriteControllerTarget(writer, synthesizer, source.GetTargetVolume(), "volume");
	pWriteControllerTarget(writer, synthesizer, source.GetTargetPanning(), "panning");
	
	const syneEffect::List &effects = source.GetEffects();
	const int effectCount = effects.GetCount();
	int i;
	
	for(i=0; i<effectCount; i++){
		pWriteEffect(writer, synthesizer, *effects.GetAt(i));
	}
}

void syneLoadSaveSynthesizer::pWriteControllerTarget(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneControllerTarget &target, const char *name){
	if(target.GetLinks().IsEmpty()){
		return;
	}
	
	writer.WriteOpeningTagStart("target");
	writer.WriteAttributeString("name", name);
	writer.WriteOpeningTagEnd();
	
	const syneLink::List &links = synthesizer.GetLinks();
	target.GetLinks().Visit([&](syneLink *link){
		writer.WriteDataTagInt("link", links.IndexOf(link));
	});
	
	writer.WriteClosingTag("target");
}

void syneLoadSaveSynthesizer::pWriteSource(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneSource &source){
	switch(source.GetType()){
	case deSynthesizerSourceVisitorIdentify::estSound:
		pWriteSourceSound(writer, synthesizer, (syneSourceSound&)source);
		break;
		
	case deSynthesizerSourceVisitorIdentify::estWave:
		pWriteSourceWave(writer, synthesizer, (syneSourceWave&)source);
		break;
		
	case deSynthesizerSourceVisitorIdentify::estChain:
		pWriteSourceChain(writer, synthesizer, (syneSourceChain&)source);
		break;
		
	case deSynthesizerSourceVisitorIdentify::estGroup:
		pWriteSourceGroup(writer, synthesizer, (syneSourceGroup&)source);
		break;
		
	case deSynthesizerSourceVisitorIdentify::estSynthesizer:
		pWriteSourceSynthesizer(writer, synthesizer, (syneSourceSynthesizer&)source);
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void syneLoadSaveSynthesizer::pWriteSourceSound(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneSourceSound &source){
	writer.WriteOpeningTag("sourceSound");
	
	pWriteSourceCommon(writer, synthesizer, source);
	
	writer.WriteDataTagString("sound", source.GetPathSound());
	if(fabsf(source.GetMinSpeed() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("minSpeed", source.GetMinSpeed());
	}
	if(fabsf(source.GetMaxSpeed() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("maxSpeed", source.GetMaxSpeed());
	}
	if(source.GetLooping()){
		writer.WriteDataTagBool("looping", true);
	}
	
	pWriteControllerTarget(writer, synthesizer, source.GetTargetSpeed(), "speed");
	pWriteControllerTarget(writer, synthesizer, source.GetTargetPlay(), "play");
	
	writer.WriteClosingTag("sourceSound");
}

void syneLoadSaveSynthesizer::pWriteSourceWave(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneSourceWave &source){
	writer.WriteOpeningTag("sourceWave");
	
	pWriteSourceCommon(writer, synthesizer, source);
	
	switch(source.GetWaveType()){
	case deSynthesizerSourceWave::ewtSine:
		writer.WriteDataTagString("type", "sine");
		break;
		
	case deSynthesizerSourceWave::ewtSquare:
		writer.WriteDataTagString("type", "square");
		break;
		
	case deSynthesizerSourceWave::ewtSawTooth:
		writer.WriteDataTagString("type", "sawtooth");
		break;
		
	case deSynthesizerSourceWave::ewtTriangle:
		writer.WriteDataTagString("type", "triangle");
		break;
	}
	
	writer.WriteDataTagFloat("minFrequency", source.GetMinFrequency());
	writer.WriteDataTagFloat("maxFrequency", source.GetMaxFrequency());
	
	pWriteControllerTarget(writer, synthesizer, source.GetTargetFrequency(), "frequency");
	
	writer.WriteClosingTag("sourceWave");
}

void syneLoadSaveSynthesizer::pWriteSourceChain(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneSourceChain &source){
	writer.WriteOpeningTag("sourceChain");
	
	pWriteSourceCommon(writer, synthesizer, source);
	
	const decStringList &pathSounds = source.GetPathSounds();
	const int count = pathSounds.GetCount();
	int i;
	for(i=0; i<count; i++){
		writer.WriteDataTagString("sound", pathSounds.GetAt(i));
	}
	
	if(fabsf(source.GetMinSpeed() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("minSpeed", source.GetMinSpeed());
	}
	if(fabsf(source.GetMaxSpeed() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("maxSpeed", source.GetMaxSpeed());
	}
	
	pWriteControllerTarget(writer, synthesizer, source.GetTargetSpeed(), "speed");
	pWriteControllerTarget(writer, synthesizer, source.GetTargetSelect(), "selection");
	pWriteControllerTarget(writer, synthesizer, source.GetTargetPlay(), "play");
	
	writer.WriteClosingTag("sourceChain");
}

void syneLoadSaveSynthesizer::pWriteSourceGroup(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneSourceGroup &source){
	writer.WriteOpeningTag("sourceGroup");
	
	pWriteSourceCommon(writer, synthesizer, source);
	
	const int count = source.GetSources().GetCount();
	int i;
	for(i=0; i<count; i++){
		writer.WriteNewline();
		pWriteSource(writer, synthesizer, *source.GetSources().GetAt(i));
	}
	
	switch(source.GetApplicationType()){
	case deSynthesizerSourceGroup::eatAll:
		writer.WriteDataTagString("type", "all");
		break;
		
	case deSynthesizerSourceGroup::eatSelect:
		writer.WriteDataTagString("type", "selection");
		break;
		
	case deSynthesizerSourceGroup::eatSolo:
		writer.WriteDataTagString("type", "solo");
		break;
	}
	
	pWriteControllerTarget(writer, synthesizer, source.GetTargetSelect(), "selection");
	
	writer.WriteClosingTag("sourceGroup");
}

void syneLoadSaveSynthesizer::pWriteSourceSynthesizer(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneSourceSynthesizer &source){
	const syneController::List &controllers = synthesizer.GetControllers();
	
	writer.WriteOpeningTag("sourceSynthesizer");
	
	pWriteSourceCommon(writer, synthesizer, source);
	
	writer.WriteDataTagString("synthesizer", source.GetPathSynthesizer());
	writer.WriteDataTagInt("connectionCount", source.GetConnections().GetCount());
	
	source.GetConnections().VisitIndexed([&](int i, syneController *controller){
		writer.WriteOpeningTagStart("connection");
		writer.WriteAttributeInt("target", i);
		
		if(controller){
			writer.WriteAttributeInt("controller", controllers.IndexOf(controller));
			
		}else{
			writer.WriteAttributeInt("controller", -1);
		}
		
		writer.WriteOpeningTagEnd(true);
	});
	
	writer.WriteClosingTag("sourceSynthesizer");
}

void syneLoadSaveSynthesizer::pWriteEffectCommon(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneEffect &effect){
	writer.WriteDataTagBool("enabled", effect.GetEnabled());
	
	if(fabsf(effect.GetStrength() - 1.0f) > FLOAT_SAFE_EPSILON){
		writer.WriteDataTagFloat("strength", effect.GetStrength());
	}
	
	pWriteControllerTarget(writer, synthesizer, effect.GetTargetStrength(), "strength");
}

void syneLoadSaveSynthesizer::pWriteEffect(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneEffect &effect){
	switch(effect.GetType()){
	case deSynthesizerEffectVisitorIdentify::eetStretch:
		pWriteEffectStretch(writer, synthesizer, (syneEffectStretch&)effect);
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void syneLoadSaveSynthesizer::pWriteEffectStretch(decXmlWriter &writer,
const syneSynthesizer &synthesizer, const syneEffectStretch &effect){
	writer.WriteOpeningTag("effectStretch");
	
	pWriteEffectCommon(writer, synthesizer, effect);
	
	writer.WriteDataTagFloat("minTime", effect.GetMinTime());
	writer.WriteDataTagFloat("maxTime", effect.GetMaxTime());
	writer.WriteDataTagFloat("minPitch", effect.GetMinPitch());
	writer.WriteDataTagFloat("maxPitch", effect.GetMaxPitch());
	
	pWriteControllerTarget(writer, synthesizer, effect.GetTargetTime(), "time");
	pWriteControllerTarget(writer, synthesizer, effect.GetTargetPitch(), "pitch");
	
	writer.WriteClosingTag("effectStretch");
}



void syneLoadSaveSynthesizer::pReadSynthesizer(const decXmlElementTag &root, syneSynthesizer &synthesizer){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "controller"){
			pReadController(*tag, synthesizer);
			
		}else if(tagName == "link"){
			pReadLink(*tag, synthesizer);
			
		}else if(tagName == "channels"){
			synthesizer.SetChannelCount(GetCDataInt(*tag));
			
		}else if(tagName == "sampleRate"){
			synthesizer.SetSampleRate(GetCDataInt(*tag));
			
		}else if(tagName == "bytesPerSample"){
			synthesizer.SetBytesPerSample(GetCDataInt(*tag));
			
		}else if(tagName == "sampleCount"){
			synthesizer.SetSampleCount(GetCDataInt(*tag));
			
		}else{
			const syneSource::Ref source(pReadSource(*tag, synthesizer));
			if(source){
				synthesizer.AddSource(source);
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
}

void syneLoadSaveSynthesizer::pReadController(const decXmlElementTag &root, syneSynthesizer &synthesizer){
	const syneController::Ref controller(syneController::Ref::New());
	synthesizer.AddController(controller);
	
	int i;
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "name"){
			controller->SetName(GetCDataString(*tag));
			
		}else if(tagName == "clamp"){
			controller->SetClamp(GetCDataBool(*tag));
			
		}else if(tagName == "limit"){
			pReadControllerLimit(*tag, *controller);
			
		}else if(tagName == "limits"){
			controller->SetMinimumValue(GetAttributeFloat(*tag, "min"));
			controller->SetMaximumValue(GetAttributeFloat(*tag, "max"));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void syneLoadSaveSynthesizer::pReadControllerLimit(
const decXmlElementTag &root, syneController &controller){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "minimum"){
			controller.SetMinimumValue(GetCDataFloat(*tag));
			
		}else if(tagName == "maximum"){
			controller.SetMaximumValue(GetCDataFloat(*tag));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

void syneLoadSaveSynthesizer::pReadLink(const decXmlElementTag &root, syneSynthesizer &synthesizer){
	const syneLink::Ref link(syneLink::Ref::New());
	int i;
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "name"){
			link->SetName(GetCDataString(*tag));
			
		}else if(tagName == "controller"){
			const int index = GetCDataInt(*tag);
			
			if(index == -1){
				link->SetController(nullptr);
				
			}else{
				link->SetController(synthesizer.GetControllers().GetAt(index));
			}
			
		}else if(tagName == "repeat"){
			link->SetRepeat(GetCDataInt(*tag));
			
		}else if(tagName == "curve"){
			ReadCurveBezier(*tag, link->GetCurve());
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	link->UpdateCurve();
	synthesizer.AddLink(link);
}

syneSource::Ref syneLoadSaveSynthesizer::pReadSource(const decXmlElementTag &root, syneSynthesizer &synthesizer){
	const decString tagName(root.GetName());
	
	if(tagName == "sourceSound"){
		return pReadSourceSound(root, synthesizer);
		
	}else if(tagName == "sourceWave"){
		return pReadSourceWave(root, synthesizer);
		
	}else if(tagName == "sourceChain"){
		return pReadSourceChain(root, synthesizer);
		
	}else if(tagName == "sourceGroup"){
		return pReadSourceGroup(root, synthesizer);
		
	}else if(tagName == "sourceSynthesizer"){
		return pReadSourceSynthesizer(root, synthesizer);
	}
	
	return {};
}

syneSource::Ref syneLoadSaveSynthesizer::pReadSourceSound(const decXmlElementTag &root, syneSynthesizer &synthesizer){
	const syneSourceSound::Ref source(syneSourceSound::Ref::New(synthesizer.GetEngine()));
	
	int i;
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(pReadSourceCommon(*tag, synthesizer, *source)){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "sound"){
			source->SetPathSound(GetCDataString(*tag));
			
		}else if(tagName == "minSpeed"){
			source->SetMinSpeed(GetCDataFloat(*tag));
			
		}else if(tagName == "maxSpeed"){
			source->SetMaxSpeed(GetCDataFloat(*tag));
			
		}else if(tagName == "looping"){
			source->SetLooping(GetCDataBool(*tag));
			
		}else if(tagName == "target"){
			const decString name(GetAttributeString(*tag, "name"));
			
			if(name == "blendFactor"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetBlendFactor());
				
			}else if(name == "volume"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetVolume());
				
			}else if(name == "panning"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetPanning());
				
			}else if(name == "speed"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetSpeed());
				
			}else if(name == "play"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetPlay());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	return source;
}

syneSource::Ref syneLoadSaveSynthesizer::pReadSourceWave(const decXmlElementTag &root, syneSynthesizer &synthesizer){
	const syneSourceWave::Ref source(syneSourceWave::Ref::New());
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(pReadSourceCommon(*tag, synthesizer, *source)){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "type"){
			const decString type(GetCDataString(*tag));
			
			if(type == "sine"){
				source->SetWaveType(deSynthesizerSourceWave::ewtSine);
				
			}else if(type == "square"){
				source->SetWaveType(deSynthesizerSourceWave::ewtSquare);
				
			}else if(type == "sawtooth"){
				source->SetWaveType(deSynthesizerSourceWave::ewtSawTooth);
				
			}else if(type == "triangle"){
				source->SetWaveType(deSynthesizerSourceWave::ewtTriangle);
				
			}else{
				LogErrorUnknownValue(*tag, type);
			}
			
		}else if(tagName == "minFrequency"){
			source->SetMinFrequency(GetCDataFloat(*tag));
			
		}else if(tagName == "maxFrequency"){
			source->SetMaxFrequency(GetCDataFloat(*tag));
			
		}else if(tagName == "target"){
			const decString name(GetAttributeString(*tag, "name"));
			
			if(name == "blendFactor"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetBlendFactor());
				
			}else if(name == "volume"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetVolume());
				
			}else if(name == "panning"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetPanning());
				
			}else if(name == "frequency"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetFrequency());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	return source;
}

syneSource::Ref syneLoadSaveSynthesizer::pReadSourceChain(const decXmlElementTag &root, syneSynthesizer &synthesizer){
	const syneSourceChain::Ref source(syneSourceChain::Ref::New(synthesizer.GetEngine()));
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(pReadSourceCommon(*tag, synthesizer, *source)){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "sound"){
			source->AddPathSound(GetCDataString(*tag));
			
		}else if(tagName == "minSpeed"){
			source->SetMinSpeed(GetCDataFloat(*tag));
			
		}else if(tagName == "maxSpeed"){
			source->SetMaxSpeed(GetCDataFloat(*tag));
			
		}else if(tagName == "target"){
			const decString name(GetAttributeString(*tag, "name"));
			
			if(name == "blendFactor"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetBlendFactor());
				
			}else if(name == "volume"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetVolume());
				
			}else if(name == "panning"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetPanning());
				
			}else if(name == "speed"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetSpeed());
				
			}else if(name == "selection"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetSelect());
				
			}else if(name == "play"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetPlay());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	return source;
}

syneSource::Ref syneLoadSaveSynthesizer::pReadSourceGroup(const decXmlElementTag &root, syneSynthesizer &synthesizer){
	const syneSourceGroup::Ref source(syneSourceGroup::Ref::New());
	const int elementCount = root.GetElementCount();
	int i;
	
	for(i=0; i<elementCount; i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(pReadSourceCommon(*tag, synthesizer, *source)){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "type"){
			const decString name(GetCDataString(*tag));
			
			if(name == "all"){
				source->SetApplicationType(deSynthesizerSourceGroup::eatAll);
				
			}else if(name == "selection"){
				source->SetApplicationType(deSynthesizerSourceGroup::eatSelect);
				
			}else if(name == "solo"){
				source->SetApplicationType(deSynthesizerSourceGroup::eatSolo);
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
			
		}else if(tagName == "target"){
			const decString name(GetAttributeString(*tag, "name"));
			
			if(name == "blendFactor"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetBlendFactor());
				
			}else if(name == "volume"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetVolume());
				
			}else if(name == "panning"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetPanning());
				
			}else if(name == "selection"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetSelect());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
			
		}else{
			const syneSource::Ref childSource(pReadSource(*tag, synthesizer));	
			if(childSource){
				source->AddSource(childSource);
				
			}else{
				LogWarnUnknownTag(root, *tag);
			}
		}
	}
	
	return source;
}

syneSource::Ref syneLoadSaveSynthesizer::pReadSourceSynthesizer(const decXmlElementTag &root, syneSynthesizer &synthesizer){
	const syneSourceSynthesizer::Ref source(syneSourceSynthesizer::Ref::New(synthesizer.GetEngine()));
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(pReadSourceCommon(*tag, synthesizer, *source)){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "synthesizer"){
			source->SetPathSynthesizer(GetCDataString(*tag));
			
		}else if(tagName == "connectionCount"){
			source->SetConnectionCount(GetCDataInt(*tag));
			
		}else if(tagName == "connection"){
			const int target = GetAttributeInt(*tag, "target");
			const int controller = GetAttributeInt(*tag, "controller");
			
			if(controller == -1){
				source->SetControllerAt(target, nullptr);
				
			}else{
				source->SetControllerAt(target, synthesizer.GetControllers().GetAt(controller));
			}
			
		}else if(strcmp(tag->GetName(), "target") == 0){
			const decString name(GetAttributeString(*tag, "name"));
			
			if(name == "blendFactor"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetBlendFactor());
				
			}else if(name == "volume"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetVolume());
				
			}else if(name == "panning"){
				pReadControllerTarget(*tag, synthesizer, source->GetTargetPanning());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	return source;
}

bool syneLoadSaveSynthesizer::pReadSourceCommon(const decXmlElementTag &root,
syneSynthesizer &synthesizer, syneSource &source){
	const decString tagName(root.GetName());
	
	if(tagName == "name"){
		source.SetName(GetCDataString(root));
		return true;
		
	}else if(tagName == "enabled"){
		source.SetEnabled(GetCDataBool(root));
		return true;
		
	}else if(tagName == "mixMode"){
		const decString name(GetCDataString(root));
		
		if(name == "blend"){
			source.SetMixMode(deSynthesizerSource::emmBlend);
			
		}else if(name == "add"){
			source.SetMixMode(deSynthesizerSource::emmAdd);
			
		}else{
			LogErrorUnknownValue(root, name);
		}
		
		return true;
		
	}else if(tagName == "blendFactor"){
		source.SetBlendFactor(GetCDataFloat(root));
		return true;
		
	}else if(tagName == "minVolume"){
		source.SetMinVolume(GetCDataFloat(root));
		return true;
		
	}else if(tagName == "maxVolume"){
		source.SetMaxVolume(GetCDataFloat(root));
		return true;
		
	}else if(tagName == "minPanning"){
		source.SetMinPanning(GetCDataFloat(root));
		return true;
		
	}else if(tagName == "maxPanning"){
		source.SetMaxPanning(GetCDataFloat(root));
		return true;
		
	}else{
		const syneEffect::Ref effect(pReadEffect(root, synthesizer));
		if(effect){
			source.AddEffect(effect);
			return true;
		}
	}
	
	return false;
}

void syneLoadSaveSynthesizer::pReadControllerTarget(const decXmlElementTag &root,
syneSynthesizer &synthesizer, syneControllerTarget &target){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "link"){
			target.AddLink(synthesizer.GetLinks().GetAt(GetCDataInt(*tag)));
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
}

bool syneLoadSaveSynthesizer::pReadEffectCommon(const decXmlElementTag &root,
syneSynthesizer &synthesizer, syneEffect &effect){
	const decString tagName(root.GetName());
	
	if(tagName == "enabled"){
		effect.SetEnabled(GetCDataBool(root));
		return true;
		
	}else if(tagName == "strength"){
		effect.SetStrength(GetCDataFloat(root));
		return true;
	}
	
	return false;
}

syneEffect::Ref syneLoadSaveSynthesizer::pReadEffect(const decXmlElementTag &root, syneSynthesizer &synthesizer){
	const decString tagName(root.GetName());
	
	if(tagName == "effectStretch"){
		return pReadEffectStretch(root, synthesizer);
	}
	
	return {};
}

syneEffect::Ref syneLoadSaveSynthesizer::pReadEffectStretch(const decXmlElementTag &root, syneSynthesizer &synthesizer){
	const syneEffectStretch::Ref effect(syneEffectStretch::Ref::New());
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(pReadEffectCommon(*tag, synthesizer, *effect)){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "minTime"){
			effect->SetMinTime(GetCDataFloat(*tag));
			
		}else if(tagName == "maxTime"){
			effect->SetMaxTime(GetCDataFloat(*tag));
			
		}else if(tagName == "minPitch"){
			effect->SetMinPitch(GetCDataFloat(*tag));
			
		}else if(tagName == "maxPitch"){
			effect->SetMaxPitch(GetCDataFloat(*tag));
			
		}else if(tagName == "target"){
			const decString name(GetAttributeString(*tag, "name"));
			
			if(name == "strength"){
				pReadControllerTarget(*tag, synthesizer, effect->GetTargetStrength());
				
			}else if(name == "time"){
				pReadControllerTarget(*tag, synthesizer, effect->GetTargetTime());
				
			}else if(name == "pitch"){
				pReadControllerTarget(*tag, synthesizer, effect->GetTargetPitch());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
			
		}else{
			LogWarnUnknownTag(root, *tag);
		}
	}
	
	return effect;
}
