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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoalRTParallelEnvProbe.h"
#include "deoalRTPTTraceSoundRays.h"
#include "../deoalRayTraceHitElement.h"
#include "../deoalSoundRay.h"
#include "../deoalSoundRaySegment.h"
#include "../deoalRayTraceConfig.h"
#include "../../../deAudioOpenAL.h"
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATDebug.h"
#include "../../../audiothread/deoalATLogger.h"
#include "../../../component/deoalAComponent.h"
#include "../../../component/deoalAComponentTexture.h"
#include "../../../model/deoalAModel.h"
#include "../../../model/deoalModelFace.h"
#include "../../../world/deoalAWorld.h"
#include "../../../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalRTPTaskEnvProbe
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTPTTraceSoundRays::deoalRTPTTraceSoundRays(deoalRTParallelEnvProbe &owner) :
deParallelTask(&owner.GetAudioThread().GetOal()),
pOwner(owner),
pWorld(NULL),
    pRTWorldBVH(NULL),
pProbeConfig(NULL),
pRange(0.0f),
pRefDist(1.0f),
pRollOff(0.0f),
pDistanceOffset(0.0f),
pUseAttenuation(false),
pFirstRay(0),
#ifndef RTPTTSR_ONE_TASK_PER_RAY
pRayCount(0),
#endif
pAddRayMinLength(0.2f), // 0.1f
pThresholdTransmit(1e-3f),
pThresholdReflect(1e-3f /*1e-4f*/),
pMaxBounceCount(20),
pMaxTransmitCount(2),
pInitialRayLength(10.0f), // 5.0f
pInverseRayTracing(false),
pDetectOutsideLength(100.0),
pBackStepDistance(1e-4)
{
	(void)pOwner; // silence compiler warning
	
	pWOVRayHitsElement.SetResult(&pRTResult);
	pRTWOVRayHitsElement.SetResult(&pRTResult);
	SetMarkFinishedAfterRun(true);
	SetLowPriority(true);
	
	/*
	Parallel-CalcProbePreCalc: pos=(0.000,1.600,-4.000) range=60.000 rays=162
	none) time: 8.80ms 9.28ms 9.60ms 9.43ms
	10m) time: 7.26ms 7.99ms 7.62ms
	5m) time: 7.75ms 7.16ms 8.83ms
	2.5m) time: 8.49ms 8.24ms 8.39ms

	Parallel-CalcProbePreCalc: pos=(0.000,1.600,-4.000) range=300.000 rays=162 maxBounces=100 maxTransmit=3
	none) time: 44.86ms 41.45ms 45.13ms
	10m) time: 32.03ms 31.25ms 31.85ms
	5m) time: 30.26ms 29.99ms 29.11ms
	2.5m) time: 42.15ms 32.00ms 42.02ms
	
	Parallel-CalcProbePreCalc: pos=(0.000,1.600,-4.000) range=300.000 rays=162 maxBounces=20 maxTransmit=3
	10m) time: 22.44ms 24.89ms 23.13ms
	
	Parallel-CalcProbePreCalc: pos=(0.000,1.600,-4.000) range=60.000 rays=162 maxBounces=20 maxTransmit=3
	10m) time: 28.17ms 23.28ms 18.42ms
	
	
	
	Parallel-CalcProbePreCalc: pos=(0.000,1.600,-4.000) range=300.000 rays=162 maxBounces=100 maxTransmit=1
	10m) time: 53.19ms 62.38ms 53.97ms
	
	Parallel-CalcProbePreCalc: pos=(0.000,1.600,-4.000) range=300.000 rays=162 maxBounces=20 maxTransmit=3
	10m) time: 21.93ms 22.94ms 23.42ms
	
	Parallel-CalcProbePreCalc: pos=(0.000,1.600,-4.000) range=300.000 rays=162 maxBounces=20 maxTransmit=1
	10m) time: 15.16ms 16.43ms 15.58ms
	
	Parallel-CalcProbePreCalc: pos=(0.000,1.600,-4.000) range=300.000 rays=162 maxBounces=20 maxTransmit=0
	10m) time: 7.97ms 7.23ms 7.54ms
	
	
	
	Parallel-CalcProbePreCalc: pos=(0.000,1.600,-4.000) range=60.000 rays=162 maxBounces=20 maxTransmit=1
	10m) time: 14.47ms 15.72ms 14.78ms
	*/
}

deoalRTPTTraceSoundRays::~deoalRTPTTraceSoundRays(){
}



// Manegement
///////////////

void deoalRTPTTraceSoundRays::SetWorld(deoalAWorld *world, deoalRTWorldBVH *rtBVH){
	pWorld = world;
	pRTWorldBVH = rtBVH;
}

void deoalRTPTTraceSoundRays::SetProbeConfig(const deoalRayTraceConfig *probeConfig){
	pProbeConfig = probeConfig;
}

void deoalRTPTTraceSoundRays::SetPosition(const decDVector &position){
	pPosition = position;
}

void deoalRTPTTraceSoundRays::SetRange(float range){
	pRange = range;
}

void deoalRTPTTraceSoundRays::SetAttenuationParameters(float refDist, float rollOff, float distanceOffset){
	pRefDist = refDist;
	pRollOff = rollOff;
	pDistanceOffset = distanceOffset;
	pUseAttenuation = fabsf(pRefDist - 1.0f) > FLOAT_SAFE_EPSILON
		|| fabsf(pRollOff) > FLOAT_SAFE_EPSILON || fabsf(pDistanceOffset) > 0.001f;
}

void deoalRTPTTraceSoundRays::SetFirstRay(int firstRay){
	pFirstRay = firstRay;
}

#ifndef RTPTTSR_ONE_TASK_PER_RAY
void deoalRTPTTraceSoundRays::SetRayCount(int rayCount){
	pRayCount = rayCount;
}
#endif

void deoalRTPTTraceSoundRays::SetAddRayMinLength(float length){
	pAddRayMinLength = length;
}

void deoalRTPTTraceSoundRays::SetThresholdTransmit(float gain){
	pThresholdTransmit  = gain;
}

void deoalRTPTTraceSoundRays::SetThresholdReflect(float gain){
	pThresholdReflect = gain;
}

void deoalRTPTTraceSoundRays::SetMaxBounceCount(int count){
	pMaxBounceCount = count;
}

