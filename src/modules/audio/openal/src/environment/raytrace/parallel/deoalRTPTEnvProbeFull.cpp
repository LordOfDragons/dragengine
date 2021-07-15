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
#include "deoalRTPTEnvProbeFull.h"
#include "../deoalRayTraceHitElement.h"
#include "../deoalSoundRayInteraction.h"
#include "../deoalRayTraceConfig.h"
#include "../../deoalEnvProbe.h"
#include "../../../deAudioOpenAL.h"
#include "../../../audiothread/deoalAudioThread.h"
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

deoalRTPTEnvProbeFull::deoalRTPTEnvProbeFull( deoalRTParallelEnvProbe &owner ) :
deParallelTask( &owner.GetAudioThread().GetOal() ),
pOwner( owner ),
pWorld( NULL ),
pEnvProbe( NULL ),
pProbeConfig( NULL ),
pFirstRay( 0 ),
pRayCount( 0 ),
pBackStepDistance( 1e-4 )
{
	(void)pOwner; // silence compiler warning
	
	pWOVRayHitsElement.SetResult( &pRTResult );
	SetMarkFinishedAfterRun( true );
	SetLowPriority( true );
}

deoalRTPTEnvProbeFull::~deoalRTPTEnvProbeFull(){
}



// Manegement
///////////////

void deoalRTPTEnvProbeFull::SetWorld( deoalAWorld *world ){
	pWorld = world;
}

void deoalRTPTEnvProbeFull::SetEnvProbe( const deoalEnvProbe *envProbe ){
	pEnvProbe = envProbe;
}

void deoalRTPTEnvProbeFull::SetProbeConfig( const deoalRayTraceConfig *probeConfig ){
	pProbeConfig = probeConfig;
}

void deoalRTPTEnvProbeFull::SetFirstRay( int firstRay ){
	pFirstRay = firstRay;
}

void deoalRTPTEnvProbeFull::SetRayCount( int rayCount ){
	pRayCount = rayCount;
}

void deoalRTPTEnvProbeFull::SetListenerPosition( const decDVector &position ){
	pListenerPosition = position;
}

void deoalRTPTEnvProbeFull::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}



#ifdef THAN_DEBUG
decString thanTemp;
#endif

#define BOUNCE_HACK1
#define BOUNCE_HACK2

// #define BOUNCE_HACK1 *2.0
// #define BOUNCE_HACK2 *0.2

// gaussA = 1.0 / sqrt( 2.0 * pi)
#define RAY_GAUSS_A 0.3989422804014327

// gaussB = -0.5
#define RAY_GAUSS_B -0.5

// gauss beam width factor
#define RAY_GAUSS_WIDTH_SIGMA ( 1.0 / 2.35482 )
#define RAY_GAUSS_WIDTH_INV_SIGMA 2.35482


void deoalRTPTEnvProbeFull::Run(){
	const decVector * const rayDirections = pProbeConfig->GetRayDirections() + pFirstRay;
	
	pWOVRayHitsElement.SetLayerMask( pLayerMask );
	
	pGainLow = 0.0f;
	pGainMedium = 0.0f;
	pGainHigh = 0.0f;
	pDelay = 0.0f;
	pFirstHit = true;
	
	sTraceRay ray;
	ray.position = pEnvProbe->GetPosition();
	ray.distance = 0.0f;
	ray.remainingDistance = pEnvProbe->GetRange() BOUNCE_HACK1;
	ray.bounce = 0;
	
	sTraceGain gain;
	gain.low = 1.0f;
	gain.medium = 1.0f;
	gain.high = 1.0f;
	
	pRayGaussWidthFactor = tan( pProbeConfig->GetOpeningAngle() ) * 2.0;
	
	for( pRayIndex=0; pRayIndex<pRayCount; pRayIndex++ ){
		ray.direction = rayDirections[ pRayIndex ];
		
		#ifdef THAN_DEBUG
			pOwner.GetAudioThread().GetLogger().LogInfoFormat( "RAY (%d,%.3f) (%.3f,%.3f,%.3f)", pRayIndex,
				ray.remainingDistance, ray.direction.x, ray.direction.y, ray.direction.z );
			thanTemp.Format( "PATH (%.3f,%.3f,%.3f)", ray.position.x, ray.position.y, ray.position.z );
		#endif
		pTraceRay( ray, gain );
		#ifdef THAN_DEBUG
			pOwner.GetAudioThread().LogInfo( thanTemp );
		#endif
// 		pOwner.GetAudioThread().GetLogger().LogInfoFormat( "(%p %d) Total Bounces: %d", this, pRayIndex, pBounce );
	}
}

