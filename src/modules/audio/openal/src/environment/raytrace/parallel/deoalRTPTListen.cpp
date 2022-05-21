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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoalRTParallelEnvProbe.h"
#include "deoalRTPTListen.h"
#include "../deoalSoundRay.h"
#include "../deoalSoundRaySegment.h"
#include "../deoalRayTraceHitElement.h"
#include "../../deoalEnvProbe.h"
#include "../../../deoalBasics.h"
#include "../../../deAudioOpenAL.h"
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"
#include "../../../world/deoalAWorld.h"
#include "../../../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalRTPTListen
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalRTPTListen::deoalRTPTListen( deoalRTParallelEnvProbe &owner ) :
deParallelTask( &owner.GetAudioThread().GetOal() ),
pOwner( owner ),
pWorld( NULL ),
    pRTWorldBVH( NULL ),
pFirstRay( 0 ),
#ifndef RTPTL_ONE_TASK_PER_RAY
pRayCount( 0 ),
#endif
pListenProbe( NULL ),
pSourceProbe( NULL )
{
	(void)pOwner; // silence compiler warning
	
	pWOVRayHitsElement.SetResult( &pRTResult );
	pRTWOVRayHitsElement.SetResult( &pRTResult );
	SetMarkFinishedAfterRun( true );
	SetLowPriority( true );
}

deoalRTPTListen::~deoalRTPTListen(){
}



// Manegement
///////////////

void deoalRTPTListen::SetWorld( deoalAWorld *world, deoalRTWorldBVH *rtWorldBVH ){
	pWorld = world;
	pRTWorldBVH = rtWorldBVH;
}

void deoalRTPTListen::SetFirstRay( int firstRay ){
	pFirstRay = firstRay;
}

#ifndef RTPTL_ONE_TASK_PER_RAY
void deoalRTPTListen::SetRayCount( int rayCount ){
	pRayCount = rayCount;
}
#endif

void deoalRTPTListen::SetListenPosition( const decDVector &position ){
	pListenPosition = position;
}

void deoalRTPTListen::SetListenProbe( const deoalEnvProbe *probe ){
	pListenProbe = probe;
}

void deoalRTPTListen::SetSourceProbe( const deoalEnvProbe *probe ){
	pSourceProbe = probe;
}

void deoalRTPTListen::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}



void deoalRTPTListen::Run(){
	if( ! pSourceProbe ){
		DETHROW( deeInvalidParam );
	}
	
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	pRTWOVRayBlocked.StartTiming();
	#endif
	
	pWOVRayHitsElement.SetLayerMask( pLayerMask );
	pWOVRayBlocked.SetLayerMask( pLayerMask );
	
	const float range = pSourceProbe->GetRange();
	
	pFRSumDistance = 0.0f;
	pFRMinDistance = range;
	pFRMaxDistance = 0.0f;
	pFRGainLow = 0.0f;
	pFRGainMedium = 0.0f;
	pFRGainHigh = 0.0f;
	pFRPanDirection.SetZero();
	pFRPanDirectionWeightSum = 0.0f;
	pFRCount = 0;
	
	pLRGainLow = 0.0f;
	pLRGainMedium = 0.0f;
	pLRGainHigh = 0.0f;
	pLRPanDirection.SetZero();
	pLRPanDirectionWeightSum = 0.0f;
	pLRCount = 0;
	
	pHasHighestGain = false;
	pHighestGainGain = 0.0f;
	pHighestGainDistance = 0.0f;
	
	pAbsorptionSumLow = 0.0f;
	pAbsorptionSumMedium = 0.0f;
	pAbsorptionSumHigh = 0.0f;
	pAbsorptionCount = 0;
	
	pMeanFreePathSum = 0.0f;
	pMeanFreePathCount = 0;
	
	pDecayTimeSumLow = 0.0f;
	pDecayTimeSumMedium = 0.0f;
	pDecayTimeSumHigh = 0.0f;
	pDecayTimeCount = 0;
	
	pLimitRevTimeSumLow = 0.0f;
	pLimitRevTimeSumMedium = 0.0f;
	pLimitRevTimeSumHigh = 0.0f;
	pLimitRevTimeCount = 0;
	pUnlimitRevTimeCount = 0;
	
	pDirectSoundDistance = ( pListenPosition - pSourceProbe->GetPosition() ).Length();
	
	pSepDistFirstLateRefl = pDirectSoundDistance;
	if( pListenProbe ){
		//pSepDistFirstLateRefl += decMath::max( pSourceProbe->GetSepTimeFirstLateRefl(),
		//	pListenProbe->GetSepTimeFirstLateRefl() ) * SOUND_SPEED;
		pSepDistFirstLateRefl += pListenProbe->GetSepTimeFirstLateRefl() * SOUND_SPEED;
		
	}else{
		pSepDistFirstLateRefl += pSourceProbe->GetSepTimeFirstLateRefl() * SOUND_SPEED;
	}
	
	pImpulseResponse.Clear();
	
	/*
	NOTE about the performance of the different approaches:
	
	listener sphere:
		room: finalFRGain=(0.162,0.157,0.151) finalLRGain=(0.882,0.777,0.678) time=0.80ms
		outside: finalFRGain=(0.000,0.000,0.000) finalLRGain=(0.022,0.018,0.015) time=0.12ms
		=> with increasing range or ray count the gain raises out of control (over 10 for
		=> late reverberation gain). this is not working
		
		corner, room, deadEnd:
		300m, L0: lrGain=(1.8,2.3,0.1) rays=12k time=(5,2)ms
		300m, L2: lrGain=(10,10,1) rays=200k time=(70,8)ms
	
	gauss beam:
		room: finalFRGain=(0.030,0.029,0.027) finalLRGain=(0.146,0.129,0.114) time=0.73ms
		outside: finalFRGain=(0.000,0.000,0.000) finalLRGain=(0.002,0.002,0.002) time=0.41ms
		=> with increasing range the late reverberation gain seems fine for 300m range.
		=> with 30m range this becomes inaudible too early.
		=> also with increasing ray count the gain drop raises out of control (0.030 inside
		=> room instead of 0.130 with low ray count). this is not working
		
		corner, room, deadEnd: => 2x opening angle
		300m, L0: lrGain=(0.27,0.32,0.013)
		300m, L2: lrGain=(0.14,0.17,0.008)
		
		300m, L0: lrGain=(0.30,0.35,0.013) rays=12k time=(6,2)ms
		300m, L2: lrGain=(0.16,0.20,0.013) rays=200k time=(90,25)ms
		 30m, L0: lrGain=(0.16,0.20,0.001) rays=1.7k time=(1,0.25)ms
		 30m, L2: lrGain=(0.06,0.06,0.000) rays=26k time=(0.14,0.25)ms
		 30m, L0: lrGain=(0.30,0.37,0.002) rays=1.7k time=(1,0.25)ms    => rolloff = 0.5
		 30m, L2: lrGain=(0.10,0.12,0.001) rays=26k time=(0.14,0.25)ms  => rolloff = 0.5
		 30m, L0: lrGain=(0.53,0.65,0.004) rays=1.7k time=(1,0.25)ms    => rolloff = 0.25
		 30m, L2: lrGain=(0.19,0.21,0.001) rays=26k time=(0.14,0.25)ms  => rolloff = 0.25
		
		corner, room, deadEnd: => 3x opening angle
		300m, L0: lrGain=(0.31,0.37,0.016) rays=12k time=(6,2)ms
		300m, L2: lrGain=(0.21,0.25,0.011) rays=200k time=(90,32)ms
		 30m, L0: lrGain=(0.18,0.21,0.001) rays=1.7k time=(1,0.25)ms
		 30m, L2: lrGain=(0.08,0.10,0.000) rays=26k time=(0.14,0.25)ms
		
		corner, room, deadEnd: => 4x opening angle
		300m, L0: lrGain=(0.32,0.38,0.020) rays=12k time=(6,2)ms
		300m, L2: lrGain=(0.24,0.28,0.012) rays=200k time=(90,36)ms
		 30m, L0: lrGain=(0.19,0.23,0.002) rays=1.7k time=(1,0.25)ms
		 30m, L2: lrGain=(0.10,0.13,0.000) rays=26k time=(0.14,0.25)ms
		
		corner, room, deadEnd: => 1.5x opening angle
		300m, L0: lrGain=(0.27,0.33,0.013)
		300m, L2: lrGain=(0.14,0.16,0.010)
		 30m, L0: lrGain=(0.14,0.18,0.001)
		 30m, L2: lrGain=(0.04,0.04,0.000)
	
	linear beam:
		300m, L0: lrGain=(0.25,0.31,0.016)
		300m, L2: lrGain=(0.13,0.16,0.008)
		 30m, L0: lrGain=(0.13,0.16,0.001)
		 30m, L2: lrGain=(0.04,0.04,0.000)
		
		300m, L0: lrGain=(0.20,0.25,0.015) time=(5.5,2.0)ms
		300m, L2: lrGain=(0.08,0.09,0.007) time=(75,10)ms
		 30m, L0: lrGain=(0.08,0.10,0.001) time=(0,0)ms
		 30m, L2: lrGain=(0.01,0.01,0.000) time=(0,0)ms
		
		300m, L0: lrGain=(0.695,0.843,0.042)
		300m, L2: lrGain=(1.094,1.281,0.096)
		
	hit-counts-all:
		room: finalFRGain=(0.029,0.028,0.027) finalLRGain=(0.132,0.116,0.102) time=0.52ms
		outside: finalFRGain=(0.000,0.000,0.000) finalLRGain=(0.003,0.002,0.002) time=0.09ms
		=> with increasing range the late reverberation gain seems fine for 300m range.
		=> with 30m range this becomes inaudible too early.
		=> with increasing ray count the results stay rather similar.
	
	huygens:
		room: finalFRGain=(0.094,0.090,0.087) finalLRGain=(0.094,0.082,0.073) time=3.88ms
		outside: finalFRGain=(0.000,0.000,0.000) finalLRGain=(0.000,0.000,0.000) time=3.01ms
		=> too time consuming and bad results. not looking at this any more
	
	
	performance:
	- using high ray count works fine enough with the listener calculation part. this takes
	  roughly 0.22ms for listener sphere to deal with 34k ray segments. with 200k rays this
	  goes up to 10ms so care has to be still taken.
	
	*/
	
	/*
	large rev: 200, 80    300, 60
	furnished: 350, 200   150, 40
	small rev: 600, 250   280, 130
	thickness: 700, 150   40, 0
	common:    450, 200   370, 100
	dead-end:  250, 0     200, 0
	*/
// 	RunLinearBeam();
	RunSphereReceiver();
	
// 	RunGaussBeam();
// 	RunListenerSphere();
// 	RunHuygens();
	
	#ifdef RTWOVRAYBLOCKED_DO_TIMING
	pRTWOVRayBlocked.EndTiming();
	#endif
}

