/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "peeEmitter.h"
#include "peeEmitterListener.h"
#include "peeController.h"
#include "peeType.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterManager.h>
#include <dragengine/resources/particle/deParticleEmitterController.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceManager.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>



// Definitions
////////////////

#define LOGSOURCE "Particle Emitter Editor"



// Class peeEmitter
/////////////////////

// Constructor, destructor
////////////////////////////

peeEmitter::peeEmitter( igdeEnvironment *environment, peeLoadSaveSystem &loadSaveSystem ) :
igdeEditableEntity( environment ),
pLoadSaveSystem( loadSaveSystem )
{
	deEngine * const engine = GetEngine();
	
	pEngWorld = NULL;
	pCamera = NULL;
	
	pDDEmitter = NULL;
	
	pSky = NULL;
	pEnvObject = NULL;
	
	pEngEmitter = NULL;
	pEngEmitterInstance = NULL;
	
	pActiveController = NULL;
	pActiveType = NULL;
	
	pWarmUpTime = 0.0f;
	pBurstLifetime = 1.0f;
	pEmitBurst = false;
	pEnableCasting = true;
	
	pBurstInterval = 1.0f;
	
	try{
		SetFilePath( "new.depemit" );
		
		// create world
		pEngWorld = engine->GetWorldManager()->CreateWorld();
		pEngWorld->SetGravity( decVector( 0.0f, -9.81f, 0.0f ) );
		pEngWorld->SetDisableLights( false );
		pEngWorld->SetAmbientLight( decColor( 0.0f, 0.0f, 0.0f ) );
		
		// create camera
		pCamera = new igdeCamera( engine );
		pCamera->SetEngineWorld( pEngWorld );
		pCamera->Reset();
		pCamera->SetPosition( decDVector( 0.0, 1.0, 0.0 ) );
		pCamera->SetOrientation( decVector( 0.0f, 180.0f, 0.0f ) );
		pCamera->SetDistance( 5.0f );
		pCamera->SetHighestIntensity( 20.0f );
		pCamera->SetLowestIntensity( 18.0f );
		pCamera->SetAdaptionTime( 4.0f );
		
		// create sky
		pSky = new igdeWSky( *environment );
		pSky->SetGDDefaultSky();
		pSky->SetWorld( pEngWorld );
		
		// create the environment wrapper object
		pEnvObject = new igdeWObject( *environment );
		pEnvObject->SetWorld( pEngWorld );
		pEnvObject->SetPosition( decDVector( 0.0, 0.0, 0.0 ) );
		
		decLayerMask layerMask;
		layerMask.SetBit( peeEmitter::eclmObjects );
		pEnvObject->SetCollisionFilter( decCollisionFilter( layerMask ) );
		pEnvObject->SetCollisionFilterFallback( decCollisionFilter( layerMask ) );
		
		pEnvObject->SetGDClassName( "IGDETestTerrain" );
		
		// create particle emitter instance
		pEngEmitterInstance = engine->GetParticleEmitterInstanceManager()->CreateInstance();
		pEngWorld->AddParticleEmitter( pEngEmitterInstance );
		
		pEngEmitterInstance->SetPosition( decDVector( pPosition ) );
		pEngEmitterInstance->SetWarmUpTime( pWarmUpTime );
		pEngEmitterInstance->SetEnableCasting( pEnableCasting );
		
		layerMask.ClearMask();
		layerMask.SetBit( peeEmitter::eclmHeightTerrains );
		layerMask.SetBit( peeEmitter::eclmObjects );
		layerMask.SetBit( peeEmitter::eclmPropFields );
		layerMask.SetBit( peeEmitter::eclmWind );
		pEngEmitterInstance->SetCollisionFilter( decCollisionFilter( layerMask, layerMask ) );
		
		// create debug drawers
		pDDEmitter = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDDEmitter->SetXRay( true );
		pEngWorld->AddDebugDrawer( pDDEmitter );
		
		// create debug drawer shape
		pDDSEmitter.SetArrowLength( 0.25f );
		pDDSEmitter.SetArrowSize( 0.02f );
		pDDSEmitter.SetParentDebugDrawer( pDDEmitter );
		pDDSEmitter.SetVisible( true );
		
		// set default parameters
		SetPosition( decVector( 0.0f, 1.0f, 0.0f ) );
		SetOrientation( decVector( 0.0f, 0.0f, 0.0f ) ); // 90.0f, 0.0f, 0.0f
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	SetChanged( false );
	SetSaved( false );
}

peeEmitter::~peeEmitter(){
	pCleanUp();
}



// Management
///////////////

void peeEmitter::SetPosition( const decVector &position ){
	if( ! pPosition.IsEqualTo( position ) ){
		const decDVector dposition( position );
		
		pPosition = position;
		
		pEngEmitterInstance->SetPosition( dposition );
		pDDSEmitter.SetPosition( dposition );
		
		NotifyEmitterChanged();
	}
}

void peeEmitter::SetOrientation( const decVector &orientation ){
	if( ! pOrientation.IsEqualTo( orientation ) ){
		const decQuaternion &qorientation = decMatrix::CreateRotation( orientation * DEG2RAD ).ToQuaternion();
		
		pOrientation = orientation;
		
		pEngEmitterInstance->SetOrientation( qorientation );
		pDDSEmitter.SetOrientation( qorientation );
		
		NotifyEmitterChanged();
	}
}



void peeEmitter::SetWarmUpTime( float warmUpTime ){
	if( fabsf( warmUpTime - pWarmUpTime ) > FLOAT_SAFE_EPSILON ){
		pWarmUpTime = warmUpTime;
		pEngEmitterInstance->SetWarmUpTime( warmUpTime );
		NotifyEmitterChanged();
	}
}

void peeEmitter::SetBurstLifetime( float lifetime ){
	if( fabsf( lifetime - pBurstLifetime ) > FLOAT_SAFE_EPSILON ){
		pBurstLifetime = lifetime;
		FreeEmitter();
		NotifyEmitterChanged();
	}
}

void peeEmitter::SetEmitBurst( bool emitBurst ){
	if( emitBurst != pEmitBurst ){
		pEmitBurst = emitBurst;
		
		FreeEmitter();
		NotifyEmitterChanged();
		
		pEngEmitterInstance->SetEnableCasting( pEnableCasting );
	}
}

void peeEmitter::SetEnableCasting( bool enableCasting ){
	if( enableCasting != pEnableCasting ){
		pEnableCasting = enableCasting;
		
		NotifyEmitterChanged();
		
		if( enableCasting ){
			pBurstTimer = 0.0f;
			pEngEmitterInstance->ResetBurst();
		}
		
		pEngEmitterInstance->SetEnableCasting( enableCasting );
	}
}



void peeEmitter::SetBurstInterval( float interval ){
	if( fabs( interval - pBurstInterval ) > FLOAT_SAFE_EPSILON ){
		pBurstInterval = interval;
		NotifyViewChanged();
	}
}



void peeEmitter::Dispose(){
	RemoveAllTypes();
	RemoveAllControllers();
	
	GetUndoSystem()->RemoveAll();
}

void peeEmitter::Reset(){
}

void peeEmitter::Update( float elapsed ){
	if( ! pEngEmitter ){
		RebuildEmitter();
	}
	
	if( pEmitBurst ){
		float burstTime = pBurstLifetime + pBurstInterval;
		
		if( burstTime < 0.1f ){
			burstTime = 0.1f;
		}
		
		pBurstTimer += elapsed;
		
		if( pBurstTimer >= burstTime ){
			pBurstTimer = fmod( pBurstTimer, burstTime );
			pEngEmitterInstance->ResetBurst();
			pEngEmitterInstance->SetEnableCasting( pEnableCasting );
		}
	}
	
	pEnvObject->Update( elapsed );
	
	pEngWorld->Update( elapsed );
	pEngWorld->ProcessPhysics( elapsed );
}

void peeEmitter::FreeEmitter(){
	pEngEmitterInstance->SetEmitter( NULL );
	
	if( pEngEmitter ){
		pEngEmitter->FreeReference();
		pEngEmitter = NULL;
	}
}

void peeEmitter::RebuildEmitter(){
	deParticleEmitterController *engController = NULL;
	const int controllerCount = pControllerList.GetCount();
	const int typeCount = pTypeList.GetCount();
	int i;
	
	FreeEmitter();
	
	for( i=0; i<controllerCount; i++ ){
		pControllerList.GetAt( i )->SetEngineControllerIndex( i );
	}
	for( i=0; i<typeCount; i++ ){
		pTypeList.GetAt( i )->SetIndex( i );
	}
	
	pBurstTimer = 0.0f;
	
	try{
		pEngEmitter = GetEngine()->GetParticleEmitterManager()->CreateParticleEmitter();
		
		for( i=0; i<controllerCount; i++ ){
			engController = new deParticleEmitterController;
			pControllerList.GetAt( i )->UpdateEngineController( *engController );
			pEngEmitter->AddController( engController );
			engController = NULL;
		}
		
		pEngEmitter->SetBurstLifetime( pBurstLifetime );
		pEngEmitter->SetEmitBurst( pEmitBurst );
		
		pEngEmitter->SetTypeCount( typeCount );
		for( i=0; i<typeCount; i++ ){
			pTypeList.GetAt( i )->UpdateEngineType( pEngEmitter->GetTypeAt( i ) );
			pEngEmitter->NotifyTypeChangedAt( i );
		}
		
		pEngEmitterInstance->SetEmitter( pEngEmitter );
		
	}catch( const deException &e ){
		if( engController ){
			delete engController;
		}
		e.PrintError();
		FreeEmitter();
		return;
	}
}



// Controllers
////////////////

void peeEmitter::AddController( peeController *controller ){
	pControllerList.Add( controller );
	controller->SetEmitter( this );
	NotifyControllerStructureChanged();
	
	if( ! pActiveController ){
		SetActiveController( controller );
	}
}

void peeEmitter::InsertControllerAt( peeController *controller, int index ){
	pControllerList.InsertAt( controller, index );
	controller->SetEmitter( this );
	NotifyControllerStructureChanged();
	
	if( ! pActiveController ){
		SetActiveController( controller );
	}
}

void peeEmitter::MoveControllerTo( peeController *controller, int index ){
	pControllerList.MoveTo( controller, index );
	NotifyControllerStructureChanged();
}

void peeEmitter::RemoveController( peeController *controller ){
	if( ! controller || controller->GetEmitter() != this ) DETHROW( deeInvalidParam );
	
	if( controller->GetActive() ){
		if( pControllerList.GetCount() == 1 ){
			SetActiveController( NULL );
			
		}else{
			if( pControllerList.GetAt( 0 ) == controller ){
				SetActiveController( pControllerList.GetAt( 1 ) );
				
			}else{
				SetActiveController( pControllerList.GetAt( 0 ) );
			}
		}
	}
	
	controller->SetEmitter( NULL );
	pControllerList.Remove( controller );
	NotifyControllerStructureChanged();
}

void peeEmitter::RemoveAllControllers(){
	const int count = pControllerList.GetCount();
	int i;
	
	SetActiveController( NULL );
	
	for( i=0; i<count; i++ ){
		pControllerList.GetAt( i )->SetEmitter( NULL );
	}
	pControllerList.RemoveAll();
	NotifyControllerStructureChanged();
}

bool peeEmitter::HasActiveController() const{
	return pActiveController != NULL;
}

void peeEmitter::SetActiveController( peeController *controller ){
	if( controller != pActiveController ){
		if( pActiveController ){
			pActiveController->SetActive( false );
			pActiveController->FreeReference();
		}
		
		pActiveController = controller;
		
		if( controller ){
			controller->AddReference();
			controller->SetActive( true );
		}
		
		NotifyActiveControllerChanged();
	}
}



// Types
//////////

void peeEmitter::AddType( peeType *type ){
	FreeEmitter();
	
	pTypeList.Add( type );
	type->SetEmitter( this );
	NotifyTypeStructureChanged();
	
	if( ! pActiveType ){
		SetActiveType( type );
	}
}

void peeEmitter::InsertTypeAt( peeType *type, int index ){
	FreeEmitter();
	
	pTypeList.InsertAt( type, index );
	type->SetEmitter( this );
	NotifyTypeStructureChanged();
	
	if( ! pActiveType ){
		SetActiveType( type );
	}
}

void peeEmitter::MoveTypeTo( peeType *type, int index ){
	FreeEmitter();
	pTypeList.MoveTo( type, index );
	NotifyTypeStructureChanged();
}

void peeEmitter::RemoveType( peeType *type ){
	if( ! type || type->GetEmitter() != this ){
		DETHROW( deeInvalidParam );
	}
	
	FreeEmitter();
	
	if( type->GetActive() ){
		if( pTypeList.GetCount() == 1 ){
			SetActiveType( NULL );
			
		}else{
			if( pTypeList.GetAt( 0 ) == type ){
				SetActiveType( pTypeList.GetAt( 1 ) );
				
			}else{
				SetActiveType( pTypeList.GetAt( 0 ) );
			}
		}
	}
	
	type->SetEmitter( NULL );
	pTypeList.Remove( type );
	NotifyTypeStructureChanged();
}

void peeEmitter::RemoveAllTypes(){
	const int count = pTypeList.GetCount();
	int i;
	
	FreeEmitter();
	SetActiveType( NULL );
	
	for( i=0; i<count; i++ ){
		pTypeList.GetAt( i )->SetEmitter( NULL );
	}
	pTypeList.RemoveAll();
	NotifyTypeStructureChanged();
}

bool peeEmitter::HasActiveType() const{
	return pActiveType != NULL;
}

void peeEmitter::SetActiveType( peeType *type ){
	if( type != pActiveType ){
		if( pActiveType ){
			pActiveType->SetActive( false );
			pActiveType->FreeReference();
		}
		
		pActiveType = type;
		
		if( type ){
			type->AddReference();
			type->SetActive( true );
		}
		
		NotifyActiveTypeChanged();
	}
}



// Listeners
//////////////

void peeEmitter::AddListener( peeEmitterListener *listener ){
	if( ! listener ) DETHROW( deeInvalidParam );
	
	pListeners.Add( listener );
}

void peeEmitter::RemoveListener( peeEmitterListener *listener ){
	pListeners.RemoveIfPresent( listener );
}



void peeEmitter::NotifyStateChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->StateChanged( this );
	}
}

