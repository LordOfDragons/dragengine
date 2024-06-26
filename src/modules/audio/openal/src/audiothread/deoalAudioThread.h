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

#ifndef _DEOALAUDIOTHREAD_H_
#define _DEOALAUDIOTHREAD_H_

#include "deoalATLeakTracker.h"
#include "../configuration/deoalConfiguration.h"

#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/utils/decTimeHistory.h>
#include <dragengine/threading/deBarrier.h>
#include <dragengine/threading/deThread.h>
#include <dragengine/threading/deMutex.h>

class deAudioOpenAL;
class deoalAMicrophone;
class deoalATContext;
class deoalATDebug;
class deoalATDelayed;
class deoalATLogger;
class deoalATRayTracing;
class deoalAWorld;
class deoalCaches;
class deoalCapabilities;
class deoalConfiguration;
class deoalDebugInfo;
class deoalDecodeBuffer;
class deoalDevMode;
class deoalExtensions;
class deoalRTParallelEnvProbe;
class deoalRayTraceHitElementList;
class deoalRayTraceResult;
class deoalSharedBufferList;
class deoalSoundRayInteractionList;
class deoalEffectSlotManager;
class deoalSharedEffectSlotManager;
class deoalSourceManager;
class deoalSpeakerList;
class deoalWOVCollectElements;
class deoalWOVRayHitsElement;



/**
 * Audio thread.
 */
class deoalAudioThread : public deThread{
private:
	enum eThreadStates{
		etsStopped,
		etsInitialize,
		etsSynchronize,
		etsAudio,
		etsFinishedAudio,
		etsCleaningUp,
		etsFrozen
	};
	
	deAudioOpenAL &pOal;
	
	bool pAsyncAudio;
	
	deoalATContext *pContext;
	
	deoalConfiguration pConfiguration;
	deoalATLeakTracker pLeakTracker;
	
	deoalDebugInfo *pDebugInfo;
	deoalATDebug *pDebug;
	deoalSpeakerList *pSpeakerList;
	deoalATLogger *pLogger;
	deoalATDelayed *pDelayed;
	deoalATRayTracing *pRayTracing;
	
	deoalCaches *pCaches;
	deoalDecodeBuffer *pDecodeBuffer;
	
	deoalExtensions *pExtensions;
	deoalCapabilities *pCapabilities;
	deoalEffectSlotManager *pEffectSlotManager;
	deoalSharedEffectSlotManager *pSharedEffectSlotManager;
	deoalSourceManager *pSourceManager;
	deoalSharedBufferList *pSharedBufferList;
	
	deoalRTParallelEnvProbe *pRTParallelEnvProbe;
	deoalRayTraceResult *pRTResultDirect;
	deoalRayTraceHitElementList *pRTHitElementList;
	deoalSoundRayInteractionList *pSRInteractionList;
	deoalWOVRayHitsElement *pWOVRayHitsElement;
	deoalWOVCollectElements *pWOVCollectElements;
	
	deoalAMicrophone *pActiveMicrophone;
	deoalAMicrophone *pDeactiveMicrophone;
	deoalAWorld *pActiveWorld;
	
	decObjectSet pProcessOnceWorld; // audio thread
	
	decTimer pTimerElapsed; // audio thread
	float pElapsed; // audio thread
	float pElapsedFull; // audio thread
	
	// time history
	decTimeHistory pTimeHistoryMain; // both (main, audio)
	decTimeHistory pTimeHistoryAudio; // audio thread
	decTimeHistory pTimeHistoryAudioEstimated; // audio thread
	decTimeHistory pTimeHistoryUpdate; // both (main, audio)
	decTimer pTimerMain; // main thread
	decTimer pTimerAudio; // audio thread
	float pEstimatedAudioTime; // both (main, audio)
	float pAccumulatedMainTime; // main thread
	float pFrameTimeLimit; // audio thread
	int pFPSRate; // main thread
	bool pReadyToWait; // shared (main, audio)
	bool pWaitSkipped; // audio thread
	float pWaitSkippedElapsed; // audio thread
	deMutex pMutexShared;
	
	// thread control
	eThreadStates pThreadState;
	bool pThreadFailure;
	deBarrier pBarrierSyncIn;
	deBarrier pBarrierSyncOut;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create audio thread. */
	deoalAudioThread( deAudioOpenAL &oal );
	
	/** Clean up audio thread. */
	virtual ~deoalAudioThread();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenAL module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	
	
	/** Async audio. */
	inline bool GetAsyncAudio() const{ return pAsyncAudio; }
	
	/** Set async audio. */
	void SetAsyncAudio( bool asyncAudio );
	
	
	
	/** Active microphone. */
	inline deoalAMicrophone *GetActiveMicrophone() const{ return pActiveMicrophone; }
	
	/** Set active microphone. */
	void SetActiveMicrophone( deoalAMicrophone *microphone );
	
	/** Active world if a microphone is active and has a parent world. */
	inline deoalAWorld *GetActiveWorld() const{ return pActiveWorld; }
	
	
	
	/** Initialize. */
	void Init();
	
	/** Clean up. */
	void CleanUp();
	
