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

#include "deoglDecalMeshBuilderFace.h"

#include <dragengine/common/exceptions.h>



// Class deoglDecalMeshBuilderFace
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDecalMeshBuilderFace::deoglDecalMeshBuilderFace(){
	pPoints[ 0 ] = 0;
	pPoints[ 1 ] = 0;
	pPoints[ 2 ] = 0;
}

deoglDecalMeshBuilderFace::~deoglDecalMeshBuilderFace(){
}



// Management
///////////////

int deoglDecalMeshBuilderFace::GetPointAt( int index ) const{
	if( index < 0 || index > 2 ){
		DETHROW( deeInvalidParam );
	}
	return pPoints[ index ];
}

void deoglDecalMeshBuilderFace::SetPoint1( int point ){
	pPoints[ 0 ] = point;
}

void deoglDecalMeshBuilderFace::SetPoint2( int point ){
	pPoints[ 1 ] = point;
}

void deoglDecalMeshBuilderFace::SetPoint3( int point ){
	pPoints[ 2 ] = point;
}

void deoglDecalMeshBuilderFace::SetFaceNormal( const decVector &normal ){
	pNormal = normal;
}
