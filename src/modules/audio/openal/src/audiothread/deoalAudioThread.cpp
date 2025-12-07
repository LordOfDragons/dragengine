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
#include <errno.h>

#include "deoalAudioThread.h"
#include "deoalDebugInfo.h"
#include "deoalATLogger.h"
#include "deoalATContext.h"
#include "deoalATDebug.h"
#include "deoalATDelayed.h"
#include "deoalATRayTracing.h"
#include "../deoalBasics.h"
#include "../deAudioOpenAL.h"
#include "../deoalCaches.h"
#include "../buffer/deoalSharedBufferList.h"
#include "../capabilities/deoalCapabilities.h"
#include "../component/deoalComponent.h"
#include "../effect/deoalEffectSlotManager.h"
#include "../effect/deoalSharedEffectSlotManager.h"
#include "../environment/raytrace/deoalRayTraceResult.h"
#include "../environment/raytrace/deoalRayTraceHitElementList.h"
#include "../environment/raytrace/deoalSoundRayInteractionList.h"
#include "../environment/raytrace/parallel/deoalRTParallelEnvProbe.h"
#include "../environment/raytrace/visitors/deoalWOVRayHitsElement.h"
#include "../environment/raytrace/visitors/deoalWOVCollectElements.h"
#include "../extensions/deoalExtensions.h"
#include "../microphone/deoalMicrophone.h"
#include "../microphone/deoalAMicrophone.h"
#include "../model/deoalModel.h"
#include "../skin/deoalSkin.h"
#include "../sound/deoalDecodeBuffer.h"
#include "../sound/deoalSound.h"
#include "../source/deoalSourceManager.h"
#include "../speaker/deoalSpeaker.h"
#include "../speaker/deoalSpeakerList.h"
#include "../world/deoalAWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/parallel/deParallelProcessing.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/threading/deMutexGuard.h>



// #define DEBUG_SYNC 1

#ifdef DEBUG_SYNC
	#define DEBUG_SYNC_RT_FAILURE printf("RT(%i): set failure to %d\n", __LINE__, pThreadFailure);
	#define DEBUG_SYNC_RT_WAIT(b) printf("RT(%i): wait on barrier %s\n", __LINE__, b);
	#define DEBUG_SYNC_RT_PASS(b) printf("RT(%i): passed barrier %s\n", __LINE__, b);
	#define DEBUG_SYNC_MT_STATE printf("MT(%i): state changed to %i\n", __LINE__, pThreadState);
	#define DEBUG_SYNC_MT_WAIT(b) printf("MT(%i): wait on barrier %s\n", __LINE__, b);
	#define DEBUG_SYNC_MT_PASS(b) printf("MT(%i): passed barrier %s\n", __LINE__, b);
#else
	#define DEBUG_SYNC_RT_FAILURE
	#define DEBUG_SYNC_RT_WAIT(b)
	#define DEBUG_SYNC_RT_PASS(b)
	#define DEBUG_SYNC_MT_STATE
	#define DEBUG_SYNC_MT_WAIT(b)
	#define DEBUG_SYNC_MT_PASS(b)
#endif



// Class deoalAudioThread
///////////////////////////

// Constructor, destructor
////////////////////////////

deoalAudioThread::deoalAudioThread(deAudioOpenAL &oal) :
pOal(oal),

pAsyncAudio(false),

pContext(nullptr),

pLeakTracker(*this),

pDebugInfo(nullptr),
pDebug(nullptr),
pSpeakerList(nullptr),
pLogger(nullptr),
pDelayed(nullptr),
pRayTracing(nullptr),

pCaches(nullptr),
pDecodeBuffer(nullptr),

pExtensions(nullptr),
pCapabilities(nullptr),
pEffectSlotManager(nullptr),
pSharedEffectSlotManager(nullptr),
pSourceManager(nullptr),
pSharedBufferList(nullptr),

pRTParallelEnvProbe(nullptr),
pRTResultDirect(nullptr),
pRTHitElementList(nullptr),
pSRInteractionList(nullptr),
pWOVRayHitsElement(nullptr),
pWOVCollectElements(nullptr),

pActiveMicrophone(nullptr),
pDeactiveMicrophone(nullptr),
pActiveWorld(nullptr),

pElapsed(0.0f),
pElapsedFull(0.0f),

