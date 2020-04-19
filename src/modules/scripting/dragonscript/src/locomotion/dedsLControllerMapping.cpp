/* 
 * Drag[en]gine DragonScript Script Module
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

#include "dedsLocomotion.h"
#include "dedsLControllerMapping.h"

#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>



// Class dedsLControllerMapping
/////////////////////////////////

// Constructor, destructor
////////////////////////////

dedsLControllerMapping::dedsLControllerMapping() :
pController( 0 ),
pAttribute( eaElapsedTime ){
}

dedsLControllerMapping::dedsLControllerMapping( const dedsLControllerMapping &mapping ) :
pAnimatorInstance( mapping.pAnimatorInstance ),
pController( mapping.pController ),
pAttribute( mapping.pAttribute ){
}



// Management
///////////////

void dedsLControllerMapping::SetAnimatorInstance( deAnimatorInstance *animatorInstance ){
	pAnimatorInstance = animatorInstance;
}

void dedsLControllerMapping::SetController( int controller ){
	pController = controller;
}

void dedsLControllerMapping::SetAttribute( eAttributes attribute ){
	pAttribute = attribute;
}



void dedsLControllerMapping::Apply( const dedsLocomotion &locomotion, float elapsed ){
	if( ! pAnimatorInstance ){
		return;
	}
	
	const int controllerCount = pAnimatorInstance->GetControllerCount();
	if( pController < 0 || pController >= controllerCount ){
		return;
	}
	
	deAnimatorController &controller = pAnimatorInstance->GetControllerAt( pController );
	
	switch( pAttribute ){
	case eaElapsedTime:
		controller.IncrementCurrentValue( elapsed );
		break;
		
	case eaLookVertical:
		controller.SetCurrentValue( locomotion.GetLookVertical().GetValue() );
		break;
		
	case eaLookHorizontal:
		controller.SetCurrentValue( locomotion.GetLookHorizontal().GetValue() );
		break;
		
	case eaMovingSpeed:
		controller.SetCurrentValue( locomotion.GetMovingSpeed() );
		break;
		
	case eaMovingDirection:
		controller.SetCurrentValue( locomotion.GetMovingDirection() );
		break;
		
	case eaRelativeMovingSpeed:
		if( fabs( locomotion.GetMovingDirection() ) > 90.0f ){
			controller.SetCurrentValue( -locomotion.GetMovingSpeed() );
			
		}else{
			controller.SetCurrentValue( locomotion.GetMovingSpeed() );
		}
		break;
		
	case eaTurningSpeed:
		controller.SetCurrentValue( locomotion.GetTurningSpeed() );
		break;
		
	case eaStance:
		controller.SetCurrentValue( locomotion.GetStance().GetValue() );
		break;
		
	case eaDisplacement:
		controller.IncrementCurrentValue( fabsf( locomotion.GetMovingSpeed() ) * elapsed );
// 			if( pResetTimeWalk ){
// 				controller.SetCurrentValue( fabsf( locomotion.GetMovingSpeed() ) * elapsed );
// 				
// 			}else{
// 				controller.IncrementCurrentValue( fabsf( locomotion.GetMovingSpeed() ) * elapsed );
// 			}
		break;
		
	case eaTimeTurnIP:
		if( locomotion.GetReverseTimeTurnIP() ){
			controller.SetCurrentValue( controller.GetMaximumValue() - controller.GetCurrentValue() );
		}
		if( locomotion.GetResetTimeTurnIP() ){
			controller.SetCurrentValue( elapsed );
			
		}else{
			controller.IncrementCurrentValue( elapsed );
		}
		break;
		
	case eaTiltOffset:
		controller.SetCurrentValue( locomotion.GetTiltOffset() );
		break;
		
	case eaTiltVertical:
		controller.SetCurrentValue( locomotion.GetTiltVertical().GetValue() );
		break;
		
	case eaTiltHorizontal:
		controller.SetCurrentValue( locomotion.GetTiltHorizontal().GetValue() );
		break;
		
	default:
		return;
	}
	
	pAnimatorInstance->NotifyControllerChangedAt( pController );
}

void dedsLControllerMapping::ApplyPost( const dedsLocomotion &locomotion, float elapsed ){
	if( ! pAnimatorInstance ){
		return;
	}
	
	const int controllerCount = pAnimatorInstance->GetControllerCount();
	if( pController < 0 || pController >= controllerCount ){
		return;
	}
	
	deAnimatorController &controller = pAnimatorInstance->GetControllerAt( pController );
	
	switch( pAttribute ){
	case eaTiltOffset:
		controller.SetCurrentValue( locomotion.GetTiltOffset() );
		break;
		
	case eaTiltVertical:
		controller.SetCurrentValue( locomotion.GetTiltVertical().GetValue() );
		break;
		
	case eaTiltHorizontal:
		controller.SetCurrentValue( locomotion.GetTiltHorizontal().GetValue() );
		break;
		
	default:
		return;
	}
	
	pAnimatorInstance->NotifyControllerChangedAt( pController );
}



// Operators
//////////////

dedsLControllerMapping &dedsLControllerMapping::operator=( const dedsLControllerMapping &mapping ){
	pAnimatorInstance = mapping.pAnimatorInstance;
	pController = mapping.pController;
	pAttribute = mapping.pAttribute;
	return *this;
}
