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

#ifndef _DEOALRAYTRACEPARALLEL_H_
#define _DEOALRAYTRACEPARALLEL_H_

#include "../../../audiothread/deoalATRayTracing.h"

#include <dragengine/common/collection/decThreadSafeObjectOrderedSet.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/utils/decTimeHistory.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/threading/deBarrier.h>


class deoalAudioThread;
class deoalEnvProbe;
class deoalRayTraceConfig;
class deoalEnvProbeListener;
class deoalRTPTTraceSoundRays;
class deoalRTPTListen;
class deoalRTPTListenFinish;
// class deoalRTPTEnvProbeFull;
class deoalRTPTRoomEstimate;
class deoalRTPTRoomEstimateFinish;
class deoalRTPTTraceSoundRaysFinish;
class deoalSoundRayList;
class deoalAWorld;
class deoalRTWorldBVH;

class deParallelTask;
class deParallelProcessing;


/**
 * \brief Environment probe parallel ray tracing.
 */
class deoalRTParallelEnvProbe{
public:
	/** \brief Room parameters. */
	struct sRoomParameters{
		decDVector minExtend;
		decDVector maxExtend;
		float roomVolume;
		float roomSurface;
		float sabineMedium;
		float sabineLow;
		float sabineHigh;
		float roomAbsorptionLow;
		float roomAbsorptionMedium;
		float roomAbsorptionHigh;
		
		float meanFreePath;
		float separationTimeFirstLateReflection;
		float avgAbsorptionLow;
		float avgAbsorptionMedium;
		float avgAbsorptionHigh;
		
		float reverberationTimeMedium;
		float reverberationTimeLow;
		float reverberationTimeHigh;
		float echoDelay;
	};
	
	/** \brief Sound tracing types. */
	enum eSoundTraceTypes{
		/** \brief Microphone sound tracing. */
	};
	
	
	
private:
	deoalAudioThread &pAudioThread;
	deBarrier pBarrier;
	deMutex pMutex;
	
	decThreadSafeObjectOrderedSet pTasksTraceSoundRays;
	decPointerList pTasksReadyTraceSoundRays;
	decPointerList pTasksRunningTraceSoundRays;
	decPointerList pTasksWaitTraceSoundRays;
	decThreadSafeObjectOrderedSet pTasksTraceSoundRaysFinish;
	decPointerList pTasksReadyTraceSoundRaysFinish;
	decPointerList pTasksRunningTraceSoundRaysFinish;
	
	decThreadSafeObjectOrderedSet pTasksListen;
	decPointerList pTasksReadyListen;
	decPointerList pTasksRunningListen;
	decPointerList pTasksWaitListen;
	decThreadSafeObjectOrderedSet pTasksListenFinish;
	decPointerList pTasksReadyListenFinish;
	decPointerList pTasksRunningListenFinish;
	
// 	decThreadSafeObjectOrderedSet pTasksFull;
	
	decThreadSafeObjectOrderedSet pTasksRoomEstimate;
	decPointerList pTasksReadyRoomEstimate;
	decPointerList pTasksRunningRoomEstimate;
	decPointerList pTasksWaitRoomEstimate;
	decThreadSafeObjectOrderedSet pTasksRoomEstimateFinish;
	decPointerList pTasksReadyRoomEstimateFinish;
	decPointerList pTasksRunningRoomEstimateFinish;
	
	decTimer pTimer;
	decTimeHistory pTimeHistoryTraceSoundRays;
	decTimeHistory pTimeHistoryEstimateRoom;
	decTimeHistory pTimeHistoryListen;
	int pCounterTraceSoundRays;
	int pCounterEstimateRoom;
	int pCounterListen;
	float pElapsedRTTime;
	deParallelTask *pBarrierTask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray trace parallel. */
	deoalRTParallelEnvProbe( deoalAudioThread &audioThread );
	
	/** \brief Clean up ray trace parallel. */
	~deoalRTParallelEnvProbe();
	/*@}*/
	
	
	
public:
	/** \name Manegement */
	/*@{*/
	/** \brief Module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	
	
	/** \brief Trace sound rays. */
	void TraceSoundRays( sRoomParameters &roomParameters, deoalSoundRayList &soundRayList,
		const decDVector &position, float range, float refDist, float rollOff,
		float distanceOffset, deoalAWorld &world, deoalRTWorldBVH *rtWorldBVH,
		const decLayerMask &layerMask, const deoalATRayTracing::sConfigSoundTracing &config );
	
// 	void TraceSoundRays( sRoomParameters &roomParameters, deoalSoundRayList &soundRayList,
// 		const decDVector &position, float range, deoalAWorld &world,
// 		const decLayerMask &layerMask, const deoalATRayTracing::sConfigSoundTracing &config );
	
