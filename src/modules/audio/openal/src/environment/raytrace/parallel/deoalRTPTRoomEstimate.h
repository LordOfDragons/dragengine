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

#ifndef _DEOALRTPTROOMESTIMATE_H_
#define _DEOALRTPTROOMESTIMATE_H_

#include "../deoalRayTraceResult.h"
#include "../visitors/deoalWOVRayHitsElement.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/parallel/deParallelTask.h>

class deoalRayTraceConfig;
class deoalRTParallelEnvProbe;
class deoalAWorld;


// use 1-ray-per-task
#define RTPTRE_ONE_TASK_PER_RAY 1


/**
 * \brief Ray tracing parallel task estimating room parameters.
 * 
 * These inputs are required by the task:
 * - World
 * - Environment probe configuration
 * - Index of first ray from configuration to process
 * - Number of rays from configuration to process
 * - Position
 * 
 * These are the results calculated by the task:
 * - Minimum extend
 * - Maximum extend
 * - Sum room volume
 * - Sum room surface
 * - Sum room sabines
 * - Sum absorptions
 * - Number of rays hitting room geometry
 */
class deoalRTPTRoomEstimate : public deParallelTask{
private:
	struct sHitResult{
		const deoalRayTraceHitElement *element;
		float distance;
	};
	
	deoalRTParallelEnvProbe &pOwner;
	
	deoalAWorld *pWorld;
	const deoalRayTraceConfig *pProbeConfig;
	int pFirstRay;
	#ifndef RTPTRE_ONE_TASK_PER_RAY
	int pRayCount;
	#endif
	decDVector pPosition;
	float pRange;
	float pInitialRayLength;
	decLayerMask pLayerMask;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	float pRoomVolume;
	float pRoomSurface;
	float pSabineMedium;
	float pSabineLow;
	float pSabineHigh;
	float pAbsorptionLow;
	float pAbsorptionMedium;
	float pAbsorptionHigh;
	int pHitCount;
	
	deoalRayTraceResult pRTResult;
	deoalWOVRayHitsElement pWOVRayHitsElement;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray trace parallel task. */
	deoalRTPTRoomEstimate( deoalRTParallelEnvProbe &owner );
	
protected:
	/** \brief Clean up ray trace parallel task. */
	virtual ~deoalRTPTRoomEstimate();
	/*@}*/
	
	
	
public:
	/** \name Manegement */
	/*@{*/
	/** \brief Parallel task implementation. */
	virtual void Run();
	
	/** \brief Processing of task Run() finished. */
	virtual void Finished();
	
	
	
	/** \brief Set world. */
	void SetWorld( deoalAWorld *world );
	
	/** \brief Set environment probe configuration. */
	void SetProbeConfig( const deoalRayTraceConfig *probeConfig );
	
	/** \brief Set index of first ray from configuration to process. */
	void SetFirstRay( int firstRay );
	
	/** \brief Set number of rays from configuration to process. */
	#ifndef RTPTRE_ONE_TASK_PER_RAY
	void SetRayCount( int rayCount );
	#endif
	
	/** \brief Position to trace from. */
	void SetPosition( const decDVector &position );
	
	/** \brief Maximum range to trace rays for. */
	void SetRange( float range );
	
	/** \brief Initial length of rays for testing before enlarging. */
	void SetInitialRayLength( float length );
	
	/** \brief Layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** \brief Minimum extend. */
	inline const decDVector &GetMinExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decDVector &GetMaxExtend() const{ return pMaxExtend; }
	
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
	
	/** \brief Number of rays hitting room geometry. */
	inline int GetHitCount() const{ return pHitCount; }
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Short task name for debugging. */
	virtual decString GetDebugName() const;
	
	/** \brief Task details for debugging. */
	virtual decString GetDebugDetails() const;
	/*@}*/
	
	
	
private:
	bool pTraceRay( const decVector &direction, sHitResult &hitResult );
};

#endif
