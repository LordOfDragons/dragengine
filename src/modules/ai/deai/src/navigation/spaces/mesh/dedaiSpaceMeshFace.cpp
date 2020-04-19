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
