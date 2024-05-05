/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
