/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debpParticleEmitter.h"
#include "debpParticleEmitterInstance.h"
#include "debpParticleEmitterInstanceType.h"

#include "../debpCommon.h"
#include "../dePhysicsBullet.h"
#include "../coldet/debpCollisionDetection.h"
#include "../component/debpComponent.h"
#include "../forcefield/debpForceField.h"
#include "../visitors/debpClosestRayResultCallback.h"
#include "../world/debpWorld.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"

#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceReference.h>
#include <dragengine/resources/particle/deParticleEmitterController.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceType.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/resources/world/deWorld.h>



#ifndef random
#define random rand
#endif



//#define DO_TIMING
#ifdef DO_TIMING
#include "dragengine/common/utils/decTimer.h"
static decTimer timer;
#endif

// Class debpParticleEmitterInstance
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

debpParticleEmitterInstance::debpParticleEmitterInstance( dePhysicsBullet *bullet, deParticleEmitterInstance *instance ){
	if( ! bullet || ! instance ){
		DETHROW( deeInvalidParam );
	}
	
	pBullet = bullet;
	pInstance = instance;
	
	pParentWorld = NULL;
	
	pTypes = NULL;
	pTypeCount = 0;
	
	pBurstTimer = 0.0f;
	
	pCanCollide = false;
	
	pDirtyEmitterMatrix = true;
	pCheckForLastParticle = false;
	pEnsureCastOnce = false;
	
	pLastPosition = instance->GetPosition();
	
	EmitterChanged();
	CollisionFilterChanged();
}

debpParticleEmitterInstance::~debpParticleEmitterInstance(){
	pCleanUp();
}



// Management
///////////////

void debpParticleEmitterInstance::SetParentWorld( debpWorld *parentWorld ){
	pParentWorld = parentWorld;
}



debpParticleEmitter *debpParticleEmitterInstance::GetParticleEmitter() const{
	if( pInstance->GetEmitter() ){
		return ( debpParticleEmitter* )pInstance->GetEmitter()->GetPeerPhysics();
		
	}else{
		return NULL;
	}
}

const decDMatrix &debpParticleEmitterInstance::GetEmitterMatrix(){
	if( pDirtyEmitterMatrix ){
		pEmitterMatrix.SetWorld( pInstance->GetPosition(), pInstance->GetOrientation() );
		pDirtyEmitterMatrix = false;
	}
	
	return pEmitterMatrix;
}

decDMatrix debpParticleEmitterInstance::GetEmitterMatrixByDistance( float distance ) const{
	return decDMatrix::CreateWorld( pLastPosition * ( 1.0f - distance ) + pInstance->GetPosition() * distance, pInstance->GetOrientation() );
}

debpParticleEmitterInstanceType &debpParticleEmitterInstance::GetTypeAt( int index ){
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTypes[ index ];
}

