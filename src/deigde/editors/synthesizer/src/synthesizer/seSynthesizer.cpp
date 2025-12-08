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

#include "seSynthesizer.h"
#include "seSynthesizerNotifier.h"
#include "controller/seController.h"
#include "link/seLink.h"
#include "source/seSource.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deMicrophoneManager.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/sound/deSpeakerManager.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerManager.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstanceManager.h>
#include <dragengine/resources/synthesizer/deSynthesizerController.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/systems/deAudioSystem.h>



// Definitions
////////////////

#define LOGSOURCE "Synthesizer Editor"



// Class seSynthesizer
/////////////////////

// Constructor, destructor
////////////////////////////

seSynthesizer::seSynthesizer(igdeEnvironment *environment, seLoadSaveSystem &lssys) :
igdeEditableEntity(environment),

pLoadSaveSystem(lssys),
pEngWorld(NULL),

pEngSynthesizer(NULL),
pEngSynthesizerInstance(NULL),
pEngSpeaker(NULL),
pEngMicrophone(NULL),

pActiveController(NULL),
pActiveLink(NULL),
pActiveSource(NULL),

pChannelCount(1),
pSampleRate(44100),
pBytesPerSample(2),
pSampleCount(pSampleRate * 10),
pPlayTime(10.0f)
{
	try{
		SetFilePath("new.desynth");
		
		pCreateWorld();
		
		pEngSynthesizerInstance.TakeOver(GetEngine()->GetSynthesizerInstanceManager()->CreateSynthesizerInstance();
		pEngSynthesizerInstance->SetSynthesizer(pEngSynthesizer);
		pEngSynthesizerInstance->SetSampleCount(pSampleCount);
		
		pEngSpeaker->SetSynthesizer(pEngSynthesizerInstance);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

seSynthesizer::~seSynthesizer(){
	pCleanUp();
}



// Management
///////////////

void seSynthesizer::SetChannelCount(int channelCount){
	channelCount = decMath::max(channelCount, 1);
	if(pChannelCount == channelCount){
		return;
	}
	
	pChannelCount = channelCount;
	
	pEngSynthesizer->SetChannelCount(channelCount);
	NotifySynthesizerChanged();
}

void seSynthesizer::SetSampleRate(int sampleRate){
	sampleRate = decMath::max(sampleRate, 1);
	if(pSampleRate == sampleRate){
		return;
	}
	
	pSampleRate = sampleRate;
	pUpdatePlayTime();
	
	pEngSynthesizer->SetSampleRate(sampleRate);
	NotifySynthesizerChanged();
}

void seSynthesizer::SetBytesPerSample(int bytesPerSample){
	bytesPerSample = decMath::max(bytesPerSample, 1);
	if(pBytesPerSample == bytesPerSample){
		return;
	}
	
	pBytesPerSample = bytesPerSample;
	
	pEngSynthesizer->SetBytesPerSample(bytesPerSample);
	NotifySynthesizerChanged();
}

void seSynthesizer::SetSampleCount(int sampleCount){
	sampleCount = decMath::max(sampleCount, 0);
	
	if(pSampleCount == sampleCount){
		return;
	}
	
	pSampleCount = sampleCount;
	pUpdatePlayTime();
	
	Stop();
	
	pEngSynthesizer->SetSampleCount(sampleCount);
	pEngSynthesizerInstance->SetSampleCount(sampleCount);
	pEngSpeaker->SetPlayPosition(0, sampleCount);
	NotifySynthesizerChanged();
}



void seSynthesizer::Dispose(){
	GetUndoSystem()->RemoveAll();
	RemoveAllSources();
	RemoveAllLinks();
	RemoveAllControllers();
}

void seSynthesizer::Reset(){
	GetUndoSystem()->RemoveAll();
}

void seSynthesizer::ActivateMicrophone(){
	GetEngine()->GetAudioSystem()->SetActiveMicrophone(pEngMicrophone);
}



// Engine Specific
////////////////////

void seSynthesizer::Update(float elapsed){
	pEngWorld->Update(elapsed);
}



// Editing
////////////

bool seSynthesizer::GetLooping() const{
	return pEngSpeaker->GetLooping();
}

void seSynthesizer::SetLooping(bool looping){
	if(looping == pEngSpeaker->GetLooping()){
		return;
	}
	pEngSpeaker->SetLooping(looping);
	NotifyPlaybackChanged();
}

bool seSynthesizer::GetStopped() const{
	return pEngSpeaker->GetStopped();
}

bool seSynthesizer::GetPaused() const{
	return pEngSpeaker->GetPaused();
}

bool seSynthesizer::GetPlaying() const{
	return pEngSpeaker->GetPlaying();
}

void seSynthesizer::Play(){
	if(pEngSpeaker->GetPlaying()){
		return;
	}
	pEngSpeaker->Play();
	NotifyPlaybackChanged();
}

void seSynthesizer::Pause(){
	if(pEngSpeaker->GetPaused()){
		return;
	}
	pEngSpeaker->Pause();
	NotifyPlaybackChanged();
}

void seSynthesizer::Stop(){
	if(pEngSpeaker->GetStopped()){
		return;
	}
	pEngSpeaker->Stop();
	NotifyPlaybackChanged();
}

float seSynthesizer::GetSpeed() const{
	return pEngSpeaker->GetPlaySpeed();
}

void seSynthesizer::SetSpeed(float speed){
	if(fabsf(speed - pEngSpeaker->GetPlaySpeed()) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pEngSpeaker->SetPlaySpeed(speed);
	NotifyPlaybackChanged();
}

float seSynthesizer::GetVolume() const{
	return pEngSpeaker->GetVolume();
}

void seSynthesizer::SetVolume(float volume){
	if(fabsf(volume - pEngSpeaker->GetVolume()) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pEngSpeaker->SetVolume(volume);
	NotifyPlaybackChanged();
}



// Controllers
////////////////

void seSynthesizer::AddController(seController *controller){
	pControllers.Add(controller);
	controller->SetSynthesizer(this);
	
	NotifyControllerStructureChanged();
}

void seSynthesizer::InsertControllerAt(seController *controller, int index){
	pControllers.Insert(controller, index);
	controller->SetSynthesizer(this);
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void seSynthesizer::MoveControllerTo(seController *controller, int index){
	pControllers.Move(controller, index);
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void seSynthesizer::RemoveController(seController *controller){
	const int index = pControllers.IndexOf(controller);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(pActiveController == controller){
		if(pControllers.GetCount() == 1){
			SetActiveController(NULL);
			
		}else if(index < pControllers.GetCount() - 1){
			SetActiveController(pControllers.GetAt(index + 1));
			
		}else{
			SetActiveController(pControllers.GetAt(index - 1));
		}
	}
	
	controller->SetSynthesizer(NULL);
	pControllers.Remove(controller);
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void seSynthesizer::RemoveAllControllers(){
	SetActiveController(NULL);
	
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetSynthesizer(NULL);
	}
	pControllers.RemoveAll();
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void seSynthesizer::SetActiveController(seController *controller){
	if(pActiveController == controller){
		return;
	}
	
	pActiveController = controller;
	NotifyActiveControllerChanged();
}



// Links
//////////

void seSynthesizer::AddLink(seLink *link){
	pLinks.Add(link);
	link->SetSynthesizer(this);
	NotifyLinkStructureChanged();
}

void seSynthesizer::InsertLinkAt(seLink *link, int index){
	pLinks.Insert(link, index);
	link->SetSynthesizer(this);
	
	RebuildSources();
	NotifyLinkStructureChanged();
}

void seSynthesizer::MoveLinkTo(seLink *link, int index){
	pLinks.Move(link, index);
	RebuildSources();
	NotifyLinkStructureChanged();
}

void seSynthesizer::RemoveLink(seLink *link){
	const int index = pLinks.IndexOf(link);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(pActiveLink == link){
		if(pLinks.GetCount() == 1){
			SetActiveLink(NULL);
			
		}else if(index < pLinks.GetCount() - 1){
			SetActiveLink(pLinks.GetAt(index + 1));
			
		}else{
			SetActiveLink(pLinks.GetAt(index - 1));
		}
	}
	
	pLinks.Remove(link);
	
	RebuildSources();
	NotifyLinkStructureChanged();
}

void seSynthesizer::RemoveAllLinks(){
	SetActiveLink(NULL);
	
	const int count = pLinks.GetCount();
	int i;
	for(i=0; i<count; i++){
		pLinks.GetAt(i)->SetSynthesizer(NULL);
	}
	pLinks.RemoveAll();
	
	RebuildSources();
	NotifyLinkStructureChanged();
}

void seSynthesizer::SetActiveLink(seLink *link){
	if(pActiveLink == link){
		return;
	}
	
	pActiveLink = link;
	NotifyActiveLinkChanged();
}

int seSynthesizer::CountLinkUsage(seLink *link) const{
	const int count = pSources.GetCount();
	int i, usageCount = 0;
	
	for(i=0; i<count; i++){
		usageCount += pSources.GetAt(i)->CountLinkUsage(link);
	}
	
	return usageCount;
}



// Sources
////////////

void seSynthesizer::AddSource(seSource *source){
	pSources.Add(source);
	source->SetSynthesizer(this);
	
	RebuildSources();
	
	NotifySourceStructureChanged();
}

void seSynthesizer::InsertSourceAt(seSource *source, int index){
	pSources.Insert(source, index);
	source->SetSynthesizer(this);
	
	RebuildSources();
	
	NotifySourceStructureChanged();
}

void seSynthesizer::MoveSourceTo(seSource *source, int index){
	pSources.Move(source, index);
	RebuildSources();
	NotifySourceStructureChanged();
}

void seSynthesizer::RemoveSource(seSource *source){
	const int index = pSources.IndexOf(source);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(pActiveSource == source){
		if(pSources.GetCount() == 1){
			SetActiveSource(NULL);
			
		}else if(index < pSources.GetCount() - 1){
			SetActiveSource(pSources.GetAt(index + 1));
			
		}else{
			SetActiveSource(pSources.GetAt(index - 1));
		}
	}
	
	source->SetSynthesizer(NULL);
	pSources.Remove(source);
	
	RebuildSources();
	NotifySourceStructureChanged();
}

void seSynthesizer::RemoveAllSources(){
	SetActiveSource(NULL);
	
	const int count = pSources.GetCount();
	int i;
	for(i=0; i<count; i++){
		pSources.GetAt(i)->SetSynthesizer(NULL);
	}
	pSources.RemoveAll();
	
	RebuildSources();
	NotifySourceStructureChanged();
}

void seSynthesizer::SetActiveSource(seSource *source){
	if(pActiveSource == source){
		return;
	}
	
	pActiveSource = source;
	NotifyActiveSourceChanged();
}

void seSynthesizer::RebuildSources(){
	deSynthesizerSource *engSource = NULL;
	const int count = pSources.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pSources.GetAt(i)->SetEngineSource(NULL);
	}
	
	pEngSynthesizer->RemoveAllSources();
	
	try{
		for(i=0; i<count; i++){
			seSource &source = *pSources.GetAt(i);
			
			engSource = source.CreateEngineSource();
			pEngSynthesizer->AddSource(engSource);
			source.SetEngineSource(engSource);
			engSource->FreeReference();
			engSource = NULL;
		}
		
	}catch(const deException &){
		if(engSource){
			engSource->FreeReference();
		}
		throw;
	}
}

void seSynthesizer::DirectoryChanged(){
	const int count = pSources.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pSources.GetAt(i)->SynthesizerDirectoryChanged();
	}
}



// Notifiers
//////////////

void seSynthesizer::AddNotifier(seSynthesizerNotifier *notifier){
	pListeners.Add(notifier);
}

void seSynthesizer::RemoveNotifier(seSynthesizerNotifier *notifier){
	pListeners.Remove(notifier);
}



void seSynthesizer::NotifyStateChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->StateChanged(this);
	}
}

void seSynthesizer::NotifyUndoChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->UndoChanged(this);
	}
}

void seSynthesizer::NotifySynthesizerChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->SynthesizerChanged(this);
	}
	
	SetChanged(true);
}

