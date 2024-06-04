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
