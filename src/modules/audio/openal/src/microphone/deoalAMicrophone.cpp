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

#include "deoalAMicrophone.h"
#include "deoalMicrophone.h"
#include "../deoalBasics.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATDebug.h"
#include "../audiothread/deoalATLogger.h"
#include "../audiothread/deoalDebugInfo.h"
#include "../effect/deoalSharedEffectSlotManager.h"
#include "../environment/deoalEnvProbe.h"
#include "../environment/deoalEnvProbeList.h"
#include "../environment/raytrace/deoalSoundRay.h"
#include "../environment/raytrace/deoalSoundRaySegment.h"
#include "../extensions/deoalExtensions.h"
#include "../speaker/deoalASpeaker.h"
#include "../world/deoalAWorld.h"
#include "../world/octree/deoalWorldOctree.h"
#include "../world/octree/deoalWOVPrepareRayTrace.h"
#include "../world/octree/deoalWOVFindSpeakers.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/debug/deDebugBlockInfo.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>



// Class deoalAMicrophone
///////////////////////////

// Constructor, destructor
////////////////////////////

deoalAMicrophone::deoalAMicrophone(deoalAudioThread &audioThread) :
pAudioThread(audioThread),
pVolume(1.0f),
pMuted(true),
pSpeakerGain(1.0f),
pEnableAuralization(true),
pParentWorld(NULL),
pOctreeNode(NULL),
pEnvProbeList(NULL),
pActive(false),
pEnvProbe(NULL),
pDirtyEnvProbe(true),
pDirtyGeometry(true),
pDirtyGain(true),

pWorldMarkedRemove(false),
pLLWorldPrev(NULL),
pLLWorldNext(NULL)
{
	pWOVPrepareRayTrace.SetRTWorldBVH(&pRTWorldBVH);
	
	try{
		// set cone stuff ( not working )
		//OAL_CHECK( pAudioThread, alListenerf( AL_CONE_INNER_ANGLE, 90.0f ) );
		//OAL_CHECK( pAudioThread, alListenerf( AL_CONE_OUTER_ANGLE, 180.0f ) );
		//OAL_CHECK( pAudioThread, alListenerf( AL_CONE_OUTER_GAIN, 0.5f ) );
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	LEAK_CHECK_CREATE(audioThread, Microphone);
}

deoalAMicrophone::~deoalAMicrophone(){
	LEAK_CHECK_FREE(pAudioThread, Microphone);
	
	pCleanUp();
}



// Management
///////////////

void deoalAMicrophone::SetGeometry(const decDVector &position, const decQuaternion &orientation){
	pPosition = position;
	pOrientation = orientation;
	pDirtyGeometry = true;
	pWOVPrepareRayTrace.SetCenter(position);
}

void deoalAMicrophone::SetVelocity(const decVector &velocity){
	pVelocity = velocity;
	pDirtyGeometry = true;
}

void deoalAMicrophone::SetVolume(float volume){
	pVolume = volume;
	pDirtyGain = true;
}

void deoalAMicrophone::SetMuted(bool muted){
	pMuted = muted;
	pDirtyGain = true;
}

void deoalAMicrophone::SetLayerMask(const decLayerMask &layerMask){
	pLayerMask = layerMask;
	pWOVPrepareRayTrace.SetLayerMask(layerMask);
	
	if(pEnvProbeList){
		pEnvProbeList->SetLayerMask(layerMask);
	}
}

void deoalAMicrophone::SetSpeakerGain(float gain){
	pSpeakerGain = gain;
}

void deoalAMicrophone::SetEnableAuralization(bool enable){
	pEnableAuralization = enable;
}



void deoalAMicrophone::SetActive(bool active){
	// WARNING Called during synchronization time from main thread.
	
	if(active == pActive){
		return;
	}
	
	if(pActive){
		pEnableAttachedSpeakers(false);
		pActiveSpeakers.EnableAll(false);
		pActiveSpeakers.RemoveAll();
		
		if(pParentWorld){
			pParentWorld->SetAllSpeakersEnabled(false);
		}
	}
	
	pActive = active;
	
	if(active){
		pDirtyGain = true;
		pEnableAttachedSpeakers(true);
		
		if(pParentWorld){
			pDirtyGeometry = true;
		}
	}
}



int deoalAMicrophone::GetSpeakerCount() const{
	return pSpeakers.GetCount();
}

deoalASpeaker *deoalAMicrophone::GetSpeakerAt(int index) const{
	return (deoalASpeaker*)pSpeakers.GetAt(index);
}

void deoalAMicrophone::AddSpeaker(deoalASpeaker *speaker){
	// WARNING Called during synchronization time from main thread.
	
	pInvalidateSpeakers.RemoveIfPresent(speaker);
	pSpeakers.Add(speaker);
	
	speaker->SetPositionless(true);
	speaker->SetEnabled(pActive);
	speaker->SetParentMicrophone(this);
}

void deoalAMicrophone::RemoveSpeaker(deoalASpeaker *speaker){
	// WARNING Called during synchronization time from main thread.
	
	const int index = pSpeakers.IndexOf(speaker);
	DEASSERT_TRUE(index != -1)
	
	speaker->SetEnabled(false);
	speaker->SetParentMicrophone(nullptr);
	
	if(pAudioThread.GetActiveMicrophone() == this){
		InvalidateSpeaker(speaker);
	}
	
	pSpeakers.RemoveFrom(index);
}

void deoalAMicrophone::RemoveAllSpeakers(){
	// WARNING Called during synchronization time from main thread.
	
	const bool isActive = pAudioThread.GetActiveMicrophone() == this;
	const int count = pSpeakers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoalASpeaker * const speaker = (deoalASpeaker*)pSpeakers.GetAt(i);
		speaker->SetEnabled(false);
		speaker->SetParentMicrophone(nullptr);
		if(isActive){
			InvalidateSpeaker(speaker);
		}
	}
	
	pSpeakers.RemoveAll();
}

