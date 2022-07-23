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

#include "deAnimatorController.h"

#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// class deAnimatorController
///////////////////////////////

// constructor, destructor
////////////////////////////

deAnimatorController::deAnimatorController() :
pMinValue( 0.0f ),
pMaxValue( 1.0f ),
pCurValue( 0.0f ),
pClamp( true ),
pFrozen( false ){
}

deAnimatorController::~deAnimatorController(){
}



// Management
///////////////

void deAnimatorController::SetName( const char *name ){
	pName = name;
}

void deAnimatorController::SetValueRange( float minValue, float maxValue ){
	if( minValue > maxValue ){
		minValue = maxValue;
	}
	
	if( pFrozen ){
		return;
	}
	
	pMinValue = minValue;
	pMaxValue = maxValue;
	
	pUpdateValue();
}

void deAnimatorController::SetCurrentValue( float value ){
	if( pFrozen ){
		return;
	}
	
	pCurValue = value;
	
	pUpdateValue();
}

void deAnimatorController::IncrementCurrentValue( float incrementBy ){
	if( pFrozen ){
		return;
	}
	
	pCurValue += incrementBy;
	
	pUpdateValue();
}

void deAnimatorController::SetFrozen( bool frozen ){
	pFrozen = frozen;
}

void deAnimatorController::SetClamp( bool clamp ){
	pClamp = clamp;
}

void deAnimatorController::SetVector( const decVector &vector ){
	pVector = vector;
}



deAnimatorController &deAnimatorController::operator=( const deAnimatorController &controller ){
	pName = controller.pName;
	pMinValue = controller.pMinValue;
	pMaxValue = controller.pMaxValue;
	pCurValue = controller.pCurValue;
	pClamp = controller.pClamp;
	pFrozen = controller.pFrozen;
	pVector = controller.pVector;
	return *this;
}



// Private functions
//////////////////////

void deAnimatorController::pUpdateValue(){
	const float range = pMaxValue - pMinValue;
	const bool hasRange = range > 1e-5f;
	
	if( pClamp ){
		if( pCurValue < pMinValue ){
			pCurValue = pMinValue;
			
		}else if( pCurValue > pMaxValue ){
			pCurValue = pMaxValue;
		}
		
	}else{
		if( hasRange ){
			pCurValue = fmodf( pCurValue - pMinValue, range );
			if( pCurValue < 0 ){
				pCurValue += pMinValue + range;
				
			}else{
				pCurValue += pMinValue;
			}
			
		}else{
			pCurValue = pMinValue;
		}
	}
}