const debpParticleEmitterInstanceType &debpParticleEmitterInstance::GetTypeAt( int index ) const{
	if( index < 0 || index >= pTypeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTypes[ index ];
}



void debpParticleEmitterInstance::PrepareParticles( float elapsed ){
	debpParticleEmitter * const emitter = GetParticleEmitter();
	const float travelledDistance = ( float )( pInstance->GetPosition() - pLastPosition ).Length();
	
	const bool casting = pInstance->GetEnableCasting() || pEnsureCastOnce;
	pEnsureCastOnce = false;
	
	if( emitter ){
		int i;
		
		emitter->UpdateParameters();
		
		pForceFieldSimulation.Update( elapsed );
		
		// update the particles
		pBurstTimer += elapsed;
		
		for( i=0; i<pTypeCount; i++ ){
//timer.Reset();
			pTypes[ i ].PrepareParticles( casting, elapsed, travelledDistance );
//pBullet->LogInfoFormat( "PrepareParticle (%i) = %iys", pTypes[ i ].GetParticlesCount(), ( int )( timer.GetElapsedTime() * 1000000.0f ) );
		}
		
		if( pInstance->GetEmitter()->GetEmitBurst() ){
			if( pInstance->GetEmitter()->GetBurstLifetime() > FLOAT_SAFE_EPSILON ){
				pInstance->SetBurstTime( pBurstTimer / pInstance->GetEmitter()->GetBurstLifetime() );
				
			}else{
				pInstance->SetBurstTime( 0.0f );
			}
			
			if( casting && pBurstTimer >= pInstance->GetEmitter()->GetBurstLifetime() ){
				pInstance->SetEnableCasting( false );
				RequestCheckForLastParticle();
			}
			
		}else{
			pInstance->SetBurstTime( 0.0f );
		}
		
		/*{
		int t, T=0;
		for( t=0; t<pTypeCount; t++ ) T += pTypes[ t ].GetParticlesCount();
		pBullet->LogInfoFormat( "PrepareParticles %p: ec=%i eb=%i bt=%f blt=%f cflp=%i pc=%i ralpd=%i", this,
			pInstance->GetEnableCasting()?1:0, pInstance->GetEmitter()->GetEmitBurst()?1:0,
			pBurstTimer, pInstance->GetEmitter()->GetBurstLifetime(), pCheckForLastParticle?1:0, T,
			pInstance->GetRemoveAfterLastParticleDied()?1:0 );
		}*/
	}
	
	pLastPosition = pInstance->GetPosition();
	
	ApplyForceFields( elapsed );
	
	//CheckForLastParticle(); // too early. StepParticles can also trigger a check which we would cancel the next time PrepareParticle is called
}

void debpParticleEmitterInstance::ApplyForceFields( float elapsed ){
	// TODO track force fields to update only if they are nearby
	if( ! pParentWorld ){
		return;
	}
	
	const deWorld &world = pParentWorld->GetWorld();
	deForceField *forceField = world.GetRootForceField();
	int i;
	
	while( forceField ){
		if( forceField->GetCollisionFilter().Collides( pInstance->GetCollisionFilter() ) ){
			const debpForceField &bpForceField = *( ( debpForceField* )forceField->GetPeerPhysics() );
			
			// TODO check if the force field overlaps the component
			
			for( i=0; i<pTypeCount; i++ ){
				pTypes[ i ].ApplyForceField( bpForceField, elapsed );
			}
		}
		forceField = forceField->GetLLWorldNext();
	}
}

void debpParticleEmitterInstance::StepParticles( float elapsed ){
	int i;
	
	for( i=0; i<pTypeCount; i++ ){
//timer.Reset();
		pTypes[ i ].StepParticles( elapsed );
//pBullet->LogInfoFormat( "StepParticle (%i) = %iys", pTypes[ t ].GetParticlesCount(), ( int )( timer.GetElapsedTime() * 1000000.0f ) );
	}
	
	CheckForLastParticle(); // StepParticles is called last so the check has to be done here not in PrepareParticles
}

void debpParticleEmitterInstance::FinishStepping(){
	int i;
	
	pInstance->SetReferencePosition( pInstance->GetPosition() ); // particles will be relative to this positon for rendering
	
	for( i=0; i<pTypeCount; i++ ){
		pTypes[ i ].FinishStepping();
	}
}

void debpParticleEmitterInstance::RequestCheckForLastParticle(){
	pCheckForLastParticle = true;
}

void debpParticleEmitterInstance::CheckForLastParticle(){
	if( ! pCheckForLastParticle || pInstance->GetEnableCasting() ){
		return;
	}
	if( pInstance->GetEmitter()->GetEmitBurst() && pBurstTimer < pInstance->GetEmitter()->GetBurstLifetime() ){
		return;
	}
	
	int t;
	for( t=0; t<pTypeCount; t++ ){
		if( pTypes[ t ].GetParticlesCount() > 0 ){
			return;
		}
	}
	
	pCheckForLastParticle = false;
	
	deParticleEmitterInstanceReference guard( pInstance );
	pInstance->NotifyLastParticleDied();
	if( pInstance->GetRemoveAfterLastParticleDied() ){
		pInstance->GetParentWorld()->RemoveParticleEmitter( pInstance );
	}
}



// Notifications
//////////////////

void debpParticleEmitterInstance::EmitterChanged(){
	pLastPosition = pInstance->GetPosition();
	pUpdateTypes();
	CollisionFilterChanged(); // setting emitter potentially changes the layer mask
}

void debpParticleEmitterInstance::PositionChanged(){
	pDirtyEmitterMatrix = true;
}

void debpParticleEmitterInstance::OrientationChanged(){
	pDirtyEmitterMatrix = true;
}

void debpParticleEmitterInstance::EnableCastingChanged(){
	pLastPosition = pInstance->GetPosition();
	
	if( pInstance->GetEnableCasting() ){
		if( pInstance->GetWarmUpTime() > FLOAT_SAFE_EPSILON ){
			pSimulateWarpUp();
		}
		pCheckForLastParticle = false;
		pEnsureCastOnce = true;
	}
}

void debpParticleEmitterInstance::WarmUpTimeChanged(){
}

void debpParticleEmitterInstance::CollisionFilterChanged(){
	pCanCollide = pInstance->GetCollisionFilter().CanCollide();
}

void debpParticleEmitterInstance::ControllerChanged( int controller ){
	const deParticleEmitter * const emitter = pInstance->GetEmitter();
	
	if( emitter ){
		int t;
		
		for( t=0; t<pTypeCount; t++ ){
			const deParticleEmitterParameter &parameter = emitter->GetTypeAt( pTypes[ t ].GetType() ).GetParameter( deParticleEmitterType::epInterval );
			
			if( parameter.GetControllerValue() > -1 || parameter.GetControllerSpread() > -1 ){
				pTypes[ t ].UpdateCastIntervals();
			}
		}
	}
}

void debpParticleEmitterInstance::TypeChanged( int type ){
	pTypes[ type ].OnTypeChanged();
}



void debpParticleEmitterInstance::ResetBurst(){
	int i;
	
	pBurstTimer = 0.0f;
	
	for( i=0; i<pTypeCount; i++ ){
		pTypes[ i ].ResetBurst();
	}
}

void debpParticleEmitterInstance::KillAllParticles(){
	int i;
	
	for( i=0; i<pTypeCount; i++ ){
		pTypes[ i ].KillAllParticles();
	}
}



// Private Functions
//////////////////////

void debpParticleEmitterInstance::pCleanUp(){
	if( pTypes ){
		delete [] pTypes;
	}
}

void debpParticleEmitterInstance::pUpdateTypes(){
	if( pTypes ){
		delete [] pTypes;
		pTypes = NULL;
	}
	pTypeCount = 0;
	
	const int typeCount = pInstance->GetTypeCount();
	if( typeCount == 0 ){
		return;
	}
	
	pTypes = new debpParticleEmitterInstanceType[ typeCount ];
	
	for( pTypeCount=0; pTypeCount<typeCount; pTypeCount++ ){
		pTypes[ pTypeCount ].SetInstance( this );
		pTypes[ pTypeCount ].SetType( pTypeCount );
		pTypes[ pTypeCount ].OnTypeChanged();
	}
}

void debpParticleEmitterInstance::pSimulateWarpUp(){
	float remainingTime = pInstance->GetWarmUpTime();
	const float timeStep = 1.0f / 30.0f;
	float simulationStep;
	
	while( remainingTime > 0.0f ){
		if( remainingTime < timeStep ){
			simulationStep = remainingTime;
			
		}else{
			simulationStep = timeStep;
		}
		
		PrepareParticles( simulationStep );
		StepParticles( simulationStep );
		
		remainingTime -= timeStep;
	}
}
