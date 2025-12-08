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

pRootComponent(nullptr),
pTailComponent(nullptr),
pComponentCount(0),

pRootSpeaker(nullptr),
pTailSpeaker(nullptr),
pSpeakerCount(0),

pRootMicrophone(nullptr),
pTailMicrophone(nullptr),
pMicrophoneCount(0),

pRootSoundLevelMeter(nullptr),
pTailSoundLevelMeter(nullptr),
pSoundLevelMeterCount(0),

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
	
	deoalASpeaker *speaker = pRootSpeaker;
	while(speaker){
		speaker->SetEnabled(enabled);
		speaker = speaker->GetLLWorldNext();
	}
}

void deoalAWorld::UpdateAllSpeakers(){
	deoalASpeaker *speaker = pRootSpeaker;
	while(speaker){
		speaker->PrepareProcessAudio();
		speaker = speaker->GetLLWorldNext();
	}
}

void deoalAWorld::UpdateSoundLevelMetering(){
	deoalASoundLevelMeter *soundLevelMeter = pRootSoundLevelMeter;
	while(soundLevelMeter){
		soundLevelMeter->FindSpeakers();
		soundLevelMeter->MeterSpeakers();
		soundLevelMeter = soundLevelMeter->GetLLWorldNext();
	}
}

void deoalAWorld::PrepareProcessAudio(){
	UpdateAllSpeakers();
	UpdateSoundLevelMetering();
}

void deoalAWorld::ProcessDeactivate(){
	deoalASpeaker *speaker = pRootSpeaker;
	while(speaker){
		speaker->ProcessDeactivate();
		speaker = speaker->GetLLWorldNext();
	}
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
	
	if(pTailComponent){
		pTailComponent->SetLLWorldNext(component);
		component->SetLLWorldPrev(pTailComponent);
		component->SetLLWorldNext(nullptr);
		pTailComponent = component;
		
	}else{
		pTailComponent = component;
		pRootComponent = component;
		component->SetLLWorldPrev(nullptr);
		component->SetLLWorldNext(nullptr);
	}
	
	component->AddReference();
	pComponentCount++;
	
	component->SetParentWorld(this);
}

