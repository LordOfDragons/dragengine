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

#include "deoalEnvProbe.h"
#include "deoalEnvProbeList.h"
#include "deoalEnvProbeLevel.h"
#include "deoalEnvProbeListener.h"
#include "deoalEnvProbeListenerCached.h"
#include "raytrace/deoalRayTraceConfig.h"
#include "raytrace/deoalRayTraceResult.h"
#include "raytrace/deoalRayTraceHitElement.h"
#include "raytrace/deoalSoundRayInteraction.h"
#include "raytrace/deoalSoundRayInteractionList.h"
#include "raytrace/parallel/deoalRTParallelEnvProbe.h"
#include "../deoalBasics.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../audiothread/deoalATDebug.h"
#include "../devmode/deoalDevMode.h"
#include "../microphone/deoalAMicrophone.h"
#include "../soundLevelMeter/deoalASoundLevelMeter.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>



// Class deoalEnvProbe
////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalEnvProbe::deoalEnvProbe( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pRange( 0.0f ),
pAttenuationRefDist( 1.0f ),
pAttenuationRolloff( 0.0f ),
pAttenuationDistanceOffset( 0.0f ),
pRTConfig( NULL ),
pRayCount( 0 ),
pRayOpeningAngle( 0.0f ),
pReverberationTimeMedium( 0.0f ),
pReverberationTimeLow( 0.0f ),
pReverberationTimeHigh( 0.0f ),
pEchoDelay( 0.0f ),
pRoomVolume( 0.0f ),
pRoomSurface( 0.0f ),
pRoomSabineLow( 0.0f ),
pRoomSabineMedium( 0.0f ),
pRoomSabineHigh( 0.0f ),
pMeanFreePath( 0.0f ),
pSepTimeFirstLateRefl( 0.03f ),
pAvgAbsorptionLow( 0.0f ),
pAvgAbsorptionMedium( 0.0f ),
pAvgAbsorptionHigh( 0.0f ),
pListenerRange( 1.0f ),
// pListenerRange( 0.05f ),
pListenerTracking( 1 ),
pEstimated( true ),
pLastUsed( 0 ),
pOctreeNode( NULL ){
}

deoalEnvProbe::~deoalEnvProbe(){
	pRemoveAllListeners();
	pRemoveAllLevels();
}



// Manegement
///////////////

void deoalEnvProbe::SetPosition( const decDVector &position ){
	pPosition = position;
	Invalidate();
}

void deoalEnvProbe::SetRange( float range ){
	pRange = range;
	Invalidate();
}

void deoalEnvProbe::SetAttenuation( float refDist, float rolloff, float distanceOffset ){
	pAttenuationRefDist = refDist;
	pAttenuationRolloff = rolloff;
	pAttenuationDistanceOffset = distanceOffset;
	Invalidate();
}

void deoalEnvProbe::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
	Invalidate();
}

void deoalEnvProbe::SetRTConfig( const deoalATRayTracing::sConfigSoundTracing *config ){
	pRTConfig = config;
	Invalidate();
}

