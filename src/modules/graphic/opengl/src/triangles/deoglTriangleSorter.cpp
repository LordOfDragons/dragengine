/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <string.h>

#include "deoglTriangleSorter.h"

#include <dragengine/common/exceptions.h>



// Class deoglTriangleSorter
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTriangleSorter::deoglTriangleSorter() :
pTriangles( NULL ),
pTriangleCount( 0 ),
pTriangleSize( 0 ){
}

deoglTriangleSorter::~deoglTriangleSorter(){
	pCleanUp();
}



// Management
///////////////

const decVector &deoglTriangleSorter::GetTriangleVertex1( int triangle ) const{
	if( triangle < 0 || triangle >= pTriangleCount ){
		DETHROW( deeInvalidParam );
	}
	return pTriangles[ triangle ]->vertex1;
}

const decVector &deoglTriangleSorter::GetTriangleVertex2( int triangle ) const{
	if( triangle < 0 || triangle >= pTriangleCount ){
		DETHROW( deeInvalidParam );
	}
	return pTriangles[ triangle ]->vertex2;
}

const decVector &deoglTriangleSorter::GetTriangleVertex3( int triangle ) const{
	if( triangle < 0 || triangle >= pTriangleCount ){
		DETHROW( deeInvalidParam );
	}
	return pTriangles[ triangle ]->vertex3;
}

void deoglTriangleSorter::AddTriangle( const decVector &vertex1,
const decVector &vertex2, const decVector &vertex3 ){
	if( pTriangleCount == pTriangleSize ){
		const int newSize = pTriangleSize * 3 / 2 + 1;
		
		sTriangle ** const newArray = new sTriangle*[ newSize ];
		if( pTriangles ){
			memcpy( newArray, pTriangles, sizeof( sTriangle* ) * pTriangleSize );
			delete [] pTriangles;
		}
		
		pTriangles = newArray;
		
		while( pTriangleSize < newSize ){
			pTriangles[ pTriangleSize++ ] = new sTriangle;
		}
	}
	
	sTriangle &triangle = *pTriangles[ pTriangleCount++ ];
	triangle.vertex1 = vertex1;
	triangle.vertex2 = vertex2;
	triangle.vertex3 = vertex3;
	triangle.distance = 0.0f;
}

void deoglTriangleSorter::RemoveAllTriangles(){
	pTriangleCount = 0;
}



// Sorting
////////////

void deoglTriangleSorter::SortLinear( const decVector &position, const decVector &view ){
	// calculate distance to reference point using view direction as projection axis
	const float factor = 1.0f / 3.0f;
	int i;
	
	for( i=0; i<pTriangleCount; i++ ){
		sTriangle &t = *pTriangles[ i ];
		const decVector triangleCenter( ( t.vertex1 + t.vertex2 + t.vertex3 ) * factor );
		const decVector triangleView( triangleCenter - position );
		t.distance = triangleView * view;
	}
	
	// sort by distance
	if( pTriangleCount > 0 ){
		pQuickSortDistance( 0, pTriangleCount - 1 );
	}
}

void deoglTriangleSorter::SortRadial( const decVector &position ){
	// calculate distance to reference point. squared distance is enough so we do
	// not have to calculate the square root.
	const float factor = 1.0f / 3.0f;
	int i;
	
	for( i=0; i<pTriangleCount; i++ ){
		sTriangle &t = *pTriangles[ i ];
		const decVector triangleCenter( ( t.vertex1 + t.vertex2 + t.vertex3 ) * factor );
		const decVector direction( triangleCenter - position );
		t.distance = direction * direction;
	}
	
	// sort by distance
	if( pTriangleCount > 0 ){
		pQuickSortDistance( 0, pTriangleCount - 1 );
	}
	
	// debug
	/*
	printf( "[TRIANGLESORTER] ( %i Triangles )\n", pTriangleCount );
	for( i=0; i<pTriangleCount; i++ ){
		printf( "%g, ", pTriangles[ i ]->distance );
	}
	printf( "\n\n" );
	*/
}



// Private Functions
//////////////////////

void deoglTriangleSorter::pCleanUp(){
	if( pTriangles ){
		while( pTriangleSize > 0 ){
			pTriangleSize--;
			delete pTriangles[ pTriangleSize ];
		}
		delete [] pTriangles;
	}
}

void deoglTriangleSorter::pQuickSortDistance( int left, int right ){
	sTriangle * const pivotFace = pTriangles[ left ];
	const float pivot = pivotFace->distance;
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( ( pTriangles[ right ]->distance >= pivot ) && ( left < right ) ){
			right--;
		}
		if( left != right ){
			pTriangles[ left ] = pTriangles[ right ];
			left++;
		}
		while( ( pTriangles[ left ]->distance <= pivot ) && ( left < right ) ){
			left++;
		}
		if( left != right ){
			pTriangles[ right ] = pTriangles[ left ];
			right--;
		}
	}
	
	pTriangles[ left ] = pivotFace;
	if( l_hold < left ){
		pQuickSortDistance( l_hold, left - 1 );
	}
	if( r_hold > left ){
		pQuickSortDistance( left + 1, r_hold );
	}
}
