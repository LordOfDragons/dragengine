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

#include "deoglPersistentRenderTaskPool.h"
#include "deoglPersistentRenderTask.h"
#include "deoglPersistentRenderTaskShader.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskInstance.h"
#include "deoglPersistentRenderTaskSubInstance.h"
#include "deoglPersistentRenderTaskOwner.h"
#include "../../../capabilities/deoglCapabilities.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../renderthread/deoglRTLogger.h"
#include "../../../renderthread/deoglRTBufferObject.h"
#include "../../../skin/shader/deoglSkinShader.h"
#include "../../../shaders/deoglShaderProgram.h"
#include "../../../shaders/deoglShaderUnitSourceCode.h"
#include "../../../shaders/paramblock/deoglSPBParameter.h"
#include "../../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../../texture/cubemap/deoglCubeMap.h"
#include "../../../texture/texture2d/deoglTexture.h"
#include "../../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglPersistentRenderTask
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTask::deoglPersistentRenderTask( deoglPersistentRenderTaskPool &pool ) :
pPool( pool ),
pRenderParamBlock( NULL ),
pTBOInstances( 0 ),
pSPBInstanceMaxEntries( 0 ),
pUseSPBInstanceFlags( false ){
}

deoglPersistentRenderTask::~deoglPersistentRenderTask(){
	RemoveAllShaders();
	RemoveAllOwners();
}



// Management
///////////////

void deoglPersistentRenderTask::PrepareForRender( deoglRenderThread &renderThread ){
	if( pShaders.GetCount() == 0 ){
		return;
	}
	
	pCalcSPBInstancesMaxEntries( renderThread );
	pAssignSPBInstances( renderThread );
	pUpdateSPBInstances();
}

void deoglPersistentRenderTask::SetRenderParamBlock( deoglSPBlockUBO *paramBlock ){
	pRenderParamBlock = paramBlock;
}

void deoglPersistentRenderTask::SetTBOInstances( GLuint tbo ){
	pTBOInstances = tbo;
}

void deoglPersistentRenderTask::SetUseSPBInstanceFlags( bool useFlags ){
	pUseSPBInstanceFlags = useFlags;
}



int deoglPersistentRenderTask::GetOwnerCount() const{
	return pOwners.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTask::GetRootOwner() const{
	return pOwners.GetRoot();
}

deoglPersistentRenderTaskOwner *deoglPersistentRenderTask::GetOwnerWith( deObject *owner, unsigned int hash ) const{
	deoglPersistentRenderTaskOwner *rtowner;
	return pOwnersMap.GetAt( owner, hash, ( void** )&rtowner ) ? rtowner : NULL;
}

deoglPersistentRenderTaskOwner *deoglPersistentRenderTask::AddOwner( deObject *owner, unsigned int hash ){
	// commented out in the name of performance
// 	if( pOwnersMap.Has( owner, hash ) ){
// 		DETHROW( deeInvalidParam );
// 	}
	
	deoglPersistentRenderTaskOwner * const rtowner = pPool.GetOwner();
	rtowner->SetOwner( owner, hash );
	pOwners.Add( &rtowner->GetLLTask() );
	pOwnersMap.SetAt( owner, hash, rtowner );
	return rtowner;
}

void deoglPersistentRenderTask::RemoveOwner( deoglPersistentRenderTaskOwner *owner ){
	if( ! owner ){
		DETHROW( deeInvalidParam );
	}
	
	pOwnersMap.Remove( owner->GetOwner(), owner->GetHash() );
	pOwners.Remove( &owner->GetLLTask() );
	pPool.ReturnOwner( owner );
}

void deoglPersistentRenderTask::RemoveAllOwners(){
	decPointerLinkedList::cListEntry *iter = pOwners.GetRoot();
	while( iter ){
		pPool.ReturnOwner( ( deoglPersistentRenderTaskOwner* )iter->GetOwner() );
		iter = iter->GetNext();
	}
	pOwners.RemoveAll();
	pOwnersMap.RemoveAll();
}



int deoglPersistentRenderTask::GetShaderCount() const{
	return pShaders.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTask::GetRootShader() const{
	return pShaders.GetRoot();
}

deoglPersistentRenderTaskShader *deoglPersistentRenderTask::GetShaderWith( deoglShaderProgram *shader ) const{
	decPointerLinkedList::cListEntry *iter = pShaders.GetRoot();
	while( iter ){
		deoglPersistentRenderTaskShader * const rtshader = ( deoglPersistentRenderTaskShader* )iter->GetOwner();
		if( rtshader->GetShader() == shader ){
			return rtshader;
		}
		iter = iter->GetNext();
	}
	
	return NULL;
}

deoglPersistentRenderTaskShader *deoglPersistentRenderTask::AddShader( deoglShaderProgram *shader ){
	deoglPersistentRenderTaskShader * const rtshader = pPool.GetShader();
	pShaders.Add( &rtshader->GetLLTask() );
	rtshader->SetParentTask( this );
	rtshader->SetShader( shader );
	return rtshader;
}

void deoglPersistentRenderTask::RemoveShader( deoglPersistentRenderTaskShader *shader ){
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
	
	pShaders.Remove( &shader->GetLLTask() );
	pPool.ReturnShader( shader );
}

void deoglPersistentRenderTask::RemoveAllShaders(){
	decPointerLinkedList::cListEntry *iter = pShaders.GetRoot();
	while( iter ){
		pPool.ReturnShader( ( deoglPersistentRenderTaskShader* )iter->GetOwner() );
		iter = iter->GetNext();
	}
	pShaders.RemoveAll();
}



int deoglPersistentRenderTask::GetTotalPointCount() const{
	decPointerLinkedList::cListEntry *iter = pShaders.GetRoot();
	int totalPointCount = 0;
	
	while( iter ){
		totalPointCount += ( ( deoglPersistentRenderTaskShader* )iter->GetOwner() )->GetTotalPointCount();
		iter = iter->GetNext();
	}
	
	return totalPointCount;
}

int deoglPersistentRenderTask::GetTotalTextureCount() const{
	decPointerLinkedList::cListEntry *iter = pShaders.GetRoot();
	int totalTextureCount = 0;
	
	while( iter ){
		totalTextureCount += ( ( deoglPersistentRenderTaskShader* )iter->GetOwner() )->GetTextureCount();
		iter = iter->GetNext();
	}
	
	return totalTextureCount;
}

int deoglPersistentRenderTask::GetTotalVAOCount() const{
	decPointerLinkedList::cListEntry *iter = pShaders.GetRoot();
	int totalVAOCount = 0;
	
	while( iter ){
		totalVAOCount += ( ( deoglPersistentRenderTaskShader* )iter->GetOwner() )->GetTotalVAOCount();
		iter = iter->GetNext();
	}
	
	return totalVAOCount;
}

int deoglPersistentRenderTask::GetTotalInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pShaders.GetRoot();
	int totalInstanceCount = 0;
	
	while( iter ){
		totalInstanceCount += ( ( deoglPersistentRenderTaskShader* )iter->GetOwner() )->GetTotalInstanceCount();
		iter = iter->GetNext();
	}
	
	return totalInstanceCount;
}

int deoglPersistentRenderTask::GetTotalSubInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pShaders.GetRoot();
	int totalSubInstanceCount = 0;
	
	while( iter ){
		totalSubInstanceCount += ( ( deoglPersistentRenderTaskShader* )iter->GetOwner() )->GetTotalSubInstanceCount();
		iter = iter->GetNext();
	}
	
	return totalSubInstanceCount;
}

