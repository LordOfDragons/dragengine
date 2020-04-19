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
#include "deoglEdgeFinderFace.h"
#include "dragengine/common/exceptions.h"



// class deoglEdgeFinderFace
////////////////////////////////

// constructor, destructor
////////////////////////////

deoglEdgeFinderFace::deoglEdgeFinderFace(){
	pVertex[ 0 ] = 0;
	pVertex[ 1 ] = 0;
	pVertex[ 2 ] = 0;
	pEdge[ 0 ] = 0;
	pEdge[ 1 ] = 0;
	pEdge[ 2 ] = 0;
}

deoglEdgeFinderFace::~deoglEdgeFinderFace(){
}



// management
///////////////

void deoglEdgeFinderFace::SetVertex1( int index ){
	if( index < 0 ) DETHROW( deeInvalidParam );
	pVertex[ 0 ] = index;
}

void deoglEdgeFinderFace::SetVertex2( int index ){
	if( index < 0 ) DETHROW( deeInvalidParam );
	pVertex[ 1 ] = index;
}

void deoglEdgeFinderFace::SetVertex3( int index ){
	if( index < 0 ) DETHROW( deeInvalidParam );
	pVertex[ 2 ] = index;
}

int deoglEdgeFinderFace::GetVertexAt( int vertex ) const{
	if( vertex < 0 || vertex > 2 ) DETHROW( deeInvalidParam );
	return pVertex[ vertex ];
}

void deoglEdgeFinderFace::SetVertexAt( int vertex, int index ){
	if( vertex < 0 || vertex > 2 ) DETHROW( deeInvalidParam );
	pVertex[ vertex ] = index;
}

void deoglEdgeFinderFace::SetEdge1( int index ){
	if( index < 0 ) DETHROW( deeInvalidParam );
	pEdge[ 0 ] = index;
}

void deoglEdgeFinderFace::SetEdge2( int index ){
	if( index < -1 ) DETHROW( deeInvalidParam );
	pEdge[ 1 ] = index;
}

void deoglEdgeFinderFace::SetEdge3( int index ){
	if( index < -1 ) DETHROW( deeInvalidParam );
	pEdge[ 2 ] = index;
}

int deoglEdgeFinderFace::GetEdgeAt( int edge ) const{
	if( edge < 0 || edge > 2 ) DETHROW( deeInvalidParam );
	return pEdge[ edge ];
}

void deoglEdgeFinderFace::SetEdgeAt( int edge, int index ){
	if( edge < 0 || edge > 2 ) DETHROW( deeInvalidParam );
	pEdge[ edge ] = index;
}

void deoglEdgeFinderFace::Clear( int vertex1, int vertex2, int vertex3 ){
	if( vertex1 < 0 || vertex2 < 0 || vertex3 < 0 ) DETHROW( deeInvalidParam );
	pVertex[ 0 ] = vertex1;
	pVertex[ 1 ] = vertex2;
	pVertex[ 2 ] = vertex3;
	pEdge[ 0 ] = 0;
	pEdge[ 1 ] = 0;
	pEdge[ 2 ] = 0;
}
