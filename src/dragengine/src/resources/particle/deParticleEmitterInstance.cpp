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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deParticleEmitter.h"
#include "deParticleEmitterInstance.h"
#include "deParticleEmitterInstanceManager.h"
#include "deParticleEmitterInstanceType.h"
#include "deParticleEmitterController.h"
#include "deParticleEmitterType.h"
#include "../../deEngine.h"
#include "../../systems/modules/graphic/deBaseGraphicParticleEmitterInstance.h"
#include "../../systems/modules/physics/deBasePhysicsParticleEmitterInstance.h"
#include "../../systems/modules/scripting/deBaseScriptingParticleEmitterInstance.h"
#include "../../common/exceptions.h"



// Class deParticleEmitterInstance
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deParticleEmitterInstance::deParticleEmitterInstance( deParticleEmitterInstanceManager *manager ) :
deResource( manager ),
pControllers( NULL ),
pControllerCount( 0 ),
pControllerSize( 0 )
{
	pEnableCasting = false;
	pRemoveAfterLastParticleDied = false;
	pTimeScale = 1.0f;
	pWarmUpTime = 0.0f;
	pBurstTime = 0.0f;
	pLayerMask.SetBit( 0 );
	
	pTypes = NULL;
	pTypeCount = 0;
	
	pPeerGraphic = NULL;
	pPeerPhysics = NULL;
	pPeerScripting = NULL;
	
	pParentWorld = NULL;
	pLLWorldPrev = NULL;
	pLLWorldNext = NULL;
}

deParticleEmitterInstance::~deParticleEmitterInstance(){
	if( pPeerScripting ){
		delete pPeerScripting;
		pPeerScripting = NULL;
	}
	if( pPeerPhysics ){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	if( pTypes ){
		delete [] pTypes;
	}
	if( pControllers ){
		delete [] pControllers;
	}
}



// Management
///////////////

void deParticleEmitterInstance::SetEmitter( deParticleEmitter *emitter ){
	if( pEmitter == emitter ){
		return;
	}
	
	pEmitter = emitter;
	
	pUpdateControllers();
	pUpdateTypes();
	
	if( pPeerGraphic ){
		pPeerGraphic->EmitterChanged();
	}
	if( pPeerPhysics ){
		pPeerPhysics->EmitterChanged();
	}
}



void deParticleEmitterInstance::SetPosition( const decDVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		
		if( pPeerGraphic ){
			pPeerGraphic->PositionChanged();
		}
		if( pPeerPhysics ){
			pPeerPhysics->PositionChanged();
		}
	}
}

void deParticleEmitterInstance::SetOrientation( const decQuaternion &orientation ){
	if( ! orientation.IsEqualTo( pOrientation ) ){
		pOrientation = orientation;
		
		if( pPeerGraphic ){
			pPeerGraphic->OrientationChanged();
		}
		if( pPeerPhysics ){
			pPeerPhysics->OrientationChanged();
		}
	}
}

void deParticleEmitterInstance::SetReferencePosition( const decDVector &position ){
	if( ! position.IsEqualTo( pReferencePosition ) ){
		pReferencePosition = position;
		
		if( pPeerGraphic ){
			pPeerGraphic->ReferencePositionChanged();
		}
	}
}

void deParticleEmitterInstance::SetEnableCasting( bool enable ){
	if( enable != pEnableCasting ){
		pEnableCasting = enable;
		
		if( pPeerGraphic ){
			pPeerGraphic->EnableCastingChanged();
		}
		if( pPeerPhysics ){
			pPeerPhysics->EnableCastingChanged();
		}
	}
}

void deParticleEmitterInstance::SetRemoveAfterLastParticleDied( bool remove ){
	pRemoveAfterLastParticleDied = remove;
}

void deParticleEmitterInstance::SetTimeScale( float scale ){
	pTimeScale = scale;
}

void deParticleEmitterInstance::SetWarmUpTime( float warmUpTime ){
	if( fabsf( warmUpTime - pWarmUpTime ) > FLOAT_SAFE_EPSILON ){
		pWarmUpTime = warmUpTime;
		
		if( pEmitter->GetGraphicModuleSimulates() ){
			if( pPeerGraphic ){
				pPeerGraphic->WarmUpTimeChanged();
			}
			
		}else{
			if( pPeerPhysics ){
				pPeerPhysics->WarmUpTimeChanged();
			}
		}
	}
}

void deParticleEmitterInstance::SetBurstTime( float burstTime ){
	if( fabsf( burstTime - pBurstTime ) > FLOAT_SAFE_EPSILON ){
		pBurstTime = burstTime;
		
		if( pPeerGraphic ){
			pPeerGraphic->BurstTimeChanged();
		}
	}
}



void deParticleEmitterInstance::SetLayerMask( const decLayerMask &layerMask ){
	if( layerMask == pLayerMask ){
		return;
	}
	
	pLayerMask = layerMask;
	
	if( pPeerGraphic ){
		pPeerGraphic->LayerMaskChanged();
	}
}

void deParticleEmitterInstance::SetCollisionFilter( const decCollisionFilter &collisionFilter ){
	if( collisionFilter == pCollisionFilter ){
		return;
	}
	
	pCollisionFilter = collisionFilter;
	
	if( pPeerGraphic ){
		pPeerGraphic->CollisionFilterChanged();
	}
	if( pPeerPhysics ){
		pPeerPhysics->CollisionFilterChanged();
	}
}