void deoalRTPTTraceSoundRays::SetMaxTransmitCount(int count){
	pMaxTransmitCount = count;
}

void deoalRTPTTraceSoundRays::SetInitialRayLength(float length){
	pInitialRayLength = length;
}

void deoalRTPTTraceSoundRays::SetLayerMask(const decLayerMask &layerMask){
	pLayerMask = layerMask;
}

void deoalRTPTTraceSoundRays::SetInverseRayTracing(bool inverseRayTracing){
	pInverseRayTracing = inverseRayTracing;
}

void deoalRTPTTraceSoundRays::SetDetectOutsideLength(double detectOutsideLength){
	pDetectOutsideLength = decMath::max(detectOutsideLength, 1.0);
}



void deoalRTPTTraceSoundRays::Run(){
	#ifdef WOVRAYHITSELEMENT_DO_TIMING
	pWOVRayHitsElement.StartTiming();
	#endif
	#ifdef RTWOVRAYHITSELEMENT_DO_TIMING
	pRTWOVRayHitsElement.StartTiming();
	#endif
	#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
	pRTWOVRayHitsClosest.StartTiming();
	#endif
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	pRTWOVRayBlocked.StartTiming();
	#endif
	
	#ifndef RTPTTSR_ONE_TASK_PER_RAY
	const decVector * const rayDirections = pProbeConfig->GetRayDirections() + pFirstRay;
	#endif
	
	pWOVRayHitsElement.SetLayerMask(pLayerMask);
	pWOVRayBlocked.SetLayerMask(pLayerMask);
	
	pSoundRayList.RemoveAllRays();
	#ifndef RTPTTSR_ONE_TASK_PER_RAY
	pSoundRayList.ReserveSize(pRayCount, pRayCount * pMaxBounceCount, pMaxTransmitCount);
		// assume 20 bounces per ray
	#else
	pSoundRayList.ReserveSize(1, pMaxBounceCount, pMaxTransmitCount);
	#endif
	pMeanFreePath = 0.0f;
	pRoomVolume = 0.0f;
	pRoomSurface = 0.0f;
	pSabineLow = 0.0f;
	pSabineMedium = 0.0f;
	pSabineHigh = 0.0f;
	pAbsorptionLow = 0.0f;
	pAbsorptionMedium = 0.0f;
	pAbsorptionHigh = 0.0f;
	pAbsorptionCount = 0;
	pFirstHitCount = 0;
	pMinExtend.SetZero();
	pMaxExtend.SetZero();
	
	pFirstHit = true;
	
	sTraceRay ray;
	ray.position = pPosition;
	ray.distance = 0.0f;
	ray.remainingDistance = pRange;
	ray.bounces = 0;
	ray.transmissions = 0;
	ray.transmittedRayIndex = -1;
	
	sTraceGain gain;
	gain.low = 1.0f;
	gain.medium = 1.0f;
	gain.high = 1.0f;
	
	sTraceAbsorptionSum absorptionSum;
	absorptionSum.low = 0.0f;
	absorptionSum.medium = 0.0f;
	absorptionSum.high = 0.0f;
	
	#ifndef RTPTTSR_ONE_TASK_PER_RAY
	for(pRayIndex=0; pRayIndex<pRayCount; pRayIndex++){
		ray.rayIndex = pSoundRayList.AddRay();
		ray.direction = rayDirections[pRayIndex];
		pTraceRay(ray, gain, absorptionSum);
	}
	#else
	ray.rayIndex = pSoundRayList.AddRay();
	ray.direction = pProbeConfig->GetRayDirections()[pFirstRay];
	ray.normal.SetZero();
	pTraceRay(ray, gain, absorptionSum);
	#endif
	
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	pRTWOVRayBlocked.EndTiming();
	#endif
	#ifdef RTWOVRAYHITSCLOSEST_DO_TIMING
	pRTWOVRayHitsClosest.EndTiming();
	#endif
	#ifdef RTWOVRAYHITSELEMENT_DO_TIMING
	pRTWOVRayHitsElement.EndTiming();
	#endif
	#ifdef WOVRAYHITSELEMENT_DO_TIMING
	pWOVRayHitsElement.EndTiming();
	#endif
}

void deoalRTPTTraceSoundRays::Finished(){
	// this task never runs alone. it is always a dependency and clean up by these tasks
	/*
	if(GetDependedOnBy().GetCount() == 0){
		pOwner.Enable(this);
	}
	*/
}



// Debugging
//////////////

decString deoalRTPTTraceSoundRays::GetDebugName() const{
	return "OpenAL-RTPTTraceSoundRays";
}

decString deoalRTPTTraceSoundRays::GetDebugDetails() const{
	return "";
}



// Protected Functions
////////////////////////

#define USE_STEPWISE_RAY_LENGTH 1

// #define DEBUG_CORRECTNESS

#ifdef DEBUG_CORRECTNESS
#include <dragengine/threading/deMutex.h>
static deMutex mutex;
#endif

