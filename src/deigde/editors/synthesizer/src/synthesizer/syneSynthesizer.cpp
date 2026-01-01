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

#include "syneSynthesizer.h"
#include "syneSynthesizerNotifier.h"
#include "controller/syneController.h"
#include "link/syneLink.h"
#include "source/syneSource.h"

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



// Class syneSynthesizer
/////////////////////

// Constructor, destructor
////////////////////////////

syneSynthesizer::syneSynthesizer(igdeEnvironment *environment, syneLoadSaveSystem &lssys) :
igdeEditableEntity(environment),
pLoadSaveSystem(lssys),
pChannelCount(1),
pSampleRate(44100),
pBytesPerSample(2),
pSampleCount(pSampleRate * 10),
pPlayTime(10.0f)
{
	try{
		SetFilePath("new.desynth");
		
		pCreateWorld();
		
		pEngSynthesizerInstance = GetEngine()->GetSynthesizerInstanceManager()->CreateSynthesizerInstance();
		pEngSynthesizerInstance->SetSynthesizer(pEngSynthesizer);
		pEngSynthesizerInstance->SetSampleCount(pSampleCount);
		
		pEngSpeaker->SetSynthesizer(pEngSynthesizerInstance);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

syneSynthesizer::~syneSynthesizer(){
	pCleanUp();
}



// Management
///////////////

void syneSynthesizer::SetChannelCount(int channelCount){
	channelCount = decMath::max(channelCount, 1);
	if(channelCount == pChannelCount){
		return;
	}
	
	pChannelCount = channelCount;
	
	pEngSynthesizer->SetChannelCount(channelCount);
	NotifySynthesizerChanged();
}

void syneSynthesizer::SetSampleRate(int sampleRate){
	sampleRate = decMath::max(sampleRate, 1);
	if(sampleRate == pSampleRate){
		return;
	}
	
	pSampleRate = sampleRate;
	pUpdatePlayTime();
	
	pEngSynthesizer->SetSampleRate(sampleRate);
	NotifySynthesizerChanged();
}

void syneSynthesizer::SetBytesPerSample(int bytesPerSample){
	bytesPerSample = decMath::max(bytesPerSample, 1);
	if(bytesPerSample == pBytesPerSample){
		return;
	}
	
	pBytesPerSample = bytesPerSample;
	
	pEngSynthesizer->SetBytesPerSample(bytesPerSample);
	NotifySynthesizerChanged();
}

void syneSynthesizer::SetSampleCount(int sampleCount){
	sampleCount = decMath::max(sampleCount, 0);
	
	if(sampleCount == pSampleCount){
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



void syneSynthesizer::Dispose(){
	GetUndoSystem()->RemoveAll();
	RemoveAllSources();
	RemoveAllLinks();
	RemoveAllControllers();
}

void syneSynthesizer::Reset(){
	GetUndoSystem()->RemoveAll();
}

void syneSynthesizer::ActivateMicrophone(){
	GetEngine()->GetAudioSystem()->SetActiveMicrophone(pEngMicrophone);
}



// Engine Specific
////////////////////

void syneSynthesizer::Update(float elapsed){
	pEngWorld->Update(elapsed);
}



// Editing
////////////

bool syneSynthesizer::GetLooping() const{
	return pEngSpeaker->GetLooping();
}

void syneSynthesizer::SetLooping(bool looping){
	if(looping == pEngSpeaker->GetLooping()){
		return;
	}
	pEngSpeaker->SetLooping(looping);
	NotifyPlaybackChanged();
}

bool syneSynthesizer::GetStopped() const{
	return pEngSpeaker->GetStopped();
}

bool syneSynthesizer::GetPaused() const{
	return pEngSpeaker->GetPaused();
}

bool syneSynthesizer::GetPlaying() const{
	return pEngSpeaker->GetPlaying();
}

void syneSynthesizer::Play(){
	if(pEngSpeaker->GetPlaying()){
		return;
	}
	pEngSpeaker->Play();
	NotifyPlaybackChanged();
}

void syneSynthesizer::Pause(){
	if(pEngSpeaker->GetPaused()){
		return;
	}
	pEngSpeaker->Pause();
	NotifyPlaybackChanged();
}

void syneSynthesizer::Stop(){
	if(pEngSpeaker->GetStopped()){
		return;
	}
	pEngSpeaker->Stop();
	NotifyPlaybackChanged();
}

float syneSynthesizer::GetSpeed() const{
	return pEngSpeaker->GetPlaySpeed();
}

void syneSynthesizer::SetSpeed(float speed){
	if(fabsf(speed - pEngSpeaker->GetPlaySpeed()) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pEngSpeaker->SetPlaySpeed(speed);
	NotifyPlaybackChanged();
}

float syneSynthesizer::GetVolume() const{
	return pEngSpeaker->GetVolume();
}

void syneSynthesizer::SetVolume(float volume){
	if(fabsf(volume - pEngSpeaker->GetVolume()) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pEngSpeaker->SetVolume(volume);
	NotifyPlaybackChanged();
}



// Controllers
////////////////

void syneSynthesizer::AddController(syneController *controller){
	pControllers.Add(controller);
	controller->SetSynthesizer(this);
	
	NotifyControllerStructureChanged();
}

void syneSynthesizer::InsertControllerAt(syneController *controller, int index){
	pControllers.Insert(controller, index);
	controller->SetSynthesizer(this);
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void syneSynthesizer::MoveControllerTo(syneController *controller, int index){
	pControllers.Move(controller, index);
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void syneSynthesizer::RemoveController(syneController *controller){
	const int index = pControllers.IndexOf(controller);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(controller == pActiveController){
		if(pControllers.GetCount() == 1){
			SetActiveController(nullptr);
			
		}else if(index < pControllers.GetCount() - 1){
			SetActiveController(pControllers.GetAt(index + 1));
			
		}else{
			SetActiveController(pControllers.GetAt(index - 1));
		}
	}
	
	controller->SetSynthesizer(nullptr);
	pControllers.Remove(controller);
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void syneSynthesizer::RemoveAllControllers(){
	SetActiveController(nullptr);
	
	const int count = pControllers.GetCount();
	int i;
	for(i=0; i<count; i++){
		pControllers.GetAt(i)->SetSynthesizer(nullptr);
	}
	pControllers.RemoveAll();
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void syneSynthesizer::SetActiveController(syneController *controller){
	if(controller == pActiveController){
		return;
	}
	
	pActiveController = controller;
	NotifyActiveControllerChanged();
}



// Links
//////////

void syneSynthesizer::AddLink(syneLink *link){
	pLinks.Add(link);
	link->SetSynthesizer(this);
	NotifyLinkStructureChanged();
}

void syneSynthesizer::InsertLinkAt(syneLink *link, int index){
	pLinks.Insert(link, index);
	link->SetSynthesizer(this);
	
	RebuildSources();
	NotifyLinkStructureChanged();
}

void syneSynthesizer::MoveLinkTo(syneLink *link, int index){
	pLinks.Move(link, index);
	RebuildSources();
	NotifyLinkStructureChanged();
}

void syneSynthesizer::RemoveLink(syneLink *link){
	const int index = pLinks.IndexOf(link);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(link == pActiveLink){
		if(pLinks.GetCount() == 1){
			SetActiveLink(nullptr);
			
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

void syneSynthesizer::RemoveAllLinks(){
	SetActiveLink(nullptr);
	
	pLinks.Visit([](syneLink &link){
		link.SetSynthesizer(nullptr);
	});
	pLinks.RemoveAll();
	
	RebuildSources();
	NotifyLinkStructureChanged();
}

void syneSynthesizer::SetActiveLink(syneLink *link){
	if(link == pActiveLink){
		return;
	}
	
	pActiveLink = link;
	NotifyActiveLinkChanged();
}

int syneSynthesizer::CountLinkUsage(syneLink *link) const{
	int usageCount = 0;
	
	pSources.Visit([&](syneSource &source){
		usageCount += source.CountLinkUsage(link);
	});
	
	return usageCount;
}



// Sources
////////////

void syneSynthesizer::AddSource(syneSource *source){
	pSources.Add(source);
	source->SetSynthesizer(this);
	
	RebuildSources();
	
	NotifySourceStructureChanged();
}

void syneSynthesizer::InsertSourceAt(syneSource *source, int index){
	pSources.Insert(source, index);
	source->SetSynthesizer(this);
	
	RebuildSources();
	
	NotifySourceStructureChanged();
}

void syneSynthesizer::MoveSourceTo(syneSource *source, int index){
	pSources.Move(source, index);
	RebuildSources();
	NotifySourceStructureChanged();
}

void syneSynthesizer::RemoveSource(syneSource *source){
	const int index = pSources.IndexOf(source);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(source == pActiveSource){
		if(pSources.GetCount() == 1){
			SetActiveSource(nullptr);
			
		}else if(index < pSources.GetCount() - 1){
			SetActiveSource(pSources.GetAt(index + 1));
			
		}else{
			SetActiveSource(pSources.GetAt(index - 1));
		}
	}
	
	source->SetSynthesizer(nullptr);
	pSources.Remove(source);
	
	RebuildSources();
	NotifySourceStructureChanged();
}

void syneSynthesizer::RemoveAllSources(){
	SetActiveSource(nullptr);
	
	const int count = pSources.GetCount();
	int i;
	for(i=0; i<count; i++){
		pSources.GetAt(i)->SetSynthesizer(nullptr);
	}
	pSources.RemoveAll();
	
	RebuildSources();
	NotifySourceStructureChanged();
}

void syneSynthesizer::SetActiveSource(syneSource *source){
	if(source == pActiveSource){
		return;
	}
	
	pActiveSource = source;
	NotifyActiveSourceChanged();
}

void syneSynthesizer::RebuildSources(){
	pSources.Visit([](syneSource &source){
		source.SetEngineSource(nullptr);
	});
	
	pEngSynthesizer->RemoveAllSources();
	
	pSources.Visit([&](syneSource &source){
		const deSynthesizerSource::Ref engSource(source.CreateEngineSource());
		pEngSynthesizer->AddSource(engSource);
		source.SetEngineSource(engSource);
	});
}

void syneSynthesizer::DirectoryChanged(){
	pSources.Visit([](syneSource &source){
		source.SynthesizerDirectoryChanged();
	});
}



// Notifiers
//////////////

void syneSynthesizer::AddNotifier(syneSynthesizerNotifier *notifier){
	pListeners.Add(notifier);
}

void syneSynthesizer::RemoveNotifier(syneSynthesizerNotifier *notifier){
	pListeners.Remove(notifier);
}

void syneSynthesizer::NotifyStateChanged(){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.StateChanged(this);
	});
}

void syneSynthesizer::NotifyUndoChanged(){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.UndoChanged(this);
	});
}

void syneSynthesizer::NotifySynthesizerChanged(){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.SynthesizerChanged(this);
	});
	
	SetChanged(true);
}

void syneSynthesizer::NotifyPlaybackChanged(){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.PlaybackChanged(this);
	});
}



void syneSynthesizer::NotifyActiveControllerChanged(){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.ActiveControllerChanged(this, pActiveController);
	});
}

void syneSynthesizer::NotifyControllerChanged(syneController *controller){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.ControllerChanged(this, controller);
	});
	
	SetChanged(true);
}

void syneSynthesizer::NotifyControllerNameChanged(syneController *controller){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.ControllerNameChanged(this, controller);
	});
	
	SetChanged(true);
}

