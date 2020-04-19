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

#include "deSynthesizerController.h"

#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deSynthesizerController
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerController::deSynthesizerController() :
pMinValue( 0.0f ),
pMaxValue( 1.0f ),
pClamp( true ){
}

deSynthesizerController::deSynthesizerController( const deSynthesizerController &controller ) :
pName( controller.pName ),
pMinValue( controller.pMinValue ),
pMaxValue( controller.pMaxValue ),
pClamp( controller.pClamp ),
pCurve( controller.pCurve ){
}

deSynthesizerController::~deSynthesizerController(){
}



// Management
///////////////

void deSynthesizerController::SetName( const char *name ){
	pName = name;
}

void deSynthesizerController::SetValueRange( float minValue, float maxValue ){
	pMinValue = minValue;
	pMaxValue = decMath::max( maxValue, minValue );
}

void deSynthesizerController::SetClamp( bool clamp ){
	pClamp = clamp;
}

void deSynthesizerController::SetCurve( const decCurveBezier &curve ){
	pCurve = curve;
}



// Operators
//////////////

deSynthesizerController &deSynthesizerController::operator=( const deSynthesizerController &controller ){
	pName = controller.pName;
	pMinValue = controller.pMinValue;
	pMaxValue = controller.pMaxValue;
	pCurve = controller.pCurve;
	pClamp = controller.pClamp;
	return *this;
}