void deoalRTPTTraceSoundRays::pTraceRay(const sTraceRay &ray, const sTraceGain &gain,
const sTraceAbsorptionSum &absorptionSum){
	#ifdef DEBUG_CORRECTNESS
		deoalRayTraceResult expected;
	#endif
	// use a multi-length testing approach.
	// 
	// sound range can easily be 300m or similar large value. by testing rays with the
	// entire distance a lot of collision testing is done which is hidden from sight.
	// this is required since the closest distance has to be found. to reduce the time
	// required the ray is first set to a length of pInitialRayLength. if this does
	// not result in a collision the length of the ray is doubled and checked again
	// but this time starting at the position where the previous ray test stopped.
	// the length is clamped to the range for the last check.
	// 
	// an alternative version would be to increment the ray size in equally sized length
	// intervals. it is though to be expected that if a ray is not hitting an obstacle
	// that the sound is located in a larger environment so doubling the length requires
	// less collision testing.
	// 
	// this version should be not slower in large environments because not hitting
	// anything does few collision tests. important is cutting down on collision tests
	// in small environments since those are very expensive
#ifdef USE_STEPWISE_RAY_LENGTH
	const deoalRayTraceHitElement *hitElement = NULL;
	decDVector rayOrigin(ray.position);
	float rayLength = pInitialRayLength;
	float rayOffset = 0.0f;
	int hitIndex = 0, hitCount;
	
	while(rayOffset < ray.remainingDistance){
		const float nextRayOffset = decMath::min(rayOffset + rayLength, ray.remainingDistance);
		const decDVector rayEnd(ray.position + ray.direction * nextRayOffset);
		rayLength *= 2.0f;
		
		hitElement = NULL;
		pRTResult.Clear();
		
		if(pRTWorldBVH){
			pRTWOVRayHitsClosest.SetRay(*pRTWorldBVH, rayOrigin, rayEnd - rayOrigin);
			pRTWOVRayHitsClosest.SetFrontFacing(true);
			pRTWOVRayHitsClosest.SetLimitDistance(1.0f);
			pRTWOVRayHitsClosest.VisitBVH(*pRTWorldBVH);
			
			if(pRTWOVRayHitsClosest.GetHasResult()){
				hitElement = &pRTWOVRayHitsClosest.GetResult();
			}
			
			#ifdef DEBUG_CORRECTNESS
				if(pOwner.GetAudioThread().GetDebug().GetLogCalcEnvProbe()){
					expected.Clear();
					pWOVRayHitsElement.SetResult(&expected);
					pWOVRayHitsElement.SetRay(rayOrigin, rayEnd - rayOrigin);
					pWOVRayHitsElement.VisitNode(*pWorld->GetOctree());
					pWOVRayHitsElement.SetResult(&pRTResult);
				}
			#endif
			
			#if 0
			if(pOwner.GetAudioThread().GetDebug().GetLogCalcEnvProbe()){
									mutex.Lock();
				pRTResult.Clear();
				pRTWOVRayHitsElement.SetRay(*pRTWorldBVH, rayOrigin, rayEnd - rayOrigin);
				pRTWOVRayHitsElement.VisitBVH(*pRTWorldBVH);
				
				const deoalRayTraceHitElement *e1 = NULL;
				int i;
				for(i=0; i<pRTResult.GetElementCount(); i++){
					const deoalRayTraceHitElement &e = pRTResult.GetElementAt(i);
					if(e.GetForwardFacing()){
						e1 = &e;
						break;
					}
				}
				
				if(e1){
					if(pRTWOVRayHitsClosest.GetHasResult()){
						const deoalRayTraceHitElement &e2 = pRTWOVRayHitsClosest.GetResult();
						
						if(/*e1->GetComponent() == e2.GetComponent()*/
							//&& e1->GetComponentFace() == e2.GetComponentFace()
							/*&&*/ fabsf( e1->GetDistance() - e2.GetDistance() ) < 1e-3f
							&& e1->GetForwardFacing() == e2.GetForwardFacing()
							//&& e1->GetNormal().IsEqualTo( e2.GetNormal(), 1e3f )
							&& e1->GetPoint().IsEqualTo(e2.GetPoint(), 1e3f)){
								pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Forward(+) Matching.",
									pFirstRay, ray.bounces, ray.transmissions);
						}else{
							pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Forward(+) Not Matching!",
								pFirstRay, ray.bounces, ray.transmissions);
							pRTResult.DebugPrint(pOwner.GetAudioThread(), "Expected: ");
							deoalRayTraceResult found;
							found.AddElement(pRTWOVRayHitsClosest.GetResult());
							found.DebugPrint(pOwner.GetAudioThread(), "Found: ");
							
									pRTWOVRayHitsClosest.SetRay(*pRTWorldBVH, rayOrigin, rayEnd - rayOrigin);
									pRTWOVRayHitsClosest.SetFrontFacing(true);
									pRTWOVRayHitsClosest.SetLimitDistance(1.0f);
									pRTWOVRayHitsClosest.VisitBVH(*pRTWorldBVH);
						}
						
					}else{
						pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Forward(+) Not Matching!",
							pFirstRay, ray.bounces, ray.transmissions);
						pRTResult.DebugPrint(pOwner.GetAudioThread(), "Expected: ");
					}
					
				}else{
					if(pRTWOVRayHitsClosest.GetHasResult()){
						pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Forward(-) Not Matching!",
							pFirstRay, ray.bounces, ray.transmissions);
						pRTResult.DebugPrint(pOwner.GetAudioThread(), "Expected: ");
						deoalRayTraceResult found;
						found.AddElement(pRTWOVRayHitsClosest.GetResult());
						found.DebugPrint(pOwner.GetAudioThread(), "Found: ");
						
					}else{
						pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Forward(-) Matching.",
							pFirstRay, ray.bounces, ray.transmissions);
					}
				}
									mutex.Unlock();
			}
			#endif
			
			#if 0
			if(pOwner.GetAudioThread().GetDebug().GetLogCalcEnvProbe()){
				deoalRayTraceResult expectedResult;
				pWOVRayHitsElement.SetRay(rayOrigin, rayEnd - rayOrigin);
				pWOVRayHitsElement.SetResult(&expectedResult);
				pWOVRayHitsElement.VisitNode(*pWorld->GetOctree());
				pWOVRayHitsElement.SetResult(&pRTResult);
				
				bool matching = true;
				if(pRTResult.GetElementCount() == expectedResult.GetElementCount()){
					const int count = expectedResult.GetElementCount();
					int i;
					for(i=0; i<count; i++){
						const deoalRayTraceHitElement &e1 = expectedResult.GetElementAt(i);
						const deoalRayTraceHitElement &e2 = pRTResult.GetElementAt(i);
						
						if(e1.GetComponent() == e2.GetComponent()
							&& e1.GetComponentFace() == e2.GetComponentFace()
							&& fabsf(e1.GetDistance() - e2.GetDistance()) < 1e-3f
							&& e1.GetForwardFacing() == e2.GetForwardFacing()
							&& e1.GetNormal().IsEqualTo(e2.GetNormal(), 1e3f)
							&& e1.GetPoint().IsEqualTo(e2.GetPoint(), 1e3f)){
								continue;
						}
						
						if(i == count - 1){
							matching = false;
							break;
						}
						
						const deoalRayTraceHitElement &e1b = expectedResult.GetElementAt(i + 1);
						const deoalRayTraceHitElement &e2b = pRTResult.GetElementAt(i + 1);
						
						if(e1.GetComponent() == e2b.GetComponent()
							&& e1.GetComponentFace() == e2b.GetComponentFace()
							&& fabsf(e1.GetDistance() - e2b.GetDistance()) < 1e-3f
							&& e1.GetForwardFacing() == e2b.GetForwardFacing()
							&& e1.GetNormal().IsEqualTo(e2b.GetNormal(), 1e3f)
							&& e1.GetPoint().IsEqualTo(e2b.GetPoint(), 1e3f)
							
							&& e1b.GetComponent() == e2.GetComponent()
							&& e1b.GetComponentFace() == e2.GetComponentFace()
							&& fabsf(e1b.GetDistance() - e2.GetDistance()) < 1e-3f
							&& e1b.GetForwardFacing() == e2.GetForwardFacing()
							&& e1b.GetNormal().IsEqualTo(e2.GetNormal(), 1e3f)
							&& e1b.GetPoint().IsEqualTo(e2.GetPoint(), 1e3f)){
								i++;
								continue;
						}
						
						matching = false;
						break;
					}
				}else{
					matching = false;
				}
				
				if(matching){
					pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Matching.",
						pFirstRay, ray.bounces, ray.transmissions);
				}else{
					pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Not Matching!",
						pFirstRay, ray.bounces, ray.transmissions);
					expectedResult.DebugPrint(pOwner.GetAudioThread(), "Expected: ");
					pRTResult.DebugPrint(pOwner.GetAudioThread(), "Found: ");
				}
			}
			#endif
			
		}else{
			pWOVRayHitsElement.SetRay(rayOrigin, rayEnd - rayOrigin);
			pWOVRayHitsElement.VisitNode(*pWorld->GetOctree());
			
			// find result. use the first forward facing element. backward facing elements are
			// ignored because the probe can be inside geometry. without skipping backwards facing
			// faces this can result in incorrect probe data
			hitCount = pRTResult.GetElementCount();
			
			for(hitIndex=0; hitIndex<hitCount; hitIndex++){
				const deoalRayTraceHitElement &he = pRTResult.GetElementAt(hitIndex);
				if(he.GetForwardFacing()){
					hitElement = &he;
					break;
				}
			}
		}
		
		if(hitElement){
			break;
		}
		
		rayOffset = nextRayOffset;
		rayOrigin = rayEnd;
	}
	
	const decDVector scaledDirection(ray.direction * rayOffset);
	