// time history
pTimeHistoryMain(29, 2),
pTimeHistoryAudio(29, 2),
pTimeHistoryAudioEstimated(29, 2),
pTimeHistoryUpdate(29, 2),
pEstimatedAudioTime(0.0f),
pAccumulatedMainTime(0.0f),
pFrameTimeLimit(1.0f / 30.0f),
pReadyToWait(false),
pWaitSkipped(false),
pWaitSkippedElapsed(0.0f),

// thread control parameters
pThreadState(etsStopped),
pThreadFailure(false),
pBarrierSyncIn(2),
pBarrierSyncOut(2)
{
	try{
		pConfiguration = oal.GetConfiguration();
		pConfiguration.SetDirty(false);
		oal.GetConfiguration().SetDirty(false);
		pPrepareConfiguration();
		
		pAsyncAudio = oal.GetConfiguration().GetAsyncAudio();
		
		pLogger = new deoalATLogger(*this);
		
		#ifdef OS_BEOS
		SetName("OpenAL Audio");
		#endif
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoalAudioThread::~deoalAudioThread(){
	pCleanUp();
}



// Management
///////////////

void deoalAudioThread::SetAsyncAudio(bool asyncAudio){
	pAsyncAudio = asyncAudio;
}



void deoalAudioThread::Init(){
	pDebugInfo = new deoalDebugInfo(*this);
	
	if(pAsyncAudio){
		pLogger->LogInfoFormat("Init audio using asynchronous audio");
		
		Start();
		
		DEBUG_SYNC_MT_WAIT("out")
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_MT_PASS("out")
		
		pLogger->Synchronize();
		
		if(pThreadFailure){
			pThreadState = etsStopped;
			DEBUG_SYNC_MT_STATE
			WaitForExit();
			DETHROW(deeInvalidAction);
		}
		
		pThreadState = etsSynchronize;
		DEBUG_SYNC_MT_STATE
		
		pTimerMain.Reset();
		
	}else{
		pLogger->LogInfoFormat("Init audio using synchronous audio");
		
		OAL_INIT_THREAD_CHECK;
		pInitThreadPhase1();
	}
}

void deoalAudioThread::CleanUp(){
	if(pAsyncAudio){
		if(pThreadState == etsStopped){
			return;
		}
		
		WaitFinishAudio();
		
		pThreadState = etsCleaningUp;
		DEBUG_SYNC_MT_STATE
		DEBUG_SYNC_MT_WAIT("out")
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_MT_PASS("out")
		WaitForExit();
		
		pLogger->Synchronize();
		if(pThreadFailure){
			pThreadState = etsStopped;
			DEBUG_SYNC_MT_STATE
			Stop();
			DETHROW(deeInvalidAction);
		}
		
	}else{
		pCleanUpThread();
	}
	
	SetActiveMicrophone(nullptr);
	
		pDeactiveMicrophone = NULL;
	}
	
		pActiveWorld = NULL;
	}
	pProcessOnceWorld.RemoveAll();
	
	if(pDebugInfo){
		delete pDebugInfo;
		pDebugInfo = NULL;
	}
}