void deoalRTPTEnvProbeFull::Finished(){
}



// Debugging
//////////////

decString deoalRTPTEnvProbeFull::GetDebugName() const{
	return "OpenAL-RTPTEnvProbeFull";
}

decString deoalRTPTEnvProbeFull::GetDebugDetails() const{
	return "";
}



// Protected Functions
////////////////////////

void deoalRTPTEnvProbeFull::pTraceRay( const sTraceRay &ray, const sTraceGain &gain ){
	const decDVector scaledDirection( ray.direction * ray.remainingDistance );
	
	pRTResult.Clear();
	pWOVRayHitsElement.SetRay( ray.position, scaledDirection );
	pWOVRayHitsElement.VisitNode( *pWorld->GetOctree() );
	
	// find result. use the first forward facing element. backward facing elements are
	// ignored because due to snapping the probe position can end up inside a component.
	// without skipping backwards facing faces this can result in incorrect probe data
	const deoalRayTraceHitElement *hitElement = NULL;
	const int hitCount = pRTResult.GetElementCount();
	int hitIndex;
	
	for( hitIndex=0; hitIndex<hitCount; hitIndex++ ){
		const deoalRayTraceHitElement &he = pRTResult.GetElementAt( hitIndex );
		if( he.GetForwardFacing() ){
			hitElement = &he;
			break;
		}
	}
	
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
	const double beamWidth = pRayGaussWidthFactor * ray.distance;
	const double beamWidthSquared = beamWidth * beamWidth;
	
	const double closestOnRayLambda = ray.direction * ( pListenerPosition - ray.position );
	const double closestOnRayDistance = hitElement ? hitElement->GetDistance() : ray.remainingDistance;
	if( closestOnRayLambda > 0.0 && closestOnRayLambda < closestOnRayDistance ){
		const decDVector closestOnRay( ray.position + ray.direction * closestOnRayLambda );
		const double distSquared = ( closestOnRay - pListenerPosition ).LengthSquared();
		if( distSquared < beamWidthSquared ){
			const double invSigma = RAY_GAUSS_WIDTH_INV_SIGMA / beamWidth;
			const double factor = ( RAY_GAUSS_A * invSigma ) * exp( ( RAY_GAUSS_B * invSigma * invSigma ) * distSquared );
			pGainLow += gain.low * factor;
			pGainMedium += gain.medium * factor;
			pGainHigh += gain.high * factor;
			#ifdef THAN_DEBUG
			thanTemp.Append( " [HIT]" );
			#endif
			/*
			pOwner.GetAudioThread().GetLogger().LogInfoFormat( "(%p %d %d) Affect: dist=%3f rem=%.3f gain=(%.3f,%.3f,%.3f) gauss=%.3f",
				this, pRayIndex, ray.bounce, ray.distance, ray.remainingDistance,
				gain.low * factor, gain.medium * factor, gain.high * factor, factor );
			*/
		}
	}
	
	// apply result
	if( ! hitElement ){
		// ray hits nothing and ends
		#ifdef THAN_DEBUG
		thanTemp.AppendFormat( " (%.3f,%.3f,%.3f)", (ray.position + scaledDirection).x,
			(ray.position + scaledDirection).y, (ray.position + scaledDirection).z );
		#endif
		pUpdateExtends( ray.position + scaledDirection );
		return;
	}
	
	// ray hits something
	const float hitDistance = hitElement->GetDistance();
	const decDVector &hitPoint = hitElement->GetPoint();
	pUpdateExtends( hitPoint );
	
	// find thickness for transmission calculation
	const deoalRayTraceHitElement *hitElementBack = NULL;
	for( hitIndex++; hitIndex<hitCount; hitIndex++ ){
		const deoalRayTraceHitElement &he = pRTResult.GetElementAt( hitIndex );
		if( ! he.GetForwardFacing() ){
			hitElementBack = &he;
			break;
		}
	}
	
	// calculate the gain and stop if ray becomes inaudible
	//pRTResult.DebugPrint( oal, "hitElement" );
	deoalAComponent &component = *hitElement->GetComponent();
	const deoalModelFace &face = component.GetModel()->GetFaceAt( hitElement->GetComponentFace() );
	const deoalAComponentTexture &texture = component.GetModelTextureAt( face.GetTexture() );
	const decDVector &hitNormal = hitElement->GetNormal();
	
	const float absorbedLow = gain.low * texture.GetAbsorptionLow();
	const float absorbedMedium = gain.medium * texture.GetAbsorptionMedium();
	const float absorbedHigh = gain.high * texture.GetAbsorptionHigh();
	
	const float nonAbsLow = gain.low - absorbedLow;
	const float nonAbsMedium = gain.medium - absorbedMedium;
	const float nonAbsHigh = gain.high - absorbedHigh;
	
	float transmitLow = 0.0f;
	float transmitMedium = 0.0f;
	float transmitHigh = 0.0f;
	
	if( hitElementBack ){
		const float thickness = hitElementBack->GetDistance() - hitElement->GetDistance();
		transmitLow = decMath::linearStep( thickness, 0.0f,
			texture.GetTransmissionLow(), nonAbsLow, 0.0f );
		transmitMedium = decMath::linearStep( thickness, 0.0f,
			texture.GetTransmissionMedium(), nonAbsMedium, 0.0f );
		transmitHigh = decMath::linearStep( thickness, 0.0f,
			texture.GetTransmissionHigh(), nonAbsHigh, 0.0f );
	}
	
	sTraceRay reflectedRay;
	reflectedRay.distance = ray.distance + hitDistance;
	reflectedRay.remainingDistance = ray.remainingDistance - hitDistance;
	
	const double dotOut = -( hitNormal * ray.direction );
	const float attGain = pEnvProbe->AttenuatedGain( reflectedRay.distance BOUNCE_HACK2 ); // * ( float )dotOut;
	
	sTraceGain reflectedGain;
	reflectedGain.low = ( nonAbsLow - transmitLow ) * attGain;
	reflectedGain.medium = ( nonAbsMedium - transmitMedium ) * attGain;
	reflectedGain.high = ( nonAbsHigh - transmitHigh ) * attGain;
	
	// apply transmission
	if( hitElementBack && ( transmitLow > FLOAT_SAFE_EPSILON
	|| transmitMedium > FLOAT_SAFE_EPSILON || transmitHigh > FLOAT_SAFE_EPSILON ) ){
		// ray can continue. store parameters here so it is safe to use
	}
	
	// bounce ray and continue if still audible. from here on many the ray-trace result
	// can not be used anymore since it has been changed
	if( reflectedGain.low > 1e-4f || reflectedGain.medium > 1e-4f || reflectedGain.high > 1e-4f ){
		/*
		pOwner.GetAudioThread().GetLogger().LogInfoFormat( "(%p %d %d) Bounce: dist=%3f rem=%.3f gain=(%.3f,%.3f,%.3f) "
			" refl=(%.3f,%.3f,%.3f) abs=(%.3f,%.3f,%.3f) tra=(%.3f,%.3f,%.3f)",
			this, pRayIndex, ray.bounce + 1, reflectedRay.distance, reflectedRay.remainingDistance,
			gain.low, gain.medium, gain.high, reflectedGain.low, reflectedGain.medium, reflectedGain.high,
			absorbedLow, absorbedMedium, absorbedHigh, transmitLow, transmitMedium, transmitHigh );
		*/
		reflectedRay.bounce = ray.bounce + 1;
		if( reflectedRay.bounce < 100 ){ // avoid stack overflow and in general running away
			// if we start right at the collision point chances are the next ray check hits
			// the same element we just hit with 0 distance causing a false collision which
			// in turn kills off the entire ray on the spot. to avoid this problem we step
			// back on the ray a tiny bit and use this as ray origin. this introduces a small
			// error in the calculation which is though neglectable
			#ifdef THAN_DEBUG
			thanTemp.AppendFormat( " (%.3f,%.3f,%.3f)", hitPoint.x, hitPoint.y, hitPoint.z );
			#endif
			reflectedRay.position = hitPoint - ray.direction * pBackStepDistance;
			reflectedRay.direction = ray.direction + hitNormal * ( dotOut * 2.0 );
			pTraceRay( reflectedRay, reflectedGain );
		}
// 	}else{
// 		pOwner.GetAudioThread().GetLogger().LogInfoFormat( "(%p %d) Died out after %.3f", this, pRayIndex, distance );
	}
	
	// transmit ray if still audible
	if( hitElementBack ){
		// ray can continue. not done right now but would continue with ray in reduced form
// 		sray.SetParameters( component.GetTextureAt( face.GetTexture() ),
// 			hitElementBack->GetDistance() - hitDistance );
	}
}

void deoalRTPTEnvProbeFull::pUpdateExtends( const decDVector &position ){
	if( pFirstHit ){
		pMinExtend = pMaxExtend = position;
		pFirstHit = false;
		
	}else{
		pMinExtend.SetSmallest( position );
		pMaxExtend.SetLargest( position );
	}
}

