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

#include "deAnimator.h"
#include "deAnimatorReference.h"
#include "deAnimatorInstance.h"
#include "deAnimatorInstanceManager.h"
#include "controller/deAnimatorController.h"
#include "../animation/deAnimation.h"
#include "../component/deComponent.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/animator/deBaseAnimatorAnimatorInstance.h"



// Class deAnimatorInstance
/////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorInstance::deAnimatorInstance( deAnimatorInstanceManager *manager ) :
deResource( manager ),

pBlendMode( deAnimatorRule::ebmBlend ),
pBlendFactor( 1.0f ),
pEnableRetargeting( true ),
pProtectDynamicBones( false ),

pControllers( NULL ),
pControllerCount( 0 ),
pControllerSize( 0 ),

pPeerAnimator( NULL ){
}

deAnimatorInstance::~deAnimatorInstance(){
	pCleanUp();
}



// Management
///////////////

void deAnimatorInstance::SetAnimator( deAnimator *animator, bool keepValues ){
	if( pAnimator == animator ){
		return;
	}
	
	if( keepValues && pAnimator && animator && pControllerCount > 0 && animator->GetControllerCount() > 0 ){
		// NOTE the guard is required to keep the controller name alive during transfering.
		//      we can not use pControllers names since they are destroyed during
		//      pUpdateControllers(). instead controller names from guarded animator is used
		const deAnimatorReference guard( pAnimator );
		const int count = pControllerCount;
		struct sControllerValue{
			const char *name;
			float value;
			decVector vector;
		} * const transfer = new sControllerValue[ count ];
		int i;
		
		try{
			for( i=0; i<count; i++ ){
				transfer[ i ].name = pAnimator->GetControllerAt( i )->GetName();
				transfer[ i ].value = pControllers[ i ].GetCurrentValue();
				transfer[ i ].vector = pControllers[ i ].GetVector();
			}
			
			pAnimator = animator;
			pUpdateControllers();
			
			for( i=0; i<count; i++ ){
				const int index = IndexOfControllerNamed( transfer[ i ].name );
				if( index == -1 ){
					continue;
				}
				
				pControllers[ index ].SetCurrentValue( transfer[ i ].value );
				pControllers[ index ].SetVector( transfer[ i ].vector );
			}
			
			delete [] transfer;
			
		}catch( const deException & ){
			if( transfer ){
				delete [] transfer;
			}
			throw;
		}
		
	}else{
		pAnimator = animator;
		pUpdateControllers();
	}
	
	if( pPeerAnimator ){
		pPeerAnimator->AnimatorChanged();
	}
}

void deAnimatorInstance::SetComponent( deComponent *component ){
	if( pComponent == component ){
		return;
	}
	
	pComponent = component;
	
	if( pPeerAnimator ){
		pPeerAnimator->ComponentChanged();
	}
}

void deAnimatorInstance::SetAnimation( deAnimation *animation ){
	if( pAnimation == animation ){
		return;
	}
	
	pAnimation = animation;
	
	if( pPeerAnimator ){
		pPeerAnimator->AnimationChanged();
	}
}



void deAnimatorInstance::SetBlendMode( deAnimatorRule::eBlendModes mode ){
	if( mode < deAnimatorRule::ebmBlend || mode > deAnimatorRule::ebmBlend ){
		DETHROW( deeInvalidParam );
	}
	
	if( mode == pBlendMode ){
		return;
	}
	
	pBlendMode = mode;
	
	if( pPeerAnimator ){
		pPeerAnimator->BlendFactorChanged();
	}
}

void deAnimatorInstance::SetBlendFactor( float factor ){
	factor = decMath::clamp( factor, 0.0f, 1.0f );
	
	if( fabsf( factor - pBlendFactor ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pBlendFactor = factor;
	
	if( pPeerAnimator ){
		pPeerAnimator->BlendFactorChanged();
	}
}

void deAnimatorInstance::SetEnableRetargeting( bool enableRetargeting ){
	if( enableRetargeting == pEnableRetargeting ){
		return;
	}
	
	pEnableRetargeting = enableRetargeting;
	
	if( pPeerAnimator ){
		pPeerAnimator->EnableRetargetingChanged();
	}
}

void deAnimatorInstance::SetProtectDynamicBones( bool protectDynamicBones ){
	if( protectDynamicBones == pProtectDynamicBones ){
		return;
	}
	
	pProtectDynamicBones = protectDynamicBones;
	
	if( pPeerAnimator ){
		pPeerAnimator->ProtectDynamicBonesChanged();
	}
}



deAnimatorController &deAnimatorInstance::GetControllerAt( int index ){
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	return pControllers[ index ];
}

const deAnimatorController &deAnimatorInstance::GetControllerAt( int index ) const{
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	return pControllers[ index ];
}

int deAnimatorInstance::IndexOfControllerNamed( const char *name ) const{
	int i;
	for( i=0; i<pControllerCount; i++ ){
		if( pControllers[ i ].GetName() == name ){
			return i;
		}
	}
	return -1;
}

void deAnimatorInstance::NotifyControllerChangedAt( int index ){
	if( index < 0 || index >= pControllerCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerAnimator ){
		pPeerAnimator->ControllerChanged( index );
	}
}



void deAnimatorInstance::Apply( bool direct ){
	if( pPeerAnimator ){
		pPeerAnimator->Apply( direct );
	}
}

void deAnimatorInstance::CaptureStateInto( int identifier ){
	if( pPeerAnimator ){
		pPeerAnimator->CaptureStateInto( identifier );
	}
}

void deAnimatorInstance::StoreFrameInto( int identifier, const char *moveName, float moveTime ){
	if( ! moveName ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerAnimator ){
		pPeerAnimator->StoreFrameInto( identifier, moveName, moveTime );
	}
}



// System Peers
/////////////////

void deAnimatorInstance::SetPeerAnimator( deBaseAnimatorAnimatorInstance *peer ){
	if( peer == pPeerAnimator ){
		return;
	}
	
	if( pPeerAnimator ){
		delete pPeerAnimator;
	}
	
	pPeerAnimator = peer;
}



// Private function
/////////////////////

void deAnimatorInstance::pCleanUp(){
	if( pPeerAnimator ){
		delete pPeerAnimator;
		pPeerAnimator = NULL;
	}
	
	if( pControllers ){
		delete [] pControllers;
	}
}

void deAnimatorInstance::pUpdateControllers(){
	if( ! pAnimator ){
		pControllerCount = 0;
		return;
	}
	
	const int controllerCount = pAnimator->GetControllerCount();
	
	if( controllerCount > pControllerSize ){
		deAnimatorController * const newArray = new deAnimatorController[ controllerCount ];
		
		delete [] pControllers;
		pControllers = newArray;
		
		pControllerSize = controllerCount;
	}
	
	for( pControllerCount=0; pControllerCount<controllerCount; pControllerCount++ ){
		pControllers[ pControllerCount ] = *pAnimator->GetControllerAt( pControllerCount );
	}
}