void syneSynthesizer::NotifyControllerRangeChanged(syneController *controller){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.ControllerRangeChanged(this, controller);
	});
	
	SetChanged(true);
}

void syneSynthesizer::NotifyControllerCurveChanged(syneController *controller){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.ControllerCurveChanged(this, controller);
	});
	
	SetChanged(true);
}

void syneSynthesizer::NotifyControllerStructureChanged(){
	pUpdateEngineControllers();
	
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.ControllerStructureChanged(this);
	});
	
	SetChanged(true);
}



void syneSynthesizer::NotifyActiveLinkChanged(){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.ActiveLinkChanged(this, pActiveLink);
	});
}

void syneSynthesizer::NotifyLinkChanged(syneLink *link){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.LinkChanged(this, link);
	});
	
	SetChanged(true);
}

void syneSynthesizer::NotifyLinkNameChanged(syneLink *link){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.LinkNameChanged(this, link);
	});
	
	SetChanged(true);
}

void syneSynthesizer::NotifyLinkStructureChanged(){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.LinkStructureChanged(this);
	});
	
	SetChanged(true);
}



void syneSynthesizer::NotifyActiveSourceChanged(){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.ActiveSourceChanged(this, pActiveSource);
	});
}

void syneSynthesizer::NotifySourceChanged(syneSource *source){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.SourceChanged(this, source);
	});
	
	SetChanged(true);
}

