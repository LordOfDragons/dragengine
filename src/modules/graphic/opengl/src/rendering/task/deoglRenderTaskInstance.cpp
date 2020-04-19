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

#include "deoglRenderTaskInstance.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/deoglSPBlockMemory.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBElement.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskInstance
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskInstance::deoglRenderTaskInstance() :
pParamBlock( NULL ),
pParamBlockSpecial( NULL ),

pFirstPoint( 0 ),
pPointCount( 0 ),
pFirstIndex( 0 ),
pIndexCount( 0 ),
pSubInstanceCount( 0 ),
pSubInstanceSPB( NULL ),
pGroup( NULL ),
pDoubleSided( false ),
pPrimitiveType( GL_TRIANGLES ),
pTessPatchVertexCount( 3 ),

pSIIndexInstanceSPB( NULL ),
pSIIndexInstanceFirst( 0 ),

pNextInstance( NULL ),

pLLNext( NULL ){
}

deoglRenderTaskInstance::~deoglRenderTaskInstance(){
}



// Management
///////////////

void deoglRenderTaskInstance::SetParameterBlock( deoglShaderParameterBlock *block ){
	pParamBlock = block;
}

void deoglRenderTaskInstance::SetParameterBlockSpecial( deoglShaderParameterBlock *block ){
	pParamBlockSpecial = block;
}

void deoglRenderTaskInstance::SetFirstPoint( int firstPoint ){
	pFirstPoint = firstPoint;
}

void deoglRenderTaskInstance::SetPointCount( int pointCount ){
	pPointCount = pointCount;
}

void deoglRenderTaskInstance::SetFirstIndex( int firstIndex ){
	pFirstIndex = firstIndex;
}

void deoglRenderTaskInstance::SetIndexCount( int indexCount ){
	pIndexCount = indexCount;
}

void deoglRenderTaskInstance::SetSubInstanceCount( int subInstanceCount ){
	pSubInstanceCount = subInstanceCount;
}

void deoglRenderTaskInstance::SetSubInstanceSPB( deoglSharedSPB *spb ){
	pSubInstanceSPB = spb;
}

void deoglRenderTaskInstance::SetGroup( deoglRenderTaskInstanceGroup *group ){
	pGroup = group;
}

void deoglRenderTaskInstance::SetDoubleSided( bool doubleSided ){
	pDoubleSided = doubleSided;
}

void deoglRenderTaskInstance::SetPrimitiveType( GLenum primitiveType ){
	pPrimitiveType = primitiveType;
}

void deoglRenderTaskInstance::SetTessPatchVertexCount( int count ){
	pTessPatchVertexCount = count;
}

void deoglRenderTaskInstance::SetNextInstance( deoglRenderTaskInstance *instance ){
	pNextInstance = instance;
}



void deoglRenderTaskInstance::AddSubInstance( int indexInstance, int flags ){
	pSIIndexInstance.Add( indexInstance );
	pSIFlags.Add( flags );
	pSubInstanceCount++;
}

void deoglRenderTaskInstance::SetSIIndexInstanceParam( deoglShaderParameterBlock *paramBlock,
int firstIndex ){
	pSIIndexInstanceSPB = paramBlock;
	pSIIndexInstanceFirst = firstIndex;
}

void deoglRenderTaskInstance::WriteSIIndexInstanceInt( bool useFlags ){
	if( ! pSIIndexInstanceSPB ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pSIIndexInstance.GetCount();
	int i;
	
	if( useFlags ){
		struct sIndexFlags{
			GLuint index;
			GLuint flags;
		};
		sIndexFlags * const data = ( sIndexFlags* )pSIIndexInstanceSPB->GetMappedBuffer()
			+ pSIIndexInstanceFirst;
		for( i=0; i<count; i++ ){
			data[ i ].index = ( GLuint )pSIIndexInstance.GetAt( i );
			data[ i ].flags = ( GLuint )pSIFlags.GetAt( i );
		}
		
	}else{
		struct sIndex{
			GLuint index;
		};
		sIndex * const data = ( sIndex* )pSIIndexInstanceSPB->GetMappedBuffer()
			+ pSIIndexInstanceFirst;
		for( i=0; i<count; i++ ){
			data[ i ].index = ( GLuint )pSIIndexInstance.GetAt( i );
		}
	}
}

void deoglRenderTaskInstance::WriteSIIndexInstanceShort( bool useFlags ){
	const int count = pSIIndexInstance.GetCount();
	int i;
	
	if( useFlags ){
		struct sIndexFlags{
			GLushort index;
			GLushort flags;
		};
		sIndexFlags * const data = ( sIndexFlags* )pSIIndexInstanceSPB->GetMappedBuffer()
			+ pSIIndexInstanceFirst;
		for( i=0; i<count; i++ ){
			data[ i ].index = ( GLushort )pSIIndexInstance.GetAt( i );
			data[ i ].flags = ( GLushort )pSIFlags.GetAt( i );
		}
		
	}else{
		struct sIndex{
			GLuint index;
		};
		sIndex * const data = ( sIndex* )pSIIndexInstanceSPB->GetMappedBuffer()
			+ pSIIndexInstanceFirst;
		for( i=0; i<count; i++ ){
			data[ i ].index = ( GLushort )pSIIndexInstance.GetAt( i );
		}
	}
}



void deoglRenderTaskInstance::Clear(){
	pParamBlock = NULL;
	pParamBlockSpecial = NULL;
	
	pFirstPoint = 0;
	pPointCount = 0;
	pFirstIndex = 0;
	pIndexCount = 0;
	pSubInstanceCount = 0;
	pSubInstanceSPB = NULL;
	pGroup = NULL;
	pDoubleSided = false;
	pPrimitiveType = GL_TRIANGLES;
	pTessPatchVertexCount = 3;
	
	pSIIndexInstance.RemoveAll();
	pSIFlags.RemoveAll();
	pSIIndexInstanceSPB = NULL;
	pSIIndexInstanceFirst = 0;
}



// Linked List
////////////////

void deoglRenderTaskInstance::SetLLNext( deoglRenderTaskInstance *instance ){
	pLLNext = instance;
}
