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

#include "syneSourceChain.h"
#include "../syneSynthesizer.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceChain.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <deigde/environment/igdeEnvironment.h>



// Class syneSourceChain
////////////////////////

// Constructor, destructor
////////////////////////////

syneSourceChain::syneSourceChain(deEngine *engine) :
syneSource(deSynthesizerSourceVisitorIdentify::estChain),
pEngine(engine),
pMinSpeed(1.0f),
pMaxSpeed(1.0f),
pTargetSpeed(syneControllerTarget::Ref::New()),
pTargetSelect(syneControllerTarget::Ref::New()),
pTargetPlay(syneControllerTarget::Ref::New()){
}

syneSourceChain::syneSourceChain(const syneSourceChain &copy) : 
syneSource(copy),
pEngine(copy.pEngine),
pPathSounds(copy.pPathSounds),
pSounds(copy.pSounds),
pMinSpeed(copy.pMinSpeed),
pMaxSpeed(copy.pMaxSpeed),
pTargetSpeed(syneControllerTarget::Ref::New(copy.pTargetSpeed)),
pTargetSelect(syneControllerTarget::Ref::New(copy.pTargetSelect)),
pTargetPlay(syneControllerTarget::Ref::New(copy.pTargetPlay)){
}

syneSourceChain::~syneSourceChain(){
}



// Management
///////////////

void syneSourceChain::SetPathSound(int index, const char *path){
	if(pPathSounds.GetAt(index) == path){
		return;
	}
	pPathSounds.SetAt(index, path);
	UpdateSounds();
	NotifySourceChanged();
}

void syneSourceChain::AddPathSound(const char *path){
	pPathSounds.Add(path);
	UpdateSounds();
	NotifySourceChanged();
}

void syneSourceChain::InsertPathSound(const char *path, int index){
	pPathSounds.Insert(path, index);
	UpdateSounds();
	NotifySourceChanged();
}

void syneSourceChain::MovePathSound(int from, int to){
	pPathSounds.Move(from, to);
	UpdateSounds();
	NotifySourceChanged();
}

void syneSourceChain::RemovePathSound(int index){
	pPathSounds.RemoveFrom(index);
	UpdateSounds();
	NotifySourceChanged();
}

void syneSourceChain::RemoveAllPathSounds(){
	pPathSounds.RemoveAll();
	UpdateSounds();
	NotifySourceChanged();
}

void syneSourceChain::UpdateSounds(){
	pSounds.RemoveAll();
	
	pPathSounds.Visit([&](const decString &path){
		deSound::Ref sound;
		
		if(!path.IsEmpty() && GetSynthesizer()){
			const decString &basePath = GetSynthesizer()->GetDirectoryPath();
			
			try{
				sound = pEngine->GetSoundManager()->LoadSound(path, basePath, false);
				
			}catch(const deException &){
				GetSynthesizer()->GetEnvironment()->GetLogger()->LogInfoFormat("Synthesizer Editor",
					"Failed to load sound '%s' (base directory '%s')", path.GetString(), basePath.GetString());
			}
		}
		
		pSounds.Add(sound);
	});
	
	if(GetEngineSource()){
		deSynthesizerSourceChain &source = GetEngineSource().DynamicCast<deSynthesizerSourceChain>();
		source.RemoveAllSounds();
		pSounds.Visit([&](deSound *sound){
			source.AddSound(sound);
		});
	}
}

void syneSourceChain::SetMinSpeed(float speed){
	if(fabsf(speed - pMinSpeed) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinSpeed = speed;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceChain>()->SetMinSpeed(speed);
		NotifySourceChanged();
	}
}

void syneSourceChain::SetMaxSpeed(float speed){
	if(fabsf(speed - pMaxSpeed) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxSpeed = speed;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceChain>()->SetMaxSpeed(speed);
		NotifySourceChanged();
	}
}



void syneSourceChain::UpdateTargets(){
	deSynthesizerSourceChain *const source = GetEngineSource().DynamicCast<deSynthesizerSourceChain>();
	
	syneSource::UpdateTargets();
	
	if(source){
		syneSynthesizer *const synthesizer = GetSynthesizer();
		DEASSERT_NOTNULL(synthesizer);
		
		pTargetSpeed->UpdateEngineTarget(*synthesizer, source->GetTargetSpeed());
		pTargetSelect->UpdateEngineTarget(*synthesizer, source->GetTargetSelect());
		pTargetPlay->UpdateEngineTarget(*synthesizer, source->GetTargetPlay());
	}
}

int syneSourceChain::CountLinkUsage(syneLink *link) const{
	int usageCount = syneSource::CountLinkUsage(link);
	
	if(pTargetSpeed->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetSelect->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetPlay->GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void syneSourceChain::RemoveLinkFromTargets(syneLink *link){
	syneSource::RemoveLinkFromTargets(link);
	
	pTargetSpeed->RemoveLink(link);
	pTargetSelect->RemoveLink(link);
	pTargetPlay->RemoveLink(link);
	
	UpdateTargets();
}

void syneSourceChain::RemoveLinksFromAllTargets(){
	syneSource::RemoveLinksFromAllTargets();
	
	pTargetSpeed->RemoveAllLinks();
	pTargetSelect->RemoveAllLinks();
	pTargetPlay->RemoveAllLinks();
	
	UpdateTargets();
}



deSynthesizerSource::Ref syneSourceChain::CreateEngineSource(){
	const deSynthesizerSourceChain::Ref engSource(deSynthesizerSourceChain::Ref::New());
	
	InitEngineSource(engSource);
	
	engSource->RemoveAllSounds();
	pSounds.Visit([&](deSound *sound){
		engSource->AddSound(sound);
	});
	engSource->SetMinSpeed(pMinSpeed);
	engSource->SetMaxSpeed(pMaxSpeed);
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer);
	
	pTargetSpeed->UpdateEngineTarget(*synthesizer, engSource->GetTargetSpeed());
	pTargetSelect->UpdateEngineTarget(*synthesizer, engSource->GetTargetSelect());
	pTargetPlay->UpdateEngineTarget(*synthesizer, engSource->GetTargetPlay());
	
	return engSource;
}



syneSource::Ref syneSourceChain::CreateCopy() const{
	return syneSourceChain::Ref::New(*this);
}



void syneSourceChain::ListLinks(syneLink::List &list){
	syneSource::ListLinks(list);
	pTargetSpeed->AddLinksToList(list);
	pTargetSelect->AddLinksToList(list);
	pTargetPlay->AddLinksToList(list);
}



void syneSourceChain::SynthesizerChanged(){
	syneSource::SynthesizerChanged();
	UpdateSounds();
	NotifyEngineOnlySourceChanged();
}

void syneSourceChain::SynthesizerDirectoryChanged(){
	syneSource::SynthesizerDirectoryChanged();
	UpdateSounds();
	NotifyEngineOnlySourceChanged();
}



// Operators
//////////////

syneSourceChain &syneSourceChain::operator=(const syneSourceChain &copy){
	pPathSounds = copy.pPathSounds;
	pSounds = copy.pSounds;
	UpdateSounds();
	NotifySourceChanged();
	SetMinSpeed(copy.pMinSpeed);
	SetMaxSpeed(copy.pMaxSpeed);
	pTargetSpeed = copy.pTargetSpeed;
	pTargetPlay = copy.pTargetPlay;
	syneSource::operator=(copy);
	return *this;
}
