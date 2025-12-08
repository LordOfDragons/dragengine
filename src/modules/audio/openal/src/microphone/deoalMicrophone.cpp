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

#include "deoalMicrophone.h"
#include "deoalAMicrophone.h"
#include "../deoalBasics.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATDebug.h"
#include "../devmode/deoalDevMode.h"
#include "../speaker/deoalSpeaker.h"
#include "../speaker/deoalASpeaker.h"
#include "../world/deoalWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/debug/deDebugBlockInfo.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoalMicrophone
//////////////////////////

// Constructor, destructor
////////////////////////////

deoalMicrophone::deoalMicrophone(deAudioOpenAL &oal, const deMicrophone &microphone) :
pOal(oal),
pMicrophone(microphone),
pAMicrophone(new deoalAMicrophone(oal.GetAudioThread())),

pParentWorld(NULL),
pActive(false),

pDirtyMicrophone(true),
pDirtyGeometry(true),
pDirtyVelocity(true),
pDirtySpeakers(true),
pDirtyOctreeNode(true),
pDirtyLayerMask(true),

pLLSyncWorld(this)
{
	try{
		deSpeaker *speaker = microphone.GetRootSpeaker();
		while(speaker){
			SpeakerAdded(speaker);
			speaker = speaker->GetLLMicrophoneNext();
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoalMicrophone::~deoalMicrophone(){
	pCleanUp();
}



// Management
///////////////

void deoalMicrophone::SetActive(bool active){
	if(active == pActive){
		return;
	}
	
	pActive = active;
	
	if(pParentWorld && pOal.GetDevMode()->GetEnabled()){
		pParentWorld->DevModeChanged();
	}
}

void deoalMicrophone::SetParentWorld(deoalWorld *world){
	if(world == pParentWorld){
		return;
	}
	
	pParentWorld = world;
	
	pDirtyOctreeNode = true;
	
	if(pActive && world && pOal.GetDevMode()->GetEnabled()){
		world->DevModeChanged();
	}
}

void deoalMicrophone::Synchronize(){
	// WARNING Called during synchronization time from main thread.
	
	if(pDirtyMicrophone){
		pAMicrophone->SetVolume(pMicrophone.GetVolume());
		pAMicrophone->SetMuted(pMicrophone.GetMuted());
		pAMicrophone->SetSpeakerGain(pMicrophone.GetSpeakerGain());
		pAMicrophone->SetEnableAuralization(pMicrophone.GetEnableAuralization());
		pDirtyMicrophone = false;
	}
	
	if(pDirtyGeometry){
		pAMicrophone->SetGeometry(pMicrophone.GetPosition(), pMicrophone.GetOrientation());
		pDirtyGeometry = false;
	}
	
	if(pDirtyVelocity){
		pAMicrophone->SetVelocity(pMicrophone.GetVelocity());
		pDirtyVelocity = false;
	}
	
	if(pDirtyLayerMask){
		pAMicrophone->SetLayerMask(pMicrophone.GetLayerMask());
		pDirtyLayerMask = false;
	}
	
	if(pDirtyOctreeNode){
		pAMicrophone->UpdateOctreeNode();
		pDirtyOctreeNode = false;
	}
	
	pSyncSpeakers();
	
	// if this is the active microphone re-add it for synchronization the next frame.
	// using permanent re-synchronization is better for the active microphone case tracking
	// changes in the microphone, speakers and components all the time. this is the case
	// because only one microphone can be active in contrary to other situations where
	// multiple elements can change and need to be tracked
	if(pActive && pParentWorld){
		pParentWorld->AddSyncMicrophone(this);
	}
}



void deoalMicrophone::AddSyncSpeaker(deoalSpeaker *speaker){
	if(!speaker){
		DETHROW(deeInvalidParam);
	}
	if(speaker->GetLLSyncMic().GetList()){
		return;
	}
	
	pListSyncSpeakers.Add(&speaker->GetLLSyncMic());
	pRequiresSync();
}

void deoalMicrophone::RemoveSyncSpeaker(deoalSpeaker *speaker){
	if(!speaker){
		DETHROW(deeInvalidParam);
	}
	if(speaker->GetLLSyncMic().GetList()){
		pListSyncSpeakers.Remove(&speaker->GetLLSyncMic());
	}
}



// Notifications
//////////////////

void deoalMicrophone::TypeChanged(){
}

void deoalMicrophone::PositionChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalMicrophone::OrientationChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalMicrophone::VelocityChanged(){
	pDirtyVelocity = true;
	
	pRequiresSync();
}

void deoalMicrophone::MutedChanged(){
	pDirtyMicrophone = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalMicrophone::VolumeChanged(){
	pDirtyMicrophone = true;
	
	pRequiresSync();
}

void deoalMicrophone::LayerMaskChanged(){
	pDirtyLayerMask = true;
	
	pRequiresSync();
	
	if(pParentWorld){
		pParentWorld->SetDirtyAllMicLayerMask();
	}
}

void deoalMicrophone::SpeakerGainChanged(){
	pDirtyMicrophone = true;
	
	pRequiresSync();
}

void deoalMicrophone::EnableAuralizationChanged(){
	pDirtyMicrophone = true;
	
	pRequiresSync();
}



void deoalMicrophone::SpeakerAdded(deSpeaker *speaker){
	deoalSpeaker * const oalSpeaker = (deoalSpeaker*)speaker->GetPeerAudio();
	oalSpeaker->SetParentMicrophone(this);
	AddSyncSpeaker(oalSpeaker);
	
	pDirtySpeakers = true;
	
	pRequiresSync();
}

void deoalMicrophone::SpeakerRemoved(deSpeaker *speaker){
	deoalSpeaker * const oalSpeaker = (deoalSpeaker*)speaker->GetPeerAudio();
	RemoveSyncSpeaker(oalSpeaker);
	oalSpeaker->GetASpeaker()->SetMicrophoneMarkedRemove(true);
	oalSpeaker->SetParentMicrophone(NULL);
	
	pDirtySpeakers = true;
	
	pRequiresSync();
}

void deoalMicrophone::AllSpeakersRemoved(){
	deSpeaker *speaker = pMicrophone.GetRootSpeaker();
	while(speaker){
		deoalSpeaker * const oalSpeaker = (deoalSpeaker*)speaker->GetPeerAudio();
		RemoveSyncSpeaker(oalSpeaker);
		oalSpeaker->GetASpeaker()->SetMicrophoneMarkedRemove(true);
		oalSpeaker->SetParentMicrophone(NULL);
		speaker = speaker->GetLLMicrophoneNext();
	}
	
	pDirtySpeakers = true;
	
	pRequiresSync();
}



// Private Functions
//////////////////////

void deoalMicrophone::pCleanUp(){
	if(pActive){
		pOal.SetActiveMicrophone(NULL);
	}
	
	if(pAMicrophone){
		pAMicrophone = NULL;
	}
	
	AllSpeakersRemoved();
}



void deoalMicrophone::pSyncSpeakers(){
	// WARNING Called during synchronization time from main thread.
	
	// add new speakers
	if(pDirtySpeakers){
		pAMicrophone->RemoveRemovalMarkedSpeakers();
		
		deSpeaker *engSpeaker = pMicrophone.GetRootSpeaker();
		while(engSpeaker){
			deoalASpeaker * const speaker =
				((deoalSpeaker*)engSpeaker->GetPeerAudio())->GetASpeaker();
			if(speaker->GetParentMicrophone() != pAMicrophone){
				pAMicrophone->AddSpeaker(speaker);
			}
			engSpeaker = engSpeaker->GetLLMicrophoneNext();
		}
		
		pDirtySpeakers = false;
	}
	
	// find active speakers. this can enable/disable speakers which speaker
	// synchronization relies upon
	pAMicrophone->FindActiveSpeakers();
	
	// synchronize speakers requiring to be synchronized. this has to be done in a way speakers
	// can add themselves to synchronize again for the next frame from inside Synchronize().
	decPointerLinkedList::cListEntry * const tailSpeaker = pListSyncSpeakers.GetTail();
	while(pListSyncSpeakers.GetRoot()){
		decPointerLinkedList::cListEntry * const entry = pListSyncSpeakers.GetRoot();
		deoalSpeaker &speaker = *((deoalSpeaker*)entry->GetOwner());
		pListSyncSpeakers.Remove(entry);
		
		speaker.Synchronize(); // can potentially re-add the speaker
		
		if(entry == tailSpeaker){
			break; // processed last speaker. re-added speaker will come next
		}
	}
}



void deoalMicrophone::pRequiresSync(){
	if(!pLLSyncWorld.GetList() && pParentWorld){
		pParentWorld->AddSyncMicrophone(this);
	}
}
