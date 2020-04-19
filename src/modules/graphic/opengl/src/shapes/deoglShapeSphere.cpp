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

#include "deoglShapeSphere.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define RING_COUNT		9
#define SEGMENT_COUNT	20



// Class deoglShapeSphere
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglShapeSphere::deoglShapeSphere( deoglRenderThread &renderThread ) : deoglShape( renderThread ){
	SetPointOffsetLines( 0 );
	SetPointCountLines( ( RING_COUNT * SEGMENT_COUNT + SEGMENT_COUNT * ( RING_COUNT + 1 ) ) * 2 );
	
	SetPointOffsetFaces( GetPointCountLines() );
	SetPointCountFaces( RING_COUNT * SEGMENT_COUNT * 3 * 2 );
}

deoglShapeSphere::~deoglShapeSphere(){
}



// Management
///////////////

void deoglShapeSphere::CalcMatrix( decMatrix &matrix, const decVector &position, float radius ){
	matrix = decMatrix::CreateScale( radius, radius, radius ) * decMatrix::CreateTranslation( position );
}

void deoglShapeSphere::AddVBOLines( sVBOData *data ){
	const float stepAngleSegment = PI * 2.0f / ( float )( SEGMENT_COUNT );
	const float stepAngleRing = PI / ( float )( RING_COUNT + 1 );
	float radius, angle, height, x, z;
	int i, j, base;
	
	base = 0;
	for( i=0; i<RING_COUNT; i++ ){
		angle = stepAngleRing * ( float )( i + 1 ); // first ring is actually at i=1
		radius = sinf( angle );
		height = cosf( angle );
		
		for( j=0; j<SEGMENT_COUNT; j++ ){
			angle = stepAngleSegment * ( float )j;
			data[ base++ ].SetSelFalse( sinf( angle ) * radius, height, cosf( angle ) * radius );
			
			angle = stepAngleSegment * ( float )( j + 1 );
			data[ base++ ].SetSelFalse( sinf( angle ) * radius, height, cosf( angle ) * radius );
		}
	}
	
	for( i=0; i<SEGMENT_COUNT; i++ ){
		angle = stepAngleSegment * ( float )i;
		x = sinf( angle );
		z = cosf( angle );
		
		for( j=0; j<RING_COUNT+1; j++ ){
			angle = stepAngleRing * ( float )j;
			radius = sinf( angle );
			data[ base++ ].SetSelFalse( x * radius, cosf( angle ), z * radius );
			
			angle = stepAngleRing * ( float )( j + 1 );
			radius = sinf( angle );
			data[ base++ ].SetSelFalse( x * radius, cosf( angle ), z * radius );
		}
	}
}

void deoglShapeSphere::AddVBOFaces( sVBOData *data ){
	const float stepAngleSegment = PI * 2.0f / ( float )( SEGMENT_COUNT );
	const float stepAngleRing = PI / ( float )( RING_COUNT + 1 );
	float radius1, radius2, angle1, height1, height2, angle2;
	int i, j, base;
	
	base = 0;
	for( i=0; i<RING_COUNT+1; i++ ){
		angle1 = stepAngleRing * ( float )i;
		radius1 = sinf( angle1 );
		height1 = cosf( angle1 );
		
		angle2 = stepAngleRing * ( float )( i + 1 );
		radius2 = sinf( angle2 );
		height2 = cosf( angle2 );
		
		if( i == 0 ){
			for( j=0; j<SEGMENT_COUNT; j++ ){
				angle1 = stepAngleSegment * ( float )j;
				angle2 = stepAngleSegment * ( float )( j + 1 );
				
				data[ base++ ].SetSelFalse( 0.0f, 1.0f, 0.0f );
				data[ base++ ].SetSelFalse( sinf( angle2 ) * radius2, height2, cosf( angle2 ) * radius2 );
				data[ base++ ].SetSelFalse( sinf( angle1 ) * radius2, height2, cosf( angle1 ) * radius2 );
			}
			
		}else if( i == RING_COUNT ){
			for( j=0; j<SEGMENT_COUNT; j++ ){
				angle1 = stepAngleSegment * ( float )j;
				angle2 = stepAngleSegment * ( float )( j + 1 );
				
				data[ base++ ].SetSelFalse( 0.0f, -1.0f, 0.0f );
				data[ base++ ].SetSelFalse( sinf( angle1 ) * radius1, height1, cosf( angle1 ) * radius1 );
				data[ base++ ].SetSelFalse( sinf( angle2 ) * radius1, height1, cosf( angle2 ) * radius1 );
			}
			
		}else{
			for( j=0; j<SEGMENT_COUNT; j++ ){
				angle1 = stepAngleSegment * ( float )j;
				angle2 = stepAngleSegment * ( float )( j + 1 );
				
				data[ base++ ].SetSelFalse( sinf( angle1 ) * radius2, height2, cosf( angle1 ) * radius2 );
				data[ base++ ].SetSelFalse( sinf( angle1 ) * radius1, height1, cosf( angle1 ) * radius1 );
				data[ base++ ].SetSelFalse( sinf( angle2 ) * radius1, height1, cosf( angle2 ) * radius1 );
				
				data[ base++ ].SetSelFalse( sinf( angle1 ) * radius2, height2, cosf( angle1 ) * radius2 );
				data[ base++ ].SetSelFalse( sinf( angle2 ) * radius1, height1, cosf( angle2 ) * radius1 );
				data[ base++ ].SetSelFalse( sinf( angle2 ) * radius2, height2, cosf( angle2 ) * radius2 );
			}
		}
	}
}
