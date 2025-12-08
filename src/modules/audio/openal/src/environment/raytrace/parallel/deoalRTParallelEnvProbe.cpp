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

#include "deoalRTParallelEnvProbe.h"
#include "deoalRTPTTraceSoundRays.h"
#include "deoalRTPTTraceSoundRaysFinish.h"
#include "deoalRTPTListen.h"
#include "deoalRTPTListenFinish.h"
// #include "deoalRTPTEnvProbeFull.h"
#include "deoalRTPTRoomEstimate.h"
#include "deoalRTPTRoomEstimateFinish.h"
#include "../deoalSoundRayInteraction.h"
#include "../deoalRayTraceConfig.h"
#include "../../deoalEnvProbe.h"
#include "../../deoalEnvProbeListener.h"
#include "../../../deoalBasics.h"
#include "../../../deAudioOpenAL.h"
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"
#include "../../../audiothread/deoalATDebug.h"
#include "../../../devmode/deoalDevMode.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/threading/deMutexGuard.h>



// Constants
//////////////

/**
 * \brief Reverberation time factor.
 * 
 * 24.0 * log( 10.0 ) / speedSound
 */
#define REVERB_TIME_FACTOR 0.1611138f



// Class deoalRTParallelEnvProbe
//////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTParallelEnvProbe::deoalRTParallelEnvProbe(deoalAudioThread &audioThread) :
pAudioThread(audioThread),
pBarrier(2),

pTimeHistoryTraceSoundRays(19, 1),
pTimeHistoryEstimateRoom(19, 1),
pTimeHistoryListen(19, 1),
pCounterTraceSoundRays(0),
pCounterEstimateRoom(0),
pCounterListen(0),
pElapsedRTTime(0.0f),
pBarrierTask(nullptr)
{
#if 0
	// NOTE
	// all tasks except the finish tasks have to be non-reused before their Finish() method
	// has been called. this is required because the finish tasks have these regular tasks
	// as dependency. if reused before Finish() method is called the deParallelProcessing
	// modifies them in the main thread when it is time to call Finish(). this can cause
	// very bad bug or crashes. for this reason regular tasks are stored in two lists moving
	// them from the available list to the running list and back once Finished() is called.
	// this ensures no problem can happen.
	// 
	// for the finish tasks this extra work is not necessary. because no other tasks depend
	// on them there is nothing to modify by deParallelProcessing in the main thread. it is
	// not fully correct to do it like this but it is working in a safe way as long as
	// nobody depends on these finish tasks
	
// 	const int taskCount = 8; //42; // TODO store in config for example
	const int rayCount = 42;
	
	if(true){
		deoalRTPTTraceSoundRays::Ref task;
		int i;
		for(i=0; i<rayCount; i++){
			task.TakeOverWith(*this);
			pTasksTraceSoundRays.Add(task);
			pTasksReadyTraceSoundRays.Add(task);
		}
	}
	
	if(true){
		deoalRTPTListen::Ref task;
		int i;
		for(i=0; i<rayCount; i++){
			task.TakeOver(new deoalRTPTListen(*this));
			pTasksListen.Add(task);
			pTasksReadyListen.Add(task);
		}
	}
	
// 	if( true ){
// 		deoalRTPTEnvProbeFull::Ref task;
// 		int i;
// 		for( i=0; i<taskCount; i++ ){
// 			task.TakeOver( new deoalRTPTEnvProbeFull( *this ) );
// 			pTasksFull.Add( task );
// 		}
// 	}
	
	if(true){
		deoalRTPTRoomEstimate::Ref task;
		int i;
		for(i=0; i<rayCount; i++){
			task.TakeOver(new deoalRTPTRoomEstimate(*this));
			pTasksRoomEstimate.Add(task);
			pTasksReadyRoomEstimate.Add(task);
		}
	}
#endif
}

deoalRTParallelEnvProbe::~deoalRTParallelEnvProbe(){
#if 0
	pAudioThread.GetLogger().LogInfoFormat(
		"RTParallelEnvProbe: TraceSoundRays: tasks=%d ready=%d running=%d\n",
		pTasksTraceSoundRays.GetCount(), pTasksReadyTraceSoundRays.GetCount(),
		pTasksRunningTraceSoundRays.GetCount());
	
	pAudioThread.GetLogger().LogInfoFormat(
		"RTParallelEnvProbe: TraceSoundRaysFinish: tasks=%d ready=%d running=%d\n",
		pTasksTraceSoundRaysFinish.GetCount(), pTasksReadyTraceSoundRaysFinish.GetCount(),
		pTasksRunningTraceSoundRaysFinish.GetCount());
	
	pAudioThread.GetLogger().LogInfoFormat(
		"RTParallelEnvProbe: Listen: tasks=%d ready=%d running=%d\n",
		pTasksListen.GetCount(), pTasksReadyListen.GetCount(),
		pTasksRunningListen.GetCount());
	
	pAudioThread.GetLogger().LogInfoFormat(
		"RTParallelEnvProbe: ListenFinish: tasks=%d ready=%d running=%d\n",
		pTasksListenFinish.GetCount(), pTasksReadyListenFinish.GetCount(),
		pTasksRunningListenFinish.GetCount());
	
	pAudioThread.GetLogger().LogInfoFormat(
		"RTParallelEnvProbe: RoomEstimate: tasks=%d ready=%d running=%d\n",
		pTasksRoomEstimate.GetCount(), pTasksReadyRoomEstimate.GetCount(),
		pTasksRunningRoomEstimate.GetCount());
	
	pAudioThread.GetLogger().LogInfoFormat(
		"RTParallelEnvProbe: RoomEstimateFinish: tasks=%d ready=%d running=%d\n",
		pTasksRoomEstimateFinish.GetCount(), pTasksReadyRoomEstimateFinish.GetCount(),
		pTasksRunningRoomEstimateFinish.GetCount());
#endif
}



