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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seSourceSound.h"
#include "../seSynthesizer.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSound.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <deigde/environment/igdeEnvironment.h>



// Class seSourceSound
////////////////////////

// Constructor, destructor
////////////////////////////

seSourceSound::seSourceSound(deEngine *engine) :
seSource(deSynthesizerSourceVisitorIdentify::estSound),
pEngine(engine),
pMinSpeed(1.0f),
pMaxSpeed(1.0f),
pLooping(false){
}

seSourceSound::seSourceSound(const seSourceSound &copy) : 
seSource(copy),
pEngine(copy.pEngine),
pPathSound(copy.pPathSound),
pSound(copy.pSound),
pMinSpeed(copy.pMinSpeed),
pMaxSpeed(copy.pMaxSpeed),
pLooping(copy.pLooping),
pTargetSpeed(copy.pTargetSpeed),
pTargetPlay(copy.pTargetPlay){
}

seSourceSound::~seSourceSound(){
}



// Management
///////////////

void seSourceSound::SetPathSound(const char *path){
	if(pPathSound == path){
		return;
	}
	
	pPathSound = path;
	UpdateSound();
	NotifySourceChanged();
}

void seSourceSound::UpdateSound(){
	if(pSound){
		pSound = nullptr;
	}
	
	if(!pPathSound.IsEmpty() && GetSynthesizer()){
		const decString &basePath = GetSynthesizer()->GetDirectoryPath();
		
		try{
			pSound = pEngine->GetSoundManager()->LoadSound(pPathSound, basePath, false);
			
		}catch(const deException &){
			GetSynthesizer()->GetEnvironment()->GetLogger()->LogInfoFormat("Synthesizer Editor",
				"Failed to load sound '%s' (base directory '%s')", pPathSound.GetString(), basePath.GetString());
		}
	}
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceSound>()->SetSound(pSound);
	}
}

void seSourceSound::SetMinSpeed(float speed){
	if(fabsf(speed - pMinSpeed) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinSpeed = speed;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceSound>()->SetMinSpeed(speed);
		NotifySourceChanged();
	}
}

void seSourceSound::SetMaxSpeed(float speed){
	if(fabsf(speed - pMaxSpeed) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxSpeed = speed;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceSound>()->SetMaxSpeed(speed);
		NotifySourceChanged();
	}
}

void seSourceSound::SetLooping(bool looping){
	if(looping == pLooping){
		return;
	}
	
	pLooping = looping;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceSound>()->SetLooping(looping);
		NotifySourceChanged();
	}
}



void seSourceSound::UpdateTargets(){
	deSynthesizerSourceSound * const source = GetEngineSource().DynamicCast<deSynthesizerSourceSound>();
	
	seSource::UpdateTargets();
	
	if(source){
		seSynthesizer * const synthesizer = GetSynthesizer();
		DEASSERT_NOTNULL(synthesizer);
		
		pTargetSpeed.UpdateEngineTarget(*synthesizer, source->GetTargetSpeed());
		pTargetPlay.UpdateEngineTarget(*synthesizer, source->GetTargetPlay());
	}
}

int seSourceSound::CountLinkUsage(seLink *link) const{
	int usageCount = seSource::CountLinkUsage(link);
	
	if(pTargetSpeed.GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetPlay.GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void seSourceSound::RemoveLinkFromTargets(seLink *link){
	seSource::RemoveLinkFromTargets(link);
	
	pTargetSpeed.RemoveLink(link);
	pTargetPlay.RemoveLink(link);
	
	UpdateTargets();
}

void seSourceSound::RemoveLinksFromAllTargets(){
	seSource::RemoveLinksFromAllTargets();
	
	pTargetSpeed.RemoveAllLinks();
	pTargetPlay.RemoveAllLinks();
	
	UpdateTargets();
}



deSynthesizerSource::Ref seSourceSound::CreateEngineSource(){
	const deSynthesizerSourceSound::Ref engSource(deSynthesizerSourceSound::Ref::New());
	
	InitEngineSource(engSource);
	
	engSource->SetSound(pSound);
	engSource->SetMinSpeed(pMinSpeed);
	engSource->SetMaxSpeed(pMaxSpeed);
	engSource->SetLooping(pLooping);
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer);
	
	pTargetSpeed.UpdateEngineTarget(*synthesizer, engSource->GetTargetSpeed());
	pTargetPlay.UpdateEngineTarget(*synthesizer, engSource->GetTargetPlay());
	
	return engSource;
}



seSource::Ref seSourceSound::CreateCopy() const{
	return seSourceSound::Ref::New(*this);
}



void seSourceSound::ListLinks(seLink::List &list){
	seSource::ListLinks(list);
	pTargetSpeed.AddLinksToList(list);
	pTargetPlay.AddLinksToList(list);
}



void seSourceSound::SynthesizerChanged(){
	seSource::SynthesizerChanged();
	UpdateSound();
	NotifyEngineOnlySourceChanged();
}

void seSourceSound::SynthesizerDirectoryChanged(){
	seSource::SynthesizerDirectoryChanged();
	UpdateSound();
	NotifyEngineOnlySourceChanged();
}



// Operators
//////////////

seSourceSound &seSourceSound::operator=(const seSourceSound &copy){
	SetPathSound(copy.pPathSound);
	SetMinSpeed(copy.pMinSpeed);
	SetMaxSpeed(copy.pMaxSpeed);
	SetLooping(copy.pLooping);
	pTargetSpeed = copy.pTargetSpeed;
	pTargetPlay = copy.pTargetPlay;
	seSource::operator=(copy);
	return *this;
}
