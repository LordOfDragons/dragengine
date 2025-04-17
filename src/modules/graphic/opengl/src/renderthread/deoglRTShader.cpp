/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRTShader.h"
#include "deoglRTChoices.h"
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
	
	#ifdef OS_ANDROID_QUEST
	defines.SetDefines("QUEST_BUG_EMITVERTEX");
	#endif
	
	const int renderDocDebugFlags = pRenderThread.GetChoices().GetRenderDocDebugFlags();
	if(renderDocDebugFlags != 0){
		if(renderDocDebugFlags & deoglRTChoices::RenderDocDebugFlagGI){
			defines.SetDefine("RENDER_DOC_DEBUG_GI", true);
		}
		if(renderDocDebugFlags & deoglRTChoices::RenderDocDebugFlagOccTest){
			defines.SetDefine("RENDER_DOC_DEBUG_OCCTEST", true);
		}
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