void seSynthesizer::NotifyPlaybackChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->PlaybackChanged(this);
	}
}



void seSynthesizer::NotifyActiveControllerChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->ActiveControllerChanged(this, pActiveController);
	}
}

void seSynthesizer::NotifyControllerChanged(seController *controller){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->ControllerChanged(this, controller);
	}
	
	SetChanged(true);
}

void seSynthesizer::NotifyControllerNameChanged(seController *controller){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->ControllerNameChanged(this, controller);
	}
	
	SetChanged(true);
}

void seSynthesizer::NotifyControllerRangeChanged(seController *controller){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->ControllerRangeChanged(this, controller);
	}
	
	SetChanged(true);
}

void seSynthesizer::NotifyControllerCurveChanged(seController *controller){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->ControllerCurveChanged(this, controller);
	}
	
	SetChanged(true);
}

void seSynthesizer::NotifyControllerStructureChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	pUpdateEngineControllers();
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->ControllerStructureChanged(this);
	}
	
	SetChanged(true);
}



void seSynthesizer::NotifyActiveLinkChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->ActiveLinkChanged(this, pActiveLink);
	}
}

void seSynthesizer::NotifyLinkChanged(seLink *link){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->LinkChanged(this, link);
	}
	
	SetChanged(true);
}