void deoalAMicrophone::RemoveRemovalMarkedSpeakers(){
	// WARNING Called during synchronization time from main thread.
	
	const int count = pSpeakers.GetCount();
	int i;
	
	for(i=count-1; i>=0; i--){
		deoalASpeaker * const speaker = (deoalASpeaker*)pSpeakers.GetAt(i);
		if(speaker->GetMicrophoneMarkedRemove()){
			RemoveSpeaker(speaker);
		}
	}
}



void deoalAMicrophone::FindActiveSpeakers(){
	// determine list of speakers
	deoalSpeakerList &list = pAudioThread.GetSpeakerList();
	list.RemoveAll();
	if(pParentWorld){
		deoalWOVFindSpeakers::FindSpeakers(*pParentWorld, pPosition, pLayerMask, list);
	}
	
	// enable/disable speakers.
	pActiveSpeakers.FlagAll(false);
	list.FlagAll(true);
	
	pActiveSpeakers.FlagToEnabledAll();
	list.FlagToEnabledAll();
	
	// store speaker list
	pActiveSpeakers = list;
	
	// if the maximum range becomes larger mark env probe dirty. becoming smaller is not a
	// problem since we can reuse the information. only if the range becomes larger we need
	// to extend the result. theoretically we could just extend the existing results but this
	// is difficult to do and most probably not work the hazzle
	if(pEnvProbeList && !pDirtyEnvProbe){
		pDirtyEnvProbe = pMaxActiveSpeakerRange() > pEnvProbeList->GetRange();
	}
}



void deoalAMicrophone::SetParentWorld(deoalAWorld *world){
	// WARNING Called during synchronization time from main thread.
	
	if(world == pParentWorld){
		return;
	}
	
	// if the microphone is the active one disable all speakers. otherwise they
	// will continue to play although they are in a different world
	if(pActive){
		// pEnableAttachedSpeakers(false); // these can still play it outside a world
		pActiveSpeakers.EnableAll(false);
	}
	
	pActiveSpeakers.RemoveAll();
	
	if(pEnvProbeList){
		delete pEnvProbeList;
		pEnvProbeList = nullptr;
	}
	
	if(pOctreeNode){
		pOctreeNode->RemoveMicrophone(this);
	}
	
	pParentWorld = world;
	
	if(pActive){
		// if the microphone is the active one enable all active speakers if added to a new world
		// pEnableAttachedSpeakers(true); // not disabled above so do not enable here
		
		if(world){
			pActiveSpeakers.EnableAll(true);
		}
	}
}