#else
	const decDVector scaledDirection(ray.direction * ray.remainingDistance);
	const deoalRayTraceHitElement *hitElement = NULL;
	int hitIndex, hitCount;
	
	pRTResult.Clear();
	pWOVRayHitsElement.SetRay(ray.position, scaledDirection);
	pWOVRayHitsElement.VisitNode(*pWorld->GetOctree());
	
	// find result. use the first forward facing element. backward facing elements are
	// ignored because the probe can be inside geometry. without skipping backwards facing
	// faces this can result in incorrect probe data
	hitCount = pRTResult.GetElementCount();
	hitElement = NULL;
	
	for(hitIndex=0; hitIndex<hitCount; hitIndex++){
		const deoalRayTraceHitElement &he = pRTResult.GetElementAt(hitIndex);
		if(he.GetForwardFacing()){
			hitElement = &he;
			break;
		}
	}
	
	const float rayOffset = 0.0f;
#endif
	
	// if ray is not too short add it to the sound ray list. also do not add the ray if this
	// it has not bounced yet. direct contribution is calculated elsewhere. this calculation
	// is only for the indirect contribution
	const float hitDistance = hitElement ? hitElement->GetDistance() + rayOffset : ray.remainingDistance;
	
	if(ray.bounces > 0 && hitDistance >= pAddRayMinLength){
		deoalSoundRaySegment &segment = pSoundRayList.GetSegmentAt(pSoundRayList.AddSegment());
		
		deoalSoundRay &sray = ray.transmittedRayIndex != -1
			? pSoundRayList.GetTransmittedRayAt(ray.transmittedRayIndex)
			: pSoundRayList.GetRayAt(ray.rayIndex);
		sray.IncSegmentCount();
		
		segment.SetPosition(ray.position - pPosition);
		segment.SetDirection(ray.direction);
		segment.SetNormal(ray.normal);
		segment.SetLength(hitDistance);
		segment.SetDistance(ray.distance);
		segment.SetGainLow(gain.low);
		segment.SetGainMedium(gain.medium);
		segment.SetGainHigh(gain.high);
		segment.SetAbsorptionSumLow(absorptionSum.low);
		segment.SetAbsorptionSumMedium(absorptionSum.medium);
		segment.SetAbsorptionSumHigh(absorptionSum.high);
		segment.SetBounceCount(ray.bounces);
		segment.SetTransmittedCount(ray.transmissions);
		
		// add to mean free path. if path are too short they are not added which overall counts
		// them as having a length of 0. this is a small error. more interesting is how to deal
		// with rays hitting nothing. the value is used in listening calculation for two things:
		// receiver sphere radius and reverberation time. the calculations are like this:
		// 
		//   receiverRadius = meanFreePath * sqrt(pi * 2 / rayCount)
		//   reverberationTime = 13.8 * -(meanFreePath * INV_SOUND_SPEED) / log(1 - absorption)
		// 
		// assuming we stand on a plane then all sound rays go into or reflect into nothing
		// after the first bounce. what happens to the receiver radius and revrberation time?
		// the reverberation time should be 0 since no reverbe returns from hitting nothing.
		// in this case mean-free-path has to be 0 to achieve this result. receiver radius on
		// the other hand would be wrong if mean-free-path is 0. in this case the mean-free-path
		// should be the maximum length the rays can travel.
		// 
		// both situations require different values. how to get them to match? the receiver
		// radius is going to win to make this work. for reverberation time we also need to
		// absorption. if nothing is hit 1 is used as absorption which is correct: all sound
		// is absorbed by hitting nothing. thus the mean-free-path of maximum length value
		// will be compensated by the absorption value of 1.
		// 
		// for this reason pRange is added if nothing is hit otherwise the hit distance
		/*
		if(hitElement){
			pMeanFreePath += hitDistance;
			
		}else{
			pMeanFreePath += pRange;
		}
		*/
		pMeanFreePath += hitDistance;
	}
	
	// ray hits nothing and ends
	if(! hitElement){
		const decDVector rayPosition(ray.position + scaledDirection);
		pUpdateExtends(rayPosition);
		DetectRayOutside(ray, rayPosition);
		return;
	}
	
	// ray hits something
	const decDVector &hitPoint = hitElement->GetPoint();
	pUpdateExtends(hitPoint);
	
	const decDVector &hitNormal = hitElement->GetNormal();
	const double dotOut = -(hitNormal * ray.direction);
	
	// calculate the absorption and transmission gain at the hit point. physically correctly
	// for inverse listening the exciting face should be used but this way we can not speed
	// up the ray tracing. using the front face we can calculate how far the ray can penetrate
	// into the wall before becoming inaudible. the back face has to be found inside this
	// range which optimizes the ray tracing
	deoalAComponent &component = *hitElement->GetComponent();
	const deoalModelFace &face = component.GetModel()->GetFaceAt(hitElement->GetComponentFace());
	const deoalAComponentTexture &texture = component.GetModelTextureAt(face.GetTexture());
	
	const float absorbedLow = gain.low * texture.GetAbsorptionLow();
	const float absorbedMedium = gain.medium * texture.GetAbsorptionMedium();
	const float absorbedHigh = gain.high * texture.GetAbsorptionHigh();
	
	const float nonAbsLow = gain.low - absorbedLow;
	const float nonAbsMedium = gain.medium - absorbedMedium;
	const float nonAbsHigh = gain.high - absorbedHigh;
	
	//const deoalAComponentTexture &textureTransmit = textureBack ? *textureBack : texture;
	const deoalAComponentTexture &textureTransmit = texture;
	
	// find thickness for transmission calculation. this is tricky and has some problematic
	// corner cases. basically the first back facing hit element is looked for. if models
	// are properly closed off this should be the exit point of the model. if models are not
	// properly closed off this can cause escaping rays. the same happens if different models
	// are penetrating each other. all of these situations can cause rays to leak and this
	// is a major problem. there is no way to figure out if this is the case.
	// 
	// right now we use a counter to figure out when we found the back face. whenever a front
	// face is hit the counter is incremented. when a backface is hit the counter is
	// decremented. if the counter becomes 0 all front facing hits should be paired with a
	// back facing hit. this helps in situations of penetrating geometry
	// 
	// if geometry is close to each other or touching (for example door panels touching in a
	// closed state) errors do result. this happens because transmitted rays are moved forward
	// by pBackStepDistance to avoid collisions straight away. in this case the next front
	// facing collision is missed and rays penetrate with wrong gain through geometry which
	// has audible effect. to counter this problem all collision inside pBackStepDistance range
	// after the back facing hit element are applied too while face winding counting. this can
	// potentially result in a negative face winding count but this is better than wrong ray
	// penetration
	const deoalRayTraceHitElement *hitElementBack = NULL;
