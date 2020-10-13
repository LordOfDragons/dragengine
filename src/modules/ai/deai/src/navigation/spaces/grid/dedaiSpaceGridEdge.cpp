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

#include "dedaiSpaceGridEdge.h"

#include <dragengine/common/exceptions.h>



// Class dedaiSpaceGridEdge
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiSpaceGridEdge::dedaiSpaceGridEdge() :
pGrid( NULL ),
pIndex( 0 ),
pVertex1( 0 ),
pVertex2( 0 ),
pTypeNumber1( 0 ),
pTypeNumber2( 0 ),
pLength( 0.0f ){
}

dedaiSpaceGridEdge::~dedaiSpaceGridEdge(){
}



// Management
///////////////

void dedaiSpaceGridEdge::SetGrid( dedaiSpaceGrid *grid ){
	pGrid = grid;
}

void dedaiSpaceGridEdge::SetIndex( unsigned short index ){
	pIndex = index;
}

void dedaiSpaceGridEdge::SetVertex1( unsigned short vertex ){
	pVertex1 = vertex;
}

void dedaiSpaceGridEdge::SetVertex2( unsigned short vertex ){
	pVertex2 = vertex;
}

void dedaiSpaceGridEdge::SetTypeNumber1( unsigned short typeNumber ){
	pTypeNumber1 = typeNumber;
}

void dedaiSpaceGridEdge::SetTypeNumber2( unsigned short typeNumber ){
	pTypeNumber2 = typeNumber;
}

void dedaiSpaceGridEdge::SetLength( float length ){
	pLength = length;
}