void deoalAMicrophone::SetOctreeNode(deoalWorldOctree *node){
	pOctreeNode = node;
}

void deoalAMicrophone::UpdateOctreeNode(){
	if(pParentWorld && pActive && !pMuted){
		pParentWorld->GetOctree()->InsertMicrophoneIntoTree(this, 8);
		
	}else if(pOctreeNode){
		pOctreeNode->RemoveMicrophone(this);
	}
}

void deoalAMicrophone::PrepareQuickDispose(){
	if(pEnvProbeList){
		pEnvProbeList->PrepareQuickDispose();
		delete pEnvProbeList;
		pEnvProbeList = NULL;
	}
	
	pParentWorld = NULL;
	pOctreeNode = NULL;
}



deoalEnvProbe *deoalAMicrophone::GetEnvProbe(){
	#ifdef LISTENER_CENTRIC_RAY_CAST
// 	if( pEnvProbe && ! pEnvProbe->GetOctreeNode() ){
// 		// env probe has been invalidated
// 		pDirtyEnvProbe = true;
// 	}
	// this check is disabled because with pDirtyEnvProbe is set to true for each frame update
	
	if(!pDirtyEnvProbe){
		return pEnvProbe;
	}
	
	pDirtyEnvProbe = false;
	pEnvProbe = NULL;
	
	if(!pParentWorld || pActiveSpeakers.GetCount() == 0){
		return NULL;
	}
	
	// calculate range of probe. set this to the largest range of all speakers
	const float range = pMaxActiveSpeakerRange();
// 	const float range = 200.0f; // debug
	
	if(!pEnvProbeList){
		const float reuseDistance = 0.25; // 1.0, 0.05
		const int maxProbeCount = 100;
		
		pEnvProbeList = new deoalEnvProbeList(*pParentWorld, reuseDistance, maxProbeCount);
		pEnvProbeList->SetRange(range);
		pEnvProbeList->SetLayerMask(pLayerMask);
		pEnvProbeList->SetRTWorldBVH(&pRTWorldBVH);
		pEnvProbeList->SetRTConfig(&pAudioThread.GetRayTracing().GetConfigSoundTracingMicrophone());
	}
	
	// prepare for ray-tracing all components touched by the audible distance. no ray can
	// go farther than the audible distance. this allows us to prepare only those components
	// which are potentially affected improving performance
	// 
	// NOTE the visitor parameters are kept up to date so they are not set here
	pWOVPrepareRayTrace.SetRadius(range);
	pWOVPrepareRayTrace.Visit(*pParentWorld);
	
	// trace sound rays
	try{
		pEnvProbe = pEnvProbeList->GetProbeTraceSoundRays(pPosition);
		
	}catch(const deException &e){
		pAudioThread.GetLogger().LogException(e);
		// exceptions can happen during cancelling. using NULL is fine
	}
	
	return pEnvProbe;
	
	#else
	return NULL;
	#endif
}