// 	const deoalAComponentTexture *textureBack = NULL;
	float hitElementBackOffset = 0.0f;
	
	if(ray.transmissions < pMaxTransmitCount){
		if(pRTWorldBVH){
			hitElementBackOffset = hitDistance;
			pRTResult.Clear();
			hitIndex = -1;
			
			const float maxThickness = decMath::max(
				textureTransmit.GetTransmissionLow() * nonAbsLow,
				textureTransmit.GetTransmissionMedium() * nonAbsMedium,
				textureTransmit.GetTransmissionHigh() * nonAbsHigh);
			if(maxThickness > 0.01f){
				pRTWOVRayHitsElement.SetRay(*pRTWorldBVH, hitPoint, ray.direction * maxThickness);
				pRTWOVRayHitsElement.VisitBVH(*pRTWorldBVH);
			}
		}
		
		const int count = pRTResult.GetElementCount();
		int findBackCounter = 1;
		
		for(hitIndex++; hitIndex<count; hitIndex++){
			const deoalRayTraceHitElement &he = pRTResult.GetElementAt(hitIndex);
			
			if(he.GetComponent() == hitElement->GetComponent()
			&& he.GetComponentFace() == hitElement->GetComponentFace()){
				continue;
			}
			
			if(he.GetForwardFacing()){
				findBackCounter++;
				continue;
			}
			
			findBackCounter--;
			if(findBackCounter > 0){
				continue;
			}
			
			hitElementBack = &he;
			
	// 		const float thresholdDistance = he.GetDistance() + pBackStepDistance;
			const deoalRayTraceHitElement *heLast = &he;
			for(hitIndex++; hitIndex<count; hitIndex++){
				const deoalRayTraceHitElement &he2 = pRTResult.GetElementAt(hitIndex);
	// 			if( he2.GetDistance() > thresholdDistance ){
				if(he2.GetDistance() > heLast->GetDistance() + pBackStepDistance){
					hitIndex--;
					break;
				}
				
				if(he2.GetForwardFacing()){
					findBackCounter++;
					
				}else{
					findBackCounter--;
					hitElementBack = &he2;
				}
				
				heLast = &he2;
			}
			
			if(findBackCounter > 0){
				hitElementBack = NULL;
				continue;
			}
			
// 			if( pInverseRayTracing ){
// 				deoalAComponent &component = *hitElementBack->GetComponent();
// 				textureBack = &component.GetModelTextureAt( component.GetModel()->GetFaceAt(
// 					hitElementBack->GetComponentFace() ).GetTexture() );
// 			}
			break;
		}
	}
	
	#ifdef DEBUG_CORRECTNESS
	if(pRTWorldBVH && pOwner.GetAudioThread().GetDebug().GetLogCalcEnvProbe()){
		mutex.Lock();
		
		const float maxThickness = decMath::max(
			textureTransmit.GetTransmissionLow() * nonAbsLow,
			textureTransmit.GetTransmissionMedium() * nonAbsMedium,
			textureTransmit.GetTransmissionHigh() * nonAbsHigh);
		
		// expected
		const deoalRayTraceHitElement *e1 = NULL;
		int hi;
		for(hi=0; hi<expected.GetElementCount(); hi++){
			const deoalRayTraceHitElement &e = expected.GetElementAt(hi);
			if(e.GetForwardFacing()){
				e1 = &e;
				break;
			}
		}
		
		const deoalRayTraceHitElement *e2 = NULL;
		int findBackCounter = 1;
		for(hi++; hi<expected.GetElementCount(); hi++){
			const deoalRayTraceHitElement &he = expected.GetElementAt(hi);
			if(he.GetForwardFacing()){
				findBackCounter++;
				continue;
			}
			findBackCounter--;
			if(findBackCounter > 0){
				continue;
			}
			e2 = &he;
			
			const deoalRayTraceHitElement *heLast = &he;
			for(hi++; hi<expected.GetElementCount(); hi++){
				const deoalRayTraceHitElement &he2 = expected.GetElementAt(hi);
				if(he2.GetDistance() > heLast->GetDistance() + pBackStepDistance){
					hi--;
					break;
				}
				if(he2.GetForwardFacing()){
					findBackCounter++;
				}else{
					findBackCounter--;
					e2 = &he2;
				}
				heLast = &he2;
			}
			
			if(findBackCounter > 0){
				e2 = NULL;
				continue;
			}
			break;
		}
		
		// compare to found
		bool matching = true;
		if(e1){
			if(! hitElement
			|| e1->GetForwardFacing() != hitElement->GetForwardFacing()
			|| ! e1->GetPoint().IsEqualTo(hitElement->GetPoint(), 1e3f)){
				pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Forward(+) Not Matching! (maxThick=%.3f)",
					pFirstRay, ray.bounces, ray.transmissions, maxThickness);
				matching = false;
			}
		}else if(hitElement){
			pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Forward(-) Not Matching! (maxThick=%.3f)",
				pFirstRay, ray.bounces, ray.transmissions, maxThickness);
			matching = false;
		}
		
		if(ray.transmissions < pMaxTransmitCount && maxThickness > 0.01f){
			if(e2 && (e2->GetPoint() - e1->GetPoint()).Length() <= maxThickness){
				if(! hitElementBack
				|| e2->GetForwardFacing() != hitElementBack->GetForwardFacing()
				|| ! e2->GetPoint().IsEqualTo(hitElementBack->GetPoint(), 1e3f)){
					pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Backward(+) Not Matching! (maxThick=%.3f)",
						pFirstRay, ray.bounces, ray.transmissions, maxThickness);
					matching = false;
				}
			}else if(hitElementBack){
				pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Backward(-) Not Matching! (maxThick=%.3f)",
					pFirstRay, ray.bounces, ray.transmissions, maxThickness);
				matching = false;
			}
		}
		
		if(matching){
			pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: Matching (maxThick=%.3f)",
				pFirstRay, ray.bounces, ray.transmissions, maxThickness);
		}else{
			expected.DebugPrint(pOwner.GetAudioThread(), "Expected: ");
			deoalRayTraceResult found;
			if(hitElement){
				found.AddElement(*hitElement);
			}
			if(hitElementBack){
				found.AddElement(*hitElementBack);
			}
			found.DebugPrint(pOwner.GetAudioThread(), "Found: ");
			pRTResult.DebugPrint(pOwner.GetAudioThread(), "Last RTResult: ");
		}
		
		mutex.Unlock();
	}
	#endif
	
	// sum up absorptions for reverberation calculation. there is no correct way to weight
	// the sum physically correctly with the way ray-tracing works so just averaging them
	// is as correct/wrong as anything else (but faster)
	pAbsorptionLow += textureTransmit.GetAbsorptionLow();
	pAbsorptionMedium += textureTransmit.GetAbsorptionMedium();
	pAbsorptionHigh += textureTransmit.GetAbsorptionHigh();
	pAbsorptionCount++;
	
	sTraceGain transmittedGain;
	transmittedGain.low = 0.0f;
	transmittedGain.medium = 0.0f;
	transmittedGain.high = 0.0f;
	
	sTraceAbsorptionSum transmittedAbsorptionSum;
	transmittedAbsorptionSum.low = 0.0f;
	transmittedAbsorptionSum.medium = 0.0f;
	transmittedAbsorptionSum.high = 0.0f;
	
	// the transmitted rays are not traced the transmitted gain is lost. this would
	// result in weaker reflections
	if(hitElementBack){
		const float thickness = hitElementBack->GetDistance()
			+ hitElementBackOffset - hitElement->GetDistance();
		
		transmittedGain.low = decMath::linearStep(thickness, 0.0f,
			textureTransmit.GetTransmissionLow(), nonAbsLow, 0.0f);
		transmittedGain.medium = decMath::linearStep(thickness, 0.0f,
			textureTransmit.GetTransmissionMedium(), nonAbsMedium, 0.0f);
		transmittedGain.high = decMath::linearStep(thickness, 0.0f,
			textureTransmit.GetTransmissionHigh(), nonAbsHigh, 0.0f);
		
		transmittedAbsorptionSum.low = absorptionSum.low + textureTransmit.GetAbsorptionLow();
		transmittedAbsorptionSum.medium = absorptionSum.medium + textureTransmit.GetAbsorptionMedium();
		transmittedAbsorptionSum.high = absorptionSum.high + textureTransmit.GetAbsorptionHigh();
		
		/*
		pOwner.GetAudioThread().GetLogger().LogInfoFormat("Ray #%d:%d:%d: thickness=%.3f gain=(%.3f,%.3f,%.3f)",
			pFirstRay + pRayIndex, ray.bounces, ray.transmissions, thickness,
			transmittedGain.low, transmittedGain.medium, transmittedGain.high);
		*/
	}
	
	sTraceRay reflectedRay;
	reflectedRay.rayIndex = ray.rayIndex;
	reflectedRay.transmittedRayIndex = ray.transmittedRayIndex;
	reflectedRay.distance = ray.distance + hitDistance;
	reflectedRay.remainingDistance = ray.remainingDistance - hitDistance;
	
	sTraceGain reflectedGain;
	reflectedGain.low = nonAbsLow - transmittedGain.low;
	reflectedGain.medium = nonAbsMedium - transmittedGain.medium;
	reflectedGain.high = nonAbsHigh - transmittedGain.high;
	
	sTraceAbsorptionSum reflectedAbsorptionSum;
	reflectedAbsorptionSum.low = absorptionSum.low + texture.GetAbsorptionLow();
	reflectedAbsorptionSum.medium = absorptionSum.medium + texture.GetAbsorptionMedium();
	reflectedAbsorptionSum.high = absorptionSum.high + texture.GetAbsorptionHigh();
	
	// update room parameters with first hit rays
	if(ray.bounces == 0){
		float roomFactor = hitDistance * hitDistance;
		pRoomVolume += roomFactor * hitDistance;
		
		roomFactor *= 1.0f + (1.0f - (float)dotOut) * 2.0f;
			// apply first a scale factor to make area larger if at shallow angle. the value of
			// the scale factor is found experimental and is not based on calculations. this would
			// be difficult to do and most probably not much more true than this version here
		
		pSabineMedium += roomFactor * texture.GetAbsorptionMedium();
		pSabineLow += roomFactor * texture.GetAbsorptionLow();
		pSabineHigh += roomFactor * texture.GetAbsorptionHigh();
		
		pRoomSurface += roomFactor;
		
		pFirstHitCount++;
	}
	
	// the distance attenuation is not applied to the gain directly. if this would be done
	// the attenuation of previous bounces multiplies into the attenuation of this bounce
	// causing gain to drop exponentially. also if the attenuation would be calculated only
	// over the distance travelled since the start of the ray the result would be too low.
	// for example the total distance is 6 then the attenuation is 1/6. if this is now split
	// over one bounce after 3m the attenuation would be (1/3)/3 which is 1/9 instead of 1/6.
	// thus the attenuation calculation is only used to check when the gain becomes inaudible.
	// the final calculation step will calculate the distance attenuation
	//const float attGain = pEnvProbe->AttenuatedGain( reflectedRay.distance ); // * ( float )dotOut;
	
	// NOTE if source centric tracing is done pDistanceOffset can be larger than 0 and far
	//      away sound sources can be traced. if listener centris pDistanceOffset is always 0
	const float attGain = Attenuate(reflectedRay.distance);
	
	// WARNING
	// store transmission parameters aside. this is required since bouncing rays overwites
	// the ray tracing result by the time the transmission ray is cast
	// 
	// these values are NOT save to be used anymore (any anything potentially referencing them)!
	// - pWOVRayHitsElement
	// - pRTResult
	// - hitElement
	
