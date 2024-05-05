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
#include "../deoalRayTraceHitElement.h"
#include "../deoalRayTraceConfig.h"
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

deoalRTPTRoomEstimate::deoalRTPTRoomEstimate( deoalRTParallelEnvProbe &owner ) :
deParallelTask( &owner.GetAudioThread().GetOal() ),
pOwner( owner ),
pWorld( NULL ),
pProbeConfig( NULL ),
pFirstRay( 0 ),
#ifndef RTPTRE_ONE_TASK_PER_RAY
pRayCount( 0 ),
#endif
pRange( 0.0f ),
pInitialRayLength( 10.0f )
{
	(void)pOwner; // silence compiler warning
	
	pWOVRayHitsElement.SetResult( &pRTResult );
	SetMarkFinishedAfterRun( true );
	SetLowPriority( true );
}

deoalRTPTRoomEstimate::~deoalRTPTRoomEstimate(){
}



// Manegement
///////////////

void deoalRTPTRoomEstimate::SetWorld( deoalAWorld *world ){
	pWorld = world;
}

void deoalRTPTRoomEstimate::SetProbeConfig( const deoalRayTraceConfig *probeConfig ){
	pProbeConfig = probeConfig;
}

void deoalRTPTRoomEstimate::SetFirstRay( int firstRay ){
	pFirstRay = firstRay;
}

#ifndef RTPTRE_ONE_TASK_PER_RAY
void deoalRTPTRoomEstimate::SetRayCount( int rayCount ){
	pRayCount = rayCount;
}
#endif

void deoalRTPTRoomEstimate::SetPosition( const decDVector &position ){
	pPosition = position;
}

void deoalRTPTRoomEstimate::SetRange( float range ){
	pRange = range;
}

void deoalRTPTRoomEstimate::SetInitialRayLength( float length ){
	pInitialRayLength = length;
}

void deoalRTPTRoomEstimate::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}



void deoalRTPTRoomEstimate::Run(){
	#ifndef RTPTRE_ONE_TASK_PER_RAY
	const decVector * const rayDirections = pProbeConfig->GetRayDirections() + pFirstRay;
	int i;
	#endif
	
	pWOVRayHitsElement.SetLayerMask( pLayerMask );
	
	sHitResult hitResult;
	bool firstHit = true;
	
	pRoomVolume = 0.0f;
	pRoomSurface = 0.0f;
	pSabineLow = 0.0f;
	pSabineMedium = 0.0f;
	pSabineHigh = 0.0f;
	pAbsorptionLow = 0.0f;
	pAbsorptionMedium = 0.0f;
	pAbsorptionHigh = 0.0f;
	pHitCount = 0;
	
	#ifdef RTPTRE_ONE_TASK_PER_RAY
	const decVector &rayDirection = pProbeConfig->GetRayDirections()[ pFirstRay ];
	if( pTraceRay( rayDirection, hitResult ) ){
	#else
	for( i=0; i<pRayCount; i++ ){
		if( ! pTraceRay( rayDirections[ i ], hitResult ) ){
			continue;
		}
	#endif
		
		const decDVector &hitPoint = hitResult.element->GetPoint();
		
		// update estimated room extends
		if( firstHit ){
			pMinExtend = pMaxExtend = hitPoint;
			firstHit = false;
			
		}else{
			pMinExtend.SetSmallest( hitPoint );
			pMaxExtend.SetLargest( hitPoint );
		}
		
		// update room parameters
		float roomFactor = hitResult.distance * hitResult.distance;
		pRoomVolume += roomFactor * hitResult.distance;
		
		#ifdef RTPTRE_ONE_TASK_PER_RAY
		const float dotOut = -( float )( hitResult.element->GetNormal() * rayDirection );
		#else
		const float dotOut = -( hitResult.element->GetNormal() * rayDirections[ i ] );
		#endif
		roomFactor *= 1.0f + ( 1.0f - dotOut ) * 2.0f;
			// apply first a scale factor to make area larger if at shallow angle.
			// the value of the scale factor is found experimental and is not based
			// on calculations. this would be difficult to do and most probably not
			// much more true than this version here
		
		deoalAComponent &component = *hitResult.element->GetComponent();
		const deoalModelFace &face = component.GetModel()->GetFaceAt( hitResult.element->GetComponentFace() );
		const deoalAComponentTexture &texture = component.GetModelTextureAt( face.GetTexture() );
		
		pAbsorptionLow += texture.GetAbsorptionMedium();
		pAbsorptionMedium += texture.GetAbsorptionLow();
		pAbsorptionHigh += texture.GetAbsorptionHigh();
		
		pSabineMedium += roomFactor * texture.GetAbsorptionMedium();
		pSabineLow += roomFactor * texture.GetAbsorptionLow();
		pSabineHigh += roomFactor * texture.GetAbsorptionHigh();
		
		pRoomSurface += roomFactor;
		
		// number of rays hitting room geometry
		pHitCount++;
	}
}

void deoalRTPTRoomEstimate::Finished(){
	// this task never runs alone. it is always a dependency and clean up by these tasks
	/*
	if( GetDependedOnBy().GetCount() == 0 ){
		pOwner.Enable( this );
	}
	*/
}



// Debugging
//////////////

decString deoalRTPTRoomEstimate::GetDebugName() const{
	return "OpenAL-RTPTRoomEstimate";
}

decString deoalRTPTRoomEstimate::GetDebugDetails() const{
	return "";
}



// Private Functions
//////////////////////

bool deoalRTPTRoomEstimate::pTraceRay( const decVector &direction, sHitResult &hitResult ){
	decDVector rayOrigin( pPosition );
	float rayLength = pInitialRayLength;
	float rayOffset = 0.0f;
	int i;
	
	while( rayOffset < pRange ){
		const float nextRayOffset = decMath::min( rayOffset + rayLength, pRange );
		const decDVector rayEnd( pPosition + direction * nextRayOffset );
		rayLength *= 2.0f;
		
		pRTResult.Clear();
		pWOVRayHitsElement.SetRay( rayOrigin, rayEnd - rayOrigin );
		pWOVRayHitsElement.VisitNode( *pWorld->GetOctree() );
		
		const int hitCount = pRTResult.GetElementCount();
		
		for( i=0; i<hitCount; i++ ){
			const deoalRayTraceHitElement &he = pRTResult.GetElementAt( i );
			if( he.GetForwardFacing() ){
				hitResult.element = &he;
				hitResult.distance = rayOffset + he.GetDistance();
				return true;
			}
		}
		
		rayOffset = nextRayOffset;
		rayOrigin = rayEnd;
	}
	
	return false;
}
