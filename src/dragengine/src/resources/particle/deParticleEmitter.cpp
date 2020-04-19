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
#include "deParticleEmitterType.h"
#include "deParticleEmitterManager.h"
#include "deParticleEmitterController.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicParticleEmitter.h"
#include "../../systems/modules/physics/deBasePhysicsParticleEmitter.h"



// Class deParticleEmitter
////////////////////////////

// Constructor, destructor
////////////////////////////

deParticleEmitter::deParticleEmitter( deParticleEmitterManager *manager ) :
deResource( manager ),
pControllers( NULL ),
pControllerCount( 0 ),
pControllerSize( 0 )
{
	pEmitBurst = false;
	pBurstLifetime = 1.0f;
	
	pTypes = NULL;
	pTypeCount = 0;
	
	pGraphicModuleSimlates = false;
	
	pPeerGraphic = NULL;
	pPeerPhysics = NULL;
}

deParticleEmitter::~deParticleEmitter(){
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
	RemoveAllControllers();
	if( pControllers ){
		delete [] pControllers;
	}
}



// Management
///////////////

void deParticleEmitter::SetBurstLifetime( float lifetime ){
	if( fabsf( lifetime - pBurstLifetime ) > FLOAT_SAFE_EPSILON ){
		pBurstLifetime = lifetime;
		
		if( pPeerGraphic ){
			pPeerGraphic->BurstChanged();
		}
		if( pPeerPhysics ){
			pPeerPhysics->BurstChanged();
		}
	}
}

void deParticleEmitter::SetEmitBurst( bool emitBurst ){
	if( emitBurst != pEmitBurst ){
		pEmitBurst = emitBurst;
		
		if( pPeerGraphic ){
			pPeerGraphic->BurstChanged();
		}
		if( pPeerPhysics ){
			pPeerPhysics->BurstChanged();
		}
	}
}



void deParticleEmitter::SetTypeCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pTypeCount ){
		deParticleEmitterType *newArray = NULL;
		
		if( count > 0 ){
			newArray = new deParticleEmitterType[ count ];
		}
		
		if( pTypes ){
			delete [] pTypes;
		}
		
		pTypes = newArray;
		pTypeCount = count;
		
		if( pPeerGraphic ){
			pPeerGraphic->TypeCountChanged();
		}
		if( pPeerPhysics ){
			pPeerPhysics->TypeCountChanged();
		}
	}
}

deParticleEmitterType &deParticleEmitter::GetTypeAt( int index ){
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTypes[ index ];
}

const deParticleEmitterType &deParticleEmitter::GetTypeAt( int index ) const{
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTypes[ index ];
}

void deParticleEmitter::NotifyTypeChangedAt( int type ){
	if( type < 0 || type >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerGraphic ){
		pPeerGraphic->TypeChanged( type );
	}
	if( pPeerPhysics ){
		pPeerPhysics->TypeChanged( type );
	}
}



void deParticleEmitter::SetGraphicModuleSimulates( bool graphicModuleSimulates ){
	if( graphicModuleSimulates != pGraphicModuleSimlates ){
		pGraphicModuleSimlates = graphicModuleSimulates;
		
		if( pPeerPhysics ){
			pPeerPhysics->GraphicModuleRequestsChanged();
		}
	}
}



// Controller Management
//////////////////////////

deParticleEmitterController *deParticleEmitter::GetControllerAt( int index ) const{
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	return pControllers[ index ];
}

int deParticleEmitter::IndexOfController( deParticleEmitterController *controller ) const{
	if( ! controller ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pControllerCount; i++ ){
		if( pControllers[ i ] == controller ){
			return i;
		}
	}
	
	return -1;
}

int deParticleEmitter::IndexOfControllerNamed( const char *name ) const{
	int i;
	for( i=0; i<pControllerCount; i++ ){
		if( pControllers[ i ]->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool deParticleEmitter::HasController( deParticleEmitterController *controller ) const{
	if( ! controller ) DETHROW( deeInvalidParam );
	
	int i;
	
	for( i=0; i<pControllerCount; i++ ){
		if( pControllers[ i ] == controller ){
			return true;
		}
	}
	
	return false;
}

void deParticleEmitter::AddController( deParticleEmitterController *controller ){
	if( ! controller ) DETHROW( deeInvalidParam );
	
	if( pControllerCount == pControllerSize ){
		int i, newSize = pControllerSize * 3 / 2 + 1;
		deParticleEmitterController **newArray = new deParticleEmitterController*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pControllers ){
			for( i=0; i<pControllerSize; i++ ) newArray[ i ] = pControllers[ i ];
			delete [] pControllers;
		}
		pControllers = newArray;
		pControllerSize = newSize;
	}
	pControllers[ pControllerCount ] = controller;
	pControllerCount++;
	
	if( pPeerGraphic ){
		pPeerGraphic->ControllerCountChanged();
	}
	if( pPeerPhysics ){
		pPeerPhysics->ControllerCountChanged();
	}
}

void deParticleEmitter::RemoveController( deParticleEmitterController *controller ){
	int index = IndexOfController( controller );
	if( index == -1 ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=index+1; i<pControllerCount; i++ ){
		pControllers[ i - 1 ] = pControllers[ i ];
	}
	pControllerCount--;
	delete controller;
	
	if( pPeerGraphic ){
		pPeerGraphic->ControllerCountChanged();
	}
	if( pPeerPhysics ){
		pPeerPhysics->ControllerCountChanged();
	}
}

void deParticleEmitter::RemoveAllControllers(){
	while( pControllerCount > 0 ){
		delete pControllers[ pControllerCount - 1 ];
		pControllerCount--;
	}
	
	if( pPeerGraphic ){
		pPeerGraphic->ControllerCountChanged();
	}
	if( pPeerPhysics ){
		pPeerPhysics->ControllerCountChanged();
	}
}

void deParticleEmitter::NotifyControllerChangedAt( int index ){
	if( index < 0 || index >= pControllerCount ) DETHROW( deeInvalidParam );
	
	if( pPeerGraphic ){
		pPeerGraphic->ControllerChanged( index );
	}
	if( pPeerPhysics ){
		pPeerPhysics->ControllerChanged( index );
	}
}



// System Peers
/////////////////

void deParticleEmitter::SetPeerGraphic( deBaseGraphicParticleEmitter *peer ){
	if( peer != pPeerGraphic ){
		if( pPeerGraphic ){
			delete pPeerGraphic;
		}
		
		pPeerGraphic = peer;
	}
}

void deParticleEmitter::SetPeerPhysics( deBasePhysicsParticleEmitter *peer ){
	if( peer != pPeerPhysics ){
		if( pPeerPhysics ){
			delete pPeerPhysics;
		}
		
		pPeerPhysics = peer;
	}
}
