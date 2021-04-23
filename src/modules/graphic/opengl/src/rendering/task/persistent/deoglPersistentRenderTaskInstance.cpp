/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglPersistentRenderTaskInstance.h"
#include "../../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../../shaders/paramblock/deoglSPBlockMemory.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPBElement.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskInstance
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskInstance::deoglPersistentRenderTaskInstance(
	deoglSharedSPB *spb, deoglSharedSPBRTIGroup *group ) :
pParamBlock( NULL ),
pParamBlockSpecial( NULL ),
pOwner( NULL ),

pFirstPoint( 0 ),
pPointCount( 0 ),
pFirstIndex( 0 ),
pIndexCount( 0 ),
pDoubleSided( false ),
pPrimitiveType( GL_TRIANGLES ),
pTessPatchVertexCount( 3 ),

pSubInstances( NULL ),
pSubInstanceCount( 0 ),
pSubInstanceSize( 0 ),
pSubInstanceSPB( spb ),
pSubInstanceSPBRGroup( group ),
pSIIndexInstanceSPB( NULL ),
pSIIndexInstanceFirst( 0 ){
}

deoglPersistentRenderTaskInstance::~deoglPersistentRenderTaskInstance(){
	if( pSubInstances ){
		delete [] pSubInstances;
	}
}



// Management
///////////////

void deoglPersistentRenderTaskInstance::SetParameterBlock( deoglShaderParameterBlock *block ){
	pParamBlock = block;
}

void deoglPersistentRenderTaskInstance::SetParameterBlockSpecial( deoglShaderParameterBlock *block ){
	pParamBlockSpecial = block;
}

void deoglPersistentRenderTaskInstance::SetOwner( void *owner ){
	pOwner = owner;
}

void deoglPersistentRenderTaskInstance::SetFirstPoint( int firstPoint ){
	pFirstPoint = firstPoint;
}

void deoglPersistentRenderTaskInstance::SetPointCount( int pointCount ){
	pPointCount = pointCount;
}

void deoglPersistentRenderTaskInstance::SetFirstIndex( int firstIndex ){
	pFirstIndex = firstIndex;
}

void deoglPersistentRenderTaskInstance::SetIndexCount( int indexCount ){
	pIndexCount = indexCount;
}

void deoglPersistentRenderTaskInstance::SetDoubleSided( bool doubleSided ){
	pDoubleSided = doubleSided;
}

void deoglPersistentRenderTaskInstance::SetPrimitiveType( GLenum primitiveType ){
	pPrimitiveType = primitiveType;
}

void deoglPersistentRenderTaskInstance::SetTessPatchVertexCount( int count ){
	pTessPatchVertexCount = count;
}



const deoglPersistentRenderTaskInstance::sSubInstance &deoglPersistentRenderTaskInstance::GetSubinstanceAt( int index ) const{
	if( index < 0 || index >= pSubInstanceCount ){
		DETHROW( deeInvalidParam );
	}
	return pSubInstances[ index ];
}

void deoglPersistentRenderTaskInstance::AddSubInstance( int indexInstance, int flags, void *owner ){
	if( pSubInstanceCount == pSubInstanceSize ){
		const int newSize = pSubInstanceCount * 3 / 2 + 1;
		sSubInstance * const newArray = new sSubInstance[ newSize ];
		if( pSubInstances ){
			memcpy( newArray, pSubInstances, sizeof( sSubInstance ) * pSubInstanceCount );
			delete [] pSubInstances;
		}
		pSubInstances = newArray;
		pSubInstanceSize = newSize;
	}
	
	sSubInstance &subInstance = pSubInstances[ pSubInstanceCount++ ];
	subInstance.indexInstance = indexInstance;
	subInstance.flags = flags;
	subInstance.owner = owner;
}

void deoglPersistentRenderTaskInstance::RemoveSubInstance( int index ){
	if( index < 0 || index >= pSubInstanceCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( index < pSubInstanceCount - 1 ){
		pSubInstances[ index ] = pSubInstances[ pSubInstanceCount - 1 ];
	}
	pSubInstanceCount--;
}

void deoglPersistentRenderTaskInstance::RemoveAllSubInstances(){
	pSubInstanceCount = 0;
}

void deoglPersistentRenderTaskInstance::SetSIIndexInstanceParam(
deoglShaderParameterBlock *paramBlock, int firstIndex ){
	pSIIndexInstanceSPB = paramBlock;
	pSIIndexInstanceFirst = firstIndex;
}

void deoglPersistentRenderTaskInstance::WriteSIIndexInstanceInt( bool useFlags ){
	if( ! pSIIndexInstanceSPB ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	if( useFlags ){
		struct sIndexFlags{
			GLuint index;
			GLuint flags;
		} * const data = ( sIndexFlags* )pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		for( i=0; i<pSubInstanceCount; i++ ){
			data[ i ].index = ( GLuint )pSubInstances[ i ].indexInstance;
			data[ i ].flags = ( GLuint )pSubInstances[ i ].flags;
		}
		
	}else{
		struct sIndex{
			GLuint index;
		} * const data = ( sIndex* )pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		for( i=0; i<pSubInstanceCount; i++ ){
			data[ i ].index = ( GLuint )pSubInstances[ i ].indexInstance;
		}
	}
}

void deoglPersistentRenderTaskInstance::WriteSIIndexInstanceShort( bool useFlags ){
	if( ! pSIIndexInstanceSPB ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	if( useFlags ){
		struct sIndexFlags{
			GLushort index;
			GLushort flags;
		} * const data = ( sIndexFlags* )pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		for( i=0; i<pSubInstanceCount; i++ ){
			data[ i ].index = ( GLushort )pSubInstances[ i ].indexInstance;
			data[ i ].flags = ( GLushort )pSubInstances[ i ].flags;
		}
		
	}else{
		struct sIndex{
			GLuint index;
		} * const data = ( sIndex* )pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		for( i=0; i<pSubInstanceCount; i++ ){
			data[ i ].index = ( GLushort )pSubInstances[ i ].indexInstance;
		}
	}
}



void deoglPersistentRenderTaskInstance::RemoveOwnedBy( void *owner ){
	int i;
	for( i=0; i<pSubInstanceCount; i++ ){
		if( pSubInstances[ i ].owner != owner ){
			continue;
		}
		
		if( i < pSubInstanceCount - 1 ){
			pSubInstances[ i ] = pSubInstances[ pSubInstanceCount - 1 ];
		}
		pSubInstanceCount--;
		i--;
	}
}