void deoalRTPTListen::Finished(){
	// this task never runs alone. it is always a dependency and clean up by these tasks
	/*
	if( GetDependedOnBy().GetCount() == 0 ){
		pOwner.Enable( this );
	}
	*/
}



// Debugging
//////////////

decString deoalRTPTListen::GetDebugName() const{
	return "OpenAL-RTPTListen";
}

decString deoalRTPTListen::GetDebugDetails() const{
	return "";
}



// Private Functions
//////////////////////

void deoalRTPTListen::RunLinearBeam(){
	const deoalSoundRayList &soundRayList = pListenProbe ?
		pListenProbe->GetSoundRayList() : pSourceProbe->GetSoundRayList();
	const decVector &targetPositionWorld = pListenProbe ?
		pSourceProbe->GetPosition() : pListenPosition;
	const decVector targetPosition( targetPositionWorld - ( pListenProbe ?
		pListenProbe->GetPosition() : pSourceProbe->GetPosition() ) );
	const decVector gainPosition( pListenProbe ?
		decVector( pListenPosition - pListenProbe->GetPosition() ) : targetPosition );
	deoalWorldOctree &octree = *pWorld->GetOctree();
	#ifndef RTPTL_ONE_TASK_PER_RAY
	const int lastRay = pFirstRay + pRayCount;
	int i;
	#endif
	
	const float baseBeamRadius = tanf( pListenProbe ?
		pListenProbe->GetRayOpeningAngle() : pSourceProbe->GetRayOpeningAngle() );
// 	const float beamNormalize = 1.0f / baseBeamRadius;
// 	const float beamNormalize = ( float )pEnvProbe->GetRayCount();
	
	/* 
	convergence testing (late-reverberation gain)
	
	dynamic radius, openingAngle*N*distance
	1:  0.268, 0.104 (*2.577)
	2:  0.326, 0.176 (*1.852)
	3:  0.359, 0.222 (*1.617)
	4:  0.370, 0.258 (*1.434)
	*/
	
	#ifndef RTPTL_ONE_TASK_PER_RAY
	for( i=pFirstRay; i<lastRay; i++ ){
		const deoalSoundRay &soundRay = soundRayList.GetRayAt( i );
	#else
		const deoalSoundRay &soundRay = soundRayList.GetRayAt( pFirstRay );
	#endif
		
		float closestBeamDistance = 0.0f;
		float closestDistSquared = 0.0f;
		int closestSegment = -1;
		int closestBounces = 0;
		
		ClosestToRay( closestBeamDistance, closestDistSquared, closestSegment, closestBounces,
			soundRayList, soundRay, targetPosition, baseBeamRadius, octree, targetPositionWorld );
		
		// if there is a closest segment apply the gain
		if( closestSegment == -1 ){
			#ifdef RTPTL_ONE_TASK_PER_RAY
			return;
			#else
			continue;
			#endif
		}
		
		// calculate weight
		float factor = /* beamNormalize * */
			decMath::max( 1.0f - sqrt( closestDistSquared )
				/ ( baseBeamRadius * closestBeamDistance ), 0.0f );
		
		// precalc already applied distance attenuation so no need to do it here again unless
		// listener probe is used. in this case no attenutation has been applied so far
		if( pListenProbe ){
			factor *= pSourceProbe->AttenuatedGain( closestBeamDistance );
		}
		
		// the calculation here is equal to an attenuation of 1/d. by multiplying by
		// d=closestListenerDistance this allowed to apply the artist specified attenuation
// 		const float factor = closestWeight * pSourceProbe->AttenuatedGain( closestListenerDistance )
// 			* closestListenerDistance;
		
		const deoalSoundRaySegment &segment = soundRayList.GetSegmentAt( closestSegment );
		const float gainLow = segment.GetGainLow() * factor;
		const float gainMedium = segment.GetGainMedium() * factor;
		const float gainHigh = segment.GetGainHigh() * factor;
		
		if( pListenProbe ){
			// for listener centric we can not use the segment position to calculate the direction.
			// this would calculate the direction from the source to the segment point. this is
			// not what we need. to get the correct result we can use the first segment of the
			// ray. the position of this segment lines up with the direction the ray has been
			// initially cast into
			ApplyIndirectGains( closestBounces, soundRayList.GetSegmentAt(
				soundRay.GetFirstSegment() ).GetPosition() - gainPosition,
				closestBeamDistance, gainLow, gainMedium, gainHigh );
			
		}else{
			ApplyIndirectGains( closestBounces, segment.GetPosition() - gainPosition,
				closestBeamDistance, gainLow, gainMedium, gainHigh );
		}
		
	#ifndef RTPTL_ONE_TASK_PER_RAY
	}
	#endif
}

void deoalRTPTListen::ClosestToRay( float &closestBeamDistance, float &closestDistSquared,
int &closestSegment, int &closestBounces, const deoalSoundRayList &soundRayList,
const deoalSoundRay &soundRay, const decVector &targetPosition, float baseBeamRadius,
deoalWorldOctree &octree, const decVector &targetPositionWorld ){
	const int segmentCount = soundRay.GetSegmentCount();
	const int firstSegment = soundRay.GetFirstSegment();
	int i;
	
	for( i=0; i<segmentCount; i++ ){
		const deoalSoundRaySegment &segment = soundRayList.GetSegmentAt( firstSegment + i );
		
		const decVector rayTargetDirection( targetPosition - segment.GetPosition() );
		const float lambda = segment.GetDirection() * rayTargetDirection;
		if( lambda < 0.0 || lambda > segment.GetLength() ){
			continue;
		}
		
		const decVector closest( segment.GetPosition() + segment.GetDirection() * lambda );
		const decVector targetClosestDirection( closest - targetPosition );
		
		// if the ray is outside the contribution radius ignore it
		const float beamDistance = segment.GetDistance() + lambda;
		const float tarClosDistSquared = targetClosestDirection.LengthSquared();
		
		const float beamRadius = baseBeamRadius * beamDistance;
		if( tarClosDistSquared > beamRadius * beamRadius ){
			continue;
		}
		
		// if the segment is farther away ignore it
		if( closestSegment != -1 && tarClosDistSquared >= closestDistSquared ){
			continue;
		}
		
		// if there is no direct path to the listener ignore it
		if( IsRayBlocked( octree, targetPositionWorld, targetClosestDirection ) ){
			continue;
		}
		
		// this segment is better. store it as best segment
		closestSegment = firstSegment + i;
		closestDistSquared = tarClosDistSquared;
		closestBeamDistance = beamDistance;
		closestBounces = segment.GetBounceCount();
		
		#if 0
		if( IsRayBlocked( octree, pListenerPosition, listenerDirection ) ){
			continue; // no direct path to the listener
		}
		
		const float attGain = factor * pSourceProbe->AttenuatedGain( listenerDistance );
		const float gainLow = segment.GetGainLow() * attGain;
		const float gainMedium = segment.GetGainMedium() * attGain;
		const float gainHigh = segment.GetGainHigh() * attGain;
		
		ApplyIndirectGains( j + 1, -rayListenerDirection, listenerDistance,
			gainLow, gainMedium, gainHigh );
		
		break; // ray has hit listener. stop processing the rest of the ray
		#endif
	}
	
	const int transmittedRayCount = soundRay.GetTransmittedRayCount();
	const int firstTransmittedRay = soundRay.GetFirstTransmittedRay();
	
	for( i=0; i<transmittedRayCount; i++ ){
		ClosestToRay( closestBeamDistance, closestDistSquared, closestSegment, closestBounces,
			soundRayList, soundRayList.GetTransmittedRayAt( firstTransmittedRay + i ),
			targetPosition, baseBeamRadius, octree, targetPositionWorld );
	}
}