// Manegement
///////////////

// #include "../../../world/deoalAWorld.h"
// #include "../../../component/deoalAComponent.h"
// #include "../../../model/deoalAModel.h"

void deoalRTParallelEnvProbe::TraceSoundRays(sRoomParameters &roomParameters,
deoalSoundRayList &soundRayList, const decDVector &position, float range,
float refDist, float rollOff, float distanceOffset, deoalAWorld &world,
deoalRTWorldBVH *rtWorldBVH, const decLayerMask &layerMask,
const deoalATRayTracing::sConfigSoundTracing &config){
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		pAudioThread.GetLogger().LogInfoFormat(
			"Parallel-TraceSoundRays: pos=(%.3f,%.3f,%.3f) range=%.3f rays=%d",
			position.x, position.y, position.z, range, config.rtConfig->GetRayCount());
	}
	
	pTimer.Reset();
	
	pRunTraceSoundRaysUsingTasks(roomParameters, soundRayList, position, range,
		refDist, rollOff, distanceOffset, world, rtWorldBVH, layerMask, config);
	#if defined WOVRAYHITSELEMENT_DO_TIMING || defined RTWOVRAYHITSELEMENT_DO_TIMING \
	|| defined RTWOVRAYHITSCLOSEST_DO_TIMING || defined RTWOVRAYBLOCKED_DO_TIMING
	const decPointerList wovTimingTasks(pTasksWaitTraceSoundRays);
	#endif
	
	const float elapsed = pTimer.GetElapsedTime();
	pTimeHistoryTraceSoundRays.Add(elapsed);
	pCounterTraceSoundRays++;
	pElapsedRTTime += elapsed;
	
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		const float timeCalcMS = elapsed * 1e3f;
		
		const decDVector roomCenter((roomParameters.minExtend + roomParameters.maxExtend) * 0.5);
		const decDVector roomSize(roomParameters.maxExtend - roomParameters.minExtend);
		const double avgRoomSize = (roomSize.x + roomSize.y + roomSize.z) / 3.0;
		
		pAudioThread.GetLogger().LogInfoFormat(
			"Result-TraceSoundRays: minExt=(%.3f,%.3f,%.3f) maxExt=(%.3f,%.3f,%.3f) "
			"center=(%.3f,%.3f,%.3f) size=(%.3f,%.3f,%.3f) avgSize=%.3f mfp=%.3f echoDelay=%.3f "
			"volume=%.3f surface=%.3f abs=%.3f sabine=(%.3f,%.3f,%.3f) revTime=(%.3f,%.3f,%.3f) "
			"sepTimeFLR=%.4f rays=%d segments=%d time=%.2fms",
			roomParameters.minExtend.x, roomParameters.minExtend.y, roomParameters.minExtend.z,
			roomParameters.maxExtend.x, roomParameters.maxExtend.y, roomParameters.maxExtend.z,
			roomCenter.x, roomCenter.y, roomCenter.z, roomSize.x, roomSize.y, roomSize.z,
			avgRoomSize, roomParameters.meanFreePath, roomParameters.echoDelay,
			roomParameters.roomVolume, roomParameters.roomSurface, roomParameters.roomAbsorptionMedium,
			roomParameters.sabineLow, roomParameters.sabineMedium, roomParameters.sabineHigh,
			roomParameters.reverberationTimeLow, roomParameters.reverberationTimeMedium,
			roomParameters.reverberationTimeHigh, roomParameters.separationTimeFirstLateReflection,
			soundRayList.GetRayCount(), soundRayList.GetSegmentCount(), timeCalcMS);
	}
	
	#ifdef WOVRAYHITSELEMENT_DO_TIMING
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		const int count = wovTimingTasks.GetCount();
		int i;
		for(i=0; i<count; i++){
			const deoalWOVRayHitsElement &wovrhe =
				((deoalRTPTTraceSoundRays*)wovTimingTasks.GetAt(i))->wovRayHitsElement();
			pAudioThread.GetLogger().LogInfoFormat(
				"WOVRHE(%03d): all=%.3f comp=%.3f(%d) compBVH=%.3f(%d) face=%.3f(%d) rayAdd=%d rayFound=%d", i,
				wovrhe.timingAll*1000, wovrhe.timingComponent*1000, wovrhe.timingComponentCount,
				wovrhe.timingComponentOctree*1000, wovrhe.timingComponentOctreeCount,
				wovrhe.timingFaces*1000, wovrhe.timingFacesCount, wovrhe.rayAddCount, wovrhe.rayFoundCount);
		}
	}
	#endif
	#ifdef RTWOVRAYHITSELEMENT_DO_TIMING
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		const int count = wovTimingTasks.GetCount();
		int i;
		for(i=0; i<count; i++){
			const deoalRTWOVRayHitsElement &rtwovrhe =
				((deoalRTPTTraceSoundRays*)wovTimingTasks.GetAt(i))->rtwovRayHitsElement();
			pAudioThread.GetLogger().LogInfoFormat(
				"RTWOVRHE(%03d): all=%.3f(%d) comp=%.3f(%d) compBVH=%.3f(%d) face=%.3f(%d)", i,
				rtwovrhe.timingAll*1e3f, rtwovrhe.timingAllCount, rtwovrhe.timingComponent*1e3f,
				rtwovrhe.timingComponentCount, rtwovrhe.timingComponentOctree*1e3f,
				rtwovrhe.timingComponentOctreeCount, rtwovrhe.timingFaces*1e3f, rtwovrhe.timingFacesCount);
		}
	}
	#endif
	#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		const int count = wovTimingTasks.GetCount();
		int i;
		for(i=0; i<count; i++){
			const deoalRTWOVRayHitsClosest &rtwovrhc =
				((deoalRTPTTraceSoundRays*)wovTimingTasks.GetAt(i))->rtwovRayHitsClosest();
			pAudioThread.GetLogger().LogInfoFormat(
				"RTWOVRHC(%03d): all=%.3f(%d) comp=%.3f(%d) compBVH=%.3f(%d) face=%.3f(%d)", i,
				rtwovrhc.timingAll*1e3f, rtwovrhc.timingAllCount, rtwovrhc.timingComponent*1e3f,
				rtwovrhc.timingComponentCount, rtwovrhc.timingComponentOctree*1e3f,
				rtwovrhc.timingComponentOctreeCount, rtwovrhc.timingFaces*1e3f, rtwovrhc.timingFacesCount);
		}
	}
	#endif
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		const int count = wovTimingTasks.GetCount();
		int i;
		for(i=0; i<count; i++){
			const deoalRTWOVRayBlocked &rtwovrb =
				((deoalRTPTTraceSoundRays*)wovTimingTasks.GetAt(i))->rtwovRayBlocked();
			pAudioThread.GetLogger().LogInfoFormat(
				"RTWOVRB(%03d): all=%.3f(%d) comp=%.3f(%d) compBVH=%.3f(%d) face=%.3f(%d)", i,
				rtwovrb.timingAll*1e3f, rtwovrb.timingAllCount, rtwovrb.timingComponent*1e3f,
				rtwovrb.timingComponentCount, rtwovrb.timingComponentOctree*1e3f,
				rtwovrb.timingComponentOctreeCount, rtwovrb.timingFaces*1e3f, rtwovrb.timingFacesCount);
		}
	}
	#endif
}

