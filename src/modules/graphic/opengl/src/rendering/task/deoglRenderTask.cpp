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

#include "deoglRenderTask.h"
#include "deoglRenderTaskShader.h"
#include "deoglRenderTaskTexture.h"
#include "deoglRenderTaskVAO.h"
#include "deoglRenderTaskInstance.h"
#include "deoglRenderTaskInstanceGroup.h"
#include "shared/deoglRenderTaskSharedShader.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTBufferObject.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderUnitSourceCode.h"
#include "../../shaders/paramblock/deoglSPBParameter.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../utils/deoglQuickSorter.h"
#include "../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglRenderTask
//////////////////////////

unsigned int deoglRenderTask::pUpdateTracking = 0;

// Constructor, destructor
////////////////////////////

deoglRenderTask::deoglRenderTask() :
pRenderParamBlock( NULL ),
pTBOInstances( 0 ),
pSPBInstanceMaxEntries( 0 ),
pUseSPBInstanceFlags( false ),
pTrackingNumber( 0 ),

pRootShader( NULL ),
pTailShader( NULL ),
pShaderCount( 0 ),
pHasShader( NULL ),
pHasShaderCount( 0 ),
pHasShaderSize( 0 ),

pRootTexturePool( NULL ),
pTailTexturePool( NULL ),
pNextTexturePool( NULL ),
pTexturePoolCount( 0 ),

pRootVAOPool( NULL ),
pTailVAOPool( NULL ),
pNextVAOPool( NULL ),
pVAOPoolCount( 0 ),

pRootInstancePool( NULL ),
pTailInstancePool( NULL ),
pNextInstancePool( NULL ),
pInstancePoolCount( 0 ){
}

deoglRenderTask::~deoglRenderTask(){
	const int shaderCount = pListShaders.GetCount();
	int i;
	
	if( pHasShader ){
		delete [] pHasShader;
	}
	
	for( i=0; i<shaderCount; i++ ){
		delete ( deoglRenderTaskShader* )pListShaders.GetAt( i );
	}
	pListShaders.RemoveAll();
	
	while( pRootTexturePool ){
		pTailTexturePool = pRootTexturePool;
		pRootTexturePool = pRootTexturePool->GetLLNext();
		delete pTailTexturePool;
	}
	pTailTexturePool = NULL;
	pNextTexturePool = NULL;
	pTexturePoolCount = 0;
	
	while( pRootVAOPool ){
		pTailVAOPool = pRootVAOPool;
		pRootVAOPool = pRootVAOPool->GetLLNext();
		delete pTailVAOPool;
	}
	pTailVAOPool = NULL;
	pNextVAOPool = NULL;
	pVAOPoolCount = 0;
	
	while( pRootInstancePool ){
		pTailInstancePool = pRootInstancePool;
		pRootInstancePool = pRootInstancePool->GetLLNext();
		delete pTailInstancePool;
	}
	pTailInstancePool = NULL;
	pNextInstancePool = NULL;
	pInstancePoolCount = 0;
}



// Management
///////////////

void deoglRenderTask::Clear(){
	pHasShaderCount = 0;
	pRootShader = NULL;
	pTailShader = NULL;
	pShaderCount = 0;
	
	pListInstanceGroup.RemoveAll();
	pListVAOs.RemoveAll();
	pListTUCs.RemoveAll();
	pNextTexturePool = pRootTexturePool;
	pNextVAOPool = pRootVAOPool;
	pNextInstancePool = pRootInstancePool;
	
	SetRenderParamBlock( NULL );
	SetTBOInstances( 0 );
	pUseSPBInstanceFlags = false;
	pTrackingNumber = UpdateTracking();
}

void deoglRenderTask::PrepareForRender( deoglRenderThread &renderThread ){
	if( pShaderCount == 0 ){
		return;
	}
	
	pCalcSPBInstancesMaxEntries( renderThread );
	pAssignSPBInstances( renderThread );
	pUpdateSPBInstances();
}

void deoglRenderTask::SortInstancesByDistance( deoglQuickSorter &sorter,
const decDVector &position, const decDVector &direction ){
	const double posDotDir = position * direction;
	deoglRenderTaskTexture *texture;
	deoglRenderTaskVAO *vao;
	int i;
	
	for( i=0; i<pShaderCount; i++ ){
		const deoglRenderTaskShader &shader = *( ( deoglRenderTaskShader* )pListShaders.GetAt( i ) );
		
		texture = shader.GetRootTexture();
		
		while( texture ){
			vao = texture->GetRootVAO();
			
			while( vao ){
				vao->SortInstancesByDistance( sorter, position, direction, posDotDir );
				vao = vao->GetNextVAO();
			}
			
			texture = texture->GetNextTexture();
		}
	}
}