	/**
	 * Main thread wait for audio thread to finish.
	 * 
	 * Called only by main thread. Wraps WaitFinishAudio with debug time measuring if enabled
	 * and waiting optimization. If audio is done asynchronously time history is used to judge
	 * if audio is finished soon enough to wait for this event or to skip synchronization and
	 * running another game frame update.
	 * 
	 * \retval true Waited for audio to finish. Caller has to do synchronization and trigger
	 *              a new frame audio. Returned always for synchronous audio. Returned for
	 *              asynchronous audio if judging by the time history there is not enough time
	 *              left to do another full game frame update.
	 * \retval false Not waiting for audio to finish. Caller has to do another game frame update
	 *               before trying again. Never returned for synchronous audio. Returned for
	 *               asynchronous audio if judging by the time history there is enough time left
	 *               to do another full game frame update.
	 */
	bool MainThreadWaitFinishAudio();
	
	/** Wait for audio thread to finish. */
	void WaitFinishAudio();
	
	/** Retain resource data. */
	void RetainResourceData();
	
	/** Finalize asynchronously loaded resources. */
	void FinalizeAsyncResLoading();
	
	/** Synchronize. */
	void Synchronize();
	
	/**
	 * Freeze audio thread.
	 * 
	 * Waits until the audio thread finished rendering and no parallel task is running.
	 * Upon returning from this function data members of the audio thread can be accessed
	 * directly for example for configuration or debugging purpose.
	 */
	void Freeze();
	
	/** Unfreeze audio thread. */
	void Unfreeze();
	
	
	
	/** Run audio thread. */
	virtual void Run();
	
	
	
	/** Context. */
	inline deoalATContext &GetContext(){ return *pContext; }
	inline const deoalATContext &GetContext() const{ return *pContext; }
	
	
	
	/** Logger. */
	inline deoalATLogger &GetLogger() const{ return *pLogger; }
	
	/** Debug information. */
	inline deoalDebugInfo &GetDebugInfo() const{ return *pDebugInfo; }
	
	/** Debug. */
	inline deoalATDebug &GetDebug() const{ return *pDebug; }
	
	/** Delayed. */
	inline deoalATDelayed &GetDelayed() const{ return *pDelayed; }
	
	/** Ray tracing. */
	inline deoalATRayTracing &GetRayTracing() const{ return *pRayTracing; }
	
	/** Shared speaker list. */
	inline deoalSpeakerList &GetSpeakerList() const{ return *pSpeakerList; }
	
	/** Caches. */
	inline deoalCaches &GetCaches() const{ return *pCaches; }
	
	/** Shared decode buffer. */
	inline deoalDecodeBuffer &GetDecodeBuffer() const{ return *pDecodeBuffer; }
	
	/** Configuration. */
	inline deoalConfiguration &GetConfiguration(){ return pConfiguration; }
	inline const deoalConfiguration &GetConfiguration() const{ return pConfiguration; }
	
	/** Leak tracker. */
	inline deoalATLeakTracker &GetLeakTracker(){ return pLeakTracker; }
	
	/** Extensions. */
	inline deoalExtensions &GetExtensions() const{ return *pExtensions; }
	
	/** Capabilities. */
	inline deoalCapabilities &GetCapabilities() const{ return *pCapabilities; }
	
	/** Effect slot manager. */
	inline deoalEffectSlotManager &GetEffectSlotManager() const{ return *pEffectSlotManager; }
	
	/** Shared effect slot manager. */
	inline deoalSharedEffectSlotManager &GetSharedEffectSlotManager() const{ return *pSharedEffectSlotManager; }
	
	/** Source manager. */
	inline deoalSourceManager &GetSourceManager() const{ return *pSourceManager; }
	
	/** Shared buffer list. */
	inline deoalSharedBufferList &GetSharedBufferList() const{ return *pSharedBufferList; }
	
	/** Parallel env-probe ray-tracer. */
	inline deoalRTParallelEnvProbe &GetRTParallelEnvProbe() const{ return *pRTParallelEnvProbe; }
	
	/** Shared ray trace result for direct sound tracing. */
	inline deoalRayTraceResult &GetRTResultDirect() const{ return *pRTResultDirect; }
	
	/** Shared ray trace hit element list. */
	inline deoalRayTraceHitElementList &GetRTHitElementList() const{ return *pRTHitElementList; }
	
	/** Shared sound ray interaction list. */
	inline deoalSoundRayInteractionList &GetSRInteractionList() const{ return *pSRInteractionList; }
	
	/** Shared ray hits element world octree visitor. */
	inline deoalWOVRayHitsElement &GetWOVRayHitsElement() const{ return *pWOVRayHitsElement; }
	
	/** Shared collect elements world octree visitor. */
	inline deoalWOVCollectElements &GetWOVCollectElements() const{ return *pWOVCollectElements; }
	
	
	
	/** Elapsed time. */
	inline float GetElapsed() const{ return pElapsed; }
	
	/** Elapsed time since the last full update. */
	inline float GetElapsedFull() const{ return pElapsedFull; }
	
	/** FPS Rate. */
	inline int GetFPSRate() const{ return pFPSRate; }
	
	
	
	/** Main thread time history. */
	inline decTimeHistory &GetTimeHistoryMain(){ return pTimeHistoryMain; }
	inline const decTimeHistory &GetTimeHistoryMain() const{ return pTimeHistoryMain; }
	
	/** Audio thread time history. */
	inline decTimeHistory &GetTimeHistoryAudio(){ return pTimeHistoryAudio; }
	inline const decTimeHistory &GetTimeHistoryAudio() const{ return pTimeHistoryAudio; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pInitThreadPhase1();
	void pCleanUpThread();
	void pLimitFrameRate( float elapsed );
	
	void pSyncConfiguration();
	void pPrepareConfiguration();
	
	void pProcessAudio();
	void pProcessAudioFast();
	
	void pReportLeaks();
};

#endif
