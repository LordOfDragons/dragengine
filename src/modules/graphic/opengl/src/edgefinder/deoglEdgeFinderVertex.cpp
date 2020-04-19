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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deoglEdgeFinderVertex.h"
#include "dragengine/common/exceptions.h"



// class deoglEdgeFinderVertex
////////////////////////////////

// constructor, destructor
////////////////////////////

deoglEdgeFinderVertex::deoglEdgeFinderVertex(){
	pEdges = NULL;
	pEdgeCount = 0;
	pEdgeSize = 0;
	pFaces = NULL;
	pFaceCount = 0;
	pFaceSize = 0;
	try{
		pEdges = new int[ 10 ];
		if( ! pEdges ) DETHROW( deeOutOfMemory );
		pEdgeSize = 10;
		pFaces = new int[ 10 ];
		if( ! pFaces ) DETHROW( deeOutOfMemory );
		pFaceSize = 10;
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglEdgeFinderVertex::~deoglEdgeFinderVertex(){
	pCleanUp();
}



// management
///////////////

void deoglEdgeFinderVertex::Clear(){
	RemoveAllEdges();
	RemoveAllFaces();
}



// edges
//////////

int deoglEdgeFinderVertex::GetEdgeAt( int index ) const{
	if( index < 0 || index >= pEdgeCount ) DETHROW( deeInvalidParam );
	return pEdges[ index ];
}

bool deoglEdgeFinderVertex::HasEdge( int index ) const{
	int i;
	for( i=0; i<pEdgeCount; i++ ){
		if( pEdges[ i ] == index ) return true;
	}
	return false;
}

void deoglEdgeFinderVertex::AddEdge( int index ){
	if( pEdgeCount == pEdgeSize ){
		int i, newSize = pEdgeSize * 3 / 2 + 1;
		int *newArray = new int[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pEdges ){
			for( i=0; i<pEdgeCount; i++ ) newArray[ i ] = pEdges[ i ];
			delete [] pEdges;
		}
		pEdges = newArray;
		pEdgeSize = newSize;
	}
	pEdges[ pEdgeCount ] = index;
	pEdgeCount++;
}

void deoglEdgeFinderVertex::RemoveAllEdges(){
	pEdgeCount = 0;
}



// faces
//////////

int deoglEdgeFinderVertex::GetFaceAt( int index ) const{
	if( index < 0 || index >= pFaceCount ) DETHROW( deeInvalidParam );
	return pFaces[ index ];
}

bool deoglEdgeFinderVertex::HasFace( int index ) const{
	int i;
	for( i=0; i<pFaceCount; i++ ){
		if( pFaces[ i ] == index ) return true;
	}
	return false;
}

void deoglEdgeFinderVertex::AddFace( int index ){
	if( pFaceCount == pFaceSize ){
		int i, newSize = pFaceSize * 3 / 2 + 1;
		int *newArray = new int[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pFaces ){
			for( i=0; i<pFaceCount; i++ ) newArray[ i ] = pFaces[ i ];
			delete [] pFaces;
		}
		pFaces = newArray;
		pFaceSize = newSize;
	}
	pFaces[ pFaceCount ] = index;
	pFaceCount++;
}

void deoglEdgeFinderVertex::RemoveAllFaces(){
	pFaceCount = 0;
}



// private functions
//////////////////////

void deoglEdgeFinderVertex::pCleanUp(){
	if( pEdges ) delete [] pEdges;
	if( pFaces ) delete [] pFaces;
}
