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
class deoalSourceManager;
class deoalSpeakerList;
class deoalWOVCollectElements;
class deoalWOVRayHitsElement;



/**
 * \brief Audio thread.
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
	decTimer pTimerMain;
	decTimer pTimerAudio;
	float pEstimatedAudioTime;
	float pAccumulatedMainTime;
	float pFrameTimeLimit;
	bool pReadyToWait;
	
	// thread control
	eThreadStates pThreadState;
	bool pThreadFailure;
	deBarrier pBarrierSyncIn;
	deBarrier pBarrierSyncOut;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create audio thread. */
	deoalAudioThread( deAudioOpenAL &oal );
	
	/** \brief Clean up audio thread. */
	virtual ~deoalAudioThread();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	
	
	/** \brief Async audio. */
	inline bool GetAsyncAudio() const{ return pAsyncAudio; }
	
	/** \brief Set async audio. */
	void SetAsyncAudio( bool asyncAudio );
	
	
	
	/** \brief Active microphone. */
	inline deoalAMicrophone *GetActiveMicrophone() const{ return pActiveMicrophone; }
	
	/** \brief Set active microphone. */
	void SetActiveMicrophone( deoalAMicrophone *microphone );
	
	/** \brief Active world if a microphone is active and has a parent world. */
	inline deoalAWorld *GetActiveWorld() const{ return pActiveWorld; }
	
	
	
	/** \brief Initialize. */
	void Init();
	
	/** \brief Clean up. */
	void CleanUp();
	
	/**
	 * \brief Main thread wait for audio thread to finish.
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
	
	/** \brief Wait for audio thread to finish. */
	void WaitFinishAudio();
	
	/** \brief Retain resource data. */
	void RetainResourceData();
	
	/** \brief Finalize asynchronously loaded resources. */
	void FinalizeAsyncResLoading();
	
	/** \brief Synchronize. */
	void Synchronize();
	
	/**
	 * \brief Freeze audio thread.
	 * 
	 * Waits until the audio thread finished rendering and no parallel task is running.
	 * Upon returning from this function data members of the audio thread can be accessed
	 * directly for example for configuration or debugging purpose.
	 */
	void Freeze();
	
	/** \brief Unfreeze audio thread. */
	void Unfreeze();
	
	
	
	/** \brief Run audio thread. */
	virtual void Run();
	
	
	
	/** \brief Context. */
	inline deoalATContext &GetContext(){ return *pContext; }
	inline const deoalATContext &GetContext() const{ return *pContext; }
	
	
	
	/** \brief Logger. */
	inline deoalATLogger &GetLogger() const{ return *pLogger; }
	
	/** \brief Debug information. */
	inline deoalDebugInfo &GetDebugInfo() const{ return *pDebugInfo; }
	
	/** \brief Debug. */
	inline deoalATDebug &GetDebug() const{ return *pDebug; }
	
	/** \brief Delayed. */
	inline deoalATDelayed &GetDelayed() const{ return *pDelayed; }
	
	/** \brief Ray tracing. */
	inline deoalATRayTracing &GetRayTracing() const{ return *pRayTracing; }
	
	/** \brief Shared speaker list. */
	inline deoalSpeakerList &GetSpeakerList() const{ return *pSpeakerList; }
	
	/** \brief Caches. */
	inline deoalCaches &GetCaches() const{ return *pCaches; }
	
	/** \brief Shared decode buffer. */
	inline deoalDecodeBuffer &GetDecodeBuffer() const{ return *pDecodeBuffer; }
	
	/** \brief Configuration. */
	inline deoalConfiguration &GetConfiguration(){ return pConfiguration; }
	inline const deoalConfiguration &GetConfiguration() const{ return pConfiguration; }
	
	/** \brief Leak tracker. */
	inline deoalATLeakTracker &GetLeakTracker(){ return pLeakTracker; }
	
	/** \brief Extensions. */
	inline deoalExtensions &GetExtensions() const{ return *pExtensions; }
	
	/** \brief Capabilities. */
	inline deoalCapabilities &GetCapabilities() const{ return *pCapabilities; }
	
	/** \brief Source manager. */
	inline deoalSourceManager &GetSourceManager() const{ return *pSourceManager; }
	
	/** \brief Shared buffer list. */
	inline deoalSharedBufferList &GetSharedBufferList() const{ return *pSharedBufferList; }
	
	/** \brief Parallel env-probe ray-tracer. */
	inline deoalRTParallelEnvProbe &GetRTParallelEnvProbe() const{ return *pRTParallelEnvProbe; }
	
	/** \brief Shared ray trace result for direct sound tracing. */
	inline deoalRayTraceResult &GetRTResultDirect() const{ return *pRTResultDirect; }
	
	/** \brief Shared ray trace hit element list. */
	inline deoalRayTraceHitElementList &GetRTHitElementList() const{ return *pRTHitElementList; }
	
	/** \brief Shared sound ray interaction list. */
	inline deoalSoundRayInteractionList &GetSRInteractionList() const{ return *pSRInteractionList; }
	
	/** \brief Shared ray hits element world octree visitor. */
	inline deoalWOVRayHitsElement &GetWOVRayHitsElement() const{ return *pWOVRayHitsElement; }
	
	/** \brief Shared collect elements world octree visitor. */
	inline deoalWOVCollectElements &GetWOVCollectElements() const{ return *pWOVCollectElements; }
	
	
	
	/** \brief Elapsed time. */
	inline float GetElapsed() const{ return pElapsed; }
	
	
	
	/** \brief Main thread time history. */
	inline decTimeHistory &GetTimeHistoryMain(){ return pTimeHistoryMain; }
	inline const decTimeHistory &GetTimeHistoryMain() const{ return pTimeHistoryMain; }
	
	/** \brief Audio thread time history. */
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
	
	void pReportLeaks();
};

#endif