void deoglPersistentRenderTask::Clear(){
	RemoveAllShaders();
	RemoveAllOwners();
}

void deoglPersistentRenderTask::RemoveOwnedBy( deoglPersistentRenderTaskOwner &owner ){
	// remove sub instances
	int i, count = owner.GetSubInstanceCount();
	for( i=0; i<count; i++ ){
		deoglPersistentRenderTaskSubInstance * const subInstance = owner.GetSubInstanceAt( i );
		deoglPersistentRenderTaskInstance * const instance = subInstance->GetParentInstance();
		instance->RemoveSubInstance( subInstance );
		instance->RemoveFromParentIfEmpty();
	}
	owner.RemoveAllSubInstances();
	
	// remove instances
	count = owner.GetInstanceCount();
	for( i=0; i<count; i++ ){
		deoglPersistentRenderTaskInstance * const instance = owner.GetInstanceAt( i );
		deoglPersistentRenderTaskVAO * const vao = instance->GetParentVAO();
		vao->RemoveInstance( instance );
		vao->RemoveFromParentIfEmpty();
	}
	owner.RemoveAllInstances();
}



// Private Functions
//////////////////////

void deoglPersistentRenderTask::pCalcSPBInstancesMaxEntries( deoglRenderThread &renderThread ){
	// since std140 layout adds a lot of padding between array elements we use ivec4.
	// this groups indices in blocks of four so the final index is pSPB[i/4][i%4].
	//pSPBInstanceMaxEntries = ( renderThread.GetCapabilities().GetUBOMaxSize() / 16 ) * 4;
	pSPBInstanceMaxEntries = renderThread.GetBufferObject().GetInstanceArraySizeUBO();
	
	if( pUseSPBInstanceFlags ){
		// if instance flags are used the vector <instanceIndex, instanceFlags> is
		// used instead. this reduces the max entries count by factor 2 but allows
		// to transport a scratch flags value in a fast way
		pSPBInstanceMaxEntries /= 2;
	}
}

