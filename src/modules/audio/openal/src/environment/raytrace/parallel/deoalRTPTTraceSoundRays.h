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

#ifndef _DEOALRTPTTRACESOUNDRAYS_H_
#define _DEOALRTPTTRACESOUNDRAYS_H_

#include "../deoalSoundRayList.h"
#include "../deoalRayTraceResult.h"
#include "../visitors/deoalRTWOVRayHitsElement.h"
#include "../visitors/deoalRTWOVRayHitsClosest.h"
#include "../visitors/deoalRTWOVRayBlocked.h"
#include "../visitors/deoalWOVRayHitsElement.h"
#include "../visitors/deoalWOVRayBlocked.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/parallel/deParallelTask.h>

class deoalRayTraceConfig;
class deoalRTParallelEnvProbe;
class deoalAWorld;
class deoalRTWorldBVH;

// use 1-ray-per-task. this is ~26% faster than tracing multiple rays per task
#define RTPTTSR_ONE_TASK_PER_RAY 1


/**
 * \brief Ray tracing parallel task pre-calculating environment probe information.
 * 
 * These inputs are required by the task:
 * - World
 * - Environment probe
 * - Environment probe configuration
 * - Index of first ray from configuration to process
 * - Number of rays from configuration to process
 * 
 * These are the results calculated by the task:
 * - Sound ray list
 * - Minimum extend
 * - Maximum extend
 * - Sum mean free path
 * - Sum room volume
 * - Sum room surface
 * - Sum room sabines
 * - Sum absorptions
 */
class deoalRTPTTraceSoundRays : public deParallelTask{
private:
	struct sTraceRay{
		decDVector position;
		decDVector direction;
		decDVector normal;
		float distance;
		float remainingDistance;
		int bounces;
		int transmissions;
		int rayIndex;
		int transmittedRayIndex;
	};
	
	struct sTraceGain{
		float low;
		float medium;
		float high;
	};
	
	struct sTraceAbsorptionSum {
		float low;
		float medium;
		float high;
	};
	
	
	
	deoalRTParallelEnvProbe &pOwner;
	
	deoalAWorld *pWorld;
	deoalRTWorldBVH *pRTWorldBVH;
	const deoalRayTraceConfig *pProbeConfig;
	decDVector pPosition;
	float pRange;
	float pRefDist;
	float pRollOff;
	float pDistanceOffset;
	bool pUseAttenuation;
	int pFirstRay;
	#ifndef RTPTTSR_ONE_TASK_PER_RAY
	int pRayCount;
	#endif
	float pAddRayMinLength;
	float pThresholdTransmit;
	float pThresholdReflect;
	int pMaxBounceCount;
	int pMaxTransmitCount;
	float pInitialRayLength;
	decLayerMask pLayerMask;
	bool pInverseRayTracing;
	double pDetectOutsideLength;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
	deoalSoundRayList pSoundRayList;
	
	float pMeanFreePath;
	
	float pRoomVolume;
	float pRoomSurface;
	
	float pSabineMedium;
	float pSabineLow;
	float pSabineHigh;
	
	float pAbsorptionLow;
	float pAbsorptionMedium;
	float pAbsorptionHigh;
	int pAbsorptionCount;
	
	int pFirstHitCount;
	
	deoalRayTraceResult pRTResult;
	deoalWOVRayHitsElement pWOVRayHitsElement;
	deoalRTWOVRayHitsElement pRTWOVRayHitsElement;
	deoalRTWOVRayHitsClosest pRTWOVRayHitsClosest;
	deoalWOVRayBlocked pWOVRayBlocked;
	deoalRTWOVRayBlocked pRTWOVRayBlocked;
	bool pFirstHit;
	double pBackStepDistance;
	#ifndef RTPTTSR_ONE_TASK_PER_RAY
	int pRayIndex;
	#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray trace parallel task. */
	deoalRTPTTraceSoundRays( deoalRTParallelEnvProbe &owner );
	
protected:
	/** \brief Clean up ray trace parallel task. */
	virtual ~deoalRTPTTraceSoundRays();
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
	
	/** \brief Set position to trace rays from. */
	void SetPosition( const decDVector &position );
	
	/** \brief Set trace range. */
	void SetRange( float range );
	
