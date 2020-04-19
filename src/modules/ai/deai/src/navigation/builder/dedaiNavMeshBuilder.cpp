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
#include <string.h>

#include "dedaiNavMeshBuilder.h"
#include "dedaiNavMeshBuilderEdge.h"
#include "dedaiNavMeshBuilderFace.h"

#include <dragengine/common/exceptions.h>



// Class dedaiNavMeshBuilder
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiNavMeshBuilder::dedaiNavMeshBuilder(){
}

dedaiNavMeshBuilder::~dedaiNavMeshBuilder(){
	Clear();
}



// Management
///////////////

dedaiNavMeshBuilderEdge *dedaiNavMeshBuilder::GetEdgeWith( int vertex1, int vertex2 ){
	dedaiNavMeshBuilderEdge *edge = NULL;
	const int count = pEdges.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		edge = ( dedaiNavMeshBuilderEdge* )pEdges.GetAt( i );
		
		if( ( edge->GetVertex1() == vertex1 && edge->GetVertex2() == vertex2 )
		||  ( edge->GetVertex1() == vertex2 && edge->GetVertex2() == vertex1 ) ){
			return edge;
		}
	}
	
	edge = NULL;
	
	try{
		edge = new dedaiNavMeshBuilderEdge( vertex1, vertex2 );
		pEdges.Add( edge );
		
	}catch( const deException & ){
		if( edge ){
			delete edge;
		}
		throw;
	}
	
	return edge;
}

dedaiNavMeshBuilderFace *dedaiNavMeshBuilder::AddFace(){
	dedaiNavMeshBuilderFace *face = NULL;
	
	try{
		face = new dedaiNavMeshBuilderFace;
		pFaces.Add( face );
		
	}catch( const deException & ){
		if( face ){
			delete face;
		}
		throw;
	}
	
	return face;
}

void dedaiNavMeshBuilder::UpdateIndices(){
	int i, count;
	
	count = pEdges.GetCount();
	for( i=0; i<count; i++ ){
		( ( dedaiNavMeshBuilderEdge* )pEdges.GetAt( i ) )->SetIndex( i );
	}
	
	count = pFaces.GetCount();
	for( i=0; i<count; i++ ){
		( ( dedaiNavMeshBuilderFace* )pFaces.GetAt( i ) )->SetIndex( i );
	}
}

void dedaiNavMeshBuilder::Clear(){
	int i, count;
	
	count = pFaces.GetCount();
	for( i=0; i<count; i++ ){
		delete ( dedaiNavMeshBuilderFace* )pFaces.GetAt( i );
	}
	pFaces.RemoveAll();
	
	count = pEdges.GetCount();
	for( i=0; i<count; i++ ){
		delete ( dedaiNavMeshBuilderEdge* )pEdges.GetAt( i );
	}
	pEdges.RemoveAll();
}
