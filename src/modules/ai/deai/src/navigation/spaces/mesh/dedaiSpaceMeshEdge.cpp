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

#include "dedaiSpaceMeshEdge.h"
#include "dedaiSpaceMeshFace.h"

#include <dragengine/common/exceptions.h>



// Class dedaiSpaceMeshEdge
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiSpaceMeshEdge::dedaiSpaceMeshEdge() :
pVertex1( 0 ),
pVertex2( 0 ),
pFace1( -1 ),
pFace2( -1 ){
}

dedaiSpaceMeshEdge::~dedaiSpaceMeshEdge(){
}



// Management
///////////////

void dedaiSpaceMeshEdge::SetVertex1( unsigned short vertex ){
	pVertex1 = vertex;
}

void dedaiSpaceMeshEdge::SetVertex2( unsigned short vertex ){
	pVertex2 = vertex;
}

void dedaiSpaceMeshEdge::SetFace1( short face ){
	pFace1 = face;
}

void dedaiSpaceMeshEdge::SetFace2( short face ){
	pFace2 = face;
}
