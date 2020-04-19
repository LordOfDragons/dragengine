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

#include "dedaiSpaceGridVertex.h"

#include <dragengine/common/exceptions.h>



// Class dedaiSpaceGridVertex
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiSpaceGridVertex::dedaiSpaceGridVertex() :
pGrid( NULL ),
pFirstEdge( 0 ),
pIndex( 0 ),
pEdgeCount( 0 ),
pFirstLink( 0 ),
pLinkCount( 0 ),

pEnabled( true ),

pPathParent( NULL ),
pPathCostF( 0.0f ),
pPathCostG( 0.0f ),
pPathCostH( 0.0f ),
pPathType( epftFree ){
}

dedaiSpaceGridVertex::~dedaiSpaceGridVertex(){
}



// Management
///////////////

void dedaiSpaceGridVertex::SetGrid( dedaiSpaceGrid *grid ){
	pGrid = grid;
}

void dedaiSpaceGridVertex::SetIndex( unsigned short index ){
	pIndex = index;
}



void dedaiSpaceGridVertex::SetPosition( const decVector &position ){
	pPosition = position;
}

void dedaiSpaceGridVertex::SetFirstEdge( unsigned short first ){
	pFirstEdge = first;
}

void dedaiSpaceGridVertex::SetEdgeCount( unsigned short count ){
	pEdgeCount = count;
}

void dedaiSpaceGridVertex::SetFirstLink( unsigned short first ){
	pFirstLink = first;
}

void dedaiSpaceGridVertex::SetLinkCount( unsigned short count ){
	pLinkCount = count;
}



void dedaiSpaceGridVertex::SetEnabled( bool enabled ){
	pEnabled = enabled;
}



void dedaiSpaceGridVertex::SetPathParent( dedaiSpaceGridVertex *face ){
	pPathParent = face;
}

void dedaiSpaceGridVertex::SetPathCostF( float cost ){
	pPathCostF = cost;
}

void dedaiSpaceGridVertex::SetPathCostG( float cost ){
	pPathCostG = cost;
}

void dedaiSpaceGridVertex::SetPathCostH( float cost ){
	pPathCostH = cost;
}

void dedaiSpaceGridVertex::SetPathType( ePathFindingTypes type ){
	pPathType = type;
}

void dedaiSpaceGridVertex::ClearPath(){
	pPathParent = NULL;
	pPathCostF = 0.0f;
	pPathCostG = 0.0f;
	pPathCostH = 0.0f;
	pPathType = epftFree;
}
