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

#include "deoalAWorld.h"
#include "octree/deoalWorldOctree.h"
#include "octree/deoalWorldOctreeVisitor.h"
#include "octree/deoalWOVInvalidateEnvProbes.h"
#include "../audiothread/deoalAudioThread.h"
#include "../component/deoalAComponent.h"
#include "../environment/deoalEnvProbeList.h"
#include "../microphone/deoalAMicrophone.h"
#include "../speaker/deoalASpeaker.h"
#include "../soundLevelMeter/deoalASoundLevelMeter.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deMicrophone.h>



// Class deoalAWorld
//////////////////////

// Constructor, destructor
////////////////////////////

deoalAWorld::deoalAWorld(deoalAudioThread &audioThread, const decDVector &size) :
pAudioThread(audioThread),
pOctree(nullptr),
pSpeakerGain(1.0f)
{
	try{
		pOctree = new deoalWorldOctree(decDVector(), size * 0.5);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	LEAK_CHECK_CREATE(audioThread, World);
}

deoalAWorld::~deoalAWorld(){
	LEAK_CHECK_FREE(pAudioThread, World);
	
	pCleanUp();
}



// Management
///////////////

void deoalAWorld::VisitRegion(const decDVector &center, const decDVector &halfSize,
deoalWorldOctreeVisitor &visitor){
	pOctree->VisitNodesColliding(&visitor, center - halfSize, center + halfSize);
}

void deoalAWorld::InvalidateEnvProbes(const decDVector &minExtend,
const decDVector &maxExtend, const decLayerMask &layerMask){
	deoalWOVInvalidateEnvProbes visitor(minExtend, maxExtend, layerMask);
	VisitRegion(minExtend, maxExtend, visitor);
}



bool deoalAWorld::IsAudible() const{
	return pAudioThread.GetActiveWorld() == this;
}

void deoalAWorld::SetAllSpeakersEnabled(bool enabled){
	// WARNING Called during synchronization time from main thread.
	
	pSpeakers.Visit([&](deoalASpeaker *speaker){
		speaker->SetEnabled(enabled);
	});
}

void deoalAWorld::UpdateAllSpeakers(){
	pSpeakers.Visit([](deoalASpeaker *speaker){
		speaker->PrepareProcessAudio();
	});
}

void deoalAWorld::UpdateSoundLevelMetering(){
	pSoundLevelMeters.Visit([](deoalASoundLevelMeter *soundLevelMeter){
		soundLevelMeter->FindSpeakers();
		soundLevelMeter->MeterSpeakers();
	});
}

void deoalAWorld::PrepareProcessAudio(){
	UpdateAllSpeakers();
	UpdateSoundLevelMetering();
}

void deoalAWorld::ProcessDeactivate(){
	pSpeakers.Visit([](deoalASpeaker *speaker){
		speaker->ProcessDeactivate();
	});
}

void deoalAWorld::SetAllMicLayerMask(const decLayerMask &layerMask){
	pAllMicLayerMask = layerMask;
}

void deoalAWorld::SetSpeakerGain(float gain){
	pSpeakerGain = gain;
}



// Components
///////////////

void deoalAWorld::AddComponent(deoalAComponent *component){
	if(!component){
		DETHROW(deeInvalidParam);
	}
	
	if(component->GetParentWorld()){
		if(component->GetWorldMarkedRemove()){
			component->GetParentWorld()->RemoveComponent(component);
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
	
	pComponents.Add(&component->GetLLWorld());
	component->SetParentWorld(this);
}

void deoalAWorld::RemoveComponent(deoalAComponent *component){
	if(component->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	component->SetParentWorld(nullptr);
	component->SetWorldMarkedRemove(false);
	pComponents.Remove(&component->GetLLWorld());
}

void deoalAWorld::RemoveAllComponents(){
	pComponents.Visit([](deoalAComponent *component){
		component->SetParentWorld(nullptr);
		component->SetWorldMarkedRemove(false);
	});
	pComponents.RemoveAll();
}

void deoalAWorld::RemoveRemovalMarkedComponents(){
	pComponents.RemoveIf([&](deoalAComponent *component){
		if(component->GetWorldMarkedRemove()){
			component->SetParentWorld(nullptr);
			component->SetWorldMarkedRemove(false);
			return true;
		}
		return false;
	});
}



// Speakers
///////////////

void deoalAWorld::AddSpeaker(deoalASpeaker *speaker){
	// WARNING Called during synchronization time from main thread.
	
	if(!speaker){
		DETHROW(deeInvalidParam);
	}
	
	if(speaker->GetParentWorld()){
		if(speaker->GetWorldMarkedRemove()){
			speaker->GetParentWorld()->RemoveSpeaker(speaker);
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
	
	pSpeakers.Add(&speaker->GetLLWorld());
	speaker->SetParentWorld(this);
	speaker->SetPositionless(false); // TODO using option
	speaker->SetEnabled(false); // ensure it is disabled. active microphone will enable
}

void deoalAWorld::RemoveSpeaker(deoalASpeaker *speaker){
	// WARNING Called during synchronization time from main thread.
	
	if(speaker->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(pAudioThread.GetActiveMicrophone()){
		pAudioThread.GetActiveMicrophone()->InvalidateSpeaker(speaker);
	}
	
	speaker->SetParentWorld(nullptr);
	speaker->SetEnabled(false);
	speaker->SetPositionless(true);
	speaker->SetWorldMarkedRemove(false);
	pSpeakers.Remove(&speaker->GetLLWorld());
}

void deoalAWorld::RemoveAllSpeakers(){
	// WARNING Called during synchronization time from main thread.
	
	pSpeakers.Visit([&](deoalASpeaker *speaker){
		if(pAudioThread.GetActiveMicrophone()){
			pAudioThread.GetActiveMicrophone()->InvalidateSpeaker(speaker);
		}
		
		speaker->SetParentWorld(nullptr);
		speaker->SetWorldMarkedRemove(false);
		speaker->SetEnabled(false);
	});
	pSpeakers.RemoveAll();
}

void deoalAWorld::RemoveRemovalMarkedSpeakers(){
	// WARNING Called during synchronization time from main thread.
	
	pSpeakers.RemoveIf([&](deoalASpeaker *speaker){
		if(speaker->GetWorldMarkedRemove()){
			speaker->SetParentWorld(nullptr);
			speaker->SetWorldMarkedRemove(false);
			speaker->SetEnabled(false);
			return true;
		}
		return false;
	});
}



// Microphones
///////////////

void deoalAWorld::AddMicrophone(deoalAMicrophone *microphone){
	if(!microphone){
		DETHROW(deeInvalidParam);
	}
	
	if(microphone->GetParentWorld()){
		if(microphone->GetWorldMarkedRemove()){
			microphone->GetParentWorld()->RemoveMicrophone(microphone);
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
	
	pMicrophones.Add(&microphone->GetLLWorld());
	microphone->SetParentWorld(this);
}

void deoalAWorld::RemoveMicrophone(deoalAMicrophone *microphone){
	if(microphone->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	microphone->SetParentWorld(nullptr);
	microphone->SetWorldMarkedRemove(false);
	pMicrophones.Remove(&microphone->GetLLWorld());
}

void deoalAWorld::RemoveAllMicrophones(){
	pMicrophones.Visit([](deoalAMicrophone *microphone){
		microphone->SetParentWorld(nullptr);
		microphone->SetWorldMarkedRemove(false);
	});
	pMicrophones.RemoveAll();
}

void deoalAWorld::RemoveRemovalMarkedMicrophones(){
	pMicrophones.RemoveIf([&](deoalAMicrophone *microphone){
		if(microphone->GetWorldMarkedRemove()){
			microphone->SetParentWorld(nullptr);
			microphone->SetWorldMarkedRemove(false);
			return true;
		}
		return false;
	});
}



// SoundLevelMeters
/////////////////////

void deoalAWorld::AddSoundLevelMeter(deoalASoundLevelMeter *soundLevelMeter){
	// WARNING Called during synchronization time from main thread.
	
	if(!soundLevelMeter){
		DETHROW(deeInvalidParam);
	}
	
	if(soundLevelMeter->GetParentWorld()){
		if(soundLevelMeter->GetWorldMarkedRemove()){
			soundLevelMeter->GetParentWorld()->RemoveSoundLevelMeter(soundLevelMeter);
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
	
	pSoundLevelMeters.Add(&soundLevelMeter->GetLLWorld());
	soundLevelMeter->SetParentWorld(this);
}

void deoalAWorld::RemoveSoundLevelMeter(deoalASoundLevelMeter *soundLevelMeter){
	// WARNING Called during synchronization time from main thread.
	
	if(soundLevelMeter->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	soundLevelMeter->SetParentWorld(nullptr);
	soundLevelMeter->SetWorldMarkedRemove(false);
	pSoundLevelMeters.Remove(&soundLevelMeter->GetLLWorld());
}

void deoalAWorld::RemoveAllSoundLevelMeters(){
	// WARNING Called during synchronization time from main thread.
	
	pSoundLevelMeters.Visit([](deoalASoundLevelMeter *soundLevelMeter){
		soundLevelMeter->SetParentWorld(nullptr);
		soundLevelMeter->SetWorldMarkedRemove(false);
	});
	pSoundLevelMeters.RemoveAll();
}

void deoalAWorld::RemoveRemovalMarkedSoundLevelMeters(){
	// WARNING Called during synchronization time from main thread.
	
	pSoundLevelMeters.RemoveIf([&](deoalASoundLevelMeter *soundLevelMeter){
		if(soundLevelMeter->GetWorldMarkedRemove()){
			soundLevelMeter->SetParentWorld(nullptr);
			soundLevelMeter->SetWorldMarkedRemove(false);
			return true;
		}
		return false;
	});
}



// Private Functions
//////////////////////

void deoalAWorld::pCleanUp(){
	// dispose of elements. this is done using a PrepareQuickDispose call which modifies some
	// internal parameters to make clean-up calls perform much faster compared to calling
	// RemoveAll* functions. this cuts down processing time for cleaning up world objects
	// 
	// this also means ClearAll() is not called on the octree since PrepareQuickDispose()
	// is called on the octree content
	pComponents.Visit([](deoalAComponent *component){
		component->PrepareQuickDispose();
	});
	
	pSpeakers.Visit([](deoalASpeaker *speaker){
		speaker->PrepareQuickDispose();
	});
	
	pMicrophones.Visit([](deoalAMicrophone *microphone){
		microphone->PrepareQuickDispose();
	});
	
	if(pOctree){
		delete pOctree;
	}
}
