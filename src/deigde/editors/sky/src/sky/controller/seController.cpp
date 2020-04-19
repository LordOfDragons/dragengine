/* 
 * Drag[en]gine IGDE Sky Editor
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
#include "../seSky.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSkyController.h>
#include <dragengine/resources/sky/deSkyInstance.h>



// Class seController
///////////////////////

// Constructor, destructor
////////////////////////////

seController::seController() :
pSky( NULL ),
pIndex( -1 ),

pName( "Controller" ),

pMinValue( 0.0f ),
pMaxValue( 1.0f ),
pCurValue( 0.0f ),
pClamp( true ),
pFrozen( false ),

pSelected( false ),
pActive( false ){
}

seController::~seController(){
}



// Management
///////////////

void seController::SetSky( seSky *sky ){
	pSky = sky;
}

void seController::SetIndex( int index ){
	pIndex = index;
}

void seController::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	if( pSky ){
		pSky->NotifyControllerNameChanged( this );
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
	
	pCurValue = pCheckValue( pCurValue );
	
	if( pSky ){
		pSky->NotifyControllerChanged( this );
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
	
	pCurValue = pCheckValue( pCurValue );
	
	if( pSky ){
		pSky->NotifyControllerChanged( this );
	}
}

void seController::SetCurrentValue( float value ){
	if( pFrozen ){
		return;
	}
	
	value = pCheckValue( value );
	if( fabsf( value - pCurValue ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pCurValue = value;
	
	if( pSky && pSky->GetEngineSkyInstance() ){
		pSky->GetEngineSkyInstance()->GetControllerAt( pIndex ).SetCurrentValue( pCurValue );
		pSky->GetEngineSkyInstance()->NotifyControllerChangedAt( pIndex );
	}
	
	if( pSky ){
		pSky->NotifyControllerValueChanged( this );
	}
}

void seController::SetFrozen( bool frozen ){
	if( frozen == pFrozen ){
		return;
	}
	
	pFrozen = frozen;
	if( pSky ){
		pSky->NotifyControllerChanged( this );
	}
}

void seController::SetClamp( bool clamp ){
	if( clamp == pClamp ){
		return;
	}
	
	pClamp = clamp;
	if( pSky ){
		pSky->NotifyControllerChanged( this );
	}
}

void seController::SetSelected( bool selected ){
	pSelected = selected;
}

void seController::SetActive( bool active ){
	pActive = active;
}



// Private Functions
//////////////////////

float seController::pCheckValue( float value ){
	if( pClamp ){
		return decMath::clamp( value, pMinValue, pMaxValue );
		
	}else{
		return decMath::normalize( value, pMinValue, pMaxValue );
	}
}