void deoglRenderTask::SetRenderParamBlock( deoglSPBlockUBO *paramBlock ){
	pRenderParamBlock = paramBlock;
}

void deoglRenderTask::SetTBOInstances( GLuint tbo ){
	pTBOInstances = tbo;
}

void deoglRenderTask::SetUseSPBInstanceFlags( bool useFlags ){
	pUseSPBInstanceFlags = useFlags;
}



unsigned int deoglRenderTask::UpdateTracking(){
	// additional tracker value for shaders and texture unit configurations to avoid
	// the need for a clearing operation. the render task class stores a global tracker
	// number. whenever a new render task is started (using the Clear function). the
	// global tracker number is increased by one. while building the render task the
	// global tracker number is compared to the tracker number stored in the
	// shader/tuc. if it is not the same the shader/tuc has not been added to the
	// render task yet. the render task is set and the tracker number updated. if the
	// tracker number is the same the render task is already set and can be used
	// directly.
	// 
	// this method updates the tracker number by one wrapping around upon overflowing
	// and returns the new tracker number
	return ++pUpdateTracking;
}



deoglRenderTaskShader *deoglRenderTask::AddShader( deoglRenderTaskSharedShader *shader ){
	const int index = shader->GetIndex();
	
	if( index >= pHasShaderCount ){
		if( index >= pHasShaderSize ){
			deoglRenderTaskShader ** const newArray = new deoglRenderTaskShader*[ index + 1 ];
			
			if( pHasShader ){
				if( pHasShaderCount > 0 ){
					memcpy( newArray, pHasShader, sizeof( deoglRenderTaskShader* ) * pHasShaderCount );
				}
				delete [] pHasShader;
			}
			
			pHasShader = newArray;
			pHasShaderSize = index + 1;
		}
		
		if( pHasShaderCount <= index ){
			memset( pHasShader + pHasShaderCount, 0, sizeof( deoglRenderTaskShader* ) * ( index - pHasShaderCount + 1 ) );
			pHasShaderCount = index + 1;
		}
		pHasShaderCount++;
	}
	
	deoglRenderTaskShader *rtshader = pHasShader[ index ];
	if( rtshader ){
		return rtshader;
	}
	
	if( pShaderCount == pListShaders.GetCount() ){
		rtshader = new deoglRenderTaskShader;
		pListShaders.Add( rtshader );
		
	}else{
		rtshader = ( deoglRenderTaskShader* )pListShaders.GetAt( pShaderCount );
		rtshader->Reset();
	}
	pShaderCount++;
	
	if( pTailShader ){
		pTailShader->SetNextShader( rtshader );
	}
	rtshader->SetNextShader( NULL );
	
	pTailShader = rtshader;
	
	if( ! pRootShader ){
		pRootShader = rtshader;
	}
	
	rtshader->SetShader( shader );
	pHasShader[ index ] = rtshader;
	return rtshader;
}



deoglRenderTaskTexture *deoglRenderTask::TextureFromPool(){
	deoglRenderTaskTexture *texture = pNextTexturePool;
	
	if( texture ){
		texture->Reset();
		
	}else{
		texture = new deoglRenderTaskTexture;
		
		if( pTailTexturePool ){
			pTailTexturePool->SetLLNext( texture );
		}
		
		pTailTexturePool = texture;
		
		if( ! pRootTexturePool ){
			pRootTexturePool = texture;
		}
		
		pTexturePoolCount++;
	}
	
	pNextTexturePool = texture->GetLLNext();
	
	return texture;
}

deoglRenderTaskVAO *deoglRenderTask::VAOFromPool(){
	deoglRenderTaskVAO *vao = pNextVAOPool;
	
	if( vao ){
		vao->Reset();
		
	}else{
		vao = new deoglRenderTaskVAO;
		
		if( pTailVAOPool ){
			pTailVAOPool->SetLLNext( vao );
		}
		
		pTailVAOPool = vao;
		
		if( ! pRootVAOPool ){
			pRootVAOPool = vao;
		}
		
		pVAOPoolCount++;
	}
	
	pNextVAOPool = vao->GetLLNext();
	
	return vao;
}

