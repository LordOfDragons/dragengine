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

#include "deoalRTPTTraceSoundRays.h"
#include "deoalRTPTTraceSoundRaysFinish.h"
#include "../deoalRayTraceConfig.h"
#include "../../../deAudioOpenAL.h"
#include "../../../deoalBasics.h"
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"

#include <dragengine/common/exceptions.h>



// Constants
//////////////

/**
 * \brief Reverberation time factor.
 * 
 * 24.0 * log( 10.0 ) / speedSound
 */
#define REVERB_TIME_FACTOR 0.1611138f



// Class deoalRTPTaskEnvProbe
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTPTTraceSoundRaysFinish::deoalRTPTTraceSoundRaysFinish(deoalRTParallelEnvProbe &owner) :
deParallelTask(&owner.GetAudioThread().GetOal()),
pOwner(owner),
pRange(0.0f),
pProbeConfig(NULL),
pSoundRayList(NULL),
pRoomParameters(NULL)
{
	SetMarkFinishedAfterRun(true);
	SetLowPriority(true);
}

deoalRTPTTraceSoundRaysFinish::~deoalRTPTTraceSoundRaysFinish(){
}



// Manegement
///////////////

void deoalRTPTTraceSoundRaysFinish::AddDependencies(const decPointerList &tasks){
	const int count = tasks.GetCount();
	int i;
	
	pTasks.RemoveAll();
	
	for(i=0; i<count; i++){
		deParallelTask * const task = (deParallelTask*)tasks.GetAt(i);
		AddDependsOn(task);
		pTasks.Add(task);
	}
}

void deoalRTPTTraceSoundRaysFinish::SetRange(float range){
	pRange = range;
}

void deoalRTPTTraceSoundRaysFinish::SetProbeConfig(const deoalRayTraceConfig *probeConfig){
	pProbeConfig = probeConfig;
}

void deoalRTPTTraceSoundRaysFinish::SetSoundRayList(deoalSoundRayList *soundRayList){
	pSoundRayList = soundRayList;
}

void deoalRTPTTraceSoundRaysFinish::SetRoomParameters(
deoalRTParallelEnvProbe::sRoomParameters *roomParameters){
	pRoomParameters = roomParameters;
}



void deoalRTPTTraceSoundRaysFinish::Run(){
	try{
		pRun();
		
	}catch(...){
		Cancel();
		pOwner.FinishTaskFinished(this);
		throw;
	}
	
	pOwner.FinishTaskFinished(this);
}

void deoalRTPTTraceSoundRaysFinish::Finished(){
	if(IsCancelled()){
		pOwner.FinishTaskFinished(this);
	}
	pOwner.Enable(this);
}



// Debugging
//////////////

decString deoalRTPTTraceSoundRaysFinish::GetDebugName() const{
	return "OpenAL-RTPTTraceSoundRaysFinish";
}

decString deoalRTPTTraceSoundRaysFinish::GetDebugDetails() const{
	return "";
}



// Private Functions
//////////////////////