bool deoalAudioThread::MainThreadWaitFinishAudio(){
	if(pAsyncAudio){
		const float gameTime = pTimerMain.GetElapsedTime();
		pDebugInfo->StoreTimeThreadMain(gameTime);
		
		const deMutexGuard lock(pMutexShared); // due to pTimeHistoryMain, pEstimatedAudioTime,
		                                         // pReadyToWait
		pTimeHistoryMain.Add(gameTime);
		//pLogger->LogInfo( decString("TimeHistory Main: ") + pTimeHistoryMain.DebugPrint() );
		
		if(pTimeHistoryMain.HasMetrics()){
			pAccumulatedMainTime += gameTime;
			
			const float ratioTimes = pOal.GetConfiguration().GetAsyncAudioSkipSyncTimeRatio();
			const float remainingTime = pEstimatedAudioTime - pAccumulatedMainTime;
			const float estimatedGameTime = decMath::max(pTimeHistoryMain.GetAverage(), 0.001f);
				// stay above 1ms
			
			/*
			pLogger->LogInfoFormat("SkinSync: skip=%d: estimRenTime=%.1fms estimGameTime=%.1fms"
				" remainTime=%.1fms ratio=%.3f threshold=%f",
				remainingTime / estimatedGameTime >= ratioTimes, pEstimatedAudioTime * 1e3f,
				estimatedGameTime * 1e3f, remainingTime * 1e3f,
				remainingTime / estimatedGameTime, ratioTimes);
			*/
			
			if(remainingTime / estimatedGameTime >= ratioTimes && !pReadyToWait){
// 				pBarrierSyncOut.TryWait( 0 ); // sanity kick to ensure thread can never block forver
				return false; // enough time to do another game frame update
			}
		}
		
		// if audio thread has not signaled to be ready for waiting do another game frame
		// update. this avoids the main thread waiting for the audio thread when the audio
		// thread is taking longer than estimated. this happens because the audio thread
		// tries to minimize expensive tasks until required. this makes prediction less
		// accurate than graphic rendering for example. it is better to skip a synchronization
		// than having the main thread stutter on random waits
		if(!pReadyToWait){
// 			pBarrierSyncOut.TryWait( 0 ); // sanity kick to ensure thread can never block forver
			return false;
		}
		
		// not enough time for another game frame update. do synchronization
		pAccumulatedMainTime = 0.0f;
	}
	
	pDebugInfo->ResetTimersMainThread();
	
	WaitFinishAudio();
	
	pDebugInfo->StoreTimeThreadMainWaitFinish();
	
	return pThreadState == etsFinishedAudio;
}

void deoalAudioThread::WaitFinishAudio(){
	{
	const deMutexGuard lock(pMutexShared);
	if(!pReadyToWait){
		return; // true if audio thread is waiting on pBarrierSyncIn otherwise pBarrierSyncOut
	}
	}
	
	switch(pThreadState){
	case etsStopped:
	case etsFinishedAudio:
	case etsFrozen:
		return;
		
	default:
		break;
	}
	
	DEBUG_SYNC_MT_WAIT("in")
	pBarrierSyncIn.Wait();
	DEBUG_SYNC_MT_PASS("in")
	
	pThreadState = etsFinishedAudio;
	DEBUG_SYNC_MT_STATE
}

void deoalAudioThread::RetainResourceData(){
}

void deoalAudioThread::FinalizeAsyncResLoading(){
}

void deoalAudioThread::Synchronize(){
	pLogger->Synchronize();
	
	pSyncConfiguration();
	pDebug->Synchronize();
	
	pDebugInfo->StoreTimeThreadMainSynchronize();
	
	pDebugInfo->UpdateDebugInfo();
	
	{
	const deMutexGuard lock(pMutexShared); // due to pTimeHistoryUpdate
	pFPSRate = 0;
	if(pTimeHistoryUpdate.HasMetrics()){
		pFPSRate = (int)(1.0f / pTimeHistoryUpdate.GetAverage());
	}
	}
	
	if(pAsyncAudio){
		// begin audio next frame unless thread is not active
		if(pThreadState == etsStopped){
			return;
		}
		
		pThreadState = etsAudio;
		DEBUG_SYNC_MT_STATE
		
		DEBUG_SYNC_MT_WAIT("out");
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_MT_PASS("out");
		
		pTimerMain.Reset();
		
	}else{
		pProcessAudio();
	}
}

void deoalAudioThread::Freeze(){
	if(pThreadState != etsAudio){
		return;
	}
	
	WaitFinishAudio();
	// wait for all pending parallel tasks to finish running
	
	pThreadState = etsFrozen;
	DEBUG_SYNC_MT_STATE
}

void deoalAudioThread::Unfreeze(){
	if(pThreadState != etsFrozen){
		return;
	}
	
	pThreadState = etsSynchronize; // do not audio, go to synchronize in barrier again
	DEBUG_SYNC_MT_STATE
	
	DEBUG_SYNC_MT_WAIT("out");
	pBarrierSyncOut.Wait();
	DEBUG_SYNC_MT_PASS("out");
}