void deoalAWorld::RemoveComponent(deoalAComponent *component){
	if(component->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	component->SetParentWorld(nullptr);
	component->SetWorldMarkedRemove(false);
	
	if(component->GetLLWorldPrev()){
		component->GetLLWorldPrev()->SetLLWorldNext(component->GetLLWorldNext());
		
	}else{
		pRootComponent = component->GetLLWorldNext();
	}
	
	if(component->GetLLWorldNext()){
		component->GetLLWorldNext()->SetLLWorldPrev(component->GetLLWorldPrev());
		
	}else{
		pTailComponent = component->GetLLWorldPrev();
	}
	
	pComponentCount--;
	component->FreeReference();
}

void deoalAWorld::RemoveAllComponents(){
	while(pRootComponent){
		deoalAComponent * const next = pRootComponent->GetLLWorldNext();
		pRootComponent->SetLLWorldPrev(nullptr); // ensure root has no prev
		
		pRootComponent->SetParentWorld(nullptr);
		pRootComponent->SetWorldMarkedRemove(false);
		pComponentCount--;
		
		pRootComponent = next;
	}
	
	pTailComponent = NULL;
}

void deoalAWorld::RemoveRemovalMarkedComponents(){
	deoalAComponent *component = pRootComponent;
	while(component){
		deoalAComponent * const next = component->GetLLWorldNext();
		
		if(component->GetWorldMarkedRemove()){
			RemoveComponent(component);
		}
		
		component = next;
	}
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
	
	if(pTailSpeaker){
		pTailSpeaker->SetLLWorldNext(speaker);
		speaker->SetLLWorldPrev(pTailSpeaker);
		speaker->SetLLWorldNext(nullptr);
		pTailSpeaker = speaker;
		
	}else{
		pTailSpeaker = speaker;
		pRootSpeaker = speaker;
		speaker->SetLLWorldPrev(nullptr);
		speaker->SetLLWorldNext(nullptr);
	}
	
	speaker->AddReference();
	pSpeakerCount++;
	
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
	
	if(speaker->GetLLWorldPrev()){
		speaker->GetLLWorldPrev()->SetLLWorldNext(speaker->GetLLWorldNext());
		
	}else{
		pRootSpeaker = speaker->GetLLWorldNext();
	}
	
	if(speaker->GetLLWorldNext()){
		speaker->GetLLWorldNext()->SetLLWorldPrev(speaker->GetLLWorldPrev());
		
	}else{
		pTailSpeaker = speaker->GetLLWorldPrev();
	}
	
	pSpeakerCount--;
	speaker->FreeReference();
}

void deoalAWorld::RemoveAllSpeakers(){
	// WARNING Called during synchronization time from main thread.
	
	while(pRootSpeaker){
		deoalASpeaker * const next = pRootSpeaker->GetLLWorldNext();
		pRootSpeaker->SetLLWorldPrev(nullptr); // ensure root has no prev
		
		if(pAudioThread.GetActiveMicrophone()){
			pAudioThread.GetActiveMicrophone()->InvalidateSpeaker(pRootSpeaker);
		}
		
		pRootSpeaker->SetParentWorld(nullptr);
		pRootSpeaker->SetWorldMarkedRemove(false);
		pRootSpeaker->SetEnabled(false);
		
		pSpeakerCount--;
		
		pRootSpeaker = next;
	}
	
	pTailSpeaker = NULL;
}

void deoalAWorld::RemoveRemovalMarkedSpeakers(){
	// WARNING Called during synchronization time from main thread.
	
	deoalASpeaker *speaker = pRootSpeaker;
	while(speaker){
		deoalASpeaker * const next = speaker->GetLLWorldNext();
		
		if(speaker->GetWorldMarkedRemove()){
			RemoveSpeaker(speaker);
		}
		
		speaker = next;
	}
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
	
	if(pTailMicrophone){
		pTailMicrophone->SetLLWorldNext(microphone);
		microphone->SetLLWorldPrev(pTailMicrophone);
		microphone->SetLLWorldNext(nullptr);
		pTailMicrophone = microphone;
		
	}else{
		pTailMicrophone = microphone;
		pRootMicrophone = microphone;
		microphone->SetLLWorldPrev(nullptr);
		microphone->SetLLWorldNext(nullptr);
	}
	
	microphone->AddReference();
	pMicrophoneCount++;
	
	microphone->SetParentWorld(this);
}

void deoalAWorld::RemoveMicrophone(deoalAMicrophone *microphone){
	if(microphone->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	microphone->SetParentWorld(nullptr);
	microphone->SetWorldMarkedRemove(false);
	
	if(microphone->GetLLWorldPrev()){
		microphone->GetLLWorldPrev()->SetLLWorldNext(microphone->GetLLWorldNext());
		
	}else{
		pRootMicrophone = microphone->GetLLWorldNext();
	}
	
	if(microphone->GetLLWorldNext()){
		microphone->GetLLWorldNext()->SetLLWorldPrev(microphone->GetLLWorldPrev());
		
	}else{
		pTailMicrophone = microphone->GetLLWorldPrev();
	}
	
	pMicrophoneCount--;
	microphone->FreeReference();
}

void deoalAWorld::RemoveAllMicrophones(){
	while(pRootMicrophone){
		deoalAMicrophone * const next = pRootMicrophone->GetLLWorldNext();
		pRootMicrophone->SetLLWorldPrev(nullptr); // ensure root has no prev
		
		pRootMicrophone->SetParentWorld(nullptr);
		pRootMicrophone->SetWorldMarkedRemove(false);
		pMicrophoneCount--;
		
		pRootMicrophone = next;
	}
	
	pTailMicrophone = NULL;
}

void deoalAWorld::RemoveRemovalMarkedMicrophones(){
	deoalAMicrophone *microphone = pRootMicrophone;
	while(microphone){
		deoalAMicrophone * const next = microphone->GetLLWorldNext();
		
		if(microphone->GetWorldMarkedRemove()){
			RemoveMicrophone(microphone);
		}
		
		microphone = next;
	}
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
	
	if(pTailSoundLevelMeter){
		pTailSoundLevelMeter->SetLLWorldNext(soundLevelMeter);
		soundLevelMeter->SetLLWorldPrev(pTailSoundLevelMeter);
		soundLevelMeter->SetLLWorldNext(nullptr);
		pTailSoundLevelMeter = soundLevelMeter;
		
	}else{
		pTailSoundLevelMeter = soundLevelMeter;
		pRootSoundLevelMeter = soundLevelMeter;
		soundLevelMeter->SetLLWorldPrev(nullptr);
		soundLevelMeter->SetLLWorldNext(nullptr);
	}
	
	soundLevelMeter->AddReference();
	pSoundLevelMeterCount++;
	
	soundLevelMeter->SetParentWorld(this);
}

void deoalAWorld::RemoveSoundLevelMeter(deoalASoundLevelMeter *soundLevelMeter){
	// WARNING Called during synchronization time from main thread.
	
	if(soundLevelMeter->GetParentWorld() != this){
		DETHROW(deeInvalidParam);
	}
	
	soundLevelMeter->SetParentWorld(nullptr);
	soundLevelMeter->SetWorldMarkedRemove(false);
	
	if(soundLevelMeter->GetLLWorldPrev()){
		soundLevelMeter->GetLLWorldPrev()->SetLLWorldNext(soundLevelMeter->GetLLWorldNext());
		
	}else{
		pRootSoundLevelMeter = soundLevelMeter->GetLLWorldNext();
	}
	
	if(soundLevelMeter->GetLLWorldNext()){
		soundLevelMeter->GetLLWorldNext()->SetLLWorldPrev(soundLevelMeter->GetLLWorldPrev());
		
	}else{
		pTailSoundLevelMeter = soundLevelMeter->GetLLWorldPrev();
	}
	
	pSoundLevelMeterCount--;
	soundLevelMeter->FreeReference();
}

void deoalAWorld::RemoveAllSoundLevelMeters(){
	// WARNING Called during synchronization time from main thread.
	
	while(pRootSoundLevelMeter){
		deoalASoundLevelMeter * const next = pRootSoundLevelMeter->GetLLWorldNext();
		pRootSoundLevelMeter->SetLLWorldPrev(nullptr); // ensure root has no prev
		
		pRootSoundLevelMeter->SetParentWorld(nullptr);
		pRootSoundLevelMeter->SetWorldMarkedRemove(false);
		
		pSoundLevelMeterCount--;
		
		pRootSoundLevelMeter = next;
	}
	
	pTailSoundLevelMeter = NULL;
}

void deoalAWorld::RemoveRemovalMarkedSoundLevelMeters(){
	// WARNING Called during synchronization time from main thread.
	
	deoalASoundLevelMeter *soundLevelMeter = pRootSoundLevelMeter;
	while(soundLevelMeter){
		deoalASoundLevelMeter * const next = soundLevelMeter->GetLLWorldNext();
		
		if(soundLevelMeter->GetWorldMarkedRemove()){
			RemoveSoundLevelMeter(soundLevelMeter);
		}
		
		soundLevelMeter = next;
	}
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
	while(pRootComponent){
		deoalAComponent * const next = pRootComponent->GetLLWorldNext();
		pRootComponent->PrepareQuickDispose();
		pRootComponent = next;
	}
	
	while(pRootSpeaker){
		deoalASpeaker * const next = pRootSpeaker->GetLLWorldNext();
		pRootSpeaker->PrepareQuickDispose();
		pRootSpeaker = next;
	}
	
	while(pRootMicrophone){
		deoalAMicrophone * const next = pRootMicrophone->GetLLWorldNext();
		pRootMicrophone->PrepareQuickDispose();
		pRootMicrophone = next;
	}
	
	if(pOctree){
		delete pOctree;
	}
}