void deoalRTPTTraceSoundRaysFinish::pRun(){
	// collect and apply parameters from tasks
	pRoomParameters->meanFreePath = 0.0f;
	pRoomParameters->sabineLow = 0.0f;
	pRoomParameters->sabineMedium = 0.0f;
	pRoomParameters->sabineHigh = 0.0f;
	pRoomParameters->roomVolume = 0.0f;
	pRoomParameters->roomSurface = 0.0f;
	pRoomParameters->avgAbsorptionLow = 0.0f;
	pRoomParameters->avgAbsorptionMedium = 0.0f;
	pRoomParameters->avgAbsorptionHigh = 0.0f;
	int absorptionCount = 0;
	int firstHitCount = 0;
	int i;
	
	// collect data from tasks
	const int taskCount = pTasks.GetCount();
	int soundRayTransmittedRayCount = 0;
	int soundRaySegmentCount = 0;
	int soundRayCount = 0;
	
	for(i=0; i<taskCount; i++){
		const deoalRTPTTraceSoundRays &task = *((deoalRTPTTraceSoundRays*)pTasks.GetAt(i));
		
		if(task.IsCancelled()){
			DETHROW(deeInvalidAction); // task failed
		}
		
		const deoalSoundRayList &srl = task.GetSoundRayList();
		soundRayCount += srl.GetRayCount();
		soundRaySegmentCount += srl.GetSegmentCount();
		soundRayTransmittedRayCount += srl.GetTransmittedRayCount();
	}
	
	pSoundRayList->RemoveAllRays();
	pSoundRayList->ReserveSize(soundRayCount, soundRaySegmentCount, soundRayTransmittedRayCount);
	
	for(i=0; i<taskCount; i++){
		const deoalRTPTTraceSoundRays &task = *((deoalRTPTTraceSoundRays*)pTasks.GetAt(i));
		
		if(i == 0){
			pRoomParameters->minExtend = task.GetMinExtend();
			pRoomParameters->maxExtend = task.GetMaxExtend();
			
		}else{
			pRoomParameters->minExtend.SetSmallest(task.GetMinExtend());
			pRoomParameters->maxExtend.SetLargest(task.GetMaxExtend());
		}
		
		*pSoundRayList += task.GetSoundRayList();
		pRoomParameters->meanFreePath += task.GetMeanFreePath();
		
		pRoomParameters->roomVolume += task.GetRoomVolume();
		pRoomParameters->roomSurface += task.GetRoomSurface();
		pRoomParameters->sabineLow += task.GetSabineLow();
		pRoomParameters->sabineMedium += task.GetSabineMedium();
		pRoomParameters->sabineHigh += task.GetSabineHigh();
		firstHitCount += task.GetFirstHitCount();
		
		pRoomParameters->avgAbsorptionLow += task.GetAbsorptionLow();
		pRoomParameters->avgAbsorptionMedium += task.GetAbsorptionMedium();
		pRoomParameters->avgAbsorptionHigh += task.GetAbsorptionHigh();
		absorptionCount += task.GetAbsorptionCount();
	}
	
	// calculate room parameters
	if(pSoundRayList->GetSegmentCount() > 0){
		pRoomParameters->meanFreePath /= (float)pSoundRayList->GetSegmentCount();
	}
	
	pRoomParameters->echoDelay = pRoomParameters->meanFreePath * INV_SOUND_SPEED;
	
	if(firstHitCount < pProbeConfig->GetRayCount()){
		// rays hit nothing count as hitting with absorption=1 at distance=range
		float factor = pRange * pRange * (float)(pProbeConfig->GetRayCount() - firstHitCount);
		pRoomParameters->roomVolume += factor * pRange;
		
		pRoomParameters->sabineMedium += factor; // absorption = 1 for all frequencies
		pRoomParameters->sabineLow += factor;
		pRoomParameters->sabineHigh += factor;
		
		pRoomParameters->avgAbsorptionLow += factor;
		pRoomParameters->avgAbsorptionMedium += factor;
		pRoomParameters->avgAbsorptionHigh += factor;
		absorptionCount += pProbeConfig->GetRayCount() - firstHitCount;
	}
	
	const float invAbsorptionCount = 1.0f / (float)absorptionCount;
	pRoomParameters->avgAbsorptionLow *= invAbsorptionCount;
	pRoomParameters->avgAbsorptionMedium *= invAbsorptionCount;
	pRoomParameters->avgAbsorptionHigh *= invAbsorptionCount;
	
	pRoomParameters->sabineMedium *= pProbeConfig->GetRayUnitSurface();
	pRoomParameters->sabineLow *= pProbeConfig->GetRayUnitSurface();
	pRoomParameters->sabineHigh *= pProbeConfig->GetRayUnitSurface();
	
	pRoomParameters->roomSurface = decMath::max(0.01f,
		pRoomParameters->roomSurface * pProbeConfig->GetRayUnitSurface());
	
	pRoomParameters->roomAbsorptionLow = pRoomParameters->sabineLow / pRoomParameters->roomSurface;
	pRoomParameters->roomAbsorptionMedium = pRoomParameters->sabineMedium / pRoomParameters->roomSurface;
	pRoomParameters->roomAbsorptionHigh = pRoomParameters->sabineHigh / pRoomParameters->roomSurface;
	
	pRoomParameters->roomVolume = decMath::max(0.01f,
		pRoomParameters->roomVolume * pProbeConfig->GetRayUnitVolume());
// 	pRoomParameters->roomG = 10.0f * log10f( decMath::max( FLOAT_SAFE_EPSILON,
// 		4.0f * ( 1.0f - pRoomParameters->roomAbsorptionMedium )
// 			/ decMath::max( pRoomParameters->sabineMedium, 0.01f ) ) );
		// alternate version? according to a paper:
		//   Itot = integral I(i)*dt  => total intensity in impuilse response.
		//                               maybe sum of gains of all rays works too?
		//   gainRev = sqrt(Itot / Itotrev)
		// where
		//   Itotrev = rt60 / (6 * log(10))
	
	// there are different ways the reverberation time can be estimated. a common way is using
	// the room volume and surface like this:
	// 
	//   RT60 = 0.161 * roomVolume / roomSabine
	//   => roomSabine = sum( wallAbsorption(i) * wallSurface(i) )
	// 
	// for use with first hits the surface area is the unit-sphere area of a single beam enlarged
	// to the hit distance with some dot-hack to account for hit angles. this version though
	// suffers from instable RT60 values (in one example ranging from 3s to 39s) because the
	// room volume and hit surface areas are instable with low ray counts (32). it requires a
	// considerable larger number of rays (1024) before this becomes somewhat stable.
	// 
	// another solution is using the mean free path like this:
	// 
	//   RT60 = 13.8 * meanFreePath / (-soundSpeed * ln(1 - roomAbsorption))
	//        ~= 13.8 * meanFreePath / (soundSpeed * roomAbsorption)
	// 
	// the short version is comparable to the full version but somewhat less accurate. the
	// roomAbsorption can be calculated in different ways too for example as:
	// 
	//   roomAbsorption = roomSabine / roomSurface
	// 
	// this version has the same problem as above since it is based on the instable roomSurface
	// although roomSurface is less instable than roomVolume. to avoid the instable values the
	// average of all hit surface absorptions can be used. this is as good/bad as the roomSurface
	// based version since we can not calculate the proper surface anyways.
	// 
	// two rooms have been examined to check out the results. the first room is a large
	// reverberant room of size 14x4.5x17m . the second room is a hallways of size 20x2.5x1.9m .
	// the following are the starting values for each room (for low and high frequencies):
	// 
	// room1: sabine=  5,49.1; volume=743.9; surface=512.7; meanFreePath=3.639; avgAbs=0.01,0.1
	// room2: sabine=3.4,22.9; volume=812.2; surface=268.9; meanFreePath=2.674; avgAbs=0.01,0.1
	// 
	// these values have been found for the rooms using each of the three methods:
	// - Sabine using 6-walls only with all same average absorption
	// - Sabine using ray-trace room volume, surface and sabine
	// - Mean-free-path using average absorption
	// 
	// room1:
	// - low=22.84; high=2.28
	// - low=24.1;  high=2.4
	// - low=14.94; high=1.45
	// 
	// room2:
	// - low= 8.24; high=0.82
	// - low=38.3;  high=5.7 (a little further) low=3.7; high=1.2
	// - low=10.61; high=1.01
	// 
	// the ray-trace room volume based version is volatile as the second room value shows.
	// estimated times from 3 to 39 is not usable at all. the mean-free-path based version
	// stays stable in all situations since it is based only on the mean free path (which
	// is average of all ray segments) and the absorption (which we defined as average of
	// all ray segments too). the reason why it is larger than the ground truth version is
	// because the room1 is connected to room2 and thus mean-free-path increases. this is
	// though a nice feature avoiding the need to track different rooms
	pRoomParameters->reverberationTimeLow = 0.0f;
	pRoomParameters->reverberationTimeMedium = 0.0f;
	pRoomParameters->reverberationTimeHigh = 0.0f;
	
	const float rtfactor = 13.8f * pRoomParameters->meanFreePath * -INV_SOUND_SPEED;
	if(pRoomParameters->avgAbsorptionLow > FLOAT_SAFE_EPSILON){
		pRoomParameters->reverberationTimeLow = rtfactor
			/ logf(1.0f - decMath::min(pRoomParameters->avgAbsorptionLow, 0.99f));
	}
	if(pRoomParameters->avgAbsorptionMedium > FLOAT_SAFE_EPSILON){
		pRoomParameters->reverberationTimeMedium = rtfactor
			/ logf(1.0f - decMath::min(pRoomParameters->avgAbsorptionMedium, 0.99f));
	}
	if(pRoomParameters->avgAbsorptionHigh > FLOAT_SAFE_EPSILON){
		pRoomParameters->reverberationTimeHigh = rtfactor
			/ logf(1.0f - decMath::min(pRoomParameters->avgAbsorptionHigh, 0.99f));
	}
	
	/*
	const float factorRevTime = pRoomParameters->roomVolume * REVERB_TIME_FACTOR;
	if(pRoomParameters->sabineLow > FLOAT_SAFE_EPSILON){
		pRoomParameters->reverberationTimeLow = factorRevTime / pRoomParameters->sabineLow;
	}
	if(pRoomParameters->sabineMedium > FLOAT_SAFE_EPSILON){
		pRoomParameters->reverberationTimeMedium = factorRevTime / pRoomParameters->sabineMedium;
	}
	if(pRoomParameters->sabineHigh > FLOAT_SAFE_EPSILON){
		pRoomParameters->reverberationTimeHigh = factorRevTime / pRoomParameters->sabineHigh;
	}
	*/
	
	// calculate separation time between first reflections and late reverberations. there
	// exist different ways to estimate this:
	// 
	// 1) t[s] = (meanFreePath / soundSpeed) * 4
	//          // 4 times the time it takes for a ray to travel mean-free-path.
	// 
	// 2) t[ms] = sqrt(roomVolume)
	// 
	// 3) t[ms] = echosPerSecond / (4 * pi * soundSpeed / roomVolume)
	//          = echosPerSecond * roomVolume / (4 * pi * soundSpeed)
	//          // schroeder: echosPerSecond = 1'000
	//          // griesinger: echosPerSecond = 10'000
	//          // rubak/johansen: echosPerSecond = 4'000
	// 
	// 4) t[ms] = 80
	//          // common fixed value found in literature. some use 30. an arbitrary value
	//          // disregarding the room size. the paper using this method explicitly stated
	//          // the value to be "after the direct path time" hence not from t0 where the
	//          // sound waves radiate from the sound source
	// 
	// 5) reflectionCount < 4
	//          // this version depends indirectly on the room size. instead of giving a
	//          // fixed time the number of bounces is used. in larger rooms rays travel
	//          // longer before reaching the bounce count. this creates larger transition
	//          // time values than smaller rooms. another possible value is 6 instead of 4.
	// 
	// two example values for 1, 2, 3: (42ms, 27ms, 172ms), (44ms, 29ms, 203ms)
	// 
	// use here is 1) since 3) is quite undefined and personally colored, 2) is based solely
	// on the room volume and 1) is based on averaging the mean free path which is based on
	// actual traced rays. thus 1) seems to be a good candiate with 2) following up
	pRoomParameters->separationTimeFirstLateReflection =
		pRoomParameters->meanFreePath * INV_SOUND_SPEED * 4.0f;
// 	pRoomParameters->separationTimeFirstLateReflection = 1000.0f
// 		* pRoomParameters->roomVolume / ( 4.0f * PI * SOUND_SPEED );
	
	// roomG is difference of sound volume in dB relative to the original sound volume.
	// converting this to linear scaling factor is enough
// 	pRoomParameters->reverberationGain = powf( 2.0f, pRoomParameters->roomG / 6.0f );
}