// 	if( pFirstRay == 22 ){
// 		const float thickness = hitElementBack ? hitElementBack->GetDistance() - hitElement->GetDistance() : -1.0f;
// 		pOwner.GetAudioThread().GetLogger().LogInfoFormat(
// 			"TraceRay: seg=%d boun=%d tra=%d attGain=%f in(%f,%f,%f) abs=(%f,%f,%f) thi=%f tra=(%f,%f,%f) refl=(%f,%f,%f)",
// 			pSoundRayList.GetRayAt( ray.rayIndex ).GetSegmentCount(), ray.bounces, ray.transmissions, 
// 			attGain, gain.low, gain.medium, gain.high, absorbedLow, absorbedMedium, absorbedHigh,
// 			thickness, transmittedGain.low, transmittedGain.medium, transmittedGain.high,
// 			reflectedGain.low, reflectedGain.medium, reflectedGain.high );
// 	}
#if 0
	if(pFirstRay == 22 && pSoundRayList.GetRayAt(ray.rayIndex).GetSegmentCount() == 1
	&& ray.bounces == 1 && ray.transmissions == 0){
		pRTResult.DebugPrint(pOwner.GetAudioThread(), "TraceSound");
		
		/*
Ray-Trace Result (3 elements):
- distance=2.505 point=(-18.976,0.959,56.696) component=(-19.100,0.000,56.700)
  model=/model/prop/door2.demodel face=64 frontFacing=1
- distance=2.842 point=(-19.240,0.751,56.666) component=(-19.100,0.000,56.700)
  model=/model/prop/door2.demodel face=58 frontFacing=0
- distance=4.063 point=(-20.197,0.000,56.560) component=(-12.900,0.000,56.700)
  model=/model/room/room6.demodel face=176 frontFacing=1
ApplyIndirectGains ray=22 seg=2 bounce=3 trans=1 gain=0.258258,0.045015,0.000000 tcds=0.620091 lambda=1.231206 halfDri=0.637743 dri=1.275486 factor=0.432647
EAX: 0.109070 1.000000 0.000000 (0.078889 0.109070; 0.000000 0.000000)

Ray-Trace Result (5 elements):
- distance=2.510 point=(-18.980,0.955,56.700) component=(-19.100,0.000,56.700)
  model=/model/prop/door2.demodel face=34 frontFacing=1
- distance=2.513 point=(-18.982,0.954,56.700) component=(-19.100,0.000,56.700)
  model=/model/prop/door2.demodel face=33 frontFacing=0
- distance=2.513 point=(-18.982,0.954,56.700) component=(-19.100,0.000,56.700)
  model=/model/prop/door2.demodel face=60 frontFacing=1
- distance=2.843 point=(-19.240,0.751,56.671) component=(-19.100,0.000,56.700)
  model=/model/prop/door2.demodel face=58 frontFacing=0
- distance=4.063 point=(-20.197,0.000,56.565) component=(-12.900,0.000,56.700)
  model=/model/room/room6.demodel face=176 frontFacing=1
ApplyIndirectGains ray=22 seg=2 bounce=3 trans=1 gain=0.371371,0.286700,0.218773 tcds=0.615435 lambda=1.231660 halfDri=0.641887 dri=1.283775 factor=0.435830
EAX: 0.108675 1.000000 0.063526 (0.080500 0.108675; 0.006904 0.000000)
		*/
	}
