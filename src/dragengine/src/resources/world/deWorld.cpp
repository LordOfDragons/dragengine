/* 
 * Drag[en]gine Game Engine
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

#include "deWorld.h"
#include "deWorldManager.h"
#include "../billboard/deBillboard.h"
#include "../camera/deCamera.h"
#include "../collider/deCollider.h"
#include "../component/deComponent.h"
#include "../debug/deDebugDrawer.h"
#include "../forcefield/deForceField.h"
#include "../light/deLight.h"
#include "../navigation/navigator/deNavigator.h"
#include "../navigation/space/deNavigationSpace.h"
#include "../navigation/blocker/deNavigationBlocker.h"
#include "../network/deNetworkState.h"
#include "../particle/deParticleEmitterInstance.h"
#include "../propfield/dePropField.h"
#include "../sensor/deLumimeter.h"
#include "../sensor/deTouchSensor.h"
#include "../sensor/deSoundLevelMeter.h"
#include "../smoke/deSmokeEmitter.h"
#include "../sound/deMicrophone.h"
#include "../sound/deSpeaker.h"
#include "../sky/deSkyInstance.h"
#include "../terrain/heightmap/deHeightTerrain.h"
#include "../probe/deEnvMapProbe.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/ai/deBaseAIWorld.h"
#include "../../systems/modules/audio/deBaseAudioWorld.h"
#include "../../systems/modules/graphic/deBaseGraphicWorld.h"
#include "../../systems/modules/physics/deBasePhysicsWorld.h"
#include "../../systems/modules/network/deBaseNetworkWorld.h"



// Class deWorld
//////////////////

// Constructor, destructor
////////////////////////////

deWorld::deWorld( deWorldManager *manager ) :
deResource( manager ),

pHeightTerrain( NULL ),
pSize( 1000.0, 1000.0, 1000.0 ),

pDisableLights( false ),

pSkyRoot( NULL ),
pSkyTail( NULL ),
pSkyCount( 0 ),

pBillboardRoot( NULL ),
pBillboardTail( NULL ),
pBillboardCount( 0 ),

pCameraRoot( NULL ),
pCameraTail( NULL ),
pCameraCount( 0 ),

pColliderRoot( NULL ),
pColliderTail( NULL ),
pColliderCount( 0 ),

pComponentRoot( NULL ),
pComponentTail( NULL ),
pComponentCount( 0 ),

pDebugDrawerRoot( NULL ),
pDebugDrawerTail( NULL ),
pDebugDrawerCount( 0 ),

pEnvMapProbeRoot( NULL ),
pEnvMapProbeTail( NULL ),
pEnvMapProbeCount( 0 ),

pForceFieldRoot( NULL ),
pForceFieldTail( NULL ),
pForceFieldCount( 0 ),

pLightRoot( NULL ),
pLightTail( NULL ),
pLightCount( 0 ),

pLumimeterRoot( NULL ),
pLumimeterTail( NULL ),
pLumimeterCount( 0 ),

pMicrophoneRoot( NULL ),
pMicrophoneTail( NULL ),
pMicrophoneCount( 0 ),

pNavSpaceRoot( NULL ),
pNavSpaceTail( NULL ),
pNavSpaceCount( 0 ),

pNavBlockerRoot( NULL ),
pNavBlockerTail( NULL ),
pNavBlockerCount( 0 ),

pNavigatorRoot( NULL ),
pNavigatorTail( NULL ),
pNavigatorCount( 0 ),

pNetworkStateRoot( NULL ),
pNetworkStateTail( NULL ),
pNetworkStateCount( 0 ),

pParticleEmitterRoot( NULL ),
pParticleEmitterTail( NULL ),
pParticleEmitterCount( 0 ),

pPropFieldRoot( NULL ),
pPropFieldTail( NULL ),
pPropFieldCount( 0 ),

pSmokeEmitterRoot( NULL ),
pSmokeEmitterTail( NULL ),
pSmokeEmitterCount( 0 ),

pSpeakerRoot( NULL ),
pSpeakerTail( NULL ),
pSpeakerCount( 0 ),

pTouchSensorRoot( NULL ),
pTouchSensorTail( NULL ),
pTouchSensorCount( 0 ),

pSoundLevelMeterRoot( NULL ),
pSoundLevelMeterTail( NULL ),
pSoundLevelMeterCount( 0 ),

pPeerGraphic ( NULL ),
pPeerPhysics ( NULL ),
pPeerAudio ( NULL ),
pPeerNetwork ( NULL ),
pPeerAI( NULL ){
}

deWorld::~deWorld(){
	pCleanUp();
}



// Management
///////////////

void deWorld::Clear(){
	RemoveAllDebugDrawers();
	
	RemoveAllNetworkStates();
	
	RemoveAllCameras();
	RemoveAllMicrophones();
	
	RemoveAllTouchSensors();
	RemoveAllSoundLevelMeters();
	RemoveAllLumimeters();
	RemoveAllEnvMapProbes();
	
	RemoveAllParticleEmitters();
	RemoveAllSmokeEmitters();
	
	RemoveAllColliders();
	RemoveAllLights();
	RemoveAllComponents();
	RemoveAllSpeakers();
	RemoveAllPropFields();
	RemoveAllForceFields();
	RemoveAllBillboards();
	
	RemoveAllNavigators();
	RemoveAllNavigationBlockers();
	RemoveAllNavigationSpaces();
	
	RemoveAllSkies();
}

// #define DO_SPECIAL_DEBUG
#ifdef DO_SPECIAL_DEBUG
	#include "../../common/utils/decTimer.h"
	#include "../../logger/deLogger.h"
	static decTimer timer;
#endif
void deWorld::Update( float elapsed ){
	#ifdef DO_SPECIAL_DEBUG
	unsigned int elapsedGra = 0;
	unsigned int elapsedPhy = 0;
	unsigned int elapsedAud = 0;
	unsigned int elapsedAI = 0;
	timer.Reset();
	#endif
	
	if( pPeerGraphic ){
		pPeerGraphic->Update( elapsed );
		#ifdef DO_SPECIAL_DEBUG
		elapsedGra = ( unsigned int )( (double)timer.GetElapsedTime() * 1e6 );
		#endif
	}
	if( pPeerPhysics ){
		pPeerPhysics->Update( elapsed );
		#ifdef DO_SPECIAL_DEBUG
		elapsedPhy = ( unsigned int )( (double)timer.GetElapsedTime() * 1e6 );
		#endif
	}
	if( pPeerAudio ){
		pPeerAudio->Update( elapsed );
		#ifdef DO_SPECIAL_DEBUG
		elapsedAud = ( unsigned int )( (double)timer.GetElapsedTime() * 1e6 );
		#endif
	}
	if( pPeerAI ){
		pPeerAI->Update( elapsed );
		#ifdef DO_SPECIAL_DEBUG
		elapsedAI = ( unsigned int )( (double)timer.GetElapsedTime() * 1e6 );
		#endif
	}
	
	#ifdef DO_SPECIAL_DEBUG
	GetEngine()->GetLogger()->LogInfoFormat( "Dragengine", "deWorld Update Gra %iys", elapsedGra );
	GetEngine()->GetLogger()->LogInfoFormat( "Dragengine", "deWorld Update Phy %iys", elapsedPhy );
	GetEngine()->GetLogger()->LogInfoFormat( "Dragengine", "deWorld Update Aud %iys", elapsedAud );
	GetEngine()->GetLogger()->LogInfoFormat( "Dragengine", "deWorld Update AI %iys", elapsedAI );
	#endif
}

void deWorld::ProcessPhysics( float elapsed ){
	if( pPeerPhysics ){
		pPeerPhysics->ProcessPhysics( elapsed );
	}
}



void deWorld::SetSize( const decDVector &size ){
	if( size.x < 1.0 || size.y < 1.0 || size.z < 1.0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! size.IsEqualTo( pSize ) ){
		pSize = size;
		
		if( pPeerGraphic ){
			pPeerGraphic->SizeChanged();
		}
		if( pPeerPhysics ){
			pPeerPhysics->SizeChanged();
		}
		if( pPeerAudio ){
			pPeerAudio->SizeChanged();
		}
	}
}

void deWorld::SetGravity( const decVector &gravity ){
	if( ! gravity.IsEqualTo( pGravity ) ){
		pGravity = gravity;
		
		pNotifyPhysicsChanged();
	}
}



void deWorld::SetDisableLights( bool disable ){
	if( disable == pDisableLights ){
		return;
	}
	
	pDisableLights = disable;
	pNotifyLightingChanged();
}

void deWorld::SetAmbientLight( const decColor &color ){
	if( color.IsEqualTo( pAmbient ) ){
		return;
	}
	
	pAmbient = color;
	pNotifyLightingChanged();
}

void deWorld::SetLightColorMatrix( const decMatrix &matrix ){
	if( matrix.IsEqualTo( pLightColorMatrix ) ){
		return;
	}
	
	pLightColorMatrix = matrix;
	pNotifyLightingChanged();
}



void deWorld::SetHeightTerrain( deHeightTerrain *heightTerrain ){
	if( heightTerrain != pHeightTerrain ){
		if( pHeightTerrain ) pHeightTerrain->FreeReference();
		
		pHeightTerrain = heightTerrain;
		
		if( pHeightTerrain ) pHeightTerrain->AddReference();
		
		if( pPeerGraphic ){
			pPeerGraphic->HeightTerrainChanged();
		}
		if( pPeerPhysics ){
			pPeerPhysics->HeightTerrainChanged();
		}
		if( pPeerAI ){
			pPeerAI->HeightTerrainChanged();
		}
	}
}



// Skies
//////////

void deWorld::AddSky( deSkyInstance *sky ){
	if( ! sky || sky->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pSkyTail ){
		pSkyTail->SetLLWorldNext( sky );
		sky->SetLLWorldPrev( pSkyTail );
		sky->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		sky->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		sky->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pSkyRoot = sky;
	}
	
	pSkyTail = sky;
	pSkyCount++;
	sky->SetParentWorld( this );
	sky->AddReference();
	
	if( pPeerGraphic ){
		pPeerGraphic->SkyAdded( sky );
	}
}

void deWorld::RemoveSky( deSkyInstance *sky ){
	if( ! sky || sky->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( sky->GetLLWorldPrev() ){
		sky->GetLLWorldPrev()->SetLLWorldNext( sky->GetLLWorldNext() );
	}
	if( sky->GetLLWorldNext() ){
		sky->GetLLWorldNext()->SetLLWorldPrev( sky->GetLLWorldPrev() );
	}
	if( sky == pSkyRoot ){
		pSkyRoot = sky->GetLLWorldNext();
	}
	if( sky == pSkyTail ){
		pSkyTail = sky->GetLLWorldPrev();
	}
	pSkyCount--;
	
	sky->SetParentWorld( NULL );
	sky->SetLLWorldPrev( NULL );
	sky->SetLLWorldNext( NULL );
	if( pPeerGraphic ){
		pPeerGraphic->SkyRemoved( sky );
	}
	sky->FreeReference();
}

void deWorld::RemoveAllSkies(){
	if( pPeerGraphic ){
		pPeerGraphic->AllSkiesRemoved();
	}
	
	while( pSkyTail ){
		deSkyInstance * const next = pSkyTail->GetLLWorldPrev();
		pSkyTail->SetParentWorld( NULL );
		pSkyTail->FreeReference();
		pSkyTail = next;
		pSkyCount--;
	}
	pSkyRoot = NULL;
}



// Billboards
///////////////

void deWorld::AddBillboard( deBillboard *billboard ){
	if( ! billboard || billboard->GetParentWorld() ) DETHROW( deeInvalidParam );
	
	if( pBillboardTail ){
		pBillboardTail->SetLLWorldNext( billboard );
		billboard->SetLLWorldPrev( pBillboardTail );
		billboard->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		billboard->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		billboard->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pBillboardRoot = billboard;
	}
	
	pBillboardTail = billboard;
	pBillboardCount++;
	billboard->SetParentWorld( this );
	billboard->AddReference();
	
	if( pPeerGraphic ){
		pPeerGraphic->BillboardAdded( billboard );
	}
}

void deWorld::RemoveBillboard( deBillboard *billboard ){
	if( ! billboard || billboard->GetParentWorld() != this ) DETHROW( deeInvalidParam );
	
	if( billboard->GetLLWorldPrev() ){
		billboard->GetLLWorldPrev()->SetLLWorldNext( billboard->GetLLWorldNext() );
	}
	if( billboard->GetLLWorldNext() ){
		billboard->GetLLWorldNext()->SetLLWorldPrev( billboard->GetLLWorldPrev() );
	}
	if( billboard == pBillboardRoot ){
		pBillboardRoot = billboard->GetLLWorldNext();
	}
	if( billboard == pBillboardTail ){
		pBillboardTail = billboard->GetLLWorldPrev();
	}
	pBillboardCount--;
	
	billboard->SetParentWorld( NULL );
	billboard->SetLLWorldPrev( NULL );
	billboard->SetLLWorldNext( NULL );
	if( pPeerGraphic ){
		pPeerGraphic->BillboardRemoved( billboard );
	}
	billboard->FreeReference();
}

void deWorld::RemoveAllBillboards(){
	if( pPeerGraphic ){
		pPeerGraphic->AllBillboardsRemoved();
	}
	
	while( pBillboardTail ){
		deBillboard * const next = pBillboardTail->GetLLWorldPrev();
		pBillboardTail->SetParentWorld( NULL );
		pBillboardTail->FreeReference();
		pBillboardTail = next;
		pBillboardCount--;
	}
	pBillboardRoot = NULL;
}



// Cameras
////////////

void deWorld::AddCamera( deCamera *camera ){
	if( ! camera || camera->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pCameraTail ){
		pCameraTail->SetLLWorldNext( camera );
		camera->SetLLWorldPrev( pCameraTail );
		camera->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		camera->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		camera->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pCameraRoot = camera;
	}
	
	pCameraTail = camera;
	pCameraCount++;
	camera->SetParentWorld( this );
	camera->AddReference();
	
	if( pPeerGraphic ){
		pPeerGraphic->CameraAdded( camera );
	}
}

void deWorld::RemoveCamera( deCamera *camera ){
	if( ! camera || camera->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( camera->GetLLWorldPrev() ){
		camera->GetLLWorldPrev()->SetLLWorldNext( camera->GetLLWorldNext() );
	}
	if( camera->GetLLWorldNext() ){
		camera->GetLLWorldNext()->SetLLWorldPrev( camera->GetLLWorldPrev() );
	}
	if( camera == pCameraRoot ){
		pCameraRoot = camera->GetLLWorldNext();
	}
	if( camera == pCameraTail ){
		pCameraTail = camera->GetLLWorldPrev();
	}
	pCameraCount--;
	
	camera->SetParentWorld( NULL );
	camera->SetLLWorldPrev( NULL );
	camera->SetLLWorldNext( NULL );
	if( pPeerGraphic ){
		pPeerGraphic->CameraRemoved( camera );
	}
	camera->FreeReference();
}

void deWorld::RemoveAllCameras(){
	if( pPeerGraphic ){
		pPeerGraphic->AllCamerasRemoved();
	}
	
	while( pCameraTail ){
		deCamera * const next = pCameraTail->GetLLWorldPrev();
		pCameraTail->SetParentWorld( NULL );
		pCameraTail->FreeReference();
		pCameraTail = next;
		pCameraCount--;
	}
	pCameraRoot = NULL;
}



// Colliders
//////////////

void deWorld::AddCollider( deCollider *collider ){
	if( ! collider || collider->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pColliderTail ){
		pColliderTail->SetLLWorldNext( collider );
		collider->SetLLWorldPrev( pColliderTail );
		collider->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		collider->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		collider->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pColliderRoot = collider;
	}
	
	pColliderTail = collider;
	pColliderCount++;
	collider->SetParentWorld( this );
	collider->AddReference();
	
	if( pPeerPhysics ){
		pPeerPhysics->ColliderAdded( collider );
	}
}

void deWorld::RemoveCollider( deCollider *collider ){
	if( ! collider || collider->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( collider->GetLLWorldPrev() ){
		collider->GetLLWorldPrev()->SetLLWorldNext( collider->GetLLWorldNext() );
	}
	if( collider->GetLLWorldNext() ){
		collider->GetLLWorldNext()->SetLLWorldPrev( collider->GetLLWorldPrev() );
	}
	if( collider == pColliderRoot ){
		pColliderRoot = collider->GetLLWorldNext();
	}
	if( collider == pColliderTail ){
		pColliderTail = collider->GetLLWorldPrev();
	}
	pColliderCount--;
	
	collider->SetParentWorld( NULL );
	collider->SetLLWorldPrev( NULL );
	collider->SetLLWorldNext( NULL );
	if( pPeerPhysics ){
		pPeerPhysics->ColliderRemoved( collider );
	}
	collider->FreeReference();
}

void deWorld::RemoveAllColliders(){
	if( pPeerPhysics ){
		pPeerPhysics->AllCollidersRemoved();
	}
	
	while( pColliderTail ){
		deCollider * const next = pColliderTail->GetLLWorldPrev();
		pColliderTail->SetParentWorld( NULL );
		pColliderTail->FreeReference();
		pColliderTail = next;
		pColliderCount--;
	}
	pColliderRoot = NULL;
}



// Components
///////////////

void deWorld::AddComponent( deComponent *component ){
	if( ! component || component->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pComponentTail ){
		pComponentTail->SetLLWorldNext( component );
		component->SetLLWorldPrev( pComponentTail );
		component->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		component->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		component->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pComponentRoot = component;
	}
	
	pComponentTail = component;
	pComponentCount++;
	component->SetParentWorld( this );
	component->AddReference();
	
	if( pPeerGraphic ){
		pPeerGraphic->ComponentAdded( component );
	}
	if( pPeerPhysics ){
		pPeerPhysics->ComponentAdded( component );
	}
	if( pPeerAudio ){
		pPeerAudio->ComponentAdded( component );
	}
}

void deWorld::RemoveComponent( deComponent *component ){
	if( ! component || component->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( component->GetLLWorldPrev() ){
		component->GetLLWorldPrev()->SetLLWorldNext( component->GetLLWorldNext() );
	}
	if( component->GetLLWorldNext() ){
		component->GetLLWorldNext()->SetLLWorldPrev( component->GetLLWorldPrev() );
	}
	if( component == pComponentRoot ){
		pComponentRoot = component->GetLLWorldNext();
	}
	if( component == pComponentTail ){
		pComponentTail = component->GetLLWorldPrev();
	}
	pComponentCount--;
	
	component->SetParentWorld( NULL );
	component->SetLLWorldPrev( NULL );
	component->SetLLWorldNext( NULL );
	if( pPeerGraphic ){
		pPeerGraphic->ComponentRemoved( component );
	}
	if( pPeerPhysics ){
		pPeerPhysics->ComponentRemoved( component );
	}
	if( pPeerAudio ){
		pPeerAudio->ComponentRemoved( component );
	}
	component->FreeReference();
}

void deWorld::RemoveAllComponents(){
	if( pPeerGraphic ){
		pPeerGraphic->AllComponentsRemoved();
	}
	if( pPeerPhysics ){
		pPeerPhysics->AllComponentsRemoved();
	}
	if( pPeerAudio ){
		pPeerAudio->AllComponentsRemoved();
	}
	
	while( pComponentTail ){
		deComponent * const next = pComponentTail->GetLLWorldPrev();
		pComponentTail->SetParentWorld( NULL );
		pComponentTail->FreeReference();
		pComponentTail = next;
		pComponentCount--;
	}
	pComponentRoot = NULL;
}



// DebugDrawers
/////////////////

void deWorld::AddDebugDrawer( deDebugDrawer *debugDrawer ){
	if( ! debugDrawer || debugDrawer->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDebugDrawerTail ){
		pDebugDrawerTail->SetLLWorldNext( debugDrawer );
		debugDrawer->SetLLWorldPrev( pDebugDrawerTail );
		debugDrawer->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		debugDrawer->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		debugDrawer->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pDebugDrawerRoot = debugDrawer;
	}
	
	pDebugDrawerTail = debugDrawer;
	pDebugDrawerCount++;
	debugDrawer->SetParentWorld( this );
	debugDrawer->AddReference();
	
	if( pPeerGraphic ){
		pPeerGraphic->DebugDrawerAdded( debugDrawer );
	}
}

void deWorld::RemoveDebugDrawer( deDebugDrawer *debugDrawer ){
	if( ! debugDrawer || debugDrawer->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( debugDrawer->GetLLWorldPrev() ){
		debugDrawer->GetLLWorldPrev()->SetLLWorldNext( debugDrawer->GetLLWorldNext() );
	}
	if( debugDrawer->GetLLWorldNext() ){
		debugDrawer->GetLLWorldNext()->SetLLWorldPrev( debugDrawer->GetLLWorldPrev() );
	}
	if( debugDrawer == pDebugDrawerRoot ){
		pDebugDrawerRoot = debugDrawer->GetLLWorldNext();
	}
	if( debugDrawer == pDebugDrawerTail ){
		pDebugDrawerTail = debugDrawer->GetLLWorldPrev();
	}
	pDebugDrawerCount--;
	
	debugDrawer->SetParentWorld( NULL );
	debugDrawer->SetLLWorldPrev( NULL );
	debugDrawer->SetLLWorldNext( NULL );
	if( pPeerGraphic ){
		pPeerGraphic->DebugDrawerRemoved( debugDrawer );
	}
	debugDrawer->FreeReference();
}

void deWorld::RemoveAllDebugDrawers(){
	if( pPeerGraphic ){
		pPeerGraphic->AllDebugDrawersRemoved();
	}
	
	while( pDebugDrawerTail ){
		deDebugDrawer * const next = pDebugDrawerTail->GetLLWorldPrev();
		pDebugDrawerTail->SetParentWorld( NULL );
		pDebugDrawerTail->FreeReference();
		pDebugDrawerTail = next;
		pDebugDrawerCount--;
	}
	pDebugDrawerRoot = NULL;
}



// Environment Map Probes
///////////////////////////

void deWorld::AddEnvMapProbe( deEnvMapProbe *envMapProbe ){
	if( ! envMapProbe || envMapProbe->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pEnvMapProbeTail ){
		pEnvMapProbeTail->SetLLWorldNext( envMapProbe );
		envMapProbe->SetLLWorldPrev( pEnvMapProbeTail );
		envMapProbe->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		envMapProbe->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		envMapProbe->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pEnvMapProbeRoot = envMapProbe;
	}
	
	pEnvMapProbeTail = envMapProbe;
	pEnvMapProbeCount++;
	envMapProbe->SetParentWorld( this );
	envMapProbe->AddReference();
	
	if( pPeerGraphic ){
		pPeerGraphic->EnvMapProbeAdded( envMapProbe );
	}
}

void deWorld::RemoveEnvMapProbe( deEnvMapProbe *envMapProbe ){
	if( ! envMapProbe || envMapProbe->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( envMapProbe->GetLLWorldPrev() ){
		envMapProbe->GetLLWorldPrev()->SetLLWorldNext( envMapProbe->GetLLWorldNext() );
	}
	if( envMapProbe->GetLLWorldNext() ){
		envMapProbe->GetLLWorldNext()->SetLLWorldPrev( envMapProbe->GetLLWorldPrev() );
	}
	if( envMapProbe == pEnvMapProbeRoot ){
		pEnvMapProbeRoot = envMapProbe->GetLLWorldNext();
	}
	if( envMapProbe == pEnvMapProbeTail ){
		pEnvMapProbeTail = envMapProbe->GetLLWorldPrev();
	}
	pEnvMapProbeCount--;
	
	envMapProbe->SetParentWorld( NULL );
	envMapProbe->SetLLWorldPrev( NULL );
	envMapProbe->SetLLWorldNext( NULL );
	if( pPeerGraphic ){
		pPeerGraphic->EnvMapProbeRemoved( envMapProbe );
	}
	envMapProbe->FreeReference();
}

void deWorld::RemoveAllEnvMapProbes(){
	if( pPeerGraphic ){
		pPeerGraphic->AllEnvMapProbesRemoved();
	}
	
	while( pEnvMapProbeTail ){
		deEnvMapProbe * const next = pEnvMapProbeTail->GetLLWorldPrev();
		pEnvMapProbeTail->SetParentWorld( NULL );
		pEnvMapProbeTail->FreeReference();
		pEnvMapProbeTail = next;
		pEnvMapProbeCount--;
	}
	pEnvMapProbeRoot = NULL;
}



// Force fields
/////////////////

void deWorld::AddForceField( deForceField *forceField ){
	if( ! forceField || forceField->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pForceFieldTail ){
		pForceFieldTail->SetLLWorldNext( forceField );
		forceField->SetLLWorldPrev( pForceFieldTail );
		forceField->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		forceField->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		forceField->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pForceFieldRoot = forceField;
	}
	
	pForceFieldTail = forceField;
	pForceFieldCount++;
	forceField->SetParentWorld( this );
	forceField->AddReference();
	
	if( pPeerPhysics ){
		pPeerPhysics->ForceFieldAdded( forceField );
	}
}

void deWorld::RemoveForceField( deForceField *forceField ){
	if( ! forceField || forceField->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( forceField->GetLLWorldPrev() ){
		forceField->GetLLWorldPrev()->SetLLWorldNext( forceField->GetLLWorldNext() );
	}
	if( forceField->GetLLWorldNext() ){
		forceField->GetLLWorldNext()->SetLLWorldPrev( forceField->GetLLWorldPrev() );
	}
	if( forceField == pForceFieldRoot ){
		pForceFieldRoot = forceField->GetLLWorldNext();
	}
	if( forceField == pForceFieldTail ){
		pForceFieldTail = forceField->GetLLWorldPrev();
	}
	pForceFieldCount--;
	
	forceField->SetParentWorld( NULL );
	forceField->SetLLWorldPrev( NULL );
	forceField->SetLLWorldNext( NULL );
	if( pPeerPhysics ){
		pPeerPhysics->ForceFieldRemoved( forceField );
	}
	forceField->FreeReference();
}

void deWorld::RemoveAllForceFields(){
	if( pPeerPhysics ){
		pPeerPhysics->AllForceFieldsRemoved();
	}
	
	while( pForceFieldTail ){
		deForceField * const next = pForceFieldTail->GetLLWorldPrev();
		pForceFieldTail->SetParentWorld( NULL );
		pForceFieldTail->FreeReference();
		pForceFieldTail = next;
		pForceFieldCount--;
	}
	pForceFieldRoot = NULL;
}



// Lights
////////////

void deWorld::AddLight( deLight *light ){
	if( ! light || light->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLightTail ){
		pLightTail->SetLLWorldNext( light );
		light->SetLLWorldPrev( pLightTail );
		light->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		light->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		light->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pLightRoot = light;
	}
	
	pLightTail = light;
	pLightCount++;
	light->SetParentWorld( this );
	light->AddReference();
	
	if( pPeerGraphic ){
		pPeerGraphic->LightAdded( light );
	}
}

void deWorld::RemoveLight( deLight *light ){
	if( ! light || light->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( light->GetLLWorldPrev() ){
		light->GetLLWorldPrev()->SetLLWorldNext( light->GetLLWorldNext() );
	}
	if( light->GetLLWorldNext() ){
		light->GetLLWorldNext()->SetLLWorldPrev( light->GetLLWorldPrev() );
	}
	if( light == pLightRoot ){
		pLightRoot = light->GetLLWorldNext();
	}
	if( light == pLightTail ){
		pLightTail = light->GetLLWorldPrev();
	}
	pLightCount--;
	
	light->SetParentWorld( NULL );
	light->SetLLWorldPrev( NULL );
	light->SetLLWorldNext( NULL );
	if( pPeerGraphic ){
		pPeerGraphic->LightRemoved( light );
	}
	light->FreeReference();
}

void deWorld::RemoveAllLights(){
	if( pPeerGraphic ){
		pPeerGraphic->AllLightsRemoved();
	}
	
	while( pLightTail ){
		deLight * const next = pLightTail->GetLLWorldPrev();
		pLightTail->SetParentWorld( NULL );
		pLightTail->FreeReference();
		pLightTail = next;
		pLightCount--;
	}
	pLightRoot = NULL;
}



// Lumimeters
////////////

void deWorld::AddLumimeter( deLumimeter *lumimeter ){
	if( ! lumimeter || lumimeter->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLumimeterTail ){
		pLumimeterTail->SetLLWorldNext( lumimeter );
		lumimeter->SetLLWorldPrev( pLumimeterTail );
		lumimeter->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		lumimeter->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		lumimeter->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pLumimeterRoot = lumimeter;
	}
	
	pLumimeterTail = lumimeter;
	pLumimeterCount++;
	lumimeter->SetParentWorld( this );
	lumimeter->AddReference();
	
	if( pPeerGraphic ){
		pPeerGraphic->LumimeterAdded( lumimeter );
	}
}

void deWorld::RemoveLumimeter( deLumimeter *lumimeter ){
	if( ! lumimeter || lumimeter->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( lumimeter->GetLLWorldPrev() ){
		lumimeter->GetLLWorldPrev()->SetLLWorldNext( lumimeter->GetLLWorldNext() );
	}
	if( lumimeter->GetLLWorldNext() ){
		lumimeter->GetLLWorldNext()->SetLLWorldPrev( lumimeter->GetLLWorldPrev() );
	}
	if( lumimeter == pLumimeterRoot ){
		pLumimeterRoot = lumimeter->GetLLWorldNext();
	}
	if( lumimeter == pLumimeterTail ){
		pLumimeterTail = lumimeter->GetLLWorldPrev();
	}
	pLumimeterCount--;
	
	lumimeter->SetParentWorld( NULL );
	lumimeter->SetLLWorldPrev( NULL );
	lumimeter->SetLLWorldNext( NULL );
	if( pPeerGraphic ){
		pPeerGraphic->LumimeterRemoved( lumimeter );
	}
	lumimeter->FreeReference();
}

void deWorld::RemoveAllLumimeters(){
	if( pPeerGraphic ){
		pPeerGraphic->AllLumimetersRemoved();
	}
	
	while( pLumimeterTail ){
		deLumimeter * const next = pLumimeterTail->GetLLWorldPrev();
		pLumimeterTail->SetParentWorld( NULL );
		pLumimeterTail->FreeReference();
		pLumimeterTail = next;
		pLumimeterCount--;
	}
	pLumimeterRoot = NULL;
}



// Microphones
////////////

void deWorld::AddMicrophone( deMicrophone *microphone ){
	if( ! microphone || microphone->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pMicrophoneTail ){
		pMicrophoneTail->SetLLWorldNext( microphone );
		microphone->SetLLWorldPrev( pMicrophoneTail );
		microphone->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		microphone->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		microphone->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pMicrophoneRoot = microphone;
	}
	
	pMicrophoneTail = microphone;
	pMicrophoneCount++;
	microphone->SetParentWorld( this );
	microphone->AddReference();
	
	if( pPeerAudio ){
		pPeerAudio->MicrophoneAdded( microphone );
	}
}

void deWorld::RemoveMicrophone( deMicrophone *microphone ){
	if( ! microphone || microphone->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( microphone->GetLLWorldPrev() ){
		microphone->GetLLWorldPrev()->SetLLWorldNext( microphone->GetLLWorldNext() );
	}
	if( microphone->GetLLWorldNext() ){
		microphone->GetLLWorldNext()->SetLLWorldPrev( microphone->GetLLWorldPrev() );
	}
	if( microphone == pMicrophoneRoot ){
		pMicrophoneRoot = microphone->GetLLWorldNext();
	}
	if( microphone == pMicrophoneTail ){
		pMicrophoneTail = microphone->GetLLWorldPrev();
	}
	pMicrophoneCount--;
	
	microphone->SetParentWorld( NULL );
	microphone->SetLLWorldPrev( NULL );
	microphone->SetLLWorldNext( NULL );
	if( pPeerAudio ){
		pPeerAudio->MicrophoneRemoved( microphone );
	}
	microphone->FreeReference();
}

void deWorld::RemoveAllMicrophones(){
	if( pPeerAudio ){
		pPeerAudio->AllMicrophonesRemoved();
	}
	
	while( pMicrophoneTail ){
		deMicrophone * const next = pMicrophoneTail->GetLLWorldPrev();
		pMicrophoneTail->SetParentWorld( NULL );
		pMicrophoneTail->FreeReference();
		pMicrophoneTail = next;
		pMicrophoneCount--;
	}
	pMicrophoneRoot = NULL;
}



// Navigation Spaces
//////////////////////

void deWorld::AddNavigationSpace( deNavigationSpace *navspace ){
	if( ! navspace || navspace->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pNavSpaceTail ){
		pNavSpaceTail->SetLLWorldNext( navspace );
		navspace->SetLLWorldPrev( pNavSpaceTail );
		navspace->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		navspace->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		navspace->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pNavSpaceRoot = navspace;
	}
	
	pNavSpaceTail = navspace;
	pNavSpaceCount++;
	navspace->SetParentWorld( this );
	navspace->AddReference();
	
	if( pPeerAI ){
		pPeerAI->NavigationSpaceAdded( navspace );
	}
}

void deWorld::RemoveNavigationSpace( deNavigationSpace *navspace ){
	if( ! navspace || navspace->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( navspace->GetLLWorldPrev() ){
		navspace->GetLLWorldPrev()->SetLLWorldNext( navspace->GetLLWorldNext() );
	}
	if( navspace->GetLLWorldNext() ){
		navspace->GetLLWorldNext()->SetLLWorldPrev( navspace->GetLLWorldPrev() );
	}
	if( navspace == pNavSpaceRoot ){
		pNavSpaceRoot = navspace->GetLLWorldNext();
	}
	if( navspace == pNavSpaceTail ){
		pNavSpaceTail = navspace->GetLLWorldPrev();
	}
	pNavSpaceCount--;
	
	navspace->SetParentWorld( NULL );
	navspace->SetLLWorldPrev( NULL );
	navspace->SetLLWorldNext( NULL );
	if( pPeerAI ){
		pPeerAI->NavigationSpaceRemoved( navspace );
	}
	navspace->FreeReference();
}

void deWorld::RemoveAllNavigationSpaces(){
	if( pPeerAI ){
		pPeerAI->AllNavigationSpacesRemoved();
	}
	
	while( pNavSpaceTail ){
		deNavigationSpace * const next = pNavSpaceTail->GetLLWorldPrev();
		pNavSpaceTail->SetParentWorld( NULL );
		pNavSpaceTail->FreeReference();
		pNavSpaceTail = next;
		pNavSpaceCount--;
	}
	pNavSpaceRoot = NULL;
}



// Navigation Blockers
////////////////////////

void deWorld::AddNavigationBlocker( deNavigationBlocker *blocker ){
	if( ! blocker || blocker->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pNavBlockerTail ){
		pNavBlockerTail->SetLLWorldNext( blocker );
		blocker->SetLLWorldPrev( pNavBlockerTail );
		blocker->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		blocker->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		blocker->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pNavBlockerRoot = blocker;
	}
	
	pNavBlockerTail = blocker;
	pNavBlockerCount++;
	blocker->SetParentWorld( this );
	blocker->AddReference();
	
	if( pPeerAI ){
		pPeerAI->NavigationBlockerAdded( blocker );
	}
}

void deWorld::RemoveNavigationBlocker( deNavigationBlocker *blocker ){
	if( ! blocker || blocker->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( blocker->GetLLWorldPrev() ){
		blocker->GetLLWorldPrev()->SetLLWorldNext( blocker->GetLLWorldNext() );
	}
	if( blocker->GetLLWorldNext() ){
		blocker->GetLLWorldNext()->SetLLWorldPrev( blocker->GetLLWorldPrev() );
	}
	if( blocker == pNavBlockerRoot ){
		pNavBlockerRoot = blocker->GetLLWorldNext();
	}
	if( blocker == pNavBlockerTail ){
		pNavBlockerTail = blocker->GetLLWorldPrev();
	}
	pNavBlockerCount--;
	
	blocker->SetParentWorld( NULL );
	blocker->SetLLWorldPrev( NULL );
	blocker->SetLLWorldNext( NULL );
	if( pPeerAI ){
		pPeerAI->NavigationBlockerRemoved( blocker );
	}
    blocker->FreeReference();
}

void deWorld::RemoveAllNavigationBlockers(){
	if( pPeerAI ){
		pPeerAI->AllNavigationBlockersRemoved();
	}
	
	while( pNavBlockerTail ){
		deNavigationBlocker * const next = pNavBlockerTail->GetLLWorldPrev();
		pNavBlockerTail->SetParentWorld( NULL );
		pNavBlockerTail->FreeReference();
		pNavBlockerTail = next;
		pNavBlockerCount--;
	}
	pNavBlockerRoot = NULL;
}



// Navigators
///////////////

void deWorld::AddNavigator( deNavigator *navigator ){
	if( ! navigator || navigator->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pNavigatorTail ){
		pNavigatorTail->SetLLWorldNext( navigator );
		navigator->SetLLWorldPrev( pNavigatorTail );
		navigator->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		navigator->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		navigator->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pNavigatorRoot = navigator;
	}
	
	pNavigatorTail = navigator;
	pNavigatorCount++;
	navigator->SetParentWorld( this );
	navigator->AddReference();
	
	if( pPeerAI ){
		pPeerAI->NavigatorAdded( navigator );
	}
}

void deWorld::RemoveNavigator( deNavigator *navigator ){
	if( ! navigator || navigator->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( navigator->GetLLWorldPrev() ){
		navigator->GetLLWorldPrev()->SetLLWorldNext( navigator->GetLLWorldNext() );
	}
	if( navigator->GetLLWorldNext() ){
		navigator->GetLLWorldNext()->SetLLWorldPrev( navigator->GetLLWorldPrev() );
	}
	if( navigator == pNavigatorRoot ){
		pNavigatorRoot = navigator->GetLLWorldNext();
	}
	if( navigator == pNavigatorTail ){
		pNavigatorTail = navigator->GetLLWorldPrev();
	}
	pNavigatorCount--;
	
	navigator->SetParentWorld( NULL );
	navigator->SetLLWorldPrev( NULL );
	navigator->SetLLWorldNext( NULL );
	if( pPeerAI ){
		pPeerAI->NavigatorRemoved( navigator );
	}
	navigator->FreeReference();
}

void deWorld::RemoveAllNavigators(){
	if( pPeerAI ){
		pPeerAI->AllNavigatorsRemoved();
	}
	
	while( pNavigatorTail ){
		deNavigator * const next = pNavigatorTail->GetLLWorldPrev();
		pNavigatorTail->SetParentWorld( NULL );
		pNavigatorTail->FreeReference();
		pNavigatorTail = next;
		pNavigatorCount--;
	}
	pNavigatorRoot = NULL;
}



// Network states
///////////////////

void deWorld::AddNetworkState( deNetworkState *networkState ){
	if( ! networkState || networkState->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pNetworkStateTail ){
		pNetworkStateTail->SetLLWorldNext( networkState );
		networkState->SetLLWorldPrev( pNetworkStateTail );
		networkState->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		networkState->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		networkState->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pNetworkStateRoot = networkState;
	}
	
	pNetworkStateTail = networkState;
	pNetworkStateCount++;
	networkState->SetParentWorld( this );
	networkState->AddReference();
	
	if( pPeerNetwork ){
		pPeerNetwork->NetworkStateAdded( networkState );
	}
}

void deWorld::RemoveNetworkState( deNetworkState *networkState ){
	if( ! networkState || networkState->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( networkState->GetLLWorldPrev() ){
		networkState->GetLLWorldPrev()->SetLLWorldNext( networkState->GetLLWorldNext() );
	}
	if( networkState->GetLLWorldNext() ){
		networkState->GetLLWorldNext()->SetLLWorldPrev( networkState->GetLLWorldPrev() );
	}
	if( networkState == pNetworkStateRoot ){
		pNetworkStateRoot = networkState->GetLLWorldNext();
	}
	if( networkState == pNetworkStateTail ){
		pNetworkStateTail = networkState->GetLLWorldPrev();
	}
	pNetworkStateCount--;
	
	networkState->SetParentWorld( NULL );
	networkState->SetLLWorldPrev( NULL );
	networkState->SetLLWorldNext( NULL );
	if( pPeerNetwork ){
		pPeerNetwork->NetworkStateRemoved( networkState );
	}
	networkState->FreeReference();
}

void deWorld::RemoveAllNetworkStates(){
	if( pPeerNetwork ){
		pPeerNetwork->AllNetworkStatesRemoved();
	}
	
	while( pNetworkStateTail ){
		deNetworkState * const next = pNetworkStateTail->GetLLWorldPrev();
		pNetworkStateTail->SetParentWorld( NULL );
		pNetworkStateTail->FreeReference();
		pNetworkStateTail = next;
		pNetworkStateCount--;
	}
	pNetworkStateRoot = NULL;
}



// Particle Emitters
//////////////////////

void deWorld::AddParticleEmitter( deParticleEmitterInstance *emitter ){
	if( ! emitter || emitter->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pParticleEmitterTail ){
		pParticleEmitterTail->SetLLWorldNext( emitter );
		emitter->SetLLWorldPrev( pParticleEmitterTail );
		emitter->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		emitter->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		emitter->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pParticleEmitterRoot = emitter;
	}
	
	pParticleEmitterTail = emitter;
	pParticleEmitterCount++;
	emitter->SetParentWorld( this );
	emitter->AddReference();
	
	if( pPeerGraphic ){
		pPeerGraphic->ParticleEmitterAdded( emitter );
	}
	if( pPeerPhysics ){
		pPeerPhysics->ParticleEmitterAdded( emitter );
	}
}

void deWorld::RemoveParticleEmitter( deParticleEmitterInstance *emitter ){
	if( ! emitter || emitter->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( emitter->GetLLWorldPrev() ){
		emitter->GetLLWorldPrev()->SetLLWorldNext( emitter->GetLLWorldNext() );
	}
	if( emitter->GetLLWorldNext() ){
		emitter->GetLLWorldNext()->SetLLWorldPrev( emitter->GetLLWorldPrev() );
	}
	if( emitter == pParticleEmitterRoot ){
		pParticleEmitterRoot = emitter->GetLLWorldNext();
	}
	if( emitter == pParticleEmitterTail ){
		pParticleEmitterTail = emitter->GetLLWorldPrev();
	}
	pParticleEmitterCount--;
	
	emitter->SetParentWorld( NULL );
	emitter->SetLLWorldPrev( NULL );
	emitter->SetLLWorldNext( NULL );
	if( pPeerGraphic ){
		pPeerGraphic->ParticleEmitterRemoved( emitter );
	}
	if( pPeerPhysics ){
		pPeerPhysics->ParticleEmitterRemoved( emitter );
	}
	emitter->FreeReference();
}

void deWorld::RemoveAllParticleEmitters(){
	if( pPeerGraphic ){
		pPeerGraphic->AllParticleEmittersRemoved();
	}
	if( pPeerPhysics ){
		pPeerPhysics->AllParticleEmittersRemoved();
	}
	
	while( pParticleEmitterTail ){
		deParticleEmitterInstance * const next = pParticleEmitterTail->GetLLWorldPrev();
		pParticleEmitterTail->SetParentWorld( NULL );
		pParticleEmitterTail->FreeReference();
		pParticleEmitterTail = next;
		pParticleEmitterCount--;
	}
	pParticleEmitterRoot = NULL;
}



// PropFields
////////////

void deWorld::AddPropField( dePropField *propField ){
	if( ! propField || propField->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPropFieldTail ){
		pPropFieldTail->SetLLWorldNext( propField );
		propField->SetLLWorldPrev( pPropFieldTail );
		propField->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		propField->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		propField->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pPropFieldRoot = propField;
	}
	
	pPropFieldTail = propField;
	pPropFieldCount++;
	propField->SetParentWorld( this );
	propField->AddReference();
	
	if( pPeerGraphic ){
		pPeerGraphic->PropFieldAdded( propField );
	}
	if( pPeerPhysics ){
		pPeerPhysics->PropFieldAdded( propField );
	}
}

void deWorld::RemovePropField( dePropField *propField ){
	if( ! propField || propField->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( propField->GetLLWorldPrev() ){
		propField->GetLLWorldPrev()->SetLLWorldNext( propField->GetLLWorldNext() );
	}
	if( propField->GetLLWorldNext() ){
		propField->GetLLWorldNext()->SetLLWorldPrev( propField->GetLLWorldPrev() );
	}
	if( propField == pPropFieldRoot ){
		pPropFieldRoot = propField->GetLLWorldNext();
	}
	if( propField == pPropFieldTail ){
		pPropFieldTail = propField->GetLLWorldPrev();
	}
	pPropFieldCount--;
	
	propField->SetParentWorld( NULL );
	propField->SetLLWorldPrev( NULL );
	propField->SetLLWorldNext( NULL );
	if( pPeerGraphic ){
		pPeerGraphic->PropFieldRemoved( propField );
	}
	if( pPeerPhysics ){
		pPeerPhysics->PropFieldRemoved( propField );
	}
	propField->FreeReference();
}

void deWorld::RemoveAllPropFields(){
	if( pPeerGraphic ){
		pPeerGraphic->AllPropFieldsRemoved();
	}
	if( pPeerPhysics ){
		pPeerPhysics->AllPropFieldsRemoved();
	}
	
	while( pPropFieldTail ){
		dePropField * const next = pPropFieldTail->GetLLWorldPrev();
		pPropFieldTail->SetParentWorld( NULL );
		pPropFieldTail->FreeReference();
		pPropFieldTail = next;
		pPropFieldCount--;
	}
	pPropFieldRoot = NULL;
}



// Smoke emitters
///////////////////

void deWorld::AddSmokeEmitter( deSmokeEmitter *smokeEmitter ){
	if( ! smokeEmitter || smokeEmitter->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pSmokeEmitterTail ){
		pSmokeEmitterTail->SetLLWorldNext( smokeEmitter );
		smokeEmitter->SetLLWorldPrev( pSmokeEmitterTail );
		smokeEmitter->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		smokeEmitter->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		smokeEmitter->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pSmokeEmitterRoot = smokeEmitter;
	}
	
	pSmokeEmitterTail = smokeEmitter;
	pSmokeEmitterCount++;
	smokeEmitter->SetParentWorld( this );
	smokeEmitter->AddReference();
	
	if( pPeerGraphic ){
		pPeerGraphic->SmokeEmitterAdded( smokeEmitter );
	}
	if( pPeerPhysics ){
		pPeerPhysics->SmokeEmitterAdded( smokeEmitter );
	}
}

void deWorld::RemoveSmokeEmitter( deSmokeEmitter *smokeEmitter ){
	if( ! smokeEmitter || smokeEmitter->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( smokeEmitter->GetLLWorldPrev() ){
		smokeEmitter->GetLLWorldPrev()->SetLLWorldNext( smokeEmitter->GetLLWorldNext() );
	}
	if( smokeEmitter->GetLLWorldNext() ){
		smokeEmitter->GetLLWorldNext()->SetLLWorldPrev( smokeEmitter->GetLLWorldPrev() );
	}
	if( smokeEmitter == pSmokeEmitterRoot ){
		pSmokeEmitterRoot = smokeEmitter->GetLLWorldNext();
	}
	if( smokeEmitter == pSmokeEmitterTail ){
		pSmokeEmitterTail = smokeEmitter->GetLLWorldPrev();
	}
	pSmokeEmitterCount--;
	
	smokeEmitter->SetParentWorld( NULL );
	smokeEmitter->SetLLWorldPrev( NULL );
	smokeEmitter->SetLLWorldNext( NULL );
	if( pPeerGraphic ){
		pPeerGraphic->SmokeEmitterRemoved( smokeEmitter );
	}
	if( pPeerPhysics ){
		pPeerPhysics->SmokeEmitterRemoved( smokeEmitter );
	}
	smokeEmitter->FreeReference();
}

void deWorld::RemoveAllSmokeEmitters(){
	if( pPeerGraphic ){
		pPeerGraphic->AllSmokeEmittersRemoved();
	}
	if( pPeerPhysics ){
		pPeerPhysics->AllSmokeEmittersRemoved();
	}
	
	while( pSmokeEmitterTail ){
		deSmokeEmitter * const next = pSmokeEmitterTail->GetLLWorldPrev();
		pSmokeEmitterTail->SetParentWorld( NULL );
		pSmokeEmitterTail->FreeReference();
		pSmokeEmitterTail = next;
		pSmokeEmitterCount--;
	}
	pSmokeEmitterRoot = NULL;
}



// Collision Detection
////////////////////////

void deWorld::PointHits( const decDVector &point, deBaseScriptingCollider *listener,
const decCollisionFilter &collisionFilter ){
	if( pPeerPhysics ){
		pPeerPhysics->PointHits( point, listener, collisionFilter );
	}
}

void deWorld::RayHits( const decDVector &rayOrigin, const decVector &rayDirection,
deBaseScriptingCollider *listener, const decCollisionFilter &collisionFilter ){
	if( pPeerPhysics ){
		pPeerPhysics->RayHits( rayOrigin, rayDirection, listener, collisionFilter );
	}
}

void deWorld::ColliderHits( deCollider *collider, deBaseScriptingCollider *listener ){
	if( pPeerPhysics ){
		pPeerPhysics->ColliderHits( collider, listener );
	}
}

void deWorld::ColliderMoveHits( deCollider *collider, const decVector &displacement, deBaseScriptingCollider *listener ){
	if( pPeerPhysics ){
		pPeerPhysics->ColliderMoveHits( collider, displacement, listener );
	}
}

void deWorld::ColliderRotateHits( deCollider *collider, const decVector &rotation, deBaseScriptingCollider *listener ){
	if( pPeerPhysics ){
		pPeerPhysics->ColliderRotateHits( collider, rotation, listener );
	}
}

void deWorld::ColliderMoveRotateHits( deCollider *collider, const decVector &displacement,
const decVector &rotation, deBaseScriptingCollider *listener ){
	if( pPeerPhysics ){
		pPeerPhysics->ColliderMoveRotateHits( collider, displacement, rotation, listener );
	}
}



// Speakers
////////////

void deWorld::AddSpeaker( deSpeaker *speaker ){
	if( ! speaker || speaker->GetParentWorld() || speaker->GetParentMicrophone() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pSpeakerTail ){
		pSpeakerTail->SetLLWorldNext( speaker );
		speaker->SetLLWorldPrev( pSpeakerTail );
		speaker->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		speaker->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		speaker->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pSpeakerRoot = speaker;
	}
	
	pSpeakerTail = speaker;
	pSpeakerCount++;
	speaker->SetParentWorld( this );
	speaker->AddReference();
	
	if( pPeerAudio ){
		pPeerAudio->SpeakerAdded( speaker );
	}
}

void deWorld::RemoveSpeaker( deSpeaker *speaker ){
	if( ! speaker || speaker->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( speaker->GetLLWorldPrev() ){
		speaker->GetLLWorldPrev()->SetLLWorldNext( speaker->GetLLWorldNext() );
	}
	if( speaker->GetLLWorldNext() ){
		speaker->GetLLWorldNext()->SetLLWorldPrev( speaker->GetLLWorldPrev() );
	}
	if( speaker == pSpeakerRoot ){
		pSpeakerRoot = speaker->GetLLWorldNext();
	}
	if( speaker == pSpeakerTail ){
		pSpeakerTail = speaker->GetLLWorldPrev();
	}
	pSpeakerCount--;
	
	speaker->SetParentWorld( NULL );
	speaker->SetLLWorldPrev( NULL );
	speaker->SetLLWorldNext( NULL );
	if( pPeerAudio ){
		pPeerAudio->SpeakerRemoved( speaker );
	}
	speaker->FreeReference();
}

void deWorld::RemoveAllSpeakers(){
	if( pPeerAudio ){
		pPeerAudio->AllSpeakersRemoved();
	}
	
	while( pSpeakerTail ){
		deSpeaker * const next = pSpeakerTail->GetLLWorldPrev();
		pSpeakerTail->SetParentWorld( NULL );
		pSpeakerTail->FreeReference();
		pSpeakerTail = next;
		pSpeakerCount--;
	}
	pSpeakerRoot = NULL;
}



// Sound level meters
///////////////////////

void deWorld::AddSoundLevelMeter( deSoundLevelMeter *soundLevelMeter ){
	if( ! soundLevelMeter || soundLevelMeter->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pSoundLevelMeterTail ){
		pSoundLevelMeterTail->SetLLWorldNext( soundLevelMeter );
		soundLevelMeter->SetLLWorldPrev( pSoundLevelMeterTail );
		soundLevelMeter->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		soundLevelMeter->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		soundLevelMeter->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pSoundLevelMeterRoot = soundLevelMeter;
	}
	
	pSoundLevelMeterTail = soundLevelMeter;
	pSoundLevelMeterCount++;
	soundLevelMeter->SetParentWorld( this );
	soundLevelMeter->AddReference();
	
	if( pPeerAudio ){
		pPeerAudio->SoundLevelMeterAdded( soundLevelMeter );
	}
}

void deWorld::RemoveSoundLevelMeter( deSoundLevelMeter *soundLevelMeter ){
	if( ! soundLevelMeter || soundLevelMeter->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( soundLevelMeter->GetLLWorldPrev() ){
		soundLevelMeter->GetLLWorldPrev()->SetLLWorldNext( soundLevelMeter->GetLLWorldNext() );
	}
	if( soundLevelMeter->GetLLWorldNext() ){
		soundLevelMeter->GetLLWorldNext()->SetLLWorldPrev( soundLevelMeter->GetLLWorldPrev() );
	}
	if( soundLevelMeter == pSoundLevelMeterRoot ){
		pSoundLevelMeterRoot = soundLevelMeter->GetLLWorldNext();
	}
	if( soundLevelMeter == pSoundLevelMeterTail ){
		pSoundLevelMeterTail = soundLevelMeter->GetLLWorldPrev();
	}
	pSoundLevelMeterCount--;
	
	soundLevelMeter->SetParentWorld( NULL );
	soundLevelMeter->SetLLWorldPrev( NULL );
	soundLevelMeter->SetLLWorldNext( NULL );
	if( pPeerAudio ){
		pPeerAudio->SoundLevelMeterRemoved( soundLevelMeter );
	}
	soundLevelMeter->FreeReference();
}

void deWorld::RemoveAllSoundLevelMeters(){
	if( pPeerAudio ){
		pPeerAudio->AllSoundLevelMetersRemoved();
	}
	
	while( pSoundLevelMeterTail ){
		deSoundLevelMeter * const next = pSoundLevelMeterTail->GetLLWorldPrev();
		pSoundLevelMeterTail->SetParentWorld( NULL );
		pSoundLevelMeterTail->FreeReference();
		pSoundLevelMeterTail = next;
		pSoundLevelMeterCount--;
	}
	pSoundLevelMeterRoot = NULL;
}



// Touch sensors
//////////////////

void deWorld::AddTouchSensor( deTouchSensor *touchSensor ){
	if( ! touchSensor || touchSensor->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTouchSensorTail ){
		pTouchSensorTail->SetLLWorldNext( touchSensor );
		touchSensor->SetLLWorldPrev( pTouchSensorTail );
		touchSensor->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		touchSensor->SetLLWorldPrev( NULL ); // not required by definition, just to make sure...
		touchSensor->SetLLWorldNext( NULL ); // not required by definition, just to make sure...
		pTouchSensorRoot = touchSensor;
	}
	
	pTouchSensorTail = touchSensor;
	pTouchSensorCount++;
	touchSensor->SetParentWorld( this );
	touchSensor->AddReference();
	
	if( pPeerPhysics ){
		pPeerPhysics->TouchSensorAdded( touchSensor );
	}
}

void deWorld::RemoveTouchSensor( deTouchSensor *touchSensor ){
	if( ! touchSensor || touchSensor->GetParentWorld() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( touchSensor->GetLLWorldPrev() ){
		touchSensor->GetLLWorldPrev()->SetLLWorldNext( touchSensor->GetLLWorldNext() );
	}
	if( touchSensor->GetLLWorldNext() ){
		touchSensor->GetLLWorldNext()->SetLLWorldPrev( touchSensor->GetLLWorldPrev() );
	}
	if( touchSensor == pTouchSensorRoot ){
		pTouchSensorRoot = touchSensor->GetLLWorldNext();
	}
	if( touchSensor == pTouchSensorTail ){
		pTouchSensorTail = touchSensor->GetLLWorldPrev();
	}
	pTouchSensorCount--;
	
	touchSensor->SetParentWorld( NULL );
	touchSensor->SetLLWorldPrev( NULL );
	touchSensor->SetLLWorldNext( NULL );
	if( pPeerPhysics ){
		pPeerPhysics->TouchSensorRemoved( touchSensor );
	}
	touchSensor->FreeReference();
}

void deWorld::RemoveAllTouchSensors(){
	if( pPeerPhysics ){
		pPeerPhysics->AllTouchSensorsRemoved();
	}
	
	while( pTouchSensorTail ){
		deTouchSensor * const next = pTouchSensorTail->GetLLWorldPrev();
		pTouchSensorTail->SetParentWorld( NULL );
		pTouchSensorTail->FreeReference();
		pTouchSensorTail = next;
		pTouchSensorCount--;
	}
	pTouchSensorRoot = NULL;
}



// Peers
//////////

void deWorld::SetPeerGraphic( deBaseGraphicWorld *peer ){
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}

void deWorld::SetPeerPhysics( deBasePhysicsWorld *peer ){
	if( pPeerPhysics ){
		delete pPeerPhysics;
	}
	pPeerPhysics = peer;
}

void deWorld::SetPeerAudio( deBaseAudioWorld *peer ){
	if( pPeerAudio ){
		delete pPeerAudio;
	}
	pPeerAudio = peer;
}

void deWorld::SetPeerNetwork( deBaseNetworkWorld *peer ){
	if( pPeerNetwork ){
		delete pPeerNetwork;
	}
	pPeerNetwork = peer;
}

void deWorld::SetPeerAI( deBaseAIWorld *peer ){
	if( pPeerAI ){
		delete pPeerAI;
	}
	pPeerAI = peer;
}



// private function
/////////////////////

void deWorld::pCleanUp(){
	if( pPeerAI ){
		delete pPeerAI;
		pPeerAI = NULL;
	}
	if( pPeerNetwork ){
		delete pPeerNetwork;
		pPeerNetwork = NULL;
	}
	if( pPeerPhysics ){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
	if( pPeerAudio ){
		delete pPeerAudio;
		pPeerAudio = NULL;
	}
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	Clear();
	
	if( pHeightTerrain ){
		pHeightTerrain->FreeReference();
	}
}



void deWorld::pNotifyPhysicsChanged(){
	if( pPeerPhysics ){
		pPeerPhysics->PhysicsChanged();
	}
}

void deWorld::pNotifyLightingChanged(){
	if( pPeerGraphic ){
		pPeerGraphic->LightingChanged();
	}
}
