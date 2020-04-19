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
#include <stdio.h>

#include "decSmoothVector.h"



// Class decSmoothVector
/////////////////////////

// Constructor, destructor
////////////////////////////

decSmoothVector::decSmoothVector() :
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

decSmoothVector::decSmoothVector( const decSmoothVector &copy ) :
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

decSmoothVector::~decSmoothVector(){
}



// Management
///////////////

void decSmoothVector::SetValue( const decVector &value ){
	pValue = value;
}

void decSmoothVector::SetGoal( const decVector &goal ){
	pGoal = goal;
}

void decSmoothVector::SetAdjustTime( float adjustTime ){
	pAdjustTime = decMath::max( adjustTime, 0.0f );
	pUpdateFactors();
}

void decSmoothVector::SetAdjustRange( float range ){
	pAdjustRange = decMath::max( range, 0.0f );
	pUpdateFactors();
}

void decSmoothVector::SetChangeSpeed( float changeSpeed ){
	pChangeSpeed = changeSpeed;
}



void decSmoothVector::Reset(){
	pValue.SetZero();
	pGoal.SetZero();
	pChangeSpeed = 0.0f;
}

void decSmoothVector::Update( float elapsed ){
	if( elapsed < 0.001f ){
		return;
	}
	
	decVector value( pGoal );
	
	if( pSmoothed ){
		const float factorTime = decMath::min( elapsed * pFactorTime, 1.0f );
		const decVector vectorAdjust( ( value - pValue ) * factorTime );
		const float vectorAdjustLength = vectorAdjust.Length();
		
		if( vectorAdjustLength > FLOAT_SAFE_EPSILON ){
			float adjustValue = vectorAdjustLength;
			const float changeSpeedDifference = ( adjustValue / elapsed ) - pChangeSpeed;
			const float limitChangeSpeed = pFactorLimit * elapsed;
			
			if( changeSpeedDifference > limitChangeSpeed && adjustValue > 0.0f ){
				adjustValue = ( pChangeSpeed + limitChangeSpeed ) * elapsed;
				value = pValue + vectorAdjust * ( adjustValue / vectorAdjustLength );
				
			}else if( changeSpeedDifference < -limitChangeSpeed && adjustValue < 0.0f ){
				adjustValue = ( pChangeSpeed - limitChangeSpeed ) * elapsed;
				value = pValue + vectorAdjust * ( adjustValue / vectorAdjustLength );
				
			}else{
				value = pValue + vectorAdjust;
			}
		}
	}
	
	pChangeSpeed = ( value - pValue ).Length() / elapsed;
	pValue = value;
}



// Operators
//////////////

bool decSmoothVector::operator==( const decSmoothVector &other ) const{
	return pValue.IsEqualTo( other.pValue );
}

bool decSmoothVector::operator!=( const decSmoothVector &other ) const{
	return ! pValue.IsEqualTo( other.pValue );
}

decSmoothVector &decSmoothVector::operator=( const decSmoothVector &other ){
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

void decSmoothVector::pUpdateFactors(){
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
