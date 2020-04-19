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
#include <string.h>
#include <stdlib.h>
#include "decDefaultBSPTree.h"
#include "decBSPTreeFace.h"
#include "../exceptions.h"



// Class decDefaultBSPTree
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

decDefaultBSPTree::decDefaultBSPTree( const decVector &faceNormal, float distance ) : decBSPTree( faceNormal, distance ){
	pFaces = NULL;
	pFaceCount = 0;
	pFaceSize = 0;
}

decDefaultBSPTree::~decDefaultBSPTree(){
	RemoveAllFaces();
	if( pFaces ) delete [] pFaces;
}



// Management
///////////////

decBSPTree *decDefaultBSPTree::CreateBSPTree( const decVector &faceNormal, float distance ) const{
	decDefaultBSPTree *bsptree = new decDefaultBSPTree( faceNormal, distance );
	if( ! bsptree ) DETHROW( deeOutOfMemory );
	
	return bsptree;
}

void decDefaultBSPTree::ClearNodeContent(){
	RemoveAllFaces();
}

void decDefaultBSPTree::AddCoplanarFace( const decBSPTreeFace &face ){
	int v, vertexCount = face.GetVertexCount();
	decBSPTreeFace *newFace = NULL;
	
	try{
		newFace = new decBSPTreeFace;
		if( ! newFace ) DETHROW( deeOutOfMemory );
		
		for( v=0; v<vertexCount; v++ ){
			newFace->AddVertex( face.GetVertexAt( v ) );
		}
		
		newFace->SetUserData( face.GetUserData() );
		
		AddFace( newFace );
		
	}catch( const deException & ){
		if( newFace ) delete newFace;
		throw;
	}
}



// Faces
//////////

decBSPTreeFace *decDefaultBSPTree::GetFaceAt( int index ) const{
	if( index < 0 || index >= pFaceCount ) DETHROW( deeInvalidParam );
	
	return pFaces[ index ];
}

int decDefaultBSPTree::IndexOfFace( decBSPTreeFace *face ) const{
	int i;
	
	for( i=0; i<pFaceCount; i++ ){
		if( pFaces[ i ] == face ) return i;
	}
	
	return -1;
}

void decDefaultBSPTree::AddFace( decBSPTreeFace *face ){
	if( ! face ) DETHROW( deeInvalidParam );
	
	if( pFaceCount == pFaceSize ){
		int newSize = pFaceSize * 3 / 2 + 1;
		decBSPTreeFace **newArray = new decBSPTreeFace*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pFaces ){
			memcpy( newArray, pFaces, sizeof( decBSPTreeFace* ) * pFaceSize );
			delete [] pFaces;
		}
		pFaces = newArray;
		pFaceSize = newSize;
	}
	
	pFaces[ pFaceCount ] = face;
	pFaceCount++;
}

void decDefaultBSPTree::RemoveFace( decBSPTreeFace *face ){
	int i, index = IndexOfFace( face );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pFaceCount; i++ ){
		pFaces[ i - 1 ] = pFaces[ i ];
	}
	pFaces[ pFaceCount - 1 ] = NULL;
	pFaceCount--;
	
	delete face;
}

void decDefaultBSPTree::RemoveAllFaces(){
	while( pFaceCount > 0 ){
		pFaceCount--;
		delete pFaces[ pFaceCount ];
	}
}
