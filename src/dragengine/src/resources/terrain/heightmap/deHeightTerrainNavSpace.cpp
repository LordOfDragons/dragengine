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
#include <string.h>

#include "deHeightTerrainNavSpace.h"
#include "deHeightTerrainNavSpaceEdge.h"
#include "../../navigation/space/deNavigationSpaceFace.h"
#include "../../../common/exceptions.h"



// Class deHeightTerrainNavSpace
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deHeightTerrainNavSpace::deHeightTerrainNavSpace() :
pType( deNavigationSpace::estMesh ),
pLayer( 0 ),
pSnapDistance( 0.001f ),
pSnapAngle( DEG2RAD * 180.0f ),
pCorners( NULL ),
pCornerCount( 0 ),
pEdges( NULL ),
pEdgeCount( 0 ),
pFaces( NULL ),
pFaceCount( 0 ){
}

deHeightTerrainNavSpace::~deHeightTerrainNavSpace(){
	if( pFaces ){
		delete [] pFaces;
	}
	if( pEdges ){
		delete [] pEdges;
	}
	if( pCorners ){
		delete [] pCorners;
	}
}



// Management
///////////////

void deHeightTerrainNavSpace::SetType( deNavigationSpace::eSpaceTypes type ){
	if( type < deNavigationSpace::estGrid || type > deNavigationSpace::estVolume ){
		DETHROW( deeInvalidParam );
	}
	pType = type;
}

void deHeightTerrainNavSpace::SetLayer( int layer ){
	pLayer = layer;
}



void deHeightTerrainNavSpace::SetSnapDistance( float distance ){
	pSnapDistance = decMath::max( distance, 0.0f );
}

void deHeightTerrainNavSpace::SetSnapAngle( float angle ){
	pSnapAngle = decMath::max( angle, 0.0f );
}



// Corners
////////////

void deHeightTerrainNavSpace::SetCornerCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pCornerCount ){
		return;
	}
	
	if( pCorners ){
		delete [] pCorners;
	}
	pCorners = NULL;
	pCornerCount = 0;
	
	if( count > 0 ){
		pCorners = new unsigned int[ count ];
		pCornerCount = count;
	}
}

unsigned int deHeightTerrainNavSpace::GetCornerAt( int index ) const{
	if( index < 0 || index >= pCornerCount ){
		DETHROW( deeInvalidParam );
	}
	return pCorners[ index ];
}

void deHeightTerrainNavSpace::SetCornerAt( int index, unsigned int corner ){
	if( index < 0 || index >= pCornerCount ){
		DETHROW( deeInvalidParam );
	}
	pCorners[ index ] = corner;
}



// Edges
//////////

void deHeightTerrainNavSpace::SetEdgeCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pEdgeCount ){
		return;
	}
	
	if( pEdges ){
		delete [] pEdges;
	}
	pEdges = NULL;
	pEdgeCount = 0;
	
	if( count > 0 ){
		pEdges = new deHeightTerrainNavSpaceEdge[ count ];
		pEdgeCount = count;
	}
}

deHeightTerrainNavSpaceEdge &deHeightTerrainNavSpace::GetEdgeAt( int index ) const{
	if( index < 0 || index >= pEdgeCount ){
		DETHROW( deeInvalidParam );
	}
	return pEdges[ index ];
}



// Faces
//////////

void deHeightTerrainNavSpace::SetFaceCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pFaceCount ){
		return;
	}
	
	if( pFaces ){
		delete [] pFaces;
	}
	pFaces = NULL;
	pFaceCount = 0;
	
	if( count > 0 ){
		pFaces = new deNavigationSpaceFace[ count ];
		pFaceCount = count;
	}
}

deNavigationSpaceFace &deHeightTerrainNavSpace::GetFaceAt( int index ) const{
	if( index < 0 || index >= pFaceCount ){
		DETHROW( deeInvalidParam );
	}
	return pFaces[ index ];
}