deoglRenderTaskInstance *deoglRenderTask::InstanceFromPool(){
	deoglRenderTaskInstance *instance = pNextInstancePool;
	
	if( ! instance ){
		instance = new deoglRenderTaskInstance;
		
		if( pTailInstancePool ){
			pTailInstancePool->SetLLNext( instance );
		}
		
		pTailInstancePool = instance;
		
		if( ! pRootInstancePool ){
			pRootInstancePool = instance;
		}
		
		pInstancePoolCount++;
	}
	
	pNextInstancePool = instance->GetLLNext();
	
	instance->Clear();
	return instance;
}



int deoglRenderTask::GetTotalPointCount() const{
	int s, totalPointCount = 0;
	
	for( s=0; s<pShaderCount; s++ ){
		totalPointCount += ( ( deoglRenderTaskShader* )pListShaders.GetAt( s ) )->GetTotalPointCount();
	}
	
	return totalPointCount;
}

int deoglRenderTask::GetTotalTextureCount() const{
	int s, totalTextureCount = 0;
	
	for( s=0; s<pShaderCount; s++ ){
		totalTextureCount += ( ( deoglRenderTaskShader* )pListShaders.GetAt( s ) )->GetTextureCount();
	}
	
	return totalTextureCount;
}

int deoglRenderTask::GetTotalVAOCount() const{
	int s, totalVAOCount = 0;
	
	for( s=0; s<pShaderCount; s++ ){
		totalVAOCount += ( ( deoglRenderTaskShader* )pListShaders.GetAt( s ) )->GetTotalVAOCount();
	}
	
	return totalVAOCount;
}

int deoglRenderTask::GetTotalInstanceCount() const{
	int s, totalInstanceCount = 0;
	
	for( s=0; s<pShaderCount; s++ ){
		totalInstanceCount += ( ( deoglRenderTaskShader* )pListShaders.GetAt( s ) )->GetTotalInstanceCount();
	}
	
	return totalInstanceCount;
}

int deoglRenderTask::GetTotalSubInstanceCount() const{
	int s, totalSubInstanceCount = 0;
	
	for( s=0; s<pShaderCount; s++ ){
		totalSubInstanceCount += ( ( deoglRenderTaskShader* )pListShaders.GetAt( s ) )->GetTotalSubInstanceCount();
	}
	
	return totalSubInstanceCount;
}



void deoglRenderTask::AddTUC( deoglTexUnitsConfig *tuc ){
	if( ! tuc ){
		DETHROW( deeInvalidParam );
	}
	
	tuc->SetRenderTaskTrackingNumber( pTrackingNumber );
	tuc->SetRenderTaskTUCIndex( pListTUCs.GetCount() );
	pListTUCs.Add( tuc );
}

void deoglRenderTask::AddVAO( deoglVAO *vao ){
	if( ! vao ){
		DETHROW( deeInvalidParam );
	}
	
	vao->SetRenderTaskTrackingNumber( pTrackingNumber );
	vao->SetRenderTaskVAOIndex( pListVAOs.GetCount() );
	pListVAOs.Add( vao );
}

void deoglRenderTask::AddInstanceGroup( deoglRenderTaskInstanceGroup *group ){
	if( ! group ){
		DETHROW( deeInvalidParam );
	}
	
	group->SetTrackingNumber( pTrackingNumber );
	group->SetIndex( pListInstanceGroup.GetCount() );
	pListInstanceGroup.Add( group );
}



// Debugging
//////////////

