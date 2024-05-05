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