void deoalAudioThread::Run(){
	OAL_INIT_THREAD_CHECK;
	
	// initialize
	try{
		pInitThreadPhase1();
		pThreadFailure = false;
		DEBUG_SYNC_RT_FAILURE
		
	}catch(const deException &exception){
		pLogger->LogException(exception);
		
		pThreadFailure = true;
		DEBUG_SYNC_RT_FAILURE
		
		DEBUG_SYNC_RT_WAIT("out")
		pBarrierSyncOut.Wait();
		DEBUG_SYNC_RT_PASS("out")
		
		try{
			pCleanUpThread();
			
		}catch(const deException &exception2){
			pLogger->LogException(exception2);
		}
		
		return;
	}
	
	DEBUG_SYNC_RT_WAIT("out")
	pBarrierSyncOut.Wait();
	DEBUG_SYNC_RT_PASS("out")
	
	// maximum timeout used for skipping waiting on the barrier to avoid buffer underruns.
	// 
	// for sound streaming 1s is buffered using 5 chunks of 200ms length. this is resiliant
	// to a longer delay in the main thread. the same applies to video streaming.
	// 
	// for synthesizer streaming 100ms is buffered using 2 50ms buffers. here short buffer
	// time is required to ensure quick reaction to changing synthesizer controllers.
	// this processing is resiliant only to short delays.
	// 
	// the limiting factor is thus synthesizer processing. if we want to avoid buffer
	// underruns in those we have to refill buffers before 100ms finishes. 75ms should be
	// enough but to be on the safe side 50ms is used. this equals a frame rate of 20Hz.
	// the main thread should never get this slow so 50ms is a good value to use.
	const int maxSyncSkipDelay = 50;
	
	// audio loop
	pWaitSkipped = false;
	pWaitSkippedElapsed = 0.0f;
	
	pTimerElapsed.Reset();
	
	while(true){
		// wait for entering synchronize. we use a timeout here to avoid a long delay by the
		// main thread causing the buffers to underrun.
		{
		const deMutexGuard lock(pMutexShared);
		pReadyToWait = true;
		}
		
		const int barrierTimeout = decMath::max(maxSyncSkipDelay - (int)(pElapsed * 1000.0f), 0);
		
		DEBUG_SYNC_RT_WAIT("in")
		if(pBarrierSyncIn.TryWait(barrierTimeout)){
			DEBUG_SYNC_RT_PASS("in")
			
			{
			const deMutexGuard lock(pMutexShared);
			pReadyToWait = false;
			}
			
			// main thread is messing with our state here. proceed to next barrier doing nothing
			// except alter the estimated render time. this value is used by the main thread
			// only outside the synchronization part so we can update it here
			{
			const deMutexGuard lock(pMutexShared); // due to pEstimatedAudioTime
			pEstimatedAudioTime = decMath::max(pTimeHistoryAudio.GetAverage(),
				pTimeHistoryAudioEstimated.GetAverage(), pFrameTimeLimit);
			}
			
			// wait for leaving synchronize
			DEBUG_SYNC_RT_WAIT("out")
			pBarrierSyncOut.Wait();
			DEBUG_SYNC_RT_PASS("out")
			
			if(pThreadState == etsCleaningUp){
				break;
				
			}else if(pThreadState == etsAudio){
				try{
					pRTParallelEnvProbe->ResetCounters();
					pRTParallelEnvProbe->ResetElapsedRTTime();
					pTimerAudio.Reset();
					
					pProcessAudio();
					pThreadFailure = false;
					DEBUG_SYNC_RT_FAILURE
					
					const float timeAudio = pTimerAudio.GetElapsedTime();
					pTimeHistoryAudio.Add(timeAudio);
					//pLogger->LogInfo( decString("TimeHistory Audio: ") + pTimeHistoryAudio.DebugPrint() );
					/*
					pLogger->LogInfoFormat("AudioThread Elapsed %dms (FPS %d)",
						(int)(elapsed * 1000.0f), (int)(1.0f / decMath::max(elapsed, 0.001f)));
					*/
					
					// we do not use directly the time history average value. this is because we
					// avoid time consuming processes like ray tracing as much as possible. this
					// means some elapsed audio processing times will be low while other in
					// between high. to counter this problem we also estimate an expected processing
					// time by using the timing data collected by the ray tracer.
					const float elapsedNonRTTime = decMath::max(0.0f,
						timeAudio - pRTParallelEnvProbe->GetElapsedRTTime());
					const int assumedRTTraceRaysCount = 1;
					const int assumedRTEstimateRoomCount = 0;
					const int assumedRTListenCount = 2;
					const float estimatedAvgRTTime =
						pRTParallelEnvProbe->GetTimeHistoryTraceSoundRays().GetAverage()
							* assumedRTTraceRaysCount
						+ pRTParallelEnvProbe->GetTimeHistoryEstimateRoom().GetAverage()
							* assumedRTEstimateRoomCount
						+ pRTParallelEnvProbe->GetTimeHistoryListen().GetAverage()
							* assumedRTListenCount;
					pTimeHistoryAudioEstimated.Add(elapsedNonRTTime + estimatedAvgRTTime);
					
					// apply frame limiter
					pLimitFrameRate(timeAudio);
					{
					const deMutexGuard lock(pMutexShared); // due to pTimeHistoryMain
					pDebugInfo->StoreTimeFrameLimiter(pTimeHistoryMain,
						pTimeHistoryAudio, pTimeHistoryAudioEstimated);
					}
					pThreadFailure = false;
					DEBUG_SYNC_RT_FAILURE
					
				}catch(const deException &exception){
					pLogger->LogException(exception);
					pTimerAudio.Reset();
					pThreadFailure = true;
					DEBUG_SYNC_RT_FAILURE
				}
			}
			
		}else{
			// waiting skipped to avoid buffer underruns. only fill up buffers avoiding
			// all processing which takes potentially a long time.
			// 
			// for this reason we do also not update the timings since this is no full
			// audio run.
			// 
			// also the pReadyToWait flag is not cleared so the main thread waits on
			// the barrier in case it arrives while we fill up buffers
			DEBUG_SYNC_RT_PASS("in")
			
			if(pThreadState == etsCleaningUp){
				break;
				
			}else if(pThreadState == etsAudio){
				try{
					pProcessAudioFast();
					pThreadFailure = false;
					DEBUG_SYNC_RT_FAILURE
					
				}catch(const deException &exception){
					pLogger->LogException(exception);
					pTimerAudio.Reset();
					pThreadFailure = true;
					DEBUG_SYNC_RT_FAILURE
				}
			}
		}
	}
	
	// clean up
	try{
		pCleanUpThread();
		pThreadFailure = false;
		DEBUG_SYNC_RT_FAILURE
		
	}catch(const deException &exception){
		pLogger->LogException(exception);
		pThreadFailure = true;
		DEBUG_SYNC_RT_FAILURE
	}
}