void deoalAMicrophone::ProcessAudio(){
	if(!pActive){
		return;
	}
	
	// update openal resources
	if(pDirtyGeometry){
		pDirtyGeometry = false;
		
		OAL_CHECK(pAudioThread, alListener3f(AL_POSITION,
			(float)pPosition.x, (float)pPosition.y, (float)-pPosition.z));
		
		const decMatrix matrix(decMatrix::CreateFromQuaternion(pOrientation));
		const decVector up(matrix.TransformUp());
		const decVector view(matrix.TransformView());
		const ALfloat parameters[] = {view.x, view.y, -view.z, up.x, up.y, -up.z};
		
		OAL_CHECK(pAudioThread, alListenerfv(AL_ORIENTATION, &parameters[0]));
		
		OAL_CHECK(pAudioThread, alListener3f(AL_VELOCITY, pVelocity.x, pVelocity.y, -pVelocity.z));
	}
	
	if(pDirtyGain){
		pDirtyGain = false;
		
		OAL_CHECK(pAudioThread, alListenerf(AL_GAIN, pMuted ? 0.0f : pVolume));
	}
	
	// prepare environment probe list
	if(pEnvProbeList){
		pEnvProbeList->PrepareProcessAudio();
	}
	
	// update all speakers
	pActiveSpeakers.UpdateAll();
	
	int i, count = pInvalidateSpeakers.GetCount();
	for(i=0; i<count; i++){
		((deoalASpeaker*)pInvalidateSpeakers.GetAt(i))->PrepareProcessAudio();
	}
	pInvalidateSpeakers.RemoveAll();
	pAudioThread.GetDebugInfo().StoreTimeAudioThreadSpeakersUpdate();
	
	// process speakers stored in the world and this microphone
	if(pParentWorld){
		pParentWorld->PrepareProcessAudio();
	}
	pAudioThread.GetDebugInfo().StoreTimeAudioThreadWorldProcess();
	
	count = pSpeakers.GetCount();
	for(i=0; i<count; i++){
		((deoalASpeaker*)pSpeakers.GetAt(i))->PrepareProcessAudio();
	}
	pAudioThread.GetDebugInfo().StoreTimeAudioThreadSpeakersProcess();
	
	// process effects
	pProcessEffects();
	pAudioThread.GetDebugInfo().StoreTimeAudioThreadEffectsProcess();
}

void deoalAMicrophone::ProcessAudioFast(){
	if(!pActive){
		return;
	}
	
	pActiveSpeakers.UpdateAll();
	
	int i, count = pInvalidateSpeakers.GetCount();
	for(i=0; i<count; i++){
		((deoalASpeaker*)pInvalidateSpeakers.GetAt(i))->PrepareProcessAudio();
	}
	pInvalidateSpeakers.RemoveAll();
	
	if(pParentWorld){
		pParentWorld->PrepareProcessAudio();
	}
	
	count = pSpeakers.GetCount();
	for(i=0; i<count; i++){
		((deoalASpeaker*)pSpeakers.GetAt(i))->PrepareProcessAudio();
	}
}

void deoalAMicrophone::ProcessDeactivate(){
	int i, count = pInvalidateSpeakers.GetCount();
	for(i=0; i<count; i++){
		((deoalASpeaker*)pInvalidateSpeakers.GetAt(i))->ProcessDeactivate();
	}
	pInvalidateSpeakers.RemoveAll();
	
	count = pSpeakers.GetCount();
	for(i=0; i<count; i++){
		((deoalASpeaker*)pSpeakers.GetAt(i))->ProcessDeactivate();
	}
	
	if(pParentWorld){
		pParentWorld->ProcessDeactivate();
	}
}

void deoalAMicrophone::InvalidateSpeaker(deoalASpeaker *speaker){
	pActiveSpeakers.RemoveIfExisting(speaker);
	pInvalidateSpeakers.AddIfAbsent(speaker);
}



enum eDebugInfo{
	ediPosition,
	ediVelocity,
	ediRange,
	ediSoundRayCount,
	ediRoomSize,
	ediRoomVolSurf,
	ediRoomSabine,
	ediMeanFreePath,
	ediSepTimeFirstLate,
	ediReverbeTime,
	ediEchoDelay,
	ediProbeCount
};

