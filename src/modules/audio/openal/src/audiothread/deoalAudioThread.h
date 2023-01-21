/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOALAUDIOTHREAD_H_
#define _DEOALAUDIOTHREAD_H_

#include "deoalATLeakTracker.h"
#include "../configuration/deoalConfiguration.h"

#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/utils/decTimeHistory.h>
#include <dragengine/threading/deBarrier.h>
#include <dragengine/threading/deThread.h>

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
	
	decTimer pTimerElapsed;
	float pElapsed;
	
	// time history
	decTimeHistory pTimeHistoryMain;
	decTimeHistory pTimeHistoryAudio;
	decTimeHistory pTimeHistoryAudioEstimated;
	decTimeHistory pTimeHistoryUpdate;
	decTimer pTimerMain;
	decTimer pTimerAudio;
	float pEstimatedAudioTime;
	float pAccumulatedMainTime;
	float pFrameTimeLimit;
	int pFPSRate;
	bool pReadyToWait;
	bool pWaitSkipped;
	float pWaitSkippedElapsed;
	
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
