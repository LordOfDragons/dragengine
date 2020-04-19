/* 
 * Drag[en]gine Game Engine
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

#include "deNavigationSpaceEdge.h"
#include "../../../common/exceptions.h"



// Class deNavigationSpaceEdge
////////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationSpaceEdge::deNavigationSpaceEdge(){
	pVertex1 = 0;
	pVertex2 = 0;
	pType1 = 0;
	pType2 = 0;
}

deNavigationSpaceEdge::~deNavigationSpaceEdge(){
}



// Management
///////////////

void deNavigationSpaceEdge::SetVertex1( unsigned short vertex ){
	pVertex1 = vertex;
}

void deNavigationSpaceEdge::SetVertex2( unsigned short vertex ){
	pVertex2 = vertex;
}

void deNavigationSpaceEdge::SetType1( unsigned short type ){
	pType1 = type;
}

void deNavigationSpaceEdge::SetType2( unsigned short type ){
	pType2 = type;
}