void deoalAMicrophone::DebugUpdateInfo(deDebugBlockInfo &debugInfo){
	if(debugInfo.GetEntryCount() == 0){
		debugInfo.AddEntry("Position", "");
		debugInfo.AddEntry("Velocity", "");
		debugInfo.AddEntry("Range", "");
		debugInfo.AddEntry("Sound Rays", "");
		debugInfo.AddEntry("Hit Space Size", "");
		debugInfo.AddEntry("Room Vol/Surf", "");
		debugInfo.AddEntry("Room Sabine", "");
		debugInfo.AddEntry("Mean Free Path", "");
		debugInfo.AddEntry("Sep Time F/L", "");
		debugInfo.AddEntry("Reverbe Time", "");
		debugInfo.AddEntry("Echo Delay", "");
		debugInfo.AddEntry("Env-Probes", "");
	}
	
	decString text;
	text.Format("(%+.3f,%+.3f,%+.3f)", pPosition.x, pPosition.y, pPosition.z);
	debugInfo.SetEntryText(ediPosition, text);
	
	text.Format("(%+.3f,%+.3f,%+.3f)", pVelocity.x, pVelocity.y, pVelocity.z);
	debugInfo.SetEntryText(ediVelocity, text);
	
	text.Format("%.1f", pEnvProbe ? pEnvProbe->GetRange() : pMaxActiveSpeakerRange());
	debugInfo.SetEntryText(ediRange, text);
	
	const decColor colorGood(1.0f, 1.0f, 1.0f);
	const decColor colorWarning(1.0f, 0.0f, 0.0f);
	
	deoalEnvProbe * const envProbe = GetEnvProbe();
	
	if(envProbe){
		text.Format("%d %d", envProbe->GetSoundRayList().GetRayCount(),
			envProbe->GetSoundRayList().GetSegmentCount());
		debugInfo.SetEntryText(ediSoundRayCount, text);
		
		const decDVector roomSize(envProbe->GetMaxExtend() - envProbe->GetMinExtend());
		text.Format("(%.1f,%.1f,%.1f)", roomSize.x, roomSize.y, roomSize.z);
		debugInfo.SetEntryText(ediRoomSize, text);
		debugInfo.SetEntryColor(ediRoomSize, roomSize < decVector(100.0f, 100.0f, 100.0f)
			? colorGood : colorWarning);
		
		text.Format("%.1f %.1f", envProbe->GetRoomVolume(), envProbe->GetRoomSurface());
		debugInfo.SetEntryText(ediRoomVolSurf, text);
		debugInfo.SetEntryColor(ediRoomVolSurf, envProbe->GetRoomVolume() < 2000.0f
			|| envProbe->GetRoomSurface() > 1000.0f ? colorGood : colorWarning);
		
		text.Format("%.1f %.1f", envProbe->GetRoomSabineLow(), envProbe->GetRoomSabineHigh());
		debugInfo.SetEntryText(ediRoomSabine, text);
		
		text.Format("%.3f", envProbe->GetMeanFreePath());
		debugInfo.SetEntryText(ediMeanFreePath, text);
		
		text.Format("%.3f", envProbe->GetSepTimeFirstLateRefl());
		debugInfo.SetEntryText(ediSepTimeFirstLate, text);
		
		text.Format("%.1f %.1f", envProbe->GetReverberationTimeLow(),
			envProbe->GetReverberationTimeHigh());
		debugInfo.SetEntryText(ediReverbeTime, text);
		
		text.Format("%.3f", envProbe->GetEchoDelay());
		debugInfo.SetEntryText(ediEchoDelay, text);
		
		int probeCount = 0;
		int validProbeCount = 0;
		if(pEnvProbeList){
			probeCount = pEnvProbeList->GetProbeCount();
			validProbeCount = pEnvProbeList->GetValidProbeCount();
		}
		text.Format("%d %d", probeCount, validProbeCount);
		debugInfo.SetEntryText(ediProbeCount, text);
		
	}else{
		debugInfo.SetEntryText(ediSoundRayCount, "-");
		debugInfo.SetEntryText(ediRoomSize, "-");
		debugInfo.SetEntryColor(ediRoomSize, colorGood);
		debugInfo.SetEntryText(ediRoomVolSurf, "-");
		debugInfo.SetEntryColor(ediRoomVolSurf, colorGood);
		debugInfo.SetEntryText(ediRoomSabine, "-");
		debugInfo.SetEntryText(ediReverbeTime, "-");
		debugInfo.SetEntryText(ediEchoDelay, "-");
		debugInfo.SetEntryText(ediProbeCount, "-");
	}
	
	debugInfo.UpdateView();
}

void deoalAMicrophone::DebugCaptureRays(deDebugDrawer &debugDrawer, bool xray, bool volume){
	pDebugCaptureRays(debugDrawer, xray, volume);
}