// void deoalRTParallelEnvProbe::TraceSoundRays( sRoomParameters &roomParameters,
// deoalSoundRayList &soundRayList, const decDVector &position, float range,
// deoalAWorld &world, const decLayerMask &layerMask,
// const deoalATRayTracing::sConfigSoundTracing &config ){
// 	TraceSoundRays( roomParameters, soundRayList, position,
// 		range, 1.0f, 0.0f, world, layerMask, config );
// }

void deoalRTParallelEnvProbe::Listen(const deoalEnvProbe &sourceProbe,
const deoalEnvProbe *listenProbe, deoalEnvProbeListener &listener, deoalAWorld &world,
deoalRTWorldBVH *rtWorldBVH, const decLayerMask &layerMask, const decDVector &position){
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		if(listenProbe){
			pAudioThread.GetLogger().LogInfoFormat(
				"Parallel-CalcProbeListener: pos=(%.3f,%.3f,%.3f) source=(%.3f,%.3f,%.3f) rays=%d segments=%d",
				position.x, position.y, position.z, sourceProbe.GetPosition().x,
				sourceProbe.GetPosition().y, sourceProbe.GetPosition().z,
				listenProbe->GetSoundRayList().GetRayCount(),
				listenProbe->GetSoundRayList().GetSegmentCount());
			
		}else{
			pAudioThread.GetLogger().LogInfoFormat(
				"Parallel-CalcProbeListener: pos=(%.3f,%.3f,%.3f) source=(%.3f,%.3f,%.3f) rays=%d segments=%d",
				position.x, position.y, position.z, sourceProbe.GetPosition().x,
				sourceProbe.GetPosition().y, sourceProbe.GetPosition().z,
				sourceProbe.GetSoundRayList().GetRayCount(),
				sourceProbe.GetSoundRayList().GetSegmentCount());
		}
	}
	
	pTimer.Reset();
	
	if(!pRunListenUsingTasks(sourceProbe, listenProbe, listener, world, rtWorldBVH, layerMask, position)){
		return; // cancelled
	}
	
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	const decPointerList wovTimingTasks(pTasksWaitListen);
	#endif
	
	const float elapsed = pTimer.GetElapsedTime();
	pTimeHistoryListen.Add(elapsed);
	pCounterListen++;
	pElapsedRTTime += elapsed;
	
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		const float timeCalcMS = elapsed * 1e3f;
		
		pAudioThread.GetLogger().LogInfoFormat(
			"Result-CalcProbeListener: frDelay=%.3f revDelay=%.3f finalFRGain=(%.3f,%.3f,%.3f)"
			" finalLRGain=(%.3f,%.3f,%.3f) frPan=(%.3f,%.3f,%.3f)[%.3f]"
			" lrPan=(%.3f,%.3f,%.3f)[%.3f] time=%.2fms",
			listener.GetReflectionDelay(), listener.GetReverberationDelay(),
			listener.GetReflectedLow(), listener.GetReflectedMedium(), listener.GetReflectedHigh(),
			listener.GetReverberationGainLow(), listener.GetReverberationGainMedium(),
			listener.GetReverberationGainHigh(), listener.GetReflectionPan().x,
			listener.GetReflectionPan().y, listener.GetReflectionPan().z,
			listener.GetReflectionPan().Length(), listener.GetReverberationPan().x,
			listener.GetReverberationPan().y, listener.GetReverberationPan().z,
			listener.GetReverberationPan().Length(), timeCalcMS);
		/*
		if(frPanDirection.Length() > 0.001f) frPanDirection.Normalize();
		if(lrPanDirection.Length() > 0.001f) lrPanDirection.Normalize();
		pAudioThread.GetLogger().LogInfoFormat(
			"NOR frPan=(%.3f,%.3f,%.3f) lrPan=(%.3f,%.3f,%.3f)",
			frPanDirection.x, frPanDirection.y, frPanDirection.z,
			lrPanDirection.x, lrPanDirection.y, lrPanDirection.z);
		*/
	}
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		const int count = wovTimingTasks.GetCount();
		int i;
		for(i=0; i<count; i++){
			const deoalRTWOVRayBlocked &rtwovrb =
				((deoalRTPTListen*)wovTimingTasks.GetAt(i))->rtwovRayBlocked();
			pAudioThread.GetLogger().LogInfoFormat(
				"RTWOVRB(%03d): all=%.3f(%d) comp=%.3f(%d) compBVH=%.3f(%d) face=%.3f(%d)", i,
				rtwovrb.timingAll*1e3f, rtwovrb.timingAllCount, rtwovrb.timingComponent*1e3f,
				rtwovrb.timingComponentCount, rtwovrb.timingComponentOctree*1e3f,
				rtwovrb.timingComponentOctreeCount, rtwovrb.timingFaces*1e3f, rtwovrb.timingFacesCount);
		}
	}
	#endif
}

