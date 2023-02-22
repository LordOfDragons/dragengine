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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "peeEmitter.h"
#include "peeType.h"
#include "../gui/peeWindowMain.h"
#include "../loadsave/peeLoadSaveSystem.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/particleemitter/igdeGDParticleEmitter.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/model/deModelManager.h>



// Definitions
////////////////

#define LOGSOURCE "Particle Emitter Editor"



// Class peeType
//////////////////

// Constructor, destructor
////////////////////////////

peeType::peeType( deEngine *engine, const char *name ) :
pEmitter( NULL ),
pEngine( engine ),
pEngSkin( NULL ),
pEngCastModel( NULL ),
pEngCastSkin( NULL ),

pIndex( -1 ),
pName( name ),

pActiveParameter( NULL ),

pCastFrom( deParticleEmitterType::ecfVertex ),
pSimulationType( deParticleEmitterType::estParticle ),
pIntervalAsDistance( false ),

pEngTrailEmitter( NULL ),
pActiveTrailController( deParticleEmitterType::eecLifetime ),

pPhysicsSize( 0.1f ),
pCollisionResponse( deParticleEmitterType::ecrPhysical ),
pEngCollisionEmitter( NULL ),
pEmitMinImpulse( 0.01f ),
pActiveEmitController( deParticleEmitterType::eecLifetime ),

