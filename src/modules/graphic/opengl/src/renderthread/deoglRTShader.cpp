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

#include "deoglRTShader.h"
#include "deoglRenderThread.h"
#include "../capabilities/deoglCapabilities.h"
#include "../extensions/deoglExtensions.h"
#include "../light/shader/deoglLightShaderManager.h"
#include "../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/shader/deoglSkinShaderManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texsamplerconfig/deoglTexSamplerConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>



// Class deoglRTShader
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTShader::deoglRTShader( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pTexUnitsConfigList( nullptr ),
pShaderManager( nullptr ),
pSkinShaderManager( nullptr ),
pLightShaderManager( nullptr ),
pCurShaderProg( nullptr ),
pDirtySSBOSkinTextures( true )
{
	int i;
	for( i=0; i<ETSC_COUNT; i++ ){
		pTexSamplerConfigs[ i ] = nullptr;
	}
	
	try{
		pTexUnitsConfigList = new deoglTexUnitsConfigList( renderThread );
		pCreateTexSamplerConfigs();
		
		pShaderManager = new deoglShaderManager( renderThread );
		pShaderManager->LoadUnitSourceCodes();
		pShaderManager->LoadSources();
		pShaderManager->ValidateCaches();
		
		pSkinShaderManager = new deoglSkinShaderManager( renderThread );
		pLightShaderManager = new deoglLightShaderManager( renderThread );
		
		// NOTE we can not create here SSBOs or alike due to the initialization order
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRTShader::~deoglRTShader(){
	pCleanUp();
}



// Management
///////////////

deoglTexSamplerConfig *deoglRTShader::GetTexSamplerConfig( const deoglRTShader::eTextureSamplerConfigurations type ) const{
	if( type < 0 || type >= ETSC_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return pTexSamplerConfigs[ type ];
}

void deoglRTShader::ActivateShader( const deoglShaderProgram *shader ){
	if( shader == pCurShaderProg ){
		return;
	}
	
	if( shader ){
		deoglShaderCompiled * const compiled = shader->GetCompiled();
		DEASSERT_NOTNULL( compiled )
		compiled->Activate();
		
	}else{
		OGL_CHECK( pRenderThread, pglUseProgram( 0 ) );
	}
	
	pCurShaderProg = shader;
}

void deoglRTShader::SetCommonDefines( deoglShaderDefines &defines ) const{
	defines.SetDefine( "HIGH_PRECISION", true );
	defines.SetDefine( "HIGHP", "highp" ); // if not supported by GPU medp
	
	if( pglUniformBlockBinding ){
		defines.SetDefine( "UBO", true );
		
		if( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() ){
			defines.SetDefine( "UBO_IDMATACCBUG", true );
		}
		if( pRenderThread.GetCapabilities().GetUBODirectLinkDeadloop().Broken() ){
			defines.SetDefine( "BUG_UBO_DIRECT_LINK_DEAD_LOOP", true );
		}
	}
	
	if( pRenderThread.GetExtensions().SupportsGSInstancing() ){
		defines.SetDefine( "GS_INSTANCING", true );
	}
	
	// OpenGL extensions would define symbols for these extentions which would work in
	// shaders but our pre-processor does not know about them. so add them manually.
	// this is mainly required for broken intel and nvidia drivers
	if( pRenderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_ARB_shader_viewport_layer_array ) ){
		defines.SetDefine( "EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY", true );
		
	}else if( pRenderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_AMD_vertex_shader_layer ) ){
		defines.SetDefine( "EXT_AMD_VERTEX_SHADER_LAYER", true );
	}
	
	if( pRenderThread.GetExtensions().GetHasExtension( deoglExtensions::ext_ARB_shader_draw_parameters ) ){
		defines.SetDefine( "EXT_ARB_SHADER_DRAW_PARAMETERS", true );
	}
}



void deoglRTShader::InvalidateSSBOSkinTextures(){
	pDirtySSBOSkinTextures = true;
}

void deoglRTShader::UpdateSSBOSkinTextures(){
	if( ! pDirtySSBOSkinTextures ){
		return;
	}
	
	pDirtySSBOSkinTextures = false;
	
	if( ! pSSBOSkinTextures ){
		pSSBOSkinTextures.TakeOver( new deoglSPBlockSSBO( pRenderThread, deoglSPBlockSSBO::etStream ) );
		pSSBOSkinTextures->SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		pSSBOSkinTextures->SetParameterCount( 1 );
		pSSBOSkinTextures->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 ); // uvec4
		pSSBOSkinTextures->MapToStd140();
		pSSBOSkinTextures->SetElementCount( 1 );
	}
	
	deoglRenderTaskSharedPool &pool = pRenderThread.GetRenderTaskSharedPool();
	const deMutexGuard guard( pool.GetMutexSkinTextures() );
	
	const int pipelinesPerTexture = deoglSkinTexturePipelinesList::PipelineTypesCount
		* deoglSkinTexturePipelines::TypeCount * deoglSkinTexturePipelines::ModifiersPerType;
	const int count = pool.GetSkinTextureCount();
	const int pipelineCount = count * pipelinesPerTexture;
	const int pipelinesPerElement = 8;
	const int elementCount = ( ( pipelineCount - 1 ) / pipelinesPerElement ) + 1;
	deoglSPBlockSSBO &ssbo = pSSBOSkinTextures;
	int i, j, k, l;
	
	if( elementCount > ssbo.GetElementCount() ){
		ssbo.SetElementCount( elementCount );
	}
	
	const deoglSPBMapBuffer mapped( ssbo );
	uint16_t *values = ( uint16_t* )ssbo.GetMappedBuffer();
	
	for( i=0; i<count; i++ ){
		const deoglSkinTexture * const texture = pool.GetSkinTextureAt( i );
		if( ! texture ){
			for( j=0; j<pipelinesPerTexture; j++ ){
				*( values++ ) = 0;
			}
			continue;
		}
		
		const deoglSkinTexturePipelinesList &stpsl = texture->GetPipelines();
		for( j=0; j<deoglSkinTexturePipelinesList::PipelineTypesCount; j++ ){
			const deoglSkinTexturePipelines &stps = stpsl.GetAt( ( deoglSkinTexturePipelinesList::ePipelineTypes )j );
			
			for( k=0; k<deoglSkinTexturePipelines::TypeCount; k++ ){
				for( l=0; l<deoglSkinTexturePipelines::ModifiersPerType; l++ ){
					const deoglSkinTexturePipeline * const stp = stps.GetWith( ( deoglSkinTexturePipelines::eTypes )k, l );
					
					if( stp ){
						*( values++ ) = ( uint16_t )decMath::max( stp->GetPipeline()->GetRTSIndex(), 0 );
						
					}else{
						*( values++ ) = 0;
					}
				}
			}
		}
	}
}



// Private Functions
//////////////////////

void deoglRTShader::pCleanUp(){
	int i;
	
	if( pLightShaderManager ){
		delete pLightShaderManager;
	}
	if( pSkinShaderManager ){
		delete pSkinShaderManager;
	}
	if( pShaderManager ){
		delete pShaderManager;
	}
	
	for( i=0; i<ETSC_COUNT; i++ ){
		if( pTexSamplerConfigs[ i ] ){
			delete pTexSamplerConfigs[ i ];
		}
	}
	
	if( pTexUnitsConfigList ){
		delete pTexUnitsConfigList;
	}
}



void deoglRTShader::pCreateTexSamplerConfigs(){
	pTexSamplerConfigs[ etscClampNearest ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscClampNearest ]->SetFilterMode( deoglTextureStageManager::etfNearest );
	pTexSamplerConfigs[ etscClampNearest ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscClampNearest ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscClampLinear ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscClampLinear ]->SetFilterMode( deoglTextureStageManager::etfLinear );
	pTexSamplerConfigs[ etscClampLinear ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscClampLinear ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscClampNearestMipMap ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscClampNearestMipMap ]->SetFilterMode( deoglTextureStageManager::etfNearestMipMap );
	pTexSamplerConfigs[ etscClampNearestMipMap ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscClampNearestMipMap ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscClampLinearMipMap ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscClampLinearMipMap ]->SetFilterMode( deoglTextureStageManager::etfLinearMipMap );
	pTexSamplerConfigs[ etscClampLinearMipMap ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscClampLinearMipMap ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscClampLinearMipMapNearest ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscClampLinearMipMapNearest ]->SetFilterMode( deoglTextureStageManager::etfLinearMipMapNearest );
	pTexSamplerConfigs[ etscClampLinearMipMapNearest ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscClampLinearMipMapNearest ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscRepeatNearest ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscRepeatNearest ]->SetFilterMode( deoglTextureStageManager::etfNearest );
	pTexSamplerConfigs[ etscRepeatNearest ]->SetWrapMode( GL_REPEAT );
	pTexSamplerConfigs[ etscRepeatNearest ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscRepeatLinear ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscRepeatLinear ]->SetFilterMode( deoglTextureStageManager::etfLinear );
	pTexSamplerConfigs[ etscRepeatLinear ]->SetWrapMode( GL_REPEAT );
	pTexSamplerConfigs[ etscRepeatLinear ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscRepeatLinearMipMap ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscRepeatLinearMipMap ]->SetFilterMode( deoglTextureStageManager::etfLinearMipMap );
	pTexSamplerConfigs[ etscRepeatLinearMipMap ]->SetWrapMode( GL_REPEAT );
	pTexSamplerConfigs[ etscRepeatLinearMipMap ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscShadowClampNearest ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscShadowClampNearest ]->SetFilterMode( deoglTextureStageManager::etfNearest );
	pTexSamplerConfigs[ etscShadowClampNearest ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscShadowClampNearest ]->SetDepthCompareMode( true );
	
	pTexSamplerConfigs[ etscShadowClampLinear ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscShadowClampLinear ]->SetFilterMode( deoglTextureStageManager::etfLinear );
	pTexSamplerConfigs[ etscShadowClampLinear ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscShadowClampLinear ]->SetDepthCompareMode( true );
	
	pTexSamplerConfigs[ etscShadowClampLinearInverse ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscShadowClampLinearInverse ]->SetFilterMode( deoglTextureStageManager::etfLinear );
	pTexSamplerConfigs[ etscShadowClampLinearInverse ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscShadowClampLinearInverse ]->SetDepthCompareMode( true );
	pTexSamplerConfigs[ etscShadowClampLinearInverse ]->SetDepthCompareFunc( GL_GEQUAL );
	
	pTexSamplerConfigs[ etscShadowRepeatNearest ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscShadowRepeatNearest ]->SetFilterMode( deoglTextureStageManager::etfNearest );
	pTexSamplerConfigs[ etscShadowRepeatNearest ]->SetWrapMode( GL_REPEAT );
	pTexSamplerConfigs[ etscShadowRepeatNearest ]->SetDepthCompareMode( true );
	
	pTexSamplerConfigs[ etscShadowRepeatLinear ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscShadowRepeatLinear ]->SetFilterMode( deoglTextureStageManager::etfLinear );
	pTexSamplerConfigs[ etscShadowRepeatLinear ]->SetWrapMode( GL_REPEAT );
	pTexSamplerConfigs[ etscShadowRepeatLinear ]->SetDepthCompareMode( true );
}