#if 0
void deoalRTParallelEnvProbe::CalcProbeFull(deoalEnvProbe &envProbe, deoalAWorld &world,
const deoalRayTraceConfig &probeConfig, const decDVector &listenPosition){
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		pAudioThread.GetLogger().LogInfoFormat(
			"Parallel-CalcProbeFull: pos=(%.3f,%.3f,%.3f) range=%.3f listen=(%.3f,%.3f,%.3f) rays=%d",
			envProbe.GetPosition().x, envProbe.GetPosition().y, envProbe.GetPosition().z,
			envProbe.GetRange(), listenPosition.x, listenPosition.y, listenPosition.z,
			probeConfig.GetRayCount());
		pDebugTimer.Reset();
	}
	
	pStartTasksFull(envProbe, world, probeConfig, listenPosition);
	pAudioThread.GetOal().GetGameEngine()->GetParallelProcessing().WaitForTasks(
		(deParallelTask**)pTasksFull, pTaskFullCount);
	pProcessResultsFull(probeConfig);
}
#endif

void deoalRTParallelEnvProbe::EstimateRoomParameters(sRoomParameters &roomParameters,
const decDVector &position, float range, deoalAWorld &world,
const decLayerMask &layerMask, const deoalRayTraceConfig &probeConfig){
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		pAudioThread.GetLogger().LogInfoFormat(
			"Parallel-RoomEstimate: pos=(%.3f,%.3f,%.3f) range=%.3f rays=%d",
			position.x, position.y, position.z, range, probeConfig.GetRayCount());
	}
	
	pTimer.Reset();
	
	pRunRoomEstimateUsingTasks(roomParameters, position, range, world, layerMask, probeConfig);
	
	const float elapsed = pTimer.GetElapsedTime();
	pTimeHistoryEstimateRoom.Add(elapsed);
	pCounterEstimateRoom++;
	pElapsedRTTime += elapsed;
	
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		const float timeCalcMS = elapsed * 1e3f;
		
		pAudioThread.GetLogger().LogInfoFormat(
			"Result-RoomEstimate: minExt=(%.3f,%.3f,%.3f) maxExt=(%.3f,%.3f,%.3f) "
			"volume=%.3f surface=%.3f sabine=(%.3f,%.3f,%.3f) mfp=%.3f sepTFLR=%.3f "
			"roomAbs=(%.3f,%.3f,%.3f) revTime=(%.3f,%.3f,%.3f) echoDelay=%.3f time=%.2fms",
			roomParameters.minExtend.x, roomParameters.minExtend.y, roomParameters.minExtend.z,
			roomParameters.maxExtend.x, roomParameters.maxExtend.y, roomParameters.maxExtend.z,
			roomParameters.roomVolume, roomParameters.roomSurface, roomParameters.sabineLow,
			roomParameters.sabineMedium, roomParameters.sabineHigh, roomParameters.meanFreePath,
			roomParameters.separationTimeFirstLateReflection, roomParameters.roomAbsorptionLow,
			roomParameters.roomAbsorptionMedium, roomParameters.roomAbsorptionHigh,
			roomParameters.reverberationTimeLow, roomParameters.reverberationTimeMedium,
			roomParameters.reverberationTimeHigh, roomParameters.echoDelay, timeCalcMS);
	}
}



void deoalRTParallelEnvProbe::ResetCounters(){
	pCounterTraceSoundRays = 0;
	pCounterEstimateRoom = 0;
	pCounterListen = 0;
}

void deoalRTParallelEnvProbe::ResetElapsedRTTime(){
	pElapsedRTTime = 0.0f;
}



void deoalRTParallelEnvProbe::FinishTaskFinished(deParallelTask *task){
	if(!pAudioThread.GetAsyncAudio()){
		return;
	}
	
	{
	const deMutexGuard lock(pMutex);
	if(pBarrierTask != task){
		return;
	}
	}
	
	pBarrier.Wait();
}



/*
void deoalRTParallelEnvProbe::Enable(deoalRTPTTraceSoundRays *task){
	deMutexGuard lock(pMutex);
	
	const int index = pTasksRunningTraceSoundRays.IndexOf(task);
	if(index != -1){
		pTasksReadyTraceSoundRays.Add(task);
		pTasksRunningTraceSoundRays.RemoveFrom(index);
	}
}
*/

void deoalRTParallelEnvProbe::Enable(deoalRTPTTraceSoundRaysFinish *task){
	deMutexGuard lock(pMutex);
	
	// enable trace tasks then remove them all from the finish task
	const int count = task->GetTraceTasks().GetCount();
	int i;
	for(i=0; i<count; i++){
		deoalRTPTTraceSoundRays * const ttask = (deoalRTPTTraceSoundRays*)task->GetTraceTasks().GetAt(i);
		const int index = pTasksRunningTraceSoundRays.IndexOf(ttask);
		if(index != -1){
			pTasksRunningTraceSoundRays.RemoveFrom(index);
			pTasksReadyTraceSoundRays.Add(ttask);
		}
	}
	task->GetTraceTasks().RemoveAll();
	
	// enable finish task
	const int index = pTasksRunningTraceSoundRaysFinish.IndexOf(task);
	if(index != -1){
		pTasksRunningTraceSoundRaysFinish.RemoveFrom(index);
		pTasksReadyTraceSoundRaysFinish.Add(task);
	}
}

