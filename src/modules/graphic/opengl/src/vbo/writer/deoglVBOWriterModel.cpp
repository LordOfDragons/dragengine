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

#include "deoglVBOWriterModel.h"
#include "../deoglSharedVBOBlock.h"

#include <dragengine/common/exceptions.h>



// Class deoglVBOWriterModel
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglVBOWriterModel::deoglVBOWriterModel( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pDataPoints( nullptr ),
pDataIndices( nullptr ){
}

deoglVBOWriterModel::~deoglVBOWriterModel(){
}



// Management
///////////////

void deoglVBOWriterModel::Reset( deoglSharedVBOBlock *vboBlock ){
	if( vboBlock ){
		pDataPoints = ( GLfloat* )vboBlock->GetData();
		pDataIndices = ( GLuint* )vboBlock->GetIndexData();
		
	}else{
		pDataPoints = nullptr;
		pDataIndices = nullptr;
	}
}



void deoglVBOWriterModel::WritePoint( const decVector &position, const decVector &normal,
const decVector &tangent, bool negateTangent, const decVector2 &texCoord,
const decVector &realNormal ){
	DEASSERT_NOTNULL( pDataPoints )
	
	GLfloat * const ptrPosition = pDataPoints;
	ptrPosition[ 0 ] = ( GLfloat )position.x;
	ptrPosition[ 1 ] = ( GLfloat )position.y;
	ptrPosition[ 2 ] = ( GLfloat )position.z;
	
	GLfloat * const ptrRealNormal = pDataPoints + 4;
	ptrRealNormal[ 0 ] = ( GLfloat )realNormal.x;
	ptrRealNormal[ 1 ] = ( GLfloat )realNormal.y;
	ptrRealNormal[ 2 ] = ( GLfloat )realNormal.z;
	
	GLfloat * const ptrNormal = pDataPoints + 8;
	ptrNormal[ 0 ] = ( GLfloat )normal.x;
	ptrNormal[ 1 ] = ( GLfloat )normal.y;
	ptrNormal[ 2 ] = ( GLfloat )normal.z;
	
	GLfloat * const ptrTangent = pDataPoints + 12;
	ptrTangent[ 0 ] = ( GLfloat )tangent.x;
	ptrTangent[ 1 ] = ( GLfloat )tangent.y;
	ptrTangent[ 2 ] = ( GLfloat )tangent.z;
	ptrTangent[ 3 ] = ( GLfloat )( negateTangent ? -1.0f : 1.0f );
	
	GLfloat * const ptrTexCoord = pDataPoints + 16;
	ptrTexCoord[ 0 ] = ( GLfloat )texCoord.x;
	ptrTexCoord[ 1 ] = ( GLfloat )texCoord.y;
	
	pDataPoints += 20;
}

void deoglVBOWriterModel::WritePoint( const decVector &position, const decVector &normal,
const decVector &tangent, bool negateTangent, const decVector2 &texCoord,
const decVector &realNormal, int weights ){
	WritePoint( position, normal, tangent, negateTangent, texCoord, realNormal );
	
	*( ( GLint* )( pDataPoints - 2 ) ) = ( GLint )weights;
}

void deoglVBOWriterModel::WriteTexCoordSetPoint( const decVector &tangent,
bool negateTangent, const decVector2 &texCoord ){
	DEASSERT_NOTNULL( pDataPoints )
	
	GLfloat * const ptrTangent = pDataPoints;
	ptrTangent[ 0 ] = ( GLfloat )tangent.x;
	ptrTangent[ 1 ] = ( GLfloat )tangent.y;
	ptrTangent[ 2 ] = ( GLfloat )tangent.z;
	ptrTangent[ 3 ] = ( GLfloat )( negateTangent ? -1.0f : 1.0f );
	
	GLfloat * const ptrTexCoord = pDataPoints + 2;
	ptrTexCoord[ 0 ] = ( GLfloat )texCoord.x;
	ptrTexCoord[ 1 ] = ( GLfloat )texCoord.y;
	
	pDataPoints += 4;
}

void deoglVBOWriterModel::WriteIndices( int index1, int index2, int index3 ){
	DEASSERT_NOTNULL( pDataIndices )
	
	pDataIndices[ 0 ] = ( GLuint )index1;
	pDataIndices[ 1 ] = ( GLuint )index2;
	pDataIndices[ 2 ] = ( GLuint )index3;
	
	pDataIndices += 3;
}
