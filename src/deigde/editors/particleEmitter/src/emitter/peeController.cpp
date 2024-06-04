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