void deoglPersistentRenderTask::pAssignSPBInstances( deoglRenderThread &renderThread ){
	decPointerLinkedList::cListEntry *iterShader = pShaders.GetRoot();
	const int componentsPerIndex = pUseSPBInstanceFlags ? 2 : 1;
	deoglShaderParameterBlock *paramBlock = NULL;
	int paramBlockCount = 0;
	int firstIndex = 0;
	
	while( iterShader ){
		const deoglPersistentRenderTaskShader &shader = *( ( deoglPersistentRenderTaskShader* )iterShader->GetOwner() );
		decPointerLinkedList::cListEntry *iterTexture = shader.GetRootTexture();
		
		while( iterTexture ){
			deoglPersistentRenderTaskTexture &texture = *( ( deoglPersistentRenderTaskTexture* )iterTexture->GetOwner() );
			decPointerLinkedList::cListEntry *iterVAO = texture.GetRootVAO();
			
			while( iterVAO ){
				deoglPersistentRenderTaskVAO &vao = *( ( deoglPersistentRenderTaskVAO* )iterVAO->GetOwner() );
				decPointerLinkedList::cListEntry *iterInstance = vao.GetRootInstance();
				
				while( iterInstance ){
					deoglPersistentRenderTaskInstance &instance = *( ( deoglPersistentRenderTaskInstance* )iterInstance->GetOwner() );
					
					if( ! paramBlock || firstIndex + instance.GetSubInstanceCount() > pSPBInstanceMaxEntries ){
						if( paramBlock ){
							paramBlock->SetElementCount( componentsPerIndex
								* decMath::max( ( ( firstIndex - 1 ) / 4 ) + 1, 1 ) );
						}
						
						if( paramBlockCount == pSPBInstances.GetCount() ){
							pCreateSPBInstanceParamBlock( renderThread );
						}
						paramBlock = pSPBInstances.GetAt( paramBlockCount++ );
						firstIndex = 0;
					}
					
					instance.SetSIIndexInstanceParam( paramBlock, firstIndex );
					firstIndex += instance.GetSubInstanceCount();
					
					iterInstance = iterInstance->GetNext();
				}
				iterVAO = iterVAO->GetNext();
			}
			iterTexture = iterTexture->GetNext();
		}
		iterShader = iterShader->GetNext();
	}
	
	if( paramBlock ){
		paramBlock->SetElementCount( componentsPerIndex
			* decMath::max( ( ( firstIndex - 1 ) / 4 ) + 1, 1 ) );
	}
}

void deoglPersistentRenderTask::pUpdateSPBInstances(){
	deoglShaderParameterBlock *paramBlock = NULL;
	
	try{
		decPointerLinkedList::cListEntry *iterShader = pShaders.GetRoot();
		
		while( iterShader ){
			const deoglPersistentRenderTaskShader &shader = *( ( deoglPersistentRenderTaskShader* )iterShader->GetOwner() );
			decPointerLinkedList::cListEntry *iterTexture = shader.GetRootTexture();
			
			while( iterTexture ){
				deoglPersistentRenderTaskTexture &texture = *( ( deoglPersistentRenderTaskTexture* )iterTexture->GetOwner() );
				decPointerLinkedList::cListEntry *iterVAO = texture.GetRootVAO();
				
				while( iterVAO ){
					deoglPersistentRenderTaskVAO &vao = *( ( deoglPersistentRenderTaskVAO* )iterVAO->GetOwner() );
					decPointerLinkedList::cListEntry *iterInstance = vao.GetRootInstance();
					
					while( iterInstance ){
						deoglPersistentRenderTaskInstance &instance = *( ( deoglPersistentRenderTaskInstance* )iterInstance->GetOwner() );
						
						if( instance.GetSIIndexInstanceSPB() != paramBlock ){
							if( paramBlock ){
								paramBlock->UnmapBuffer();
								paramBlock = NULL;
							}
							
							instance.GetSIIndexInstanceSPB()->MapBuffer();
							paramBlock = instance.GetSIIndexInstanceSPB();
						}
						
						instance.WriteSIIndexInstanceInt( pUseSPBInstanceFlags );
						
						iterInstance = iterInstance->GetNext();
					}
					iterVAO = iterVAO->GetNext();
				}
				iterTexture = iterTexture->GetNext();
			}
			iterShader = iterShader->GetNext();
		}
		
		if( paramBlock ){
			paramBlock->UnmapBuffer();
		}
		
	}catch( const deException & ){
		if( paramBlock ){
			paramBlock->UnmapBuffer();
		}
		throw;
	}
}

void deoglPersistentRenderTask::pCreateSPBInstanceParamBlock( deoglRenderThread &renderThread ){
	// since std140 layout adds a lot of padding between array elements we use ivec4.
	// this groups indices in blocks of four so the final index is pSPB[i/4][i%4]
	deoglSPBlockUBO * const ubo = new deoglSPBlockUBO( renderThread );
	
	try{
		ubo->SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		ubo->SetParameterCount( 1 );
		ubo->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 );
		ubo->MapToStd140();
		ubo->SetBindingPoint( deoglSkinShader::eubInstanceIndex );
		pSPBInstances.Add( ubo );
		
	}catch( const deException & ){
		if( ubo ){
			ubo->FreeReference();
		}
		throw;
	}
	
	ubo->FreeReference();
}
