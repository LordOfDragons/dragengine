/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeController.h"
#include "../aeAnimator.h"
#include "../locomotion/aeAnimatorLocomotion.h"
#include "../locomotion/aeAnimatorLocomotionLeg.h"
#include "../wakeboard/aeWakeboard.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/common/exceptions.h>



// Class aeController
///////////////////////

// Constructor, destructor
////////////////////////////

aeController::aeController( const char *name ) :
pAnimator( nullptr ),
pEngControllerIndex( -1 ),
pName( name ),
pMinValue( 0.0f ),
pMaxValue( 1.0f ),
pCurValue( 0.0f ),
pClamp( true ),
pFrozen( false ),
pLocoAttr( aeAnimatorLocomotion::eaNone ),
pLocoLeg( 0 ){
}

aeController::aeController( const aeController &copy ) :
pAnimator( nullptr ),
pEngControllerIndex( -1 ),
pName( copy.pName ),
pMinValue( copy.pMinValue ),
pMaxValue( copy.pMaxValue ),
pCurValue( copy.pCurValue ),
pClamp( copy.pClamp ),
pFrozen( copy.pFrozen ),
pLocoAttr( copy.pLocoAttr ),
pLocoLeg( copy.pLocoLeg ){
}

aeController::~aeController(){
	pCleanUp();
}



// Management
///////////////

void aeController::SetAnimator( aeAnimator *animator ){
	if( animator != pAnimator ){
		pAnimator = animator;
		pEngControllerIndex = -1;
	}
}

void aeController::SetEngineControllerIndex( int index ){
	if( index != pEngControllerIndex ){
		pEngControllerIndex = index;
		
		if( index != -1 ){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			deAnimatorController &controller = instance.GetControllerAt( index );
			
			controller.SetName( pName );
			controller.SetValueRange( pMinValue, pMaxValue );
			controller.SetCurrentValue( pCurValue );
			controller.SetFrozen( pFrozen );
			controller.SetClamp( pClamp );
			controller.SetVector( pVector );
			
			pCurValue = controller.GetCurrentValue();
			
			instance.NotifyControllerChangedAt( index );
		}
	}
}



void aeController::SetName( const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	
	pName = name;
	
	if( pAnimator ){
		pAnimator->NotifyControllerNameChanged( this );
	}
}



void aeController::SetMinimumValue( float value ){
	if( fabsf( value - pMinValue ) > 1e-5f ){
		if( value > pMaxValue ){
			pMinValue = value;
			pMaxValue = value;
			
		}else{
			pMinValue = value;
		}
		
		if( pEngControllerIndex != -1 ){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			deAnimatorController &controller = instance.GetControllerAt( pEngControllerIndex );
			
			controller.SetValueRange( pMinValue, pMaxValue );
			
			instance.NotifyControllerChangedAt( pEngControllerIndex );
		}
		
		pCurValue = pCheckValue( pCurValue );
		
		if( pAnimator ){
			pAnimator->NotifyControllerChanged( this );
			pAnimator->NotifyControllerValueChanged( this );
		}
	}
}

void aeController::SetMaximumValue( float value ){
	if( fabsf( value - pMaxValue ) > 1e-5f ){
		if( value < pMinValue ){
			pMinValue = value;
			pMaxValue = value;
			
		}else{
			pMaxValue = value;
		}
		
		if( pEngControllerIndex != -1 ){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			deAnimatorController &controller = instance.GetControllerAt( pEngControllerIndex );
			
			controller.SetValueRange( pMinValue, pMaxValue );
			
			instance.NotifyControllerChangedAt( pEngControllerIndex );
		}
		
		pCurValue = pCheckValue( pCurValue );
		
		if( pAnimator ){
			pAnimator->NotifyControllerChanged( this );
			pAnimator->NotifyControllerValueChanged( this );
		}
	}
}

void aeController::SetCurrentValue( float value ){
	if( ! pFrozen ){
		value = pCheckValue( value );
		
		if( fabsf( value - pCurValue ) > 1e-5f ){
			pCurValue = value;
			
			if( pEngControllerIndex != -1 ){
				deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
				deAnimatorController &controller = instance.GetControllerAt( pEngControllerIndex );
				
				controller.SetCurrentValue( value );
				
				instance.NotifyControllerChangedAt( pEngControllerIndex );
			}
			
			if( pAnimator ){
				pAnimator->NotifyControllerValueChanged( this );
			}
		}
	}
}

void aeController::IncrementCurrentValue( float incrementBy ){
	SetCurrentValue( pCurValue + incrementBy );
}

void aeController::SetFrozen( bool frozen ){
	if( frozen != pFrozen ){
		pFrozen = frozen;
		
		if( pEngControllerIndex != -1 ){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			deAnimatorController &controller = instance.GetControllerAt( pEngControllerIndex );
			
			controller.SetFrozen( frozen );
			
			instance.NotifyControllerChangedAt( pEngControllerIndex );
		}
		
		if( pAnimator ){
			pAnimator->NotifyControllerChanged( this );
		}
	}
}

