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

#include "deoglModelFace.h"

#include <dragengine/common/exceptions.h>


// Class deoglModelFace
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglModelFace::deoglModelFace(){
}

deoglModelFace::~deoglModelFace(){
}



// Management
///////////////

void deoglModelFace::SetVertex1( int vertex ){
	pVertices[ 0 ] = vertex;
}

void deoglModelFace::SetVertex2( int vertex ){
	pVertices[ 1 ] = vertex;
}

void deoglModelFace::SetVertex3( int vertex ){
	pVertices[ 2 ] = vertex;
}

void deoglModelFace::SetVertexAt( int index, int vertex ){
	if( index < 0 || index > 2 ) DETHROW( deeInvalidParam );
	
	pVertices[ index ] = vertex;
}

void deoglModelFace::SetFaceNormal( const decVector &normal ){
	pFaceNormal = normal;
}

void deoglModelFace::SetExtends( const decVector &minExtend, const decVector &maxExtend ){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void deoglModelFace::SetCenter( const decVector &center ){
	pCenter = center;
}

void deoglModelFace::SetTexture( int texture ){
	pTexture = texture;
}

void deoglModelFace::SetIndex( int index ){
	pIndex = index;
}