void peeEmitter::NotifyUndoChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->UndoChanged( this );
	}
}

void peeEmitter::NotifyEmitterChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->EmitterChanged( this );
	}
	
	SetChanged( true );
}

void peeEmitter::NotifySkyChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->SkyChanged( this );
	}
}

void peeEmitter::NotifyEnvObjectChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->EnvObjectChanged( this );
	}
}

void peeEmitter::NotifyViewChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->ViewChanged( this );
	}
}

void peeEmitter::NotifyCameraChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->CameraChanged( this );
	}
}



void peeEmitter::NotifyControllerStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	FreeEmitter();
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->ControllerStructureChanged( this );
	}
	
	SetChanged( true );
}

void peeEmitter::NotifyControllerNameChanged( peeController *controller ){
	const int count = pListeners.GetCount();
	int i;
	
	FreeEmitter();
	
	for( i=0; i<count; i++ ){
		( ( peeEmitterListener* )pListeners.GetAt( i ) )->ControllerNameChanged( this, controller );
	}
	
	SetChanged( true );
}

void peeEmitter::NotifyControllerChanged( peeController *controller ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->ControllerChanged( this, controller );
	}
	
	SetChanged( true );
}

void peeEmitter::NotifyControllerValueChanged( peeController *controller ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->ControllerValueChanged( this, controller );
	}
}