void aeController::SetClamp( bool clamp ){
	if( clamp != pClamp ){
		pClamp = clamp;
		
		if( pEngControllerIndex != -1 ){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			deAnimatorController &controller = instance.GetControllerAt( pEngControllerIndex );
			
			controller.SetClamp( clamp );
			
			instance.NotifyControllerChangedAt( pEngControllerIndex );
		}
		
		if( pAnimator ){
			pAnimator->NotifyControllerChanged( this );
		}
	}
}

void aeController::SetVector( const decVector &vector ){
	if( ! vector.IsEqualTo( pVector ) ){
		pVector = vector;
		
		if( pEngControllerIndex != -1 ){
			deAnimatorInstance &instance = *pAnimator->GetEngineAnimatorInstance();
			deAnimatorController &controller = instance.GetControllerAt( pEngControllerIndex );
			
			controller.SetVector( vector );
			
			instance.NotifyControllerChangedAt( pEngControllerIndex );
		}
		
		if( pAnimator ){
			pAnimator->NotifyControllerValueChanged( this );
		}
	}
}

void aeController::SetLocomotionAttribute( int attribute ){
	if( attribute != pLocoAttr ){
		pLocoAttr = attribute;
			
		if( pAnimator ){
			pAnimator->NotifyControllerChanged( this );
		}
	}
}

void aeController::SetLocomotionLeg( int leg ){
	pLocoLeg = leg;
}



