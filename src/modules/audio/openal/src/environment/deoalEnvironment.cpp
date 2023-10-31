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

#include "deoalEnvironment.h"
#include "deoalEnvironmentDebug.h"
#include "deoalEnvProbe.h"
#include "deoalEnvProbeList.h"
#include "deoalEnvProbeListener.h"
#include "raytrace/deoalRayTraceResult.h"
#include "raytrace/deoalRayTraceHitElement.h"
#include "raytrace/deoalSoundRay.h"
#include "raytrace/deoalSoundRaySegment.h"
#include "raytrace/deoalEnergyHistogram.h"
#include "raytrace/visitors/deoalWOVRayHitsElement.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../audiothread/deoalATDebug.h"
#include "../component/deoalAComponent.h"
#include "../component/deoalAComponentTexture.h"
#include "../configuration/deoalConfiguration.h"
#include "../devmode/deoalDevMode.h"
#include "../extensions/efx.h"
#include "../extensions/deoalExtensions.h"
#include "../microphone/deoalAMicrophone.h"
#include "../model/deoalAModel.h"
#include "../model/deoalModelFace.h"
#include "../skin/deoalSkin.h"
#include "../skin/deoalSkinTexture.h"
#include "../world/deoalAWorld.h"
#include "../world/octree/deoalWorldOctree.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/debug/deDebugBlockInfo.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasImageReference.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasVisitorIdentify.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoalEnvironment
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalEnvironment::deoalEnvironment( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pWorld( NULL ),
pRange( 1.0f ),
pRangeSquared( 1.0f ),
pAttenuationRefDist( 1.0f ),
pAttenuationRolloff( 0.0f ),
pAttenuationDistanceOffset( 0.0f ),
pValid( false ),
pGainLow( 1.0f ),
pGainMedium( 1.0f ),
pGainHigh( 1.0f ),
pBandPassGain( 1.0f ),
pBandPassGainLF( 1.0f ),
pBandPassGainHF( 1.0f ),
pReverbGain( 0.32f ),
pReverbGainLF( 0.0f ),
pReverbGainHF( 0.89f ),
pReverbDecayTime( 1.49f ),
pReverbDecayHFRatio( 0.83f ),
pReverbDecayLFRatio( 1.0f ),
pReverbReflectionGain( 0.05f ),
pReverbReflectionDelay( 0.007f ),
pReverbLateReverbGain( 1.26f ),
pReverbLateReverbDelay( 0.011f ),
pReverbEchoTime( 0.25f ),
pResetListenerSmooth( true ),
pEnvProbe( NULL ),
pDebug( NULL )
{
	#ifdef LISTENER_CENTRIC_RAY_CAST
	pEnvProbe = new deoalEnvProbe( audioThread );
	#endif
}

deoalEnvironment::~deoalEnvironment(){
	SetWorld( NULL );
	
	if( pDebug ){
		delete pDebug;
	}
	
	if( pEnvProbe ){
		delete pEnvProbe;
	}
}



// Manegement
///////////////

void deoalEnvironment::SetWorld( deoalAWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	if( pEnvProbe && pEnvProbe->GetOctreeNode() ){
		pEnvProbe->GetOctreeNode()->RemoveEnvProbe( pEnvProbe );
	}
	
	pWorld = world;
}

void deoalEnvironment::SetPosition( const decDVector &position ){
	pPosition = position;
	
	if( pEnvProbe ){
		pEnvProbe->SetPosition( position );
		
		if( pEnvProbe->GetOctreeNode() ){
			pEnvProbe->GetOctreeNode()->RemoveEnvProbe( pEnvProbe );
		}
	}
}

void deoalEnvironment::SetRange( float range ){
	pRange = decMath::max( range, 0.0f );
	pRangeSquared = pRange * pRange;
	
	if( pEnvProbe ){
		pEnvProbe->SetRange( pRange );
		
		if( pEnvProbe->GetOctreeNode() ){
			pEnvProbe->GetOctreeNode()->RemoveEnvProbe( pEnvProbe );
		}
	}
}

void deoalEnvironment::SetAttenuation( float refDist, float rolloff, float distanceOffset ){
	pAttenuationRefDist = refDist;
	pAttenuationRolloff = rolloff;
	pAttenuationDistanceOffset = distanceOffset;
	
	if( pEnvProbe ){
		pEnvProbe->SetAttenuation( refDist, rolloff, distanceOffset );
		
		if( pEnvProbe->GetOctreeNode() ){
			pEnvProbe->GetOctreeNode()->RemoveEnvProbe( pEnvProbe );
		}
	}
}

void deoalEnvironment::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
	
	if( pEnvProbe ){
		pEnvProbe->SetLayerMask( layerMask );
		
		if( pEnvProbe->GetOctreeNode() ){
			pEnvProbe->GetOctreeNode()->RemoveEnvProbe( pEnvProbe );
		}
	}
}