#endif
	
	// transmit ray if still audible. we have to add the transmitted ray before continue
	// bouncing to keep the order intact. the actual transmission is done after bouncing
	int bounceTransmissions = ray.transmissions;
	sTraceRay transmittedRay;
	transmittedRay.transmittedRayIndex = -1;
	
	if(transmittedGain.low * attGain > pThresholdTransmit
	|| transmittedGain.medium * attGain > pThresholdTransmit
	|| transmittedGain.high * attGain > pThresholdTransmit){
		const float hitDistanceBack = hitElementBack->GetDistance() + hitElementBackOffset + rayOffset;
		bounceTransmissions++;
		transmittedRay.rayIndex = ray.rayIndex;
		transmittedRay.transmissions = bounceTransmissions; //ray.transmissions + 1;
		transmittedRay.distance = ray.distance + hitDistanceBack;
		transmittedRay.remainingDistance = ray.remainingDistance - hitDistanceBack;
		transmittedRay.position = hitElementBack->GetPoint() + ray.direction * pBackStepDistance;
		transmittedRay.direction = ray.direction;
		transmittedRay.normal = ray.normal;
		transmittedRay.bounces = ray.bounces;
		
		transmittedRay.transmittedRayIndex = pSoundRayList.AddTransmittedRay();
		if(ray.transmittedRayIndex != -1){
			pSoundRayList.GetTransmittedRayAt(ray.transmittedRayIndex).IncTransmittedRayCount();
		}else{
			pSoundRayList.GetRayAt(ray.rayIndex).IncTransmittedRayCount();
		}
		
		/*
		pOwner.GetAudioThread().GetLogger().LogInfoFormat(
			"Transmit #%d:%d:%d: thickness=%.3f gain=(%.3f,%.3f,%.3f) origin=(%.3f,%.3f,%.3f)",
			pFirstRay + pRayIndex, ray.bounces, ray.transmissions,
			hitElementBack->GetDistance() - hitElement->GetDistance(),
			transmittedGain.low, transmittedGain.medium, transmittedGain.high,
			(hitElementBack->GetPoint() + ray.direction * pBackStepDistance).x,
			(hitElementBack->GetPoint() + ray.direction * pBackStepDistance).y,
			(hitElementBack->GetPoint() + ray.direction * pBackStepDistance).z);
		*/
	}
	
	// bounce ray and continue if still audible
	if(ray.bounces < pMaxBounceCount && (
	   reflectedGain.low * attGain > pThresholdReflect
	|| reflectedGain.medium * attGain > pThresholdReflect
	|| reflectedGain.high * attGain > pThresholdReflect)){
		/*
		pOwner.GetAudioThread().GetLogger().LogInfoFormat("(%p %d %d) Bounce: dist=%3f rem=%.3f gain=(%.3f,%.3f,%.3f) "
			" refl=(%.3f,%.3f,%.3f) abs=(%.3f,%.3f,%.3f) tra=(%.3f,%.3f,%.3f)",
			this, pRayIndex, ray.bounce + 1, reflectedRay.distance, reflectedRay.remainingDistance,
			gain.low, gain.medium, gain.high, reflectedGain.low, reflectedGain.medium, reflectedGain.high,
			absorbedLow, absorbedMedium, absorbedHigh, transmitLow, transmitMedium, transmitHigh);
		*/
		
		reflectedRay.bounces = ray.bounces + 1;
		
		// if we start right at the collision point chances are the next ray check hits
		// the same element we just hit with 0 distance causing a false collision which
		// in turn kills off the entire ray on the spot. to avoid this problem we step
		// back on the ray a tiny bit and use this as ray origin. this introduces a small
		// error in the calculation which is though neglectable
		reflectedRay.transmissions = bounceTransmissions; //ray.transmissions;
		reflectedRay.position = hitPoint - ray.direction * pBackStepDistance;
		reflectedRay.direction = ray.direction + hitNormal * (dotOut * 2.0);
		reflectedRay.normal = hitNormal;
		pTraceRay(reflectedRay, reflectedGain, reflectedAbsorptionSum);
		
// 	}else{
// 		pOwner.GetAudioThread().GetLogger().LogInfoFormat( "(%p %d) Died out after %.3f", this, pRayIndex, distance );
		
	}else{
		// detect if ray is outside. has to be done if there is no hit or ray stops bouncing
		DetectRayOutside(ray, ray.position + scaledDirection);
	}
	
	// transmit ray if still audible. we can only rely on the parameters we have stored aside
	// since the previous bouncings have modified the ray tracing results
	if(transmittedRay.transmittedRayIndex != -1){
		// adjust the first segment and transmitted ray count. this is required since bouncing
		// can add more segments and transmitted rays before this transmitted ray is processed
		deoalSoundRay &tray = pSoundRayList.GetTransmittedRayAt(transmittedRay.transmittedRayIndex);
		tray.SetFirstSegment(pSoundRayList.GetSegmentCount());
		tray.SetFirstTransmittedRay(pSoundRayList.GetTransmittedRayCount());
		
		pTraceRay(transmittedRay, transmittedGain, transmittedAbsorptionSum);
	}
}

