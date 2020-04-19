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

#ifndef _DEOALRTPTENVPROBEFIRSTFULL_H_
#define _DEOALRTPTENVPROBEFIRSTFULL_H_

#include "../deoalSoundRayInteractionList.h"
#include "../deoalRayTraceResult.h"
#include "../visitors/deoalWOVRayHitsElement.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/parallel/deParallelTask.h>

class deoalEnvProbe;
class deoalRayTraceConfig;
class deoalRTParallelEnvProbe;
class deoalAWorld;

// #define THAN_DEBUG 1


/**
 * \brief Ray tracing parallel task calculating environment probe full information.
 * 
 * These inputs are required by the task:
 * - World
 * - Environment probe
 * - Environment probe configuration
 * - Index of first ray from configuration to process
 * - Number of rays from configuration to process
 * - Listen position
 * 
 * These are the results calculated by the task:
 * - Sound ray interaction list
 * - Minimum extend
 * - Maximum extend
 * - Gain for low, medium and high frequencies
 * - Delay
 */
class deoalRTPTEnvProbeFull : public deParallelTask{
private:
	struct sTraceRay{
		decDVector position;
		decDVector direction;
		float distance;
		float remainingDistance;
		int bounce;
	};
	
	struct sTraceGain{
		float low;
		float medium;
		float high;
	};
	
	
	
	deoalRTParallelEnvProbe &pOwner;
	
	deoalAWorld *pWorld;
	const deoalEnvProbe *pEnvProbe;
	const deoalRayTraceConfig *pProbeConfig;
	int pFirstRay;
	int pRayCount;
	decDVector pListenerPosition;
	decLayerMask pLayerMask;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
	float pGainLow;
	float pGainMedium;
	float pGainHigh;
	float pDelay;
	
	deoalRayTraceResult pRTResult;
	deoalWOVRayHitsElement pWOVRayHitsElement;
	bool pFirstHit;
	double pBackStepDistance;
	int pRayIndex;
	double pRayGaussWidthFactor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray trace parallel task. */
	deoalRTPTEnvProbeFull( deoalRTParallelEnvProbe &owner );
	
protected:
	/** \brief Clean up ray trace parallel task. */
	virtual ~deoalRTPTEnvProbeFull();
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
	
	/** \brief Set environment probe. */
	void SetEnvProbe( const deoalEnvProbe *envProbe );
	
	/** \brief Set environment probe configuration. */
	void SetProbeConfig( const deoalRayTraceConfig *probeConfig );
	
	/** \brief Set index of first ray from configuration to process. */
	void SetFirstRay( int firstRay );
	
	/** \brief Set number of rays from configuration to process. */
	void SetRayCount( int rayCount );
	
	/** \brief Listener position. */
	void SetListenerPosition( const decDVector &position );
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** \brief Minimum extend. */
	inline const decDVector &GetMinExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decDVector &GetMaxExtend() const{ return pMaxExtend; }
	
	/** \brief Gain low frequencies. */
	inline float GetGainLow() const{ return pGainLow; }
	
	/** \brief Gain medium frequencies. */
	inline float GetGainMedium() const{ return pGainMedium; }
	
	/** \brief Gain high frequencies. */
	inline float GetGainHigh() const{ return pGainHigh; }
	
	/** \brief Delay. */
	inline float GetDelay() const{ return pDelay; }
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Short task name for debugging. */
	virtual decString GetDebugName() const;
	
	/** \brief Task details for debugging. */
	virtual decString GetDebugDetails() const;
	/*@}*/
	
	
	
protected:
	void pTraceRay( const sTraceRay &ray, const sTraceGain &gain );
	void pUpdateExtends( const decDVector &position );
};

#endif