/*
void deoalRTParallelEnvProbe::Enable(deoalRTPTRoomEstimate *task){
	deMutexGuard lock(pMutex);
	
	const int index = pTasksRunningRoomEstimate.IndexOf(task);
	if(index != -1){
		pTasksReadyRoomEstimate.Add(task);
		pTasksRunningRoomEstimate.RemoveFrom(index);
	}
}
*/

void deoalRTParallelEnvProbe::Enable(deoalRTPTRoomEstimateFinish *task){
	deMutexGuard lock(pMutex);
	
	// enable estimate tasks then remove them all from the finish task
	const int count = task->GetEstimateTasks().GetCount();
	int i;
	for(i=0; i<count; i++){
		deoalRTPTRoomEstimateFinish * const etask = (deoalRTPTRoomEstimateFinish*)task->GetEstimateTasks().GetAt(i);
		const int index = pTasksRunningRoomEstimate.IndexOf(etask);
		if(index != -1){
			pTasksRunningRoomEstimate.RemoveFrom(index);
			pTasksReadyRoomEstimate.Add(etask);
		}
	}
	task->GetEstimateTasks().RemoveAll();
	
	// enable finish task
	const int index = pTasksRunningRoomEstimateFinish.IndexOf(task);
	if(index != -1){
		pTasksRunningRoomEstimateFinish.RemoveFrom(index);
		pTasksReadyRoomEstimateFinish.Add(task);
	}
}

/*
void deoalRTParallelEnvProbe::Enable(deoalRTPTListen *task){
	deMutexGuard lock(pMutex);
	
	const int index = pTasksRunningListen.IndexOf(task);
	if(index != -1){
		pTasksReadyListen.Add(task);
		pTasksRunningListen.RemoveFrom(index);
	}
}
*/

void deoalRTParallelEnvProbe::Enable(deoalRTPTListenFinish *task){
	const deMutexGuard lock(pMutex);
	
	// enable listen tasks then remove them all from the finish task
	const int count = task->GetListenTasks().GetCount();
	int i;
	for(i=0; i<count; i++){
		deoalRTPTListen * const ltask = (deoalRTPTListen*)task->GetListenTasks().GetAt(i);
		const int index = pTasksRunningListen.IndexOf(ltask);
		if(index != -1){
			pTasksRunningListen.RemoveFrom(index);
			pTasksReadyListen.Add(ltask);
		}
	}
	task->GetListenTasks().RemoveAll();
	
	// enable finish task
	const int index = pTasksRunningListenFinish.IndexOf(task);
	if(index != -1){
		pTasksRunningListenFinish.RemoveFrom(index);
		pTasksReadyListenFinish.Add(task);
	}
}



// Private Functions
//////////////////////

