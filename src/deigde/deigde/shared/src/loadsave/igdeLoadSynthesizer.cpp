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

#include "igdeLoadSynthesizer.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSound.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceWave.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceChain.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceGroup.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerController.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/deSynthesizerManager.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectStretch.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/sound/deSound.h>



// Class igdeLoadSynthesizer
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoadSynthesizer::igdeLoadSynthesizer(igdeEnvironment &environment, deLogger *logger, const char *loggerSource) :
igdeBaseXML(logger, loggerSource),
pEnvironment(environment),
pName("Drag[en]gine Synthesizer"),
pPattern("*.desynth"),
pDefaultExtension(".desynth"){
}

igdeLoadSynthesizer::~igdeLoadSynthesizer(){
}



// Management
///////////////

void igdeLoadSynthesizer::SetName(const char *name){
	pName = name;
}

void igdeLoadSynthesizer::SetPattern(const char *pattern){
	pPattern = pattern;
}

void igdeLoadSynthesizer::SetDefaultExtension(const char *extension){
	pDefaultExtension = extension;
}



// Loading and saving
///////////////////////

void igdeLoadSynthesizer::Load(const decString &pathSynthesizer,
deSynthesizer &synthesizer, decBaseFileReader &reader){
	decPath basePath;
	basePath.SetFromUnix(pathSynthesizer.GetString());
	if(basePath.GetComponentCount() > 1){
		basePath.RemoveLastComponent();
		
	}else{
		basePath.SetFromUnix("/");
	}
	
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::New());
	
	decXmlParser(GetLogger()).ParseXml(&reader, xmlDoc);
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if(!root || strcmp(root->GetName(), "synthesizer") != 0){
		DETHROW(deeInvalidParam);
	}
	
	pReadSynthesizer(*root, basePath.GetPathUnix(), synthesizer);
}



// Private Functions
//////////////////////

void igdeLoadSynthesizer::pReadSynthesizer(const decXmlElementTag &root,
const char *basePath, deSynthesizer &synthesizer){
	int i;
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
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
			const deSynthesizerSource::Ref source(pReadSource(*tag, basePath, synthesizer));
			if(source){
				synthesizer.AddSource(source);
			}
		}
	}
}

void igdeLoadSynthesizer::pReadController(const decXmlElementTag &root, deSynthesizer &synthesizer){
	deSynthesizerController::Ref controller(deSynthesizerController::Ref::New());
	
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
			
		}else if(tagName == "limits"){
			controller->SetValueRange(GetAttributeFloat(*tag, "min"), GetAttributeFloat(*tag, "max"));
		}
	}
	
	synthesizer.AddController(controller);
}

void igdeLoadSynthesizer::pReadLink(const decXmlElementTag &root, deSynthesizer &synthesizer){
	const deSynthesizerLink::Ref link(deSynthesizerLink::Ref::New());
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "controller"){
			link->SetController(GetCDataInt(*tag));
			
		}else if(tagName == "repeat"){
			link->SetRepeat(GetCDataInt(*tag));
			
		}else if(tagName == "curve"){
			ReadCurveBezier(*tag, link->GetCurve());
		}
	}
	
	synthesizer.AddLink(link);
}

deSynthesizerSource::Ref igdeLoadSynthesizer::pReadSource(const decXmlElementTag &root,
const char *basePath, deSynthesizer &synthesizer){
	const decString tagName(root.GetName());
	
	if(tagName == "sourceSound"){
		return pReadSourceSound(root, basePath, synthesizer);
		
	}else if(tagName == "sourceWave"){
		return pReadSourceWave(root, synthesizer);
		
	}else if(tagName == "sourceChain"){
		return pReadSourceChain(root, basePath, synthesizer);
		
	}else if(tagName == "sourceGroup"){
		return pReadSourceGroup(root, basePath, synthesizer);
		
	}else if(tagName == "sourceSynthesizer"){
		return pReadSourceSynthesizer(root, basePath, synthesizer);
	}
	
	return {};
}

