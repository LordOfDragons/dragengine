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
#include <string.h>
#include "decBSPTreeFace.h"
#include "../exceptions.h"



// Class decBSPTreeFace
/////////////////////////

// Constructor, destructor
////////////////////////////

decBSPTreeFace::decBSPTreeFace(){
	pVertices = NULL;
	pVertexCount = 0;
	pVertexSize = 0;
	pUserData = NULL;
	
	pVertices = new decVector[ 3 ];
	if( ! pVertices ) DETHROW( deeOutOfMemory );
	pVertexSize = 3;
}

decBSPTreeFace::~decBSPTreeFace(){
	if( pVertices ) delete [] pVertices;
}



// Management
///////////////

const decVector &decBSPTreeFace::GetVertexAt( int index ) const{
	if( index < 0 || index >= pVertexCount ) DETHROW( deeInvalidParam );
	
	return pVertices[ index ];
}

void decBSPTreeFace::AddVertex( const decVector &vertex ){
	if( pVertexCount == pVertexSize ){
		int newSize = pVertexSize * 3 / 2 + 1;
		decVector *newArray = new decVector[ newSize ];
		if( pVertices ){
			memcpy( newArray, pVertices, sizeof( decVector ) * pVertexSize );
			delete [] pVertices;
		}
		pVertices = newArray;
		pVertexSize = newSize;
	}
	
	pVertices[ pVertexCount ] = vertex;
	pVertexCount++;
}

void decBSPTreeFace::RemoveAllVertices(){
	pVertexCount = 0;
}

void decBSPTreeFace::SetUserData( void *userData ){
	pUserData = userData;
}