deoalRTPTListen::sSphereReceiverParams::sSphereReceiverParams(
	const deoalSoundRayList& soundRayList, const decVector& targetPosition,
	deoalWorldOctree& octree, const decVector& targetPositionWorld,
	const decVector& gainPosition, float receiverRadius,
	float receiverRadiusSquared, float invReceiverVolume ) :
soundRayList( soundRayList ),
targetPosition( targetPosition ),
octree( octree ),
targetPositionWorld( targetPositionWorld ),
gainPosition( gainPosition ),
receiverRadius( receiverRadius ),
receiverRadiusSquared( receiverRadiusSquared ),
invReceiverVolume( invReceiverVolume ){
}

deoalRTPTListen::sSphereReceiverImpinge::sSphereReceiverImpinge() :
time( 0.0f ), low( 0.0f ), medium( 0.0f ), high( 0.0f ){
}

void deoalRTPTListen::RunSphereReceiver(){
	const deoalSoundRayList &soundRayList = pListenProbe ?
		pListenProbe->GetSoundRayList() : pSourceProbe->GetSoundRayList();
	const decVector &targetPositionWorld = pListenProbe ?
		pSourceProbe->GetPosition() : pListenPosition;
	const decVector targetPosition( targetPositionWorld - ( pListenProbe ?
		pListenProbe->GetPosition() : pSourceProbe->GetPosition() ) );
	const decVector gainPosition( pListenProbe ?
		decVector( pListenPosition - pListenProbe->GetPosition() ) : targetPosition );
	deoalWorldOctree &octree = *pWorld->GetOctree();
	#ifndef RTPTL_ONE_TASK_PER_RAY
	const int lastRay = pFirstRay + pRayCount;
	int i;
	#endif
	
	// r = maxFreePath * sqrt(2*pi / N)
	// r = pow(15*Vroom / (2*pi*N), 1/3)
	// 
	// example values large reverberant room:
	// r1 = 8.14m => using mean-free-path => 1.61m
	// r2 = 3.81m
	// 
	// another version:
	// Vreceiver = log10(Vroom) * (source-receiver).length * sqrt(4.0 / N)
	// r = pow(Vreceiver*3/(4*pi), 1/3) ~= 1.25
	// depends though a lot on the distance to the source
	const float scaleRecRad = 1.0f; //2.0f; //4.0f //8.0f
		// 4: works best for door-room test case
	const float fixGainScaleRecRad = scaleRecRad;
	
	const int rayCount = pListenProbe ? pListenProbe->GetRayCount() : pSourceProbe->GetRayCount();
	const float meanFreePath = pListenProbe ?
		pListenProbe->GetMeanFreePath() : pSourceProbe->GetMeanFreePath();
	const float receiverRadius = meanFreePath * sqrtf( PI * 2.0f / ( float )rayCount ) * scaleRecRad;
// 	const float receiverRadius = powf( 15.0f * pSourceProbe->GetRoomVolume()
// 		/ ( 2.0f * PI * rayCount ), 1.0f / 3.0f );
	const float receiverRadiusSquared = receiverRadius * receiverRadius;
	const float receiverVolume = decMath::max( 0.01f,
		( 4.0f / 3.0f ) * PI * receiverRadiusSquared * receiverRadius );
	const float invReceiverVolume = fixGainScaleRecRad/*1.0f*/ / receiverVolume;
	
	// TODO receiver radius can explode if speaker is located on an open plane. in this case
	//      each non-hit adds speaker.getRange() to the mean-free-path. this causes the receiver
	//      radius to be very large. maybe adding a capping on the radius? other solutions seem
	//      to be using the distance between microphone and speaker to judge the size of the
	//      receiver.
	
	const sSphereReceiverParams params( soundRayList, targetPosition, octree, targetPositionWorld,
		gainPosition, receiverRadius, receiverRadiusSquared, invReceiverVolume );
	
	#ifndef RTPTL_ONE_TASK_PER_RAY
	for( i=pFirstRay; i<lastRay; i++ ){
		const deoalSoundRay &soundRay = soundRayList.GetRayAt( i );
	#else
		const deoalSoundRay &soundRay = soundRayList.GetRayAt( pFirstRay );
	#endif
		
		RunSphereReceiver( params, soundRay, NULL );
	#ifndef RTPTL_ONE_TASK_PER_RAY
	}
	#endif
}

