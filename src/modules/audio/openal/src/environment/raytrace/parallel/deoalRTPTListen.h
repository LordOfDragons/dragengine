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

#ifndef _DEOALRTPTLISTEN_H_
#define _DEOALRTPTLISTEN_H_

#include "../deoalRayTraceResult.h"
#include "../deoalImpulseResponse.h"
#include "../visitors/deoalRTWOVRayHitsElement.h"
#include "../visitors/deoalRTWOVRayBlocked.h"
#include "../visitors/deoalWOVRayHitsElement.h"
#include "../visitors/deoalWOVRayBlocked.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/parallel/deParallelTask.h>

class deoalEnvProbe;
class deoalRTParallelEnvProbe;
class deoalAWorld;
class deoalSoundRay;
class deoalSoundRayList;
class deoalWorldOctree;
class deoalRTWorldBVH;


// use 1-ray-per-task. this is ~26% faster than tracing multiple rays per task
#define RTPTL_ONE_TASK_PER_RAY 1



/**
 * \brief Ray tracing parallel task calculating listen to env probe.
 * 
 * These inputs are required by the task:
 * - World
 * - Index of first ray from probe to process
 * - Number of rays from probe to process
 * - Listener position
 * - Listen probe
 * - Source probe
 * 
 * These are the results calculated by the task:
 * - Full reflected gain (low, medium, high)
 * - Sum first reflection delay
 * - Minimum first reflection delay
 * - Maximum first reflection delay
 */
class deoalRTPTListen : public deParallelTask{
private:
	deoalRTParallelEnvProbe &pOwner;
	
	deoalAWorld *pWorld;
	deoalRTWorldBVH *pRTWorldBVH;
	int pFirstRay;
	#ifndef RTPTL_ONE_TASK_PER_RAY
	int pRayCount;
	#endif
	decDVector pListenPosition;
	const deoalEnvProbe *pListenProbe;
	const deoalEnvProbe *pSourceProbe;
	float pDirectSoundDistance;
	float pSepDistFirstLateRefl;
	decLayerMask pLayerMask;
	
	float pFRSumDistance;
	float pFRMinDistance;
	float pFRMaxDistance;
	float pFRGainLow;
	float pFRGainMedium;
	float pFRGainHigh;
	decVector pFRPanDirection;
	float pFRPanDirectionWeightSum;
	int pFRCount;
	
	float pLRGainLow;
	float pLRGainMedium;
	float pLRGainHigh;
	decVector pLRPanDirection;
	float pLRPanDirectionWeightSum;
	int pLRCount;
	
	bool pHasHighestGain;
	float pHighestGainGain;
	float pHighestGainDistance;
	
	float pAbsorptionSumLow;
	float pAbsorptionSumMedium;
	float pAbsorptionSumHigh;
	int pAbsorptionCount;
	
	float pMeanFreePathSum;
	int pMeanFreePathCount;
	
	float pDecayTimeSumLow;
	float pDecayTimeSumMedium;
	float pDecayTimeSumHigh;
	int pDecayTimeCount;
	
	float pLimitRevTimeSumLow;
	float pLimitRevTimeSumMedium;
	float pLimitRevTimeSumHigh;
	int pLimitRevTimeCount;
	int pUnlimitRevTimeCount;
	
	deoalImpulseResponse pImpulseResponse;
	
	deoalRayTraceResult pRTResult;
	deoalWOVRayHitsElement pWOVRayHitsElement;
	deoalRTWOVRayHitsElement pRTWOVRayHitsElement;
	deoalWOVRayBlocked pWOVRayBlocked;
	deoalRTWOVRayBlocked pRTWOVRayBlocked;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray trace parallel task. */
	deoalRTPTListen( deoalRTParallelEnvProbe &owner );
	
protected:
	/** \brief Clean up ray trace parallel task. */
	virtual ~deoalRTPTListen();
	/*@}*/
	
	
	
public:
	/** \name Manegement */
	/*@{*/
	/** \brief Parallel task implementation. */
	virtual void Run();
	
