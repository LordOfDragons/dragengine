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
	float pBackStepDistance;
	int pRayIndex;
	float pRayGaussWidthFactor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create ray trace parallel task. */
	deoalRTPTEnvProbeFull(deoalRTParallelEnvProbe &owner);
	
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
	void SetWorld(deoalAWorld *world);
	
	/** \brief Set environment probe. */
	void SetEnvProbe(const deoalEnvProbe *envProbe);
	
	/** \brief Set environment probe configuration. */
	void SetProbeConfig(const deoalRayTraceConfig *probeConfig);
	
	/** \brief Set index of first ray from configuration to process. */
	void SetFirstRay(int firstRay);
	
	/** \brief Set number of rays from configuration to process. */
	void SetRayCount(int rayCount);
	
	/** \brief Listener position. */
	void SetListenerPosition(const decDVector &position);
	
	/** \brief Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	
	
	/** \brief Minimum extend. */
	inline const decDVector &GetMinExtend() const{return pMinExtend;}
	
	/** \brief Maximum extend. */
	inline const decDVector &GetMaxExtend() const{return pMaxExtend;}
	
	/** \brief Gain low frequencies. */
	inline float GetGainLow() const{return pGainLow;}
	
	/** \brief Gain medium frequencies. */
	inline float GetGainMedium() const{return pGainMedium;}
	
	/** \brief Gain high frequencies. */
	inline float GetGainHigh() const{return pGainHigh;}
	
	/** \brief Delay. */
	inline float GetDelay() const{return pDelay;}
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Short task name for debugging. */
	virtual decString GetDebugName() const;
	
	/** \brief Task details for debugging. */
	virtual decString GetDebugDetails() const;
	/*@}*/
	
	
	
protected:
	void pTraceRay(const sTraceRay &ray, const sTraceGain &gain);
	void pUpdateExtends(const decDVector &position);
};

#endif
