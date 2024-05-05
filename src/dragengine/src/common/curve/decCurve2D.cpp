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
#include <string.h>

#include "decCurve2D.h"

#include "../exceptions.h"



// Class decCurve2D
/////////////////////

// Constructor, destructor
////////////////////////////

decCurve2D::decCurve2D(){
	pPoints = NULL;
	pPointCount = 0;
}

decCurve2D::decCurve2D( const decCurve2D &curve ){
	pPoints = NULL;
	
	if( curve.pPointCount > 0 ){
		pPoints = new decVector2[ curve.pPointCount ];
		if( ! pPoints ){
			DETHROW( deeOutOfMemory );
		}
		
		memcpy( pPoints, curve.pPoints, sizeof( decVector2 ) * curve.pPointCount );
	}
	
	pPointCount = curve.pPointCount;
}

decCurve2D::~decCurve2D(){
	if( pPoints ){
		delete [] pPoints;
	}
}



// Management
///////////////

const decVector2 &decCurve2D::GetPointAt( int position ) const{
	if( position < 0 || position >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pPoints[ position ];
}

int decCurve2D::IndexOfPointClosestTo( float coordinate, float threshold ) const{
	const float lower = coordinate - threshold;
	const float upper = coordinate + threshold;
	int p;
	
	for( p=0; p<pPointCount; p++ ){
		if( pPoints[ p ].x > upper ){
			break;
		}
		if( pPoints[ p ].x >= lower ){
			return p;
		}
	}
	
	return -1;
}

int decCurve2D::IndexOfPointBefore( float coordinate ) const{
	int p;
	
	for( p=0; p<pPointCount; p++ ){
		if( coordinate < pPoints[ p ].x ){
			return p - 1;
		}
	}
	
	return pPointCount - 1;
}

int decCurve2D::AddPoint( const decVector2 &point, float threshold ){
	const int index = IndexOfPointBefore( point.x );
	int p;
	
	if( index != -1 && fabsf( point.x - pPoints[ index ].x ) <= threshold ){
		pPoints[ index ].y = point.y;
		
		return index;
		
	}else{
		decVector2 * const newArray = new decVector2[ pPointCount + 1 ];
		
		for( p=0; p<=index; p++ ){
			newArray[ p ] = pPoints[ p ];
		}
		
		newArray[ index + 1 ] = point;
		
		for( p=index+1; p<pPointCount; p++ ){
			newArray[ p + 1 ] = pPoints[ p ];
		}
		
		if( pPoints ){
			delete [] pPoints;
		}
		pPoints = newArray;
		pPointCount++;
		
		return index + 1;
	}
}

void decCurve2D::RemovePointFrom( int position ){
	if( position < 0 || position >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	int p;
	
	for( p=position+1; p<pPointCount; p++ ){
		pPoints[ p - 1 ] = pPoints[ p ];
	}
	pPointCount--;
}

void decCurve2D::RemoveAllPoints(){
	if( pPoints ){
		delete [] pPoints;
		pPoints = NULL;
	}
	pPointCount = 0;
}



void decCurve2D::SetDefaultCurve(){
	decVector2 * const newArray = new decVector2[ 2 ];
	
	newArray[ 0 ].Set( 0.0f, 0.0f );
	newArray[ 1 ].Set( 1.0f, 1.0f );
	
	if( pPoints ){
		delete [] pPoints;
	}
	pPoints = newArray;
	pPointCount = 2;
}

void decCurve2D::OffsetPointsBy( float offset ){
	int p;
	
	for( p=0; p<pPointCount; p++ ){
		pPoints[ p ].y += offset;
	}
}

void decCurve2D::ScalePointsBy( float scale ){
	int p;
	
	for( p=0; p<pPointCount; p++ ){
		pPoints[ p ].y *= scale;
	}
}



float decCurve2D::EvaluateConstant( float coordinate ) const{
	if( pPointCount == 0 ){
		return 0.0f;
	}
	
	int p;
	
	for( p=1; p<pPointCount; p++ ){
		if( coordinate < pPoints[ p ].x ){
			return pPoints[ p - 1 ].y;
		}
	}
	
	return pPoints[ pPointCount - 1 ].y;
}

float decCurve2D::EvaluateLinear( float coordinate ) const{
	if( pPointCount == 0 ){
		return 0.0f;
	}
	if( pPointCount == 1 ){
		return pPoints[ 0 ].y;
	}
	
	float blend;
	int p;
	
	if( coordinate < pPoints[ 0 ].x ){
		return pPoints[ 0 ].y;
		
	}else{
		for( p=1; p<pPointCount; p++ ){
			if( coordinate < pPoints[ p ].x ){
				blend = ( coordinate - pPoints[ p - 1 ].x ) / ( pPoints[ p ].x - pPoints[ p - 1 ].x );
				return pPoints[ p - 1 ].y * ( 1.0f - blend ) + pPoints[ p ].y * blend;
			}
		}
		
		return pPoints[ pPointCount - 1 ].y;
	}
}



decCurve2D &decCurve2D::operator=( const decCurve2D &curve ){
	decVector2 *newArray = NULL;
	
	if( curve.pPointCount > 0 ){
		newArray = new decVector2[ curve.pPointCount ];
		memcpy( newArray, curve.pPoints, sizeof( decVector2 ) * curve.pPointCount );
	}
	
	if( pPoints ){
		delete [] pPoints;
	}
	pPoints = newArray;
	pPointCount = curve.pPointCount;
	
	return *this;
}