void deoalEnvironment::PrepareQuickDispose(){
	if( pEnvProbe ){
		pEnvProbe->SetOctreeNode( nullptr );
	}
}



float deoalEnvironment::Distance( const deoalEnvironment &env, bool withPan ) const{
	const float d1 = env.pCompareReverbGain - pCompareReverbGain;
	const float d2 = env.pCompareReverbGainLF - pCompareReverbGainLF;
	const float d3 = env.pCompareReverbGainHF - pCompareReverbGainHF;
	const float d4 = env.pCompareReverbDecayTime - pCompareReverbDecayTime;
	const float d5 = env.pCompareReverbDecayHFRatio - pCompareReverbDecayHFRatio;
	const float d6 = env.pCompareReverbDecayLFRatio - pCompareReverbDecayLFRatio;
	const float d7 = env.pCompareReverbReflectionGain - pCompareReverbReflectionGain;
	const float d8 = env.pCompareReverbReflectionDelay - pCompareReverbReflectionDelay;
	const float d9 = env.pCompareReverbLateReverbGain - pCompareReverbLateReverbGain;
	const float d10 = env.pCompareReverbLateReverbDelay - pCompareReverbLateReverbDelay;
	const float d11 = env.pCompareReverbEchoTime - pCompareReverbEchoTime;
	
	float distanceSquared = d1 * d1 + d2 * d2 + d3 * d3 + d4 * d4 + d5 * d5
		+ d6 * d6 + d7 * d7 + d8 * d8 + d9 * d9 + d10 * d10 + d11 * d11;
	
	if( withPan ){
		distanceSquared += ( env.pCompareReverbReflectionPan - pCompareReverbReflectionPan ).LengthSquared();
		distanceSquared += ( env.pCompareReverbLateReverbPan - pCompareReverbLateReverbPan ).LengthSquared();
	}
	
	return sqrtf( distanceSquared );
}



void deoalEnvironment::Update(){
	pSetSilent();
	
	if( ! pWorld || ! pAudioThread.GetActiveMicrophone() ){
		return;
	}
	
	// skip if not heard by the microphone
	deoalAMicrophone &microphone = *pAudioThread.GetActiveMicrophone();
	if( microphone.GetLayerMask().MatchesNot( pLayerMask ) ){
		return;
	}
	
	// skip if outside range to save processing time
	const decQuaternion &micOrient = microphone.GetOrientation();
	const decDVector &micPos = microphone.GetPosition();
	const float distanceSquared = ( float )( ( micPos - pPosition ).LengthSquared() );
	const deoalConfiguration::eAuralizationModes AuralizationMode =
		pAudioThread.GetConfiguration().GetAuralizationMode();
	
	if( distanceSquared > pRangeSquared ){
		return;
	}
	
	// calculate direct path parameters
	pGainLow = 1.0f;
	pGainMedium = 1.0f;
	pGainHigh = 1.0f;
	
	switch( AuralizationMode ){
	case deoalConfiguration::eamDirectSound:
	case deoalConfiguration::eamFull:
		pDirectPath( microphone, micPos );
		break;
		
	default:
		break;
	}
	
	// calculate environment reflection parameters
	pReverbGain = 0.0f;
	pReverbGainLF = 0.0f;
	pReverbGainHF = 0.89f;
	pReverbDecayTime = 1.49f;
	pReverbDecayHFRatio = 0.83f;
	pReverbDecayLFRatio = 1.0f;
	pReverbReflectionGain = 0.05f;
	pReverbReflectionDelay = 0.007f;
	pReverbReflectionPan.SetZero();
	pReverbLateReverbGain = 1.26f;
	pReverbLateReverbDelay = 0.011f;
	pReverbLateReverbPan.SetZero();
	pReverbEchoTime = 0.25f;
	
	switch( AuralizationMode ){
	case deoalConfiguration::eamFull:
		pEnvReflection( microphone, micPos, micOrient );
		break;
		
	default:
		break;
	}
	
	pCalcEffectParameters();
	pCalcEffectKeepAliveTimeout();
	pCalcCompareParameters();
	
	pValid = true;
	
	// debug
// 	if( pAudioThread.GetDebug().GetLogCalcEnvProbe() ){
// 		DebugPrint();
// 	}
}



void deoalEnvironment::DebugPrint(){
	if( ! pDebug ){
		pDebug = new deoalEnvironmentDebug( *this );
	}
	pDebug->Print();
}

