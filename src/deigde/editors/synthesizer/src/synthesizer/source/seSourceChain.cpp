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
pMaxSpeed(1.0f){
}

seSourceChain::seSourceChain(const seSourceChain &copy) : 
seSource(copy),
pEngine(copy.pEngine),
pPathSounds(copy.pPathSounds),
pSounds(copy.pSounds),
pMinSpeed(copy.pMinSpeed),
pMaxSpeed(copy.pMaxSpeed),
pTargetSpeed(copy.pTargetSpeed),
pTargetSelect(copy.pTargetSelect),
pTargetPlay(copy.pTargetPlay){
}

seSourceChain::~seSourceChain(){
}



// Management
///////////////

deSound *seSourceChain::GetSoundAt(int index) const{
	return (deSound*)pSounds.GetAt(index);
}

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
	pPathSounds.InsertAt(path, index);
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
	const int count = pPathSounds.GetCount();
	int i;
	
	pSounds.RemoveAll();
	
	for(i=0; i<count; i++){
		const decString &path = pPathSounds.GetAt(i);
		deSound * sound = NULL;
		
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
	}
	
	if(GetEngineSource()){
		deSynthesizerSourceChain &source = *((deSynthesizerSourceChain*)GetEngineSource());
		source.RemoveAllSounds();
		for(i=0; i<count; i++){
			source.AddSound((deSound*)pSounds.GetAt(i));
		}
	}
}

void seSourceChain::SetMinSpeed(float speed){
	if(fabsf(speed - pMinSpeed) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinSpeed = speed;
	
	if(GetEngineSource()){
		((deSynthesizerSourceChain*)GetEngineSource())->SetMinSpeed(speed);
		NotifySourceChanged();
	}
}

void seSourceChain::SetMaxSpeed(float speed){
	if(fabsf(speed - pMaxSpeed) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxSpeed = speed;
	
	if(GetEngineSource()){
		((deSynthesizerSourceChain*)GetEngineSource())->SetMaxSpeed(speed);
		NotifySourceChanged();
	}
}



void seSourceChain::UpdateTargets(){
	deSynthesizerSourceChain *source = (deSynthesizerSourceChain*)GetEngineSource();
	
	seSource::UpdateTargets();
	
	if(source){
		pTargetSpeed.UpdateEngineTarget(GetSynthesizer(), source->GetTargetSpeed());
		pTargetSelect.UpdateEngineTarget(GetSynthesizer(), source->GetTargetSelect());
		pTargetPlay.UpdateEngineTarget(GetSynthesizer(), source->GetTargetPlay());
	}
}

int seSourceChain::CountLinkUsage(seLink *link) const{
	int usageCount = seSource::CountLinkUsage(link);
	
	if(pTargetSpeed.HasLink(link)){
		usageCount++;
	}
	if(pTargetSelect.HasLink(link)){
		usageCount++;
	}
	if(pTargetPlay.HasLink(link)){
		usageCount++;
	}
	
	return usageCount;
}

void seSourceChain::RemoveLinkFromTargets(seLink *link){
	seSource::RemoveLinkFromTargets(link);
	
	if(pTargetSpeed.HasLink(link)){
		pTargetSpeed.RemoveLink(link);
	}
	if(pTargetSelect.HasLink(link)){
		pTargetSelect.RemoveLink(link);
	}
	if(pTargetPlay.HasLink(link)){
		pTargetPlay.RemoveLink(link);
	}
	
	UpdateTargets();
}

void seSourceChain::RemoveLinksFromAllTargets(){
	seSource::RemoveLinksFromAllTargets();
	
	pTargetSpeed.RemoveAllLinks();
	pTargetSelect.RemoveAllLinks();
	pTargetPlay.RemoveAllLinks();
	
	UpdateTargets();
}



deSynthesizerSource *seSourceChain::CreateEngineSource(){
	const int count = pSounds.GetCount();
	int i;
	
	deSynthesizerSourceChain *engSource = NULL;
	
	try{
		// create source
		engSource = new deSynthesizerSourceChain;
		
		// init source
		InitEngineSource(engSource);
		
		engSource->RemoveAllSounds();
		for(i=0; i<count; i++){
			engSource->AddSound((deSound*)pSounds.GetAt(i));
		}
		engSource->SetMinSpeed(pMinSpeed);
		engSource->SetMaxSpeed(pMaxSpeed);
		
		pTargetSpeed.UpdateEngineTarget(GetSynthesizer(), engSource->GetTargetSpeed());
		pTargetSelect.UpdateEngineTarget(GetSynthesizer(), engSource->GetTargetSelect());
		pTargetPlay.UpdateEngineTarget(GetSynthesizer(), engSource->GetTargetPlay());
		
	}catch(const deException &){
		if(engSource){
			engSource->FreeReference();
		}
		throw;
	}
	
	// finished
	return engSource;
}



seSource *seSourceChain::CreateCopy() const{
	return new seSourceChain(*this);
}



void seSourceChain::ListLinks(seLinkList &list){
	seSource::ListLinks(list);
	pTargetSpeed.AddLinksToList(list);
	pTargetSelect.AddLinksToList(list);
	pTargetPlay.AddLinksToList(list);
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
