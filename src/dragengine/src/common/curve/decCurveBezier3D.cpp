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

#include "decCurveBezier3D.h"
#include "decCurveBezier3DPoint.h"

#include "../exceptions.h"



// Class decCurveBezier3D
///////////////////////////

// Constructor, destructor
////////////////////////////

decCurveBezier3D::decCurveBezier3D() :
pPoints( NULL ),
pPointCount( 0 ),
pInterpolationMode( eimBezier ){
}

decCurveBezier3D::decCurveBezier3D( const decCurveBezier3D &copy ) :
pPoints( NULL ),
pPointCount( 0 ),
pInterpolationMode( eimBezier )
{
	*this = copy;
}

decCurveBezier3D::~decCurveBezier3D(){
	if( pPoints ){
		delete [] pPoints;
	}
}



// Management
///////////////

decCurveBezier3DPoint &decCurveBezier3D::GetPointAt( int position ){
	if( position < 0 || position >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pPoints[ position ];
}

const decCurveBezier3DPoint &decCurveBezier3D::GetPointAt( int position ) const{
	if( position < 0 || position >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pPoints[ position ];
}

void decCurveBezier3D::AddPoint( const decCurveBezier3DPoint &point ){
	int p;
	
	decCurveBezier3DPoint *newArray = new decCurveBezier3DPoint[ pPointCount + 1 ];
	
	for( p=0; p<pPointCount; p++ ){
		newArray[ p ] = pPoints[ p ];
	}
	newArray[ pPointCount ] = point;
	
	if( pPoints ){
		delete [] pPoints;
	}
	pPoints = newArray;
	pPointCount++;
}

void decCurveBezier3D::InsertPoint( int position, const decCurveBezier3DPoint &point ){
	if( position < 0 || position > pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	int p;
	
	decCurveBezier3DPoint *newArray = new decCurveBezier3DPoint[ pPointCount + 1 ];
	
	for( p=0; p<=position; p++ ){
		newArray[ p ] = pPoints[ p ];
	}
	newArray[ position + 1 ] = point;
	for( p=position+1; p<pPointCount; p++ ){
		newArray[ p + 1 ] = pPoints[ p ];
	}
	
	if( pPoints ){
		delete [] pPoints;
	}
	pPoints = newArray;
	pPointCount++;
}

void decCurveBezier3D::RemovePointFrom( int position ){
	if( position < 0 || position >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	int p;
	
	for( p=position+1; p<pPointCount; p++ ){
		pPoints[ p - 1 ] = pPoints[ p ];
	}
	pPointCount--;
}

void decCurveBezier3D::RemoveAllPoints(){
	pPointCount = 0;
}



void decCurveBezier3D::SetInterpolationMode( eInterpolationModes mode ){
	if( mode < eimConstant || mode > eimBezier ){
		DETHROW( deeInvalidParam );
	}
	
	pInterpolationMode = mode;
}



// Operators
//////////////

decCurveBezier3DPoint &decCurveBezier3D::operator[]( int position ){
	return GetPointAt( position );
}

const decCurveBezier3DPoint &decCurveBezier3D::operator[]( int position ) const{
	return GetPointAt( position );
}

decCurveBezier3D &decCurveBezier3D::operator=( const decCurveBezier3D &curve ){
	int p, pointCount = curve.GetPointCount();
	decCurveBezier3DPoint *newArray = NULL;
	
	if( pointCount > 0 ){
		newArray = new decCurveBezier3DPoint[ pointCount ];
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

bool decCurveBezier3D::operator==( const decCurveBezier3D &curve ) const{
	if( curve.pPointCount != pPointCount ){
		return false;
	}
	
	if( pInterpolationMode != curve.pInterpolationMode ){
		return false;
	}
	
	int i;
	for( i=0; i<pPointCount; i++ ){
		if( pPoints[ i ] != curve.pPoints[ i ] ){
			return false;
		}
	}
	
	return true;
}
