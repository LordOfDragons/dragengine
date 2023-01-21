/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
