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

#include "dedaiSpaceMesh.h"
#include "dedaiSpaceMeshCorner.h"
#include "dedaiSpaceMeshEdge.h"
#include "dedaiSpaceMeshFace.h"

#include <dragengine/common/exceptions.h>



// Class dedaiSpaceMeshFace
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiSpaceMeshFace::dedaiSpaceMeshFace() :
pMesh( NULL ),
pFirstCorner( 0 ),
pCornerCount( 0 ),
pIndex( 0 ),
pTypeNumber( 0 ),
pDistance( 0.0f ),
pEnabled( true ),

pPathParent( NULL ),
pPathCostF( 0.0f ),
pPathCostG( 0.0f ),
pPathCostH( 0.0f ),
pPathType( epftFree ){
}

dedaiSpaceMeshFace::~dedaiSpaceMeshFace(){
}



// Management
///////////////

void dedaiSpaceMeshFace::SetMesh( dedaiSpaceMesh *mesh ){
	pMesh = mesh;
}

void dedaiSpaceMeshFace::SetIndex( unsigned short index ){
	pIndex = index;
}

void dedaiSpaceMeshFace::SetTypeNumber( unsigned short typeNumber ){
	pTypeNumber = typeNumber;
}



void dedaiSpaceMeshFace::SetFirstCorner( int first ){
	pFirstCorner = first;
}

void dedaiSpaceMeshFace::SetCornerCount( unsigned short count ){
	pCornerCount = count;
}

int dedaiSpaceMeshFace::IndexOfCornerWithEdge( unsigned short edge ) const{
	if( ! pMesh ){
		DETHROW( deeInvalidParam );
	}
	
	const dedaiSpaceMeshCorner * const corners = pMesh->GetCorners();
	int i;
	
	for( i=0; i<pCornerCount; i++ ){
		if( corners[ pFirstCorner + i ].GetEdge() == edge ){
			return i;
		}
	}
	
	return -1;
}

int dedaiSpaceMeshFace::IndexOfCornerWithVertex( unsigned short vertex ) const{
	if( ! pMesh ){
		DETHROW( deeInvalidParam );
	}
	
	const dedaiSpaceMeshCorner * const corners = pMesh->GetCorners();
	int i;
	
	for( i=0; i<pCornerCount; i++ ){
		if( corners[ pFirstCorner + i ].GetVertex() == vertex ){
			return i;
		}
	}
	
	return -1;
}



void dedaiSpaceMeshFace::SetCenter( const decVector &center ){
	pCenter = center;
}

void dedaiSpaceMeshFace::SetNormal( const decVector &normal ){
	pNormal = normal;
}

void dedaiSpaceMeshFace::SetDistance( float distance ){
	pDistance = distance;
}

void dedaiSpaceMeshFace::SetExtends( const decVector &minExtend, const decVector &maxExtend ){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void dedaiSpaceMeshFace::SetEntryPoint( const decVector &entryPoint ){
	pEntryPoint = entryPoint;
}



void dedaiSpaceMeshFace::SetEnabled( bool enabled ){
	pEnabled = enabled;
}



void dedaiSpaceMeshFace::SetPathParent( dedaiSpaceMeshFace *face ){
	pPathParent = face;
}

void dedaiSpaceMeshFace::SetPathCostF( float cost ){
	pPathCostF = cost;
}

void dedaiSpaceMeshFace::SetPathCostG( float cost ){
	pPathCostG = cost;
}

void dedaiSpaceMeshFace::SetPathCostH( float cost ){
	pPathCostH = cost;
}

void dedaiSpaceMeshFace::SetPathType( ePathFindingTypes type ){
	pPathType = type;
}

void dedaiSpaceMeshFace::ClearPath(){
	pPathParent = NULL;
	pPathCostF = 0.0f;
	pPathCostG = 0.0f;
	pPathCostH = 0.0f;
	pPathType = epftFree;
}
