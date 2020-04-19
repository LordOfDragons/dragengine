/* 
 * Drag[en]gine AI Module
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

#include "dedaiConvexFace.h"
#include "dedaiConvexFaceList.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define EQUALITY_THRESHOLD 1e-3f



// Class dedaiConvexFace
////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiConvexFace::dedaiConvexFace() :
pVertices( NULL ),
pVertexCount( 0 ),
pMarker( 0 ){
}

dedaiConvexFace::dedaiConvexFace( const dedaiConvexFace &face ) :
pVertices( NULL ),
pVertexCount( face.pVertexCount ),
pNormal( face.pNormal ),
pMarker( face.pMarker )
{
	if( pVertexCount > 0 ){
		pVertices = new int[ pVertexCount ];
		memcpy( pVertices, face.pVertices, sizeof( int ) * pVertexCount );
	}
}

dedaiConvexFace::~dedaiConvexFace(){
	if( pVertices ){
		delete [] pVertices;
	}
}



// Management
///////////////

void dedaiConvexFace::SetNormal( const decVector &normal ){
	pNormal = normal;
}

void dedaiConvexFace::SetMarker( int marker ){
	pMarker = marker;
}

int dedaiConvexFace::GetVertexAt( int index ) const{
	if( index < 0 || index >= pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	return pVertices[ index ];
}

void dedaiConvexFace::SetVertexAt( int index, int vertex ){
	if( index < 0 || index >= pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	pVertices[ index ] = vertex;
}

bool dedaiConvexFace::HasVertex( int vertex ) const{
	int i;
	
	for( i=0; i<pVertexCount; i++ ){
		if( vertex == pVertices[ i ] ){
			return true;
		}
	}
	
	return false;
}

int dedaiConvexFace::IndexOfVertex( int vertex ) const{
	int i;
	
	for( i=0; i<pVertexCount; i++ ){
		if( vertex == pVertices[ i ] ){
			return i;
		}
	}
	
	return -1;
}

void dedaiConvexFace::AddVertex( int vertex ){
	int * const newArray = new int[ pVertexCount + 1 ];
	if( pVertices ){
		memcpy( newArray, pVertices, sizeof( int ) * pVertexCount );
		delete [] pVertices;
	}
	pVertices = newArray;
	pVertices[ pVertexCount ] = vertex;
	pVertexCount++;
}

void dedaiConvexFace::InsertVertex( int index, int vertex ){
	if( index < 0 || index > pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	
	int * const newArray = new int[ pVertexCount + 1 ];
	if( pVertices ){
		if( index > 0 ){
			memcpy( newArray, pVertices, sizeof( int ) * index );
		}
		if( index < pVertexCount ){
			memcpy( newArray + index + 1, pVertices + index,
				sizeof( int ) * ( pVertexCount - index ) );
		}
		delete [] pVertices;
	}
	pVertices = newArray;
	pVertices[ index ] = vertex;
	pVertexCount++;
}

void dedaiConvexFace::RemoveVertexFrom( int index ){
	if( index < 0 || index >= pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=index+1; i<pVertexCount; i++ ){
		pVertices[ i - 1 ] = pVertices[ i ];
	}
	pVertexCount--;
}

void dedaiConvexFace::RemoveAllVertices(){
	if( pVertices ){
		delete [] pVertices;
		pVertices = NULL;
	}
	pVertexCount = 0;
}



decVector dedaiConvexFace::CalculateCenter( const dedaiConvexFaceList &convexFaceList ) const{
	decVector center;
	
	if( pVertexCount > 0 ){
		int i;
		
		for( i=0; i<pVertexCount; i++ ){
			center += convexFaceList.GetVertexAt( pVertices[ i ] );
		}
		
		center /= ( float )pVertexCount;
	}
	
	return center;
}

void dedaiConvexFace::SortVertices( const dedaiConvexFaceList &convexFaceList ){
	// sort vertices clockwise around the face normal. for this the dot product of all vertices is
	// calculated with the direction from the center to the first vertex. the vertex with the
	// largest dot product wins. this is the vertex with the smallest angle relative to the vector
	// from the center to the first vertex. for this test only vertices along the clockwise
	// direction are considered. these are all vertices that have a positive dot product between
	// the direction from the center to the vertex in question and the cross product of the face
	// normal and the vector from the center to the first vertex.
	// 
	// it is important to normalize the vectors. this is unfortunately required since otherwise
	// the result can be falsified by vertices located nearly at the same location in space
	// causing an additional loop around the face resulting in an incorrect ordering.
	if( pVertexCount < 3 ){
		return;
	}
	
	int i, j;
	
	const decVector center = CalculateCenter( convexFaceList );
	
	const decVector referenceDirection = ( convexFaceList.GetVertexAt( pVertices[ 0 ] ) - center ).Normalized();
	const decVector windingDirection = pNormal % referenceDirection;
	const float windingDot = windingDirection * center - FLOAT_SAFE_EPSILON;
	
	for( i=1; i<pVertexCount-1; i++ ){
		int nextVertex = -1;
		float nextDot = 0.0f;
		
		for( j=i; j<pVertexCount; j++ ){
			const decVector &testVertex = convexFaceList.GetVertexAt( pVertices[ j ] );
			
			float testDot = ( testVertex - center ).Normalized() * referenceDirection;
			if( testVertex * windingDirection < windingDot ){
				testDot = -2.0f - testDot;
			}
			
			if( nextVertex == -1 || testDot > nextDot ){
				nextVertex = j;
				nextDot = testDot;
			}
		}
		
		if( nextVertex != -1 ){
			const int exchange = pVertices[ i ];
			pVertices[ i ] = pVertices[ nextVertex ];
			pVertices[ nextVertex ] = exchange;
		}
	}
}

bool dedaiConvexFace::IsTooSmall( const dedaiConvexFaceList &convexFaceList ) const{
	if( pVertexCount < 3 ){
		return true;
	}
	
	const decVector center = CalculateCenter( convexFaceList );
	int i;
	
	for( i=0; i<pVertexCount; i++ ){
		if( ( convexFaceList.GetVertexAt( pVertices[ i ] ) - center ).Length() < FLOAT_SAFE_EPSILON ){
			return true;
		}
	}
	
	return false;
}



// Operators
//////////////

dedaiConvexFace &dedaiConvexFace::operator=( const dedaiConvexFace &face ){
	const int vertexCount = face.pVertexCount;
	
	pMarker = face.pMarker;
	pNormal = face.pNormal;
	
	if( vertexCount != pVertexCount ){
		if( pVertices ){
			delete [] pVertices;
			pVertices = NULL;
			pVertexCount = 0;
		}
		
		if( vertexCount > 0 ){
			pVertices = new int[ vertexCount ];
			memcpy( pVertices, face.pVertices, sizeof( int ) * vertexCount );
		}
		
		pVertexCount = vertexCount;
	}
	
	return *this;
}
