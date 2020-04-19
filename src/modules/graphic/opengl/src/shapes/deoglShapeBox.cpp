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

#include "deoglShapeBox.h"

#include <dragengine/common/exceptions.h>



// Class deoglShapeBox
////////////////////////

// Constructor, destructor
////////////////////////////

deoglShapeBox::deoglShapeBox( deoglRenderThread &renderThread ) : deoglShape( renderThread ){
	SetPointOffsetLines( 0 );
	SetPointCountLines( 12 * 2 );
	
	SetPointOffsetFaces( GetPointCountLines() );
	SetPointCountFaces( 6 * 3 * 2 );
}

deoglShapeBox::~deoglShapeBox(){
}



// Management
///////////////

void deoglShapeBox::CalcMatrix( decMatrix &matrix, const decVector &position, const decQuaternion &orientation, const decVector &halfExtends ){
	matrix = decMatrix::CreateScale( halfExtends )
		* decMatrix::CreateFromQuaternion( orientation )
		* decMatrix::CreateTranslation( position );
}

void deoglShapeBox::AddVBOLines( sVBOData *data ){
	const int indices[ 24 ] = {
		0,1, 1,2, 2,3, 3,0,  // top line loop
		4,5, 5,6, 6,7, 7,4,  // bottom line loop
		0,4, 1,5, 2,6, 3,7   // loop connections
	};
	sVBOData points[ 8 ];
	int i;
	
	points[ 0 ].SetSelFalse( -1.0f,  1.0f, -1.0f );
	points[ 1 ].SetSelFalse(  1.0f,  1.0f, -1.0f );
	points[ 2 ].SetSelFalse(  1.0f,  1.0f,  1.0f );
	points[ 3 ].SetSelFalse( -1.0f,  1.0f,  1.0f );
	points[ 4 ].SetSelFalse( -1.0f, -1.0f, -1.0f );
	points[ 5 ].SetSelFalse(  1.0f, -1.0f, -1.0f );
	points[ 6 ].SetSelFalse(  1.0f, -1.0f,  1.0f );
	points[ 7 ].SetSelFalse( -1.0f, -1.0f,  1.0f );
	
	for( i=0; i<24; i++ ){
		data[ i ].SetFrom( points[ indices[ i ] ] );
	}
}

void deoglShapeBox::AddVBOFaces( sVBOData *data ){
	const int indices[ 36 ] = {
		0, 1, 2, 0, 2, 3,  // top (y+)
		7, 6, 5, 7, 5, 4,  // bottom (y-)
		6, 7, 3, 6, 3, 2,  // front (z+)
		4, 5, 1, 4, 1, 0,  // back (z-)
		5, 6, 2, 5, 2, 1,  // left (x+)
		7, 4, 0, 7, 0, 3   // right (x-)
	};
	sVBOData points[ 8 ];
	int i;
	
	points[ 0 ].SetSelFalse( -1.0f,  1.0f, -1.0f );
	points[ 1 ].SetSelFalse(  1.0f,  1.0f, -1.0f );
	points[ 2 ].SetSelFalse(  1.0f,  1.0f,  1.0f );
	points[ 3 ].SetSelFalse( -1.0f,  1.0f,  1.0f );
	points[ 4 ].SetSelFalse( -1.0f, -1.0f, -1.0f );
	points[ 5 ].SetSelFalse(  1.0f, -1.0f, -1.0f );
	points[ 6 ].SetSelFalse(  1.0f, -1.0f,  1.0f );
	points[ 7 ].SetSelFalse( -1.0f, -1.0f,  1.0f );
	
	for( i=0; i<36; i++ ){
		data[ i ].SetFrom( points[ indices[ i ] ] );
	}
}
