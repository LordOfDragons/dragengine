/* 
 * Drag[en]gine Bullet Physics Module
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
#include "debpDecalMeshBuilderFace.h"
#include "dragengine/common/exceptions.h"



// Class debpDecalMeshBuilderFace
///////////////////////////////////

// Constructor, destructor
////////////////////////////

debpDecalMeshBuilderFace::debpDecalMeshBuilderFace(){
	pPoints[ 0 ] = 0;
	pPoints[ 1 ] = 0;
	pPoints[ 2 ] = 0;
	pFaceIndex = -1;
}

debpDecalMeshBuilderFace::~debpDecalMeshBuilderFace(){
}



// Management
///////////////

void debpDecalMeshBuilderFace::SetPoint1( int point ){
	pPoints[ 0 ] = point;
}

void debpDecalMeshBuilderFace::SetPoint2( int point ){
	pPoints[ 1 ] = point;
}

void debpDecalMeshBuilderFace::SetPoint3( int point ){
	pPoints[ 2 ] = point;
}

void debpDecalMeshBuilderFace::SetFaceIndex( int faceIndex ){
	pFaceIndex = faceIndex;
}