	/** \brief Processing of task Run() finished. */
	virtual void Finished();
	
	
	
	/** \brief Set world. */
	void SetWorld( deoalAWorld *world, deoalRTWorldBVH *rtBVH );
	
	/** \brief Set index of first ray from probe to process. */
	void SetFirstRay( int firstRay );
	
	/** \brief Set number of rays from probe to process. */
	#ifndef RTPTL_ONE_TASK_PER_RAY
	void SetRayCount( int rayCount );
	#endif
	
	/** \brief Listen position. */
	void SetListenPosition( const decDVector &position );
	
	/** \brief Set listen probe. */
	void SetListenProbe( const deoalEnvProbe *probe );
	
	/** \brief Set sound source probe. */
	void SetSourceProbe( const deoalEnvProbe *probe );
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );

	
	
	/** \brief First reflection distance sum. */
	inline float GetFRSumDistance() const{ return pFRSumDistance; }
	
	/** \brief First reflection minimum distance. */
	inline float GetFRMinDistance() const{ return pFRMinDistance; }
	
	/** \brief First reflection maximum distance. */
	inline float GetFRMaxDistance() const{ return pFRMaxDistance; }
	
	/** \brief First reflection gain low frequencies. */
	inline float GetFRGainLow() const{ return pFRGainLow; }
	
	/** \brief First reflection gain medium frequencies. */
	inline float GetFRGainMedium() const{ return pFRGainMedium; }
	
	/** \brief First reflection gain high frequencies. */
	inline float GetFRGainHigh() const{ return pFRGainHigh; }
	
	/** \brief First reflection panning direction. */
	inline const decVector GetFRPanDirection() const{ return pFRPanDirection; }
	
	/** \brief First reflection panning direction weight sum. */
	inline float GetFRPanDirectionWeightSum() const{ return pFRPanDirectionWeightSum; }
	
	/** \brief Number of first reflections. */
	inline int GetFRCount() const{ return pFRCount; }
	
	
	
	/** \brief Late reflection gain low frequencies. */
	inline float GetLRGainLow() const{ return pLRGainLow; }
	
	/** \brief Late reflection gain medium frequencies. */
	inline float GetLRGainMedium() const{ return pLRGainMedium; }
	
	/** \brief Late reflection gain high frequencies. */
	inline float GetLRGainHigh() const{ return pLRGainHigh; }
	
	/** \brief Late reflection panning direction. */
	inline const decVector GetLRPanDirection() const{ return pLRPanDirection; }
	
	/** \brief Late reflection panning direction weight sum. */
	inline float GetLRPanDirectionWeightSum() const{ return pLRPanDirectionWeightSum; }
	
	/** \brief Number of late reflections. */
	inline int GetLRCount() const{ return pLRCount; }
	
	
	
	/** \brief Has highest gain contribution. */
	inline bool GetHasHighestGain() const{ return pHasHighestGain; }
	
	/** \brief Gain of highest gain contribution. */
	inline float GetHighestGainGain() const{ return pHighestGainGain; }
	
	/** \brief Distance of highest gain contribution. */
	inline float GetHighestGainDistance() const{ return pHighestGainDistance; }
	
	
	
	/** \brief Absorption sum for low frequencies of all hits. */
	inline float GetAbsorptionSumLow() const{ return pAbsorptionSumLow; }
	
	/** \brief Absorption sum for medium frequencies of all hits. */
	inline float GetAbsorptionSumMedium() const{ return pAbsorptionSumMedium; }
	
	/** \brief Absorption sum for high frequencies of all hits. */
	inline float GetAbsorptionSumHigh() const{ return pAbsorptionSumHigh; }
	
	/** \brief Number of absorptions summed up of all hits. */
	inline int GetAbsorptionCount() const{ return pAbsorptionCount; }
	
	
	
	/** \brief Mean free path sum. */
	inline float GetMeanFreePathSum() const{ return pMeanFreePathSum; }
	
	/** \brief Number of mean free path sum. */
	inline int GetMeanFreePathCount() const{ return pMeanFreePathCount; }
	
	
	
	/** \brief Decay time sum for low frequencies. */
	inline float GetDecayTimeSumLow() const{ return pDecayTimeSumLow; }
	
	/** \brief Decay time sum for medium frequencies. */
	inline float GetDecayTimeSumMedium() const{ return pDecayTimeSumMedium; }
	
	/** \brief Decay time sum for high frequencies. */
	inline float GetDecayTimeSumHigh() const{ return pDecayTimeSumHigh; }
	
	/** \brief Number of decay time sum. */
	inline int GetDecayTimeCount() const{ return pDecayTimeCount; }
	
	
	
	/** \brief Limit reverberation time sum for low frequencies. */
	inline float GetLimitRevTimeSumLow() const{ return pLimitRevTimeSumLow; }
	
	/** \brief Limit reverberation time sum for medium frequencies. */
	inline float GetLimitRevTimeSumMedium() const{ return pLimitRevTimeSumMedium; }
	
	/** \brief Limit reverberation time sum for high frequencies. */
	inline float GetLimitRevTimeSumHigh() const{ return pLimitRevTimeSumHigh; }
	
	/** \brief Limit reverberation time count. */
	inline int GetLimitRevTimeCount() const{ return pLimitRevTimeCount; }
	
	/** \brief Unlimit reverberation time count. */
	inline int GetUnlimitRevTimeCount() const{ return pUnlimitRevTimeCount; }
	
	
	
	/** \brief Unsorted impulse response. */
	inline const deoalImpulseResponse &GetImpulseResponse() const{ return pImpulseResponse; }
	
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	inline deoalRTWOVRayBlocked &rtwovRayBlocked(){ return pRTWOVRayBlocked; }
	#endif
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Short task name for debugging. */
	virtual decString GetDebugName() const;
	
	/** \brief Task details for debugging. */
	virtual decString GetDebugDetails() const;
	/*@}*/
	
	
	
