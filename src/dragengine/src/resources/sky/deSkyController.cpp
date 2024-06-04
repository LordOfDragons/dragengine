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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deSkyController.h"

#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deSkyController
//////////////////////////

// Constructor, destructor
////////////////////////////

deSkyController::deSkyController() :
pMinValue( 0.0f ),
pMaxValue( 1.0f ),
pCurValue( 0.0f ),
pClamp( true ),
pFrozen( false ){
}

deSkyController::deSkyController( const deSkyController &controller ) :
pName( controller.pName ),
pMinValue( controller.pMinValue ),
pMaxValue( controller.pMaxValue ),
pCurValue( controller.pCurValue ),
pClamp( controller.pClamp ),
pFrozen( controller.pFrozen ){
}

deSkyController::~deSkyController(){
}



// Management
///////////////

void deSkyController::SetName( const char *name ){
	pName = name;
}

void deSkyController::SetValueRange( float minValue, float maxValue ){
	if( pFrozen ){
		return;
	}
	
	pMinValue = minValue;
	pMaxValue = decMath::max( maxValue, minValue );
	
	pUpdateValue();
}

void deSkyController::SetCurrentValue( float value ){
	if( pFrozen ){
		return;
	}
	
	pCurValue = value;
	
	pUpdateValue();
}

void deSkyController::IncrementCurrentValue( float incrementBy ){
	if( pFrozen ){
		return;
	}
	
	pCurValue += incrementBy;
	
	pUpdateValue();
}

void deSkyController::SetClamp( bool clamp ){
	pClamp = clamp;
}

void deSkyController::SetFrozen( bool frozen ){
	pFrozen = frozen;
}



// Operators
//////////////

deSkyController &deSkyController::operator=( const deSkyController &controller ){
	pName = controller.pName;
	pMinValue = controller.pMinValue;
	pMaxValue = controller.pMaxValue;
	pCurValue = controller.pCurValue;
	pClamp = controller.pClamp;
	pFrozen = controller.pFrozen;
	return *this;
}



// Private functions
//////////////////////

void deSkyController::pUpdateValue(){
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
