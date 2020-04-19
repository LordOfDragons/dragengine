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

#include "dedaiNavMeshBuilderEdge.h"

#include <dragengine/common/exceptions.h>



// Class dedaiNavMeshBuilderEdge
//////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiNavMeshBuilderEdge::dedaiNavMeshBuilderEdge(){
	pIndex = 0;
	pVertex1 = 0;
	pVertex2 = 0;
	pFace1 = NULL;
	pFace2 = NULL;
}

dedaiNavMeshBuilderEdge::dedaiNavMeshBuilderEdge( int vertex1, int vertex2 ){
	pIndex = 0;
	pVertex1 = vertex1;
	pVertex2 = vertex2;
	pFace1 = NULL;
	pFace2 = NULL;
}

dedaiNavMeshBuilderEdge::~dedaiNavMeshBuilderEdge(){
}



// Management
///////////////

void dedaiNavMeshBuilderEdge::SetIndex( int index ){
	pIndex = index;
}

void dedaiNavMeshBuilderEdge::SetVertex1( int vertex ){
	pVertex1 = vertex;
}

void dedaiNavMeshBuilderEdge::SetVertex2( int vertex ){
	pVertex2 = vertex;
}

void dedaiNavMeshBuilderEdge::SetFace1( dedaiNavMeshBuilderFace *face ){
	pFace1 = face;
}

void dedaiNavMeshBuilderEdge::SetFace2( dedaiNavMeshBuilderFace *face ){
	pFace2 = face;
}
