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

#include "decSmoothDouble.h"
#include "../decMath.h"



// Class decSmoothDouble
//////////////////////////

// Constructor, destructor
////////////////////////////

decSmoothDouble::decSmoothDouble() :
pValue( 0.0 ),
pGoal( 0.0 ),
pAdjustTime( 1.0 ),
pAdjustRange( 1.0 ),
pChangeSpeed( 0.0 ),
pFactorTime( 0.0 ),
pFactorLimit( 0.0 ),
pFactorDrop( 0.0 ),
pSmoothed( true )
{
	pUpdateFactors();
}

decSmoothDouble::decSmoothDouble( const decSmoothDouble &copy ) :
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

decSmoothDouble::~decSmoothDouble(){
}



// Management
///////////////

void decSmoothDouble::SetValue( double value ){
	pValue = value;
}

void decSmoothDouble::SetGoal( double goal ){
	pGoal = goal;
}

void decSmoothDouble::SetAdjustTime( double adjustTime ){
	pAdjustTime = decMath::max( adjustTime, 0.0 );
	pUpdateFactors();
}

void decSmoothDouble::SetAdjustRange( double range ){
	pAdjustRange = decMath::max( range, 0.0 );
	pUpdateFactors();
}

void decSmoothDouble::SetChangeSpeed( double changeSpeed ){
	pChangeSpeed = changeSpeed;
}



void decSmoothDouble::Reset(){
	pValue = 0.0;
	pGoal = 0.0;
	pChangeSpeed = 0.0;
}

void decSmoothDouble::Update( double elapsed ){
	if( elapsed < 0.001 ){
		return;
	}
	
	double value = pGoal;
	
	if( pSmoothed ){
		const double factorTime = decMath::min( elapsed * pFactorTime, 1.0 );
		const double maxAdjust = value - pValue;
		double adjustValue = maxAdjust * factorTime;
		
		const double drop = pFactorDrop * elapsed;
		if( maxAdjust > 0.0 ){
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
		
		const double changeSpeedDifference = ( adjustValue / elapsed ) - pChangeSpeed;
		
		const double limitChangeSpeed = pFactorLimit * elapsed;
		
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

bool decSmoothDouble::operator==( const decSmoothDouble &other ) const{
	return fabs( pValue - other.pValue ) < DOUBLE_SAFE_EPSILON;
}

bool decSmoothDouble::operator!=( const decSmoothDouble &other ) const{
	return fabs( pValue - other.pValue ) >= DOUBLE_SAFE_EPSILON;
}

decSmoothDouble &decSmoothDouble::operator=( const decSmoothDouble &other ){
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

void decSmoothDouble::pUpdateFactors(){
	if( pAdjustTime < 0.001 ){
		pFactorTime = 0.0;
		pFactorLimit = 0.0;
		pFactorDrop = 0.0f;
		pSmoothed = false;
		
	}else{
		pFactorTime = 4.0 / pAdjustTime;
		pFactorLimit = pow( 4.0, 1.0 + log( 1.0 / pAdjustTime ) / log( 2.0 ) ) * pAdjustRange;
		pFactorDrop = pAdjustRange * 0.0025 / pAdjustTime; // 0.25% drop of entire range over entire time
		pSmoothed = true;
	}
}