void peeEmitter::NotifyActiveControllerChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->ActiveControllerChanged( this );
	}
}



void peeEmitter::NotifyTypeStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	FreeEmitter();
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->TypeStructureChanged( this );
	}
	
	SetChanged( true );
}

void peeEmitter::NotifyTypeChanged( peeType *type ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	FreeEmitter();
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->TypeChanged( this, type );
	}
	
	SetChanged( true );
}

void peeEmitter::NotifyTypeActiveTrailControllerChanged( peeType *type ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->TypeActiveTrailControllerChanged( this, type );
	}
}

void peeEmitter::NotifyTypeActiveEmitControllerChanged( peeType *type ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->TypeActiveEmitControllerChanged( this, type );
	}
}

void peeEmitter::NotifyTypeParameterChanged( peeType *type, peeParameter *parameter ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->TypeParameterChanged( this, type, parameter );
	}
	
	SetChanged( true );
}

void peeEmitter::NotifyActiveTypeParameterChanged( peeType *type ){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->ActiveTypeParameterChanged( this, type );
	}
}

void peeEmitter::NotifyActiveTypeChanged(){
	const int listenerCount = pListeners.GetCount();
	int l;
	
	for( l=0; l<listenerCount; l++ ){
		( ( peeEmitterListener* )pListeners.GetAt( l ) )->ActiveTypeChanged( this );
	}
}



// Private Functions
//////////////////////

void peeEmitter::pCleanUp(){
	pDDSEmitter.SetParentDebugDrawer( NULL );
	
	if( pSky ){
		delete pSky;
	}
	if( pEnvObject ){
		delete pEnvObject;
	}
	
	pListeners.RemoveAll();
	
	if( pCamera ){
		delete pCamera;
	}
	
	RemoveAllTypes();
	RemoveAllControllers();
	
	if( pEngEmitterInstance ){
		pEngEmitterInstance->SetEmitter( NULL );
	}
	if( pEngEmitter ){
		pEngEmitter->FreeReference();
	}
	if( pEngWorld ){
		if( pDDEmitter ){
			pEngWorld->RemoveDebugDrawer( pDDEmitter );
			pDDEmitter->FreeReference();
		}
		
		if( pEngEmitterInstance ){
			pEngWorld->RemoveParticleEmitter( pEngEmitterInstance );
			pEngEmitterInstance->FreeReference();
		}
		pEngWorld->FreeReference();
	}
}