void deoalAudioThread::SetActiveMicrophone(deoalAMicrophone *microphone){
	// WARNING called from the main thread
	
		pDeactiveMicrophone = NULL;
	}
	
	if(microphone != pActiveMicrophone){
		if(pActiveMicrophone){
			pDeactiveMicrophone = pActiveMicrophone;
			pActiveMicrophone->SetActive(false);
		}
		
		pActiveMicrophone = microphone;
		
		if(microphone){
			microphone->AddReference();
			microphone->SetActive(true);
		}
	}
	
	deoalAWorld * const world = microphone ? microphone->GetParentWorld() : nullptr;
	if(world != pActiveWorld){
		if(pActiveWorld){
			pProcessOnceWorld.AddIfAbsent(pActiveWorld);
		}
		
		pActiveWorld = world;
		
		if(world){
			world->AddReference();
			pProcessOnceWorld.RemoveIfPresent(world);
		}
	}
}



// Private Functions
//////////////////////

void deoalAudioThread::pCleanUp(){
	if(pLogger){
		delete pLogger;
	}
}



void deoalAudioThread::pInitThreadPhase1(){
	// open device. this has to be done first
	pContext = new deoalATContext(*this);
	pContext->OpenDevice();
	
	// check device extensions. this has to be done after the device has been opened but
	// before a context has been created since context creations required this information
	pExtensions = new deoalExtensions(*this);
	pExtensions->ScanDeviceExtensions();
	
	// create context. this depends on the present device extensions
	pContext->CreateContext();
	
	// check context extensions. this has to be done after the context has been created
	pExtensions->ScanContextExtensions();
	pExtensions->PrintSummary();
	if(!pExtensions->VerifyPresence()){
		pLogger->LogError("Extension problems present");
		DETHROW(deeInvalidParam);
	}
	
	// detect capabilites. this has to be done after creating the context since
	// creating the context sets the actual capabilities
	pCapabilities = new deoalCapabilities(*this);
	pCapabilities->ReportCapabilities();
	
	// debug
	pContext->LogContextInfo();
	pDebug = new deoalATDebug(*this);
	pDelayed = new deoalATDelayed(*this);
	pRayTracing = new deoalATRayTracing(*this);
	
	// create working objects
	pCaches = new deoalCaches(*this);
	pEffectSlotManager = new deoalEffectSlotManager(*this);
	pSharedEffectSlotManager = new deoalSharedEffectSlotManager(*this);
	pSourceManager = new deoalSourceManager(*this);
	pSpeakerList = new deoalSpeakerList;
	pDecodeBuffer = new deoalDecodeBuffer((44100 / 10) * 4);
	pSharedBufferList = new deoalSharedBufferList;
	
	pRTParallelEnvProbe = new deoalRTParallelEnvProbe(*this);
	pRTResultDirect = new deoalRayTraceResult;
	pRTHitElementList = new deoalRayTraceHitElementList;
	pSRInteractionList = new deoalSoundRayInteractionList;
	pWOVRayHitsElement = new deoalWOVRayHitsElement;
	pWOVCollectElements = new deoalWOVCollectElements;
	
	// set default parameters
// 	alDistanceModel( AL_INVERSE_DISTANCE_CLAMPED );
	alDistanceModel(AL_NONE);
}

