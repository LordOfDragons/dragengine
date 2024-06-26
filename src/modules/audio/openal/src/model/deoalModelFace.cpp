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
#include <stdlib.h>
#include <string.h>

#include "deoalModelFace.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelVertex.h>



// Safe distance margin
#define SAFE_DISTANCE_MARGIN 1e-5f



// Class deoalModelFace
/////////////////////////

// Constructor, destructor
////////////////////////////

deoalModelFace::deoalModelFace() :
pIndex( 0 ),
pTexture( 0 )
{
	pWeightSets[ 0 ] = -1;
	pWeightSets[ 1 ] = -1;
	pWeightSets[ 2 ] = -1;
	pEdgeDistance[ 0 ] = 0.0f;
	pEdgeDistance[ 1 ] = 0.0f;
	pEdgeDistance[ 2 ] = 0.0f;
	pEdgeDistanceSafe[ 0 ] = 0.0f;
	pEdgeDistanceSafe[ 1 ] = 0.0f;
	pEdgeDistanceSafe[ 2 ] = 0.0f;
}



// Management
///////////////

void deoalModelFace::SetVertex1( const decVector &vector ){
	pVertices[ 0 ] = vector;
}

void deoalModelFace::SetVertex2( const decVector &vector ){
	pVertices[ 1 ] = vector;
}

void deoalModelFace::SetVertex3( const decVector &vector ){
	pVertices[ 2 ] = vector;
}

const decVector &deoalModelFace::GetVertexAt( int index ) const{
	if( index < 0 || index > 2 ){
		DETHROW( deeInvalidParam );
	}
	return pVertices[ index ];
}



int deoalModelFace::GetWeightSetAt( int index ) const{
	if( index < 0 || index > 2 ){
		DETHROW( deeInvalidParam );
	}
	return pWeightSets[ index ];
}



const decVector &deoalModelFace::GetEdgeNormalAt( int index ) const{
	if( index < 0 || index > 2 ){
		DETHROW( deeInvalidParam );
	}
	return pEdgeNormal[ index ];
}

float deoalModelFace::GetEdgeDistanceAt( int index ) const{
	if( index < 0 || index > 2 ){
		DETHROW( deeInvalidParam );
	}
	return pEdgeDistance[ index ];
}

float deoalModelFace::GetEdgeDistanceSafeAt( int index ) const{
	if( index < 0 || index > 2 ){
		DETHROW( deeInvalidParam );
	}
	return pEdgeDistanceSafe[ index ];
}



void deoalModelFace::Init( const deModelLOD &lod, int index, const deModelFace &face ){
	pIndex = index;
	pTexture = face.GetTexture();
	
	const deModelVertex &vertex1 = lod.GetVertexAt( face.GetVertex1() );
	const deModelVertex &vertex2 = lod.GetVertexAt( face.GetVertex2() );
	const deModelVertex &vertex3 = lod.GetVertexAt( face.GetVertex3() );
	
	pVertices[ 0 ] = vertex1.GetPosition();
	pVertices[ 1 ] = vertex2.GetPosition();
	pVertices[ 2 ] = vertex3.GetPosition();
	
	pWeightSets[ 0 ] = vertex1.GetWeightSet();
	pWeightSets[ 1 ] = vertex2.GetWeightSet();
	pWeightSets[ 2 ] = vertex3.GetWeightSet();
	
	UpdateNormalAndEdges();
}

void deoalModelFace::InitFromStatic( const deoalModelFace &face ){
	// NOTE vertices are copied since not all have weight-sets and thus not all are calculated
	pIndex = face.pIndex;
	pTexture = face.pTexture;
	pVertices[ 0 ] = face.pVertices[ 0 ];
	pVertices[ 1 ] = face.pVertices[ 1 ];
	pVertices[ 2 ] = face.pVertices[ 2 ];
	pWeightSets[ 0 ] = face.pWeightSets[ 0 ];
	pWeightSets[ 1 ] = face.pWeightSets[ 1 ];
	pWeightSets[ 2 ] = face.pWeightSets[ 2 ];
}

void deoalModelFace::UpdateNormalAndEdges(){
	pMinExtend = pVertices[ 0 ].Smallest( pVertices[ 1 ] ).Smallest( pVertices[ 2 ] );
	pMaxExtend = pVertices[ 0 ].Largest( pVertices[ 1 ] ).Largest( pVertices[ 2 ] );
	
	const decVector edge0( pVertices[ 1 ] - pVertices[ 0 ] );
	const decVector edge1( pVertices[ 2 ] - pVertices[ 1 ] );
	const decVector edge2( pVertices[ 0 ] - pVertices[ 2 ] );
	
	const decVector normal( edge0 % edge1 );
	if( normal.IsZero() ){
		pNormal.Set( 0.0f, 0.0f, 1.0f );
	}else{
		pNormal = normal.Normalized();
	}
	
	const decVector edgeNormal0( pNormal % edge0 );
	if( edgeNormal0.IsZero() ){
		pEdgeNormal[ 0 ].Set( 0.0f, 0.0f, 1.0f );
	}else{
		pEdgeNormal[ 0 ] = edgeNormal0.Normalized();
	}
	pEdgeDistance[ 0 ] = pVertices[ 0 ] * pEdgeNormal[ 0 ];
	pEdgeDistanceSafe[ 0 ] = pEdgeDistance[ 0 ] - SAFE_DISTANCE_MARGIN;
	
	const decVector edgeNormal1( pNormal % edge1 );
	if( edgeNormal1.IsZero() ){
		pEdgeNormal[ 1 ].Set( 0.0f, 0.0f, 1.0f );
	}else{
		pEdgeNormal[ 1 ] = edgeNormal1.Normalized();
	}
	pEdgeDistance[ 1 ] = pVertices[ 1 ] * pEdgeNormal[ 1 ];
	pEdgeDistanceSafe[ 1 ] = pEdgeDistance[ 1 ] - SAFE_DISTANCE_MARGIN;
	
	const decVector edgeNormal2( pNormal % edge2 );
	if( edgeNormal2.IsZero() ){
		pEdgeNormal[ 2 ].Set( 0.0f, 0.0f, 1.0f );
	}else{
		pEdgeNormal[ 2 ] = edgeNormal2.Normalized();
	}
	pEdgeDistance[ 2 ] = pVertices[ 2 ] * pEdgeNormal[ 2 ];
	pEdgeDistanceSafe[ 2 ] = pEdgeDistance[ 2 ] - SAFE_DISTANCE_MARGIN;
}
