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

#include <stdio.h>
#include <stdlib.h>

#include "decCurveBezier.h"
#include "decCurveBezierPoint.h"

#include "../exceptions.h"



// Class decCurveBezier
/////////////////////////

// Constructor, destructor
////////////////////////////

decCurveBezier::decCurveBezier() :
pPoints( NULL ),
pPointCount( 0 ),
pInterpolationMode( eimBezier ){
}

decCurveBezier::decCurveBezier( const decCurveBezier &curve ) :
pPoints( NULL ),
pPointCount( 0 ),
pInterpolationMode( eimBezier )
{
	*this = curve;
}

decCurveBezier::~decCurveBezier(){
	if( pPoints ){
		delete [] pPoints;
	}
}



// Management
///////////////

const decCurveBezierPoint &decCurveBezier::GetPointAt( int position ) const{
	if( position < 0 || position >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pPoints[ position ];
}

int decCurveBezier::FindPointPriorTo( float coordinate ) const{
	int p;
	
	for( p=0; p<pPointCount; p++ ){
		if( coordinate < pPoints[ p ].GetPoint().x ){
			return p - 1;
		}
	}
	
	return pPointCount - 1;
}

int decCurveBezier::AddPoint( const decCurveBezierPoint &point ){
	const int position = FindPointPriorTo( point.GetPoint().x );
	int p;
	
	decCurveBezierPoint * const newArray = new decCurveBezierPoint[ pPointCount + 1 ];
	
	for( p=0; p<=position; p++ ){
		newArray[ p ] = pPoints[ p ];
	}
	
	newArray[ position + 1 ] = point;
	
	for( p=position+1; p<pPointCount; p++ ){
		newArray[ p + 1 ] = pPoints[ p ];
	}
	
	if( pPoints ) delete [] pPoints;
	pPoints = newArray;
	pPointCount++;
	
	return position + 1;
}

void decCurveBezier::RemovePointFrom( int position ){
	if( position < 0 || position >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	int p;
	
	for( p=position+1; p<pPointCount; p++ ){
		pPoints[ p - 1 ] = pPoints[ p ];
	}
	pPointCount--;
}

void decCurveBezier::RemoveAllPoints(){
	pPointCount = 0;
}



void decCurveBezier::SetInterpolationMode( eInterpolationModes mode ){
	if( mode < eimConstant || mode > eimBezier ){
		DETHROW( deeInvalidParam );
	}
	
	pInterpolationMode = mode;
}



void decCurveBezier::SetDefaultLinear(){
	RemoveAllPoints();
	AddPoint( decCurveBezierPoint( decVector2( 0.0f, 0.0f ),
		decVector2( -0.25f, -0.25f ), decVector2( 0.25f, 0.25f ) ) );
	AddPoint( decCurveBezierPoint( decVector2( 1.0f, 1.0f ),
		decVector2( 0.75f, 0.75f ), decVector2( 1.25f, 1.25f ) ) );
	pInterpolationMode = eimLinear;
}

void decCurveBezier::SetDefaultBezier(){
	RemoveAllPoints();
	AddPoint( decCurveBezierPoint( decVector2( 0.0f, 0.0f ),
		decVector2( -0.25f, 0.0f ), decVector2( 0.25f, 0.0f ) ) );
	AddPoint( decCurveBezierPoint( decVector2( 1.0f, 1.0f ),
		decVector2( 0.75f, 1.0f ), decVector2( 1.25f, 1.0f ) ) );
	pInterpolationMode = eimBezier;
}



// Operators
//////////////

bool decCurveBezier::operator==( const decCurveBezier &curve ) const{
	if( curve.pInterpolationMode != pInterpolationMode ){
		return false;
	}
	
	if( curve.pPointCount != pPointCount ){
		return false;
	}
	
	int i;
	for( i=0; i<pPointCount; i++ ){
		const decCurveBezierPoint &point = curve.GetPointAt( i );
		
		if( ! pPoints[ i ].GetPoint().IsEqualTo( point.GetPoint() ) ){
			return false;
		}
		if( ! pPoints[ i ].GetHandle1().IsEqualTo( point.GetHandle1() ) ){
			return false;
		}
		if( ! pPoints[ i ].GetHandle2().IsEqualTo( point.GetHandle2() ) ){
			return false;
		}
	}
	
	return true;
}

bool decCurveBezier::operator!=( const decCurveBezier &curve ) const{
	return ! ( *this == curve );
}

decCurveBezier &decCurveBezier::operator=( const decCurveBezier &curve ){
	int p, pointCount = curve.GetPointCount();
	decCurveBezierPoint *newArray = NULL;
	
	if( pointCount > 0 ){
		newArray = new decCurveBezierPoint[ pointCount ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		
		for( p=0; p<pointCount; p++ ){
			newArray[ p ] = curve.pPoints[ p ];
		}
	}
	
	if( pPoints ) delete [] pPoints;
	pPoints = newArray;
	pPointCount = pointCount;
	
	pInterpolationMode = curve.pInterpolationMode;
	
	return *this;
}
