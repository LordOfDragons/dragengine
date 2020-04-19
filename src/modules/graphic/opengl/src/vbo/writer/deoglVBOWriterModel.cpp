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
pDataPoints( NULL ),
pDataIndices( NULL ){
}

deoglVBOWriterModel::~deoglVBOWriterModel(){
}



// Management
///////////////

void deoglVBOWriterModel::Reset( deoglSharedVBOBlock *vboBlock ){
	if( vboBlock ){
		pDataPoints = ( char* )vboBlock->GetData();
		pDataIndices = ( GLuint* )vboBlock->GetIndexData();
		
	}else{
		pDataPoints = NULL;
		pDataIndices = NULL;
	}
}



void deoglVBOWriterModel::WritePoint( const decVector &position, const decVector &normal,
const decVector &tangent, bool negateTangent, const decVector2 &texCoord ){
	if( ! pDataPoints ){
		DETHROW( deeInvalidParam );
	}
	
	GLfloat * const ptrPosition = ( GLfloat* )pDataPoints;
	GLshort * const ptrNormal = ( GLshort* )( pDataPoints + 12 );
	GLshort * const ptrTangent = ( GLshort* )( pDataPoints + 20 );
	GLfloat * const ptrTexCoord = ( GLfloat* )( pDataPoints + 28 );
	
	ptrPosition[ 0 ] = ( GLfloat )position.x;
	ptrPosition[ 1 ] = ( GLfloat )position.y;
	ptrPosition[ 2 ] = ( GLfloat )position.z;
	
	ptrNormal[ 0 ] = ( GLshort )decMath::clamp( ( int )( normal.x * 32767.0f ), -32768, 32767 );
	ptrNormal[ 1 ] = ( GLshort )decMath::clamp( ( int )( normal.y * 32767.0f ), -32768, 32767 );
	ptrNormal[ 2 ] = ( GLshort )decMath::clamp( ( int )( normal.z * 32767.0f ), -32768, 32767 );
	
	ptrTangent[ 0 ] = ( GLshort )decMath::clamp( ( int )( tangent.x * 32767.0f ), -32768, 32767 );
	ptrTangent[ 1 ] = ( GLshort )decMath::clamp( ( int )( tangent.y * 32767.0f ), -32768, 32767 );
	ptrTangent[ 2 ] = ( GLshort )decMath::clamp( ( int )( tangent.z * 32767.0f ), -32768, 32767 );
	ptrTangent[ 3 ] = ( GLshort )( negateTangent ? -32768 : 32767 );
	
	ptrTexCoord[ 0 ] = ( GLfloat )texCoord.x;
	ptrTexCoord[ 1 ] = ( GLfloat )texCoord.y;
	
	pDataPoints += 36;
}

void deoglVBOWriterModel::WriteTexCoordSetPoint( const decVector &tangent,
bool negateTangent, const decVector2 &texCoord ){
	if( ! pDataPoints ){
		DETHROW( deeInvalidParam );
	}
	
	GLshort * const ptrTangent = ( GLshort* )pDataPoints;
	GLfloat * const ptrTexCoord = ( GLfloat* )( pDataPoints + 8 );
	
	ptrTangent[ 0 ] = ( GLshort )decMath::clamp( ( int )( tangent.x * 32767.0f ), -32768, 32767 );
	ptrTangent[ 1 ] = ( GLshort )decMath::clamp( ( int )( tangent.y * 32767.0f ), -32768, 32767 );
	ptrTangent[ 2 ] = ( GLshort )decMath::clamp( ( int )( tangent.z * 32767.0f ), -32768, 32767 );
	ptrTangent[ 3 ] = ( GLshort )( negateTangent ? -32768 : 32767 );
	
	ptrTexCoord[ 0 ] = ( GLfloat )texCoord.x;
	ptrTexCoord[ 1 ] = ( GLfloat )texCoord.y;
	
	pDataPoints += 16;
}

void deoglVBOWriterModel::WriteWeight( int weight ){
	if( ! pDataPoints ){
		DETHROW( deeInvalidParam );
	}
	
	GLint * const ptrWeight = ( GLint* )pDataPoints;
	
	ptrWeight[ 0 ] = ( GLint )weight;
	
	pDataPoints += 4;
}

void deoglVBOWriterModel::WriteIndices( int index1, int index2, int index3 ){
	if( ! pDataIndices ){
		DETHROW( deeInvalidParam );
	}
	
	pDataIndices[ 0 ] = ( GLuint )index1;
	pDataIndices[ 1 ] = ( GLuint )index2;
	pDataIndices[ 2 ] = ( GLuint )index3;
	
	pDataIndices += 3;
}