void deoalRTPTTraceSoundRays::pUpdateExtends(const decDVector &position){
	if(pFirstHit){
		pMinExtend = pMaxExtend = position;
		pFirstHit = false;
		
	}else{
		pMinExtend.SetSmallest(position);
		pMaxExtend.SetLargest(position);
	}
}

float deoalRTPTTraceSoundRays::Attenuate(float distance) const{
	if(pUseAttenuation){
		return pRefDist / (pRefDist + pRollOff * decMath::max(distance + pDistanceOffset - pRefDist, 0.0f));
	}
	return 1.0f;
}

void deoalRTPTTraceSoundRays::DetectRayOutside(const sTraceRay &ray, const decDVector &position){
	// to handle outside situations better it is required to know if the ray is outside.
	// the correct solution would be to trace the ray to infinity and check if no geometry
	// is hit. this exhaustive search would be expensive. instead we do a simple is-ray-blocked
	// check with the ray extended along the last direction by a fixed amount. if something
	// is hit during that time it is likely the ray is not outside. this is still reducing
	// performance but less than doing a full search.
	// 
	// NOTE rays are added with the outside flag set to false
	if(IsRayBlocked(position, ray.direction * pDetectOutsideLength)){
		return;
	}
	
	(ray.transmittedRayIndex != -1
		? pSoundRayList.GetTransmittedRayAt(ray.transmittedRayIndex)
		: pSoundRayList.GetRayAt(ray.rayIndex)).SetOutside(true);
}

bool deoalRTPTTraceSoundRays::IsRayBlocked(const decDVector &position, const decDVector &direction){
	if(pRTWorldBVH){
		pRTWOVRayBlocked.SetRay(*pRTWorldBVH, position, direction);
		pRTWOVRayBlocked.SetBlocked(false);
		pRTWOVRayBlocked.VisitBVH(*pRTWorldBVH);
		return pRTWOVRayBlocked.GetBlocked();
		
	}else{
		pWOVRayBlocked.SetRay(position, direction);
		pWOVRayBlocked.SetBlocked(false);
		pWOVRayBlocked.VisitNode(*pWorld->GetOctree());
		return pWOVRayBlocked.GetBlocked();
	}
}