// Render world usage
///////////////////////

void deoalAMicrophone::SetWorldMarkedRemove(bool marked){
	pWorldMarkedRemove = marked;
}

void deoalAMicrophone::SetLLWorldPrev(deoalAMicrophone *microphone){
	pLLWorldPrev = microphone;
}

void deoalAMicrophone::SetLLWorldNext(deoalAMicrophone *microphone){
	pLLWorldNext = microphone;
}



// Private Functions
//////////////////////

void deoalAMicrophone::pCleanUp(){
	RemoveAllSpeakers();
	
	pParentWorld = NULL;
	pEnvProbe = NULL;
	if(pEnvProbeList){
		delete pEnvProbeList;
		pEnvProbeList = NULL;
	}
}



void deoalAMicrophone::pEnableAttachedSpeakers(bool enable){
	// WARNING Called during synchronization time from main thread.
	
	const int count = pSpeakers.GetCount();
	int i;
	for(i=0; i<count; i++){
		((deoalASpeaker*)pSpeakers.GetAt(i))->SetEnabled(enable);
	}
}

void deoalAMicrophone::pProcessEffects(){
	// get environment probe for listener sound ray tracing. tracing sound rays is an expensive
	// process so we want to do it as little as possible. by using listener centric sound ray
	// tracing we have to do this process only once not for each sound source. sound sources
	// only use room parameter estimation which is much cheaper to calculate
	pDirtyEnvProbe = true;
	
	#if 0
	deoalAWorld * const world = GetParentWorld();
	
	if(pEnvProbe){
		pEnvProbe->RemoveUsage();
		pEnvProbe = NULL;
	}
	
	if(world && pActiveSpeakers.GetCount() > 0){
		// calculate range of probe. set this to the largest range of all audible speakers
		// but not larger than the audible distance set by the developer
		const int count = pActiveSpeakers.GetCount();
		float range = 0.0f;
		int i;
		
		for(i=0; i<count; i++){
			range = decMath::max(range, pActiveSpeakers.GetAt(i)->GetRange());
		}
		range = decMath::min(range, GetAudibleDistance());
		
		// trace sound rays. since this sound ray list is going to be used with varying sound
		// source range and attenuation parameters we use no attenuation (refDist=1, rollOff=0)
		#ifdef LISTENER_CENTRIC_RAY_CAST
		pEnvProbe = world->GetEnvProbeManager().GetProbeTraceSoundRays(pPosition, range, 1.0f, 0.0f);
		pEnvProbe->AddUsage();
		
		/*
		if(pAudioThread.GetDevMode()->GetLogCalcEnvProbe()){
			pAudioThread.LogInfoFormat("Microphone.ProcessEffects: pos=(%.3f,%.3f,%.3f) "
				"speakers=%d audDist=%.3f range=%.3f", pPosition.x, pPosition.y, pPosition.z,
				pActiveSpeakers.GetCount(), GetAudibleDistance(), range);
		}
		*/
		#endif
		
		/*
		Parallel-TraceSoundRays:
			pos=(1.594,1.600,-1.216) range=60.000 rays=162
		Result-TraceSoundRays:
			minExt=(-13.000,-0.000,-8.000) maxExt=(50.134,2.500,2.900)
			center=(18.567,1.250,-2.550) size=(63.134,2.500,10.900) avgSize=25.511
			mfp=2.570 echoDelay=0.007 volume=128.445 surface=163.822 abs=0.038
			sabine=(3.182,6.171,9.367) revTime=(6.503,3.354,2.209) sepTimeFLR=0.0206
			rays=474 segments=6317 time=52.87ms
		WOVRHE: all=52.929 comp=0.960(9088) compOctree=16.261(9088)
		Parallel-TraceSoundRays:
			pos=(0.000,1.600,-4.000) range=60.000 rays=162
		
		Parallel-TraceSoundRays:
			pos=(20.096,1.601,14.649) range=60.000 rays=162
		Result-TraceSoundRays:
			minExt=(-24.195,-18.080,-27.474) maxExt=(60.345,52.065,56.772)
			center=(18.075,16.993,14.649) size=(84.540,70.145,84.245) avgSize=79.643
			mfp=3.792 echoDelay=0.011 volume=860.109 surface=582.996 abs=0.048
			sabine=(5.915,27.943,53.394) revTime=(23.430,4.959,2.595) sepTimeFLR=0.0303
			rays=752 segments=6820 time=261.69ms
		WOVRHE: all=261.737 comp=1.975(20091) compOctree=227.608(20091)
		
		evaluation:
		case 1:
			world octree: 35.708ms
			components: 9088
				prepare: 0.960ms
				octree: 16.261ms
		case 2:
			world octree: 32.154ms
			components: 20091
				prepare: 1.975ms
				octree: 227.608ms
		*/
		
		/*
		voxelization
		- https://viscomp.alexandra.dk/?p=3836
		- https://www.geeks3d.com/20160531/mesh-voxelization/
		- https://www.forceflow.be/2012/10/26/an-accurate-method-for-voxelizing-polygon-meshes/
		- https://scialert.net/fulltext/?doi=itj.2007.1286.1289
		- http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.12.6294
		- http://ij3c.ncuteecs.org/volume/paperfile/5-1/IJ3C_6.pdf
		- https://github.com/tunabrain/sparse-voxel-octrees
		
		3d bresenhams algorithm
		- https://www.geeksforgeeks.org/bresenhams-algorithm-for-3-d-line-drawing/
		*/
	}
	#endif
	
	// update effects of all speakers in audible range
	// if( pAudioThread.GetConfiguration().GetUseSharedEffectSlots() ){
		// pAudioThread.GetSharedEffectSlotManager().ClearSpeakers();
	// }
	
	pActiveSpeakers.UpdateEffectsAll();
	
	// if( pAudioThread.GetConfiguration().GetUseSharedEffectSlots() ){
		// pAudioThread.GetSharedEffectSlotManager().AssignSpeakers();
	// }
	
	if(!pAudioThread.GetExtensions().GetHasEFX()
	|| !pAudioThread.GetConfiguration().GetEnableEFX()){
		pAudioThread.GetSharedEffectSlotManager().DropEffects();
		
	}else{
		// pAudioThread.GetSharedEffectSlotManager().DebugLogState();
	}
}

