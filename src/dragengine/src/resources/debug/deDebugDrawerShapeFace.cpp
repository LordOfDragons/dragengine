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
#include <string.h>

#include "deDebugDrawerShapeFace.h"
#include "../../common/exceptions.h"



// Class deDebugDrawerShapeFace
/////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deDebugDrawerShapeFace::deDebugDrawerShapeFace() :
pVertices( NULL ),
pVertexCount( 0 ),
pNormal( 0.0f, 0.0f, 1.0f ){
}

deDebugDrawerShapeFace::~deDebugDrawerShapeFace(){
	if( pVertices ){
		delete [] pVertices;
	}
}



// Management
///////////////

void deDebugDrawerShapeFace::SetNormal( const decVector &normal ){
	pNormal = normal;
	pNormal.Normalize();
}

void deDebugDrawerShapeFace::CalculateNormal(){
	if( pVertexCount < 3 ){
		return;
	}
	pNormal = ( ( pVertices[ 1 ] - pVertices[ 0 ] ) % ( pVertices[ 2 ] - pVertices[ 1 ] ) ).Normalized();
}

const decVector &deDebugDrawerShapeFace::GetVertexAt( int index ) const{
	if( index < 0 || index >= pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	return pVertices[ index ];
}

void deDebugDrawerShapeFace::SetVertexAt( int index, const decVector &vertex ){
	if( index < 0 || index >= pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	pVertices[ index ] = vertex;
}

void deDebugDrawerShapeFace::AddVertex( const decVector &vertex ){
	decVector * const newArray = new decVector[ pVertexCount + 1 ];
	
	if( pVertices ){
		memcpy( newArray, pVertices, sizeof( decVector ) * pVertexCount );
		delete [] pVertices;
	}
	
	pVertices = newArray;
	pVertices[ pVertexCount ] = vertex;
	pVertexCount++;
}

void deDebugDrawerShapeFace::RemoveAllVertices(){
	if( pVertices ){
		delete [] pVertices;
		pVertices = NULL;
	}
	pVertexCount = 0;
}


	
// Helper Functions
/////////////////////

void deDebugDrawerShapeFace::MakeCoplanarTo( const decVector &position ){
	int i;
	for( i=0; i<pVertexCount; i++ ){
		pVertices[ i ] += pNormal * ( pNormal * ( position - pVertices[ i ] ) );
	}
}