void seSynthesizer::NotifyLinkNameChanged(seLink *link){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->LinkNameChanged(this, link);
	}
	
	SetChanged(true);
}

void seSynthesizer::NotifyLinkStructureChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->LinkStructureChanged(this);
	}
	
	SetChanged(true);
}



void seSynthesizer::NotifyActiveSourceChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->ActiveSourceChanged(this, pActiveSource);
	}
}

void seSynthesizer::NotifySourceChanged(seSource *source){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->SourceChanged(this, source);
	}
	
	SetChanged(true);
}

void seSynthesizer::NotifySourceNameChanged(seSource *source){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->SourceNameChanged(this, source);
	}
	
	SetChanged(true);
}

void seSynthesizer::NotifySourceStructureChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->SourceStructureChanged(this);
	}
	
	SetChanged(true);
}

void seSynthesizer::NotifyActiveEffectChanged(seSource *source){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->ActiveEffectChanged(this, source);
	}
}

void seSynthesizer::NotifyEffectChanged(seSource *source, seEffect *effect){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->EffectChanged(this, source, effect);
	}
	
	SetChanged(true);
}

void seSynthesizer::NotifyEffectStructureChanged(seSource *source){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((seSynthesizerNotifier*)pListeners.GetAt(i))->EffectStructureChanged(this, source);
	}
	
	SetChanged(true);
}



