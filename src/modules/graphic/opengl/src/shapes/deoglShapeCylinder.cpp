/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <stdlib.h>

#include "deoglShapeCylinder.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define SEGMENT_COUNT	20



// Class deoglShapeCylinder
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShapeCylinder::deoglShapeCylinder( deoglRenderThread &renderThread ) : deoglShape( renderThread ){
	SetPointOffsetLines( 0 );
	SetPointCountLines( SEGMENT_COUNT * 3 * 2 );
	
	SetPointOffsetFaces( GetPointCountLines() );
	SetPointCountFaces( SEGMENT_COUNT * 4 * 3 );
}

deoglShapeCylinder::~deoglShapeCylinder(){
}



// Management
///////////////

void deoglShapeCylinder::CalcMatrices( decMatrix &matrix1, decMatrix &matrix2, const decVector &position,
const decQuaternion &orientation, float halfHeight, float topRadius, float bottomRadius ){
	const decMatrix baseMatrix = decMatrix::CreateFromQuaternion( orientation ) * decMatrix::CreateTranslation( position );
	
	matrix1 = decMatrix::CreateScale( topRadius, topRadius, topRadius )
		* decMatrix::CreateTranslation( 0.0f, halfHeight, 0.0f )
		* baseMatrix;
	
	matrix2 = decMatrix::CreateScale( bottomRadius, bottomRadius, bottomRadius )
		* decMatrix::CreateTranslation( 0.0f, -halfHeight, 0.0f )
		* baseMatrix;
}

void deoglShapeCylinder::AddVBOLines( sVBOData *data ){
	const float stepAngle = PI * 2.0f / ( float )( SEGMENT_COUNT );
	const int segmentPointCount = SEGMENT_COUNT * 2;
	int i, base;
	float angle;
	
	base = 0;
	
	// top ring
	for( i=0; i<SEGMENT_COUNT; i++ ){
		angle = stepAngle * ( float )i;
		data[ base++ ].SetSelFalse( sinf( angle ), 0.0f, cosf( angle ) );
		
		angle = stepAngle * ( float )( i + 1 );
		data[ base++ ].SetSelFalse( sinf( angle ), 0.0f, cosf( angle ) );
	}
	
	// bottom ring
	for( i=0; i<segmentPointCount; i++ ){
		data[ base++ ].SetFromSelTrue( data[ i ] );
	}
	
	// ring connections
	for( i=0; i<SEGMENT_COUNT; i++ ){
		data[ base++ ].SetFromSelFalse( data[ i * 2 ] );
		data[ base++ ].SetFromSelTrue( data[ segmentPointCount + i * 2 ] );
	}
}

void deoglShapeCylinder::AddVBOFaces( sVBOData *data ){
	const float stepAngle = PI * 2.0f / ( float )( SEGMENT_COUNT );
	int i, base;
	
	base = 0;
	
	// top cap
	for( i=0; i<SEGMENT_COUNT; i++ ){
		const float angle1 = stepAngle * ( float )( i + 1 );
		const float angle2 = stepAngle * ( float )i;
		
		data[ base++ ].SetSelFalse( sinf( angle1 ), 0.0f, cosf( angle1 ) );
		data[ base++ ].SetSelFalse( sinf( angle2 ), 0.0f, cosf( angle2 ) );
		data[ base++ ].SetSelFalse( 0.0f, 0.0f, 0.0f );
	}
	
	// bottom cap
	for( i=0; i<SEGMENT_COUNT; i++ ){
		const float angle1 = stepAngle * ( float )i;
		const float angle2 = stepAngle * ( float )( i + 1 );
		
		data[ base++ ].SetSelTrue( sinf( angle1 ), 0.0f, cosf( angle1 ) );
		data[ base++ ].SetSelTrue( sinf( angle2 ), 0.0f, cosf( angle2 ) );
		data[ base++ ].SetSelTrue( 0.0f, 0.0f, 0.0f );
	}
	
	// round-about surface
	for( i=0; i<SEGMENT_COUNT; i++ ){
		const float angle1 = stepAngle * ( float )i;
		const float x1 = sinf( angle1 );
		const float z1 = cosf( angle1 );
		
		const float angle2 = stepAngle * ( float )( i + 1 );
		const float x2 = sinf( angle2 );
		const float z2 = cosf( angle2 );
		
		data[ base++ ].SetSelTrue( x1, 0.0f, z1 );
		data[ base++ ].SetSelFalse( x1, 0.0f, z1 );
		data[ base++ ].SetSelFalse( x2, 0.0f, z2 );
		
		data[ base++ ].SetSelTrue( x1, 0.0f, z1 );
		data[ base++ ].SetSelFalse( x2, 0.0f, z2 );
		data[ base++ ].SetSelTrue( x2, 0.0f, z2 );
	}
}
