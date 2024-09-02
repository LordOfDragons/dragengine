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
#include "deoalRTPTListen.h"
#include "deoalRTPTListenFinish.h"
#include "../deoalRayTraceConfig.h"
#include "../deoalSoundRay.h"
#include "../../deoalEnvProbe.h"
#include "../../deoalEnvProbeListener.h"
#include "../../../deoalBasics.h"
#include "../../../deAudioOpenAL.h"
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoalRTPTListenFinish
////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTPTListenFinish::deoalRTPTListenFinish( deoalRTParallelEnvProbe &owner ) :
deParallelTask( &owner.GetAudioThread().GetOal() ),
pOwner( owner ),
pSourceProbe( NULL ),
pListenProbe( NULL ),
pListener( NULL )
{
	SetMarkFinishedAfterRun( true );
	SetLowPriority( true );
}

deoalRTPTListenFinish::~deoalRTPTListenFinish(){
}



// Manegement
///////////////

void deoalRTPTListenFinish::AddDependencies( const decPointerList &tasks ){
	const int count = tasks.GetCount();
	int i;
	
	pTasks.RemoveAll();
	
	for( i=0; i<count; i++ ){
		deParallelTask * const task = ( deParallelTask* )tasks.GetAt( i );
		AddDependsOn( task );
		pTasks.Add( task );
	}
}

void deoalRTPTListenFinish::SetProbes( const deoalEnvProbe *source, const deoalEnvProbe *listen ){
	pSourceProbe = source;
	pListenProbe = listen;
}

void deoalRTPTListenFinish::SetPosition( const decDVector &position ){
	pPosition = position;
}

void deoalRTPTListenFinish::SetListener( deoalEnvProbeListener *listener ){
	pListener = listener;
}



void deoalRTPTListenFinish::Run(){
	try{
		pRun();
		
	}catch( ... ){
		Cancel();
		pOwner.FinishTaskFinished( this );
		throw;
	}
	
	pOwner.FinishTaskFinished( this );
}

void deoalRTPTListenFinish::Finished(){
	if( IsCancelled() ){
		pOwner.FinishTaskFinished( this );
	}
	pOwner.Enable( this );
}



// Debugging
//////////////

decString deoalRTPTListenFinish::GetDebugName() const{
	return "OpenAL-RTPTListenFinish";
}

decString deoalRTPTListenFinish::GetDebugDetails() const{
	return "";
}



// NOTE https://books.google.ch/books?id=HtDxSt0jcRkC&pg=PA90&lpg=PA90
// 
// a version of using spherical receiver. in this paper the cone is calculated not from
// the virtual source point onwards but from the hit point on the wall. the opening
// angle is thus from the wall to the sphere receiver. the following formula is established:
// 
// Es = Ep * (1 - a) * s * (1 - cos(y / 2)) * 2 * cos(o) * exp(-m * r)
// - Es = detected scattered energy
// - Ep = energy before hitting wall
// - a = wall absorption coefficient
// - s = wall scattering coefficient (roughness so to speak)
// - y = opening angle from wall hit point to sphere listener center
// - r = distance from wall hit point to sphere listener center
// - exp(-m*r) = air absorption over distance r
// 
// air absorption is not important for us here. simplified this becomes:
// 
// Es = Er * (1 - cos(y / 2)) * 2 * cos(o)
// - Es = detected scattered energy
// - Er = energy reflected from wall
// - y = opening angle from wall hit point to sphere listener center
// - r = distance from wall hit point to sphere listener center

// NOTE receiver size and ray energy
// 
// the paper goes the inverse way by calculating the energy per ray depending on the
// listener sphere radius like this:
// 
// Ep = 2 * Esource / (N * r*r * (1 - cos(y / 2)))
// - Ep = energy of a single ray
// - Esource = energy of sound source (in our case 1)
// - N = number of shot rays
// - r = distance between sound source and listener sphere
// - y = opening angle from sound source covering listener sphere
// 
// in our approach Ep=Esource/N and the radius of listener sphere is set to the opening
// angle of the probe configuration (times 2 so it can be linearly blended)

// NOTE energy histogram
// 
// buckets with width dt. papers are notoriously vague on the value of dt. basically all
// tell to align with the human hearing resolution. one paper actually mentions something
// more solid by telling to use a value of "a few milliseconds" with larger dt farther
// away in the histogram (towards the late reverberation).