void syneSynthesizer::NotifySourceNameChanged(syneSource *source){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.SourceNameChanged(this, source);
	});
	
	SetChanged(true);
}

void syneSynthesizer::NotifySourceStructureChanged(){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.SourceStructureChanged(this);
	});
	
	SetChanged(true);
}

void syneSynthesizer::NotifyActiveEffectChanged(syneSource *source){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.ActiveEffectChanged(this, source);
	});
}

void syneSynthesizer::NotifyEffectChanged(syneSource *source, syneEffect *effect){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.EffectChanged(this, source, effect);
	});
	
	SetChanged(true);
}

void syneSynthesizer::NotifyEffectStructureChanged(syneSource *source){
	pListeners.Visit([&](syneSynthesizerNotifier &l){
		l.EffectStructureChanged(this, source);
	});
	
	SetChanged(true);
}


// Private Functions
//////////////////////

void syneSynthesizer::pCleanUp(){
	pListeners.RemoveAll();
	
	RemoveAllSources();
	RemoveAllLinks();
	RemoveAllControllers();
	
	if(pEngSynthesizerInstance){
		if(pEngSpeaker){
			pEngSpeaker->SetSynthesizer(nullptr);
		}
		
		pEngSynthesizerInstance->SetSynthesizer(nullptr);
	}
	if(pEngSpeaker){
		if(pEngMicrophone){
			pEngMicrophone->RemoveSpeaker(pEngSpeaker);
		}
	}
	
	if(pEngWorld){
		if(pEngMicrophone){
			if(GetEngine()->GetAudioSystem()->GetActiveMicrophone() == pEngMicrophone){
				GetEngine()->GetAudioSystem()->SetActiveMicrophone(nullptr);
			}
			
			pEngWorld->RemoveMicrophone(pEngMicrophone);
		}
	}
}