void deoglRenderTask::DebugPrint( deoglRTLogger &rtlogger ){
	const bool detailsInstances = true; //false;
	deoglRenderTaskInstance *instance;
	deoglRenderTaskTexture *texture;
	int s, t, v, i, u, d, sic;
	deoglRenderTaskVAO *vao;
	decString text;
	
	rtlogger.LogInfoFormat( "RenderTask %p: spb=%p shaders=%d points=%d tucs=%d vaos=%d igrps=%d",
		this, pRenderParamBlock, pShaderCount, GetTotalPointCount(), pListTUCs.GetCount(),
		pListVAOs.GetCount(), pListInstanceGroup.GetCount() );
	
	for( s=0; s<pShaderCount; s++ ){
		const deoglRenderTaskShader &shader = *( ( deoglRenderTaskShader* )pListShaders.GetAt( s ) );
		const deoglShaderDefines &defines = shader.GetShader()->GetShader()->GetDefines();
		
		rtlogger.LogInfoFormat( "- shader %i: shader=%p spb=%p textures=%i points=%i vaos=%i "
			"instances=%i subInstances=%i", s, shader.GetShader(), shader.GetParameterBlock(),
			shader.GetTextureCount(), shader.GetTotalPointCount(), shader.GetTotalVAOCount(),
			shader.GetTotalInstanceCount(), shader.GetTotalSubInstanceCount() );
		
		rtlogger.LogInfo( "  - configuration:" );
		rtlogger.LogInfoFormat( "    - vertex %s", shader.GetShader()->GetShader()->GetVertexSourceCode()
			? shader.GetShader()->GetShader()->GetVertexSourceCode()->GetFilePath() : "-" );
		rtlogger.LogInfoFormat( "    - geometry %s", shader.GetShader()->GetShader()->GetGeometrySourceCode()
			? shader.GetShader()->GetShader()->GetGeometrySourceCode()->GetFilePath() : "-" );
		rtlogger.LogInfoFormat( "    - fragment %s", shader.GetShader()->GetShader()->GetFragmentSourceCode()
			? shader.GetShader()->GetShader()->GetFragmentSourceCode()->GetFilePath() : "-" );
		
		text = "    - defines: ";
		const int defineCount = defines.GetDefineCount();
		for( d=0; d<defineCount; d++ ){
			const char *defineName = defines.GetDefineNameAt( d );
			const char *defineValue = defines.GetDefineValueAt( d );
			
			if( strlen( defineValue ) > 10 ){
				text.AppendFormat( "%s %s=%.10s...", d == 0 ? "" : ",", defineName, defineValue );
				
			}else{
				text.AppendFormat( "%s %s=%s", d == 0 ? "" : ",", defineName, defineValue );
			}
		}
		rtlogger.LogInfo( text.GetString() );
		
		texture = shader.GetRootTexture(); t=0;
		while( texture ){
			rtlogger.LogInfoFormat( "  - texture %i: spb=%p tuc=%p texture=%p vaos=%i "
				"points=%i instances=%i subInstances=%i", t, texture->GetParameterBlock(),
				texture->GetTUC(), texture->GetTexture(), texture->GetVAOCount(),
				texture->GetTotalPointCount(), texture->GetTotalInstanceCount(),
				texture->GetTotalSubInstanceCount() );
			
			const int unitCount = texture->GetTUC()->GetUnitCount();
			text.Format( "    units(" );
			for( u=0; u<unitCount; u++ ){
				const deoglTexUnitConfig &unit = texture->GetTUC()->GetUnitAt( u );
				if( unit.GetTexture() ){
					text.AppendFormat( " T%i", unit.GetTexture()->GetTexture() );
					
				}else if( unit.GetCubeMap() ){
					text.AppendFormat( " C%i", unit.GetCubeMap()->GetTexture() );
					
				}else if( unit.GetSpecial() != deoglTexUnitConfig::estNone ){
					text.AppendFormat( " S%i", unit.GetSpecial() );
					
				}else{
					text.AppendFormat( " -" );
				}
			}
			text.AppendFormat( " )" );
			rtlogger.LogInfo( text.GetString() );
			
			vao = texture->GetRootVAO(); v=0;
			while( vao ){
				sic = vao->GetTotalSubInstanceCount();
				
				rtlogger.LogInfoFormat( "    - vao %i: vao=%i instances=%i points=%i "
					"subInstances=%i", v, vao->GetVAO()->GetVAO(), vao->GetInstanceCount(),
					vao->GetTotalPointCount(), sic );
				
				if( detailsInstances ){
					instance = vao->GetRootInstance(); i=0;
					while( instance ){
						if( instance->GetSubInstanceSPB() ){
							rtlogger.LogInfoFormat( "        - instance %i: ds=%s fp=%i pc=%i "
								"fi=%i ic=%i sic=%i sispbi=%p sispbfi=%d", i,
								instance->GetDoubleSided() ? "t" : "n", instance->GetFirstPoint(),
								instance->GetPointCount(), instance->GetFirstIndex(),
								instance->GetIndexCount(), instance->GetSubInstanceCount(),
								instance->GetSIIndexInstanceSPB(),
								instance->GetSIIndexInstanceFirst() );
						}
						instance = instance->GetNextInstance(); i++;
					}
				}
				
				vao = vao->GetNextVAO(); v++;
			}
			
			texture = texture->GetNextTexture(); t++;
		}
	}
}

