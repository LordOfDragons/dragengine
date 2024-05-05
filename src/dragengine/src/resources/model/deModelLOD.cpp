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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deModelLOD.h"
#include "deModelWeight.h"
#include "deModelVertex.h"
#include "deModelFace.h"
#include "deModelTextureCoordinatesSet.h"
#include "deModelLodVertexPositionSet.h"
#include "../../common/exceptions.h"



// Class deModelLOD
/////////////////////

// Constructor, destructor
////////////////////////////

deModelLOD::deModelLOD() :
pHasLodError( false ),
pLodError( 0.01f ),
pVertexPositionSets( nullptr ),
pVertexPositionSetCount( 0 )
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
	if( pVertexPositionSets ){
		delete [] pVertexPositionSets;
	}
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



// Vertex position sets
/////////////////////////

void deModelLOD::SetVertexPositionSetCount( int count ){
	DEASSERT_TRUE( count >= 0 )
	
	if( count == pVertexPositionSetCount ){
		return;
	}
	
	if( pVertexPositionSets ){
		delete [] pVertexPositionSets;
	}
	pVertexPositionSets = nullptr;
	pVertexPositionSetCount = 0;
	
	if( count > 0 ){
		pVertexPositionSets = new deModelLodVertexPositionSet[ count ];
		pVertexPositionSetCount = count;
	}
}

deModelLodVertexPositionSet &deModelLOD::GetVertexPositionSetAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pVertexPositionSetCount )
	
	return pVertexPositionSets[ index ];
}
