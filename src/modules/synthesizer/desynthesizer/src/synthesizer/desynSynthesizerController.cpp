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