void aeController::UpdateValue( float elapsed ){
	const aeAnimatorLocomotion &locomotion = pAnimator->GetLocomotion();
	const bool wakeboarding = pAnimator->GetWakeboard().GetEnabled();
	const bool enabled = locomotion.GetEnabled();
	
	switch( pLocoAttr ){
	case aeAnimatorLocomotion::eaElapsedTime:
		IncrementCurrentValue( elapsed );
		break;
		
	case aeAnimatorLocomotion::eaMovingSpeed:
		if( enabled ){
			SetCurrentValue( locomotion.GetMovingSpeed() );
		}
		break;
		
	case aeAnimatorLocomotion::eaMovingDirection:
		if( enabled ){
			SetCurrentValue( locomotion.GetMovingDirection() );
		}
		break;
		
	case aeAnimatorLocomotion::eaRelativeMovingSpeed:
		if( enabled ){
			SetCurrentValue( locomotion.GetRelativeMovingSpeed() );
		}
		break;
		
	case aeAnimatorLocomotion::eaTurningSpeed:
		if( enabled ){
			SetCurrentValue( locomotion.GetTurnVelocity() );
		}
		break;
		
	case aeAnimatorLocomotion::eaStance:
		if( enabled ){
			SetCurrentValue( locomotion.GetStance().GetValue() );
		}
		break;
		
	case aeAnimatorLocomotion::eaDisplacement:
		if( enabled ){
			IncrementCurrentValue( locomotion.GetMovingSpeed() * elapsed );
		}
		break;
		
	case aeAnimatorLocomotion::eaRelativeDisplacement:
		if( enabled ){
			IncrementCurrentValue( locomotion.GetRelativeMovingSpeed() * elapsed );
		}
		break;
		
	case aeAnimatorLocomotion::eaLookUpDown:
		if( enabled || wakeboarding ){
			SetCurrentValue( locomotion.GetLookUpDown().GetValue() );
		}
		break;
		
	case aeAnimatorLocomotion::eaLookLeftRight:
		if( enabled || wakeboarding ){
			SetCurrentValue( locomotion.GetLookLeftRight().GetValue() );
		}
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltOffset:
		if( enabled || wakeboarding ){
			SetCurrentValue( locomotion.GetTiltOffset() );
		}
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltUpDown:
		if( enabled || wakeboarding ){
			SetCurrentValue( locomotion.GetTiltUpDown() );
		}
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltRightLeft:
		if( enabled || wakeboarding ){
			SetCurrentValue( locomotion.GetTiltLeftRight() );
		}
		break;
		
	case aeAnimatorLocomotion::eaLegGroundPosition:
		if( enabled || wakeboarding ){
			if( pLocoLeg >= 0 && pLocoLeg < locomotion.GetLegCount() ){
				SetVector( locomotion.GetLegAt( pLocoLeg )->GetGroundPosition() );
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegGroundNormal:
		if( enabled || wakeboarding ){
			if( pLocoLeg >= 0 && pLocoLeg < locomotion.GetLegCount() ){
				SetVector( locomotion.GetLegAt( pLocoLeg )->GetGroundNormal() );
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegInfluence:
		if( enabled || wakeboarding ){
			if( pLocoLeg >= 0 && pLocoLeg < locomotion.GetLegCount() ){
				SetCurrentValue( locomotion.GetLegAt( pLocoLeg )->GetIKInfluence() );
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegPosition:
		if( enabled || wakeboarding ){
			if( pLocoLeg >= 0 && pLocoLeg < locomotion.GetLegCount() ){
				SetCurrentValue( locomotion.GetLegAt( pLocoLeg )->GetIKInfluence() );
				SetVector( locomotion.GetLegAt( pLocoLeg )->GetIKPosition() );
			}
		}
		break;
		
	case aeAnimatorLocomotion::eaLegOrientation:
		if( enabled || wakeboarding ){
			if( pLocoLeg >= 0 && pLocoLeg < locomotion.GetLegCount() ){
				SetCurrentValue( locomotion.GetLegAt( pLocoLeg )->GetIKInfluence() );
				SetVector( locomotion.GetLegAt( pLocoLeg )->GetIKOrientation() );
			}
		}
		break;
	}
}

void aeController::ResetValue(){
	const aeAnimatorLocomotion &locomotion = pAnimator->GetLocomotion();
	
	switch( pLocoAttr ){
	case aeAnimatorLocomotion::eaElapsedTime:
	case aeAnimatorLocomotion::eaTimeTurnIP:
		SetCurrentValue( 0.0f );
		break;
		
	case aeAnimatorLocomotion::eaLookUpDown:
		SetCurrentValue( locomotion.GetLookUpDown().GetValue() );
		break;
		
	case aeAnimatorLocomotion::eaLookLeftRight:
		SetCurrentValue( locomotion.GetLookLeftRight().GetValue() );
		break;
		
	case aeAnimatorLocomotion::eaMovingSpeed:
		SetCurrentValue( locomotion.GetMovingSpeed() );
		break;
		
	case aeAnimatorLocomotion::eaMovingDirection:
		SetCurrentValue( locomotion.GetMovingDirection() );
		break;
		
	case aeAnimatorLocomotion::eaRelativeMovingSpeed:
		SetCurrentValue( locomotion.GetRelativeMovingSpeed() );
		break;
		
	case aeAnimatorLocomotion::eaTurningSpeed:
		SetCurrentValue( locomotion.GetTurnVelocity() );
		break;
		
	case aeAnimatorLocomotion::eaStance:
		SetCurrentValue( locomotion.GetStance().GetValue() );
		break;
		
	case aeAnimatorLocomotion::eaDisplacement:
	case aeAnimatorLocomotion::eaRelativeDisplacement:
		SetCurrentValue( 0.0f );
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltOffset:
		SetCurrentValue( locomotion.GetTiltOffset() );
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltUpDown:
		SetCurrentValue( locomotion.GetTiltUpDown() );
		break;
		
	case aeAnimatorLocomotion::eaBodyTiltRightLeft:
		SetCurrentValue( locomotion.GetTiltLeftRight() );
		break;
		
	case aeAnimatorLocomotion::eaLegGroundPosition:
		if( pLocoLeg >= 0 && pLocoLeg < locomotion.GetLegCount() ){
			SetVector( locomotion.GetLegAt( pLocoLeg )->GetGroundPosition() );
		}
		break;
		
	case aeAnimatorLocomotion::eaLegGroundNormal:
		if( pLocoLeg >= 0 && pLocoLeg < locomotion.GetLegCount() ){
			SetVector( locomotion.GetLegAt( pLocoLeg )->GetGroundNormal() );
		}
		break;
		
	case aeAnimatorLocomotion::eaLegInfluence:
		if( pLocoLeg >= 0 && pLocoLeg < locomotion.GetLegCount() ){
			SetCurrentValue( locomotion.GetLegAt( pLocoLeg )->GetIKInfluence() );
		}
		break;
		
	case aeAnimatorLocomotion::eaLegPosition:
		if( pLocoLeg >= 0 && pLocoLeg < locomotion.GetLegCount() ){
			SetCurrentValue( locomotion.GetLegAt( pLocoLeg )->GetIKInfluence() );
			SetVector( locomotion.GetLegAt( pLocoLeg )->GetIKPosition() );
		}
		break;
		
	case aeAnimatorLocomotion::eaLegOrientation:
		if( pLocoLeg >= 0 && pLocoLeg < locomotion.GetLegCount() ){
			SetCurrentValue( locomotion.GetLegAt( pLocoLeg )->GetIKInfluence() );
			SetVector( locomotion.GetLegAt( pLocoLeg )->GetIKOrientation() );
		}
		break;
	}
}

void aeController::InverseValue(){
	SetCurrentValue( pMaxValue - pCurValue );
}



// Private Functions
//////////////////////

void aeController::pCleanUp(){
	SetAnimator( NULL );
}

float aeController::pCheckValue( float value ){
	float range = pMaxValue - pMinValue;
	bool hasRange = range > 1e-5f;
	
	// make sure the current value is in the correct range
	if( pClamp ){
		if( value < pMinValue ){
			value = pMinValue;
			
		}else if( value > pMaxValue ){
			value = pMaxValue;
		}
		
	}else{
		if( hasRange ){
			value = fmodf( value - pMinValue, range );
			
			if( value < 0.0f ){
				value += pMinValue + range;
				
			}else{
				value += pMinValue;
			}
			
		}else{
			value = pMinValue;
		}
	}
	
	return value;
}
