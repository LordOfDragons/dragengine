/* 
 * Drag[en]gine Android Launcher
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

#include "decSmoothFloat.h"
#include "../decMath.h"



// Class decSmoothFloat
/////////////////////////

// Constructor, destructor
////////////////////////////

decSmoothFloat::decSmoothFloat() :
pValue( 0.0f ),
pGoal( 0.0f ),
pAdjustTime( 1.0f ),
pAdjustRange( 1.0f ),
pChangeSpeed( 0.0f ),
pFactorTime( 0.0f ),
pFactorLimit( 0.0f ),
pFactorDrop( 0.0f ),
pSmoothed( true )
{
	pUpdateFactors();
}

decSmoothFloat::decSmoothFloat( const decSmoothFloat &copy ) :
pValue( copy.pValue ),
pGoal( copy.pGoal ),
pAdjustTime( copy.pAdjustTime ),
pAdjustRange( copy.pAdjustRange ),
pChangeSpeed( copy.pChangeSpeed ),
pFactorTime( copy.pFactorTime ),
pFactorLimit( copy.pFactorLimit ),
pFactorDrop( copy.pFactorDrop ),
pSmoothed( copy.pSmoothed )
{
}

decSmoothFloat::~decSmoothFloat(){
}



// Management
///////////////

void decSmoothFloat::SetValue( float value ){
	pValue = value;
}

void decSmoothFloat::SetGoal( float goal ){
	pGoal = goal;
}

void decSmoothFloat::SetAdjustTime( float adjustTime ){
	pAdjustTime = decMath::max( adjustTime, 0.0f );
	pUpdateFactors();
}

void decSmoothFloat::SetAdjustRange( float limit ){
	pAdjustRange = decMath::max( limit, 0.0f );
	pUpdateFactors();
}

void decSmoothFloat::SetChangeSpeed( float changeSpeed ){
	pChangeSpeed = changeSpeed;
}



void decSmoothFloat::Reset(){
	pValue = 0.0f;
	pGoal = 0.0f;
	pChangeSpeed = 0.0f;
}

void decSmoothFloat::Update( float elapsed ){
	if( elapsed < 0.001f ){
		return;
	}
	
	float value = pGoal;
	
	if( pSmoothed ){
		const float factorTime = decMath::min( elapsed * pFactorTime, 1.0f );
		const float maxAdjust = value - pValue;
		float adjustValue = maxAdjust * factorTime;
		
		const float drop = pFactorDrop * elapsed;
		if( maxAdjust > 0.0f ){
			adjustValue += drop;
			if( adjustValue > maxAdjust ){
				adjustValue = maxAdjust;
			}
			
		}else{
			adjustValue -= drop;
			if( adjustValue < maxAdjust ){
				adjustValue = maxAdjust;
			}
		}
		
		const float changeSpeedDifference = ( adjustValue / elapsed ) - pChangeSpeed;
		
		const float limitChangeSpeed = pFactorLimit * elapsed;
		
		if( changeSpeedDifference > limitChangeSpeed && pValue < value ){
			adjustValue = ( pChangeSpeed + limitChangeSpeed ) * elapsed;
			
		}else if( changeSpeedDifference < -limitChangeSpeed && pValue > value ){
			adjustValue = ( pChangeSpeed - limitChangeSpeed ) * elapsed;
		}
		
		value = pValue + adjustValue;
	}
	
	pChangeSpeed = ( value - pValue ) / elapsed;
	pValue = value;
}



// Operators
//////////////

bool decSmoothFloat::operator==( const decSmoothFloat &other ) const{
	return fabsf( pValue - other.pValue ) < FLOAT_SAFE_EPSILON;
}

bool decSmoothFloat::operator!=( const decSmoothFloat &other ) const{
	return fabsf( pValue - other.pValue ) >= FLOAT_SAFE_EPSILON;
}

decSmoothFloat &decSmoothFloat::operator=( const decSmoothFloat &other ){
	pValue = other.pValue;
	pGoal = other.pGoal;
	pAdjustTime = other.pAdjustTime;
	pAdjustRange = other.pAdjustRange;
	pChangeSpeed = other.pChangeSpeed;
	pFactorTime = other.pFactorTime;
	pFactorLimit = other.pFactorLimit;
	pFactorDrop = other.pFactorDrop;
	pSmoothed = other.pSmoothed;
	return *this;
}



// Private Functions
//////////////////////

void decSmoothFloat::pUpdateFactors(){
	if( pAdjustTime < 0.001f ){
		pFactorTime = 0.0f;
		pFactorLimit = 0.0f;
		pFactorDrop = 0.0f;
		pSmoothed = false;
		
	}else{
		pFactorTime = 4.0f / pAdjustTime;
		pFactorLimit = powf( 4.0f, 1.0f + logf( 1.0f / pAdjustTime ) / logf( 2.0f ) ) * pAdjustRange;
		pFactorDrop = pAdjustRange * 0.0025f / pAdjustTime; // 0.25% drop of entire range over entire time
		pSmoothed = true;
	}
}