void deoalRTParallelEnvProbe::pRunTraceSoundRaysUsingTasks(sRoomParameters &roomParameters,
deoalSoundRayList &soundRayList, const decDVector &position, float range, float refDist,
float rollOff, float distanceOffset, deoalAWorld &world, deoalRTWorldBVH *rtWorldBVH,
const decLayerMask &layerMask, const deoalATRayTracing::sConfigSoundTracing &config){
	deMutexGuard lock(pMutex);
	DEASSERT_NULL(pBarrierTask)
	
// 	#ifdef WOVRAYHITSELEMENT_DO_TIMING
// 	deoalRTPTTraceSoundRays &task = *( ( deoalRTPTTraceSoundRays* )
// 		pTasksReadyTraceSoundRays.GetAt( pTasksReadyTraceSoundRays.GetCount() - 1 ) );
// 	pTasksWaitTraceSoundRays.Add( &task );
// 	
// 	task.SetWorld( &world );
// 	task.SetPosition( position );
// 	task.SetRange( range );
// 	task.SetAttenuationParameters( refDist, rollOff, distanceOffset );
// 	task.SetProbeConfig( config.rtConfig );
// 	task.SetAddRayMinLength( config.addRayMinLength );
// 	task.SetFirstRay( 0 );
// 	task.SetRayCount( config.rtConfig->GetRayCount() );
// 	task.SetInverseRayTracing( config.inverseRayTracing );
// 	
// 	pAddTasks( pAudioThread.GetOal().GetGameEngine()->GetParallelProcessing(),
// 		pTasksReadyTraceSoundRays, 1, pTasksRunningTraceSoundRays );
// 	task.wovRayHitsElement().StartTiming();
// 	return 1;
// 	#endif
	
	// prepare tracing tasks
	#ifdef RTPTTSR_ONE_TASK_PER_RAY
	const int taskCount = config.rtConfig->GetRayCount();
	#else
	const int taskCount = 8;
	#endif
	
	if(taskCount > pTasksReadyTraceSoundRays.GetCount()){
		while(pTasksReadyTraceSoundRays.GetCount() < taskCount){
			const deoalRTPTTraceSoundRays::Ref task(deoalRTPTTraceSoundRays::Ref::NewWith(*this));
			pTasksTraceSoundRays.Add(task);
			pTasksReadyTraceSoundRays.Add(task);
		}
	}
	
	#ifndef RTPTTSR_ONE_TASK_PER_RAY
	const int raysPerTask = probeConfig.GetRayCount() / taskCount;
	int firstRay = probeConfig.GetRayCount() - raysPerTask * (taskCount - 1);
	#endif
	int i;
	
	pTasksWaitTraceSoundRays.RemoveAll();
	
	const int firstIndex = pTasksReadyTraceSoundRays.GetCount() - 1;
	for(i=0; i<taskCount; i++){
		deoalRTPTTraceSoundRays &task = *((deoalRTPTTraceSoundRays*)
			pTasksReadyTraceSoundRays.GetAt(firstIndex - i));
		pTasksWaitTraceSoundRays.Add(&task);
		
		task.SetWorld(&world, rtWorldBVH);
		task.SetPosition(position);
		task.SetRange(range);
		task.SetAttenuationParameters(refDist, rollOff, distanceOffset);
		task.SetProbeConfig(config.rtConfig);
		task.SetAddRayMinLength(config.addRayMinLength);
		task.SetMaxBounceCount(config.maxBounceCount);
		task.SetMaxTransmitCount(config.maxTransmitCount);
		task.SetThresholdReflect(config.thresholdReflect);
		task.SetThresholdTransmit(config.thresholdTransmit);
		task.SetLayerMask(layerMask);
		task.SetInverseRayTracing(config.inverseRayTracing);
		
		#ifdef RTPTTSR_ONE_TASK_PER_RAY
		task.SetFirstRay(i);
		#else
		if(i == 0){
			task.SetFirstRay(0);
			task.SetRayCount(firstRay);
			
		}else{
			task.SetFirstRay(firstRay);
			task.SetRayCount(raysPerTask);
			firstRay += raysPerTask;
		}
		#endif
	}
	
	// prepare finish task
	if(pTasksReadyTraceSoundRaysFinish.GetCount() == 0){
		const deoalRTPTTraceSoundRaysFinish::Ref task(deoalRTPTTraceSoundRaysFinish::Ref::NewWith(*this));
		pTasksTraceSoundRaysFinish.Add(task);
		pTasksReadyTraceSoundRaysFinish.Add(task);
	}
	
	deParallelProcessing &parallel = pAudioThread.GetOal().GetGameEngine()->GetParallelProcessing();
	deoalRTPTTraceSoundRaysFinish &task = *((deoalRTPTTraceSoundRaysFinish*)
		pTasksReadyTraceSoundRaysFinish.GetAt(pTasksReadyTraceSoundRaysFinish.GetCount() - 1));
	pBarrierTask = &task;
	
	try{
		task.SetRange(range);
		task.SetProbeConfig(config.rtConfig);
		task.SetSoundRayList(&soundRayList);
		task.SetRoomParameters(&roomParameters);
		task.AddDependencies(pTasksWaitTraceSoundRays);
		
		// AddDependsOn is not thread-safe if the tasks has been added to parallel-processing already
		pAddTasks(parallel, pTasksReadyTraceSoundRays, taskCount, pTasksRunningTraceSoundRays);
		pAddTasks(parallel, pTasksReadyTraceSoundRaysFinish, 1, pTasksRunningTraceSoundRaysFinish);
		
	}catch(const deException &){
		// do NOT call RemoveAllDependsOn! we are asynchronous and the task could have started!
		pBarrierTask = nullptr;
		for(i=0; i<pTasksRunningTraceSoundRays.GetCount(); i++){
			((deParallelTask*)pTasksRunningTraceSoundRays.GetAt(i))->Cancel();
		}
		task.Cancel();
		throw;
	}
	
	// wait for finish task to finish
	lock.Unlock();
	pWaitForFinishTask(parallel, &task);
	if(task.IsCancelled()){
		DETHROW(deeInvalidParam);
	}
}



bool deoalRTParallelEnvProbe::pRunListenUsingTasks(const deoalEnvProbe &sourceProbe,
const deoalEnvProbe *listenProbe, deoalEnvProbeListener &listener, deoalAWorld &world,
deoalRTWorldBVH *rtWorldBVH, const decLayerMask &layerMask, const decDVector &position){
	deMutexGuard lock(pMutex);
	DEASSERT_NULL(pBarrierTask)
	
	// prepare listen tasks
	const deoalSoundRayList &soundRayList = listenProbe
		? listenProbe->GetSoundRayList() : sourceProbe.GetSoundRayList();
	
	#ifdef RTPTL_ONE_TASK_PER_RAY
	const int taskCount = soundRayList.GetRayCount();
	#else
	const int taskCount = 8;
	#endif
	
	if(taskCount > pTasksReadyListen.GetCount()){
		while(pTasksReadyListen.GetCount() < taskCount){
			const deoalRTPTListen::Ref task(deoalRTPTListen::Ref::NewWith(*this));
			pTasksListen.Add(task);
			pTasksReadyListen.Add(task);
		}
	}
	
	#ifndef RTPTL_ONE_TASK_PER_RAY
	const int raysPerTask = soundRayList.GetRayCount() / taskCount;
	int firstRay = soundRayList.GetRayCount() - raysPerTask * (taskCount - 1);
	#endif
	int i;
	
	pTasksWaitListen.RemoveAll();
	
	const int firstIndex = pTasksReadyListen.GetCount() - 1;
	for(i=0; i<taskCount; i++){
		deoalRTPTListen &task = *((deoalRTPTListen*)pTasksReadyListen.GetAt(firstIndex - i));
		pTasksWaitListen.Add(&task);
		
		task.SetWorld(&world, rtWorldBVH);
		task.SetListenPosition(position);
		task.SetSourceProbe(&sourceProbe);
		task.SetListenProbe(listenProbe);
		task.SetLayerMask(layerMask);
		
		#ifdef RTPTL_ONE_TASK_PER_RAY
		task.SetFirstRay(i);
		#else
		if(i == 0){
			task.SetFirstRay(0);
			task.SetRayCount(firstRay);
			
		}else{
			task.SetFirstRay(firstRay);
			task.SetRayCount(raysPerTask);
			firstRay += raysPerTask;
		}
		#endif
	}
	
	// prepare finish task
	if(pTasksReadyListenFinish.GetCount() == 0){
		const deoalRTPTListenFinish::Ref task(deoalRTPTListenFinish::Ref::NewWith(*this));
		pTasksListenFinish.Add(task);
		pTasksReadyListenFinish.Add(task);
	}
	
	deParallelProcessing &parallel = pAudioThread.GetOal().GetGameEngine()->GetParallelProcessing();
	deoalRTPTListenFinish &task = *((deoalRTPTListenFinish*)
		pTasksReadyListenFinish.GetAt(pTasksReadyListenFinish.GetCount() - 1));
	pBarrierTask = &task;
	
	try{
		task.SetProbes(&sourceProbe, listenProbe);
		task.SetPosition(position);
		task.SetListener(&listener);
		task.AddDependencies(pTasksWaitListen);
		
		// AddDependsOn is not thread-safe if the tasks has been added to parallel-processing already
		pAddTasks(parallel, pTasksReadyListen, taskCount, pTasksRunningListen);
		pAddTasks(parallel, pTasksReadyListenFinish, 1, pTasksRunningListenFinish);
		
	}catch(const deException &){
		// do NOT call RemoveAllDependsOn! we are asynchronous and the task could have started!
		pBarrierTask = nullptr;
		for(i=0; i<pTasksRunningListen.GetCount(); i++){
			((deParallelTask*)pTasksRunningListen.GetAt(i))->Cancel();
		}
		task.Cancel();
		throw;
	}
	
	// wait for finish task to finish
	lock.Unlock();
	pWaitForFinishTask(parallel, &task);
	return !task.IsCancelled();
}