// Private Functions
//////////////////////

void seSynthesizer::pCleanUp(){
	pListeners.RemoveAll();
	
	RemoveAllSources();
	RemoveAllLinks();
	RemoveAllControllers();
	
	if(pEngSynthesizerInstance){
		if(pEngSpeaker){
			pEngSpeaker->SetSynthesizer(NULL);
		}
		
		pEngSynthesizerInstance->SetSynthesizer(NULL);
	}
	
	
	if(pEngSpeaker){
		if(pEngMicrophone){
			pEngMicrophone->RemoveSpeaker(pEngSpeaker);
		}
	}
	
	if(pEngWorld){
		if(pEngMicrophone){
			if(GetEngine()->GetAudioSystem()->GetActiveMicrophone() == pEngMicrophone){
				GetEngine()->GetAudioSystem()->SetActiveMicrophone(NULL);
			}
			
			pEngWorld->RemoveMicrophone(pEngMicrophone);
		}
		
	}
}



void seSynthesizer::pCreateWorld(){
	deEngine * const engine = GetEngine();
	
	// create world
	pEngWorld.TakeOver(engine->GetWorldManager()->CreateWorld();
	pEngWorld->SetGravity(decVector(0.0f, -9.81f, 0.0f));
	pEngWorld->SetDisableLights(false);
	
	pEngWorld->SetAmbientLight(decColor(0.0f, 0.0f, 0.0f));
	
	// create microphone
	decLayerMask layerMaskAudio;
	layerMaskAudio.SetBit(0);
	
	pEngMicrophone.TakeOver(engine->GetMicrophoneManager()->CreateMicrophone();
	pEngMicrophone->SetType(deMicrophone::emtPoint);
	pEngMicrophone->SetLayerMask(layerMaskAudio);
	pEngWorld->AddMicrophone(pEngMicrophone);
	
	// create speaker
	pEngSpeaker.TakeOver(engine->GetSpeakerManager()->CreateSpeaker();
	pEngSpeaker->SetType(deSpeaker::estPoint);
	pEngSpeaker->SetLooping(false);
	pEngSpeaker->SetPlaySpeed(1.0f);
	pEngSpeaker->SetVolume(1.0f);
	pEngSpeaker->SetPlayPosition(0, pSampleCount);
	pEngSpeaker->SetLayerMask(layerMaskAudio);
	pEngMicrophone->AddSpeaker(pEngSpeaker);
	
	// create synthesizer
	pEngSynthesizer.TakeOver(engine->GetSynthesizerManager()->CreateSynthesizer();
	pEngSynthesizer->SetChannelCount(pChannelCount);
	pEngSynthesizer->SetSampleRate(pSampleRate);
	pEngSynthesizer->SetBytesPerSample(pBytesPerSample);
	pEngSynthesizer->SetSampleCount(pSampleCount);
}

void seSynthesizer::pUpdateLinks(){
	const int count = pLinks.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pLinks.GetAt(i)->UpdateController();
	}
	
	RebuildSources();
}

void seSynthesizer::pUpdateEngineControllers(){
	// set all engine controller indices in our controllers to -1
	const int count = pControllers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetEngineControllerIndex(-1);
	}
	
	// remove the synthesizer from the synthesizer instance
	pEngSynthesizerInstance->SetSynthesizer(NULL);
	
	// remove all controllers
	pEngSynthesizer->RemoveAllControllers();
	
	// add an engine controller for each controller we have
	deSynthesizerController *engController = NULL;
	
	try{
		const int controllerCount = pControllers.GetCount();
		
		for(i=0; i<controllerCount; i++){
			engController = new deSynthesizerController;
			pEngSynthesizer->AddController(engController);
			engController = NULL;
		}
		
	}catch(const deException &){
		if(engController){
			engController->FreeReference();
		}
		
		throw;
	}
	
	// assign the synthesizer to the synthesizer instance. this creates the controllers
	// inside the synthesizer instance
	pEngSynthesizerInstance->SetSynthesizer(pEngSynthesizer);
	pEngSynthesizerInstance->SetSampleCount(pSampleCount);
	
	// now assign the matching engine controller indices to our controllers
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetEngineControllerIndex(i);
	}
	
	// links have to be updated now
	pUpdateLinks();
}

void seSynthesizer::pUpdatePlayTime(){
	pPlayTime = (float)pSampleCount / (float)pSampleRate;
}