void deoalEnvironment::DebugUpdateInfo( deDebugBlockInfo &debugInfo ){
	if( ! pDebug ){
		pDebug = new deoalEnvironmentDebug( *this );
	}
	pDebug->UpdateInfo( debugInfo );
}

void deoalEnvironment::DebugUpdateDirect( deDebugDrawer &debugDrawer, const deoalAMicrophone &microphone ){
	deoalRayTraceResult &rtresult = pAudioThread.GetRTResultDirect();
	rtresult.Clear();
	
	deoalWOVRayHitsElement &visitor = pAudioThread.GetWOVRayHitsElement();
	visitor.SetRay( pPosition, microphone.GetPosition() - pPosition );
	visitor.SetResult( &rtresult );
	visitor.SetLayerMask( microphone.GetLayerMask() );
	
	pWorld->GetOctree()->VisitNodesColliding( &visitor, visitor.GetRayBoxMin(), visitor.GetRayBoxMax() );
	
	pAudioThread.GetLogger().LogInfoFormat( "DebugUpdateDirect: pos=(%f,%f,%f) mic=(%f,%f,%f)",
		pPosition.x, pPosition.y, pPosition.z, microphone.GetPosition().x,
		microphone.GetPosition().y, microphone.GetPosition().z );
	rtresult.DebugPrint( pAudioThread, "RT: " );
	
	const int count = rtresult.GetElementCount();
	decDVector lastPosition( pPosition );
	deDebugDrawerShape *ddshape = NULL;
	deDebugDrawerShapeFace *ddsface = NULL;
	const float sphereRadius = 0.01f; //0.0025f;
	
	debugDrawer.SetPosition( pPosition );
	debugDrawer.SetOrientation( decQuaternion() );
	debugDrawer.SetScale( decVector( 1.0f, 1.0f, 1.0f ) );
	debugDrawer.SetVisible( true );
	debugDrawer.SetXRay( true );
	debugDrawer.RemoveAllShapes();
	
	int i, frontBackCounter = 0;
	
	try{
		for( i=0; i<count; i++ ){
			const deoalRayTraceHitElement &hitElement = rtresult.GetElementAt( i );
			const int lastFrontBackCounter = frontBackCounter;
			
			if( hitElement.GetForwardFacing() ){
				frontBackCounter++;
				
			}else if( frontBackCounter > 0 ){
				frontBackCounter--;
			}
			
			ddshape = new deDebugDrawerShape;
			if( lastFrontBackCounter == 0 ){
				ddshape->SetFillColor( decColor( 0.0f, 1.0f, 0.0f ) );
				ddshape->SetEdgeColor( decColor( 0.0f, 1.0f, 0.0f ) );
			}else{
				ddshape->SetFillColor( decColor( 1.0f, 0.0f, 0.0f ) );
				ddshape->SetEdgeColor( decColor( 1.0f, 0.0f, 0.0f ) );
			}
			ddsface = new deDebugDrawerShapeFace;
			ddsface->AddVertex( lastPosition - pPosition );
			ddsface->AddVertex( hitElement.GetPoint() - pPosition );
			ddsface->AddVertex( lastPosition - pPosition );
			ddsface->SetNormal( decVector( 0.0f, 0.0f, 1.0f ) );
			ddshape->AddFace( ddsface );
			ddsface = NULL;
			ddshape->GetShapeList().Add( new decShapeSphere( sphereRadius, lastPosition - pPosition ) );
			debugDrawer.AddShape( ddshape );
			ddshape = NULL;
			lastPosition = hitElement.GetPoint();
		}
		
		ddshape = new deDebugDrawerShape;
		if( frontBackCounter == 0 ){
			ddshape->SetFillColor( decColor( 0.0f, 1.0f, 0.0f ) );
			ddshape->SetEdgeColor( decColor( 0.0f, 1.0f, 0.0f ) );
		}else{
			ddshape->SetFillColor( decColor( 1.0f, 0.0f, 0.0f ) );
			ddshape->SetEdgeColor( decColor( 1.0f, 0.0f, 0.0f ) );
		}
		ddsface = new deDebugDrawerShapeFace;
		ddsface->AddVertex( lastPosition - pPosition );
		ddsface->AddVertex( microphone.GetPosition() - pPosition );
		ddsface->AddVertex( lastPosition - pPosition );
		ddsface->SetNormal( decVector( 0.0f, 0.0f, 1.0f ) );
		ddshape->AddFace( ddsface );
		ddsface = NULL;
		ddshape->GetShapeList().Add( new decShapeSphere( sphereRadius, lastPosition - pPosition ) );
		ddshape->GetShapeList().Add( new decShapeSphere( sphereRadius, microphone.GetPosition() - pPosition ) );
		debugDrawer.AddShape( ddshape );
		
	}catch( const deException & ){
		if( ddsface ){
			delete ddsface;
		}
		if( ddshape ){
			delete ddshape;
		}
		throw;
	}
}

