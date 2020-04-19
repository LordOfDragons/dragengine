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

#include "deoglDepthTriangleList.h"

#include <dragengine/common/exceptions.h>



// Class deoglDepthTriangleList
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDepthTriangleList::deoglDepthTriangleList(){
	pPoints = NULL;
	pPointCount = 0;
	pPointSize = 0;
}

deoglDepthTriangleList::~deoglDepthTriangleList(){
	if( pPoints ) delete [] pPoints;
}



// Management
///////////////

void deoglDepthTriangleList::AddTriangle( const decVector &p1, const decVector &p2, const decVector &p3 ){
	if( pPointCount + 3 >= pPointSize ){
		int newSize = pPointSize * 3 / 2 + 3;
		oglVector *newArray = new oglVector[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pPoints ){
			memcpy( newArray, pPoints, sizeof( oglVector ) * pPointSize );
			delete [] pPoints;
		}
		pPoints = newArray;
		pPointSize = newSize;
	}
	
	pPoints[ pPointCount ].x = p3.x;
	pPoints[ pPointCount ].y = p3.y;
	pPoints[ pPointCount ].z = p3.z;
	pPointCount++;
	
	pPoints[ pPointCount ].x = p2.x;
	pPoints[ pPointCount ].y = p2.y;
	pPoints[ pPointCount ].z = p2.z;
	pPointCount++;
	
	pPoints[ pPointCount ].x = p1.x;
	pPoints[ pPointCount ].y = p1.y;
	pPoints[ pPointCount ].z = p1.z;
	pPointCount++;
}

void deoglDepthTriangleList::AddTriangle( const decVector &p1, const decVector &p2, const decVector &p3, float maxEdgeLength ){
	float lens[ 3 ] = { ( p2 - p1 ).Length(), ( p3 - p2 ).Length(), ( p1 - p3 ).Length() };
	int largestEdge;
	
	if( lens[ 0 ] > lens[ 1 ] ){
		if( lens[ 0 ] > lens[ 2 ] ){
			largestEdge = 0;
			
		}else{
			largestEdge = 2;
		}
		
	}else if( lens[ 1 ] > lens[ 2 ] ){
		largestEdge = 1;
		
	}else{
		largestEdge = 2;
	}
	
	if( lens[ largestEdge ] > maxEdgeLength ){
		if( largestEdge == 0 ){
			decVector middle( ( p1 + p2 ) * 0.5f );
			AddTriangle( p1, middle, p3, maxEdgeLength );
			AddTriangle( middle, p2, p3, maxEdgeLength );
			
		}else if( largestEdge == 1 ){
			decVector middle( ( p2 + p3 ) * 0.5f );
			AddTriangle( p1, p2, middle, maxEdgeLength );
			AddTriangle( p1, middle, p3, maxEdgeLength );
			
		}else{
			decVector middle( ( p3 + p1 ) * 0.5f );
			AddTriangle( p1, p2, middle, maxEdgeLength );
			AddTriangle( p2, p3, middle, maxEdgeLength );
		}
		
	}else{
		AddTriangle( p1, p2, p3 );
	}
}

void deoglDepthTriangleList::AddTriangleSpherical( const decVector &p1, const decVector &p2, const decVector &p3, float threshold ){
	pAddTriangleSpherical( p1, p1 / p1.Length(), p2, p2 / p2.Length(), p3, p3 / p3.Length(), threshold );
}

void deoglDepthTriangleList::RemoveAllTriangles(){
	pPointCount = 0;
}

void deoglDepthTriangleList::Contract(){
	// todo
}



// Private Functions
//////////////////////

void deoglDepthTriangleList::pAddTriangleSpherical( const decVector &p1, const decVector &pp1, const decVector &p2,
const decVector &pp2, const decVector &p3, const decVector &pp3, float threshold ){
	// the idea behind the spherical splitting is that with non-linear shadow mapping
	// a triangle is projected onto a sphere. in this case artifacts arise if the
	// center of the triangle is too far away from the respective sphere point it
	// would be projected to. using fixed splitting of edges too many triangles are
	// split which would not yield a noticeable artifact if left alone. to simulate
	// this behavior the triangle is projected onto an imaginary sphere and the
	// same splitting done as with the regular splitting algorithm but using the
	// edge length of the projected points instead of the points themselves.
	
	// there exists an annoying situation in which this code runs into a dead loop
	// causing a stack overflow. this happens if the triangle nearly cuts through
	// the center of the sphere. in this case a projected edge can never drop below
	// the threshold resulting in an endless loop. to counter this problem splitting
	// is rejected if the edge is nearly of length 2. this happens only if the edge
	// nearly cuts through the center.
	
	float lens[ 3 ] = { ( pp2 - pp1 ).Length(), ( pp3 - pp2 ).Length(), ( pp1 - pp3 ).Length() };
	int largestEdge;
	
	if( lens[ 0 ] > lens[ 1 ] ){
		if( lens[ 0 ] > lens[ 2 ] ){
			largestEdge = 0;
			
		}else{
			largestEdge = 2;
		}
		
	}else if( lens[ 1 ] > lens[ 2 ] ){
		largestEdge = 1;
		
	}else{
		largestEdge = 2;
	}
	
	if( lens[ largestEdge ] < 1.999f && lens[ largestEdge ] > threshold ){
		if( largestEdge == 0 ){
			decVector middle( ( p1 + p2 ) * 0.5f );
			decVector pmiddle( middle / middle.Length() );
			
			pAddTriangleSpherical( p1, pp1, middle, pmiddle, p3, pp3, threshold );
			pAddTriangleSpherical( middle, pmiddle, p2, pp2, p3, pp3, threshold );
			
		}else if( largestEdge == 1 ){
			decVector middle( ( p2 + p3 ) * 0.5f );
			decVector pmiddle( middle / middle.Length() );
			
			pAddTriangleSpherical( p1, pp1, p2, pp2, middle, pmiddle, threshold );
			pAddTriangleSpherical( p1, pp1, middle, pmiddle, p3, pp3, threshold );
			
		}else{
			decVector middle( ( p3 + p1 ) * 0.5f );
			decVector pmiddle( middle / middle.Length() );
			
			pAddTriangleSpherical( p1, pp1, p2, pp2, middle, pmiddle, threshold );
			pAddTriangleSpherical( p2, pp2, p3, pp3, middle, pmiddle, threshold );
		}
		
	}else{
		AddTriangle( p1, p2, p3 );
	}
}
