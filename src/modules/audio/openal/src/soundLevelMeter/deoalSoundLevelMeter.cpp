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

#include "deoalSoundLevelMeter.h"
#include "deoalASoundLevelMeter.h"
#include "deoalASoundLevelMeterSpeaker.h"
#include "../deoalBasics.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATDebug.h"
#include "../speaker/deoalASpeaker.h"
#include "../speaker/deoalSpeaker.h"
#include "../world/deoalWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoalSoundLevelMeter
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoalSoundLevelMeter::deoalSoundLevelMeter(deAudioOpenAL &oal, deSoundLevelMeter &soundLevelMeter) :
pOal(oal),
pSoundLevelMeter(soundLevelMeter),
pASoundLevelMeter(new deoalASoundLevelMeter(oal.GetAudioThread())),

pParentWorld(NULL),

pDirtyGeometry(true),
pDirtyOctreeNode(true),
pDirtyLayerMask(true),
pDirtyEnabled(true),

pSpeakers(NULL),
pSpeakerCount(0),
pSpeakerSize(0),

pLLSyncWorld(this){
}

deoalSoundLevelMeter::~deoalSoundLevelMeter(){
	if(pASoundLevelMeter){
		pASoundLevelMeter->FreeReference();
	}
	
	if(pSpeakers){
		delete [] pSpeakers;
	}
}



// Management
///////////////

void deoalSoundLevelMeter::SetParentWorld(deoalWorld *world){
	if(world == pParentWorld){
		return;
	}
	
	pParentWorld = world;
	
	pDirtyOctreeNode = true;
}



const deSoundLevelMeter::cAudibleSpeaker &deoalSoundLevelMeter::GetSpeakerAt(int index) const{
	if(index < 0 || index >= pSpeakerCount){
		DETHROW(deeInvalidParam);
	}
	return pSpeakers[index];
}

void deoalSoundLevelMeter::AddSpeaker(deSpeaker *speaker, float volume){
	if(pSpeakerCount == pSpeakerSize){
		const int newSize = pSpeakerSize + 5;
		deSoundLevelMeter::cAudibleSpeaker * const newArray =
			new deSoundLevelMeter::cAudibleSpeaker[newSize];
		
		if(pSpeakers){
			int i;
			for(i=0; i<pSpeakerSize; i++){
				newArray[i] = pSpeakers[i];
			}
			delete [] pSpeakers;
		}
		
		pSpeakers = newArray;
		pSpeakerSize = newSize;
	}
	
	pSpeakers[pSpeakerCount].SetSpeaker(speaker);
	pSpeakers[pSpeakerCount].SetVolume(volume);
	pSpeakerCount++;
}

void deoalSoundLevelMeter::RemoveAllSpeakers(){
	pSpeakerCount = 0;
}



void deoalSoundLevelMeter::Synchronize(){
	// WARNING Called during synchronization time from main thread.
	
	pUpdateAudibleSpeakers();
	
	if(pDirtyGeometry){
		pASoundLevelMeter->SetType(pSoundLevelMeter.GetType());
		pASoundLevelMeter->SetGeometry(pSoundLevelMeter.GetPosition(),
			pSoundLevelMeter.GetOrientation());
		pASoundLevelMeter->SetConeAngle(pSoundLevelMeter.GetConeAngle());
		pASoundLevelMeter->SetAudibleDistance(pSoundLevelMeter.GetAudibleDistance());
		pDirtyGeometry = false;
	}
	
	if(pDirtyLayerMask){
		pASoundLevelMeter->SetLayerMask(pSoundLevelMeter.GetLayerMask());
		pDirtyLayerMask = false;
	}
	
	if(pDirtyEnabled){
		pASoundLevelMeter->SetEnabled(pSoundLevelMeter.GetEnabled());
		pDirtyEnabled = false;
	}
	
	if(pDirtyOctreeNode){
		pASoundLevelMeter->UpdateOctreeNode();
		pDirtyOctreeNode = false;
	}
	
	// force synchronization the next time
	// TODO is this reasonable to do this manual request syncing? sound level meters need to
	//      be always synchronized if enabled so this seems like an overkill here
	if(pParentWorld && pSoundLevelMeter.GetEnabled()){
		pParentWorld->AddSyncSoundLevelMeter(this);
	}
}



// Notifications
//////////////////

void deoalSoundLevelMeter::TypeChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSoundLevelMeter::PositionChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSoundLevelMeter::OrientationChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSoundLevelMeter::ConeAngleChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSoundLevelMeter::AudibleDistanceChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSoundLevelMeter::LayerMaskChanged(){
	pDirtyLayerMask = true;
	
	pRequiresSync();
	
	if(pParentWorld){
		pParentWorld->SetDirtyAllMicLayerMask();
	}
}

void deoalSoundLevelMeter::EnabledChanged(){
	pDirtyEnabled = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}



int deoalSoundLevelMeter::GetAudibleSpeakerCount(){
	return pSpeakerCount;
}

const deSoundLevelMeter::cAudibleSpeaker &deoalSoundLevelMeter::GetAudibleSpeakerAt(int index){
	if(index < 0 || index >= pSpeakerCount){
		DETHROW(deeInvalidParam);
	}
	return pSpeakers[index];
}



// Private Functions
//////////////////////

void deoalSoundLevelMeter::pRequiresSync(){
	if(! pLLSyncWorld.GetList() && pParentWorld){
		pParentWorld->AddSyncSoundLevelMeter(this);
	}
}

void deoalSoundLevelMeter::pUpdateAudibleSpeakers(){
	// store old speakers aside before updating list
	int i, count;
	
	pOldSpeakers.RemoveAll();
	for(i=0; i<pSpeakerCount; i++){
		pOldSpeakers.Add(pSpeakers[i].GetSpeaker());
	}
	
	// update the audible speaker list. it contains now all audible speakers
	RemoveAllSpeakers();
	
	count = pASoundLevelMeter->GetSpeakerCount();
	for(i=0; i<count; i++){
		const deoalASoundLevelMeterSpeaker &speaker = pASoundLevelMeter->GetSpeakerAt(i);
		if(speaker.GetVolume() < 1e-3f){
			continue;
		}
		
		deoalSpeaker * const backlink = speaker.GetSpeaker()->GetBackLink();
		if(! backlink){
			continue;
		}
		
		AddSpeaker(&backlink->GetSpeaker(), speaker.GetVolume());
	}
	
	// notify scripting module about leaving speakers
	count = pOldSpeakers.GetCount();
	for(i=0; i<count; i++){
		deSpeaker * const engSpeaker = (deSpeaker*)pOldSpeakers.GetAt(i);
		const int index = pIndexOfSpeaker(engSpeaker);
		if(index == -1){
			pSoundLevelMeter.NotifySpeakerInaudible(engSpeaker);
		}
	}
	
	// notify scripting module about entering speakers
	for(i=0; i<pSpeakerCount; i++){
		if(! pOldSpeakers.Has(pSpeakers[i].GetSpeaker())){
			pSoundLevelMeter.NotifySpeakerAudible(pSpeakers[i]);
		}
	}
}

int deoalSoundLevelMeter::pIndexOfSpeaker(deSpeaker *speaker) const{
	int i;
	for(i=0; i<pSpeakerCount; i++){
		if(pSpeakers[i].GetSpeaker() == speaker){
			return i;
		}
	}
	return -1;
}