void deoalEnvironment::DebugSoundRays( deDebugDrawer &/*debugDrawer*/ ){
	if( ! pWorld ){
		return;
	}
	if( ! pDebug ){
		pDebug = new deoalEnvironmentDebug( *this );
	}
	
// 	const deoalEnvProbe &probe = *pWorld->GetEnvProbeManager().GetProbeTraceSoundRays( pPosition,
// 		pRange, pAttenuationRefDist, pAttenuationRolloff, pAttenuationDistanceOffset, pLayerMask );
// 	pDebug->SoundRays( debugDrawer, probe );
}



// Private Functions
//////////////////////

void deoalEnvironment::pDirectPath( const deoalAMicrophone &microphone, const decDVector &micPos ){
	deoalRayTraceResult &rtresult = pAudioThread.GetRTResultDirect();
	rtresult.Clear();
	
	deoalWOVRayHitsElement &visitor = pAudioThread.GetWOVRayHitsElement();
	visitor.SetRay( pPosition, micPos - pPosition );
	visitor.SetResult( &rtresult );
	visitor.SetLayerMask( microphone.GetLayerMask() );
	
// 	pAudioThread.GetLogger().LogInfoFormat( "- VisitNodesColliding (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)",
// 		visitor.GetRayBoxMin().x, visitor.GetRayBoxMin().y, visitor.GetRayBoxMin().z,
// 		visitor.GetRayBoxMax().x, visitor.GetRayBoxMax().y, visitor.GetRayBoxMax().z );
	pWorld->GetOctree()->VisitNodesColliding( &visitor, visitor.GetRayBoxMin(), visitor.GetRayBoxMax() );
	
	// apply elements in the direct path. for this start with the first forward facing face.
	// use the transmission parameters of this face. then find the next backwards facing face
	// if present. if there is no backward face drop sound level to 0 and stop. otherwise
	// use the backwards facing face to determine the thickness and apply the transmission
	// parameters of the forward facing face using this thickness. then repeat the process
	// until all triangles are exhausted.
	// 
	// now this process runs into troubles if faces interpenetrate. to deal with this a counter
	// is used to keep track of the number of encountered front and back faces. whenever a front
	// face is hit the counter in incremented. if the counter is larger than 1 apply the
	// transmission as if a back face is hit and switch to the new transmission parameters.
	const deoalRayTraceHitElement *curTransElement = NULL;
	const int count = rtresult.GetElementCount();
	int frontBackCounter = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		const deoalRayTraceHitElement &hitElement = rtresult.GetElementAt( i );
		const deoalRayTraceHitElement * const transmitFront = curTransElement;
		
		if( hitElement.GetForwardFacing() ){
			frontBackCounter++;
			curTransElement = &hitElement;
			
		}else if( frontBackCounter > 0 ){
			frontBackCounter--;
			if( frontBackCounter > 0 ){
				continue;
			}
			curTransElement = NULL;
		}
		
		if( ! transmitFront ){
			continue;
		}
		
		const float thickness = hitElement.GetDistance() - transmitFront->GetDistance();
		
		if( transmitFront->GetComponent() ){
			deoalAComponent &component = *transmitFront->GetComponent();
			const deoalModelFace &face = component.GetModel()->GetFaceAt( transmitFront->GetComponentFace() );
			const deoalAComponentTexture &compTex = component.GetModelTextureAt( face.GetTexture() );
			
			pGainLow *= decMath::linearStep( thickness, 0.0f, compTex.GetTransmissionLow(),
				1.0f - compTex.GetAbsorptionLow(), 0.0f );
			pGainMedium *= decMath::linearStep( thickness, 0.0f, compTex.GetTransmissionMedium(),
				1.0f - compTex.GetAbsorptionMedium(), 0.0f );
			pGainHigh *= decMath::linearStep( thickness, 0.0f, compTex.GetTransmissionHigh(),
				1.0f - compTex.GetAbsorptionHigh(), 0.0f );
			
// 			pAudioThread.GetLogger().LogInfoFormat(
// 				"source=(%.3f,%.3f,%.3f) thickness=%.3f trans(%.3f,%.3f,%.3f) gain=(%.3f,%.3f,%.3f)",
// 				pPosition.x, pPosition.y, pPosition.z, thickness,
// 				compTex.GetTransmissionLow(), compTex.GetTransmissionMedium(),
// 				compTex.GetTransmissionHigh(), pGainLow, pGainMedium, pGainHigh );
		}
	}
	
	if( frontBackCounter > 0 ){
		pSetSilent();
		return;
	}
