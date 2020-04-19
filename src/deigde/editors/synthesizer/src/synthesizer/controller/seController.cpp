/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include "seController.h"
#include "../seSynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>
#include <dragengine/resources/synthesizer/deSynthesizerController.h>



// Class seController
///////////////////////

// Constructor, destructor
////////////////////////////

seController::seController( const char *name ) :
pSynthesizer( NULL ),
pEngControllerIndex( -1 ),

pName( name ),

pMinValue( 0.0f ),
pMaxValue( 1.0f ),
pClamp( true ),

pEditConstantValue( 0.0f ){
}

seController::seController( const seController &copy ) :
pSynthesizer( NULL ),
pEngControllerIndex( -1 ),

pName( copy.pName ),

pMinValue( copy.pMinValue ),
pMaxValue( copy.pMaxValue ),
pClamp( copy.pClamp ),

pCurve( copy.pCurve ),

pEditConstantValue( copy.pEditConstantValue ){
}

seController::~seController(){
	pCleanUp();
}



// Management
///////////////

void seController::SetSynthesizer( seSynthesizer *synthesizer ){
	if( synthesizer == pSynthesizer ){
		return;
	}
	
	pSynthesizer = synthesizer;
	pEngControllerIndex = -1;
}

void seController::SetEngineControllerIndex( int index ){
	if( index == pEngControllerIndex ){
		return;
	}
	
	pEngControllerIndex = index;
	if( index == -1 ){
		return;
	}
	
	deSynthesizerInstance &instance = *pSynthesizer->GetEngineSynthesizerInstance();
	deSynthesizerController &controller = *instance.GetControllerAt( index );
	
	controller.SetValueRange( pMinValue, pMaxValue );
	controller.SetClamp( pClamp );
	controller.SetCurve( pCurve );
	
	instance.NotifyControllerChangedAt( index );
}



void seController::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	
	if( pSynthesizer ){
		pSynthesizer->NotifyControllerNameChanged( this );
	}
}



void seController::SetMinimumValue( float value ){
	if( fabsf( value - pMinValue ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	if( value > pMaxValue ){
		pMinValue = value;
		pMaxValue = value;
		
	}else{
		pMinValue = value;
	}
	
	if( pEngControllerIndex != -1 ){
		deSynthesizerInstance &instance = *pSynthesizer->GetEngineSynthesizerInstance();
		deSynthesizerController &controller = *instance.GetControllerAt( pEngControllerIndex );
		
		controller.SetValueRange( pMinValue, pMaxValue );
		
		instance.NotifyControllerChangedAt( pEngControllerIndex );
	}
	
	if( pSynthesizer ){
		pSynthesizer->NotifyControllerRangeChanged( this );
	}
}

void seController::SetMaximumValue( float value ){
	if( fabsf( value - pMaxValue ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	if( value < pMinValue ){
		pMinValue = value;
		pMaxValue = value;
		
	}else{
		pMaxValue = value;
	}
	
	if( pEngControllerIndex != -1 ){
		deSynthesizerInstance &instance = *pSynthesizer->GetEngineSynthesizerInstance();
		deSynthesizerController &controller = *instance.GetControllerAt( pEngControllerIndex );
		
		controller.SetValueRange( pMinValue, pMaxValue );
		
		instance.NotifyControllerChangedAt( pEngControllerIndex );
	}
	
	if( pSynthesizer ){
		pSynthesizer->NotifyControllerRangeChanged( this );
	}
}

void seController::SetClamp( bool clamp ){
	if( clamp == pClamp ){
		return;
	}
	
	pClamp = clamp;
	
	if( pEngControllerIndex != -1 ){
		deSynthesizerInstance &instance = *pSynthesizer->GetEngineSynthesizerInstance();
		deSynthesizerController &controller = *instance.GetControllerAt( pEngControllerIndex );
		
		controller.SetClamp( clamp );
		
		instance.NotifyControllerChangedAt( pEngControllerIndex );
	}
	
	if( pSynthesizer ){
		pSynthesizer->NotifyControllerChanged( this );
	}
}

void seController::SetCurve( const decCurveBezier &curve ){
	if( curve == pCurve ){
		return;
	}
	
	pCurve = curve;
	
	if( pEngControllerIndex != -1 ){
		deSynthesizerInstance &instance = *pSynthesizer->GetEngineSynthesizerInstance();
		deSynthesizerController &controller = *instance.GetControllerAt( pEngControllerIndex );
		
		controller.SetCurve( curve );
		
		instance.NotifyControllerChangedAt( pEngControllerIndex );
	}
	
	if( pSynthesizer ){
		pSynthesizer->NotifyControllerCurveChanged( this );
	}
}

void seController::SetEditConstantValue( float value ){
	pEditConstantValue = value;
}



// Operators
//////////////

seController &seController::operator=( const seController &controller ){
	SetName( controller.pName );
	SetMinimumValue( controller.pMinValue );
	SetMaximumValue( controller.pMaxValue );
	SetClamp( controller.pClamp );
	SetCurve( controller.pCurve );
	SetEditConstantValue( controller.pEditConstantValue );
	return *this;
}



// Private Functions
//////////////////////

void seController::pCleanUp(){
	SetSynthesizer( NULL );
}