void deoalAMicrophone::pDebugCaptureRays(deDebugDrawer &debugDrawer, bool xray, bool volume){
	debugDrawer.RemoveAllShapes();
	
	deoalEnvProbe * const envProbe = GetEnvProbe();
	
	if(!envProbe){
		debugDrawer.SetVisible(false);
		return;
	}
	
	debugDrawer.SetVisible(true);
	debugDrawer.SetXRay(xray);
	
	debugDrawer.SetPosition(envProbe->GetPosition());
	
	const deoalSoundRayList &srlist = envProbe->GetSoundRayList();
	const int rayCount = srlist.GetRayCount();
	const float hsvFactor = 6.0f / (float)(decMath::max(rayCount - 1, 1));
	const float colorA = 0.5f;
	decColor color;
	int i;
	
	for(i=0; i<rayCount; i++){
		const deoalSoundRay &ray = srlist.GetRayAt(i);
		if(ray.GetSegmentCount() == 0 && ray.GetTransmittedRayCount() == 0){
			continue;
		}
		
		if(volume){
			color.Set(1, 0, 0);
			
		}else{
			// colorize rays. HSV to RGB (H=0..360, S=0..1, V=0..1):
			// c = v * s = 1  // because v=1 and s=1
			// t = h / 60  // with H=0..1 => t = h * 6
			// x = c * (1 - abs(mod(t, 2) - 1))
			//   = 1 - abs(mod(t, 2) - 1)
			// m = v - c = 0
			// switch(t):
			// case 0: rgb = (c+m, x+m, m) = (1, x, 0)
			// case 1: rgb = (x+m, c+m, m) = (x, 1, 0)
			// case 2: rgb = (m, c+m, x+m) = (0, 1, x)
			// case 3: rgb = (m, x+m, c+m) = (0, x, 1)
			// case 4: rgb = (x+m, m, c+m) = (x, 0, 1)
			// case 5: rgb = (c+m, m, x+m) = (1, 0, x)
			const float hsvT = hsvFactor * i;
			const float hsvX = 1.0f - fabsf(fmodf(hsvT, 2.0f) - 1.0f);
			
			switch((int)hsvT){
			case 0: color.Set(1.0f, hsvX, 0.0f, colorA); break;
			case 1: color.Set(hsvX, 1.0f, 0.0f, colorA); break;
			case 2: color.Set(0.0f, 1.0f, hsvX, colorA); break;
			case 3: color.Set(0.0f, hsvX, 1.0f, colorA); break;
			case 4: color.Set(hsvX, 0.0f, 1.0f, colorA); break;
			case 5:
			default: color.Set(1.0f, 0.0f, hsvX, colorA);
			}
		}
		
		deDebugDrawerShape * const shape = new deDebugDrawerShape;
		shape->SetFillColor(decColor(0.0f, 0.0f, 0.0f, 0.0f));
		shape->SetEdgeColor(color);
		
		pDebugCaptureRays(*shape, srlist, ray, volume);
		
		debugDrawer.AddShape(shape);
	}
	
	deDebugDrawerShape * const shapeOrigin = new deDebugDrawerShape;
	shapeOrigin->SetFillColor(decColor(1.0f, 1.0f, 1.0f, colorA * 0.25f));
	shapeOrigin->SetEdgeColor(decColor(1.0f, 1.0f, 1.0f, colorA));
	shapeOrigin->GetShapeList().Add(new decShapeSphere(0.05f));
	debugDrawer.AddShape(shapeOrigin);
	
	for(i=0; i<rayCount; i++){
		const deoalSoundRay &ray = srlist.GetRayAt(i);
		if(ray.GetSegmentCount() == 0){
			continue;
		}
		
		deDebugDrawerShape * const shape = new deDebugDrawerShape;
		shape->SetFillColor(decColor(0.0f, 0.0f, 0.0f, 0.0f));
		shape->SetEdgeColor(decColor(1.0f, 1.0f, 1.0f, colorA));
		
		deDebugDrawerShapeFace * const face = new deDebugDrawerShapeFace;
		face->AddVertex(decVector());
		face->AddVertex(srlist.GetSegmentAt(ray.GetFirstSegment()).GetPosition());
		face->AddVertex(decVector());
		face->SetNormal(decVector(0.0f, 0.0f, 1.0f));
		shape->AddFace(face);
		
		debugDrawer.AddShape(shape);
	}
	
	debugDrawer.NotifyShapeGeometryChanged();
}

