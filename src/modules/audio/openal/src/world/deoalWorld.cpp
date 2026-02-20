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

#include "deoalWorld.h"
#include "deoalAWorld.h"
#include "octree/deoalWorldOctree.h"
#include "../deAudioOpenAL.h"
#include "../component/deoalComponent.h"
#include "../component/deoalAComponent.h"
#include "../microphone/deoalMicrophone.h"
#include "../microphone/deoalAMicrophone.h"
#include "../speaker/deoalSpeaker.h"
#include "../speaker/deoalASpeaker.h"
#include "../soundLevelMeter/deoalSoundLevelMeter.h"
#include "../soundLevelMeter/deoalASoundLevelMeter.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/sensor/deSoundLevelMeter.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoalWorld
/////////////////////

// Constructor, destructor
////////////////////////////

deoalWorld::deoalWorld(deAudioOpenAL &oal, deWorld &world) :
pOal(oal),
pWorld(world),

pDirtyComponents(false),
pDirtySpeakers(false),
pDirtyMicrophones(false),
pDirtySoundLevelMeters(false),
pDirtyAllMicLayerMask(true),
pDirtyAudioParameters(true)
{
	try{
		pAWorld = deoalAWorld::Ref::New(oal.GetAudioThread(), world.GetSize() * 0.5);
		
		world.GetComponents().Visit([&](deComponent *component){
			ComponentAdded(component);
		});
		
		world.GetSpeakers().Visit([&](deSpeaker *speaker){
			SpeakerAdded(speaker);
		});
		
		world.GetMicrophones().Visit([&](deMicrophone *microphone){
			MicrophoneAdded(microphone);
		});
		
		world.GetSoundLevelMeters().Visit([&](deSoundLevelMeter *soundLevelMeter){
			SoundLevelMeterAdded(soundLevelMeter);
		});
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoalWorld::~deoalWorld(){
	pCleanUp();
}



// Management
///////////////

bool deoalWorld::IsAudible() const{
	return pOal.GetActiveWorld() == this;
}

void deoalWorld::Synchronize(){
	pSyncAllMicLayerMask();
	pSyncAudioParameters();
	
	pSyncComponents();
	pSyncSpeakers();
	pSyncMicrophones();
	pSyncSoundLevelMeters();
}

void deoalWorld::SetDirtyAllMicLayerMask(){
	pDirtyAllMicLayerMask = true;
}

void deoalWorld::DevModeChanged(){
	pWorld.GetComponents().Visit([](deComponent *component){
		((deoalComponent*)component->GetPeerAudio())->DevModeChanged();
	});
}



void deoalWorld::AddSyncComponent(deoalComponent *component){
	if(!component){
		DETHROW(deeInvalidParam);
	}
	if(!component->GetLLSyncWorld().GetList()){
		pListSyncComponents.Add(&component->GetLLSyncWorld());
	}
}

void deoalWorld::RemoveSyncComponent(deoalComponent *component){
	if(!component){
		DETHROW(deeInvalidParam);
	}
	if(component->GetLLSyncWorld().GetList()){
		pListSyncComponents.Remove(&component->GetLLSyncWorld());
	}
}

void deoalWorld::AddSyncSpeaker(deoalSpeaker *speaker){
	if(!speaker){
		DETHROW(deeInvalidParam);
	}
	if(!speaker->GetLLSyncWorld().GetList()){
		pListSyncSpeakers.Add(&speaker->GetLLSyncWorld());
	}
}

void deoalWorld::RemoveSyncSpeaker(deoalSpeaker *speaker){
	if(!speaker){
		DETHROW(deeInvalidParam);
	}
	if(speaker->GetLLSyncWorld().GetList()){
		pListSyncSpeakers.Remove(&speaker->GetLLSyncWorld());
	}
}

void deoalWorld::AddSyncMicrophone(deoalMicrophone *microphone){
	if(!microphone){
		DETHROW(deeInvalidParam);
	}
	if(!microphone->GetLLSyncWorld().GetList()){
		pListSyncMicrophones.Add(&microphone->GetLLSyncWorld());
	}
}

void deoalWorld::RemoveSyncMicrophone(deoalMicrophone *microphone){
	if(!microphone){
		DETHROW(deeInvalidParam);
	}
	if(microphone->GetLLSyncWorld().GetList()){
		pListSyncMicrophones.Remove(&microphone->GetLLSyncWorld());
	}
}

void deoalWorld::AddSyncSoundLevelMeter(deoalSoundLevelMeter *soundLevelMeter){
	if(!soundLevelMeter){
		DETHROW(deeInvalidParam);
	}
	if(!soundLevelMeter->GetLLSyncWorld().GetList()){
		pListSyncSoundLevelMeters.Add(&soundLevelMeter->GetLLSyncWorld());
	}
}

void deoalWorld::RemoveSyncSoundLevelMeter(deoalSoundLevelMeter *soundLevelMeter){
	if(!soundLevelMeter){
		DETHROW(deeInvalidParam);
	}
	if(soundLevelMeter->GetLLSyncWorld().GetList()){
		pListSyncSoundLevelMeters.Remove(&soundLevelMeter->GetLLSyncWorld());
	}
}



// Notifications
//////////////////

void deoalWorld::Update(float){
	// this is a special check. only one world can be audible. if playback is started on a speaker
	// in a not audible world then the playback does not advance. once the world becomes audible
	// all speakers start playing at the position they have been set to start playback from. for
	// looping speakers this is not a problem but for non-looping speakers as typically used for
	// sound effects this is a problem. first too many sources are required to play them back and
	// second the speakers should have finished already a long time ago but did not get processed.
	if(IsAudible()){
		return;
	}
	
	pWorld.GetSpeakers().Visit([&](deSpeaker *speaker){
		if(speaker->GetPlaying() && !speaker->GetLooping()){
			speaker->Stop();
		}
	});
}



void deoalWorld::SizeChanged(){
}

void deoalWorld::AudioChanged(){
	pDirtyAudioParameters = true;
}

void deoalWorld::SpeakerAdded(deSpeaker *speaker){
	deoalSpeaker * const oalSpeaker = (deoalSpeaker*)speaker->GetPeerAudio();
	oalSpeaker->SetParentWorld(this);
	AddSyncSpeaker(oalSpeaker);
	pDirtySpeakers = true;
}

void deoalWorld::SpeakerRemoved(deSpeaker *speaker){
	deoalSpeaker * const oalSpeaker = (deoalSpeaker*)speaker->GetPeerAudio();
	RemoveSyncSpeaker(oalSpeaker);
	oalSpeaker->GetASpeaker()->SetWorldMarkedRemove(true);
	oalSpeaker->SetParentWorld(nullptr);
	pDirtySpeakers = true;
}

void deoalWorld::AllSpeakersRemoved(){
	pWorld.GetSpeakers().Visit([&](deSpeaker *speaker){
		deoalSpeaker * const oalSpeaker = (deoalSpeaker*)speaker->GetPeerAudio();
		RemoveSyncSpeaker(oalSpeaker);
		oalSpeaker->GetASpeaker()->SetWorldMarkedRemove(true);
		oalSpeaker->SetParentWorld(nullptr);
	});
	
	pDirtySpeakers = true;
}



void deoalWorld::MicrophoneAdded(deMicrophone *microphone){
	deoalMicrophone * const oalMicrophone = (deoalMicrophone*)microphone->GetPeerAudio();
	oalMicrophone->SetParentWorld(this);
	AddSyncMicrophone(oalMicrophone);
	pDirtyMicrophones = true;
	pDirtyAllMicLayerMask = true;
}

void deoalWorld::MicrophoneRemoved(deMicrophone *microphone){
	deoalMicrophone * const oalMicrophone = (deoalMicrophone*)microphone->GetPeerAudio();
	RemoveSyncMicrophone(oalMicrophone);
	oalMicrophone->GetAMicrophone()->SetWorldMarkedRemove(true);
	oalMicrophone->SetParentWorld(nullptr);
	pDirtyMicrophones = true;
	pDirtyAllMicLayerMask = true;
}

void deoalWorld::AllMicrophonesRemoved(){
	pWorld.GetMicrophones().Visit([&](deMicrophone *microphone){
		deoalMicrophone * const oalMicrophone = (deoalMicrophone*)microphone->GetPeerAudio();
		RemoveSyncMicrophone(oalMicrophone);
		oalMicrophone->GetAMicrophone()->SetWorldMarkedRemove(true);
		oalMicrophone->SetParentWorld(nullptr);
	});
	
	pDirtyMicrophones = true;
	pDirtyAllMicLayerMask = true;
}



void deoalWorld::ComponentAdded(deComponent *component){
	deoalComponent * const oalComponent = (deoalComponent*)component->GetPeerAudio();
	oalComponent->SetParentWorld(this);
	AddSyncComponent(oalComponent);
	pDirtyComponents = true;
}

void deoalWorld::ComponentRemoved(deComponent *component){
	deoalComponent * const oalComponent = (deoalComponent*)component->GetPeerAudio();
	RemoveSyncComponent(oalComponent);
	oalComponent->GetAComponent()->SetWorldMarkedRemove(true);
	oalComponent->SetParentWorld(nullptr);
	pDirtyComponents = true;
}

void deoalWorld::AllComponentsRemoved(){
	pWorld.GetComponents().Visit([&](deComponent *component){
		deoalComponent * const oalComponent = (deoalComponent*)component->GetPeerAudio();
		RemoveSyncComponent(oalComponent);
		oalComponent->GetAComponent()->SetWorldMarkedRemove(true);
		oalComponent->SetParentWorld(nullptr);
	});
	
	pDirtyComponents = true;
}



void deoalWorld::SoundLevelMeterAdded(deSoundLevelMeter *soundLevelMeter){
	deoalSoundLevelMeter * const oalSoundLevelMeter =
		(deoalSoundLevelMeter*)soundLevelMeter->GetPeerAudio();
	oalSoundLevelMeter->SetParentWorld(this);
	AddSyncSoundLevelMeter(oalSoundLevelMeter);
	pDirtySoundLevelMeters = true;
}

void deoalWorld::SoundLevelMeterRemoved(deSoundLevelMeter *soundLevelMeter){
	deoalSoundLevelMeter * const oalSoundLevelMeter =
		(deoalSoundLevelMeter*)soundLevelMeter->GetPeerAudio();
	RemoveSyncSoundLevelMeter(oalSoundLevelMeter);
	oalSoundLevelMeter->GetASoundLevelMeter()->SetWorldMarkedRemove(true);
	oalSoundLevelMeter->SetParentWorld(nullptr);
	pDirtySoundLevelMeters = true;
}

void deoalWorld::AllSoundLevelMetersRemoved(){
	pWorld.GetSoundLevelMeters().Visit([&](deSoundLevelMeter *soundLevelMeter){
		deoalSoundLevelMeter * const oalSoundLevelMeter =
			(deoalSoundLevelMeter*)soundLevelMeter->GetPeerAudio();
		RemoveSyncSoundLevelMeter(oalSoundLevelMeter);
		oalSoundLevelMeter->GetASoundLevelMeter()->SetWorldMarkedRemove(true);
		oalSoundLevelMeter->SetParentWorld(nullptr);
	});
	
	pDirtySoundLevelMeters = true;
}



// Private Functions
//////////////////////

void deoalWorld::pCleanUp(){
	pAWorld = nullptr;
	
	AllSoundLevelMetersRemoved();
	AllComponentsRemoved();
	AllMicrophonesRemoved();
	AllSpeakersRemoved();
}

void deoalWorld::pSyncComponents(){
	// add new components
	if(pDirtyComponents){
		pAWorld->RemoveRemovalMarkedComponents();
		
		pWorld.GetComponents().Visit([&](deComponent *engComponent){
			auto component = ((deoalComponent*)engComponent->GetPeerAudio())->GetAComponent();
			if(component->GetParentWorld() != pAWorld){
				pAWorld->AddComponent(component);
			}
		});
		
		pDirtyComponents = false;
	}
	
	// synchronize components requiring to be synchronized
	decTLinkedList<deoalComponent>::Element * const tailComponent = pListSyncComponents.GetTail();
	while(pListSyncComponents.GetRoot()){
		decTLinkedList<deoalComponent>::Element * const entry = pListSyncComponents.GetRoot();
		deoalComponent &component = *entry->GetOwner();
		pListSyncComponents.Remove(entry);
		
		component.Synchronize(); // can potentially re-add the component
		
		if(entry == tailComponent){
			break; // processed last component. re-added component will come next
		}
	}
}

void deoalWorld::pSyncSpeakers(){
	// add new speakers
	if(pDirtySpeakers){
		pAWorld->RemoveRemovalMarkedSpeakers();
		
		pWorld.GetSpeakers().Visit([&](deSpeaker *engSpeaker){
			auto speaker = ((deoalSpeaker*)engSpeaker->GetPeerAudio())->GetASpeaker();
			if(speaker->GetParentWorld() != pAWorld){
				pAWorld->AddSpeaker(speaker);
			}
		});
		
		pDirtySpeakers = false;
	}
	
	// synchronize speakers requiring to be synchronized
	decTLinkedList<deoalSpeaker>::Element * const tailSpeaker = pListSyncSpeakers.GetTail();
	while(pListSyncSpeakers.GetRoot()){
		decTLinkedList<deoalSpeaker>::Element * const entry = pListSyncSpeakers.GetRoot();
		deoalSpeaker &speaker = *entry->GetOwner();
		pListSyncSpeakers.Remove(entry);
		
		speaker.Synchronize(); // can potentially re-add the speaker
		
		if(entry == tailSpeaker){
			break; // processed last speaker. re-added speaker will come next
		}
	}
}

void deoalWorld::pSyncMicrophones(){
	// add new microphones
	if(pDirtyMicrophones){
		pAWorld->RemoveRemovalMarkedMicrophones();
		
		pWorld.GetMicrophones().Visit([&](deMicrophone *engMicrophone){
			auto microphone = ((deoalMicrophone*)engMicrophone->GetPeerAudio())->GetAMicrophone();
			if(microphone->GetParentWorld() != pAWorld){
				pAWorld->AddMicrophone(microphone);
			}
		});
		
		pDirtyMicrophones = false;
	}
	
	// synchronize microphones requiring to be synchronized
	decTLinkedList<deoalMicrophone>::Element * const tailMicrophone = pListSyncMicrophones.GetTail();
	while(pListSyncMicrophones.GetRoot()){
		decTLinkedList<deoalMicrophone>::Element * const entry = pListSyncMicrophones.GetRoot();
		deoalMicrophone &microphone = *entry->GetOwner();
		pListSyncMicrophones.Remove(entry);
		
		microphone.Synchronize(); // can potentially re-add the microphone
		
		if(entry == tailMicrophone){
			break; // processed last microphone. re-added microphone will come next
		}
	}
}

void deoalWorld::pSyncSoundLevelMeters(){
	// add new soundLevelMeters
	if(pDirtySoundLevelMeters){
		pAWorld->RemoveRemovalMarkedSoundLevelMeters();
		
		pWorld.GetSoundLevelMeters().Visit([&](deSoundLevelMeter *engSoundLevelMeter){
			auto soundLevelMeter = ((deoalSoundLevelMeter*)engSoundLevelMeter->GetPeerAudio())->GetASoundLevelMeter();
			if(soundLevelMeter->GetParentWorld() != pAWorld){
				pAWorld->AddSoundLevelMeter(soundLevelMeter);
			}
		});
		
		pDirtySoundLevelMeters = false;
	}
	
	// synchronize soundLevelMeters requiring to be synchronized
	decTLinkedList<deoalSoundLevelMeter>::Element * const tailSoundLevelMeter = pListSyncSoundLevelMeters.GetTail();
	while(pListSyncSoundLevelMeters.GetRoot()){
		decTLinkedList<deoalSoundLevelMeter>::Element * const entry = pListSyncSoundLevelMeters.GetRoot();
		deoalSoundLevelMeter &soundLevelMeter = *entry->GetOwner();
		pListSyncSoundLevelMeters.Remove(entry);
		
		soundLevelMeter.Synchronize(); // can potentially re-add the soundLevelMeter
		
		if(entry == tailSoundLevelMeter){
			break; // processed last soundLevelMeter. re-added soundLevelMeter will come next
		}
	}
}

void deoalWorld::pSyncAllMicLayerMask(){
	if(!pDirtyAllMicLayerMask){
		return;
	}
	
	// calculate layer mask with all bits from all microphones and sound level meters
	decLayerMask layerMask;
	
	pWorld.GetMicrophones().Visit([&](deMicrophone *engMicrophone){
		layerMask |= engMicrophone->GetLayerMask();
	});
	
	pWorld.GetSoundLevelMeters().Visit([&](deSoundLevelMeter *engSoundLevelMeter){
		layerMask |= engSoundLevelMeter->GetLayerMask();
	});
	
	// if the layer mask changed remove all components, clear the octree and mark
	// all components dirty for octree update
	if(layerMask == pAWorld->GetAllMicLayerMask()){
		pDirtyAllMicLayerMask = false;
		return;
	}
	
	pAWorld->GetOctree()->ClearComponents();
	
	pAWorld->SetAllMicLayerMask(layerMask);
	
	pWorld.GetComponents().Visit([&](deComponent *engComponent){
		((deoalComponent*)engComponent->GetPeerAudio())->LayerMaskChanged();
	});
	
	pDirtyAllMicLayerMask = false;
}

void deoalWorld::pSyncAudioParameters(){
	if(!pDirtyAudioParameters){
		return;
	}
	
	pDirtyAudioParameters = false;
	
	pAWorld->SetSpeakerGain(pWorld.GetSpeakerGain());
}