	/**
	 * \brief Calculate probe listen.
	 * 
	 * Updates environment probe with collected data
	 */
	void Listen( const deoalEnvProbe &sourceProbe, const deoalEnvProbe *listenProbe,
		deoalEnvProbeListener &listener, deoalAWorld &world, deoalRTWorldBVH *rtWorldBVH,
		const decLayerMask &layerMask, const decDVector &position );
	
	/**
	 * \brief Calculate probe full.
	 * 
	 * Updates environment probe with collected data
	 */
// 	void CalcProbeFull( deoalEnvProbe &envProbe, deoalAWorld &world,
// 		const deoalRayTraceConfig &probeConfig, const decDVector &listenPosition );
	
	/** \brief Estimate room parameters. */
	void EstimateRoomParameters( sRoomParameters &roomParameters, const decDVector &position,
		float range, deoalAWorld &world, const decLayerMask &layerMask,
		const deoalRayTraceConfig &probeConfig );
	
	
	
	/** \brief Time history of trace sound rays calls. */
	inline decTimeHistory &GetTimeHistoryTraceSoundRays(){ return pTimeHistoryTraceSoundRays; }
	inline const decTimeHistory &GetTimeHistoryTraceSoundRays() const{ return pTimeHistoryTraceSoundRays; }
	
	/** \brief Time history of estimate room calls. */
	inline decTimeHistory &GetTimeHistoryEstimateRoom(){ return pTimeHistoryEstimateRoom; }
	inline const decTimeHistory &GetTimeHistoryEstimateRoom() const{ return pTimeHistoryEstimateRoom; }
	
	/** \brief Time history of listen calls. */
	inline decTimeHistory &GetTimeHistoryListen(){ return pTimeHistoryListen; }
	inline const decTimeHistory &GetTimeHistoryListen() const{ return pTimeHistoryListen; }
	
	/** \brief Number of trace sound rays calls this frame update. */
	inline int GetCounterTraceSoundRays() const{ return pCounterTraceSoundRays; }
	
	/** \brief Number of estimate room calls this frame update. */
	inline int GetCounterEstimateRoom() const{ return pCounterEstimateRoom; }
	
	/** \brief Number of trace sound rays calls this frame update. */
	inline int GetCounterListen() const{ return pCounterListen; }
	
	/** \brief Reset counters. */
	void ResetCounters();
	
	/** \brief Elapsed ray tracing times for since the last reset. */
	inline float GetElapsedRTTime() const{ return pElapsedRTTime; }
	
	/** \brief Reset elapsed ray tracing times. */
	void ResetElapsedRTTime();
	
	
	
	/*
	 * \brief Finish task finished.
	 * \warning For use by finish parallel tasks only.
	 */
	void FinishTaskFinished( deParallelTask *task );
	
	/**
	 * \brief Enable task.
	 * \warning For use by respective task only.
	 */
// 	void Enable( deoalRTPTTraceSoundRays *task );
	void Enable( deoalRTPTTraceSoundRaysFinish *task );
// 	void Enable( deoalRTPTRoomEstimate *task );
	void Enable( deoalRTPTRoomEstimateFinish *task );
// 	void Enable( deoalRTPTListen *task );
	void Enable( deoalRTPTListenFinish *task );
	/*@}*/
	
	
	
private:
	void pRunTraceSoundRaysUsingTasks( sRoomParameters &roomParameters,
		deoalSoundRayList &soundRayList, const decDVector &position, float range, float refDist,
		float rollOff, float distanceOffset, deoalAWorld &world, deoalRTWorldBVH *rtWorldBVH,
		const decLayerMask &layerMask, const deoalATRayTracing::sConfigSoundTracing &config );
	
	bool pRunListenUsingTasks( const deoalEnvProbe &sourceProbe, const deoalEnvProbe *listenProbe,
		deoalEnvProbeListener &listener, deoalAWorld &world, deoalRTWorldBVH *rtWorldBVH,
		const decLayerMask &layerMask, const decDVector &position );
	
// 	void pStartTasksFull( deoalEnvProbe &envProbe, deoalAWorld &world,
// 		const deoalRayTraceConfig &probeConfig, const decDVector &listenPosition );
// 	void pProcessResultsFull( const deoalRayTraceConfig &probeConfig );
	
	void pRunRoomEstimateUsingTasks( sRoomParameters &roomParameters, const decDVector &position,
		float range, deoalAWorld &world, const decLayerMask &layerMask,
		const deoalRayTraceConfig &probeConfig );
	
	void pAddTask( deParallelProcessing &parallel, deParallelTask *task );
	void pWaitForFinishTask( deParallelProcessing &parallel, deParallelTask *task );
	void pAddTasks( deParallelProcessing &parallel, decPointerList &tasks,
		int count, decPointerList &running );
};

#endif
