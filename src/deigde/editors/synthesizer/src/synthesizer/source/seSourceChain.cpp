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

#include "seSourceChain.h"
#include "../seSynthesizer.h"

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



// Class seSourceChain
////////////////////////

// Constructor, destructor
////////////////////////////

seSourceChain::seSourceChain(deEngine *engine) :
seSource(deSynthesizerSourceVisitorIdentify::estChain),
pEngine(engine),
pMinSpeed(1.0f),
pMaxSpeed(1.0f),
pTargetSpeed(seControllerTarget::Ref::New()),
pTargetSelect(seControllerTarget::Ref::New()),
pTargetPlay(seControllerTarget::Ref::New()){
}

seSourceChain::seSourceChain(const seSourceChain &copy) : 
seSource(copy),
pEngine(copy.pEngine),
pPathSounds(copy.pPathSounds),
pSounds(copy.pSounds),
pMinSpeed(copy.pMinSpeed),
pMaxSpeed(copy.pMaxSpeed),
pTargetSpeed(seControllerTarget::Ref::New(copy.pTargetSpeed)),
pTargetSelect(seControllerTarget::Ref::New(copy.pTargetSelect)),
pTargetPlay(seControllerTarget::Ref::New(copy.pTargetPlay)){
}

seSourceChain::~seSourceChain(){
}



// Management
///////////////

void seSourceChain::SetPathSound(int index, const char *path){
	if(pPathSounds.GetAt(index) == path){
		return;
	}
	pPathSounds.SetAt(index, path);
	UpdateSounds();
	NotifySourceChanged();
}

void seSourceChain::AddPathSound(const char *path){
	pPathSounds.Add(path);
	UpdateSounds();
	NotifySourceChanged();
}

void seSourceChain::InsertPathSound(const char *path, int index){
	pPathSounds.Insert(path, index);
	UpdateSounds();
	NotifySourceChanged();
}

void seSourceChain::MovePathSound(int from, int to){
	pPathSounds.Move(from, to);
	UpdateSounds();
	NotifySourceChanged();
}

void seSourceChain::RemovePathSound(int index){
	pPathSounds.RemoveFrom(index);
	UpdateSounds();
	NotifySourceChanged();
}

void seSourceChain::RemoveAllPathSounds(){
	pPathSounds.RemoveAll();
	UpdateSounds();
	NotifySourceChanged();
}

void seSourceChain::UpdateSounds(){
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

void seSourceChain::SetMinSpeed(float speed){
	if(fabsf(speed - pMinSpeed) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinSpeed = speed;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceChain>()->SetMinSpeed(speed);
		NotifySourceChanged();
	}
}

void seSourceChain::SetMaxSpeed(float speed){
	if(fabsf(speed - pMaxSpeed) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxSpeed = speed;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceChain>()->SetMaxSpeed(speed);
		NotifySourceChanged();
	}
}



void seSourceChain::UpdateTargets(){
	deSynthesizerSourceChain *const source = GetEngineSource().DynamicCast<deSynthesizerSourceChain>();
	
	seSource::UpdateTargets();
	
	if(source){
		seSynthesizer *const synthesizer = GetSynthesizer();
		DEASSERT_NOTNULL(synthesizer);
		
		pTargetSpeed->UpdateEngineTarget(*synthesizer, source->GetTargetSpeed());
		pTargetSelect->UpdateEngineTarget(*synthesizer, source->GetTargetSelect());
		pTargetPlay->UpdateEngineTarget(*synthesizer, source->GetTargetPlay());
	}
}

int seSourceChain::CountLinkUsage(seLink *link) const{
	int usageCount = seSource::CountLinkUsage(link);
	
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

void seSourceChain::RemoveLinkFromTargets(seLink *link){
	seSource::RemoveLinkFromTargets(link);
	
	pTargetSpeed->RemoveLink(link);
	pTargetSelect->RemoveLink(link);
	pTargetPlay->RemoveLink(link);
	
	UpdateTargets();
}

void seSourceChain::RemoveLinksFromAllTargets(){
	seSource::RemoveLinksFromAllTargets();
	
	pTargetSpeed->RemoveAllLinks();
	pTargetSelect->RemoveAllLinks();
	pTargetPlay->RemoveAllLinks();
	
	UpdateTargets();
}



deSynthesizerSource::Ref seSourceChain::CreateEngineSource(){
	const deSynthesizerSourceChain::Ref engSource(deSynthesizerSourceChain::Ref::New());
	
	InitEngineSource(engSource);
	
	engSource->RemoveAllSounds();
	pSounds.Visit([&](deSound *sound){
		engSource->AddSound(sound);
	});
	engSource->SetMinSpeed(pMinSpeed);
	engSource->SetMaxSpeed(pMaxSpeed);
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer);
	
	pTargetSpeed->UpdateEngineTarget(*synthesizer, engSource->GetTargetSpeed());
	pTargetSelect->UpdateEngineTarget(*synthesizer, engSource->GetTargetSelect());
	pTargetPlay->UpdateEngineTarget(*synthesizer, engSource->GetTargetPlay());
	
	return engSource;
}



seSource::Ref seSourceChain::CreateCopy() const{
	return seSourceChain::Ref::New(*this);
}



void seSourceChain::ListLinks(seLink::List &list){
	seSource::ListLinks(list);
	pTargetSpeed->AddLinksToList(list);
	pTargetSelect->AddLinksToList(list);
	pTargetPlay->AddLinksToList(list);
}



void seSourceChain::SynthesizerChanged(){
	seSource::SynthesizerChanged();
	UpdateSounds();
	NotifyEngineOnlySourceChanged();
}

void seSourceChain::SynthesizerDirectoryChanged(){
	seSource::SynthesizerDirectoryChanged();
	UpdateSounds();
	NotifyEngineOnlySourceChanged();
}



// Operators
//////////////

seSourceChain &seSourceChain::operator=(const seSourceChain &copy){
	pPathSounds = copy.pPathSounds;
	pSounds = copy.pSounds;
	UpdateSounds();
	NotifySourceChanged();
	SetMinSpeed(copy.pMinSpeed);
	SetMaxSpeed(copy.pMaxSpeed);
	pTargetSpeed = copy.pTargetSpeed;
	pTargetPlay = copy.pTargetPlay;
	seSource::operator=(copy);
	return *this;
}