void deoalEnvProbe::SetExtends( const decDVector &minExtend, const decDVector &maxExtend ){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void deoalEnvProbe::SetRayCount( int rayCount ){
	pRayCount = rayCount;
}

void deoalEnvProbe::SetRayOpeningAngle( float angle ){
	pRayOpeningAngle = angle;
}

void deoalEnvProbe::SetReverberationTime( float timeLow, float timeMedium, float timeHigh ){
	pReverberationTimeLow = timeLow;
	pReverberationTimeMedium = timeMedium;
	pReverberationTimeHigh = timeHigh;
}

void deoalEnvProbe::SetEcho( float delay ){
	pEchoDelay = delay;
}

void deoalEnvProbe::SetRoomCenter( const decDVector &center ){
	pRoomCenter = center;
}

void deoalEnvProbe::SetRoom(float volume, float surface, float sabineLow,
float sabineMedium, float sabineHigh ){
	pRoomVolume = volume;
	pRoomSurface = surface;
	pRoomSabineLow = sabineLow;
	pRoomSabineMedium = sabineMedium;
	pRoomSabineHigh = sabineHigh;
}

void deoalEnvProbe::SetMeanFreePath( float meanFreePath ){
	pMeanFreePath = meanFreePath;
}

void deoalEnvProbe::SetSepTimeFirstLateRefl( float time ){
	pSepTimeFirstLateRefl = time;
}

void deoalEnvProbe::SetAvgAbsorption( float low, float medium, float high ){
	pAvgAbsorptionLow = low;
	pAvgAbsorptionMedium = medium;
	pAvgAbsorptionHigh = high;
}



float deoalEnvProbe::AttenuatedGain( float distance ) const{
	// OpenAL Inverse Distance Clamped Attenuation Model:
	// distance = max( distance, AL_REFERENCE_DISTANCE );
	// distance = min( distance, AL_MAX_DISTANCE );
	// gain = AL_REFERENCE_DISTANCE / (AL_REFERENCE_DISTANCE
	//        + AL_ROLLOFF_FACTOR * ( distance - AL_REFERENCE_DISTANCE ) );
	return pAttenuationRefDist / ( pAttenuationRefDist + pAttenuationRolloff
		* decMath::max( distance + pAttenuationDistanceOffset - pAttenuationRefDist, 0.0f ) );
}



void deoalEnvProbe::Invalidate(){
	pRemoveAllListeners();
	pRemoveAllLevels();
	pMaxExtend = pMinExtend = pPosition;
}



void deoalEnvProbe::EstimateRoomParameters( deoalAWorld &world,
const deoalRayTraceConfig &probeConfig ){
	/*
	Parallel-RoomEstimate:
		pos=(0.000,1.600,-4.000) range=60.000 rays=162
		minExt=(-3.000,0.000,-8.000) maxExt=(3.000,2.500,1.900)
		volume=120.436 surface 162.143 sabine=(3.309,6.421,9.662)
		mfp=2.971 sepTFLR=0.011 roomAbs=(0.020,0.040,0.060)
		revTime=(5.865,3.022,2.008) echoDelay=0.009 time=1.11ms
	*/
	deoalRTParallelEnvProbe::sRoomParameters roomParameters;
	
	pAudioThread.GetRTParallelEnvProbe().EstimateRoomParameters( roomParameters,
		pPosition, pRange, world, pLayerMask, probeConfig );
	
	pMinExtend = roomParameters.minExtend;
	pMaxExtend = roomParameters.maxExtend;
	pRoomCenter = ( pMinExtend + pMaxExtend ) * 0.5;
	pReverberationTimeLow = roomParameters.reverberationTimeLow;
	pReverberationTimeMedium = roomParameters.reverberationTimeMedium;
	pReverberationTimeHigh = roomParameters.reverberationTimeHigh;
	pRoomVolume = roomParameters.roomVolume;
	pRoomSurface = roomParameters.roomSurface;
	pRoomSabineLow = roomParameters.sabineLow;
	pRoomSabineMedium = roomParameters.sabineMedium;
	pRoomSabineHigh = roomParameters.sabineHigh;
	pMeanFreePath = roomParameters.meanFreePath;
	pSepTimeFirstLateRefl = roomParameters.separationTimeFirstLateReflection;
	pAvgAbsorptionLow = roomParameters.avgAbsorptionLow;
	pAvgAbsorptionMedium = roomParameters.avgAbsorptionMedium;
	pAvgAbsorptionHigh = roomParameters.avgAbsorptionHigh;
	pEchoDelay = roomParameters.echoDelay;
	
	pRayCount = probeConfig.GetRayCount();
	pRayOpeningAngle = probeConfig.GetOpeningAngle();
	
	pEstimated = true;
}

void deoalEnvProbe::TraceSoundRays( deoalAWorld &world, deoalRTWorldBVH *rtWorldBVH,
const deoalRayTraceConfig &probeConfig ){
	if( ! pRTConfig ){
		DETHROW( deeInvalidParam );
	}
	
	/*
	Parallel-TraceSoundRays:
		pos=(0.000,1.600,-4.000) range=60.000 rays=162
		minExt=(-13.000,-0.000,-8.000) maxExt=(46.227,2.500,2.900)
		center=(16.613,1.250,-2.550) size=(59.227,2.500,10.900) avgSize=24.209
		mfp=2.553 echoDelay=0.007 volume=121.861 surface=165.160 abs=0.039
		sabine=(3.355,6.517,9.814) revTime=(5.852,3.012,2.001) sepTimeFLR=0.0204
		rays=441 segments=5991 time=13.78ms
	*/
	deoalRTParallelEnvProbe::sRoomParameters roomParameters;
	
	pAudioThread.GetRTParallelEnvProbe().TraceSoundRays( roomParameters,
		pSoundRayList, pPosition, pRange, pAttenuationRefDist, pAttenuationRolloff,
		pAttenuationDistanceOffset, world, rtWorldBVH, pLayerMask, *pRTConfig );
	
	pMinExtend = roomParameters.minExtend;
	pMaxExtend = roomParameters.maxExtend;
	pRoomCenter = ( pMinExtend + pMaxExtend ) * 0.5;
	pReverberationTimeLow = roomParameters.reverberationTimeLow;
	pReverberationTimeMedium = roomParameters.reverberationTimeMedium;
	pReverberationTimeHigh = roomParameters.reverberationTimeHigh;
	pRoomVolume = roomParameters.roomVolume;
	pRoomSurface = roomParameters.roomSurface;
	pRoomSabineLow = roomParameters.sabineLow;
	pRoomSabineMedium = roomParameters.sabineMedium;
	pRoomSabineHigh = roomParameters.sabineHigh;
	pMeanFreePath = roomParameters.meanFreePath;
	pSepTimeFirstLateRefl = roomParameters.separationTimeFirstLateReflection;
	pAvgAbsorptionLow = roomParameters.avgAbsorptionLow;
	pAvgAbsorptionMedium = roomParameters.avgAbsorptionMedium;
	pAvgAbsorptionHigh = roomParameters.avgAbsorptionHigh;
	pEchoDelay = roomParameters.echoDelay;
	
	pRayCount = probeConfig.GetRayCount();
	pRayOpeningAngle = probeConfig.GetOpeningAngle();
	
	pEstimated = false;
}

void deoalEnvProbe::CalcListener( deoalEnvProbeListener &listener, deoalAWorld &world,
const decDVector &position, deoalAMicrophone *microphone ){
	pCalcListener( listener, world, position, microphone, NULL );
}

void deoalEnvProbe::CalcListener( deoalEnvProbeListener &listener, deoalAWorld &world,
const decDVector &position, deoalASoundLevelMeter *soundLevelMeter ){
	pCalcListener( listener, world, position, NULL, soundLevelMeter );
}

void deoalEnvProbe::SetLastUsed( unsigned int lastUsed ){
	pLastUsed = lastUsed;
}



void deoalEnvProbe::SetOctreeNode( deoalWorldOctree *node ){
	pOctreeNode = node;
}



// Private Functions
//////////////////////

void deoalEnvProbe::pRemoveAllLevels(){
	const int count = pLevels.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoalEnvProbeLevel* )pLevels.GetAt( i );
	}
	pLevels.RemoveAll();
}