deSynthesizerSource::Ref igdeLoadSynthesizer::pReadSourceSound(const decXmlElementTag &root,
const char *basePath, deSynthesizer &synthesizer){
	const deSynthesizerSourceSound::Ref source(deSynthesizerSourceSound::Ref::New());
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
			const decString path(GetCDataString(*tag));
			
			if(!path.IsEmpty()){
				try{
					source->SetSound(synthesizer.GetEngine()->GetSoundManager()->
						LoadSound(path, basePath, false));
					
				}catch(const deException &){
					LogWarnGenericProblemTag(root, tag->GetName(), "Failed loading resource file");
				}
			}
			
		}else if(tagName == "minSpeed"){
			source->SetMinSpeed(GetCDataFloat(*tag));
			
		}else if(tagName == "maxSpeed"){
			source->SetMaxSpeed(GetCDataFloat(*tag));
			
		}else if(tagName == "looping"){
			source->SetLooping(GetCDataBool(*tag));
			
		}else if(tagName == "target"){
			const decString name(GetAttributeString(*tag, "name"));
			
			if(name == "blendFactor"){
				pReadControllerTarget(*tag, source->GetTargetBlendFactor());
				
			}else if(name == "volume"){
				pReadControllerTarget(*tag, source->GetTargetVolume());
				
			}else if(name == "panning"){
				pReadControllerTarget(*tag, source->GetTargetPanning());
				
			}else if(name == "speed"){
				pReadControllerTarget(*tag, source->GetTargetSpeed());
				
			}else if(name == "play"){
				pReadControllerTarget(*tag, source->GetTargetPlay());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
		}
	}
	
	return source;
}

deSynthesizerSource::Ref igdeLoadSynthesizer::pReadSourceWave(const decXmlElementTag &root,
deSynthesizer &synthesizer){
	const deSynthesizerSourceWave::Ref source(deSynthesizerSourceWave::Ref::New());
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
				source->SetType(deSynthesizerSourceWave::ewtSine);
				
			}else if(type == "square"){
				source->SetType(deSynthesizerSourceWave::ewtSquare);
				
			}else if(type == "sawtooth"){
				source->SetType(deSynthesizerSourceWave::ewtSawTooth);
				
			}else if(type == "triangle"){
				source->SetType(deSynthesizerSourceWave::ewtTriangle);
				
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
				pReadControllerTarget(*tag, source->GetTargetBlendFactor());
				
			}else if(name == "volume"){
				pReadControllerTarget(*tag, source->GetTargetVolume());
				
			}else if(name == "panning"){
				pReadControllerTarget(*tag, source->GetTargetPanning());
				
			}else if(name == "frequency"){
				pReadControllerTarget(*tag, source->GetTargetFrequency());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
		}
	}
	
	return source;
}

deSynthesizerSource::Ref igdeLoadSynthesizer::pReadSourceChain(const decXmlElementTag &root,
const char *basePath, deSynthesizer &synthesizer){
	const deSynthesizerSourceChain::Ref source(deSynthesizerSourceChain::Ref::New());
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
			const decString path(GetCDataString(*tag));
			
			if(!path.IsEmpty()){
				try{
					source->AddSound(synthesizer.GetEngine()->GetSoundManager()->
						LoadSound(path, basePath, false));
					
				}catch(const deException &){
					LogWarnGenericProblemTag(root, tag->GetName(), "Failed loading resource file");
				}
			}
			
		}else if(tagName == "minSpeed"){
			source->SetMinSpeed(GetCDataFloat(*tag));
			
		}else if(tagName == "maxSpeed"){
			source->SetMaxSpeed(GetCDataFloat(*tag));
			
		}else if(tagName == "target"){
			const decString name(GetAttributeString(*tag, "name"));
			
			if(name == "blendFactor"){
				pReadControllerTarget(*tag, source->GetTargetBlendFactor());
				
			}else if(name == "volume"){
				pReadControllerTarget(*tag, source->GetTargetVolume());
				
			}else if(name == "panning"){
				pReadControllerTarget(*tag, source->GetTargetPanning());
				
			}else if(name == "speed"){
				pReadControllerTarget(*tag, source->GetTargetSpeed());
				
			}else if(name == "selection"){
				pReadControllerTarget(*tag, source->GetTargetSelect());
				
			}else if(name == "play"){
				pReadControllerTarget(*tag, source->GetTargetPlay());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
		}
	}
	
	return source;
}