// 	rtresult.DebugPrint( pAudioThread, "- Direct" );
}

void deoalEnvironment::pEnvReflection( deoalAMicrophone &microphone,
const decDVector &micPos, const decQuaternion &micOrient ){
	// NOTE
	// for full processing a probe with TraceSoundRays() has to be obtained. This probe
	// contains all the sound ray information required for CalcListener() to produce
	// the environment information. doing full processing though can consume >15ms on
	// common situations up to >30ms for large scale rooms. doing this for all sound
	// sources requiring probe updating is quite time consuming.
	// 
	// for this reason an alternate route is used. instead of doing full processing of
	// the probe for the sound source only the listener is doing full processing of a
	// listener probe. while still using the same amount of time it is done only once
	// per affected frame update since usually only one listener is present in the world.
	// for the sound source it is then enough to only obtain a probe using
	// EstimateRoomParameters(). this is a simple tracing estimating the room parameters
	// of the location the sound source is located in. this estimation can be calculated
	// in ~1ms and can also be further optimized if required. the environment parameters
	// are then the combination of the two room parameters. this allows situations to
	// work out properly where the sound source is located in a room with drastically
	// different reverberation.
	#ifdef LISTENER_CENTRIC_RAY_CAST
		//deoalEnvProbe &probe = *pWorld->GetEnvProbeManager().GetProbeEstimateRoom(
		//	pPosition, pRange, microphone.GetLayerMask() );
		
	#else
		deoalEnvProbe &probe = *pWorld->GetEnvProbeManager().GetProbeTraceSoundRays(
			pPosition, pRange, pAttenuationRefDist, pAttenuationRolloff,
			pAttenuationDistanceOffset, microphone.GetLayerMask() );
	#endif
	
	// indirect path reverbe effect
	// 
	// - AL_EAXREVERB_GAIN [0 -> 1; 0.32]
	//      master gain. all other gains are relative to this one.
	// - AL_EAXREVERB_GAINHF [0 -> 1; 0.89]
	//      high frequency gain relative to gain
	// - AL_EAXREVERB_GAINLF [0 -> 1; 0]
	//      low frequency gain relative to gain
	// 
	// - AL_EAXREVERB_DECAY_TIME [0.1 -> 20; 1.49] seconds
	//      reverbe decay time. min(small room, dead walls), max (large room, living walls).
	//      small/large room is clear. dead/living walls seems to mean absorbing/reflecting walls.
	//      bathroom: 1.49 . sewer pipe: 2.81 . hangar: 10.05 . 
	// - AL_EAXREVERB_DECAY_HFRATIO [0.1 -> 20; 0.83]
	//      factor times decayTime for high frequencies.
	//      <1 for natural reverbe, >1 for brilliant sounding reverbe
	// - AL_EAXREVERB_DECAY_LFRATIO [0.1 -> 20; 1]
	//      same for low frequencies.
	// 
	// - AL_EAXREVERB_REFLECTIONS_GAIN [0 -> 3.16; 0.05]
	//      initial reflection gain relative to gain. increase to simulate narrow/closer walls.
	//      works best with also reducing delay (below) for narrow/closer walls
	// - AL_EAXREVERB_REFLECTIONS_DELAY [0 -> 0.3; 0.007] seconds
	//      delay between direct-path and first-reflection. reduce: closer reflective walls.
	//      increase: farther away reflective walls. => perceived room size
	// 
	// - AL_EAXREVERB_REFLECTIONS_PAN [vector, len = 0 -> 1]
	//      panning direction relative to user sound system. uses dragengine coordinate system.
	//      z-axis is forward, x-axis to the right. length indicates how focused int he direction
	//      the panning is simulated. default is 0 meaning equally from all directions
	// 
	// - AL_EAXREVERB_LATE_REVERB_GAIN [0 -> 10; 1.26]
	//      later reverbe gain relative to main gain.
	// - AL_EAXREVERB_LATE_REVERB_DELAY [0 -> 0.1; 0.011] seconds
	//      delay between secondary reflections and first-reflection. reduce: smaller room.
	//      increase: larger room
	// 
	// - AL_EAXREVERB_LATE_REVERB_PAN [vector, len = 0 -> 1]
	//      panning direction relative to user sound system. uses dragengine coordinate system.
	//      z-axis is forward, x-axis to the right. length indicates how focused int he direction
	//      the panning is simulated. default is 0 meaning equally from all directions
	// 
	// - AL_EAXREVERB_ECHO_TIME [0.075 -> 0.25; 0.25] seconds
	//      echo repeat time.
	// - AL_EAXREVERB_ECHO_DEPTH [0 -> 1; 0]
	//      predominance of echo effect. reverbe diffusion affects how clearly the echo is
	//      distinguishable. with high diffusion the echos merge quickly while with low
	//      diffusion the echos are distinguishable.
	// 
	// since this is all a bit complicated here a summary for tricky parameters:
	// - reflectedGainLow = GAIN * GAINLF * REFLECTIONS_GAIN
	// - reflectedGainHigh = GAIN * GAINHF * REFLECTIONS_GAIN
	// - reverberationGainLow = GAIN * GAINLF * LATE_REVERB_GAIN
	// - reverberationGainHigh = GAIN * GAINHF * LATE_REVERB_GAIN
	
	const decQuaternion invMicOrient( micOrient.Conjugate() );
	
	// update for listener position
	deoalEnvProbeListener listener;
	#ifdef LISTENER_CENTRIC_RAY_CAST
	pEnvProbe->CalcListener( listener, *pWorld, micPos, &microphone );
	
	if( ! pEnvProbe->GetOctreeNode() ){
		pWorld->GetOctree()->InsertEnvProbeIntoTree( pEnvProbe, 8 );
	}
	
	#else
	probe.CalcListener( listener, *pWorld, micPos, NULL );
	#endif
	
	if( pResetListenerSmooth ){
		pListenerSmooth.Set( listener );
		pResetListenerSmooth = false;
		
	}else{
		pListenerSmooth.SetGoal( listener );
		pListenerSmooth.Update( pAudioThread.GetElapsedFull() );
		pListenerSmooth.AssignTo( listener );
	}
	
	const deoalConfiguration &config = pAudioThread.GetConfiguration();
	
	// master gain is set to the maximum gain of all frequencies (first and late) since
	// individual gains are limited to the range from 0 to 1.
	const float maxGainReflected = decMath::max( listener.GetReflectedLow(),
		listener.GetReflectedMedium(), listener.GetReflectedHigh() );
	const float maxGainLateReverb = decMath::max( listener.GetReverberationGainLow(),
		listener.GetReverberationGainMedium(), listener.GetReverberationGainHigh() );
	
	pReverbGain = decMath::clamp( decMath::max( maxGainReflected, maxGainLateReverb ),
		AL_EAXREVERB_MIN_GAIN, AL_EAXREVERB_MAX_GAIN );
	
// 	pReverbGain = decMath::clamp( maxGainReflected, AL_EAXREVERB_MIN_GAIN, AL_EAXREVERB_MAX_GAIN );
// 	pReverbGain = decMath::clamp( maxGainLateReverb, AL_EAXREVERB_MIN_GAIN, AL_EAXREVERB_MAX_GAIN );
	
	// low and high frequency scaling factors applied to all reverb calculations. this is
	// independent of the master gain hence 1 as master gain can be used. first and late
	// reflections can have different low and high frequency ratios but in the end we need
	// set values representing both adequately. an additional difficulty is if the initial
	// reflections are 0 and only late reflections are present. looking at the first reflection
	// gains to calculate the ratios would falsify the result. for this reason the maximum
	// of both gains is used. this way 0 gains do not falsify the result
	float invGain = 0.0f;
	if( pReverbGain > FLOAT_SAFE_EPSILON ){
		invGain = 1.0f / pReverbGain;
	}
	
	pReverbGainLF = decMath::clamp( invGain * decMath::max(
		listener.GetReflectedLow(), listener.GetReverberationGainLow() ),
			AL_EAXREVERB_MIN_GAINLF, AL_EAXREVERB_MAX_GAINLF );
	pReverbGainHF = decMath::clamp( invGain * decMath::max(
		listener.GetReflectedHigh(), listener.GetReverberationGainHigh() ),
			AL_EAXREVERB_MIN_GAINHF, AL_EAXREVERB_MAX_GAINHF );
	
// 	pReverbGainLF = decMath::clamp( invGain * listener.GetReflectedLow(), AL_EAXREVERB_MIN_GAINLF, AL_EAXREVERB_MAX_GAINLF );
// 	pReverbGainHF = decMath::clamp( invGain * listener.GetReflectedHigh(), AL_EAXREVERB_MIN_GAINHF, AL_EAXREVERB_MAX_GAINHF );
// 	pReverbGainLF = decMath::clamp( invGain * listener.GetReverberationGainLow(), AL_EAXREVERB_MIN_GAINLF, AL_EAXREVERB_MAX_GAINLF );
// 	pReverbGainHF = decMath::clamp( invGain * listener.GetReverberationGainHigh(), AL_EAXREVERB_MIN_GAINHF, AL_EAXREVERB_MAX_GAINHF );
	
	// decay time. use here the maximum value too. the range is up to 2 but better to keep
	// it at maximum at 1 to be on the save side
	pReverbDecayTime = decMath::clamp( decMath::max( listener.GetReverberationTimeLow(),
		listener.GetReverberationTimeMedium(), listener.GetReverberationTimeHigh() ),
		AL_EAXREVERB_MIN_DECAY_TIME, AL_EAXREVERB_MAX_DECAY_TIME );
	
	if( pReverbDecayTime > FLOAT_SAFE_EPSILON ){
		const float invDecayTime = 1.0f / pReverbDecayTime;
		pReverbDecayHFRatio = decMath::clamp( listener.GetReverberationTimeHigh() * invDecayTime,
			AL_EAXREVERB_MIN_DECAY_HFRATIO, AL_EAXREVERB_MAX_DECAY_HFRATIO );
		pReverbDecayLFRatio = decMath::clamp( listener.GetReverberationTimeLow() * invDecayTime,
			AL_EAXREVERB_MIN_DECAY_LFRATIO, AL_EAXREVERB_MAX_DECAY_LFRATIO );
		
	}else{
		pReverbDecayHFRatio = 1.0f;
		pReverbDecayLFRatio = 1.0f;
	}
	
	// early (first order) reflections
	pReverbReflectionGain = decMath::clamp( maxGainReflected * invGain
		* config.GetEAXReverbReflectionGainFactor(),
		AL_EAXREVERB_MIN_REFLECTIONS_GAIN, AL_EAXREVERB_MAX_REFLECTIONS_GAIN );
	pReverbReflectionDelay = decMath::clamp( listener.GetReflectionDelay(),
		AL_EAXREVERB_MIN_REFLECTIONS_DELAY, AL_EAXREVERB_MAX_REFLECTIONS_DELAY );
	
// 				pReverbReflectionGain = 0.0f;
	
	pReverbReflectionPan = invMicOrient * listener.GetReflectionPan();
	/*if( pReverbReflectionPan.LengthSquared() > 1.0f ){
		pReverbReflectionPan.Normalize();
	}*/
	
	// late (higher order) reflections
	pReverbLateReverbGain = decMath::clamp( maxGainLateReverb * invGain
		* config.GetEAXReverbLateReverbGainFactor(),
		AL_EAXREVERB_MIN_LATE_REVERB_GAIN, AL_EAXREVERB_MAX_LATE_REVERB_GAIN );
	pReverbLateReverbDelay = decMath::clamp( listener.GetReverberationDelay(),
		AL_EAXREVERB_MIN_LATE_REVERB_DELAY, AL_EAXREVERB_MAX_LATE_REVERB_DELAY );
	
// 				pReverbLateReverbGain = 0.0f;
	
	// echo. perhaps use the echo-depth for echo-times below min-echo-time like echo-depth =
	// decMath::linearStep(echoTime, 0, minEchoTime, 1, 0). in the example profiles a value
	// larger than 0 is though only use for forest profile
	pReverbEchoTime = decMath::clamp( listener.GetEchoDelay(),
		AL_EAXREVERB_MIN_ECHO_TIME, AL_EAXREVERB_MAX_ECHO_TIME );
	
	pReverbLateReverbPan = invMicOrient * listener.GetReverberationPan();
	/*if( pReverbLateReverbPan.LengthSquared() > 1.0f ){
		pReverbLateReverbPan.Normalize();
	}*/
	
	
// 	if(pPosition.IsEqualTo(decDVector(-24.3,1.6,56.7))){
// 		pAudioThread.GetLogger().LogInfoFormat("EAX: %f %f %f (%f %f; %f %f)",
// 			pReverbGain, pReverbGainLF, pReverbGainHF,
// 			listener.GetReflectedLow(), listener.GetReverberationGainLow(),
// 			listener.GetReflectedHigh(), listener.GetReverberationGainHigh() );
// 	}
	
	if( pDebug ){
		#ifdef LISTENER_CENTRIC_RAY_CAST
		deoalEnvProbe * const envProbe = microphone.GetEnvProbe();
		if( envProbe ){
			pDebug->Prepare( *envProbe, listener );
		}
		#else
		pDebug->Prepare( probe, listener );
		#endif
	}
}

