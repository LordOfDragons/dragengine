/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpForceField.h"
#include "debpFFVortex.h"

#include "../world/debpWorld.h"

#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/common/exceptions.h>



#ifndef random
#define random rand
#endif

// Class debpForceField
/////////////////////////

// Constructor, destructor
////////////////////////////

debpForceField::debpForceField( dePhysicsBullet &bullet, const deForceField &forceField ) :
pBullet( bullet ),
pForceField( forceField ),
pVortices( NULL ),
pVortexCount( 0 ),
pVortexSize( 0 ),
pTimeUntilNextVortex( 0.5f ),
pDirtyGeometry( true )
{
	/*
	try{
		pVortices = new debpFFVortex*[ 20 ];
		if( ! pVortices ) DETHROW( deeOutOfMemory );
		
		for( pVortexSize=0; pVortexSize<20; pVortexSize++ ){
			pVortices[ pVortexSize ] = new debpFFVortex;
			if( ! pVortices[ pVortexSize ] ) DETHROW( deeOutOfMemory );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}*/
}

debpForceField::~debpForceField(){
	pCleanUp();
}



// Management
///////////////

void debpForceField::Update( float elapsed ){
	int v, v2;
	
	pUpdateGeometry();
	
	// update next vortex timer and add one if elapsed
	pTimeUntilNextVortex -= elapsed;
	if( pTimeUntilNextVortex <= 0.0f ){
		pTimeUntilNextVortex = 0.5f;
		
		if( pVortexCount < pVortexSize ){
			float fieldRadius = pForceField.GetRadius();
			float rfactor = 1.0f / ( float )RAND_MAX;
			decVector position, view;
			float size, velocity, timeToLive;
			float arc1, arc2, dist;
			
			size = 0.1f + ( float )random() * rfactor * 9.9f; // 0.1 -> 10m
			
			arc1 = ( float )random() * rfactor * ( PI * 2.0f );
			dist = ( float )random() * rfactor * ( fieldRadius - size );
			
			position.x = sinf( arc1 ) * dist;
			position.y = 0.0f;
			position.z = cosf( arc1 ) * dist;
			
			arc1 = ( float )random() * rfactor * ( PI * 2.0f );
			arc2 = ( float )random() * rfactor * PI;
			
			view.x = sinf( arc1 ) * cosf( arc2 );
			view.y = sinf( arc2 );
			view.z = cosf( arc1 ) * cosf( arc2 );
			
			velocity = ( float )random() * rfactor * pForceField.GetForce();
			timeToLive = 0.5f + ( float )random() * rfactor * 9.5f; // 0.5 -> 10s
			
			pVortices[ pVortexCount ]->SetPosition( position );
			pVortices[ pVortexCount ]->SetView( view );
			pVortices[ pVortexCount ]->SetSize( size );
			pVortices[ pVortexCount ]->SetFullVelocity( velocity );
			pVortices[ pVortexCount ]->SetTimeToLive( timeToLive );
			pVortices[ pVortexCount ]->Update();
			pVortexCount++;
			
			//pBullet->LogInfoFormat( "Added Vortex: p=(%g,%g,%g) v=(%g,%g,%g) s=%g ve=%g ttl=%g", position.x, position.y, position.z,
			//	view.x, view.y, view.z, size, velocity, timeToLive );
		}
	}
	
	// update vortices
	for( v=0; v<pVortexCount; v++ ){
		pVortices[ v ]->IncreaseLivedTime( elapsed );
		if( pVortices[ v ]->HasDied() ){
			debpFFVortex *vortex = pVortices[ v ];
			//pBullet->LogInfoFormat( "Vortex Died: p=(%g,%g,%g) v=(%g,%g,%g) s=%g ve=%g", vortex->GetPosition().x, vortex->GetPosition().y, vortex->GetPosition().z,
			//	vortex->GetView().x, vortex->GetView().y, vortex->GetView().z, vortex->GetSize(), vortex->GetFullVelocity() );
			
			for( v2=v+1; v2<pVortexCount; v2++ ){
				pVortices[ v2 - 1 ] = pVortices[ v2 ];
			}
			
			pVortices[ pVortexCount - 1 ] = vortex;
			pVortexCount--;
			
		}else{
			pVortices[ v ]->Update();
		}
	}
}



/*
void debpForceField::GetForceAtPoint( const decVector &position, decVector &force ){
	const float flucAngle = DEG2RAD * 180.0f;
	const float ffForce = pForceField.GetForce();
	const float ffRadius = pForceField.GetRadius();
	const float ffExponent = pForceField.GetExponent();
	const float ffFluctuation = pForceField.GetFluctuation();
	const int ffType = pForceField.GetType();
	decVector direction;
	decMatrix flucmat;
	float distance;
	float addvelo, addtorque, force;
	
		direction.x = ( float )( particle.position.x - ffpos.x );
		direction.y = ( float )( particle.position.y - ffpos.y );
		direction.z = ( float )( particle.position.z - ffpos.z );
		
		distance = direction.Length();
		
		if( distance > 1e-5f && distance < ffRadius ){
			if( ffType == deForceField::eftLinear ){
				direction = ffDir;
				
			}else{
				direction /= distance;
			}
			
			flucmat.SetRotationY( pFluctDir * ffFluctuation * flucAngle );
			force = ( ffForce * powf( 1.0f - distance / ffRadius, ffExponent ) )
				* ( 1.0f + ( pFluctStr * ffFluctuation ) * 0.25f ) * elapsed;
			
			if( pParticleEmitter->GetUseLocalGravity() ){
				addvelo = force * 0.02f;
				addtorque = 0.0f; //force * ( DEG2RAD * 45.0f );
				
			}else{
				addvelo = force * 0.2f;
				addtorque = 0.0f;
			}
			
			direction = flucmat.TransformNormal( direction );
			particle.linearVelocity += direction * addvelo;
			particle.angularVelocity.x += addtorque; // += direction * addtorque;
		}
	}
}
*/



// Vortices
/////////////

debpFFVortex *debpForceField::GetVortexAt( int index ) const{
	if( index < 0 || index >= pVortexCount ){
		DETHROW( deeInvalidParam );
	}
	return pVortices[ index ];
}



// Notifications
//////////////////

void debpForceField::PositionChanged(){
	pDirtyGeometry = true;
}

void debpForceField::OrientationChanged(){
	pDirtyGeometry = true;
}

void debpForceField::InfluenceAreaChanged(){
}

void debpForceField::RadiusChanged(){
}

void debpForceField::ExponentChanged(){
}

void debpForceField::TypeChanged(){
}

void debpForceField::DirectionChanged(){
}

void debpForceField::ForceChanged(){
}

void debpForceField::FluctuationChanged(){
}

void debpForceField::CollisionFilterChanged(){
}

void debpForceField::EnabledChanged(){
}



// Private functions
//////////////////////

void debpForceField::pCleanUp(){
	if( pVortices ){
		while( pVortexSize > 0 ){
			pVortexSize--;
			if( pVortices[ pVortexSize ] ){
				delete pVortices[ pVortexSize ];
			}
		}
		delete [] pVortices;
	}
}

void debpForceField::pUpdateGeometry(){
	if( ! pDirtyGeometry ){
		return;
	}
	
	const decMatrix matRot( decMatrix::CreateFromQuaternion( pForceField.GetOrientation() ) );
	pDirection = matRot.TransformView();
	
	pVortexMatrix = decMatrix::CreateRotationY( pForceField.GetFluctuationDirection()
		* DEG2RAD * pForceField.GetRadius() ) * matRot;
	
	pDirtyGeometry = false;
}
