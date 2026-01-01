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

#include "deoalASoundLevelMeter.h"
#include "deoalASoundLevelMeterSpeaker.h"
#include "deoalSoundLevelMeter.h"
#include "deoalWOVSLMFindSpeakers.h"
#include "../deoalBasics.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../environment/deoalEnvProbe.h"
#include "../environment/deoalEnvProbeList.h"
#include "../speaker/deoalASpeaker.h"
#include "../speaker/deoalSpeaker.h"
#include "../world/deoalAWorld.h"
#include "../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalASoundLevelMeter
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalASoundLevelMeter::deoalASoundLevelMeter(deoalAudioThread &audioThread) :
pAudioThread(audioThread),
pType(deSoundLevelMeter::emtPoint),
pConeAngle(45.0f),
pAudibleDistance(1.0f),
pEnabled(false),
pParentWorld(NULL),
pOctreeNode(NULL),
pEnvProbeList(NULL),
pEnvProbe(NULL),
pDirtyEnvProbe(true),

pWorldMarkedRemove(false),
pLLWorldPrev(NULL),
pLLWorldNext(NULL)
{
	pWOVPrepareRayTrace.SetRadius(pAudibleDistance);
	LEAK_CHECK_CREATE(audioThread, SoundLevelMeter);
}

deoalASoundLevelMeter::~deoalASoundLevelMeter(){
	LEAK_CHECK_FREE(pAudioThread, SoundLevelMeter);
	
	pCleanUp();
}



// Management
///////////////

void deoalASoundLevelMeter::SetType(deSoundLevelMeter::eMeterTypes type){
	pType = type;
}

void deoalASoundLevelMeter::SetGeometry(const decDVector &position, const decQuaternion &orientation){
	pPosition = position;
	pOrientation = orientation;
	pWOVPrepareRayTrace.SetCenter(position);
	
	pDirtyEnvProbe = true;
}

void deoalASoundLevelMeter::SetConeAngle(float angle){
	pConeAngle = angle;
	
	pDirtyEnvProbe = true;
}

void deoalASoundLevelMeter::SetAudibleDistance(float audibleDistance){
	pAudibleDistance = audibleDistance;
	pWOVPrepareRayTrace.SetRadius(pAudibleDistance);
	
	if(pEnvProbeList){
		pEnvProbeList->SetRange(audibleDistance);
	}
	pDirtyEnvProbe = true;
}

void deoalASoundLevelMeter::SetLayerMask(const decLayerMask &layerMask){
	pLayerMask = layerMask;
	pWOVPrepareRayTrace.SetLayerMask(layerMask);
	
	if(pEnvProbeList){
		pEnvProbeList->SetLayerMask(layerMask);
	}
	pDirtyEnvProbe = true;
}

void deoalASoundLevelMeter::SetEnabled(bool enabled){
	pEnabled = enabled;
}



int deoalASoundLevelMeter::GetSpeakerCount() const{
	return pSpeakers.GetCount();
}

const deoalASoundLevelMeterSpeaker &deoalASoundLevelMeter::GetSpeakerAt(int index) const{
	return *((deoalASoundLevelMeterSpeaker*)pSpeakers.GetAt(index));
}

deoalASoundLevelMeterSpeaker *deoalASoundLevelMeter::GetSpeakerWith(deoalASpeaker *speaker) const{
	const int count = pSpeakers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoalASoundLevelMeterSpeaker * const slmspeaker =
			(deoalASoundLevelMeterSpeaker*)pSpeakers.GetAt(i);
		if(slmspeaker->GetSpeaker() == speaker){
			return slmspeaker;
		}
	}
	
	return NULL;
}

void deoalASoundLevelMeter::RemoveSpeakerIfPresent(deoalASpeaker *speaker){
	const int index = pIndexOfSpeaker(speaker);
	if(index != -1){
		pRemoveSpeakerFrom(index);
	}
}

void deoalASoundLevelMeter::SpeakerDropEnvProbeOctreeNode(deoalASpeaker *speaker){
	deoalASoundLevelMeterSpeaker * const slmspeaker = GetSpeakerWith(speaker);
	if(slmspeaker){
		slmspeaker->EnvProbeDropOctreeNode();
	}
}



void deoalASoundLevelMeter::SetParentWorld(deoalAWorld *world){
	// WARNING Called during synchronization time from main thread.
	
	if(world == pParentWorld){
		return;
	}
	
	pRemoveAllSpeakers();
	
	if(pEnvProbeList){
		delete pEnvProbeList;
		pEnvProbeList = NULL;
	}
	
	if(pOctreeNode){
		pOctreeNode->RemoveSoundLevelMeter(this);
	}
	
	pParentWorld = world;
}

void deoalASoundLevelMeter::SetOctreeNode(deoalWorldOctree *node){
	pOctreeNode = node;
}

void deoalASoundLevelMeter::UpdateOctreeNode(){
	if(pParentWorld && pEnabled){
		pParentWorld->GetOctree()->InsertSoundLevelMeterIntoTree(this, 8);
		
	}else if(pOctreeNode){
		pOctreeNode->RemoveSoundLevelMeter(this);
	}
}

void deoalASoundLevelMeter::PrepareQuickDispose(){
	if(pEnvProbeList){
		pEnvProbeList->PrepareQuickDispose();
		delete pEnvProbeList;
		pEnvProbeList = NULL;
	}
	
	pSpeakers.RemoveAll();
	pParentWorld = NULL;
	pOctreeNode = NULL;
}