void deoalEnvironment::pSetSilent(){
	pGainLow = 0.0f;
	pGainMedium = 0.0f;
	pGainHigh = 0.0f;
	
	pBandPassGain = 0.0f;
	pBandPassGainLF = 0.0f;
	pBandPassGainHF = 0.0f;
	
	pReverbGain = 0.0f;
	pReverbGainLF = 0.0f;
	pReverbGainHF = 0.0f;
}

void deoalEnvironment::pCalcEffectParameters(){
	// direct path muffling effect. unfortunately the band-pass filter in OpenAL is bit of a joke.
	// it is either a low-pass filter or a high-pass filter but no real band-pass filter. we need
	// to set the gain to the highest attenuation then set the lower and upper gain relative to it
	pBandPassGain = decMath::max( pGainLow, pGainMedium, pGainHigh );
	
	if( pBandPassGain > 0.001f ){
		pBandPassGainLF = pGainLow / pBandPassGain;
		pBandPassGainHF = pGainHigh / pBandPassGain;
		
	}else{
		pBandPassGainLF = 0.0f;
		pBandPassGainHF = 0.0f;
	}
}

void deoalEnvironment::pCalcEffectKeepAliveTimeout(){
	// update slot with effect parameters if changed. this also resets keep-alive and sets the
	// keep-alive timeout. this timeout is used to clear the effect after the source stopped
	// using the effect slot to improve performance. as timeout the largest delay or ech time
	// is used. it just has to be long enough for all residue sound to vanish
	const float delay = pReverbReflectionDelay + pReverbLateReverbDelay;
	
	pEffectKeepAliveTimeout = delay + pReverbDecayTime;
	
	pEffectKeepAliveTimeout = decMath::max( pEffectKeepAliveTimeout,
		delay + pReverbDecayTime * pReverbDecayHFRatio );
	
	pEffectKeepAliveTimeout = decMath::max( pEffectKeepAliveTimeout,
		delay + pReverbDecayTime * pReverbDecayLFRatio );
}