	/** \brief Set attenuation parameters. */
	void SetAttenuationParameters( float refDist, float rollOff, float distanceOffset );
	
	/** \brief Set environment probe configuration. */
	void SetProbeConfig( const deoalRayTraceConfig *probeConfig );
	
	/** \brief Set index of first ray from configuration to process. */
	void SetFirstRay( int firstRay );
	
	/** \brief Set number of rays from configuration to process. */
	#ifndef RTPTTSR_ONE_TASK_PER_RAY
	void SetRayCount( int rayCount );
	#endif
	
	/** \brief Minimum length for rays required to be added to the ray list. */
	void SetAddRayMinLength( float length );
	
	/** \brief Gain threshold to continue tracing transmit rays. */
	void SetThresholdTransmit( float gain );
	
	/** \brief Gain threshold to continue tracing reflected rays. */
	void SetThresholdReflect( float gain );
	
	/** \brief Maximum number of bounces before stopping. */
	void SetMaxBounceCount( int count );
	
	/** \brief Maximum number of transmissions before stopping. */
	void SetMaxTransmitCount( int count );
	
	/** \brief Initial length of rays for testing before enlarging. */
	void SetInitialRayLength( float length );
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** \brief Set invertse ray tracing. */
	void SetInverseRayTracing( bool inverseRayTracing );
	
	/** \brief Set detect outside length. */
	void SetDetectOutsideLength( double detectOutsideLength );
	
	
	
	/** \brief Minimum extend. */
	inline const decDVector &GetMinExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decDVector &GetMaxExtend() const{ return pMaxExtend; }
	
	/** \brief Sound ray list. */
	inline const deoalSoundRayList &GetSoundRayList() const{ return pSoundRayList; }
	
	/** \brief Mean free path. */
	inline float GetMeanFreePath() const{ return pMeanFreePath; }
	
	/** \brief Room volume sum. */
	inline float GetRoomVolume() const{ return pRoomVolume; }
	
	/** \brief Room surface sum. */
	inline float GetRoomSurface() const{ return pRoomSurface; }
	
	/** \brief Sabine sum for medium frequencies. */
	inline float GetSabineMedium() const{ return pSabineMedium; }
	
	/** \brief Sabine sum for low frequencies. */
	inline float GetSabineLow() const{ return pSabineLow; }
	
	/** \brief Sabine sum for high frequencies. */
	inline float GetSabineHigh() const{ return pSabineHigh; }
	
	/** \brief Absorption sum for low frequencies. */
	inline float GetAbsorptionLow() const{ return pAbsorptionLow; }
	
	/** \brief Absorption sum for medium frequencies. */
	inline float GetAbsorptionMedium() const{ return pAbsorptionMedium; }
	
	/** \brief Absorption sum for high frequencies. */
	inline float GetAbsorptionHigh() const{ return pAbsorptionHigh; }
	
	/** \brief Absorption count. */
	inline int GetAbsorptionCount() const{ return pAbsorptionCount; }
	
	/** \brief Number of first hit rays. */
	inline int GetFirstHitCount() const{ return pFirstHitCount; }
	
	#ifdef WOVRAYHITSELEMENT_DO_TIMING
	inline deoalWOVRayHitsElement &wovRayHitsElement(){ return pWOVRayHitsElement; }
	inline deoalRayTraceResult &rayTraceResult(){ return pRTResult; }
	#endif
	#ifdef RTWOVRAYHITSELEMENT_DO_TIMING
	inline deoalRTWOVRayHitsElement &rtwovRayHitsElement(){ return pRTWOVRayHitsElement; }
	#endif
	#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
	inline deoalRTWOVRayHitsClosest &rtwovRayHitsClosest(){ return pRTWOVRayHitsClosest; }
	#endif
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
	
	
	
protected:
	void pTraceRay( const sTraceRay &ray, const sTraceGain &gain,
		const sTraceAbsorptionSum &absorptionSum );
	void pUpdateExtends( const decDVector &position );
	float Attenuate( float distance ) const;
	void DetectRayOutside( const sTraceRay &ray, const decDVector &position );
	bool IsRayBlocked( const decDVector &position, const decDVector &direction );
};

#endif