// NOTE thoughts about deriving revebreator gains from ray testing
// 
// papers mention that the first reflections are usually done using the image source method.
// this basically means the source is reflected across a hit wall to obtain a virtual
// sound source:
// 
// Sv = Ph + normal * (normal dot (Ph - S))
// - Sv = virtual sound source
// - S = sound source (can be previous reflection Sv)
// - Ph = hit point on wall
// - normal = hit wall normal
// 
// in our system the hit points, hit distance and hit direction are stored for each hit.
// this way it is not required to reflect source position for all the hit points.
// the required virtual sound sources can be calculated like this:
// 
// Sv = Ph - direction * distance
// - Sv = virtual sound source
// - Ph = hit point on wall
// - direction = normalized outgoing ray direction after the hit
// - distance = distance from sound source to hit point
// 
// now for calculating the gain for reverberators the first valid image source reflection
// can be used. we do not need to calculate the virtual source but to validate the idea
// the ray from the virtual source to the listener sphere hits the wall at the point the
// ideal ray would have reflected straight into the listener sphere center. we can assume
// the original ray (which usually passes some distance by the listener sphere center) to
// be this ideal ray. in this case the energy dropped off at the listener would be exactly
// the wall reflected gain minus (in the above paper notation Ep). we could thus use the
// first ray segment seeing the sound source to contribute the full energy. only problem
// is that the different rays hit at different time slots and should stack in an energy
// histogram. in the end it boils down again to having the histogram and picking the
// largest energy reading as the reverberator gain.



// Private Functions
//////////////////////

// #define USE_ENERGY_HISTOGRAM 1