void deoalRTPTListen::RunSphereReceiver( const deoalRTPTListen::sSphereReceiverParams &params,
const deoalSoundRay &soundRay, const sSphereReceiverImpinge *firstImpinge ){
	const int segmentCount = soundRay.GetSegmentCount();
	if( segmentCount == 0 ){
		return;
	}
	
	const int firstSegment = soundRay.GetFirstSegment();
	int indexLastSegment = -1;
	int i;
	
	sSphereReceiverImpinge localFirstImpinge;
	sSphereReceiverImpinge lastImpinge;
	
	const float initialDistance = params.soundRayList.GetSegmentAt( firstSegment ).GetDistance();
	
	for( i=0; i<segmentCount; i++ ){
		const deoalSoundRaySegment &segment = params.soundRayList.GetSegmentAt( firstSegment + i );
		
		// we could ignore closest points if they are outside the ray segment. this would
		// though reject some rays which contribute to the listener sphere. to account for
		// this we move the dri calculation before the ray-block check. this way if the dri
		// becomes negative neither the start nor the end point is inside the sphere. avoids
		// a bunch of calculations
		const decVector rayTargetDirection( params.targetPosition - segment.GetPosition() );
		
		// NOTE performance measurements for the diffuse and specular versions (rays bounces):
		// specular 48 20: 0.2 ms
		// diffuse 48 20: 14-20 ms
		// diffuse 24 20: 8 ms
		// 
		// NOTE after some optimizations
		// diffuse 48 20: 0.4-12ms
		// diffuse 24 20: 0.2-7ms
		
		#if 1
		// this is the fully diffuse case. the idea here is the following. we need to keep the
		// probability approach working. for this use the direction from segment origin to
		// target and the receiver radius to calculate the opening angle under which the entire
		// receiver sphere is seen. we thus look at a cone starting at the segment origin which
		// is wide enough to just fully contain the receiver sphere. this cone applies the sound
		// energy to the entire receiver sphere volume in contrary to the fully specular version
		// where the sound energy is applied only to a ray segment of the receiver sphere.
		// the cone though is only a fraction of the full enegry so the gain reduction happens
		// here due to the ratio of the cone base surface area compared with the half sphere
		// instead of the fraction of receiver sphere volume hit. if the segment origin is
		// inside the listener sphere the full energy is applied
		
		// ensure the receiver sphere is in front of the hit face. use a blending factor to
		// avoid ripping off the gain when crossing the face
		const float distToFace = segment.GetNormal() * rayTargetDirection;
		if( distToFace <= 0.0f ){
			continue;
		}
		
		float factor = decMath::min( distToFace / params.receiverRadius, 1.0f );
		
		if( IsRayBlocked( params.octree, params.targetPositionWorld, -rayTargetDirection ) ){
			continue; // if there is no direct path to the listener ignore it
		}
		
		const float segTarDistance = rayTargetDirection.Length();
		
		#if 0
		if( segTarDistance > params.receiverRadius + 0.001f ){
			// the sphere has a surface area of 4*pi*r^2. the half sphere has thus a surface
			// area of 2*pi*r^2. the cone has a surface on the the sphere of 2*pi*r*h with h
			// the height of the cone cap. to calculate the required parameters we use a line
			// starting at the segment origin and touching the receiver sphere. the distance
			// from the segment origin to the contact point is the radius of the sphere (r).
			// together with the line from the segment origin to the receiver sphere center
			// and the line from the contact point to the receiver sphere center we have now
			// a right-angle triangle. we can now calculate the cone angle like this
			// (d="distance to receiver sphere" and rsr="receiver sphere radius":
			//   rsr = d * sin(a)
			//   sin(a) = rsr / d
			// 
			// also required calculating the radius of this sphere:
			//   r = sqrt(d * d - rsr * rsr)
			// 
			// we can now calculate the distance d' which runs along the distance d up to the
			// cone cap base (perpendicular to d):
			//   d' = r * cos(a)
			// 
			// a bit of goniometry helps using cos(a) = sqrt(1 - sin(a)^2):
			//   d' = r * sqrt(1 - (rsr / d)^2)
			//   d' = r * sqrt(1 - rsr*rsr / d*d)
			// 
			// and thus:
			//   h = r - d'
			//   h = r - r * sqrt(1 - rsr*rsr / d*d)
			//   h = r * (1 - sqrt(1 - rsr*rsr / d*d))
			// 
			// using the cone cap area formula above this results in (ca="cone surface area"):
			//   ca = 2 * pi * r * h
			// 
			// the ratio of the cone surface area to the half unit sphere area is then:
			//   ratio = (2 * pi * r * h) / (2 * pi * r * r)
			//         = h / r
			//         = r * (1 - sqrt(1 - rsr*rsr / d*d)) / r
			//   ratio = 1 - sqrt(1 - rsr*rsr / d*d)
			factor *= 1.0f - sqrtf( 1.0f - params.receiverRadiusSquared / ( segTarDistance * segTarDistance ) );
		}
		
		#else
		// 1/surfaceHalfSphere = 1/(2*pi*r*r) = 1/(2*pi) given r=1
		// we consider the receiver as a single point and the segment origin to radiate all
		// incoming sound energy equally across the half sphere surface. hence 1/(2*pi) of
		// the sound power is impinged on the receiver
		
		/*
		factor *= 1.0f / ( 2.0f * PI );
		if( pListenProbe ){
			const float a = pSourceProbe->AttenuatedGain( segment.GetDistance() + segTarDistance );
			factor *= a;
			//factor *= a * a;
		}
		*/
		
		
		if( pListenProbe ){
			// we work with intensities here so we need 1/(r*r) . to support artist chosen
			// roll-off factor this has to be squared(1/(rollOff*r)).
			const float attenuation = pSourceProbe->AttenuatedGain(
				segment.GetDistance() + segTarDistance );
			factor *= attenuation * attenuation;
		}
		
		
		/*
		// sphere surface 4*pi*r*r . use "virtual sound source". the energy directly hitting
		// the receiver as single point is 1/(4*pi*r*r) .
		const float fdist = segment.GetDistance() + segTarDistance;
		
		if( pListenProbe ){
			// to support artist chosen roll-off the calculation is modified. basicall the
			// attenuation gain using artist roll-off is
			//   att = 1/(rollOff*distance)
			// 
			// first rewriting the calculation:
			//   factor = 1/(4*pi) * 1/r * 1/r
			// 
			// now if rollOff=1 then att=1/r . thus the calculation can be written as:
			//   factor = 1/(4*pi) * att * att
			const float att = pSourceProbe->AttenuatedGain( fdist );
			factor *= att * att / ( 4.0f * PI );
			
			// NOTE
			// according to paper using receiver sphere the impinged intensity is:
			//   I = Wi * dri / Vr
			// 
			// if ray passes through the sphere center then
			//   dri = 2 * r
			// 
			// and thus:
			//   I = Wi * 2*r / (4/3 * pi * r*r*r)
			//     = Wi * 2 / (4/3 * pi * r*r)
			//   I = Wi * 6 / (4 * pi * r*r)
			// 
			// thus we obtain:
			//   I = Wi * 6 * factor
			factor *= 6.0f;
			
		}else{
			factor *= 6.0f / ( 4.0f * PI * fdist * fdist );
		}
		*/
		#endif
		
		const float lambda = segTarDistance;
		
		#else
		const float lambda = segment.GetDirection() * rayTargetDirection;
		
		#if 0
		const decVector closest( segment.GetPosition() + segment.GetDirection() * lambda );
		const decVector targetClosestDirection( closest - params.targetPosition );
		
		const float gaussWidthFactor = tanf( pListenProbe ?
			pListenProbe->GetRayOpeningAngle() : pSourceProbe->GetRayOpeningAngle() );
		
		const float listenerDistance = segment.GetDistance() + lambda; // => receiveDistance
		
		const float tarClosDistSquared = targetClosestDirection.LengthSquared();
		const float beamWidth = gaussWidthFactor * listenerDistance;
		const float beamWidthSquared = beamWidth * beamWidth;
		if( tarClosDistSquared > beamWidthSquared ){
			continue;
		}
		
		if( IsRayBlocked( params.octree, params.targetPositionWorld, targetClosestDirection ) ){
			continue; // if there is no direct path to the listener ignore it
		}
		
		const float RAY_GAUSS_WIDTH_INV_SIGMA = 2.35482f;
		const float RAY_GAUSS_B = -0.5f;
		const float RAY_GAUSS_A = 0.398942f;
		
		const float invSigma = RAY_GAUSS_WIDTH_INV_SIGMA / beamWidth;
		const float factor = RAY_GAUSS_A * invSigma * expf( RAY_GAUSS_B * invSigma * invSigma * tarClosDistSquared );
		if( factor <= 0.0f ){
			continue;
		}
		
		#else
		if( lambda < -params.receiverRadius || lambda > segment.GetLength() + params.receiverRadius ){
			continue;
		}
		
		// if the ray is outside the contribution radius ignore it
		const decVector closest( segment.GetPosition() + segment.GetDirection() * lambda );
		const decVector targetClosestDirection( closest - params.targetPosition );
		
		const float tarClosDistSquared = targetClosestDirection.LengthSquared();
		if( tarClosDistSquared > params.receiverRadiusSquared ){
			continue;
		}
		
		// the original calculation for dRI only works if the ray pierces the listener sphere.
		// if the ray originates inside or ends inside the listener sphere dri would be too
		// large producing a too large contribution for short rays. this is corrected by
		// looking at the receive distance combined with the calculated dri. we simply clamp
		// the interval [recD-dri/2 .. recD+dri/2] to [0 .. segmentLength]. the length of the
		// interval is then the final dri to use
		//const float dri = 2.0f * sqrtf( params.receiverRadiusSquared - tarClosDistSquared );
		const float halfDri = sqrtf( params.receiverRadiusSquared - tarClosDistSquared );
		const float dri = decMath::min( lambda + halfDri, segment.GetLength() )
			- decMath::max( lambda - halfDri, 0.0f );
		if( dri <= 0.0f ){
			continue;
		}
		
		// if there is no direct path to the listener ignore it
		if( IsRayBlocked( params.octree, params.targetPositionWorld, targetClosestDirection ) ){
			continue;
		}
		
		// this one is tricky here. the ray-tracing by nature applies the distance attenuation
		// by spreading the surface area which does the 1/r^2 law. using listener-centric
		// no attenutation has been applied so this would be physically correct. to allow though
		// the artistic values to take effect we need to apply an artificial attenuation factor.
		// this is done by taking the ratio between the artistic attenuation curve and the
		// realistic attenuation curve. this is (1/r')/(1/r) = aa/(1/r) = aa*r
		factor = dri * params.invReceiverVolume;
		#endif
		#endif
		
		const float receiveDistance = segment.GetDistance() + lambda;
		if( pListenProbe ){
// 			factor *= pSourceProbe->AttenuatedGain( receiveDistance ) * receiveDistance;
		}
		
		const float gainLow = segment.GetGainLow() * factor;
		const float gainMedium = segment.GetGainMedium() * factor;
		const float gainHigh = segment.GetGainHigh() * factor;
		
		if( pListenProbe ){
			// for listener centric we can not use the segment position to calculate the direction.
			// this would calculate the direction from the source to the segment point. this is
			// not what we need. to get the correct result we can use the first segment of the
			// ray. the position of this segment lines up with the direction the ray has been
			// initially cast into
				
// 			if(pSourceProbe->GetPosition().IsEqualTo(decDVector(-24.3,1.6,56.7))){
// 				pOwner.GetAudioThread().GetLogger().LogInfoFormat(
// 					"ApplyIndirectGains ray=%d seg=%d bounce=%d trans=%d gain=%f,%f,%f tcds=%f"
// 					" lambda=%f halfDri=%f dri=%f factor=%f",
// 					pFirstRay, i, segment.GetBounceCount(), segment.GetTransmittedCount(),
// 					gainLow, gainMedium, gainHigh, sqrtf(tarClosDistSquared), lambda, halfDri, dri, factor );
// 			}
			
			ApplyIndirectGains( segment.GetBounceCount(),
				params.soundRayList.GetSegmentAt( soundRay.GetFirstSegment() )
					.GetPosition() - params.gainPosition,
				receiveDistance, gainLow, gainMedium, gainHigh );
			
			// also update the absorption and mean free path sum. we use the segment distance
			// but minus the length of the initially cast ray. also the length of the current
			// segment is used to get one more entry for averaging unless the segment is the
			// last segment in the ray and the ray hits nothing. only problem is that if the
			// ray contributes multiple times the later hits will also add again the length of
			// the previous hits causing the early hits to out-weight the later hits.
			// to prevent this we keep track of the last segment distance. then we can add
			// this contribution one at the end and things are correct.
			indexLastSegment = firstSegment + i;
			
			// NOTE
			// we are not using gainLow, gainMedium and gainHigh since these include impinge
			// probability factor. for the decay-time calculation this would cause impulse
			// entries along the ray not to be monotonic declining. by using only the segment
			// gain values this can be enforced. this is an acceptable error
			lastImpinge.time = receiveDistance * INV_SOUND_SPEED;
			lastImpinge.low = decMath::max( /*segment.GetGainLow()*/ gainLow, FLOAT_SAFE_EPSILON );
			lastImpinge.medium = decMath::max( /*segment.GetGainMedium()*/ gainMedium, FLOAT_SAFE_EPSILON );
			lastImpinge.high = decMath::max( /*segment.GetGainHigh()*/ gainHigh, FLOAT_SAFE_EPSILON );
			
			if( ! firstImpinge ){
				localFirstImpinge = lastImpinge;
				firstImpinge = &localFirstImpinge;
			}
			
		}else{
			ApplyIndirectGains( segment.GetBounceCount(),
				segment.GetPosition() - params.gainPosition,
				receiveDistance, gainLow, gainMedium, gainHigh );
		}
	}
	
	if( indexLastSegment != -1 ){
		const deoalSoundRaySegment &segment = params.soundRayList.GetSegmentAt( indexLastSegment );
		
		pAbsorptionSumLow += segment.GetAbsorptionSumLow();
		pAbsorptionSumMedium += segment.GetAbsorptionSumMedium();
		pAbsorptionSumHigh += segment.GetAbsorptionSumHigh();
		pAbsorptionCount += segment.GetBounceCount();
		
		if( /* i == segmentCount - 1 && */ soundRay.GetOutside() ){
			const float mfpDist = segment.GetDistance() - initialDistance;
			
			pMeanFreePathSum += mfpDist;
			pMeanFreePathCount += segment.GetBounceCount() - 1;
			
			// this is one hell of a hack to deal with the problem of open spaces causing huge
			// reverberation times due to huge mean-free-path values. the idea here is the
			// following. if the ray leaves the range and flies into outer space the ray can
			// not contribute to the reverberation anymore after this last hit. this should
			// truncate the reverberation time to account for this. there is no sane way to
			// calculate this truncating so we use a simple approach since it is as good as
			// any other. using time=0 and intensity=1 point and the hit we just added we
			// calculate a virtual RT60. this becomes our truncation point. all rays not
			// leaving range will use the mean-free-path calculate RT60 as their limit RT60.
			// hence averaging all those limit RT60 yields an RT60 which is truncated stronger
			// the more rays leave range.
			float limitRevTimeLow = 0.0f;
			float limitRevTimeMedium = 0.0f;
			float limitRevTimeHigh = 0.0f;
			
			if( firstImpinge ){
				const float diffTime = lastImpinge.time - firstImpinge->time;
				if( diffTime > 1e-3f ){
					const float diffPowerDbLow = -10.0f * log10f( lastImpinge.low / firstImpinge->low );
					if( diffPowerDbLow > 1e-4f ){
						limitRevTimeLow = diffTime * 60.0f / diffPowerDbLow;
					}
					
					const float diffPowerDbMedium = -10.0f * log10f( lastImpinge.medium / firstImpinge->medium );
					if( diffPowerDbMedium > 1e-4f ){
						limitRevTimeMedium = diffTime * 60.0f / diffPowerDbMedium;
					}
					
					const float diffPowerDbHigh = -10.0f * log10f( lastImpinge.high / firstImpinge->high );
					if( diffPowerDbHigh > 1e-4f ){
						limitRevTimeHigh = diffTime * 60.0f / diffPowerDbHigh;
					}
				}
			}
			
			pLimitRevTimeSumLow = limitRevTimeLow;
			pLimitRevTimeSumMedium = limitRevTimeMedium;
			pLimitRevTimeSumHigh = limitRevTimeHigh;
			pLimitRevTimeCount++;
			
		}else{
			pMeanFreePathSum += segment.GetDistance() - initialDistance + segment.GetLength();
			pMeanFreePathCount += segment.GetBounceCount();
			pUnlimitRevTimeCount++;
		}
		
		// decay time calculation. we use RT60 for this. to calculate the RT60 we calculate
		// the power difference in dB and the time difference between the first contributing
		// segment and the last contributing segment. this results in an RT-X value where
		// X is difference in power in dB. by scaling the time to RT60 we get the decay time.
		// as with mean free path this is summed up to average later.
		// 
		// the impinge values are intensities. RT60 is defined in terms of sound pressure level.
		// the sound intensity level and sound pressure level are though identical in value.
		// hence it is enough to convert intensity to sound intensity level using:
		//   SIL = 10 * log10(intensity).
		#ifdef USE_DECAY_CURVE
		if( firstImpinge ){
			const float diffTime = lastImpinge.time - firstImpinge->time;
			if( diffTime > 1e-3f ){
				// SIL = 10 * log10(impinge.gain)
				// diffSIL = SILLast - SILFirst = 10 * log10(impingeLast / impingeFirst)
				const float diffPowerDbLow = -10.0f * log10f( lastImpinge.low / firstImpinge->low );
				if( diffPowerDbLow > 1e-4f ){
					pDecayTimeSumLow += diffTime * 60.0f / diffPowerDbLow;
				}
				
				const float diffPowerDbMedium = -10.0f * log10f( lastImpinge.medium / firstImpinge->medium );
				if( diffPowerDbMedium > 1e-4f ){
					pDecayTimeSumMedium += diffTime * 60.0f / diffPowerDbMedium;
				}
				
				const float diffPowerDbHigh = -10.0f * log10f( lastImpinge.high / firstImpinge->high );
				if( diffPowerDbHigh > 1e-4f ){
					pDecayTimeSumHigh += diffTime * 60.0f / diffPowerDbHigh;
				}
				
				pDecayTimeCount++;
			}
		}
		#endif
	}
	
	const int transmittedRayCount = soundRay.GetTransmittedRayCount();
	const int firstTransmittedRay = soundRay.GetFirstTransmittedRay();
	
	for( i=0; i<transmittedRayCount; i++ ){
		RunSphereReceiver( params, params.soundRayList
			.GetTransmittedRayAt( firstTransmittedRay + i ), firstImpinge );
	}
}