#if 0
void deoalRTParallelEnvProbe::pStartTasksFull(deoalEnvProbe &envProbe, deoalAWorld &world,
const deoalRayTraceConfig &probeConfig, const decDVector &listenPosition){
	deMutexGuard lock(pMutex);
	
	deParallelProcessing &parallel = pAudioThread.GetOal().GetGameEngine()->GetParallelProcessing();
	const int raysPerTask = probeConfig.GetRayCount() / pTaskFullCount;
	int firstRay = probeConfig.GetRayCount() - raysPerTask * (pTaskFullCount - 1);
	int i;
	
	const int firstIndex = pTasksFull.GetCount() - 1;
	for(i=0; i<taskCount; i++){
		deoalRTPTEnvProbeFull &task = *((deoalRTPTEnvProbeFull*)pTasksFull.GetAt(firstIndex - i));
		task.SetWorld(&world);
		task.SetEnvProbe(&envProbe);
		task.SetProbeConfig(&probeConfig);
		task.SetListenerPosition(listenPosition);
		
		if(i == 0){
			task.SetFirstRay(0);
			task.SetRayCount(firstRay);
			
		}else{
			task.SetFirstRay(firstRay);
			task.SetRayCount(raysPerTask);
			firstRay += raysPerTask;
		}
	}
	
	pAddTasks(parallel, pTasksFull, taskCount, pTasksRunningFull);
}

void deoalRTParallelEnvProbe::pProcessResultsFull(const deoalRayTraceConfig &probeConfig){
	// collect and apply parameters from tasks
	decDVector roomCenter, minExtend, maxExtend;
	float gainLow = 0.0f;
	float gainMedium = 0.0f;
	float gainHigh = 0.0f;
	float delay = 0.0f;
	int i;
	
	// collect data from tasks
	for(i=0; i<pTaskFullCount; i++){
		const deoalRTPTEnvProbeFull &task = *pTasksFull[i];
		
		if(task.IsCancelled()){
			DETHROW(deeInvalidAction); // task failed
		}
		
		if(i == 0){
			minExtend = task.GetMinExtend();
			maxExtend = task.GetMaxExtend();
			
		}else{
			minExtend.SetSmallest(task.GetMinExtend());
			maxExtend.SetLargest(task.GetMaxExtend());
		}
		
		gainLow += task.GetGainLow();
		gainMedium += task.GetGainMedium();
		gainHigh += task.GetGainHigh();
		delay += task.GetDelay();
	}
	
	// calculate room parameters independent of listener position
	roomCenter = (minExtend + maxExtend) / 2.0;
	
	const int rayCount = probeConfig.GetRayCount();
	const float invRayCount = 1.0f / (float)rayCount;
	gainLow *= invRayCount;
	gainMedium *= invRayCount;
	gainHigh *= invRayCount;
	delay *= invRayCount;
	
	// update environment probe with found parameters
// 	envProbe.SetExtends( minExtend, maxExtend );
// 	envProbe.SetRoomCenter( roomCenter );
// 	envProbe.SetReverberation( reverberationGain, reverberationTimeLow,
// 		reverberationTimeMedium, reverberationTimeHigh );
// 	envProbe.SetRoom( roomVolume, roomSurface, roomSabineLow, roomSabineMedium, roomSabineHigh );
// 	envProbe.SetRayCount( rayCount );
	
	if(pAudioThread.GetDebug().GetLogCalcEnvProbe()){
		const float timeCalcMS = pDebugTimer.GetElapsedTime() * 1e3f;
		
		const decDVector roomSize(maxExtend - minExtend);
		const double avgRoomSize = (roomSize.x + roomSize.y + roomSize.z) / 3.0;
		
		pAudioThread.GetLogger().LogInfoFormat(
			"Result-Full: minExt=(%.3f,%.3f,%.3f) maxExt=(%.3f,%.3f,%.3f) "
			"center=(%.3f,%.3f,%.3f) size=(%.3f,%.3f,%.3f) avgSize=%.3f "
			"gain=(%.3f,%.3f,%.3f) delay=%.3f time=%.2fms",
			minExtend.x, minExtend.y, minExtend.z, maxExtend.x, maxExtend.y, maxExtend.z,
			roomCenter.x, roomCenter.y, roomCenter.z, roomSize.x, roomSize.y, roomSize.z,
			avgRoomSize, gainLow, gainMedium, gainHigh, delay, timeCalcMS);
	}
}
#endif



