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

#include "deoglPersistentRenderTaskPool.h"
#include "deoglPersistentRenderTaskInstance.h"
#include "deoglPersistentRenderTaskSubInstance.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "../../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../../shaders/paramblock/deoglSPBlockMemory.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPBElement.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskInstance
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskInstance::deoglPersistentRenderTaskInstance( deoglPersistentRenderTaskPool &pool ) :
pPool( pool ),
pLLVAO( this ),

pParentVAO( NULL ),
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

pSubInstanceSPB( NULL ),
pSubInstanceSPBGroup( NULL ),
pSIIndexInstanceSPB( NULL ),
pSIIndexInstanceFirst( 0 ){
}

deoglPersistentRenderTaskInstance::~deoglPersistentRenderTaskInstance(){
	Clear();
}



// Management
///////////////

void deoglPersistentRenderTaskInstance::SetParentVAO( deoglPersistentRenderTaskVAO *vao ){
	pParentVAO = vao;
}

void deoglPersistentRenderTaskInstance::SetParameterBlock( deoglShaderParameterBlock *block ){
	pParamBlock = block;
}

void deoglPersistentRenderTaskInstance::SetParameterBlockSpecial( deoglShaderParameterBlock *block ){
	pParamBlockSpecial = block;
}

void deoglPersistentRenderTaskInstance::SetOwner( deoglPersistentRenderTaskOwner *owner ){
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



void deoglPersistentRenderTaskInstance::SetSubInstanceSPB( deoglSharedSPB *spb, deoglSharedSPBRTIGroup *group ){
	pSubInstanceSPB = spb;
	pSubInstanceSPBGroup = group;
}

int deoglPersistentRenderTaskInstance::GetSubInstanceCount() const{
	return pSubInstances.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTaskInstance::GetRootSubInstance() const{
	return pSubInstances.GetRoot();
}

deoglPersistentRenderTaskSubInstance *deoglPersistentRenderTaskInstance::AddSubInstance( int indexInstance, int flags ){
	deoglPersistentRenderTaskSubInstance * const subInstance = pPool.GetSubInstance();
	pSubInstances.Add( &subInstance->GetLLInstance() );
	
	subInstance->SetParentInstance( this );
	subInstance->SetIndexInstance( indexInstance );
	subInstance->SetFlags( flags );
	return subInstance;
}

void deoglPersistentRenderTaskInstance::RemoveSubInstance( deoglPersistentRenderTaskSubInstance *subInstance ){
	if( ! subInstance ){
		DETHROW( deeInvalidParam );
	}
	
	pSubInstances.Remove( &subInstance->GetLLInstance() );
	pPool.ReturnSubInstance( subInstance );
}

void deoglPersistentRenderTaskInstance::RemoveAllSubInstances(){
	decPointerLinkedList::cListEntry *iter = pSubInstances.GetRoot();
	while( iter ){
		pPool.ReturnSubInstance( ( deoglPersistentRenderTaskSubInstance* )iter->GetOwner() );
		iter = iter->GetNext();
	}
	pSubInstances.RemoveAll();
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
	
	if( useFlags ){
		struct sIndexFlags{
			GLuint index;
			GLuint flags;
		} *data = ( sIndexFlags* )pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		
		decPointerLinkedList::cListEntry *iterSubInstance = pSubInstances.GetRoot();
		while( iterSubInstance ){
			const deoglPersistentRenderTaskSubInstance &subInstance =
				*( ( deoglPersistentRenderTaskSubInstance* )iterSubInstance->GetOwner() );
			data->index = ( GLuint )subInstance.GetIndexInstance();
			data->flags = ( GLuint )subInstance.GetFlags();
			data++;
			iterSubInstance = iterSubInstance->GetNext();
		}
		
	}else{
		struct sIndex{
			GLuint index;
		} *data = ( sIndex* )pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		
		decPointerLinkedList::cListEntry *iterSubInstance = pSubInstances.GetRoot();
		while( iterSubInstance ){
			const deoglPersistentRenderTaskSubInstance &subInstance =
				*( ( deoglPersistentRenderTaskSubInstance* )iterSubInstance->GetOwner() );
			data->index = ( GLuint )subInstance.GetIndexInstance();
			data++;
			iterSubInstance = iterSubInstance->GetNext();
		}
	}
}

void deoglPersistentRenderTaskInstance::WriteSIIndexInstanceShort( bool useFlags ){
	if( ! pSIIndexInstanceSPB ){
		DETHROW( deeInvalidParam );
	}
	
	if( useFlags ){
		struct sIndexFlags{
			GLushort index;
			GLushort flags;
		} *data = ( sIndexFlags* )pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		
		decPointerLinkedList::cListEntry *iterSubInstance = pSubInstances.GetRoot();
		while( iterSubInstance ){
			const deoglPersistentRenderTaskSubInstance &subInstance =
				*( ( deoglPersistentRenderTaskSubInstance* )iterSubInstance->GetOwner() );
			data->index = ( GLushort )subInstance.GetIndexInstance();
			data->flags = ( GLushort )subInstance.GetFlags();
			data++;
			iterSubInstance = iterSubInstance->GetNext();
		}
		
	}else{
		struct sIndex{
			GLuint index;
		} *data = ( sIndex* )pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		
		decPointerLinkedList::cListEntry *iterSubInstance = pSubInstances.GetRoot();
		while( iterSubInstance ){
			const deoglPersistentRenderTaskSubInstance &subInstance =
				*( ( deoglPersistentRenderTaskSubInstance* )iterSubInstance->GetOwner() );
			data->index = ( GLushort )subInstance.GetIndexInstance();
			data++;
			iterSubInstance = iterSubInstance->GetNext();
		}
	}
}



void deoglPersistentRenderTaskInstance::Clear(){
	RemoveAllSubInstances();
	
	pParentVAO = NULL;
	pParamBlock = NULL;
	pParamBlockSpecial = NULL;
	
	pOwner = NULL;
	
	pFirstPoint = 0;
	pPointCount = 0;
	pFirstIndex = 0;
	pIndexCount = 0;
	pDoubleSided = false;
	pPrimitiveType = GL_TRIANGLES;
	pTessPatchVertexCount = 3;
	
	pSubInstanceSPB = NULL;
	pSubInstanceSPBGroup = NULL;
	pSIIndexInstanceSPB = NULL;
	pSIIndexInstanceFirst = 0;
}

void deoglPersistentRenderTaskInstance::RemoveFromParentIfEmpty(){
	if( pSubInstances.GetCount() == 0 ){
		pParentVAO->RemoveInstance( this );
	}
}