#if 0

void deoalRTPTListen::RunGaussBeam(){
	// gaussA = 1.0 / sqrt( 2.0 * pi)
	#define RAY_GAUSS_A 0.398942f
	
	// gaussB = -0.5
	#define RAY_GAUSS_B -0.5f
	
	// gauss beam width factor
	#define RAY_GAUSS_WIDTH_SIGMA ( 1.0f / 2.35482f )
	#define RAY_GAUSS_WIDTH_INV_SIGMA 2.35482f
	
	// gaussBorder = exp(b * 2.35482 * 2.35482)
	// gaussNorm(d) = gauss(d) * 1/(1-gaussBorder) + (-gaussBorder/(1-gaussBorder))
	#define RAY_GAUSS_NORM_SCALE 1.0666667f
	#define RAY_GAUSS_NORM_OFFSET -0.0666667f
	
	
	
	const deoalSoundRayList &soundRayList = pListenProbe ?
		pListenProbe->GetSoundRayList() : pSourceProbe->GetSoundRayList();
	const decDVector &probePosition = pSourceProbe->GetPosition();
	const decVector listenerPosition( pListenerPosition - probePosition );
// 	const float directDistance = listenerPosition.Length();
	deoalWorldOctree &octree = *pWorld->GetOctree();
	const int lastRay = pFirstRay + pRayCount;
	int i, j;
	
	const float gaussWidthFactor = tanf( pListenProbe ?
		pListenProbe->GetRayOpeningAngle() : pSourceProbe->GetRayOpeningAngle() );
	
// 	const float gaussInvSigma = RAY_GAUSS_WIDTH_INV_SIGMA / gaussWidthFactor;
// 	const float gaussInvSigmaSquared = gaussInvSigma * gaussInvSigma;
// 	const float gaussNorm = RAY_GAUSS_A * gaussInvSigma;
	
	const float beamRadiusEnlarge = 1.0f;
	const float beamRadiusSquaredEnlarge = beamRadiusEnlarge * beamRadiusEnlarge;
	
	/* 
	convergence testing (late-reverberation gain)
	
	dynamic radius, openingAngle*1*distance, increase beamRadiusEnlarge
	1:  0.278, 0.107
	2:  0.158, 0.019
	4:  0.125, 0.006
	8:  0.118, 0.003
	16: 0.118, 0.002
	
	dynamic radius, openingAngle*2*distance, increase beamRadiusEnlarge
	1:  0.345, 0.173
	2:  0.269, 0.060
	4:  0.246, 0.033
	8:  0.246, 0.026
	16: 0.246, 0.025

	static radius, openingAngle*1m, increase beamRadiusEnlarge
	=> beamWidth low/high ray count: 0.368m, 0.091m
	1:  0.037, 0.007
	2:  0.014, 0.005
	4:  0.000, 0.003
	8:  0.000, 0.001
	16: 0.000, 0.000

	static radius, openingAngle*10m, increase beamRadiusEnlarge
	=> beamWidth low/high ray count: 3.68m, 0.91m
	1:  0.091, 0.085
	2:  0.024, 0.003
	4:  0.018, 0.000
	8:  0.018, 0.000
	16: 0.018, 0.000

	static radius, openingAngle*20m, increase beamRadiusEnlarge
	=> beamWidth low/high ray count: 7.36m, 1.82m
	1:  0.116, 0.060
	2:  0.082, 0.005
	4:  0.082, 0.002
	8:  0.082, 0.001
	16: 0.082, 0.001
	
	static radius, 10.0m, increase beamRadiusEnlarge
	1:  0.254, 0.245
	2:  0.254, 0.245
	4:  0.254, 0.245
	8:  0.254, 0.245
	16: 0.254, 0.245
	
	static radius, 5.0m, increase beamRadiusEnlarge
	1:  0.100, 0.093
	2:  0.063, 0.054
	4:  0.063, 0.054
	8:  0.063, 0.054
	16: 0.063, 0.054
	
	static radius, 20.0m, increase beamRadiusEnlarge
	1:  0.460, 0.452
	
	the paper uses beamWidth=23.5m
	*/
	
	for( i=pFirstRay; i<lastRay; i++ ){
		const deoalSoundRay &soundRay = soundRayList.GetRayAt( i );
		const int segmentCount = soundRay.GetSegmentCount();
		const int firstSegment = soundRay.GetFirstSegment();
		
		float closestListenerDistance = 0.0f;
		float closestWeight = 0.0f;
		int closestSegment = -1;
		
		for( j=0; j<segmentCount; j++ ){
			const deoalSoundRaySegment &segment = soundRayList.GetSegmentAt( firstSegment + j );
			
			// check if the ray hits the listener along the way
			// 
			// gaussien distribution: g(x) = a*exp(b*x*x)
			// with: a = 1/(sigma*sqrt(2*pi)) = (1/sqrt(2*pi)) * (1/sigma)
			// with: b = -1/(2*sigma*sigma) = -0.5/(sigma*sigma) = -0.5 * (1/sigma) * (1/sigma)
			// width (lowest to highest) at 50% of peek value: w = 2*sigma*sqrt(2*ln(2)) = 2.35482*sigma
			// widht at 10% of peek value: w = 2*sigma*sqrt(2*ln(10)) = 4.29193*sigma
			// transform to standard(unit)-gaussian: sigma2 = x / sigma
			// 
			// beam width from ray opening angle as w ~= tan(angle) * distance * 2
			// thus: sigma = w / 2.35482 = tan(angle) * distance * 2 / 2.35482
			//             = tan(angle) * distance * 0.849
			// 
			// 42-rays:
			//   const float radius = 0.343615;
			//   const float angle = 0.330975; // 18.96 deg
			//   const float tanOpenAngle = 0.779234; // 37.93 deg
			// 
			// 162-rays:
			//   const float radius = 0.170324;
			//   const float angle = 0.168705; // 9.66 deg
			//   const float tanOpenAngle = 0.350825; // 19.33 deg
			// 
			// 642-rays:
			//   const float radius = 0.085891;
			//   const float angle = 0.085680; // 4.91 deg
			//   const float tanOpenAngle = 0.173057; // 9.82 deg
			// 
			// now there is one problem. the gaus curve sums up to 1 which causes the center value
			// to not be 1 but a lot less especially with increasing beam width. this would be
			// correct forsimulating the spread. we need though to deliver the full intensity at
			// the center if the ray hits the listener at 0 angle. to achieve this we have to
			// scale the gaus value so the center value is 1. we can also scale the border values
			// to be 0 but that's not important. important is raising the middle value to 1.
			// to achieve this the (a/sigma) is dropped and the result is right
			// 
			// to compensate also for the border values not being 0 the following is required.
			//   g(beamWidth) = exp((b/sigmaSquared)*beamWidthSquared)
			//                = exp(b*pow(beamWidth*2.35482/beamWidth,2)
			//                = exp(b*pow(2.35482,2))
			// 
			// thus the border value is independent of the beam width:
			//   gaussBorder = exp(b * 2.35482 * 2.35482)
			// 
			// so the following scaling can be applied:
			//   gaussNorm(d) = (gauss(d) - gaussBorder) / (1 - gaussBorder)
			// 
			// or using MAD
			//   gaussNorm(d) = gauss(d) * 1/(1-gaussBorder) + (-gaussBorder/(1-gaussBorder))
			// 
			// NOTE
			// the problem of the listener sphere does not apply to gauss beams. it is thus not
			// required to correc the 1/r^2
			// 
			// 
			// NOTE
			// according to better paper this is the calculation to be used:
			// - beamWidth = tan(openingAngle) * distanceAlongRay
			// - sigma = beamWidth * beamWidthScale / 2.35482
			//   the beamWidthScale should be larger than 1. at 1 the beam border is 1/2 weight.
			//   by using larger values the beamns beams overlap producing smoother results.
			//   for example using a value of 2 is the overlap scenario
			// - gaussB = -0.5 / (sigma^2)
			//   gaussB = -0.5 / (beamWidth^2 * beamWidthScale^2 / 2.35482^2)
			//   gaussB = -0.5 * 2.35482^2 / (beamWidth^2 * beamWidthScale^2)
			//   gaussB = gaussB2 / (beamWidth^2 * beamWidthScale^2)
			//   => gaussB2 = -0.5 * 2.35482^2
			//   gaussB = gaussB2 / (tan(openingAngle)^2 * distanceAlongRay^2 * beamWidthScale^2)
			// 
			// the contribution at closest distance d from ray to listener:
			//   contr(d) = exp( gaussB * d * d ) * ( gain / rayCount )
			// 
			// with one level increase the quantities change like this:
			// - rayCount' = rayCount * 4   // roughly
			// - openingAngle' = openingAngle / 2
			// => openingAngle'^2 = openingAngle^2 / 4
			// => beamWidth'^2 = beamWidth^2 / 4
			// 
			// thus the ray count reduces the result by factor 4 while the openingAngle
			// increases the result by a cocomplicated factor.
			// 
			// no distance attenuation is used. thus the range of the sound source has no
			// influence on the calculation other than reducing the chance ray to get close
			// enough to the listener.
			// 
			// the paper uses these termination conditions:
			// - number of bounces exceeds limit
			// - gain drops below threshold
			// 
			// we can add also the condition to stop if the ray travelled to the range of the
			// sound source. usually though the bounce count kicks in earlier unless the artist
			// chose a shorter range
			// 
			// since there is no distance attenuation the entire path has to be checked to find
			// the closest distance to the listener which is not obstructed. to optimize this
			// calculation the segment is skipped (especially the expensive is-blocked check)
			// if a previous segment has been found which is not blocked and is closer to the
			// ray than the current segment. the idea is that if an upcoming segment is farther
			// away from the ray then it can never be a better choice and thus the expensive
			// is-blocked check can be avoided
			// 
			// the beamWidth is fixed in the paper albeit adjusted to the ray density. here we
			// adjust the size with the travelled distance. maybe this makes a difference.
			// 
			
			const decVector rayListenerDirection( listenerPosition - segment.GetPosition() );
			const float lambda = segment.GetDirection() * rayListenerDirection;
			if( lambda < 0.0 || lambda > segment.GetLength() ){
				continue;
			}
			
			const decVector closest( segment.GetPosition() + segment.GetDirection() * lambda );
			const decVector listenerDirection( closest - listenerPosition );
			
			
			// if the ray is outside the contribution radius ignore it
			const float listenerDistance = segment.GetDistance() + lambda;
			const float distSquared = listenerDirection.LengthSquared();
			
			
			const float beamWidth = gaussWidthFactor * listenerDistance;
			const float beamWidthSquared = beamWidth * beamWidth;
			if( distSquared > beamWidthSquared * beamRadiusSquaredEnlarge ){
				continue;
			}
			
			const float invSigma = RAY_GAUSS_WIDTH_INV_SIGMA / beamWidth;
			const float factor = /* ( RAY_GAUSS_A * invSigma ) * */
				expf( ( RAY_GAUSS_B * invSigma * invSigma ) * distSquared );
				// * RAY_GAUSS_NORM_SCALE + RAY_GAUSS_NORM_OFFSET;
			
			// if the segment does not have a better weight ignore it
			if( closestSegment != -1 && factor >= closestWeight ){
				continue;
			}
			
			// this segment is better. if is also not blocked store it as best segment
			if( IsRayBlocked( octree, pListenerPosition, listenerDirection ) ){
				continue; // no direct path to the listener
			}
			
			closestSegment = j;
			closestWeight = factor;
			closestListenerDistance = listenerDistance;
			
#if 0
			// physically no indirect path can be shorter than the direct path. but due to the
			// way gaus-beams are handled it is possible for rays to have a slightly shorter
			// distance than the direct distance. to avoid this problem we consider rays with
			// listener distance less than or equal to the direct distance as a direct contribution
			// and ignore them
// 			if( soundRay.GetBounces() == 1 && listenerDistance > directDistance ){
// 				continue;
// 			}
			
			// check if the ray is blocked
			if( IsRayBlocked( octree, pListenerPosition, listenerDirection ) ){
				continue; // no direct path to the listener
			}
			
// 			const float factor = gaussNorm * expf( RAY_GAUSS_B * gaussInvSigmaSquared
// 				* distSquared / ( listenerDistance * listenerDistance ) );
			
// 			const float attGain = factor * pSourceProbe->AttenuatedGain( listenerDistance );
			const float gainLow = segment.GetGainLow(); // * attGain;
			const float gainMedium = segment.GetGainMedium(); // * attGain;
			const float gainHigh = segment.GetGainHigh(); // * attGain;
			
			ApplyIndirectGains( j + 1, -rayListenerDirection, listenerDistance,
				gainLow, gainMedium, gainHigh );
			
			break; // ray has hit listener. stop processing the rest of the ray
#endif
		}
		
		// if there is a closest segment apply the gain
		if( closestSegment == -1 ){
			continue;
		}
		
		const float factor = closestWeight; // * pSourceProbe->AttenuatedGain( closestListenerDistance );
		
		const deoalSoundRaySegment &segment = soundRayList.GetSegmentAt( firstSegment + closestSegment );
		const float gainLow = segment.GetGainLow() * factor;
		const float gainMedium = segment.GetGainMedium() * factor;
		const float gainHigh = segment.GetGainHigh() * factor;
		
		ApplyIndirectGains( closestSegment + 1, segment.GetPosition() - listenerPosition,
			closestListenerDistance, gainLow, gainMedium, gainHigh );
	}
	
	
	
	#undef RAY_GAUSS_A
	#undef RAY_GAUSS_B
	#undef RAY_GAUSS_WIDTH_SIGMA
	#undef RAY_GAUSS_WIDTH_INV_SIGMA
	#undef RAY_GAUSS_NORM_SCALE
	#undef RAY_GAUSS_NORM_OFFSET
}