int deoglRenderTask::CountUsedTexturePool() const{
	deoglRenderTaskTexture *texture = pRootTexturePool;
	int usedCount = 0;
	
	while( texture && texture != pNextTexturePool ){
		usedCount++;
		texture = texture->GetLLNext();
	}
	
	return usedCount;
}

int deoglRenderTask::CountUsedVAOPool() const{
	deoglRenderTaskVAO *vao = pRootVAOPool;
	int usedCount = 0;
	
	while( vao && vao != pNextVAOPool ){
		usedCount++;
		vao = vao->GetLLNext();
	}
	
	return usedCount;
}

int deoglRenderTask::CountUsedInstancePool() const{
	deoglRenderTaskInstance *instance = pRootInstancePool;
	int usedCount = 0;
	
	while( instance && instance != pNextInstancePool ){
		usedCount++;
		instance = instance->GetLLNext();
	}
	
	return usedCount;
}

void deoglRenderTask::DebugPrintPoolStats( deoglRTLogger &rtlogger ){
	rtlogger.LogInfoFormat( "Render Task Texture Pool: size=%i used=%i",
		pTexturePoolCount, CountUsedTexturePool() );
	rtlogger.LogInfoFormat( "Render Task VAO Pool: size=%i used=%i",
		pVAOPoolCount, CountUsedVAOPool() );
	rtlogger.LogInfoFormat( "Render Task Instance Pool: size=%i used=%i",
		pInstancePoolCount, CountUsedInstancePool() );
}



// Private Functions
//////////////////////

void deoglRenderTask::pCalcSPBInstancesMaxEntries( deoglRenderThread &renderThread ){
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

void deoglRenderTask::pAssignSPBInstances( deoglRenderThread &renderThread ){
	const int componentsPerIndex = pUseSPBInstanceFlags ? 2 : 1;
	deoglShaderParameterBlock *paramBlock = NULL;
	int paramBlockCount = 0;
	int firstIndex = 0;
	int i;
	
	for( i=0; i<pShaderCount; i++ ){
		const deoglRenderTaskShader &shader = *( ( deoglRenderTaskShader* )pListShaders.GetAt( i ) );
		deoglRenderTaskTexture *texture = shader.GetRootTexture();
		
		while( texture ){
			deoglRenderTaskVAO *vao = texture->GetRootVAO();
			
			while( vao ){
				deoglRenderTaskInstance *instance = vao->GetRootInstance();
				
				while( instance ){
					if( ! paramBlock || firstIndex + instance->GetSubInstanceCount() > pSPBInstanceMaxEntries ){
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
					
					instance->SetSIIndexInstanceParam( paramBlock, firstIndex );
					firstIndex += instance->GetSubInstanceCount();
					
					instance = instance->GetNextInstance();
				}
				
				vao = vao->GetNextVAO();
			}
			
			texture = texture->GetNextTexture();
		}
	}
	
	if( paramBlock ){
		paramBlock->SetElementCount( componentsPerIndex
			* decMath::max( ( ( firstIndex - 1 ) / 4 ) + 1, 1 ) );
	}
}

void deoglRenderTask::pUpdateSPBInstances(){
	deoglShaderParameterBlock *paramBlock = NULL;
	int i;
	
	try{
		for( i=0; i<pShaderCount; i++ ){
			const deoglRenderTaskShader &shader = *( ( deoglRenderTaskShader* )pListShaders.GetAt( i ) );
			deoglRenderTaskTexture *texture = shader.GetRootTexture();
			
			while( texture ){
				deoglRenderTaskVAO *vao = texture->GetRootVAO();
				
				while( vao ){
					deoglRenderTaskInstance *instance = vao->GetRootInstance();
					
					while( instance ){
						if( instance->GetSIIndexInstanceSPB() != paramBlock ){
							if( paramBlock ){
								paramBlock->UnmapBuffer();
								paramBlock = NULL;
							}
							
							instance->GetSIIndexInstanceSPB()->MapBuffer();
							paramBlock = instance->GetSIIndexInstanceSPB();
						}
						
						instance->WriteSIIndexInstanceInt( pUseSPBInstanceFlags );
						
						instance = instance->GetNextInstance();
					}
					
					vao = vao->GetNextVAO();
				}
				
				texture = texture->GetNextTexture();
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

void deoglRenderTask::pCreateSPBInstanceParamBlock( deoglRenderThread &renderThread ){
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
