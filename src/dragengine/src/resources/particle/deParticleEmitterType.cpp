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
#include "../model/deModel.h"
#include "../skin/deSkin.h"
#include "../../common/exceptions.h"



// Class deParticleEmitterType
////////////////////////////////

// Constructor, destructor
////////////////////////////

deParticleEmitterType::deParticleEmitterType() :
pCastFrom( ecfVertex ),
pSimulationType( estParticle ),
pIntervalAsDistance( false ),

pPhysicsSize( 0.1f ),

pCollisionResponse( ecrPhysical ),
pEmitMinImpulse( 0.01f ),

pMaxLinearVelocity( 1.0f ),
pMaxAngularVelocity( DEG2RAD * 360.0f ),
pSquaredVelocities( false )
{
	pParameters[ epTimeToLive ].SetValue( 1.0f );
	pParameters[ epInterval ].SetValue( 1.0f );
	pParameters[ epParticleCount ].SetValue( 1.0f );
	pParameters[ epCastAngleX ].SetValue( 0.0f );
	pParameters[ epCastAngleY ].SetValue( 0.0f );
	pParameters[ epSize ].SetValue( 1.0f );
	pParameters[ epRed ].SetValue( 1.0f );
	pParameters[ epGreen ].SetValue( 1.0f );
	pParameters[ epBlue ].SetValue( 1.0f );
	pParameters[ epTransparency ].SetValue( 1.0f );
	pParameters[ epEmissivity ].SetValue( 0.0f );
	pParameters[ epMass ].SetValue( 0.1f );
	pParameters[ epRotation ].SetValue( 0.0f );
	pParameters[ epLinearVelocity ].SetValue( 1.0f );
	pParameters[ epAngularVelocity ].SetValue( 0.0f );
	pParameters[ epBrownMotion ].SetValue( 0.0f );
	pParameters[ epDamping ].SetValue( 0.0f );
	pParameters[ epDrag ].SetValue( 0.0f );
	pParameters[ epGravityX ].SetSpread( 0.0f );
	pParameters[ epGravityY ].SetValue( 0.0f );
	pParameters[ epGravityZ ].SetValue( 0.0f );
	pParameters[ epLocalGravity ].SetValue( 0.0f );
	pParameters[ epForceFieldDirect ].SetValue( 0.0f );
	pParameters[ epForceFieldSurface ].SetValue( 0.0f );
	pParameters[ epForceFieldMass ].SetValue( 0.0f );
	pParameters[ epForceFieldSpeed ].SetValue( 0.0f );
	pParameters[ epElasticity ].SetValue( 1.0f );
	pParameters[ epRoughness ].SetValue( 0.0f );
	pParameters[ epEmitDirection ].SetValue( 0.0f );
	pParameters[ epBeamStart ].SetValue( 0.0f );
	pParameters[ epBeamEnd ].SetValue( 0.0f );
	
	int i;
	for( i=0; i<=eecAngularVelocity; i++ ){
		pTrailControllers[ i ] = -1;
	}
	for( i=0; i<=eecAngularVelocity; i++ ){
		pEmitControllers[ i ] = -1;
	}
}

deParticleEmitterType::~deParticleEmitterType(){
}



// Management
///////////////

void deParticleEmitterType::SetSkin( deSkin *skin ){
	pSkin = skin;
}

void deParticleEmitterType::SetModel( deModel *model ){
	pModel = model;
}

void deParticleEmitterType::SetModelSkin( deSkin *skin ){
	pModelSkin = skin;
}

void deParticleEmitterType::SetCastFrom( eCastFrom castFrom ){
	pCastFrom = castFrom;
}

void deParticleEmitterType::SetSimulationType( eSimulationTypes simulationType ){
	pSimulationType = simulationType;
}

void deParticleEmitterType::SetIntervalAsDistance( bool intervalAsDistance ){
	pIntervalAsDistance = intervalAsDistance;
}

void deParticleEmitterType::SetTrailEmitter( deParticleEmitter *emitter ){
	pTrailEmitter = emitter;
}

int deParticleEmitterType::GetTrailController( eEmitControllers controller ) const{
	return pTrailControllers[ controller ];
}

void deParticleEmitterType::SetTrailController( eEmitControllers controller, int targetController ){
	pTrailControllers[ controller ] = targetController;
}



void deParticleEmitterType::SetPhysicsSize( float size ){
	pPhysicsSize = decMath::max( size, 0.0f );
}

void deParticleEmitterType::SetCollisionResponse( eCollisionResponses response ){
	pCollisionResponse = response;
}

void deParticleEmitterType::SetCollisionEmitter( deParticleEmitter *emitter ){
	pCollisionEmitter = emitter;
}

void deParticleEmitterType::SetEmitMinImpulse( float impulse ){
	pEmitMinImpulse = decMath::max( impulse, 0.0f );
}

int deParticleEmitterType::GetEmitController( eEmitControllers controller ) const{
	return pEmitControllers[ controller ];
}

void deParticleEmitterType::SetEmitController( eEmitControllers controller, int targetController ){
	pEmitControllers[ controller ] = targetController;
}



void deParticleEmitterType::SetMaxLinearVelocity( float maxLinearVelocity ){
	pMaxLinearVelocity = decMath::max( maxLinearVelocity, 0.001f );
}

void deParticleEmitterType::SetMaxAngularVelocity( float maxAngularVelocity ){
	pMaxAngularVelocity = decMath::max( maxAngularVelocity, 0.001f );
}

void deParticleEmitterType::SetSquareVelocities( bool squareVelocities ){
	pSquaredVelocities = squareVelocities;
}



deParticleEmitterParameter &deParticleEmitterType::GetParameter( eParameters parameter ){
	return pParameters[ parameter ];
}

const deParticleEmitterParameter &deParticleEmitterType::GetParameter( eParameters parameter ) const{
	return pParameters[ parameter ];
}