void deoalRTPTListen::RunListenerSphere(){
// 	#define HIT_COUNTS_ALL
	#undef HIT_COUNTS_ALL
	
	const deoalSoundRayList &soundRayList = pListenProbe ?
		pListenProbe->GetSoundRayList() : pSourceProbe->GetSoundRayList();
	const decDVector &probePosition = pSourceProbe->GetPosition();
	const decVector listenerPosition( pListenerPosition - probePosition );
	deoalWorldOctree &octree = *pWorld->GetOctree();
	const int lastRay = pFirstRay + pRayCount;
	int i, j;
	
#if 0
	const float lsRadius = /* powf( 15.0f * pEnvProbe->GetRoomVolume()
		/ ( 2.0f * PI * ( float )pEnvProbe->GetRayCount() ), 1.0f / 3.0f ); //1.225f; */ 1.225;
	const float lsFactor1 = lsRadius * lsRadius;
	const float lsFactor2 = 3.0f / ( 2.0f * PI * lsRadius * lsRadius * lsRadius );
#endif
	
	// base contrib radius is the ray opening angle. two ray base circles touch each other.
	// this leaves an unaccounted for area near the corner of squares fitting around the
	// circles. to reach out to the corner (and accept some overlap at the axis poles) the
	// radius has to be enlarged by sqrt(2).
	const float baseContribRadius = tanf(
		( pListenProbe ? pListenProbe->GetRayOpeningAngle() : pSourceProbe->GetRayOpeningAngle() )
			* 0.5f ) * sqrtf( 2.0f );
	
	for( i=pFirstRay; i<lastRay; i++ ){
		const deoalSoundRay &soundRay = soundRayList.GetRayAt( i );
		const int segmentCount = soundRay.GetSegmentCount();
		const int firstSegment = soundRay.GetFirstSegment();
		
		for( j=0; j<segmentCount; j++ ){
			const deoalSoundRaySegment &segment = soundRayList.GetSegmentAt( firstSegment + j );
			
			// NOTE
			// about distance calculation using sphere receiver. based on ray-tracing paper.
			// this paper does not mention gauss beams but instead calculates the receiver
			// sphere radius either using length of ray or room volume. the ray contributes
			// his energy if the sphere is hit.
			// 
			// r(lm,N) = lm * pow(2*pi/N, 0.5)   // lm = maximum ray distance
			//    => r(5, 162) = 0.985
			//    => r(5, 642) = 0.495 (factor 0.5)
			//    => r(5, 2562) = 0.248 (factor 0.25)
			//    with distance*factor result is *factor
			// 
			// r(V,N) = pow(15*V/(2*pi*N), 1/3)   // V = room volume
			//    => r(124.8, 162) = 1.225
			//    => r(124.8, 642) = 0.774
			//    => r(124.8, 2562) = 0.488
			// 
			// the volume version is independent of the distance the ray travelled while the
			// distance version might better represent small rooms. that said changing the
			// receiver size with the distance could be a problem. the radius is though quite
			// large.
			// 
			// the intensity applied to the sphere is
			//   I(i) = W(i) * dts(i) / V_sphere(r)    // W(i) = W_total / N
			// with V_sphere the volume of the sphere
			//   V_sphere(r) = 4/3 * pi * r*r*r
			// 
			// T is the time the ray spend crossing the sphere. for rays hitting spot on this
			// time is larger than for rays just grazing the sphere.
			//   dts(r,d) = 2 * sqrt(r*r - d*d)  // d = closest distance sphere center to ray
			// 
			// combined:
			//   I(i) = W(i) * 2 * sqrt(r*r - d*d) * 3 / (4*pi * r*r*r)
			//        = W(i) * sqrt(r*r - d*d) * 3 / (2*pi * r*r*r)
			// 
			// this could compensate for large receiver spheres
			// 
			// 
			// 
			// NOTE
			// sabine sound pressure at position is:
			// spl(d) = Lw + 10*log(1/(4*pi*d*d) + 4*(1-avgAbs)/sabine)
			// 
			// we are interested in the indirect gain which can be calculated like this:
			//  a = 10*log(1/(4*pi*d*d) + 4*(1-avgAbs)/sabine)
			//  b = 10*log(1/(4*pi*d*d))  // direct path only, avgAbs=1
			//  gain = pow(2,a/6) - pow(2,b/6)
			// 
			// for an average room at 3m distance this is 0.63
			// 
			// 
			// 
			// NOTE
			// experiments with listener radius calculated relative to ray distance from source
			// 
			// > listeneRadius relative to ray distance from source: tan(openingAngle / 2)*distance
			// N rays)    factor = 0.082
			// N*4 rays)  factor = 0.094
			// N*16 rays) factor = 0.101
			// 
			// > listeneRadius relative to ray distance from source: tan(openingAngle)*distance
			// N rays)    factor = 0.051
			// N*4 rays)  factor = 0.084
			// N*16 rays) factor = 0.099
			// 
			// > listeneRadius relative to ray distance from source: tan(openingAngle / 4)*distance
			// N rays)    factor = 0.140
			// N*4 rays)  factor = 0.127
			// N*16 rays) factor = 0.112
			// 
			// > listeneRadius relative to ray distance from source: tan(openingAngle * 0.4)*distance
			// N rays)    factor = 0.101
			// N*4 rays)  factor = 0.107
			// N*16 rays) factor = 0.100
			// 
// 			pOwner.GetAudioThread().GetLogger().LogInfoFormat( "FIRST lp=(%f,%f,%f) srp=(%f,%f,%f) srd=(%f,%f,%f) l=%f srl=%f",
// 				listenerPosition.x, listenerPosition.y, listenerPosition.z,
// 				soundRay.GetPosition().x, soundRay.GetPosition().y, soundRay.GetPosition().z,
// 				soundRay.GetDirection().x, soundRay.GetDirection().y, soundRay.GetDirection().z,
// 				soundRay.GetDirection() * ( listenerPosition - soundRay.GetPosition() ), soundRay.GetLength() );
			
			const decVector rayListenerDirection( listenerPosition - segment.GetPosition() );
			const float lambda = segment.GetDirection() * rayListenerDirection;
			if( lambda < 0.0 || lambda > segment.GetLength() ){
				continue;
			}
			
			const decVector closest( segment.GetPosition() + segment.GetDirection() * lambda );
			const decVector listenerDirection( closest - listenerPosition );
			
			
			// if the ray is outside the contribution radius ignore it
			const float listenerDistance = segment.GetDistance() + lambda;
			const float distSquared = listenerDirection.LengthSquared();
			
			const float contribRadius = listenerDistance * baseContribRadius;
			const float contribRadiusSquared = contribRadius * contribRadius;
// 			pOwner.GetAudioThread().GetLogger().LogInfoFormat( "CHECK lp=(%f,%f,%f) srp=(%f,%f,%f) srd=(%f,%f,%f) c=(%f,%f,%f) "
// 				"srd=%f l=%f ld=%f ds=%f bcr=%f cr=%f crs=%f att=%f",
// 				listenerPosition.x, listenerPosition.y, listenerPosition.z,
// 				soundRay.GetPosition().x, soundRay.GetPosition().y, soundRay.GetPosition().z,
// 				soundRay.GetDirection().x, soundRay.GetDirection().y, soundRay.GetDirection().z,
// 				closest.x, closest.y, closest.z, soundRay.GetDistance(), lambda,
// 				listenerDistance, distSquared, baseContribRadius, contribRadius, contribRadiusSquared,
// 				1.0f / decMath::max( 1.0f, listenerDistance ) );
			if( distSquared > contribRadiusSquared ){
				continue;
			}
			
			
			// check if the ray is blocked. the ray is considered blocked if the path between
			// the closes ray point and the listener is blocked
			if( IsRayBlocked( octree, pListenerPosition, listenerDirection ) ){
				continue; // no direct path to the listener
			}
			
			
			// path is free so calculate the contribution to the listener. for this we use
			// the dot product between the ray towards the listener and the ray direction
			// itself
// 			const float factor = 1.0f;
			
			// NOTE
			// if the surface of the receiver sphere is constant then this is already taking care
			// of the distance divergence (1/r^2). in this case doing distance attenuation would
			// duplicate the attenuation causing ray energy impinged on the receiver (distance
			// travelled through the receiver) to be much smaller than it should. but with small
			// ray count a small receiver sphere is a problem. we need to be able to calculate
			// the attenuation ourself (artistic freedom) while using a receiver sphere with
			// dynamic radius (adjusted to compensate for beam spreading).
			// 
			// NOTE
			// possible way to calculate it. the beam approach is similar to an attenuation of
			// 1/d*d . the artistic approach uses rd/(rd+ro*(d-rd)). the scaling factor to apply
			// would be then:
			//   f = rd/(rd+ro*(d-rd)) / 1/d*d
			//   f = rd/(rd+ro*(d-rd)) * d*d
			//   f = AttenuatedGain(d) * d*d
			// 
			// the only requirement to fix the problem is to apply listenerDistance-squared to
			// AttenuatedGain() to compensate.
			// 
			const float attGain = pSourceProbe->AttenuatedGain( listenerDistance );
			
			#ifdef HIT_COUNTS_ALL
				const float impingeGain = attGain;
				
			#else
				const float lsFactor2 = 3.0f / ( 2.0f * PI * contribRadiusSquared * contribRadius );
				const float impingeFactor = lsFactor2 * sqrtf( contribRadiusSquared - distSquared );
				const float impingeGain = impingeFactor * ( attGain * listenerDistance * listenerDistance );
			#endif
			
			const float gainLow = segment.GetGainLow() * impingeGain;
			const float gainMedium = segment.GetGainMedium() * impingeGain;
			const float gainHigh = segment.GetGainHigh() * impingeGain;
			
			ApplyIndirectGains( j + 1, -rayListenerDirection, listenerDistance,
				gainLow, gainMedium, gainHigh );
			
			break; // ray has hit listener. stop processing the rest of the ray
		}
	}
	
	#undef HIT_COUNTS_ALL
}



