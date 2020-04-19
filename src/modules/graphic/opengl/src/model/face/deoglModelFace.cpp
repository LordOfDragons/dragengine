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

#include "deoglModelFace.h"

#include <dragengine/common/exceptions.h>


// Class deoglModelFace
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglModelFace::deoglModelFace(){
}

deoglModelFace::~deoglModelFace(){
}



// Management
///////////////

void deoglModelFace::SetVertex1( int vertex ){
	pVertices[ 0 ] = vertex;
}

void deoglModelFace::SetVertex2( int vertex ){
	pVertices[ 1 ] = vertex;
}

void deoglModelFace::SetVertex3( int vertex ){
	pVertices[ 2 ] = vertex;
}

void deoglModelFace::SetVertexAt( int index, int vertex ){
	if( index < 0 || index > 2 ) DETHROW( deeInvalidParam );
	
	pVertices[ index ] = vertex;
}

void deoglModelFace::SetFaceNormal( const decVector &normal ){
	pFaceNormal = normal;
}

void deoglModelFace::SetExtends( const decVector &minExtend, const decVector &maxExtend ){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void deoglModelFace::SetCenter( const decVector &center ){
	pCenter = center;
}

void deoglModelFace::SetTexture( int texture ){
	pTexture = texture;
}

void deoglModelFace::SetIndex( int index ){
	pIndex = index;
}