void deoalRTPTListenFinish::pRun(){
	// collect and apply parameters from tasks
// 	float gainLow = 0.0f;
// 	float gainMedium = 0.0f;
// 	float gainHigh = 0.0f;
// 	float maxGainLow = 0.0f;
// 	float maxGainMedium = 0.0f;
// 	float maxGainHigh = 0.0f;
// 	int arCount = 0;
	
	float frSumDistance = 0.0f;
	float frMinDistance = pSourceProbe->GetRange();
	float frMaxDistance = 0.0f;
	float frGainLow = 0.0f;
	float frGainMedium = 0.0f;
	float frGainHigh = 0.0f;
	decVector frPanDirection;
	float frPanDirectionWeightSum = 0.0f;
	int frCount = 0;
	
// 	float lrMaxDistanceLow = 0.0f;
// 	float lrMaxDistanceMedium = 0.0f;
// 	float lrMaxDistanceHigh = 0.0f;
	float lrGainLow = 0.0f;
	float lrGainMedium = 0.0f;
	float lrGainHigh = 0.0f;
	decVector lrPanDirection;
	float lrPanDirectionWeightSum = 0.0f;
	int lrCount = 0;
	
	float absorptionLow = 0.0f;
	float absorptionMedium = 0.0f;
	float absorptionHigh = 0.0f;
	int absorptionCount = 0;
	
	float meanFreePath = 0.0f;
	int meanFreePathCount = 0;
	
	#ifdef USE_DECAY_CURVE
	float decayTimeLow = 0.0f;
	float decayTimeMedium = 0.0f;
	float decayTimeHigh = 0.0f;
	int decayTimeCount = 0;
	#endif
	
	float limitRevTimeLow = 0.0f;
	float limitRevTimeMedium = 0.0f;
	float limitRevTimeHigh = 0.0f;
	int limitRevTimeCount = 0;
	int unlimitRevTimeCount = 0;
	
// 	bool hasHighestGain = false;
// 	float highestGainGain = 0.0f;
// 	float highestGainDistance = 0.0f;
	
	const int taskCount = pTasks.GetCount();
	int i;
	
	deoalImpulseResponse &impulseResponse = pListener->GetImpulseResponse();
	impulseResponse.Clear();
	
	#ifdef USE_ENERGY_HISTOGRAM
	deoalEnergyHistogram &energyHistogram = pListener->GetEnergyHistogram();
	const float histogramRange = pListenProbe ? pListenProbe->GetRange() : pSourceProbe->GetRange();
	const float histogramTime = histogramRange * INV_SOUND_SPEED;
	const float histogramSlotTime = 0.001f; // 1ms
	const int histogramSlots = ( int )( histogramTime / histogramSlotTime );
	
	energyHistogram.SetParameters( histogramSlots, 3, histogramSlotTime * histogramSlots );
	energyHistogram.Clear();
	#endif
	
	// collect data from tasks
	for( i=0; i<taskCount; i++ ){
		const deoalRTPTListen &task = *( ( deoalRTPTListen* )pTasks.GetAt( i ) );
		
		if( task.IsCancelled() ){
			DETHROW( deeInvalidAction ); // task failed
		}
		
// 		gainLow += task.GetGainLow();
// 		gainMedium += task.GetGainMedium();
// 		gainHigh += task.GetGainHigh();
// 		if( task.GetMaxGainLow() > maxGainLow ){
// 			maxGainLow = task.GetMaxGainLow();
// 		}
// 		if( task.GetMaxGainMedium() > maxGainMedium ){
// 			maxGainMedium = task.GetMaxGainMedium();
// 		}
// 		if( task.GetMaxGainHigh() > maxGainHigh ){
// 			maxGainHigh = task.GetMaxGainHigh();
// 		}
// 		arCount += task.GetARCount();
		
		if( task.GetFRMinDistance() < frMinDistance ){
			frMinDistance = task.GetFRMinDistance();
		}
		if( task.GetFRMaxDistance() > frMaxDistance ){
			frMaxDistance = task.GetFRMaxDistance();
		}
		frSumDistance += task.GetFRSumDistance();
		frGainLow += task.GetFRGainLow();
		frGainMedium += task.GetFRGainMedium();
		frGainHigh += task.GetFRGainHigh();
		frPanDirection += task.GetFRPanDirection();
		frPanDirectionWeightSum += task.GetFRPanDirectionWeightSum();
		frCount += task.GetFRCount();
		
// 		if( task.GetLRMaxDistanceLow() > lrMaxDistanceLow ){
// 			lrMaxDistanceLow = task.GetLRMaxDistanceLow();
// 		}
// 		if( task.GetLRMaxDistanceMedium() > lrMaxDistanceMedium ){
// 			lrMaxDistanceMedium = task.GetLRMaxDistanceMedium();
// 		}
// 		if( task.GetLRMaxDistanceHigh() > lrMaxDistanceHigh ){
// 			lrMaxDistanceHigh = task.GetLRMaxDistanceHigh();
// 		}
		lrGainLow += task.GetLRGainLow();
		lrGainMedium += task.GetLRGainMedium();
		lrGainHigh += task.GetLRGainHigh();
		lrPanDirection += task.GetLRPanDirection();
		lrPanDirectionWeightSum += task.GetLRPanDirectionWeightSum();
		lrCount += task.GetLRCount();
		
		absorptionLow += task.GetAbsorptionSumLow();
		absorptionMedium += task.GetAbsorptionSumMedium();
		absorptionHigh += task.GetAbsorptionSumHigh();
		absorptionCount += task.GetAbsorptionCount();
		
		meanFreePath += task.GetMeanFreePathSum();
		meanFreePathCount += task.GetMeanFreePathCount();
		
		#ifdef USE_DECAY_CURVE
		decayTimeLow += task.GetDecayTimeSumLow();
		decayTimeMedium += task.GetDecayTimeSumMedium();
		decayTimeHigh += task.GetDecayTimeSumHigh();
		decayTimeCount += task.GetDecayTimeCount();
		#endif
		
		limitRevTimeLow += task.GetLimitRevTimeSumLow();
		limitRevTimeMedium += task.GetLimitRevTimeSumMedium();
		limitRevTimeHigh += task.GetLimitRevTimeSumHigh();
		limitRevTimeCount += task.GetLimitRevTimeCount();
		unlimitRevTimeCount += task.GetUnlimitRevTimeCount();
		
// 		if( task.GetHasHighestGain() && ( ! hasHighestGain
// 		|| task.GetHighestGainGain() > highestGainGain ) ){
// 			hasHighestGain = true;
// 			highestGainGain = task.GetHighestGainGain();
// 			highestGainDistance = task.GetHighestGainDistance();
// 		}
		
		/*
		if( pAudioThread.GetDebug().GetLogCalcEnvProbe() ){
			const deoalRTPTEnvProbeListener::sContribution * const contributions = task.GetContributions();
			const int contributionCount = task.GetContributionCount();
			for( int j=0; j<contributionCount; j++ ){
				const deoalRTPTEnvProbeListener::sContribution &contribution = contributions[ j ];
				pAudioThread.GetLogger().LogInfoFormat(
					"- Contribution #%d:%d: dist=%.3f gain=(%.3f,%.3f,%.3f)[%.3f]",
					i, j, contribution.distance, contribution.gainLow, contribution.gainMedium,
					contribution.gainHigh, decMath::max( contribution.gainLow,
						contribution.gainMedium, contribution.gainHigh ) );
			}
		}
		*/
		
		impulseResponse.Insert( task.GetImpulseResponse() );
		
		#ifdef USE_ENERGY_HISTOGRAM
		energyHistogram.Add( task.GetImpulseResponse() );
		#endif
	}
	/*
	if( pListenProbe ){
		const float l = ( pListenProbe->GetPosition() - pSourceProbe->GetPosition() ).Length();
		pOwner.GetAudioThread().GetLogger().LogInfoFormat(
			"Direct dist=%f time=%f, Separation dist=%f time=%f ", l, l * INV_SOUND_SPEED,
			pListenProbe->GetSepTimeFirstLateRefl() * INV_SOUND_SPEED,
			pListenProbe->GetSepTimeFirstLateRefl() );
	}
	*/
	
	// calculate reverberation time using decay curve approach
	// normalize
	const float invRayCount = 1.0f / ( float )( pListenProbe
		? pListenProbe->GetRayCount() : pSourceProbe->GetRayCount() );
	
	impulseResponse *= invRayCount;
	#ifdef USE_ENERGY_HISTOGRAM
	energyHistogram *= invRayCount;
	#endif
	
	if( absorptionCount > 0 ){
		const float factor = 1.0f / ( float )absorptionCount;
		absorptionLow *= factor;
		absorptionMedium *= factor;
		absorptionHigh *= factor;
	}
	
	if( meanFreePathCount > 0 ){
		meanFreePath /= ( float )meanFreePathCount;
	}
	
	#ifdef USE_DECAY_CURVE
	if( decayTimeCount > 0 ){
		const float factor = 1.0f / ( float )decayTimeCount;
		decayTimeLow *= factor;
		decayTimeMedium *= factor;
		decayTimeHigh *= factor;
	}
	#endif
	
	// calculate reverberation time using decay curve approach
	#ifdef USE_DECAY_CURVE
	pDecayCurve = impulseResponse;
	pDecayCurve.BackwardIntegrate();
	const deoalImpulseResponse::sImpulse revTime( pDecayCurve.ReverberationTime() );
	#endif
	
	#if 0
		const float rtfactor = 13.8f * -(meanFreePath * INV_SOUND_SPEED);
		float rtlow = 0.0f, rtmed = 0.0f, rthigh = 0.0f;
		if( absorptionLow > FLOAT_SAFE_EPSILON ){
			rtlow = rtfactor / log( decMath::max( 1.0f - absorptionLow, 1e-5f ) );
		}
		if( absorptionMedium > FLOAT_SAFE_EPSILON ){
			rtmed = rtfactor / log( decMath::max( 1.0f - absorptionMedium, 1e-5f ) );
		}
		if( absorptionHigh > FLOAT_SAFE_EPSILON ){
			rthigh = rtfactor / log( decMath::max( 1.0f - absorptionHigh, 1e-5f ) );
		}
		#ifdef USE_DECAY_CURVE
		pOwner.GetAudioThread().GetLogger().LogInfoFormat(
			"ListenFinish: mfp=%.3f abs=(%.3f,%.3f,%.3f) dt1=(%.3f,%.3f,%.3f) "
			"dt2=(%.3f,%.3f,%.3f) dt3=(%.3f,%.3f,%.3f)[%.3f] pos=(%.3f,%.3f,%.3f)",
			meanFreePath, absorptionLow, absorptionMedium, absorptionHigh, rtlow, rtmed, rthigh,
			decayTimeLow, decayTimeMedium, decayTimeHigh, revTime.low, revTime.medium,
			revTime.high, revTime.time, pSourceProbe->GetPosition().x,
			pSourceProbe->GetPosition().y, pSourceProbe->GetPosition().z );
		#else
		
		float realRevTime = limitRevTimeLow + rtlow * unlimitRevTimeCount;
		if( limitRevTimeCount + unlimitRevTimeCount > 0 ){
			realRevTime /= ( float )( limitRevTimeCount + unlimitRevTimeCount );
		}
		
		pOwner.GetAudioThread().GetLogger().LogInfoFormat(
			"ListenFinish: mfp=%.3f abs=(%.3f,%.3f,%.3f) dt=(%.3f,%.3f,%.3f) lim=(%.3f,%d/%d|%.3f) pos=(%.3f,%.3f,%.3f)",
			meanFreePath, absorptionLow, absorptionMedium, absorptionHigh, rtlow, rtmed, rthigh,
			limitRevTimeLow, limitRevTimeCount, unlimitRevTimeCount, realRevTime,
			pSourceProbe->GetPosition().x, pSourceProbe->GetPosition().y, pSourceProbe->GetPosition().z );
		#endif
	#endif
	
	#if 0
		const float factor = ( float )( pListenProbe ? pListenProbe->GetRayCount() : pSourceProbe->GetRayCount() );
		const int count = impulseResponse.GetCount();
		decString text( "IR-Time:" );
		int i;
		for( i=0; i<count; i++ ){
			const deoalImpulseResponse::sImpulse &impulse = impulseResponse.GetAt( i );
			text.AppendFormat( ",%f", impulse.time );
		}
		pOwner.GetAudioThread().GetLogger().LogInfo( text );
		text = "IR-Gain:";
		for( i=0; i<count; i++ ){
			const deoalImpulseResponse::sImpulse &impulse = impulseResponse.GetAt( i );
			//text.AppendFormat( ",%f", 20.0f * log10f( impulse.low * factor ) );
			text.AppendFormat( ",%f", impulse.low * factor );
		}
		pOwner.GetAudioThread().GetLogger().LogInfo( text );
	#endif
	
	#ifdef USE_ENERGY_HISTOGRAM
		#if 0
			const int slotCount = energyHistogram.GetSlotCount();
			const float * const energies = energyHistogram.GetEntries();
			decString text;
			text.Format( "EH-Energy (dt=%f):", energyHistogram.GetSlotTime() );
			int i;
			for( i=0; i<slotCount; i++ ){
				text.AppendFormat( ",%g", energies[i] );
			}
			pOwner.GetAudioThread().GetLogger().LogInfo( text );
		#endif
	#endif
	
	// roomAbsorption = roomSabine / roomSurface  // because roomSabine is sum(surface * sabine)
	// roomConst = roomSabine / (1 - roomAbsorption)
	// roomConst = roomSabine / (1 - roomSabine/roomSurface)
	// 
	// for the calculation we need 4/roomConst so:
	// roomFactor = 4 / roomConst
	// roomFactor = 4 / ( roomSabine / (1 - roomSabine/roomSurface))
	// roomFactor = 4 * (1 - roomSabine/roomSurface) / roomSabine
	// roomFactor = 4 / roomSabine - 4 / roomSurface
// 	const float fourInvRoomSurface = 4.0f / pSourceProbe->GetRoomSurface();
// 	const float roomFactorLow = 4.0f / pSourceProbe->GetRoomSabineLow() - fourInvRoomSurface;
// 	const float roomFactorMedium = 4.0f / pSourceProbe->GetRoomSabineMedium() - fourInvRoomSurface;
// 	const float roomFactorHigh = 4.0f / pSourceProbe->GetRoomSabineHigh() - fourInvRoomSurface;
	
	// roomPressure = 10 * log10(1 / (4 * pi * distanceSquare) + roomFactor)
	// this is direct and reverberant combined
	// NOTE this is not including source distance offset since it is used for delay calculation
	//      which has to be relative to the playing source which is playing at the real position
	const float directDistance = ( float )( ( pListenProbe ? pListenProbe->GetPosition() : pPosition )
		- pSourceProbe->GetPosition() ).Length();
// 	const float invFourPiDistSquared = 1.0f / ( 4.0 * PI * directDistance * directDistance );
// 	const float roomPressureLow = 10.0f * log10f( invFourPiDistSquared + roomFactorLow );
// 	const float roomPressureMedium = 10.0f * log10f( invFourPiDistSquared + roomFactorMedium );
// 	const float roomPressureHigh = 10.0f * log10f( invFourPiDistSquared + roomFactorHigh );
	
	// room pressure is dB. we need it thgouh in intensities
// 	const float pressureToInt = 1.0f / 6.0f;
// 	const float roomIntensityLow = powf( 2.0f, roomPressureLow * pressureToInt );
// 	const float roomIntensityMedium = powf( 2.0f, roomPressureMedium * pressureToInt );
// 	const float roomIntensityHigh = powf( 2.0f, roomPressureHigh * pressureToInt );
	
	// subtract the direct contribution to get the reverberant contribution
// 	const float directIntensity = 1.0f / decMath::max( 1.0f, directDistance );
// 	const float roomRevIntLow = decMath::max( roomIntensityLow - directIntensity, 0.0f );
// 	const float roomRevIntMedium = decMath::max( roomIntensityMedium - directIntensity, 0.0f );
// 	const float roomRevIntHigh = decMath::max( roomIntensityHigh - directIntensity, 0.0f );
	
	// calculate first-reflection gain
	
// 	gainLow *= invRayCount;
// 	gainMedium *= invRayCount;
// 	gainHigh *= invRayCount;
	
	float finalFRGainLow = frGainLow * invRayCount;
	float finalFRGainMedium = frGainMedium * invRayCount;
	float finalFRGainHigh = frGainHigh * invRayCount;
	
	/*
	float frRatioLow = 0.0f;
	float frRatioMedium = 0.0f;
	float frRatioHigh = 0.0f;
	
	if( gainLow > FLOAT_SAFE_EPSILON ){
		frRatioLow = frGainLow / gainLow;
	}
	if( gainMedium > FLOAT_SAFE_EPSILON ){
		frRatioMedium = frGainMedium / gainMedium;
	}
	if( gainHigh > FLOAT_SAFE_EPSILON ){
		frRatioHigh = frGainHigh / gainHigh;
	}
	
	const float finalFRGainLow = roomRevIntLow * frRatioLow;
	const float finalFRGainMedium = roomRevIntMedium * frRatioMedium;
	const float finalFRGainHigh = roomRevIntHigh * frRatioHigh;
	*/
	
	
	// calculate late-reverberation gain
	/*
	float lrRatioLow = 0.0f;
	float lrRatioMedium = 0.0f;
	float lrRatioHigh = 0.0f;
	
	if( gainLow > FLOAT_SAFE_EPSILON ){
		lrRatioLow = lrGainLow / gainLow;
	}
	if( gainMedium > FLOAT_SAFE_EPSILON ){
		lrRatioMedium = lrGainMedium / gainMedium;
	}
	if( gainHigh > FLOAT_SAFE_EPSILON ){
		lrRatioHigh = lrGainHigh / gainHigh;
	}
	
	const float finalLRGainLow = roomRevIntLow * lrRatioLow;
	const float finalLRGainMedium = roomRevIntMedium * lrRatioMedium;
	const float finalLRGainHigh = roomRevIntHigh * lrRatioHigh;
	*/
	float finalLRGainLow = lrGainLow * invRayCount;
	float finalLRGainMedium = lrGainMedium * invRayCount;
	float finalLRGainHigh = lrGainHigh * invRayCount;
	
	
	// the calculated gains are "sound intensity". this can be converted into "sound power level":
	//   spl = 10 * log10(receiver-intensity)
	// 
	// for openal gain calculation we need "sound pressure". the "sound power level" is the
	// same in value as the "sound pressure level". with this we can go back to the openal
	// gain by using this formula:
	//   openal-gain = pow(2, spl / 6)
	// 
	// this is actually the same as going directly from receiver to openal gain like this:
	//   openal-gain = sqrt(receiver-intensity)
	finalFRGainLow = sqrtf( finalFRGainLow );
	finalFRGainMedium = sqrtf( finalFRGainMedium );
	finalFRGainHigh = sqrtf( finalFRGainHigh );
	finalLRGainLow = sqrtf( finalLRGainLow );
	finalLRGainMedium = sqrtf( finalLRGainMedium );
	finalLRGainHigh = sqrtf( finalLRGainHigh );
	
	// gains are clamped by a threshold in the sound trace task. this is by default around
	// -60db or 0.001 . openal though goes down to -100db by definition. we need to scale
	// the gains to avoid sound coming from nowhere near the lower end
	/*
	const float gainThreshold = 1e-3f;
	const float normGainScale = 1.0f / ( 1.0f - gainThreshold );
	const float normGainOffset = -gainThreshold * normGainScale;
	
	finalFRGainLow = decMath::max( finalFRGainLow * normGainScale + normGainOffset, 0.0f );
	finalFRGainMedium = decMath::max( finalFRGainMedium * normGainScale + normGainOffset, 0.0f );
	finalFRGainHigh = decMath::max( finalFRGainHigh * normGainScale + normGainOffset, 0.0f );
	finalLRGainLow = decMath::max( finalLRGainLow * normGainScale + normGainOffset, 0.0f );
	finalLRGainMedium = decMath::max( finalLRGainMedium * normGainScale + normGainOffset, 0.0f );
	finalLRGainHigh = decMath::max( finalLRGainHigh * normGainScale + normGainOffset, 0.0f );
	*/
	
	
	// calculate first and late reverberation delay
// 	float frAvgDelay = 0.0f;
	float frMinDelay = 0.0f;
// 	float frMaxDelay = 0.0f;
	if( frCount > 0 ){
// 		frAvgDelay = ( frSumDistance / ( float )frCount - directDistance ) * INV_SOUND_SPEED;
		frMinDelay = ( frMinDistance - directDistance ) * INV_SOUND_SPEED;
// 		frMaxDelay = ( frMaxDistance - directDistance ) * INV_SOUND_SPEED;
		( void )frSumDistance;
		( void )frPanDirectionWeightSum;
		( void )lrPanDirectionWeightSum;
		( void )lrCount;
	}
	
// 	const float lrGainLow = gainLow - frGainLow;
// 	const float lrGainMedium = gainMedium - frGainMedium;
// 	const float lrGainHigh = gainHigh - frGainHigh;
	
	
	// calculate the reverebration time using RT60
// 	const float lrTimeLow = lrMaxDistanceLow * INV_SOUND_SPEED;
// 	const float lrTimeMedium = lrMaxDistanceMedium * INV_SOUND_SPEED;
// 	const float lrTimeHigh = lrMaxDistanceHigh * INV_SOUND_SPEED;
	
	// 1.5 is derived from comparing different EAX preset. this value is rather similar
	// across profiles with exception to open profiles like "forest" or "plain". these
	// show values between 0.35 and 0.55 .
// 	const float revDelay = frAvgDelay * 1.5f;
	const float revDelay = frMinDelay * 1.5f;
	
	// room specific parameters. if pListenProbe exists this is the combination of the
	// pListenProbe and sourceProbe
#if 0
	float reverberationTimeLow = pSourceProbe->GetReverberationTimeLow();
	float reverberationTimeMedium = pSourceProbe->GetReverberationTimeMedium();
	float reverberationTimeHigh = pSourceProbe->GetReverberationTimeHigh();
	float echoDelay = pSourceProbe->GetEchoDelay();
	
	if( pListenProbe ){
		if( pListenProbe->GetReverberationTimeLow() > reverberationTimeLow ){
			reverberationTimeLow = pListenProbe->GetReverberationTimeLow();
		}
		if( pListenProbe->GetReverberationTimeMedium() > reverberationTimeMedium ){
			reverberationTimeMedium = pListenProbe->GetReverberationTimeMedium();
		}
		if( pListenProbe->GetReverberationTimeHigh() > reverberationTimeHigh ){
			reverberationTimeHigh = pListenProbe->GetReverberationTimeHigh();
		}
		if( pListenProbe->GetEchoDelay() > echoDelay ){
			echoDelay = pListenProbe->GetEchoDelay();
		}
	}
#endif
	
	float reverberationTimeLow = 0.0f;
	float reverberationTimeMedium = 0.0f;
	float reverberationTimeHigh = 0.0f;
	float echoDelay = 0.0f;
	
	if( pListenProbe ){
		// this uses the global listener reverberation parameters
		/*
		float reverberationTimeLow = pListenProbe->GetReverberationTimeLow();
		float reverberationTimeMedium = pListenProbe->GetReverberationTimeMedium();
		float reverberationTimeHigh = pListenProbe->GetReverberationTimeHigh();
		float echoDelay = pListenProbe->GetEchoDelay();
		*/
		
		// this calculates the reverberation parameters from the actual ray segment hits. this
		// provides a calculation more closely linked to the different path the sound took. this
		// way a sound sources in different types of rooms sound different which would not be
		// the case with globel listener parameters
		echoDelay = meanFreePath * INV_SOUND_SPEED;
		const float rtfactor = 13.8f * -echoDelay;
		if( absorptionLow > FLOAT_SAFE_EPSILON ){
			reverberationTimeLow = rtfactor / logf( decMath::max( 1.0f - absorptionLow, 1e-5f ) );
		}
		if( absorptionMedium > FLOAT_SAFE_EPSILON ){
			reverberationTimeMedium = rtfactor / logf( decMath::max( 1.0f - absorptionMedium, 1e-5f ) );
		}
		if( absorptionHigh > FLOAT_SAFE_EPSILON ){
			reverberationTimeHigh = rtfactor / logf( decMath::max( 1.0f - absorptionHigh, 1e-5f ) );
		}
		
	}else{
		reverberationTimeLow = pSourceProbe->GetReverberationTimeLow();
		reverberationTimeMedium = pSourceProbe->GetReverberationTimeMedium();
		reverberationTimeHigh = pSourceProbe->GetReverberationTimeHigh();
		echoDelay = pSourceProbe->GetEchoDelay();
	}
	
	// limit the reverberation. required for outdoor scenes since the mean-free-path calculation
	// assumes closed spaces. for open spaces this calculates reverberation times in the >15s
	// which is way too large. city spaces have around 1.5-2.5s reverberation times. what we
	// do here is figuring out how many rays shoot into the void and limit the reverberation
	// tim in this case. this code is not going to fire for indoor scenes at all so it is safe
	// to use as a fix for outdoor scenes
	const int modifyRevTimeCount = limitRevTimeCount + unlimitRevTimeCount;
	if( modifyRevTimeCount > 0 ){
		const float factor = 1.0f / ( float )( limitRevTimeCount + unlimitRevTimeCount );
		reverberationTimeLow = ( limitRevTimeLow + reverberationTimeLow * unlimitRevTimeCount ) * factor;
		reverberationTimeMedium = ( limitRevTimeMedium + reverberationTimeMedium * unlimitRevTimeCount ) * factor;
		reverberationTimeHigh = ( limitRevTimeHigh + reverberationTimeHigh * unlimitRevTimeCount ) * factor;
	}
	
	// update environment probe with found parameters
// 	float listenReverberationGainLow = 0.0f;
// 	float listenReverberationGainMedium = 0.0f;
// 	float listenReverberationGainHigh = 0.0f;
	
	if( pListenProbe ){
		// in listener centric mode things are a bit more complicated. the calculated parameters
		// represent the room the listener is located in but we need also the room the source
		// is located in. this is done by using the source probe parameters and somehow merge
		// them with the calculated parameters. the source parameters are either estimated or
		// calculated. both are fine and usually only estimated are present.
		// 
		// there is actually no correct way to merge the results except using full impulse
		// response processing.
		// 
		// to fake a result we use the maximum value of both environments. this represents
		// situations like a source being located in a bath room while the listener is located
		// in a hallway. the sound should reverb due to the bath room environment.
		// 
		// correctly the hallway reverbe would add to the bath room reverb. but if both source
		// and listener are in the same room the values would double if added.
		// 
		// for this reason the maximum is used. it is the least "wrong" in the majority of cases.
		
		// NOTE this code has a problem here. it would calculate the reverberation gain as if
		//      the listener is located in the same room as the sound source. the gain has
		//      though to be reduced depending on the amount of sound rays actually arriving
		//      at the pListener-> to do this correctly we would need the ratio between the actual
		//      gain arriving at the listener and the full gain in the middle of the room. this
		//      though is not so simple to properly estimate so we skip it for the time being
		
		#if 0
		
		float roomAbsorptionLow = 0.0f;
		float roomAbsorptionMedium = 0.0f;
		float roomAbsorptionHigh = 0.0f;
		if( pListenProbe->GetRoomSurface() > FLOAT_EPSILON ){
			const float invRoomSurface = 1.0f / pListenProbe->GetRoomSurface();
			roomAbsorptionLow = pListenProbe->GetRoomSabineLow() * invRoomSurface;
			roomAbsorptionMedium = pListenProbe->GetRoomSabineMedium() * invRoomSurface;
			roomAbsorptionHigh = pListenProbe->GetRoomSabineHigh() * invRoomSurface;
		}
		
		const float roomGLow = 10.0f
			* log10f( decMath::max( FLOAT_SAFE_EPSILON, 4.0f * ( 1.0f - roomAbsorptionLow )
			/ decMath::max( pListenProbe->GetRoomSabineLow(), 0.01f ) ) );
		const float roomGMedium = 10.0f
			* log10f( decMath::max( FLOAT_SAFE_EPSILON, 4.0f * ( 1.0f - roomAbsorptionMedium )
			/ decMath::max( pListenProbe->GetRoomSabineMedium(), 0.01f ) ) );
		const float roomGHigh = 10.0f
			* log10f( decMath::max( FLOAT_SAFE_EPSILON, 4.0f * ( 1.0f - roomAbsorptionHigh )
			/ decMath::max( pListenProbe->GetRoomSabineHigh(), 0.01f ) ) );
			
		// roomG is difference of sound volume in dB relative to the original sound volume.
		// converting this to linear scaling factor is enough
		listenReverberationGainLow = powf( 2.0f, roomGLow / 6.0f );
		listenReverberationGainMedium = powf( 2.0f, roomGMedium / 6.0f );
		listenReverberationGainHigh = powf( 2.0f, roomGHigh / 6.0f );
		
		if( listenReverberationGainLow > finalLRGainLow ){
			finalLRGainLow = listenReverberationGainLow;
		}
		if( listenReverberationGainMedium > finalLRGainMedium ){
			finalLRGainMedium = listenReverberationGainMedium;
		}
		if( listenReverberationGainHigh > finalLRGainHigh ){
			finalLRGainHigh = listenReverberationGainHigh;
		}
		
		#endif
	}
	
// 	pListener->SetReflected( finalFRGainLow, finalFRGainMedium, finalFRGainHigh, frAvgDelay );
	pListener->SetReflected( finalFRGainLow, finalFRGainMedium, finalFRGainHigh, frMinDelay );
	pListener->SetReflectionPan( frPanDirection );
	
	pListener->SetReverberation( finalLRGainLow, finalLRGainMedium, finalLRGainHigh, revDelay );
	pListener->SetReverberationPan( lrPanDirection );
	
	pListener->SetReverberationTime( reverberationTimeLow, reverberationTimeMedium, reverberationTimeHigh );
	pListener->SetEcho( echoDelay );
}