private:
	void RunLinearBeam();
	void ClosestToRay( float &closestBeamDistance, float &closestDistSquared,
		int &closestSegment, int &closestBounces, const deoalSoundRayList &soundRayList,
		const deoalSoundRay &soundRay, const decVector &targetPosition, float baseBeamRadius,
		deoalWorldOctree &octree, const decVector &targetPositionWorld );
	
	struct sSphereReceiverParams{
		const deoalSoundRayList &soundRayList;
		const decVector &targetPosition;
		deoalWorldOctree &octree;
		const decVector &targetPositionWorld;
		const decVector &gainPosition;
		float receiverRadius;
		float receiverRadiusSquared;
		float invReceiverVolume;
		
		sSphereReceiverParams( const deoalSoundRayList &soundRayList,
			const decVector &targetPosition, deoalWorldOctree &octree,
			const decVector &targetPositionWorld, const decVector &gainPosition,
			float receiverRadius, float receiverRadiusSquared, float invReceiverVolume );
	};
	
	struct sSphereReceiverImpinge {
		float time;
		float low;
		float medium;
		float high;
		
		sSphereReceiverImpinge();
	};
	
	void RunSphereReceiver();
	void RunSphereReceiver( const sSphereReceiverParams &params,
		const deoalSoundRay &soundRay, const sSphereReceiverImpinge *firstImpinge );

#if 0
	void RunGaussBeam();
	void RunListenerSphere();
	void RunHuygens();
#endif
	
	bool IsRayBlocked( deoalWorldOctree &octree, const decDVector &position,
		const decDVector &direction );
	
	void ApplyIndirectGains( int bounces, const decVector &direction,
		float distance, float gainLow, float gainMedium, float gainHigh );
};

#endif
