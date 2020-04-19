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
#include "deoglEdgeFinderEdge.h"
#include "dragengine/common/exceptions.h"



// class deoglEdgeFinderEdge
////////////////////////////////

// constructor, destructor
////////////////////////////

deoglEdgeFinderEdge::deoglEdgeFinderEdge(){
	pVertex[ 0 ] = 0;
	pVertex[ 1 ] = 0;
	pFace[ 0 ] = 0;
	pFace[ 1 ] = -1;
	pCrease = false;
}

deoglEdgeFinderEdge::~deoglEdgeFinderEdge(){
}



// management
///////////////

void deoglEdgeFinderEdge::SetVertex1( int index ){
	if( index < 0 ) DETHROW( deeInvalidParam );
	pVertex[ 0 ] = index;
}

void deoglEdgeFinderEdge::SetVertex2( int index ){
	if( index < 0 ) DETHROW( deeInvalidParam );
	pVertex[ 1 ] = index;
}

void deoglEdgeFinderEdge::SetFace1( int index ){
	if( index < 0 ) DETHROW( deeInvalidParam );
	pFace[ 0 ] = index;
}

void deoglEdgeFinderEdge::SetFace2( int index ){
	if( index < -1 ) DETHROW( deeInvalidParam );
	pFace[ 1 ] = index;
}

void deoglEdgeFinderEdge::SetCrease( bool crease ){
	pCrease = crease;
}

bool deoglEdgeFinderEdge::Matches( int vertex1, int vertex2 ) const{
	return ( vertex1 == pVertex[ 0 ] && vertex2 == pVertex[ 1 ] )
		|| ( vertex1 == pVertex[ 1 ] && vertex2 == pVertex[ 0 ] );
}

void deoglEdgeFinderEdge::Clear( int vertex1, int vertex2, int face1 ){
	if( vertex1 < 0 || vertex2 < 0 || face1 < 0 ) DETHROW( deeInvalidParam );
	pVertex[ 0 ] = vertex1;
	pVertex[ 1 ] = vertex2;
	pFace[ 0 ] = face1;
	pFace[ 1 ] = -1;
	pCrease = false;
}
