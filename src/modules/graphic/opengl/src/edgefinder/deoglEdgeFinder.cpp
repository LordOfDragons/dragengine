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
#include "deoglEdgeFinder.h"
#include "deoglEdgeFinderVertex.h"
#include "deoglEdgeFinderEdge.h"
#include "deoglEdgeFinderFace.h"
#include "dragengine/common/exceptions.h"



// class deoglEdgeFinder
//////////////////////////

// constructor, destructor
////////////////////////////

deoglEdgeFinder::deoglEdgeFinder(){
	pVertices = NULL;
	pVertexCount = 0;
	pVertexSize = 0;
	pEdges = NULL;
	pEdgeCount = 0;
	pEdgeSize = 0;
	pFaces = NULL;
	pFaceCount = 0;
	pFaceSize = 0;
}

deoglEdgeFinder::~deoglEdgeFinder(){
	pCleanUp();
}



// management
///////////////

void deoglEdgeFinder::Clear( int vertexCount ){
	if( vertexCount < 0 ) DETHROW( deeInvalidParam );
	pFaceCount = 0;
	pEdgeCount = 0;
	if( vertexCount > pVertexSize ){
		int i;
		deoglEdgeFinderVertex **newArray = new deoglEdgeFinderVertex*[ vertexCount ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		for( i=pVertexSize; i<vertexCount; i++ ) newArray[ i ] = NULL;
		if( pVertices ){
			for( i=0; i<pVertexSize; i++ ) newArray[ i ] = pVertices[ i ];
			delete [] pVertices;
		}
		pVertices = newArray;
		pVertexSize = vertexCount;
	}
	pVertexCount = 0;
	while( pVertexCount < vertexCount ){
		if( ! pVertices[ pVertexCount ] ){
			pVertices[ pVertexCount ] = new deoglEdgeFinderVertex;
			if( ! pVertices[ pVertexCount ] ) DETHROW( deeOutOfMemory );
		}
		pVertices[ pVertexCount ]->Clear();
		pVertexCount++;
	}
}

void deoglEdgeFinder::CalculateEdges(){
	deoglEdgeFinderFace *face;
	deoglEdgeFinderEdge *edge;
	deoglEdgeFinderVertex *vertex;
	int fv1, fv2, i, j, k, count;
	int foundEdge, edgeIndex;
	pEdgeCount = 0;
	for( i=0; i<pFaceCount; i++ ){
		face = pFaces[ i ];
		for( j=0; j<3; j++ ){
			// get vertices
			fv1 = face->GetVertexAt( j );
			fv2 = face->GetVertexAt( ( j + 1 ) % 3 );
			vertex = pVertices[ fv1 ];
			// find edge
			foundEdge = -1;
			count = vertex->GetEdgeCount();
			for( k=0; k<count; k++ ){
				edgeIndex = vertex->GetEdgeAt( k );
				edge = pEdges[ edgeIndex ];
				if( edge->Matches( fv1, fv2 ) && edge->GetFace2() == -1 ){
					edge->SetFace2( i );
					foundEdge = edgeIndex;
					break;
				}
			}
			if( foundEdge == -1 ){
				pAddEdge( fv1, fv2, i );
				foundEdge = pEdgeCount - 1;
				vertex->AddEdge( foundEdge );
				pVertices[ fv2 ]->AddEdge( foundEdge );
			}
			face->SetEdgeAt( j, foundEdge );
		}
	}
}



// vertex management
//////////////////////

deoglEdgeFinderVertex *deoglEdgeFinder::GetVertexAt( int index ) const{
	if( index < 0 || index >= pVertexCount ) DETHROW( deeInvalidParam );
	return pVertices[ index ];
}



// edge management
////////////////////

deoglEdgeFinderEdge *deoglEdgeFinder::GetEdgeAt( int index ) const{
	if( index < 0 || index >= pEdgeCount ) DETHROW( deeInvalidParam );
	return pEdges[ index ];
}



// face management
////////////////////

deoglEdgeFinderFace *deoglEdgeFinder::GetFaceAt( int index ) const{
	if( index < 0 || index >= pFaceCount ) DETHROW( deeInvalidParam );
	return pFaces[ index ];
}

void deoglEdgeFinder::AddFace( int vertex1, int vertex2, int vertex3 ){
	if( vertex1 < 0 || vertex2 < 0 || vertex3 < 0 ) DETHROW( deeInvalidParam );
	if( vertex1 == vertex2 || vertex1 == vertex3 || vertex2 == vertex3 ) DETHROW( deeInvalidParam );
	if( pFaceCount == pFaceSize ){
		int i, newSize = pFaceSize * 3 / 2 + 1;
		deoglEdgeFinderFace **newArray = new deoglEdgeFinderFace*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		for( i=pFaceSize; i<newSize; i++ ) newArray[ i ] = NULL;
		if( pFaces ){
			for( i=0; i<pFaceSize; i++ ) newArray[ i ] = pFaces[ i ];
			delete [] pFaces;
		}
		pFaces = newArray;
		pFaceSize = newSize;
	}
	if( ! pFaces[ pFaceCount ] ){
		pFaces[ pFaceCount ] = new deoglEdgeFinderFace;
		if( ! pFaces[ pFaceCount ] ) DETHROW( deeOutOfMemory );
	}
	pFaces[ pFaceCount ]->Clear( vertex1, vertex2, vertex3 );
	pFaceCount++;
	pVertices[ vertex1 ]->AddFace( pFaceCount - 1 );
	pVertices[ vertex2 ]->AddFace( pFaceCount - 1 );
	pVertices[ vertex3 ]->AddFace( pFaceCount - 1 );
}



// private functions
//////////////////////

void deoglEdgeFinder::pCleanUp(){
	int i;
	if( pFaces ){
		for( i=0; i<pFaceSize; i++ ){
			if( pFaces[ i ] ) delete pFaces[ i ];
		}
		delete [] pFaces;
	}
	if( pEdges ){
		for( i=0; i<pEdgeSize; i++ ){
			if( pEdges[ i ] ) delete pEdges[ i ];
		}
		delete [] pEdges;
	}
	if( pVertices ){
		for( i=0; i<pVertexSize; i++ ){
			if( pVertices[ i ] ) delete pVertices[ i ];
		}
		delete [] pVertices;
	}
}

void deoglEdgeFinder::pAddEdge( int vertex1, int vertex2, int face1 ){
	if( pEdgeCount == pEdgeSize ){
		int i, newSize = pEdgeSize * 3 / 2 + 1;
		deoglEdgeFinderEdge **newArray = new deoglEdgeFinderEdge*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		for( i=pEdgeSize; i<newSize; i++ ) newArray[ i ] = NULL;
		if( pEdges ){
			for( i=0; i<pEdgeSize; i++ ) newArray[ i ] = pEdges[ i ];
			delete [] pEdges;
		}
		pEdges = newArray;
		pEdgeSize = newSize;
	}
	if( ! pEdges[ pEdgeCount ] ){
		pEdges[ pEdgeCount ] = new deoglEdgeFinderEdge;
		if( ! pEdges[ pEdgeCount ] ) DETHROW( deeOutOfMemory );
	}
	pEdges[ pEdgeCount ]->Clear( vertex1, vertex2, face1 );
	pEdgeCount++;
}