pActive( false ),
pDirty( true )
{
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<=deParticleEmitterType::epBeamEnd; i++ ){
		pParameters[ 0 ] = NULL;
	}
	
	try{
		pSetParameter( deParticleEmitterType::epTimeToLive, 1.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epInterval, 1.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epParticleCount, 1.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epSize, 1.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epCastAngleX, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epCastAngleY, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epRed, 1.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epGreen, 1.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epBlue, 1.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epTransparency, 1.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epEmissivity, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epMass, 0.1f, 0.0f );
		pSetParameter( deParticleEmitterType::epRotation, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epLinearVelocity, 1.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epAngularVelocity, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epBrownMotion, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epDamping, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epDrag, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epGravityX, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epGravityY, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epGravityZ, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epLocalGravity, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epForceFieldDirect, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epForceFieldSurface, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epForceFieldMass, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epForceFieldSpeed, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epElasticity, 1.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epRoughness, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epEmitDirection, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epBeamStart, 0.0f, 0.0f );
		pSetParameter( deParticleEmitterType::epBeamEnd, 0.0f, 0.0f );
		
		pActiveParameter = pParameters[ 0 ];
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

peeType::~peeType(){
	SetEmitter( NULL );
	pCleanUp();
}



// Management
///////////////

void peeType::SetEmitter( peeEmitter *emitter ){
	if( emitter == pEmitter ){
		return;
	}
	
	pEmitter = emitter;
	pLoadSkin();
	pLoadModel();
	pLoadModelSkin();
	pLoadCollisionEmitter();
	pLoadTrailEmitter();
}

void peeType::SetIndex( int index ){
	pIndex = index;
}



void peeType::SetName( const char *name ){
	if( pName.Equals( name ) ){
		return;
	}
	
	pName = name;
	NotifyChanged();
}

void peeType::SetSkinPath( const char *path ){
	if( pSkinPath.Equals( path ) ){
		return;
	}
	
	pSkinPath = path;
	
	pLoadSkin();
	NotifyChanged();
}

void peeType::SetModelPath( const char *path ){
	if( pModelPath.Equals( path ) ){
		return;
	}
	
	pModelPath = path;
	
	pLoadModel();
	NotifyChanged();
}

void peeType::SetModelSkinPath( const char *path ){
	if( pModelSkinPath.Equals( path ) ){
		return;
	}
	
	pModelSkinPath = path;
	
	pLoadModelSkin();
	NotifyChanged();
}

void peeType::SetCastFrom( deParticleEmitterType::eCastFrom castFrom ){
	if( castFrom < 0 || castFrom > deParticleEmitterType::ecfVolume ){
		DETHROW( deeInvalidParam );
	}
	
	if( castFrom == pCastFrom ){
		return;
	}
	
	pCastFrom = castFrom;
	NotifyChanged();
}

void peeType::SetSimulationType( deParticleEmitterType::eSimulationTypes simulationType ){
	if( simulationType < 0 || simulationType > deParticleEmitterType::estBeam ){
		DETHROW( deeInvalidParam );
	}
	
	if( simulationType == pSimulationType ){
		return;
	}
	
	pSimulationType = simulationType;
	NotifyChanged();
}

void peeType::SetIntervalAsDistance( bool intervalAsDistance ){
	if( intervalAsDistance == pIntervalAsDistance ){
		return;
	}
	
	pIntervalAsDistance = intervalAsDistance;
	NotifyChanged();
}

void peeType::SetPathTrailEmitter( const char *path ){
	if( pPathTrailEmitter.Equals( path ) ){
		return;
	}
	
	pPathTrailEmitter = path;
	pLoadTrailEmitter();
	NotifyChanged();
}

const decString &peeType::GetTrailController( deParticleEmitterType::eEmitControllers controller ) const{
	return pTrailControllers[ controller ];
}

void peeType::SetTrailController( deParticleEmitterType::eEmitControllers controller, const char *targetController ){
	if( targetController == pTrailControllers[ controller ] ){
		return;
	}
	
	pTrailControllers[ controller ] = targetController;
	NotifyChanged();
}

void peeType::SetActiveTrailController( deParticleEmitterType::eEmitControllers index ){
	if( index == pActiveTrailController ){
		return;
	}
	
	pActiveTrailController = index;
	NotifyActiveTrailControllerChanged();
}



void peeType::SetPhysicsSize( float size ){
	if( size < 0.0f ){
		size = 0.0f;
	}
	
	if( fabsf( size - pPhysicsSize ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pPhysicsSize = size;
	NotifyChanged();
}

void peeType::SetCollisionResponse( deParticleEmitterType::eCollisionResponses response ){
	if( response == pCollisionResponse ){
		return;
	}
	
	pCollisionResponse = response;
	NotifyChanged();
}

void peeType::SetPathCollisionEmitter( const char *path ){
	if( pPathCollisionEmitter.Equals( path ) ){
		return;
	}
	
	pPathCollisionEmitter = path;
	pLoadCollisionEmitter();
	NotifyChanged();
}

void peeType::SetEmitMinImpulse( float impulse ){
	impulse = decMath::max( impulse, 0.0f );
	
	if( fabsf( impulse - pEmitMinImpulse ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pEmitMinImpulse = impulse;
	NotifyChanged();
}

const decString &peeType::GetEmitController( deParticleEmitterType::eEmitControllers controller ) const{
	return pEmitControllers[ controller ];
}

void peeType::SetEmitController( deParticleEmitterType::eEmitControllers controller, const char *targetController ){
	if( targetController == pEmitControllers[ controller ] ){
		return;
	}
	
	pEmitControllers[ controller ] = targetController;
	NotifyChanged();
}

void peeType::SetActiveEmitController( deParticleEmitterType::eEmitControllers index ){
	if( index == pActiveEmitController ){
		return;
	}
	
	pActiveEmitController = index;
	NotifyActiveEmitControllerChanged();
}



void peeType::SetActive( bool active ){
	pActive = active;
}



void peeType::NotifyChanged(){
	if( pEmitter ){
		pEmitter->NotifyTypeChanged( this );
	}
}

void peeType::NotifyActiveTrailControllerChanged(){
	if( pEmitter ){
		pEmitter->NotifyTypeActiveTrailControllerChanged( this );
	}
}

void peeType::NotifyActiveEmitControllerChanged(){
	if( pEmitter ){
		pEmitter->NotifyTypeActiveEmitControllerChanged( this );
	}
}



void peeType::UpdateEngineType( deParticleEmitterType &type ){
	int i;
	
	type.SetSkin( pEngSkin );
	type.SetModel( pEngCastModel );
	type.SetModelSkin( pEngCastSkin );
	type.SetCastFrom( ( deParticleEmitterType::eCastFrom )pCastFrom );
	type.SetSimulationType( ( deParticleEmitterType::eSimulationTypes )pSimulationType );
	type.SetIntervalAsDistance( pIntervalAsDistance );
	type.SetTrailEmitter( pEngTrailEmitter );
	type.SetPhysicsSize( pPhysicsSize );
	type.SetCollisionResponse( ( deParticleEmitterType::eCollisionResponses )pCollisionResponse );
	type.SetCollisionEmitter( pEngCollisionEmitter );
	type.SetEmitMinImpulse( pEmitMinImpulse );
	
	const deParticleEmitterType::eEmitControllers controllers[ 4 ] = {
		deParticleEmitterType::eecLifetime,
		deParticleEmitterType::eecMass,
		deParticleEmitterType::eecLinearVelocity,
		deParticleEmitterType::eecAngularVelocity
	};
	for( i=0; i<4; i++ ){
		type.SetTrailController( controllers[ i ], type.GetTrailEmitter()
			? type.GetTrailEmitter()->IndexOfControllerNamed( pTrailControllers[ i ] ) : -1 );
		type.SetEmitController( controllers[ i ], type.GetCollisionEmitter()
			? type.GetCollisionEmitter()->IndexOfControllerNamed( pEmitControllers[ i ] ) : -1 );
	}
	
	const deParticleEmitterType::eParameters parameters[ 31 ] = {
		deParticleEmitterType::epTimeToLive,
		deParticleEmitterType::epInterval,
		deParticleEmitterType::epParticleCount,
		deParticleEmitterType::epCastAngleX,
		deParticleEmitterType::epCastAngleY,
		deParticleEmitterType::epSize,
		deParticleEmitterType::epRed,
		deParticleEmitterType::epGreen,
		deParticleEmitterType::epBlue,
		deParticleEmitterType::epTransparency,
		deParticleEmitterType::epEmissivity,
		deParticleEmitterType::epMass,
		deParticleEmitterType::epRotation,
		deParticleEmitterType::epLinearVelocity,
		deParticleEmitterType::epAngularVelocity,
		deParticleEmitterType::epBrownMotion,
		deParticleEmitterType::epDamping,
		deParticleEmitterType::epDrag,
		deParticleEmitterType::epGravityX,
		deParticleEmitterType::epGravityY,
		deParticleEmitterType::epGravityZ,
		deParticleEmitterType::epLocalGravity,
		deParticleEmitterType::epForceFieldDirect,
		deParticleEmitterType::epForceFieldSurface,
		deParticleEmitterType::epForceFieldMass,
		deParticleEmitterType::epForceFieldSpeed,
		deParticleEmitterType::epElasticity,
		deParticleEmitterType::epRoughness,
		deParticleEmitterType::epEmitDirection,
		deParticleEmitterType::epBeamStart,
		deParticleEmitterType::epBeamEnd
	};
	for( i=0; i<31; i++ ){
		pParameters[ i ]->UpdateEngineParameter( type.GetParameter( parameters[ i ] ) );
	}
}



// Parameters
///////////////

peeParameter *peeType::GetParameterAt( deParticleEmitterType::eParameters index ) const{
	return pParameters[ index ];
}

int peeType::IndexOfParameter( peeParameter *parameter ) const{
	if( ! parameter ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<31; i++ ){
		if( parameter == pParameters[ i ] ){
			return i;
		}
	}
	
	return -1;
}

void peeType::SetActiveParameter( peeParameter *parameter ){
	if( ! parameter ){
		DETHROW( deeInvalidParam );
	}
	
	if( parameter == pActiveParameter ){
		return;
	}
	
	pActiveParameter->SetActive( false );
	pActiveParameter = parameter;
	parameter->SetActive( true );
	
	if( pEmitter ){
		pEmitter->NotifyActiveTypeParameterChanged( this );
	}
}



// Private Functions
//////////////////////

void peeType::pCleanUp(){
	if( pEngCollisionEmitter ){
		pEngCollisionEmitter->FreeReference();
	}
	if( pEngTrailEmitter ){
		pEngTrailEmitter->FreeReference();
	}
	
	int i;
	for( i=0; i<=deParticleEmitterType::epBeamEnd; i++ ){
		if( pParameters[ i ] ){
			delete pParameters[ i ];
		}
	}
	
	if( pEngSkin ){
		pEngSkin->FreeReference();
	}
	if( pEngCastModel ){
		pEngCastModel->FreeReference();
	}
	if( pEngCastSkin ){
		pEngCastSkin->FreeReference();
	}
}

void peeType::pSetParameter( deParticleEmitterType::eParameters index, float value, float spread ){
	pParameters[ index ] = new peeParameter( *this, index, value, spread );
}

void peeType::pLoadSkin(){
	if( pEngSkin ){
		pEngSkin->FreeReference();
		pEngSkin = NULL;
	}
	
	if( ! pSkinPath.IsEmpty() ){
		try{
			if( pEmitter ){
				pEngSkin = pEngine->GetSkinManager()->LoadSkin( pSkinPath, pEmitter->GetDirectoryPath() );
				
			}else{
				pEngSkin = pEngine->GetSkinManager()->LoadSkin( pSkinPath, "/" );
			}
			
		}catch( const deException & ){
			if( pEmitter ){
				pEngSkin = pEmitter->GetEnvironment()->GetErrorSkin();
				pEngSkin->AddReference();
			}
		}
	}
	
	if( pEmitter && pEmitter->GetEngineEmitter() ){
		pEmitter->GetEngineEmitter()->GetTypeAt( pIndex ).SetSkin( pEngSkin );
	}
}

void peeType::pLoadModel(){
	if( pEngCastModel ){
		pEngCastModel->FreeReference();
		pEngCastModel = NULL;
	}
	
	if( ! pModelPath.IsEmpty() ){
		try{
			if( pEmitter ){
				pEngCastModel = pEngine->GetModelManager()->LoadModel( pModelPath, pEmitter->GetDirectoryPath() );
				
			}else{
				pEngCastModel = pEngine->GetModelManager()->LoadModel( pModelPath, "/" );
			}
			
		}catch( const deException &e ){
			if( pEmitter ){
				pEmitter->GetLogger()->LogException( LOGSOURCE, e );
			}
		}
	}
}

void peeType::pLoadModelSkin(){
	if( pEngCastSkin ){
		pEngCastSkin->FreeReference();
		pEngCastSkin = NULL;
	}
	
	if( ! pModelSkinPath.IsEmpty() ){
		try{
			if( pEmitter ){
				pEngCastSkin = pEngine->GetSkinManager()->LoadSkin( pModelSkinPath, pEmitter->GetDirectoryPath() );
				
			}else{
				pEngCastSkin = pEngine->GetSkinManager()->LoadSkin( pModelSkinPath, "/" );
			}
			
		}catch( const deException & ){
			if( pEmitter ){
				pEngCastSkin = pEmitter->GetEnvironment()->GetErrorSkin();
				pEngCastSkin->AddReference();
			}
		}
	}
}

void peeType::pLoadCollisionEmitter(){
	if( pEngCollisionEmitter ){
		pEngCollisionEmitter->FreeReference();
		pEngCollisionEmitter = NULL;
	}
	
	if( pPathCollisionEmitter.IsEmpty() ){
		return;
	}
	
	if( ! pEmitter ){
		return;
	}
	
	peeEmitter *emitter = NULL;
	try{
		emitter = pEmitter->GetLoadSaveSystem().LoadEmitter( decPath::AbsolutePathUnix(
			pPathCollisionEmitter, pEmitter->GetDirectoryPath() ).GetPathUnix() );
		emitter->RebuildEmitter();
		pEngCollisionEmitter = emitter->GetEngineEmitter();
		pEngCollisionEmitter->AddReference();
		emitter->FreeEmitter();
		
	}catch( const deException &e ){
		if( emitter ){
			emitter->FreeEmitter();
		}
		pEmitter->GetLogger()->LogException( "DEIGDE", e );
	}
	
	if( pEngCollisionEmitter ){
		return;
	}
}

void peeType::pLoadTrailEmitter(){
	if( pEngTrailEmitter ){
		pEngTrailEmitter->FreeReference();
		pEngTrailEmitter = NULL;
	}
	
	if( pPathTrailEmitter.IsEmpty() ){
		return;
	}
	
	if( ! pEmitter ){
		return;
	}
	
	peeEmitter *emitter = NULL;
	try{
		emitter = pEmitter->GetLoadSaveSystem().LoadEmitter( decPath::AbsolutePathUnix(
			pPathTrailEmitter, pEmitter->GetDirectoryPath() ).GetPathUnix() );
		emitter->RebuildEmitter();
		pEngTrailEmitter = emitter->GetEngineEmitter();
		pEngTrailEmitter->AddReference();
		emitter->FreeEmitter();
		
	}catch( const deException &e ){
		if( emitter ){
			emitter->FreeEmitter();
		}
		pEmitter->GetLogger()->LogException( "DEIGDE", e );
	}
	
	if( pEngTrailEmitter ){
		return;
	}
}
