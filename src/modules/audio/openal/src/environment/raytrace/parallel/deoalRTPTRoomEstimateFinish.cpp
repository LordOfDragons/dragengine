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
#include "deoalRTPTRoomEstimate.h"
#include "deoalRTPTRoomEstimateFinish.h"
#include "../deoalRayTraceHitElement.h"
#include "../deoalRayTraceConfig.h"
#include "../../../deoalBasics.h"
#include "../../../deAudioOpenAL.h"
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"
#include "../../../component/deoalAComponent.h"
#include "../../../component/deoalAComponentTexture.h"
#include "../../../model/deoalAModel.h"
#include "../../../model/deoalModelFace.h"
#include "../../../world/deoalAWorld.h"

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

deoalRTPTRoomEstimateFinish::deoalRTPTRoomEstimateFinish( deoalRTParallelEnvProbe &owner ) :
deParallelTask( &owner.GetAudioThread().GetOal() ),
pOwner( owner ),
pRange( 0.0f ),
pProbeConfig( NULL ),
pRoomParameters( NULL )
{
	SetMarkFinishedAfterRun( true );
	SetLowPriority( true );
}

deoalRTPTRoomEstimateFinish::~deoalRTPTRoomEstimateFinish(){
}



// Manegement
///////////////

void deoalRTPTRoomEstimateFinish::AddDependencies( const decPointerList &tasks ){
	const int count = tasks.GetCount();
	int i;
	
	pTasks.RemoveAll();
	
	for( i=0; i<count; i++ ){
		deParallelTask * const task = ( deParallelTask* )tasks.GetAt( i );
		AddDependsOn( task );
		pTasks.Add( task );
	}
}

void deoalRTPTRoomEstimateFinish::SetRange( float range ){
	pRange = range;
}

void deoalRTPTRoomEstimateFinish::SetProbeConfig( const deoalRayTraceConfig *probeConfig ){
	pProbeConfig = probeConfig;
}

void deoalRTPTRoomEstimateFinish::SetRoomParameters(
deoalRTParallelEnvProbe::sRoomParameters *roomParameters ){
	pRoomParameters = roomParameters;
}



void deoalRTPTRoomEstimateFinish::Run(){
	try{
		pRun();
		
	}catch( ... ){
		Cancel();
		pOwner.FinishTaskFinished( this );
		throw;
	}
	
	pOwner.FinishTaskFinished( this );
}

void deoalRTPTRoomEstimateFinish::Finished(){
	if( IsCancelled() ){
		pOwner.FinishTaskFinished( this );
	}
	pOwner.Enable( this );
}



// Debugging
//////////////

decString deoalRTPTRoomEstimateFinish::GetDebugName() const{
	return "OpenAL-RTPTRoomEstimateFinish";
}

decString deoalRTPTRoomEstimateFinish::GetDebugDetails() const{
	return "";
}



// Private Functions
//////////////////////

