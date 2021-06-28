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

#include "deoalASoundLevelMeter.h"
#include "deoalASoundLevelMeterSpeaker.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../component/deoalAComponent.h"
#include "../component/deoalAComponentTexture.h"
#include "../environment/deoalEnvProbe.h"
#include "../environment/deoalEnvProbeListener.h"
#include "../environment/raytrace/deoalRayTraceResult.h"
#include "../environment/raytrace/deoalRayTraceHitElement.h"
#include "../environment/raytrace/visitors/deoalWOVRayHitsElement.h"
#include "../model/deoalAModel.h"
#include "../model/deoalModelFace.h"
#include "../speaker/deoalASpeaker.h"
#include "../world/deoalAWorld.h"
#include "../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalASoundLevelMeterSpeaker
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalASoundLevelMeterSpeaker::deoalASoundLevelMeterSpeaker(
	deoalASoundLevelMeter &soundLevelMeter, deoalASpeaker *speaker ) :
pSoundLevelMeter( soundLevelMeter ),
pSpeaker( speaker ),
pVolume( 0.0f ),
pEnvProbe( NULL ),
pListener( NULL ),
pGainLow( 0.0f ),
pGainMedium( 0.0f ),
pGainHigh( 0.0f ){
}

deoalASoundLevelMeterSpeaker::~deoalASoundLevelMeterSpeaker(){
	EnvProbeDropOctreeNode();
	
	if( pEnvProbe ){
		delete pEnvProbe;
	}
	if( pListener ){
		delete pListener;
	}
}



// Management
///////////////

void deoalASoundLevelMeterSpeaker::SetVolume( float volume ){
	pVolume = volume;
}

void deoalASoundLevelMeterSpeaker::EnvProbeDropOctreeNode(){
	if( pEnvProbe && pEnvProbe->GetOctreeNode() ){
		pEnvProbe->GetOctreeNode()->RemoveEnvProbe( pEnvProbe );
	}
}

void deoalASoundLevelMeterSpeaker::SpeakerPositionChanged(){
	if( ! pEnvProbe ){
		return;
	}
	
	pEnvProbe->SetPosition( pSpeaker->GetPosition() );
	
	if( pEnvProbe->GetOctreeNode() ){
		pEnvProbe->GetOctreeNode()->RemoveEnvProbe( pEnvProbe );
	}
}

void deoalASoundLevelMeterSpeaker::SpeakerLayerMaskChanged(){
	if( ! pEnvProbe ){
		return;
	}
	
	pEnvProbe->SetLayerMask( pSpeaker->GetLayerMask() );
	
	if( pEnvProbe->GetOctreeNode() ){
		pEnvProbe->GetOctreeNode()->RemoveEnvProbe( pEnvProbe );
	}
}

void deoalASoundLevelMeterSpeaker::SpeakerRangeChanged(){
	if( ! pEnvProbe ){
		return;
	}
	
	pEnvProbe->SetRange( pSpeaker->GetRange() );
	
	if( pEnvProbe->GetOctreeNode() ){
		pEnvProbe->GetOctreeNode()->RemoveEnvProbe( pEnvProbe );
	}
}

void deoalASoundLevelMeterSpeaker::SpeakerAttenuationChanged(){
	if( ! pEnvProbe ){
		return;
	}
	
	pEnvProbe->SetAttenuation( pSpeaker->GetAttenuationRefDist(), pSpeaker->GetAttenuationRolloff() );
	
	if( pEnvProbe->GetOctreeNode() ){
		pEnvProbe->GetOctreeNode()->RemoveEnvProbe( pEnvProbe );
	}
}

