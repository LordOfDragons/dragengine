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

#include "deParticleEmitterController.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deParticleEmitterController
//////////////////////////////////////

// Constructor, Destructor
////////////////////////////

deParticleEmitterController::deParticleEmitterController( const char *name ) :
pName( name ),
pLower( 0.0f ),
pUpper( 1.0f ),
pValue( 0.0f ),
pClamp( true ),
pFrozen( false ){
}

deParticleEmitterController::~deParticleEmitterController(){
}



// Management
///////////////

void deParticleEmitterController::SetName( const char *name ){
	pName = name;
}

void deParticleEmitterController::SetRange( float lower, float upper ){
	if( pFrozen ){
		return;
	}
	
	pLower = lower;
	pUpper = upper;
	
	pUpdateValue();
}

void deParticleEmitterController::SetValue( float value ){
	if( pFrozen ){
		return;
	}
	
	pValue = value;
	
	pUpdateValue();
}

void deParticleEmitterController::Increment( float amount ){
	if( pFrozen ){
		return;
	}
	
	pValue += amount;
	
	pUpdateValue();
}

void deParticleEmitterController::SetFrozen( bool frozen ){
	pFrozen = frozen;
}

void deParticleEmitterController::SetClamp( bool clamp ){
	pClamp = clamp;
}



// Operators
//////////////

deParticleEmitterController &deParticleEmitterController::operator=( const deParticleEmitterController &controller ){
	pName = controller.pName;
	pLower = controller.pLower;
	pUpper = controller.pUpper;
	pValue = controller.pValue;
	pClamp = controller.pClamp;
	pFrozen = controller.pFrozen;
	return *this;
}



// Private functions
//////////////////////

void deParticleEmitterController::pUpdateValue(){
	const float range = pUpper - pLower;
	const bool hasRange = range > 1e-5f;
	
	// make sure the current value is in the correct range
	if( pClamp ){
		if( pValue < pLower ){
			pValue = pLower;
			
		}else if( pValue > pUpper ){
			pValue = pUpper;
		}
		
	}else{
		if( hasRange ){
			pValue = fmodf( pValue - pLower, range );
			if( pValue < 0 ){
				pValue += pLower + range;
				
			}else{
				pValue += pLower;
			}
			
		}else{
			pValue = pLower;
		}
	}
}
