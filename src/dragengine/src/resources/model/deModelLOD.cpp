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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deModelLOD.h"
#include "deModelWeight.h"
#include "deModelVertex.h"
#include "deModelFace.h"
#include "deModelTextureCoordinatesSet.h"
#include "../../common/exceptions.h"



// Class deModelLOD
/////////////////////

// Constructor, destructor
////////////////////////////

deModelLOD::deModelLOD() :
pHasLodError( false ),
pLodError( 0.01f )
{
	pWeights = NULL;
	pWeightCount = 0;
	pWeightGroups = NULL;
	pWeightGroupCount = 0;
	
	pVertices = NULL;
	pVertexCount = 0;
	
	pNormalCount = 0;
	pTangentCount = 0;
	
	pFaces = NULL;
	pFaceCount = 0;
	
	pTextureCoordinatesSets = NULL;
	pTextureCoordinatesSetsCount = 0;
	pTextureCoordinatesCount = 0;
}

deModelLOD::~deModelLOD(){
	if( pTextureCoordinatesSets ){
		delete [] pTextureCoordinatesSets;
	}
	if( pFaces ){
		delete [] pFaces;
	}
	if( pVertices ){
		delete [] pVertices;
	}
	if( pWeightGroups ){
		delete [] pWeightGroups;
	}
	if( pWeights ){
		delete [] pWeights;
	}
}



// Management
///////////////////////////

void deModelLOD::SetHasLodError( bool hasError ){
	pHasLodError = hasError;
}

void deModelLOD::SetLodError( float error ){
	pLodError = decMath::max( error, 0.001f );
}



// Weight Sets Management
///////////////////////////

void deModelLOD::SetWeightCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pWeightCount ){
		if( pWeights ){
			delete [] pWeights;
		}
		pWeights = NULL;
		pWeightCount = 0;
		
		if( count > 0 ){
			pWeights = new deModelWeight[ count ];
			pWeightCount = count;
		}
	}
}

deModelWeight &deModelLOD::GetWeightAt( int index ) const{
	if( index < 0 || index >= pWeightCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pWeights[ index ];
}



void deModelLOD::SetWeightGroupCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pWeightGroupCount ){
		if( pWeightGroups ){
			delete [] pWeightGroups;
		}
		pWeightGroups = NULL;
		pWeightGroupCount = 0;
		
		if( count > 0 ){
			pWeightGroups = new int[ count ];
			pWeightGroupCount = count;
		}
	}
}

int deModelLOD::GetWeightGroupAt( int index ) const{
	if( index < 0 || index >= pWeightGroupCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pWeightGroups[ index ];
}

void deModelLOD::SetWeightGroupAt( int index, int weightSetCount ) const{
	if( index < 0 || index >= pWeightGroupCount || weightSetCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pWeightGroups[ index ] = weightSetCount;
}



// Vertex Management
//////////////////////

void deModelLOD::SetVertexCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pVertexCount ){
		if( pVertices ){
			delete [] pVertices;
		}
		pVertices = NULL;
		pVertexCount = 0;
		
		if( count > 0 ){
			pVertices = new deModelVertex[ count ];
			pVertexCount = count;
		}
	}
}

deModelVertex &deModelLOD::GetVertexAt( int index ) const{
	if( index < 0 || index >= pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pVertices[ index ];
}



// Face Management
////////////////////

void deModelLOD::SetNormalCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pNormalCount = count;
}

void deModelLOD::SetTangentCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTangentCount = count;
}

void deModelLOD::SetFaceCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pFaceCount ){
		if( pFaces ){
			delete [] pFaces;
		}
		pFaces = NULL;
		pFaceCount = 0;
		
		if( count > 0 ){
			pFaces = new deModelFace[ count ];
			pFaceCount = count;
		}
	}
}

deModelFace &deModelLOD::GetFaceAt( int index ) const{
	if( index < 0 || index >= pFaceCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pFaces[ index ];
}



// Texture Coordinates Sets
/////////////////////////////

void deModelLOD::SetTextureCoordinatesCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTextureCoordinatesCount = count;
}

void deModelLOD::SetTextureCoordinatesSetCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pTextureCoordinatesSetsCount ){
		if( pTextureCoordinatesSets ){
			delete [] pTextureCoordinatesSets;
		}
		pTextureCoordinatesSets = NULL;
		pTextureCoordinatesSetsCount = 0;
		
		if( count > 0 ){
			pTextureCoordinatesSets = new deModelTextureCoordinatesSet[ count ];
			pTextureCoordinatesSetsCount = count;
		}
	}
}

deModelTextureCoordinatesSet &deModelLOD::GetTextureCoordinatesSetAt( int index ) const{
	if( index < 0 || index >= pTextureCoordinatesSetsCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTextureCoordinatesSets[ index ];
}