void deoalEnvProbe::pRemoveAllListeners(){
	const int count = pListeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoalEnvProbeListenerCached* )pListeners.GetAt( i );
	}
	pListeners.RemoveAll();
}

void deoalEnvProbe::pCalcListener( deoalEnvProbeListener &listener, deoalAWorld &world,
const decDVector &position, deoalAMicrophone *microphone, deoalASoundLevelMeter *soundLevelMeter ){
	if( ! microphone && ! soundLevelMeter && pEstimated ){
		DETHROW( deeInvalidParam );
	}
	
	const bool debugLogCalcInfo = pAudioThread.GetDebug().GetLogCalcEnvProbe();
	const float invBlendRange = 0.5f / pListenerRange;
	const float blendRange = pListenerRange * 2.0f;
	const decVector localPosition( position );
	const int count = pListeners.GetCount();
	bool createProbe = true;
	float listenSum = 0.0f;
	int i;
	
	pListenerTracking++;
	
	listener.Clear();
	if( debugLogCalcInfo ){
// 		pAudioThread.GetLogger().LogInfoFormat( "CalcListener: pos=(%.3f,%.3f,%.3f) %d",
// 			position.x, position.y, position.z, count );
	}
	
	// TODO
	// 
	// - add "float pBlendNewListen" to deoalEnvProbeListener
	// - if new listener is created pBlendNewListen = 0
	// - each frame update increase pBlendNewListen by elapsedTime/blendTime clamped to max=1
	// - blendTime=0.5 for example
	// - for weighting pick listeners with double range
	// - weight listeners by range*pBlendNewListen
	// 
	// weight range has to be large enough so pBlendNewListen=1 happens before listener with
	// pBlendNewListen reaches 1. this ensures no jumping in the blending
	
	for( i=0; i<count; i++ ){
		deoalEnvProbeListenerCached &cached = *( ( deoalEnvProbeListenerCached * )pListeners.GetAt( i ) );
		
		const float distance = ( cached.GetPosition() - localPosition ).Length();
		if( distance > blendRange ){
			continue;
		}
		
		const float scaleFactor = 1.0f - distance * invBlendRange;
		listener.AddScaled( cached, scaleFactor );
		listenSum += scaleFactor;
		
		cached.SetLastUsed( pListenerTracking );
		
		if( distance <= pListenerRange ){
			createProbe = false;
		}
		
		if( debugLogCalcInfo ){
// 			pAudioThread.GetLogger().LogInfoFormat(
// 				"- AddScaled: cached %d (%.3f,%.3f,%.3f) d=%.3f lsum=%.3f "
// 				"frGain=(%.3f,%.3f,%.3f) lrGain=(%.3f,%.3f,%.3f)",
// 				i, cached.GetPosition().x, cached.GetPosition().y, cached.GetPosition().z,
// 				distance, scaleFactor, cached.GetReflectedLow(), cached.GetReflectedMedium(),
// 				cached.GetReflectedHigh(), cached.GetReverberationGainLow(),
// 				cached.GetReverberationGainMedium(), cached.GetReverberationGainHigh() );
		}
	}
	
	if( true ){
		float closestDistance = 0.0f;
		int closestIndex = -1;
		for( i=0; i<count; i++ ){
			deoalEnvProbeListenerCached &cached = *( ( deoalEnvProbeListenerCached* )
				pListeners.GetAt( i ) );
			const float distance = ( cached.GetPosition() - localPosition ).Length();
			if( closestIndex == -1 || distance < closestDistance ){
				closestIndex = i;
				closestDistance = distance;
			}
		}
		if( closestIndex != -1 ){
			deoalEnvProbeListenerCached &cached = *( ( deoalEnvProbeListenerCached* )
				pListeners.GetAt( closestIndex ) );
			listener.GetImpulseResponse() = cached.GetImpulseResponse();
		}
	}
	
	// create probe if required
	if( createProbe ){
		// GetEnvProbe can cause ray-tracing to be done
		
		// GetEnvProbe can cause Invalidate() to be called which in turn causes
		// pRemoveAllListeners to be called. this would remove the listener we use to listen.
		// to avoid this GetEnvProbe() has to be called before the pListeners array is accessed
		deoalEnvProbe * const listenProbe = microphone
			? microphone->GetEnvProbe() : soundLevelMeter->GetEnvProbe();
		
		// now it is safe to access pListeners
		deoalEnvProbeListenerCached *replaceCached = nullptr;
		
		if( count == 100 ){
			replaceCached = ( deoalEnvProbeListenerCached* )pListeners.GetAt( 0 );
			int replaceCachedAge = pListenerTracking - replaceCached->GetLastUsed();
			
			for( i=1; i<count; i++ ){
				deoalEnvProbeListenerCached * const cached =
					( deoalEnvProbeListenerCached * )pListeners.GetAt( i );
				const int cachedAge = pListenerTracking - cached->GetLastUsed();
				if( cachedAge > replaceCachedAge ){
					replaceCached = cached;
					replaceCachedAge = cachedAge;
				}
			}
			
			if( debugLogCalcInfo ){
				pAudioThread.GetLogger().LogInfoFormat( "> Replace cached (%.3f,%.3f,%.3f)",
					replaceCached->GetPosition().x, replaceCached->GetPosition().y,
					replaceCached->GetPosition().z );
			}
		}
		
		if( ! replaceCached ){
			replaceCached = new deoalEnvProbeListenerCached;
			pListeners.Add( replaceCached );
			if( debugLogCalcInfo ){
				pAudioThread.GetLogger().LogInfoFormat( "> New cached (%.3f,%.3f,%.3f)",
					replaceCached->GetPosition().x, replaceCached->GetPosition().y,
					replaceCached->GetPosition().z );
			}
		}
		
		replaceCached->SetPosition( localPosition );
		replaceCached->SetLastUsed( pListenerTracking );
		
		if( listenProbe ){
			if( microphone ){
				pAudioThread.GetRTParallelEnvProbe().Listen( *this, listenProbe, *replaceCached,
					world, &microphone->GetRTWorldBVH(), microphone->GetLayerMask(), position );
				
			}else{
				pAudioThread.GetRTParallelEnvProbe().Listen( *this, listenProbe, *replaceCached,
					world, nullptr, soundLevelMeter->GetLayerMask(), position );
			}
			
			// copy the extends from the listener. this is required since these extends are
			// only calculated during TraceSoundRays(). it is not enough to find all ray
			// segments leading from source to listener because geometry changes outside
			// this range can potentially redirect rays abck into the range and thus modify
			// the result. this is less optmial if the extends are large but never wrong
			pMinExtend = listenProbe->GetMinExtend();
			pMaxExtend = listenProbe->GetMaxExtend();
		}
		
		// add new propbe to result
		listener += *replaceCached;
		if( true ){
			listener.GetImpulseResponse() = replaceCached->GetImpulseResponse();
		}
		listenSum += 1.0f;
	}
	
	// return result
	if( listenSum > FLOAT_SAFE_EPSILON ){
		listener.Normalize( 1.0f / listenSum );
		if( debugLogCalcInfo ){
// 			pAudioThread.GetLogger().LogInfoFormat( "> Result: pos=(%.3f,%.3f,%.3f) %d lsum=%.3f",
// 				position.x, position.y, position.z, count, listenSum );
		}
	}
}
