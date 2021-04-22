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

#include "deoglPersistentRenderTask.h"
#include "deoglPersistentRenderTaskShader.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskInstance.h"
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

deoglPersistentRenderTask::deoglPersistentRenderTask() :
pRenderParamBlock( NULL ),
pTBOInstances( 0 ),
pSPBInstanceMaxEntries( 0 ),
pUseSPBInstanceFlags( false ){
}

deoglPersistentRenderTask::~deoglPersistentRenderTask(){
	RemoveAllShaders();
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



int deoglPersistentRenderTask::GetShaderCount() const{
	return pShaders.GetCount();
}

deoglPersistentRenderTaskShader *deoglPersistentRenderTask::GetShaderAt( int index ) const{
	return ( deoglPersistentRenderTaskShader* )pShaders.GetAt( index );
}

deoglPersistentRenderTaskShader *deoglPersistentRenderTask::GetShaderWith( deoglShaderProgram *shader ) const{
	const int count = pShaders.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglPersistentRenderTaskShader * const rtshader = ( deoglPersistentRenderTaskShader* )pShaders.GetAt( i );
		if( rtshader->GetShader() == shader ){
			return rtshader;
		}
	}
	
	return NULL;
}

deoglPersistentRenderTaskShader *deoglPersistentRenderTask::AddShader( deoglShaderProgram *shader ){
	deoglPersistentRenderTaskShader * const rtshader = new deoglPersistentRenderTaskShader( shader );
	pShaders.Add( rtshader );
	return rtshader;
}

void deoglPersistentRenderTask::RemoveAllShaders(){
	const int count = pShaders.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoglPersistentRenderTaskShader* )pShaders.GetAt( i );
	}
	pShaders.RemoveAll();
}



int deoglPersistentRenderTask::GetTotalPointCount() const{
	const int count = pShaders.GetCount();
	int totalPointCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		totalPointCount += ( ( deoglPersistentRenderTaskShader* )pShaders.GetAt( i ) )->GetTotalPointCount();
	}
	
	return totalPointCount;
}

int deoglPersistentRenderTask::GetTotalTextureCount() const{
	const int count = pShaders.GetCount();
	int totalTextureCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		totalTextureCount += ( ( deoglPersistentRenderTaskShader* )pShaders.GetAt( i ) )->GetTextureCount();
	}
	
	return totalTextureCount;
}

int deoglPersistentRenderTask::GetTotalVAOCount() const{
	const int count = pShaders.GetCount();
	int totalVAOCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		totalVAOCount += ( ( deoglPersistentRenderTaskShader* )pShaders.GetAt( i ) )->GetTotalVAOCount();
	}
	
	return totalVAOCount;
}

int deoglPersistentRenderTask::GetTotalInstanceCount() const{
	const int count = pShaders.GetCount();
	int totalInstanceCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		totalInstanceCount += ( ( deoglPersistentRenderTaskShader* )pShaders.GetAt( i ) )->GetTotalInstanceCount();
	}
	
	return totalInstanceCount;
}

int deoglPersistentRenderTask::GetTotalSubInstanceCount() const{
	const int count = pShaders.GetCount();
	int totalSubInstanceCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		totalSubInstanceCount += ( ( deoglPersistentRenderTaskShader* )pShaders.GetAt( i ) )->GetTotalSubInstanceCount();
	}
	
	return totalSubInstanceCount;
}

void deoglPersistentRenderTask::RemoveOwnedBy( void *owner ){
	int i, count = pShaders.GetCount();
	for( i=0; i<count; i++ ){
		deoglPersistentRenderTaskShader * const shader = ( deoglPersistentRenderTaskShader* )pShaders.GetAt( i );
		
		shader->RemoveOwnedBy( owner );
		
		if( shader->GetTextureCount() > 0 ){
			continue;
		}
		
		if( i < count - 1 ){
			pShaders.SetAt( i, pShaders.GetAt( count - 1 ) );
		}
		pShaders.RemoveFrom( count - 1 );
		count--;
		i--;
		
		delete shader;
	}
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
	const int componentsPerIndex = pUseSPBInstanceFlags ? 2 : 1;
	deoglShaderParameterBlock *paramBlock = NULL;
	const int shaderCount = pShaders.GetCount();
	int paramBlockCount = 0;
	int firstIndex = 0;
	int i;
	
	for( i=0; i<shaderCount; i++ ){
		const deoglPersistentRenderTaskShader &shader = *( ( deoglPersistentRenderTaskShader* )pShaders.GetAt( i ) );
		const int textureCount = shader.GetTextureCount();
		int j;
		
		for( j=0; j<textureCount; j++ ){
			deoglPersistentRenderTaskTexture &texture = *shader.GetTextureAt( j );
			const int vaoCount = texture.GetVAOCount();
			int k;
			
			for( k=0; k<vaoCount; k++ ){
				deoglPersistentRenderTaskVAO &vao = *texture.GetVAOAt( k );
				const int instanceCount = vao.GetInstanceCount();
				int l;
				
				for( l=0; l<instanceCount; l++ ){
					deoglPersistentRenderTaskInstance &instance = *vao.GetInstanceAt( l );
					
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
				}
			}
		}
	}
	
	if( paramBlock ){
		paramBlock->SetElementCount( componentsPerIndex
			* decMath::max( ( ( firstIndex - 1 ) / 4 ) + 1, 1 ) );
	}
}

void deoglPersistentRenderTask::pUpdateSPBInstances(){
	deoglShaderParameterBlock *paramBlock = NULL;
	
	try{
		const int shaderCount = pShaders.GetCount();
		int i;
		
		for( i=0; i<shaderCount; i++ ){
			const deoglPersistentRenderTaskShader &shader = *( ( deoglPersistentRenderTaskShader* )pShaders.GetAt( i ) );
			const int textureCount = shader.GetTextureCount();
			int j;
			
			for( j=0; j<textureCount; j++ ){
				deoglPersistentRenderTaskTexture &texture = *shader.GetTextureAt( j );
				const int vaoCount = texture.GetVAOCount();
				int k;
				
				for( k=0; k<vaoCount; k++ ){
					deoglPersistentRenderTaskVAO &vao = *texture.GetVAOAt( k );
					const int instanceCount = vao.GetInstanceCount();
					int l;
					
					for( l=0; l<instanceCount; l++ ){
						deoglPersistentRenderTaskInstance &instance = *vao.GetInstanceAt( l );
						
						if( instance.GetSIIndexInstanceSPB() != paramBlock ){
							if( paramBlock ){
								paramBlock->UnmapBuffer();
								paramBlock = NULL;
							}
							
							instance.GetSIIndexInstanceSPB()->MapBuffer();
							paramBlock = instance.GetSIIndexInstanceSPB();
						}
						
						instance.WriteSIIndexInstanceInt( pUseSPBInstanceFlags );
					}
				}
			}
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
