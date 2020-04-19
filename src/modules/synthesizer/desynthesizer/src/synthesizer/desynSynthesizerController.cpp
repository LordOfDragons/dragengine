/* 
 * Drag[en]gine Synthesizer Module
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

#include "desynSynthesizerController.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizerController.h>



// Class desynSynthesizerController
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerController::desynSynthesizerController() :
pMinValue( 0.0f ),
pMaxValue( 1.0f ),
pClamp( true ),
pDirty( false ),
pValues( NULL ),
pValueSize( 0 ){
}

desynSynthesizerController::~desynSynthesizerController(){
	if( pValues ){
		delete [] pValues;
	}
}



// Management
///////////////

float desynSynthesizerController::GetValue( int sample ) const{
	if( sample < 0 || sample >= pValueSize ){
		DETHROW( deeInvalidParam );
	}
	return pValues[ sample ];
}

void desynSynthesizerController::UpdateValues( int samples, float time, float range ){
	if( samples < 1 ){
		return;
	}
	
	if( samples > pValueSize ){
		if( pValues ){
			delete [] pValues;
			pValues = NULL;
		}
		pValueSize = 0;
		
		pValues = new float[ samples ];
		pValueSize = samples;
	}
	
	int i;
	
	for( i=0; i<samples; i++ ){
		pValues[ i ] = pCurve.Evaluate( time + range * ( float )i );
	}
}

void desynSynthesizerController::SetDirty( bool dirty ){
	pDirty = dirty;
}

void desynSynthesizerController::Update( const deSynthesizerController &controller ){
	pMinValue = controller.GetMinimumValue();
	pMaxValue = controller.GetMaximumValue();
	pClamp = controller.GetClamp();
	pCurve.SetCurve( controller.GetCurve(), controller.GetMinimumValue(), controller.GetMaximumValue() );
	pDirty = false;
}
