/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#include "peeEmitter.h"
#include "peeController.h"

#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterController.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>



// Class peeController
////////////////////////

// Constructor, destructor
////////////////////////////

peeController::peeController(){
	pEmitter = NULL;
	pEngControllerIndex = -1;
	
	pName = "Controller";
	pLower = 0.0f;
	pUpper = 1.0f;
	pValue = 0.0f;
	pClamp = true;
	pFrozen = false;
	pLinkToTime = false;
}

peeController::~peeController(){
	SetEmitter( NULL );
}



// Management
///////////////

void peeController::SetEmitter( peeEmitter *emitter ){
	pEmitter = emitter;
}

void peeController::UpdateEngineController( deParticleEmitterController &controller ){
	controller.SetName( pName );
	controller.SetRange( pLower, pUpper );
	controller.SetValue( pValue );
	controller.SetFrozen( pFrozen );
	controller.SetClamp( pClamp );
}

void peeController::SetEngineControllerIndex( int index ){
	pEngControllerIndex = index;
}



void peeController::SetName( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pName.Equals( name ) ){
		pName = name;
		
		if( pEmitter ){
			pEmitter->NotifyControllerNameChanged( this );
		}
	}
}

void peeController::SetLower( float value ){
	if( fabsf( value - pLower ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	if( value > pUpper ){
		pLower = value;
		pUpper = value;
		
	}else{
		pLower = value;
	}
	
	pUpdateRange();
	
	if( pEmitter ){
		pEmitter->NotifyControllerChanged( this );
		pEmitter->NotifyControllerValueChanged( this );
	}
}

void peeController::SetUpper( float value ){
	if( fabsf( value - pUpper ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	if( value < pLower ){
		pLower = value;
		pUpper = value;
		
	}else{
		pUpper = value;
	}
	
	pUpdateRange();
	
	if( pEmitter ){
		pEmitter->NotifyControllerChanged( this );
		pEmitter->NotifyControllerValueChanged( this );
	}
}

void peeController::SetValue( float value ){
	if( pFrozen ){
		return;
	}
	
	value = pCheckValue( value );
	if( fabsf( value - pValue ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pValue = value;
	
	if( pEmitter && pEmitter->GetEngineEmitter() ){
		deParticleEmitterInstance &engEmitterInstance = *pEmitter->GetEngineEmitterInstance();
		deParticleEmitterController &engController = engEmitterInstance.GetControllerAt( pEngControllerIndex );
		
		engController.SetValue( pValue );
		engEmitterInstance.NotifyControllerChangedAt( pEngControllerIndex );
	}
	
	if( pEmitter ){
		pEmitter->NotifyControllerValueChanged( this );
	}
}

void peeController::IncrementValue( float amount ){
	SetValue( pValue + amount );
}

void peeController::SetFrozen( bool frozen ){
	if( frozen != pFrozen ){
		pFrozen = frozen;
		
		if( pEmitter && pEmitter->GetEngineEmitter() ){
			deParticleEmitterInstance &engEmitterInstance = *pEmitter->GetEngineEmitterInstance();
			deParticleEmitterController &engController = engEmitterInstance.GetControllerAt( pEngControllerIndex );
			
			engController.SetFrozen( pFrozen );
			engController.SetRange( pLower, pUpper );
			engController.SetValue( pValue );
			
			engEmitterInstance.NotifyControllerChangedAt( pEngControllerIndex );
		}
		
		if( pEmitter ){
			pEmitter->NotifyControllerChanged( this );
		}
	}
}

void peeController::SetClamp( bool clamp ){
	if( clamp != pClamp ){
		pClamp = clamp;
		
		if( pEmitter && pEmitter->GetEngineEmitter() ){
			deParticleEmitterInstance &engEmitterInstance = *pEmitter->GetEngineEmitterInstance();
			deParticleEmitterController &engController = engEmitterInstance.GetControllerAt( pEngControllerIndex );
			
			engController.SetClamp( pClamp );
			
			engEmitterInstance.NotifyControllerChangedAt( pEngControllerIndex );
		}
		
		if( pEmitter ){
			pEmitter->NotifyControllerChanged( this );
		}
	}
}

void peeController::SetLinkToTime( bool linkToTime ){
	if( linkToTime != pLinkToTime ){
		pLinkToTime = linkToTime;
		
		if( pEmitter ){
			pEmitter->NotifyControllerChanged( this );
		}
	}
}



void peeController::SetActive( bool active ){
	pActive = active;
}



void peeController::UpdateValue( float elapsed ){
	if( pLinkToTime ){
		IncrementValue( elapsed );
	}
}



// Private Functions
//////////////////////

float peeController::pCheckValue( float value ){
	if( pClamp ){
		if( value < pLower ){
			value = pLower;
			
		}else if( value > pUpper ){
			value = pUpper;
		}
		
	}else{
		float range = pUpper - pLower;
		
		if( range > 1e-5f ){
			value = fmodf( value - pLower, range );
			
			if( value < 0.0f ){
				value += pLower + range;
				
			}else{
				value += pLower;
			}
			
		}else{
			value = pLower;
		}
	}
	
	return value;
}

void peeController::pUpdateRange(){
	if( pEmitter && pEmitter->GetEngineEmitter() ){
		deParticleEmitterInstance &engEmitterInstance = *pEmitter->GetEngineEmitterInstance();
		deParticleEmitterController &engController = engEmitterInstance.GetControllerAt( pEngControllerIndex );
		
		engController.SetRange( pLower, pUpper );
		engController.SetValue( pValue );
		
		engEmitterInstance.NotifyControllerChangedAt( pEngControllerIndex );
	}
}