void deoalASoundLevelMeterSpeaker::Listen(){
	if( ! pSoundLevelMeter.GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	pGainLow = 0.0f;
	pGainMedium = 0.0f;
	pGainHigh = 0.0f;
	
	pListenDirect();
	pListenReflected();
	
	pVolume = pSpeaker->GetVolume() * decMath::max( pGainLow, pGainMedium, pGainHigh );
}



// Private Functions
//////////////////////

void deoalASoundLevelMeterSpeaker::pListenDirect(){
	const decDVector &speakerPosition = pSpeaker->GetPosition();
	const decDVector &meterPosition = pSoundLevelMeter.GetPosition();
	const float distance = ( float )( ( speakerPosition - meterPosition ).Length() );
	const float gain = pSpeaker->AttenuatedGain( distance );
	if( gain < 1e-3f ){
		return;
	}
	
	deoalAWorld &world = *pSoundLevelMeter.GetParentWorld();
	deoalAudioThread &audioThread = pSoundLevelMeter.GetAudioThread();
	
	deoalRayTraceResult &rtresult = audioThread.GetRTResultDirect();
	rtresult.Clear();
	
	deoalWOVRayHitsElement &visitor = audioThread.GetWOVRayHitsElement();
	visitor.SetRay( meterPosition, speakerPosition - meterPosition );
	visitor.SetResult( &rtresult );
	visitor.SetLayerMask( pSoundLevelMeter.GetLayerMask() );
	
	world.GetOctree()->VisitNodesColliding( &visitor, visitor.GetRayBoxMin(), visitor.GetRayBoxMax() );
	
	const int count = rtresult.GetElementCount();
	int index = 0;
	
	float gainLow = gain;
	float gainMedium = gain;
	float gainHigh = gain;
	
	while( index < count ){
		const deoalRayTraceHitElement * const forward = pNextHitElement( rtresult, index, true );
		if( ! forward ){
			break;
		}
		
		const deoalRayTraceHitElement * const backward = pNextHitElement( rtresult, index, false );
		if( ! backward ){
			return;
		}
		
		const float thickness = backward->GetDistance() - forward->GetDistance();
		
		if( forward->GetComponent() ){
			deoalAComponent &component = *forward->GetComponent();
			const deoalModelFace &face = component.GetModel()->GetFaceAt( forward->GetComponentFace() );
			const deoalAComponentTexture &compTex = component.GetModelTextureAt( face.GetTexture() );
			
			gainLow *= decMath::linearStep( thickness, 0.0f, compTex.GetTransmissionLow(),
				1.0f - compTex.GetAbsorptionLow(), 0.0f );
			gainMedium *= decMath::linearStep( thickness, 0.0f, compTex.GetTransmissionMedium(),
				1.0f - compTex.GetAbsorptionMedium(), 0.0f );
			gainHigh *= decMath::linearStep( thickness, 0.0f, compTex.GetTransmissionHigh(),
				1.0f - compTex.GetAbsorptionHigh(), 0.0f );
			
		}else{
			return;
		}
	}
	
	pGainLow += gainLow;
	pGainMedium += gainMedium;
	pGainHigh += gainHigh;
}

void deoalASoundLevelMeterSpeaker::pListenReflected(){
	if( ! pEnvProbe ){
		pEnvProbe = new deoalEnvProbe( pSoundLevelMeter.GetAudioThread() );
		pEnvProbe->SetPosition( pSpeaker->GetPosition() );
		pEnvProbe->SetRange( pSpeaker->GetRange() );
		pEnvProbe->SetAttenuation( pSpeaker->GetAttenuationRefDist(),
			pSpeaker->GetAttenuationRolloff() );
	}
	
	if( ! pListener ){
		pListener = new deoalEnvProbeListener;
	}
	
	pEnvProbe->CalcListener( *pListener, *pSoundLevelMeter.GetParentWorld(),
		pSoundLevelMeter.GetPosition(), &pSoundLevelMeter );
	
	if( ! pEnvProbe->GetOctreeNode() ){
		pSoundLevelMeter.GetParentWorld()->GetOctree()->InsertEnvProbeIntoTree( pEnvProbe, 8 );
	}
	
// 	pSoundLevelMeter.GetAudioThread().GetLogger().LogInfoFormat( "pListenReflected: %f %f %f, %f %f %f %d",
// 		pListener->GetReflectedLow(), pListener->GetReflectedMedium(), pListener->GetReflectedHigh(),
// 		pListener->GetReverberationGainLow(), pListener->GetReverberationGainMedium(),
// 		pListener->GetReverberationGainHigh(), pListener->GetImpulseResponse().GetCount() );
	
	pGainLow += pListener->GetReflectedLow();
	pGainMedium += pListener->GetReflectedMedium();
	pGainHigh += pListener->GetReflectedHigh();
	
	pGainLow += pListener->GetReverberationGainLow();
	pGainMedium += pListener->GetReverberationGainMedium();
	pGainHigh += pListener->GetReverberationGainHigh();
}

const deoalRayTraceHitElement *deoalASoundLevelMeterSpeaker::pNextHitElement(
const deoalRayTraceResult &rtresult, int& index, bool forwardFacing ) const{
	const int count = rtresult.GetElementCount();
	while( index < count ){
		const deoalRayTraceHitElement &hitElement = rtresult.GetElementAt( index++ );
		if( hitElement.GetForwardFacing() == forwardFacing ){
			return &hitElement;
		}
	}
	return NULL;
}