void deoalEnvironment::pCalcCompareParameters(){
	pCompareReverbGain = pReverbGain;
	pCompareReverbGainLF = pReverbGainLF;
	pCompareReverbGainHF = pReverbGainHF;
	pCompareReverbDecayTime = decMath::linearStep( pReverbDecayTime,
		AL_EAXREVERB_MIN_DECAY_TIME, AL_EAXREVERB_MAX_DECAY_TIME );
	pCompareReverbDecayHFRatio = decMath::linearStep( pReverbDecayHFRatio,
		AL_EAXREVERB_MIN_DECAY_HFRATIO, AL_EAXREVERB_MAX_DECAY_HFRATIO );
	pCompareReverbDecayLFRatio = decMath::linearStep( pReverbDecayLFRatio,
		AL_EAXREVERB_MIN_DECAY_LFRATIO, AL_EAXREVERB_MAX_DECAY_LFRATIO );
	pCompareReverbReflectionGain = decMath::linearStep( pReverbReflectionGain,
		AL_EAXREVERB_MIN_REFLECTIONS_GAIN, AL_EAXREVERB_MAX_REFLECTIONS_GAIN );
	pCompareReverbReflectionDelay = decMath::linearStep( pReverbReflectionDelay,
		AL_EAXREVERB_MIN_REFLECTIONS_DELAY, AL_EAXREVERB_MAX_REFLECTIONS_DELAY );
	pCompareReverbReflectionPan = pCalcComparePan( pReverbReflectionPan );
	pCompareReverbLateReverbGain = decMath::linearStep( pReverbLateReverbGain,
		AL_EAXREVERB_MIN_LATE_REVERB_GAIN, AL_EAXREVERB_MAX_LATE_REVERB_GAIN );
	pCompareReverbLateReverbDelay = decMath::linearStep( pReverbLateReverbDelay,
		AL_EAXREVERB_MIN_LATE_REVERB_DELAY, AL_EAXREVERB_MAX_LATE_REVERB_DELAY );
	pCompareReverbLateReverbPan = pCalcComparePan( pReverbLateReverbPan );
	pCompareReverbEchoTime = decMath::linearStep( pReverbEchoTime,
		AL_EAXREVERB_MIN_ECHO_TIME, AL_EAXREVERB_MAX_ECHO_TIME );
}

decVector deoalEnvironment::pCalcComparePan( const decVector &pan ) const{
	const float length = pan.Length();
	if( length < 0.01f ){
		return decVector();
	}
	
	const float azimuth = atan2f( -pan.x, pan.z );
	const float elevation = atan2f( pan.y, sqrtf( pan.x * pan.x + pan.z * pan.z ) );
	return decVector( decMath::linearStep( azimuth, -PI, PI ),
		decMath::linearStep( elevation, -HALF_PI, HALF_PI ), length );
}