void deoalRTParallelEnvProbe::pRunRoomEstimateUsingTasks(sRoomParameters &roomParameters,
const decDVector &position, float range, deoalAWorld &world, const decLayerMask &layerMask,
const deoalRayTraceConfig &probeConfig){
	deMutexGuard lock(pMutex);
	DEASSERT_NULL(pBarrierTask)
	
	// prepare estimate tasks
	#ifdef RTPTRE_ONE_TASK_PER_RAY
	const int taskCount = probeConfig.GetRayCount();
	#else
	const int taskCount = 8;
	#endif
	
	if(taskCount > pTasksReadyRoomEstimate.GetCount()){
		while(pTasksReadyRoomEstimate.GetCount() < taskCount){
			const deoalRTPTRoomEstimate::Ref task(deoalRTPTRoomEstimate::Ref::NewWith(*this));
			pTasksRoomEstimate.Add(task);
			pTasksReadyRoomEstimate.Add(task);
		}
	}
	
	#ifndef RTPTRE_ONE_TASK_PER_RAY
	const int raysPerTask = probeConfig.GetRayCount() / taskCount;
	int firstRay = probeConfig.GetRayCount() - raysPerTask * (taskCount - 1);
	#endif
	int i;
	
	pTasksWaitRoomEstimate.RemoveAll();
	
	const int firstIndex = pTasksReadyRoomEstimate.GetCount() - 1;
	for(i=0; i<taskCount; i++){
		deoalRTPTRoomEstimate &task = *((deoalRTPTRoomEstimate*)
			pTasksReadyRoomEstimate.GetAt(firstIndex - i));
		pTasksWaitRoomEstimate.Add(&task);
		
		task.SetWorld(&world);
		task.SetProbeConfig(&probeConfig);
		task.SetPosition(position);
		task.SetRange(range);
		task.SetLayerMask(layerMask);
		
		#ifdef RTPTRE_ONE_TASK_PER_RAY
		task.SetFirstRay(i);
		#else
		if(i == 0){
			task.SetFirstRay(0);
			task.SetRayCount(firstRay);
			
		}else{
			task.SetFirstRay(firstRay);
			task.SetRayCount(raysPerTask);
			firstRay += raysPerTask;
		}
		#endif
	}
	
	// prepare finish task
	if(pTasksReadyRoomEstimateFinish.GetCount() == 0){
		const deoalRTPTRoomEstimateFinish::Ref task(deoalRTPTRoomEstimateFinish::Ref::NewWith(*this));
		pTasksRoomEstimateFinish.Add(task);
		pTasksReadyRoomEstimateFinish.Add(task);
	}
	
	deParallelProcessing &parallel = pAudioThread.GetOal().GetGameEngine()->GetParallelProcessing();
	deoalRTPTRoomEstimateFinish &task = *((deoalRTPTRoomEstimateFinish*)
		pTasksReadyRoomEstimateFinish.GetAt(pTasksReadyRoomEstimateFinish.GetCount() - 1));
	pBarrierTask = &task;
	
	try{
		task.SetRange(range);
		task.SetProbeConfig(&probeConfig);
		task.SetRoomParameters(&roomParameters);
		task.AddDependencies(pTasksWaitRoomEstimate);
		
		// AddDependsOn is not thread-safe if the tasks has been added to parallel-processing already
		pAddTasks(parallel, pTasksReadyRoomEstimate, taskCount, pTasksRunningRoomEstimate);
		pAddTasks(parallel, pTasksReadyRoomEstimateFinish, 1, pTasksRunningRoomEstimateFinish);
		
	}catch(const deException &){
		// do NOT call RemoveAllDependsOn! we are asynchronous and the task could have started!
		pBarrierTask = nullptr;
		for(i=0; i<pTasksRunningRoomEstimate.GetCount(); i++){
			((deParallelTask*)pTasksRunningRoomEstimate.GetAt(i))->Cancel();
		}
		task.Cancel();
		throw;
	}
	
	// wait for finish task to finish
	lock.Unlock();
	pWaitForFinishTask(parallel, &task);
	if(task.IsCancelled()){
		DETHROW(deeInvalidParam);
	}
}



void deoalRTParallelEnvProbe::pAddTasks(deParallelProcessing &parallel,
decPointerList &tasks, int count, decPointerList &running){
	const int firstRunning = running.GetCount();
	int i;
	
	try{
		for(i=0; i<count; i++){
			const int index = tasks.GetCount() - 1;
			deParallelTask * const task = (deParallelTask*)tasks.GetAt(index);
			tasks.RemoveFrom(index);
			running.Add(task);
			pAddTask(parallel, task);
		}
		
	}catch(const deException &){
		for(i=firstRunning; i<running.GetCount(); i++){
			((deParallelTask*)running.GetAt(i))->Cancel();
		}
		throw;
	}
}

void deoalRTParallelEnvProbe::pAddTask(deParallelProcessing &parallel, deParallelTask *task){
	if(pAudioThread.GetAsyncAudio()){
		parallel.AddTaskAsync(task);
		
	}else{
		parallel.AddTask(task);
	}
}

void deoalRTParallelEnvProbe::pWaitForFinishTask(deParallelProcessing &parallel, deParallelTask *task){
	if(pAudioThread.GetAsyncAudio()){
		pBarrier.Wait();
		{
		const deMutexGuard lock(pMutex);
		DEASSERT_TRUE(pBarrierTask == task)
		pBarrierTask = nullptr;
		}
		
	}else{
		parallel.WaitForTask(task);
	}
}