deSynthesizerSource::Ref igdeLoadSynthesizer::pReadSourceGroup(const decXmlElementTag &root,
const char *basePath, deSynthesizer &synthesizer){
	const deSynthesizerSourceGroup::Ref source(deSynthesizerSourceGroup::Ref::New());
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
				pReadControllerTarget(*tag, source->GetTargetBlendFactor());
				
			}else if(name == "volume"){
				pReadControllerTarget(*tag, source->GetTargetVolume());
				
			}else if(name == "panning"){
				pReadControllerTarget(*tag, source->GetTargetPanning());
				
			}else if(name == "selection"){
				pReadControllerTarget(*tag, source->GetTargetSelect());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
			
		}else{
			const deSynthesizerSource::Ref childSource(pReadSource(*tag, basePath, synthesizer));
			if(childSource){
				source->AddSource(childSource);
			}
		}
	}
	
	return source;
}

deSynthesizerSource::Ref igdeLoadSynthesizer::pReadSourceSynthesizer(const decXmlElementTag &root,
const char *basePath, deSynthesizer &synthesizer){
	const deSynthesizerSourceSynthesizer::Ref source(deSynthesizerSourceSynthesizer::Ref::New());
	const deEngine &engine = *synthesizer.GetEngine();
	const deVirtualFileSystem &vfs = engine.GetVirtualFileSystem();
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
			const decString path(GetCDataString(*tag));
			if(path.IsEmpty()){
				continue;
			}
			
			try{
				const deSynthesizer::Ref childSynthesizer(
					engine.GetSynthesizerManager()->CreateSynthesizer());
				Load(path, childSynthesizer, vfs.OpenFileForReading(decPath::CreatePathUnix(path)));
				source->SetSynthesizer(childSynthesizer);
				
			}catch(const deException &){
				LogWarnGenericProblemTag(root, tag->GetName(), "Failed loading synthesizer");
			}
			
		}else if(tagName == "connection"){
			const int target = GetAttributeInt(*tag, "target");
			const int controller = GetAttributeInt(*tag, "controller");
			source->SetConnectionAt(target, controller);
			
		}else if(strcmp(tag->GetName(), "target") == 0){
			const decString name(GetAttributeString(*tag, "name"));
			
			if(name == "blendFactor"){
				pReadControllerTarget(*tag, source->GetTargetBlendFactor());
				
			}else if(name == "volume"){
				pReadControllerTarget(*tag, source->GetTargetVolume());
				
			}else if(name == "panning"){
				pReadControllerTarget(*tag, source->GetTargetPanning());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
		}
	}
	
	return source;
}

bool igdeLoadSynthesizer::pReadSourceCommon(const decXmlElementTag &root,
deSynthesizer &synthesizer, deSynthesizerSource &source){
	const decString tagName(root.GetName());
	
	if(tagName == "name"){
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
		const deSynthesizerEffect::Ref effect(pReadEffect(root, synthesizer));
		if(effect){
			source.AddEffect(effect);
			return true;
		}
	}
	
	return false;
}

void igdeLoadSynthesizer::pReadControllerTarget(const decXmlElementTag &root,
deSynthesizerControllerTarget &target){
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		const decString tagName(tag->GetName());
		
		if(tagName == "link"){
			target.AddLink(GetCDataInt(*tag));
		}
	}
}

deSynthesizerEffect::Ref igdeLoadSynthesizer::pReadEffect(const decXmlElementTag &root,
deSynthesizer &synthesizer){
	const decString tagName(root.GetName());
	
	if(tagName == "effectStretch"){
		return pReadEffectStretch(root, synthesizer);
	}
	
	return {};
}

deSynthesizerEffect::Ref igdeLoadSynthesizer::pReadEffectStretch(const decXmlElementTag &root,
deSynthesizer &synthesizer){
	const deSynthesizerEffectStretch::Ref effect(deSynthesizerEffectStretch::Ref::New());
	int i;
	
	for(i=0; i<root.GetElementCount(); i++){
		decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(pReadEffectCommon(*tag, *effect)){
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
				pReadControllerTarget(*tag, effect->GetTargetStrength());
				
			}else if(name == "time"){
				pReadControllerTarget(*tag, effect->GetTargetTime());
				
			}else if(name == "pitch"){
				pReadControllerTarget(*tag, effect->GetTargetPitch());
				
			}else{
				LogErrorUnknownValue(*tag, name);
			}
		}
	}
	
	return effect;
}

bool igdeLoadSynthesizer::pReadEffectCommon(const decXmlElementTag &root, deSynthesizerEffect &effect){
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