deoalEnvProbe *deoalASoundLevelMeter::GetEnvProbe(){
	if(!pDirtyEnvProbe){
		return pEnvProbe;
	}
	
	pDirtyEnvProbe = false;
	pEnvProbe = NULL;
	
	deoalAWorld * const world = GetParentWorld();
	if(!world){
		return NULL;
	}
	
	if(!pEnvProbeList){
		const float reuseDistance = 1.0;
		const int maxProbeCount = 20;
		
		pEnvProbeList = new deoalEnvProbeList(*pParentWorld, reuseDistance, maxProbeCount);
		pEnvProbeList->SetRange(pAudibleDistance);
		pEnvProbeList->SetLayerMask(pLayerMask);
		pEnvProbeList->SetRTConfig(&pAudioThread.GetRayTracing().GetConfigSoundTracingMetering());
	}
	
	pWOVPrepareRayTrace.Visit(*pParentWorld);
	
	try{
		pEnvProbe = pEnvProbeList->GetProbeTraceSoundRays(pPosition);
		
	}catch(const deException &e){
		pAudioThread.GetLogger().LogException(e);
		// exceptions can happen during cancelling. using NULL is fine
	}
	
	return pEnvProbe;
}



void deoalASoundLevelMeter::FindSpeakers(){
	deoalSpeakerList &list = pAudioThread.GetSpeakerList();
	list.RemoveAll();
	if(pParentWorld){
		deoalWOVSLMFindSpeakers(*this, list).Visit(*pParentWorld);
	}
	
	pFlagAllSpeakers(false);
	list.FlagAll(true);
	
	int i, count = pSpeakers.GetCount();
	for(i=0; i<count; i++){
		deoalASoundLevelMeterSpeaker &speaker =
			*( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) );
		if(!speaker.GetSpeaker()->GetFlag()){
// 			pAudioThread.GetLogger().LogInfoFormat( "FindSpeakers: Remove %p", speaker.GetSpeaker() );
			pRemoveSpeakerFrom(i);
			i--;
			count--;
		}
	}
	
	pFlagAllSpeakers(false);
	count = list.GetCount();
	for(i=0; i<count; i++){
		deoalASpeaker * const speaker = list.GetAt(i);
		if(speaker->GetFlag()){
// 			pAudioThread.GetLogger().LogInfoFormat( "FindSpeakers: Add %p", speaker );
			pAddSpeaker(speaker);
		}
	}
}

void deoalASoundLevelMeter::MeterSpeakers(){
	int i, count = pSpeakers.GetCount();
	for(i=0; i<count; i++){
// 			const bool debugPrint = ( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetVolume() > 1e-3f;
		((deoalASoundLevelMeterSpeaker*)pSpeakers.GetAt(i))->Listen();
// 			if( debugPrint ){
// 				pAudioThread.GetLogger().LogInfoFormat( "Speaker %p(%f,%f,%f) Volume %f",
// 					( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetSpeaker(),
// 					( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetSpeaker()->GetPosition().x,
// 					( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetSpeaker()->GetPosition().y,
// 					( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetSpeaker()->GetPosition().z,
// 					( ( deoalASoundLevelMeterSpeaker* )pSpeakers.GetAt( i ) )->GetVolume() );
// 			}
	}
}



// World usage
////////////////

void deoalASoundLevelMeter::SetWorldMarkedRemove(bool marked){
	pWorldMarkedRemove = marked;
}

void deoalASoundLevelMeter::SetLLWorldPrev(deoalASoundLevelMeter *soundLevelMeter){
	pLLWorldPrev = soundLevelMeter;
}

void deoalASoundLevelMeter::SetLLWorldNext(deoalASoundLevelMeter *soundLevelMeter){
	pLLWorldNext = soundLevelMeter;
}



// Private Functions
//////////////////////

void deoalASoundLevelMeter::pCleanUp(){
	pEnvProbe = NULL;
	if(pEnvProbeList){
		delete pEnvProbeList;
	}
	
	pRemoveAllSpeakers();
}

void deoalASoundLevelMeter::pAddSpeaker(deoalASpeaker *speaker){
	pSpeakers.Add(deoalASoundLevelMeterSpeaker::Ref::New(*this, speaker));
	
	speaker->GetSoundLevelMeters().AddIfAbsent(this);
}

int deoalASoundLevelMeter::pIndexOfSpeaker(deoalASpeaker *speaker) const{
	const int count = pSpeakers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((deoalASoundLevelMeterSpeaker*)pSpeakers.GetAt(i))->GetSpeaker() == speaker){
			return i;
		}
	}
	
	return -1;
}

void deoalASoundLevelMeter::pRemoveSpeakerFrom(int index){
	((deoalASoundLevelMeterSpeaker*)pSpeakers.GetAt(index))
		->GetSpeaker()->GetSoundLevelMeters().RemoveIfPresent(this);
	
	const int last = pSpeakers.GetCount() - 1;
	if(index < last){
		pSpeakers.SetAt(index, pSpeakers.GetAt(last));
	}
	
	pSpeakers.RemoveFrom(last);
}

void deoalASoundLevelMeter::pRemoveAllSpeakers(){
	const int count = pSpeakers.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoalASoundLevelMeterSpeaker*)pSpeakers.GetAt(i))
			->GetSpeaker()->GetSoundLevelMeters().RemoveIfPresent(this);
	}
	pSpeakers.RemoveAll();
}

void deoalASoundLevelMeter::pFlagAllSpeakers(bool flag) const{
	const int count = pSpeakers.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoalASoundLevelMeterSpeaker*)pSpeakers.GetAt(i))->GetSpeaker()->SetFlag(flag);
	}
}

void deoalASoundLevelMeter::pDropEnvProbeOctreeNodeAllSpeakers(){
	const int count = pSpeakers.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoalASoundLevelMeterSpeaker*)pSpeakers.GetAt(i))->EnvProbeDropOctreeNode();
	}
}