void deoalAudioThread::pCleanUpThread(){
	// NOTE this call is done in the audio thread but has also main thread access
	//      in certain places. this is allowed since the thread cleanup is only called
	//      during clean up time where the main thread is blocked waiting for the
	//      audio thread to finish. thus synchronizing is safe to be used here
	
	pLogger->Synchronize();
	
	SetActiveMicrophone(nullptr);
	
		pDeactiveMicrophone = NULL;
	}
	
		pActiveWorld = NULL;
	}
	pProcessOnceWorld.RemoveAll();
	
	if(pWOVCollectElements){
		delete pWOVCollectElements;
	}
	if(pWOVRayHitsElement){
		delete pWOVRayHitsElement;
	}
	if(pSRInteractionList){
		delete pSRInteractionList;
	}
	if(pRTHitElementList){
		delete pRTHitElementList;
	}
	if(pRTResultDirect){
		delete pRTResultDirect;
	}
	if(pRTParallelEnvProbe){
		delete pRTParallelEnvProbe;
	}
	
	if(pSharedEffectSlotManager){
		delete pSharedEffectSlotManager;
	}
	if(pSpeakerList){
		delete pSpeakerList;
	}
	if(pDecodeBuffer){
		delete pDecodeBuffer;
	}
	
	if(pRayTracing){
		delete pRayTracing;
	}
	if(pDelayed){
		delete pDelayed;
	}
	
	if(pSourceManager){
		delete pSourceManager;
	}
	if(pEffectSlotManager){
		delete pEffectSlotManager;
	}
	
	if(pSharedBufferList){
		delete pSharedBufferList;
	}
	if(pCaches){
		delete pCaches;
		pCaches = NULL;
	}
	
	if(pCapabilities){
		delete pCapabilities;
	}
	if(pExtensions){
		delete pExtensions;
	}
	
	if(pDebug){
		delete pDebug;
	}
	if(pContext){
		pContext->CleanUp();
		delete pContext;
	}
	
	pReportLeaks(); // only if enabled
	
	pLogger->Synchronize();
}

void deoalAudioThread::pLimitFrameRate(float elapsed){
	#ifdef OS_W32
	decTimer timer;
	timer.Reset();
	#endif
	
	while(elapsed < pFrameTimeLimit){
		// we have some spare time due to the frame limiter. we could do here
		// some optimization work to use the time for intelligent stuff.
		
		// right now we just sleep
		#ifdef OS_W32
		elapsed += timer.GetElapsedTime();
		
		#else
		timespec timeout, remaining;
		timeout.tv_sec = 0;
		timeout.tv_nsec = (long)((pFrameTimeLimit - elapsed) * 1e9f);
		while(nanosleep(&timeout, &remaining) == -1 && errno == EINTR){
			timeout = remaining;
		}
		break;
		#endif
	}
}

