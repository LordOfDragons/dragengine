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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "peeType.h"
#include "peeEmitter.h"
#include "peeParameter.h"
#include "peeController.h"

#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterParameter.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Class peeParameter
///////////////////////

// Constructor, destructor
////////////////////////////

peeParameter::peeParameter( peeType &type, deParticleEmitterType::eParameters index,
	float value, float spread ) :
pType( type ),
pIndex( index ),
pValue( value ),
pSpread( spread ),
pControllerValue( NULL ),
pControllerSpread( NULL ),
pActive( false ){
}

peeParameter::~peeParameter(){
	if( pControllerSpread ){
		pControllerSpread->FreeReference();
	}
	if( pControllerValue ){
		pControllerValue->FreeReference();
	}
}



// Management
///////////////

void peeParameter::SetValue( float value ){
	if( fabsf( value - pValue ) > 1e-5f ){
		pValue = value;
		
		if( pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter() ){
			deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
			deParticleEmitterType &engType = engEmitter.GetTypeAt( pType.GetIndex() );
			
			engType.GetParameter( pIndex ).SetValue( value );
			engEmitter.NotifyTypeChangedAt( pType.GetIndex() );
		}
		
		NotifyChanged();
	}
}

void peeParameter::SetSpread( float spread ){
	if( fabsf( spread - pSpread ) > 1e-5f ){
		pSpread = spread;
		
		if( pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter() ){
			deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
			deParticleEmitterType &engType = engEmitter.GetTypeAt( pType.GetIndex() );
			
			engType.GetParameter( pIndex ).SetSpread( spread );
			engEmitter.NotifyTypeChangedAt( pType.GetIndex() );
		}
		
		NotifyChanged();
	}
}

void peeParameter::SetControllerValue( peeController *controller ){
	if( controller != pControllerValue ){
		if( pControllerValue ){
			pControllerValue->FreeReference();
		}
		pControllerValue = controller;
		if( controller ){
			controller->AddReference();
		}
		
		if( pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter() ){
			deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
			deParticleEmitterType &engType = engEmitter.GetTypeAt( pType.GetIndex() );
			
			engType.GetParameter( pIndex ).SetControllerValue(
				pType.GetEmitter()->GetControllers().IndexOf( controller ) );
			engEmitter.NotifyTypeChangedAt( pType.GetIndex() );
		}
		
		NotifyChanged();
	}
}

void peeParameter::SetCurveValue( const decCurveBezier &curve ){
	pCurveValue = curve;
	
	if( pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter() ){
		deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
		deParticleEmitterType &engType = engEmitter.GetTypeAt( pType.GetIndex() );
		
		engType.GetParameter( pIndex ).GetCurveValue() = curve;
		engEmitter.NotifyTypeChangedAt( pType.GetIndex() );
	}
	
	NotifyChanged();
}

void peeParameter::SetControllerSpread( peeController *controller ){
	if( controller != pControllerSpread ){
		if( pControllerSpread ){
			pControllerSpread->FreeReference();
		}
		pControllerSpread = controller;
		if( controller ){
			controller->AddReference();
		}
		
		if( pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter() ){
			deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
			deParticleEmitterType &engType = engEmitter.GetTypeAt( pType.GetIndex() );
			
			engType.GetParameter( pIndex ).SetControllerSpread(
				pType.GetEmitter()->GetControllers().IndexOf( controller ) );
			engEmitter.NotifyTypeChangedAt( pType.GetIndex() );
		}
		
		NotifyChanged();
	}
}

void peeParameter::SetCurveSpread( const decCurveBezier &curve ){
	pCurveSpread = curve;
	
	if( pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter() ){
		deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
		deParticleEmitterType &engType = engEmitter.GetTypeAt( pType.GetIndex() );
		
		engType.GetParameter( pIndex ).GetCurveSpread() = curve;
		engEmitter.NotifyTypeChangedAt( pType.GetIndex() );
	}
	
	NotifyChanged();
}

void peeParameter::SetCurveProgress( const decCurveBezier &curve ){
	pCurveProgress = curve;
	
	if( pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter() ){
		deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
		deParticleEmitterType &engType = engEmitter.GetTypeAt( pType.GetIndex() );
		
		engType.GetParameter( pIndex ).GetCurveProgress() = curve;
		engEmitter.NotifyTypeChangedAt( pType.GetIndex() );
	}
	
	NotifyChanged();
}

void peeParameter::SetCurveBeam( const decCurveBezier &curve ){
	pCurveBeam = curve;
	
	if( pType.GetEmitter() && pType.GetEmitter()->GetEngineEmitter() ){
		deParticleEmitter &engEmitter = *pType.GetEmitter()->GetEngineEmitter();
		deParticleEmitterType &engType = engEmitter.GetTypeAt( pType.GetIndex() );
		
		engType.GetParameter( pIndex ).GetCurveBeam() = curve;
		engEmitter.NotifyTypeChangedAt( pType.GetIndex() );
	}
	
	NotifyChanged();
}



void peeParameter::SetActive( bool active ){
	pActive = active;
}



void peeParameter::UpdateEngineParameter( deParticleEmitterParameter &parameter ){
	const peeControllerList &list = pType.GetEmitter()->GetControllers();
	parameter.SetValue( pValue );
	parameter.SetSpread( pSpread );
	parameter.GetCurveValue() = pCurveValue;
	parameter.GetCurveSpread() = pCurveSpread;
	parameter.GetCurveProgress() = pCurveProgress;
	parameter.GetCurveBeam() = pCurveBeam;
	parameter.SetControllerValue( list.IndexOf( pControllerValue ) );
	parameter.SetControllerSpread( list.IndexOf( pControllerSpread ) );
}



void peeParameter::NotifyChanged(){
	if( pType.GetEmitter() ){
		pType.GetEmitter()->NotifyTypeParameterChanged( &pType, this );
	}
}