void deoalRTPTRoomEstimateFinish::pRun(){
	// collect results from tasks
	const int taskCount = pTasks.GetCount();
	int hitCount = 0;
	int i;
	
	pRoomParameters->sabineLow = 0.0f;
	pRoomParameters->sabineMedium = 0.0f;
	pRoomParameters->sabineHigh = 0.0f;
	pRoomParameters->roomVolume = 0.0f;
	pRoomParameters->roomSurface = 0.0f;
	pRoomParameters->avgAbsorptionLow = 0.0f;
	pRoomParameters->avgAbsorptionMedium = 0.0f;
	pRoomParameters->avgAbsorptionHigh = 0.0f;
	
	for( i=0; i<taskCount; i++ ){
		const deoalRTPTRoomEstimate &task = *( ( deoalRTPTRoomEstimate* )pTasks.GetAt( i ) );
		if( task.IsCancelled() ){
			DETHROW( deeInvalidAction ); // task failed
		}
		
		if( i == 0 ){
			pRoomParameters->minExtend = task.GetMinExtend();
			pRoomParameters->maxExtend = task.GetMaxExtend();
			
		}else{
			pRoomParameters->minExtend.SetSmallest( task.GetMinExtend() );
			pRoomParameters->maxExtend.SetLargest( task.GetMaxExtend() );
		}
		
		pRoomParameters->roomVolume += task.GetRoomVolume();
		pRoomParameters->roomSurface += task.GetRoomSurface();
		pRoomParameters->sabineLow += task.GetSabineLow();
		pRoomParameters->sabineMedium += task.GetSabineMedium();
		pRoomParameters->sabineHigh += task.GetSabineHigh();
		pRoomParameters->avgAbsorptionLow += task.GetAbsorptionLow();
		pRoomParameters->avgAbsorptionMedium += task.GetAbsorptionMedium();
		pRoomParameters->avgAbsorptionHigh += task.GetAbsorptionHigh();
		hitCount += task.GetHitCount();
	}
	
	if( hitCount < pProbeConfig->GetRayCount() ){
		// rays hit nothing count as hitting with absorption=1 at distance=range
		float factor = pRange * pRange * ( float )( pProbeConfig->GetRayCount() - hitCount );
		pRoomParameters->roomVolume += factor * pRange;
		
		pRoomParameters->sabineMedium += factor; // absorption = 1 for all frequencies
		pRoomParameters->sabineLow += factor;
		pRoomParameters->sabineHigh += factor;
		
		pRoomParameters->avgAbsorptionLow += factor;
		pRoomParameters->avgAbsorptionMedium += factor;
		pRoomParameters->avgAbsorptionHigh += factor;
	}
	
	// normalize collected results
	pRoomParameters->sabineMedium *= pProbeConfig->GetRayUnitSurface();
	pRoomParameters->sabineLow *= pProbeConfig->GetRayUnitSurface();
	pRoomParameters->sabineHigh *= pProbeConfig->GetRayUnitSurface();
	
	pRoomParameters->roomSurface *= pProbeConfig->GetRayUnitSurface();
	pRoomParameters->roomVolume *= pProbeConfig->GetRayUnitVolume();
	
	const float invAbsorptionCount = 1.0f / ( float )pProbeConfig->GetRayCount();
	pRoomParameters->avgAbsorptionLow *= invAbsorptionCount;
	pRoomParameters->avgAbsorptionMedium *= invAbsorptionCount;
	pRoomParameters->avgAbsorptionHigh *= invAbsorptionCount;
	
	// calculate room parameters
	pRoomParameters->reverberationTimeMedium = 0.0f;
	pRoomParameters->reverberationTimeLow = 0.0f;
	pRoomParameters->reverberationTimeHigh = 0.0f;
	
	const float factorRevTime = pRoomParameters->roomVolume * REVERB_TIME_FACTOR;
	if( pRoomParameters->sabineLow > FLOAT_SAFE_EPSILON ){
		pRoomParameters->reverberationTimeLow = factorRevTime / pRoomParameters->sabineLow;
	}
	if( pRoomParameters->sabineMedium > FLOAT_SAFE_EPSILON ){
		pRoomParameters->reverberationTimeMedium = factorRevTime / pRoomParameters->sabineMedium;
	}
	if( pRoomParameters->sabineHigh > FLOAT_SAFE_EPSILON ){
		pRoomParameters->reverberationTimeHigh = factorRevTime / pRoomParameters->sabineHigh;
	}
	
	pRoomParameters->meanFreePath = 4.0f * pRoomParameters->roomVolume / pRoomParameters->roomSurface;
	pRoomParameters->echoDelay = pRoomParameters->meanFreePath * INV_SOUND_SPEED;
	
	pRoomParameters->separationTimeFirstLateReflection = pRoomParameters->echoDelay * 4.0f;
	
	pRoomParameters->roomAbsorptionLow = 0.0f;
	pRoomParameters->roomAbsorptionMedium = 0.0f;
	pRoomParameters->roomAbsorptionHigh = 0.0f;
	if( pRoomParameters->roomSurface > FLOAT_EPSILON ){
		const float invRoomSurface = 1.0f / pRoomParameters->roomSurface;
		pRoomParameters->roomAbsorptionLow = pRoomParameters->sabineLow * invRoomSurface;
		pRoomParameters->roomAbsorptionMedium = pRoomParameters->sabineMedium * invRoomSurface;
		pRoomParameters->roomAbsorptionHigh = pRoomParameters->sabineHigh * invRoomSurface;
	}
	
// 	pRoomParameters->roomG = 10.0f * log10f( decMath::max( FLOAT_SAFE_EPSILON,
// 		4.0f * ( 1.0f - pRoomParameters->roomAbsorptionMedium )
// 			/ decMath::max( pRoomParameters->sabineMedium, 0.01f ) ) );
	
	// roomG is difference of sound volume in dB relative to the original sound volume.
	// converting this to linear scaling factor is enough
// 	pRoomParameters->reverberationGain = powf( 2.0f, pRoomParameters->roomG / 6.0f );
}
