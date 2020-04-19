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

#include "deSmokeEmitter.h"
#include "deSmokeEmitterManager.h"
#include "../component/deComponent.h"
#include "../skin/deSkin.h"

#include "../../deEngine.h"
#include "../../systems/modules/graphic/deBaseGraphicSmokeEmitter.h"
#include "../../systems/modules/physics/deBasePhysicsSmokeEmitter.h"
#include "../../common/exceptions.h"



// Class deSmokeEmitter
///////////////////////////

// Constructor, destructor
////////////////////////////

deSmokeEmitter::deSmokeEmitter( deSmokeEmitterManager *manager ) : deResource( manager ){
	pCastVelocityMin = 1.0f;
	pCastVelocityMax = 1.0f;
	pCastDensityMin = 0.1f;
	pCastDensityMax = 0.1f;
	pEnableCasting = false;
	
	pComponent = NULL;
	pCastTexture = 0;
	pSkin = NULL;
	
	pUseLocalGravity = false;
	
	pPoints = NULL;
	pPointCount = 0;
	
	pPeerGraphic = NULL;
	pPeerPhysics = NULL;
	
	pParentWorld = NULL;
	pLLWorldNext = NULL;
	pLLWorldPrev = NULL;
}

deSmokeEmitter::~deSmokeEmitter(){
	if( pPeerPhysics ){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	if( pSkin ) pSkin->FreeReference();
	if( pComponent ) pComponent->FreeReference();
}



// Management
///////////////

void deSmokeEmitter::SetEmitterPosition( const decDVector &position ){
	if( ! position.IsEqualTo( pEmitterPosition ) ){
		pEmitterPosition = position;
		
		if( pPeerPhysics ) pPeerPhysics->EmitterPositionChanged();
	}
}

void deSmokeEmitter::SetEmitterOrientation( const decQuaternion &orientation ){
	if( ! orientation.IsEqualTo( pEmitterOrientation ) ){
		pEmitterOrientation = orientation;
		
		if( pPeerPhysics ) pPeerPhysics->EmitterOrientationChanged();
	}
}



void deSmokeEmitter::SetVolumePosition( const decDVector &position ){
	if( ! position.IsEqualTo( pVolumePosition ) ){
		pVolumePosition = position;
		
		if( pPeerGraphic ) pPeerGraphic->VolumePositionChanged();
	}
}



void deSmokeEmitter::SetMinCastDirection( const decVector &deviation ){
	if( ! deviation.IsEqualTo( pCastDirectionMin ) ){
		pCastDirectionMin = deviation;
		
		if( pPeerPhysics ) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetMaxCastDirection( const decVector &deviation ){
	if( ! deviation.IsEqualTo( pCastDirectionMax ) ){
		pCastDirectionMax = deviation;
		
		if( pPeerPhysics ) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetMinCastVelocity( float velocity ){
	if( fabsf( velocity - pCastVelocityMin ) > 1e-5f ){
		pCastVelocityMin = velocity;
		
		if( pPeerPhysics ) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetMaxCastVelocity( float velocity ){
	if( fabsf( velocity - pCastVelocityMax ) > 1e-5f ){
		pCastVelocityMax = velocity;
		
		if( pPeerPhysics ) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetMinCastDensity( float density ){
	if( fabsf( density - pCastDensityMin ) > 1e-5f ){
		pCastDensityMin = density;
		
		if( pPeerPhysics ) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetMaxCastDensity( float density ){
	if( fabsf( density - pCastDensityMax ) > 1e-5f ){
		pCastDensityMax = density;
		
		if( pPeerPhysics ) pPeerPhysics->CastPropertyChanged();
	}
}

void deSmokeEmitter::SetEnableCasting( bool enable ){
	if( enable != pEnableCasting ){
		pEnableCasting = enable;
		
		if( pPeerPhysics ) pPeerPhysics->EnableCastingChanged();
	}
}



void deSmokeEmitter::SetComponent( deComponent *component ){
	if( component != pComponent ){
		if( pComponent ) pComponent->FreeReference();
		
		pComponent = component;
		
		if( component ) component->AddReference();
		
		if( pPeerPhysics ) pPeerPhysics->ComponentChanged();
	}
}

void deSmokeEmitter::SetCastTexture( int texture ){
	if( texture != pCastTexture ){
		pCastTexture = texture;
		
		if( pPeerPhysics ) pPeerPhysics->CastTextureChanged();
	}
}

void deSmokeEmitter::SetSkin( deSkin *skin ){
	if( skin != pSkin ){
		if( pSkin ) pSkin->FreeReference();
		
		pSkin = skin;
		
		if( skin ) skin->AddReference();
		
		if( pPeerGraphic ) pPeerGraphic->SkinChanged();
		if( pPeerPhysics ) pPeerPhysics->SkinChanged();
	}
}

void deSmokeEmitter::SetCollisionFilter( const decCollisionFilter &collisionFilter ){
	if( collisionFilter == pCollisionFilter ){
		return;
	}
	
	pCollisionFilter = collisionFilter;
	
	if( pPeerPhysics ){
		pPeerPhysics->CollisionFilterChanged();
	}
}

void deSmokeEmitter::SetGravity( const decVector &gravity ){
	if( ! gravity.IsEqualTo( pGravity ) ){
		pGravity = gravity;
		
		if( pPeerPhysics ) pPeerPhysics->GravityChanged();
	}
}

void deSmokeEmitter::SetUseLocalGravity( bool useLocalGravity ){
	if( pUseLocalGravity != useLocalGravity ){
		pUseLocalGravity = useLocalGravity;
		
		if( pPeerPhysics ) pPeerPhysics->GravityChanged();
	}
}



void deSmokeEmitter::SetPoints( deSmokeDensityPoint *points, int count ){
	pPoints = points;
	pPointCount = count;
	
	if( pPeerGraphic ) pPeerGraphic->PointsChanged();
}

void deSmokeEmitter::NotifyPointsChanged(){
	if( pPeerGraphic ) pPeerGraphic->PointsChanged();
}



// System Peers
/////////////////

void deSmokeEmitter::SetPeerGraphic( deBaseGraphicSmokeEmitter *peer ){
	if( pPeerGraphic ) delete pPeerGraphic;
	pPeerGraphic = peer;
}

void deSmokeEmitter::SetPeerPhysics( deBasePhysicsSmokeEmitter *peer ){
	if( pPeerPhysics ) delete pPeerPhysics;
	pPeerPhysics = peer;
}



// Linked List
////////////////

void deSmokeEmitter::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deSmokeEmitter::SetLLWorldPrev( deSmokeEmitter *smokeEmitter ){
	pLLWorldPrev = smokeEmitter;
}

void deSmokeEmitter::SetLLWorldNext( deSmokeEmitter *smokeEmitter ){
	pLLWorldNext = smokeEmitter;
}