void deoalAudioThread::pSyncConfiguration(){
	deoalConfiguration &config = pOal.GetConfiguration();
	
	if(config.GetDirty()){
		pConfiguration = config;
		
		pConfiguration.SetDirty(false);
		config.SetDirty(false);
		
		pPrepareConfiguration();
		
	}else if(pConfiguration.GetDirty()){
		config.SetDirty(false);
		pConfiguration.SetDirty(false);
	}
}

void deoalAudioThread::pPrepareConfiguration(){
	if(pConfiguration.GetFrameRateLimit() == 0){
		pFrameTimeLimit = 0.0f;
		
	}else{
		pFrameTimeLimit = 1.0f / (float)pConfiguration.GetFrameRateLimit();
	}
	
	if(pRayTracing){ // is NULL during construction time only
		pRayTracing->ConfigurationChanged();
	}
}

void deoalAudioThread::pProcessAudio(){
	pDebugInfo->ResetTimersAudioThread();
	pDelayed->ProcessFreeOperations(false);
	
	pElapsedFull = pElapsed = pTimerElapsed.GetElapsedTime();
	
	if(pWaitSkipped){
		pElapsedFull += pWaitSkippedElapsed;
		// pLogger->LogInfoFormat( "ProcessAudio: skipped %.3f [%.3f, %.3f] (%.1f)", pElapsedFull, pWaitSkippedElapsed, pElapsed, 1.0f / pElapsed );
		pWaitSkippedElapsed = 0.0f;
		pWaitSkipped = false;
		
	}else{
		const deMutexGuard lock(pMutexShared); // due to pTimeHistoryUpdate
		pTimeHistoryUpdate.Add(pElapsed);
		// pLogger->LogInfoFormat( "ProcessAudio: %.3f (%.1f)", pElapsed, 1.0f / pElapsed );
	}
	
	while(pProcessOnceWorld.GetCount() > 0){
		deoalAWorld * const world = (deoalAWorld*)pProcessOnceWorld.GetAt(0);
		world->PrepareProcessAudio();
		pProcessOnceWorld.Remove(world);
	}
	
	if(pDeactiveMicrophone){
		pDeactiveMicrophone->ProcessDeactivate();
	}
	
	pEffectSlotManager->Update(pElapsedFull);
	pDebugInfo->StoreTimeAudioThreadPrepare();
	
	if(pActiveMicrophone){
		pActiveMicrophone->ProcessAudio();
		
	}else{
		OAL_CHECK(*this, alListenerf(AL_GAIN, 0.0f)); // mute listener
	}
	pDebugInfo->StoreTimeAudioThreadProcess();
	
	pDelayed->ProcessFreeOperations(false);
	pLogger->Synchronize();
	pDebugInfo->StoreTimeAudioThread();
}

void deoalAudioThread::pProcessAudioFast(){
	pElapsed = pTimerElapsed.GetElapsedTime();
	// pLogger->LogInfoFormat( "ProcessAudioFast: %.3f", pElapsed );
	
	if(pWaitSkipped){
// 		pLogger->LogWarnFormat( "Buffer underflow protection: %dms (+)", ( int )( pElapsed * 1000.0f ) );
		
	}else{
// 		pLogger->LogWarnFormat( "Buffer underflow protection: %dms", ( int )( pElapsed * 1000.0f ) );
		pWaitSkippedElapsed += pElapsed;
		pWaitSkipped = true;
	}
	
	if(pActiveMicrophone){
		pActiveMicrophone->ProcessAudioFast();
	}
	pLogger->Synchronize();
}

void deoalAudioThread::pReportLeaks(){
	// if disabled this all boils down to empty lines
	IF_LEAK_CHECK(pLogger->LogInfo("*** Leak check reports ***"));
	
	LEAK_CHECK_REPORT_LEAKS(*this, Component);
	LEAK_CHECK_REPORT_LEAKS(*this, Model);
	LEAK_CHECK_REPORT_LEAKS(*this, Microphone);
	LEAK_CHECK_REPORT_LEAKS(*this, Speaker);
	LEAK_CHECK_REPORT_LEAKS(*this, Sound);
	LEAK_CHECK_REPORT_LEAKS(*this, SoundLevelMeter);
	LEAK_CHECK_REPORT_LEAKS(*this, Skin);
	LEAK_CHECK_REPORT_LEAKS(*this, World);
	
	IF_LEAK_CHECK(pLogger->LogInfo("*** End of leak check reports ***"));
}