void deoalRTPTListen::RunHuygens(){
	const deoalSoundRayList &soundRayList = pListenProbe ?
		pListenProbe->GetSoundRayList() : pSourceProbe->GetSoundRayList();
	const decDVector &probePosition = pSourceProbe->GetPosition();
	const decVector listenerPosition( pListenerPosition - probePosition );
	deoalWorldOctree &octree = *pWorld->GetOctree();
	const int lastRay = pFirstRay + pRayCount;
	int i, j;
	
	for( i=pFirstRay; i<lastRay; i++ ){
		const deoalSoundRay &soundRay = soundRayList.GetRayAt( i );
		const int segmentCount = soundRay.GetSegmentCount();
		const int firstSegment = soundRay.GetFirstSegment();
		
		for( j=0; j<segmentCount; j++ ){
			const deoalSoundRaySegment &segment = soundRayList.GetSegmentAt( firstSegment + j );
			
			const decVector listenerRayDirection( segment.GetPosition() - pListenerPosition );
			const float listenerDirectionLength = listenerRayDirection.Length();
			const float distance = segment.GetDistance() + listenerDirectionLength;
			
			const float attGain = pSourceProbe->AttenuatedGain( distance );
			const float gainLow = segment.GetGainLow() * attGain;
			const float gainMedium = segment.GetGainMedium() * attGain;
			const float gainHigh = segment.GetGainHigh() * attGain;
			
			if( listenerDirectionLength < 0.01f ){
				ApplyIndirectGains( j + 1, listenerRayDirection, distance, gainLow, gainMedium, gainHigh );
				continue;
			}
			
			if( IsRayBlocked( octree, pListenerPosition, listenerRayDirection ) ){
				continue;
			}
			
			ApplyIndirectGains( j + 1, listenerRayDirection, distance, gainLow, gainMedium, gainHigh );
			
			break; // ray has hit listener. stop processing the rest of the ray
		}
	}
}

