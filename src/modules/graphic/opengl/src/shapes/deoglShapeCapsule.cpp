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
#include <stdlib.h>

#include "deoglShapeCapsule.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define RING_COUNT		9
#define SEGMENT_COUNT	20



// Class deoglShapeCapsule
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShapeCapsule::deoglShapeCapsule( deoglRenderThread &renderThread ) : deoglShape( renderThread ){
	SetPointOffsetLines( 0 );
	SetPointCountLines( ( ( RING_COUNT + 1 ) * SEGMENT_COUNT + SEGMENT_COUNT * ( RING_COUNT + 2 ) ) * 2 );
	
	SetPointOffsetFaces( GetPointCountLines() );
	SetPointCountFaces( ( RING_COUNT + 2 ) * SEGMENT_COUNT * 3 * 2 );
}

deoglShapeCapsule::~deoglShapeCapsule(){
}



// Management
///////////////

void deoglShapeCapsule::CalcMatrices( decMatrix &matrix1, decMatrix &matrix2, const decVector &position,
const decQuaternion &orientation, float halfHeight, float topRadius, float bottomRadius ){
	// capsules have a specific property in that their cap half spheres have to be more or less than
	// a half sphere depending on the difference between the top and the bottom radius. to simulate
	// this the vertices on the respective half spheres have to be spread or squeezed to achieve
	// this effect. this can though not be achieved just like that since matrices apply the same
	// rotation to all transformed points. this effect would require an additional parameter of some
	// sort. so for the time being this effect is simply ignored
	const decMatrix baseMatrix = decMatrix::CreateFromQuaternion( orientation ) * decMatrix::CreateTranslation( position );
	
	matrix1 = decMatrix::CreateScale( topRadius, topRadius, topRadius )
		* decMatrix::CreateTranslation( 0.0f, halfHeight, 0.0f )
		* baseMatrix;
	
	matrix2 = decMatrix::CreateScale( bottomRadius, bottomRadius, bottomRadius )
		* decMatrix::CreateTranslation( 0.0f, -halfHeight, 0.0f )
		* baseMatrix;
}

void deoglShapeCapsule::AddVBOLines( sVBOData *data ){
	const float stepAngleSegment = PI * 2.0f / ( float )( SEGMENT_COUNT );
	const float stepAngleRing = PI / ( float )( RING_COUNT + 1 );
	const int splitRing = ( RING_COUNT + 1 ) / 2;
	float radius, angle, height, x, z;
	int i, j, base;
	bool selector;
	
	base = 0;
	for( i=0; i<RING_COUNT+2; i++ ){
		if( i < splitRing ){
			angle = stepAngleRing * ( float )( i + 1 ); // first ring is actually at i=1
			radius = sinf( angle );
			height = cosf( angle );
			selector = false;
			
		}else{
			angle = stepAngleRing * ( float )i;
			radius = sinf( angle );
			height = cosf( angle );
			selector = true;
		}
		
		for( j=0; j<SEGMENT_COUNT; j++ ){
			angle = stepAngleSegment * ( float )j;
			data[ base++ ].Set( sinf( angle ) * radius, height, cosf( angle ) * radius, selector );
			
			angle = stepAngleSegment * ( float )( j + 1 );
			data[ base++ ].Set( sinf( angle ) * radius, height, cosf( angle ) * radius, selector );
		}
	}
	
	for( i=0; i<SEGMENT_COUNT; i++ ){
		angle = stepAngleSegment * ( float )i;
		x = sinf( angle );
		z = cosf( angle );
		
		for( j=0; j<RING_COUNT+2; j++ ){
			if( j < splitRing + 1 ){
				angle = stepAngleRing * ( float )j;
				radius = sinf( angle );
				selector = false;
				
			}else{
				angle = stepAngleRing * ( float )( j - 1 );
				radius = sinf( angle );
				selector = true;
			}
			data[ base++ ].Set( x * radius, cosf( angle ), z * radius, selector );
			
			if( j < splitRing ){
				angle = stepAngleRing * ( float )( j + 1 );
				radius = sinf( angle );
				selector = false;
				
			}else{
				angle = stepAngleRing * ( float )j;
				radius = sinf( angle );
				selector = true;
			}
			data[ base++ ].Set( x * radius, cosf( angle ), z * radius, selector );
		}
	}
}

void deoglShapeCapsule::AddVBOFaces( sVBOData *data ){
	const float stepAngleSegment = PI * 2.0f / ( float )( SEGMENT_COUNT );
	const float stepAngleRing = PI / ( float )( RING_COUNT + 1 );
	const int splitRing = ( RING_COUNT + 1 ) / 2;
	float radius, radius2, angle, height, height2;
	bool selector1, selector2;
	int i, j, base;
	
	base = 0;
	for( i=0; i<RING_COUNT+2; i++ ){
		if( i < splitRing ){
			angle = stepAngleRing * ( float )i;
			radius = sinf( angle );
			height = cosf( angle );
			selector1 = false;
			
			angle = stepAngleRing * ( float )( i + 1 );
			radius2 = sinf( angle );
			height2 = cosf( angle );
			selector2 = false;
			
		}else if( i > splitRing ){
			angle = stepAngleRing * ( float )( i - 1 );
			radius = sinf( angle );
			height = cosf( angle );
			selector1 = true;
			
			angle = stepAngleRing * ( float )i;
			radius2 = sinf( angle );
			height2 = cosf( angle );
			selector2 = true;
			
		}else{ // i == splitRing
			angle = stepAngleRing * ( float )i;
			radius = sinf( angle );
			height = cosf( angle );
			selector1 = false;
			
			radius2 = radius;
			height2 = height;
			selector2 = true;
		}
		
		for( j=0; j<SEGMENT_COUNT; j++ ){
			const float angle1 = stepAngleSegment * ( float )j;
			const float angle2 = stepAngleSegment * ( float )( j + 1 );
			
			data[ base++ ].Set( sinf( angle1 ) * radius2, height2, cosf( angle1 ) * radius2, selector2 );
			data[ base++ ].Set( sinf( angle1 ) * radius, height, cosf( angle1 ) * radius, selector1 );
			data[ base++ ].Set( sinf( angle2 ) * radius, height, cosf( angle2 ) * radius, selector1 );
			
			data[ base++ ].Set( sinf( angle1 ) * radius2, height2, cosf( angle1 ) * radius2, selector2 );
			data[ base++ ].Set( sinf( angle2 ) * radius, height, cosf( angle2 ) * radius, selector1 );
			data[ base++ ].Set( sinf( angle2 ) * radius2, height2, cosf( angle2 ) * radius2, selector2 );
		}
	}
}
