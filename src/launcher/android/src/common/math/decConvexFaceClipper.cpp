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

#include <stdio.h>
#include <stdlib.h>
#include "decConvexFaceClipper.h"
#include "../exceptions.h"



// Definitions
////////////////

#define EQUALITY_THRESHOLD	1e-4f



// Class decConvexFaceClipper
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

decConvexFaceClipper::decConvexFaceClipper(){
	pVertices = NULL;
	pVertexCount = 0;
}

decConvexFaceClipper::~decConvexFaceClipper(){
	if( pVertices ) delete [] pVertices;
}



// Management
///////////////

void decConvexFaceClipper::SetNormal( const decDVector &normal ){
	pNormal = normal;
}

const decDVector &decConvexFaceClipper::GetVertexAt( int position ) const{
	if( position < 0 || position >= pVertexCount ) DETHROW( deeInvalidParam );
	
	return pVertices[ position ];
}

bool decConvexFaceClipper::HasVertex( const decDVector &vertex ) const{
	int i;
	
	for( i=0; i<pVertexCount; i++ ){
		if( vertex.IsEqualTo( pVertices[ i ] ) ) return true;
	}
	
	return false;
}

int decConvexFaceClipper::IndexOfVertex( const decDVector &vertex ) const{
	int i;
	
	for( i=0; i<pVertexCount; i++ ){
		if( vertex.IsEqualTo( pVertices[ i ] ) ) return i;
	}
	
	return -1;
}

void decConvexFaceClipper::AddVertex( const decDVector &vertex ){
	decDVector *newArray = new decDVector[ pVertexCount + 1 ];
	if( ! newArray ) DETHROW( deeOutOfMemory );
	
	if( pVertices ){
		int i;
		for( i=0; i<pVertexCount; i++ ) newArray[ i ] = pVertices[ i ];
		delete [] pVertices;
	}
	pVertices = newArray;
	
	pVertices[ pVertexCount ] = vertex;
	pVertexCount++;
}

void decConvexFaceClipper::RemoveAllVertices(){
	if( pVertices ){
		delete [] pVertices;
		pVertices = NULL;
	}
	pVertexCount = 0;
}



void decConvexFaceClipper::ClipByPlane( const decDVector &planeNormal, const decDVector &planePosition ){
	double planeDot = planeNormal * planePosition;
	decDVector edgeDirection, cutVertexPosition;
	decDVector *oldVertices = pVertices;
	int oldVertexCount = pVertexCount;
	int sideFirst, sideSecond;
	double dotFirst, dotSecond;
	int vertexFirst, vertexSecond, v;
	double lambda;
	
	pVertices = NULL;
	pVertexCount = 0;
	
	try{
		for( v=0; v<oldVertexCount; v++ ){
			// get the first and second vertex of this edge
			vertexFirst = v;
			vertexSecond = ( v + 1 ) % oldVertexCount;
			
			// determine if the vertices are in front of the split plane
			dotFirst = planeNormal * oldVertices[ vertexFirst ] - planeDot;
			if( dotFirst > EQUALITY_THRESHOLD ){
				sideFirst = 1;
				
			}else if( dotFirst < -EQUALITY_THRESHOLD ){
				sideFirst = -1;
				
			}else{
				sideFirst = 0;
			}
			
			dotSecond = planeNormal * oldVertices[ vertexSecond ] - planeDot;
			if( dotSecond > EQUALITY_THRESHOLD ){
				sideSecond = 1;
				
			}else if( dotSecond < -EQUALITY_THRESHOLD ){
				sideSecond = -1;
				
			}else{
				sideSecond = 0;
			}
			
			// add vertex if front side or on the cut plane
			if( sideFirst != -1 ){
				AddVertex( oldVertices[ vertexFirst ] );
			}
			
			// if both are on different sides add cut vertex
			if( sideFirst != 0 && sideSecond != 0 && sideFirst != sideSecond ){
				// calculate the cut vertex
				edgeDirection = oldVertices[ vertexSecond ] - oldVertices[ vertexFirst ];
				lambda = ( planeDot - planeNormal * oldVertices[ vertexFirst ] )
					/ ( planeNormal * edgeDirection );
				cutVertexPosition = oldVertices[ vertexFirst ] + edgeDirection * lambda;
				
				// add vertex
				AddVertex( cutVertexPosition );
			}
		}
		
		// old vertices no more needed
		if( oldVertices ) delete [] oldVertices;
		
	}catch( const deException & ){
		if( oldVertices ) delete [] oldVertices;
		throw;
	}
}

void decConvexFaceClipper::Transform( const decDMatrix &matrix ){
	int v;
	
	for( v=0; v<pVertexCount; v++ ){
		pVertices[ v ] = matrix * pVertices[ v ];
	}
	
	pNormal = matrix.TransformNormal( pNormal );
}
