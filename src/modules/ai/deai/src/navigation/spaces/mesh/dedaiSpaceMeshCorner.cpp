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

#include "dedaiSpaceMeshCorner.h"
#include "dedaiSpaceMeshFace.h"

#include <dragengine/common/exceptions.h>



// Class dedaiSpaceMeshCorner
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiSpaceMeshCorner::dedaiSpaceMeshCorner() :
pVertex( 0 ),
pEdge( 0 ),
pTypeNumber( 0 ),
pLink( -1 ),
pEnabled( true ){
}

dedaiSpaceMeshCorner::~dedaiSpaceMeshCorner(){
}



// Management
///////////////

void dedaiSpaceMeshCorner::SetVertex( unsigned short vertex ){
	pVertex = vertex;
}

void dedaiSpaceMeshCorner::SetEdge( unsigned short edge ){
	pEdge = edge;
}

void dedaiSpaceMeshCorner::SetTypeNumber( unsigned short type ){
	pTypeNumber = type;
}

void dedaiSpaceMeshCorner::SetLink( short link ){
	pLink = link;
}

void dedaiSpaceMeshCorner::SetEnabled( bool enabled ){
	pEnabled = enabled;
}



// Operators
//////////////

dedaiSpaceMeshCorner &dedaiSpaceMeshCorner::operator=( const dedaiSpaceMeshCorner &corner ){
	pVertex = corner.pVertex;
	pEdge = corner.pEdge;
	pTypeNumber = corner.pTypeNumber;
	pLink = corner.pLink;
	pEnabled = corner.pEnabled;
	return *this;
}