deParticleEmitterController &deParticleEmitterInstance::GetControllerAt( int index ){
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	return pControllers[ index ];
}

const deParticleEmitterController &deParticleEmitterInstance::GetControllerAt( int index ) const{
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	return pControllers[ index ];
}

int deParticleEmitterInstance::IndexOfControllerNamed( const char *name ) const{
	int i;
	for( i=0; i<pControllerCount; i++ ){
		if( pControllers[ i ].GetName() == name ){
			return i;
		}
	}
	return -1;
}

void deParticleEmitterInstance::NotifyControllerChangedAt( int index ){
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerGraphic ){
		pPeerGraphic->ControllerChanged( index );
	}
	if( pPeerPhysics ){
		pPeerPhysics->ControllerChanged( index );
	}
}



deParticleEmitterInstanceType &deParticleEmitterInstance::GetTypeAt( int index ){
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTypes[ index ];
}

const deParticleEmitterInstanceType &deParticleEmitterInstance::GetTypeAt( int index ) const{
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTypes[ index ];
}

void deParticleEmitterInstance::NotifyTypeChangedAt( int index ){
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerGraphic ){
		pPeerGraphic->TypeChanged( index );
	}
	if( pPeerPhysics ){
		pPeerPhysics->TypeChanged( index );
	}
}

void deParticleEmitterInstance::NotifyTypeParticlesChangedAt( int index ){
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerGraphic ){
		pPeerGraphic->TypeParticlesChanged( index );
	}
}



void deParticleEmitterInstance::ResetBurst(){
	if( pEmitter ){
		if( pEmitter->GetGraphicModuleSimulates() ){
			if( pPeerGraphic ){
				pPeerGraphic->ResetBurst();
			}
			
		}else{
			if( pPeerPhysics ){
				pPeerPhysics->ResetBurst();
			}
		}
	}
}

void deParticleEmitterInstance::KillAllParticles(){
	if( pEmitter ){
		if( pEmitter->GetGraphicModuleSimulates() ){
			if( pPeerGraphic ){
				pPeerGraphic->KillAllParticles();
			}
			
		}else{
			if( pPeerPhysics ){
				pPeerPhysics->KillAllParticles();
			}
		}
	}
}



void deParticleEmitterInstance::NotifyLastParticleDied(){
	if( pPeerScripting ){
		pPeerScripting->LastParticleDied();
	}
}

void deParticleEmitterInstance::CollisionResponse( deCollisionInfo *collisionInfo ){
	if( pPeerScripting ){
		pPeerScripting->CollisionResponse( collisionInfo );
	}
}



// System Peers
/////////////////

void deParticleEmitterInstance::SetPeerGraphic( deBaseGraphicParticleEmitterInstance *peer ){
	if( peer != pPeerGraphic ){
		if( pPeerGraphic ){
			delete pPeerGraphic;
		}
		
		pPeerGraphic = peer;
	}
}

void deParticleEmitterInstance::SetPeerPhysics( deBasePhysicsParticleEmitterInstance *peer ){
	if( peer != pPeerPhysics ){
		if( pPeerPhysics ){
			delete pPeerPhysics;
		}
		
		pPeerPhysics = peer;
	}
}

void deParticleEmitterInstance::SetPeerScripting( deBaseScriptingParticleEmitterInstance *peer ){
	if( peer != pPeerScripting ){
		if( pPeerScripting ){
			delete pPeerScripting;
		}
		
		pPeerScripting = peer;
	}
}



// Linked List
////////////////

void deParticleEmitterInstance::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deParticleEmitterInstance::SetLLWorldPrev( deParticleEmitterInstance *instance ){
	pLLWorldPrev = instance;
}

void deParticleEmitterInstance::SetLLWorldNext( deParticleEmitterInstance *instance ){
	pLLWorldNext = instance;
}



// Private function
/////////////////////

void deParticleEmitterInstance::pUpdateControllers(){
	if( ! pEmitter ){
		pControllerCount = 0;
		return;
	}
	
	const int controllerCount = pEmitter->GetControllerCount();
	
	if( controllerCount > pControllerSize ){
		deParticleEmitterController * const newArray = new deParticleEmitterController[ controllerCount ];
		
		delete [] pControllers;
		pControllers = newArray;
		
		pControllerSize = controllerCount;
	}
	
	for( pControllerCount=0; pControllerCount<controllerCount; pControllerCount++ ){
		pControllers[ pControllerCount ] = *pEmitter->GetControllerAt( pControllerCount );
	}
}

void deParticleEmitterInstance::pUpdateTypes(){
	if( pEmitter ){
		const int typeCount = pEmitter->GetTypeCount();
		deParticleEmitterInstanceType *newArray = new deParticleEmitterInstanceType[ typeCount ];
		
		if( pTypes ){
			delete [] pTypes;
		}
		pTypes = newArray;
		pTypeCount = typeCount;
		
	}else{
		if( pTypes ){
			delete [] pTypes;
		}
		pTypes = NULL;
		pTypeCount = 0;
	}
}
