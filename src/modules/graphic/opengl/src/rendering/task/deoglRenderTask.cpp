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
#include "shared/deoglRenderTaskSharedShader.h"
#include "shared/deoglRenderTaskSharedTexture.h"
#include "shared/deoglRenderTaskSharedVAO.h"
#include "shared/deoglRenderTaskSharedInstance.h"
#include "shared/deoglRenderTaskSharedPool.h"
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

// Constructor, destructor
////////////////////////////

deoglRenderTask::deoglRenderTask( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pRenderParamBlock( NULL ),
pTBOInstances( 0 ),
pSPBInstanceMaxEntries( 0 ),
pUseSPBInstanceFlags( false ),
pForceDoubleSided( false ),

pShaderCount( 0 ),
pHasShader( NULL ),
pHasShaderCount( 0 ),
pHasShaderSize( 0 ){
}

deoglRenderTask::~deoglRenderTask(){
	if( pHasShader ){
		delete [] pHasShader;
	}
	
	const int shaderCount = pShaders.GetCount();
	int i;
	for( i=0; i<shaderCount; i++ ){
		delete ( deoglRenderTaskShader* )pShaders.GetAt( i );
	}
	pShaders.RemoveAll();
}



// Management
///////////////

void deoglRenderTask::Clear(){
	pHasShaderCount = 0;
	pShaderCount = 0;
	
	SetRenderParamBlock( NULL );
	SetTBOInstances( 0 );
	pUseSPBInstanceFlags = false;
	pForceDoubleSided = false;
}

void deoglRenderTask::PrepareForRender(){
	if( pShaderCount == 0 ){
		return;
	}
	
	pCalcSPBInstancesMaxEntries();
	pAssignSPBInstances();
	pUpdateSPBInstances();
}

void deoglRenderTask::SortInstancesByDistance( deoglQuickSorter &sorter,
const decDVector &position, const decDVector &direction ){
	const double posDotDir = position * direction;
	int i, j, k;
	
	for( i=0; i<pShaderCount; i++ ){
		const deoglRenderTaskShader &shader = *( ( deoglRenderTaskShader* )pShaders.GetAt( i ) );
		const int textureCount = shader.GetTextureCount();
		
		for( j=0; j<textureCount; j++ ){
			const deoglRenderTaskTexture &texture = *shader.GetTextureAt( j );
			const int vaoCount = texture.GetVAOCount();
			
			for( k=0; k<vaoCount; k++ ){
				texture.GetVAOAt( k )->SortInstancesByDistance( sorter, position, direction, posDotDir );
			}
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

void deoglRenderTask::SetForceDoubleSided( bool forceDoubleSided ){
	pForceDoubleSided = forceDoubleSided;
}



deoglRenderTaskShader *deoglRenderTask::GetShaderAt( int index ) const{
	return ( deoglRenderTaskShader* )pShaders.GetAt( index );
}

deoglRenderTaskShader *deoglRenderTask::AddShader( const deoglRenderTaskSharedShader *shader ){
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
	}
	
	deoglRenderTaskShader *rtshader = pHasShader[ index ];
	if( rtshader ){
		return rtshader;
	}
	
	if( pShaderCount == pShaders.GetCount() ){
		rtshader = new deoglRenderTaskShader;
		pShaders.Add( rtshader );
		
	}else{
		rtshader = ( deoglRenderTaskShader* )pShaders.GetAt( pShaderCount );
		rtshader->Reset();
	}
	pShaderCount++;
	
	rtshader->SetShader( shader );
	pHasShader[ index ] = rtshader;
	return rtshader;
}



int deoglRenderTask::GetTotalPointCount() const{
	int i, totalPointCount = 0;
	for( i=0; i<pShaderCount; i++ ){
		totalPointCount += ( ( deoglRenderTaskShader* )pShaders.GetAt( i ) )->GetTotalPointCount();
	}
	return totalPointCount;
}

int deoglRenderTask::GetTotalTextureCount() const{
	int i, totalTextureCount = 0;
	for( i=0; i<pShaderCount; i++ ){
		totalTextureCount += ( ( deoglRenderTaskShader* )pShaders.GetAt( i ) )->GetTextureCount();
	}
	return totalTextureCount;
}

int deoglRenderTask::GetTotalVAOCount() const{
	int i, totalVAOCount = 0;
	for( i=0; i<pShaderCount; i++ ){
		totalVAOCount += ( ( deoglRenderTaskShader* )pShaders.GetAt( i ) )->GetTotalVAOCount();
	}
	return totalVAOCount;
}

int deoglRenderTask::GetTotalInstanceCount() const{
	int i, totalInstanceCount = 0;
	for( i=0; i<pShaderCount; i++ ){
		totalInstanceCount += ( ( deoglRenderTaskShader* )pShaders.GetAt( i ) )->GetTotalInstanceCount();
	}
	return totalInstanceCount;
}

int deoglRenderTask::GetTotalSubInstanceCount() const{
	int i, totalSubInstanceCount = 0;
	for( i=0; i<pShaderCount; i++ ){
		totalSubInstanceCount += ( ( deoglRenderTaskShader* )pShaders.GetAt( i ) )->GetTotalSubInstanceCount();
	}
	return totalSubInstanceCount;
}



// Debugging
//////////////

void deoglRenderTask::DebugPrint( deoglRTLogger &rtlogger ){
	const bool detailsInstances = true; //false;
	int s, t, v, i, u, d, sic;
	decString text;
	
	rtlogger.LogInfoFormat( "RenderTask %p: spb=%p shaders=%d points=%d textures=%d",
		this, pRenderParamBlock, pShaderCount, GetTotalPointCount(), GetTotalTextureCount() );
	
	for( s=0; s<pShaderCount; s++ ){
		const deoglRenderTaskShader &shader = *( ( deoglRenderTaskShader* )pShaders.GetAt( s ) );
		const deoglShaderDefines &defines = shader.GetShader()->GetShader()->GetDefines();
		
		rtlogger.LogInfoFormat( "- shader %i: shader=%p textures=%i points=%i vaos=%i "
			"instances=%i subInstances=%i", s, shader.GetShader(), shader.GetTextureCount(),
			shader.GetTotalPointCount(), shader.GetTotalVAOCount(),
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
		
		const int textureCount = shader.GetTextureCount();
		for( t=0; t<textureCount; t++ ){
			const deoglRenderTaskTexture &rttexture = *shader.GetTextureAt( t );
			rtlogger.LogInfoFormat( "  - texture %i: tuc=%p texture=%p vaos=%i "
				"points=%i instances=%i subInstances=%i", t, rttexture.GetTexture()->GetTUC(),
				rttexture.GetTexture(), rttexture.GetVAOCount(), rttexture.GetTotalPointCount(),
				rttexture.GetTotalInstanceCount(), rttexture.GetTotalSubInstanceCount() );
			
			const int unitCount = rttexture.GetTexture()->GetTUC()->GetUnitCount();
			text.Format( "    units(" );
			for( u=0; u<unitCount; u++ ){
				const deoglTexUnitConfig &unit = rttexture.GetTexture()->GetTUC()->GetUnitAt( u );
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
			
			const int vaoCount = rttexture.GetVAOCount();
			for( v=0; v<vaoCount; v++ ){
				const deoglRenderTaskVAO &rtvao = *rttexture.GetVAOAt( v );
				sic = rtvao.GetTotalSubInstanceCount();
				
				rtlogger.LogInfoFormat( "    - vao %i: vao=%i instances=%i points=%i "
					"subInstances=%i", v, rtvao.GetVAO()->GetVAO()->GetVAO(),
					rtvao.GetInstanceCount(), rtvao.GetTotalPointCount(), sic );
				
				if( detailsInstances ){
					const int instanceCount = rtvao.GetInstanceCount();
					for( i=0; i<instanceCount; i++ ){
						const deoglRenderTaskInstance &rtinstance = *rtvao.GetInstanceAt( i );
						const deoglRenderTaskSharedInstance &instance = *rtinstance.GetInstance();
						if( instance.GetSubInstanceSPB() ){
							rtlogger.LogInfoFormat( "        - instance %i: ds=%s fp=%i pc=%i "
								"fi=%i ic=%i sic=%i sispbi=%p sispbfi=%d", i,
								instance.GetDoubleSided() ? "t" : "n", instance.GetFirstPoint(),
								instance.GetPointCount(), instance.GetFirstIndex(),
								instance.GetIndexCount(), instance.GetSubInstanceCount(),
								rtinstance.GetSIIndexInstanceSPB(),
								rtinstance.GetSIIndexInstanceFirst() );
						}
					}
				}
			}
		}
	}
}



// Private Functions
//////////////////////

void deoglRenderTask::pCalcSPBInstancesMaxEntries(){
	// since std140 layout adds a lot of padding between array elements we use ivec4.
	// this groups indices in blocks of four so the final index is pSPB[i/4][i%4].
	//pSPBInstanceMaxEntries = ( renderThread.GetCapabilities().GetUBOMaxSize() / 16 ) * 4;
	pSPBInstanceMaxEntries = pRenderThread.GetBufferObject().GetInstanceArraySizeUBO();
	
	if( pUseSPBInstanceFlags ){
		// if instance flags are used the vector <instanceIndex, instanceFlags> is
		// used instead. this reduces the max entries count by factor 2 but allows
		// to transport a scratch flags value in a fast way
		pSPBInstanceMaxEntries /= 2;
	}
}

void deoglRenderTask::pAssignSPBInstances(){
	const int componentsPerIndex = pUseSPBInstanceFlags ? 2 : 1;
	deoglShaderParameterBlock *paramBlock = NULL;
	int paramBlockCount = 0;
	int firstIndex = 0;
	int i, j, k, l;
	
	for( i=0; i<pShaderCount; i++ ){
		const deoglRenderTaskShader &rtshader = *( ( deoglRenderTaskShader* )pShaders.GetAt( i ) );
		const int textureCount = rtshader.GetTextureCount();
		
		for( j=0; j<textureCount; j++ ){
			const deoglRenderTaskTexture &rttexture = *rtshader.GetTextureAt( j );
			const int vaoCount = rttexture.GetVAOCount();
			
			for( k=0; k<vaoCount; k++ ){
				const deoglRenderTaskVAO &rtvao = *rttexture.GetVAOAt( k );
				const int instanceCount = rtvao.GetInstanceCount();
				
				for( l=0; l<instanceCount; l++ ){
					deoglRenderTaskInstance &rtinstance = *rtvao.GetInstanceAt( l );
					
					if( ! paramBlock || firstIndex + rtinstance.GetSubInstanceCount() > pSPBInstanceMaxEntries ){
						if( paramBlock ){
							paramBlock->SetElementCount( componentsPerIndex
								* decMath::max( ( ( firstIndex - 1 ) / 4 ) + 1, 1 ) );
						}
						
						if( paramBlockCount == pSPBInstances.GetCount() ){
							pCreateSPBInstanceParamBlock();
						}
						paramBlock = pSPBInstances.GetAt( paramBlockCount++ );
						firstIndex = 0;
					}
					
					rtinstance.SetSIIndexInstanceParam( paramBlock, firstIndex );
					firstIndex += rtinstance.GetSubInstanceCount();
				}
			}
		}
	}
	
	if( paramBlock ){
		paramBlock->SetElementCount( componentsPerIndex
			* decMath::max( ( ( firstIndex - 1 ) / 4 ) + 1, 1 ) );
	}
}

void deoglRenderTask::pUpdateSPBInstances(){
	deoglShaderParameterBlock *paramBlock = NULL;
	int i, j, k, l;
	
	try{
		for( i=0; i<pShaderCount; i++ ){
			const deoglRenderTaskShader &shader = *( ( deoglRenderTaskShader* )pShaders.GetAt( i ) );
			const int textureCount = shader.GetTextureCount();
			
			for( j=0; j<textureCount; j++ ){
				const deoglRenderTaskTexture &texture = *shader.GetTextureAt( j );
				const int vaoCount = texture.GetVAOCount();
				
				for( k=0; k<vaoCount; k++ ){
					const deoglRenderTaskVAO &vao = *texture.GetVAOAt( k );
					const int instanceCount = vao.GetInstanceCount();
					
					for( l=0; l<instanceCount; l++ ){
						deoglRenderTaskInstance &instance = *vao.GetInstanceAt( l );
						
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

void deoglRenderTask::pCreateSPBInstanceParamBlock(){
	// since std140 layout adds a lot of padding between array elements we use ivec4.
	// this groups indices in blocks of four so the final index is pSPB[i/4][i%4]
	deoglSPBlockUBO * const ubo = new deoglSPBlockUBO( pRenderThread );
	
	try{
		ubo->SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
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