void deoalAMicrophone::pDebugCaptureRays(deDebugDrawerShape &shape,
const deoalSoundRayList &rayList, const deoalSoundRay &ray, bool volume){
	const int segmentCount = ray.GetSegmentCount();
	const int firstSegment = ray.GetFirstSegment();
	int i;
	
	for(i=0; i<segmentCount; i++){
		const deoalSoundRaySegment &segment = rayList.GetSegmentAt(firstSegment + i);
		
		deDebugDrawerShapeFace * const face = new deDebugDrawerShapeFace;
		face->AddVertex(segment.GetPosition());
		face->AddVertex(segment.GetPosition() + segment.GetDirection() * segment.GetLength());
		face->AddVertex(segment.GetPosition());
		face->SetNormal(decVector(0.0f, 0.0f, 1.0f));
		shape.AddFace(face);
	}
	
	const int transmittedRayCount = ray.GetTransmittedRayCount();
	const int firstTransmittedRay = ray.GetFirstTransmittedRay();
	
	for(i=0; i<transmittedRayCount; i++){
		pDebugCaptureRays(shape, rayList, rayList.GetTransmittedRayAt(firstTransmittedRay + i), volume);
	}
}

float deoalAMicrophone::pMaxActiveSpeakerRange() const{
	const int count = pActiveSpeakers.GetCount();
	float range = 0.0f;
	int i;
	
	for(i=0; i<count; i++){
		range = decMath::max(range, pActiveSpeakers.GetAt(i)->GetRange());
	}
	
	return range;
}