#endif



bool deoalRTPTListen::IsRayBlocked( deoalWorldOctree& octree,
const decDVector &position, const decDVector &direction ){
	#if 1
	if( pRTWorldBVH ){
		pRTWOVRayBlocked.SetRay( *pRTWorldBVH, position, direction );
		pRTWOVRayBlocked.SetBlocked( false );
		pRTWOVRayBlocked.VisitBVH( *pRTWorldBVH );
		return pRTWOVRayBlocked.GetBlocked();
		
	}else{
		pWOVRayBlocked.SetRay( position, direction );
		pWOVRayBlocked.SetBlocked( false );
		pWOVRayBlocked.VisitNode( octree );
		return pWOVRayBlocked.GetBlocked();
	}
	
	#else
	pRTResult.Clear();
	
	if( pRTWorldOctree ){
		pRTWOVRayHitsElement.SetRay( *pRTWorldOctree, position, direction );
		pRTWOVRayHitsElement.VisitOctree( *pRTWorldOctree );
		
	}else{
		pWOVRayHitsElement.SetRay( position, direction );
		pWOVRayHitsElement.VisitNode( octree );
	}
	
	const int hitCount = pRTResult.GetElementCount();
	int i;
	
	for( i=0; i<hitCount; i++ ){
		const deoalRayTraceHitElement &he = pRTResult.GetElementAt( i );
		if( he.GetForwardFacing() ){
			return true;
		}
	}
	#endif
	
	return false;
}

void deoalRTPTListen::ApplyIndirectGains( int bounces, const decVector &direction,
float distance, float gainLow, float gainMedium, float gainHigh ){
	// determine if this is a first reflection or a late reflection. counts as first reflection
	// if either this is the first time the ray bounces or the distance is less than the
	// transition separation distance
	const float maxGain = decMath::max( gainLow, gainMedium, gainHigh );
	
	if( bounces > 1 && distance > pSepDistFirstLateRefl ){
	//if( bounces > 2 ){
		// late reflections parameters.
		pLRGainLow += gainLow;
		pLRGainMedium += gainMedium;
		pLRGainHigh += gainHigh;
		
		const float weight = maxGain;
		if( ! direction.IsZero() ){
			pLRPanDirection += direction.Normalized() * weight;
		}
		pLRPanDirectionWeightSum += weight;
		
		pLRCount++;
		
	}else{
		// first-reflection parameters 
		if( distance < pFRMinDistance ){
			pFRMinDistance = distance;
		}
		if( distance > pFRMaxDistance ){
			pFRMaxDistance = distance;
		}
		pFRSumDistance += distance;
		
		pFRGainLow += gainLow;
		pFRGainMedium += gainMedium;
		pFRGainHigh += gainHigh;
		
		const float weight = maxGain;
		if( ! direction.IsZero() ){
			pFRPanDirection += direction.Normalized() * weight;
		}
		pFRPanDirectionWeightSum += weight;
		
		pFRCount++;
	}
	
	if( ! pHasHighestGain || maxGain > pHighestGainGain ){
		pHasHighestGain = true;
		pHighestGainGain = maxGain;
		pHighestGainDistance = distance;
	}
	
	pImpulseResponse.Add( distance * INV_SOUND_SPEED, gainLow, gainMedium, gainHigh );
}