void syneSynthesizer::pCreateWorld(){
	deEngine * const engine = GetEngine();
	
	// create world
	pEngWorld = engine->GetWorldManager()->CreateWorld();
	pEngWorld->SetGravity(decVector(0.0f, -9.81f, 0.0f));
	pEngWorld->SetDisableLights(false);
	
	pEngWorld->SetAmbientLight(decColor(0.0f, 0.0f, 0.0f));
	
	// create microphone
	decLayerMask layerMaskAudio;
	layerMaskAudio.SetBit(0);
	
	pEngMicrophone = engine->GetMicrophoneManager()->CreateMicrophone();
	pEngMicrophone->SetType(deMicrophone::emtPoint);
	pEngMicrophone->SetLayerMask(layerMaskAudio);
	pEngWorld->AddMicrophone(pEngMicrophone);
	
	// create speaker
	pEngSpeaker = engine->GetSpeakerManager()->CreateSpeaker();
	pEngSpeaker->SetType(deSpeaker::estPoint);
	pEngSpeaker->SetLooping(false);
	pEngSpeaker->SetPlaySpeed(1.0f);
	pEngSpeaker->SetVolume(1.0f);
	pEngSpeaker->SetPlayPosition(0, pSampleCount);
	pEngSpeaker->SetLayerMask(layerMaskAudio);
	pEngMicrophone->AddSpeaker(pEngSpeaker);
	
	// create synthesizer
	pEngSynthesizer = engine->GetSynthesizerManager()->CreateSynthesizer();
	pEngSynthesizer->SetChannelCount(pChannelCount);
	pEngSynthesizer->SetSampleRate(pSampleRate);
	pEngSynthesizer->SetBytesPerSample(pBytesPerSample);
	pEngSynthesizer->SetSampleCount(pSampleCount);
}

void syneSynthesizer::pUpdateLinks(){
	pLinks.Visit([](syneLink &link){
		link.UpdateController();
	});
	
	RebuildSources();
}

void syneSynthesizer::pUpdateEngineControllers(){
	// set all engine controller indices in our controllers to -1
	pControllers.Visit([](syneController &controller){
		controller.SetEngineControllerIndex(-1);
	});
	
	// remove the synthesizer from the synthesizer instance
	pEngSynthesizerInstance->SetSynthesizer(nullptr);
	
	// remove all controllers
	pEngSynthesizer->RemoveAllControllers();
	
	// add an engine controller for each controller we have
	pControllers.Visit([&](syneController &){
		pEngSynthesizer->AddController(deSynthesizerController::Ref::New());
	});
	
	// assign the synthesizer to the synthesizer instance. this creates the controllers
	// inside the synthesizer instance
	pEngSynthesizerInstance->SetSynthesizer(pEngSynthesizer);
	pEngSynthesizerInstance->SetSampleCount(pSampleCount);
	
	// now assign the matching engine controller indices to our controllers
	pControllers.VisitIndexed([&](int i, syneController &controller){
		controller.SetEngineControllerIndex(i);
	});
	
	// links have to be updated now
	pUpdateLinks();
}

void syneSynthesizer::pUpdatePlayTime(){
	pPlayTime = (float)pSampleCount / (float)pSampleRate;
}
