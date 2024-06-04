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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRenderGeometry.h"
#include "deoglRenderReflection.h"
#include "deoglRenderWorld.h"
#include "debug/deoglRenderDebug.h"
#include "defren/deoglDeferredRendering.h"
#include "defren/deoglDRDepthMinMax.h"
#include "plan/deoglRenderPlanDebug.h"
#include "plan/deoglRenderPlanEnvMap.h"
#include "plan/deoglRenderPlan.h"
#include "light/deoglRenderGI.h"
#include "light/deoglRenderLight.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglRenderTask.h"
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../envmap/deoglEnvMapSlot.h"
#include "../envmap/deoglEnvMapSlotManager.h"
#include "../extensions/deoglExtensions.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../framebuffer/deoglRestoreFramebuffer.h"
#include "../gi/deoglGICascade.h"
#include "../gi/deoglGIState.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTChoices.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/paramblock/deoglSPBParameter.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../shapes/deoglShapeBox.h"
#include "../shapes/deoglShape.h"
#include "../shapes/deoglShapeManager.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../sky/deoglRSkyInstance.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../vao/deoglVAO.h"
#include "../world/deoglRWorld.h"
#include "../utils/collision/deoglCollisionFrustum.h"

#include <dragengine/common/exceptions.h>



//#define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer timerTotal;
static decTimer timer;

#define DEBUG_RESET_TIMERS				if(plan.GetDebugTiming()){ timer.Reset(); timerTotal.Reset(); }
#define DEBUG_PRINT_TIMER(what)			if(plan.GetDebugTiming()){ glFinish(); renderThread.GetLogger().LogInfoFormat( "Reflection %s = %iys", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) ); timer.Reset(); }
#define DEBUG_PRINT_TIMER_TOTAL(what)	if(plan.GetDebugTiming()){ glFinish(); renderThread.GetLogger().LogInfoFormat( "Reflection %s = %iys", what, ( int )( timerTotal.GetElapsedTime() * 1000000.0f ) ); }
#else
#define DEBUG_RESET_TIMERS
#define DEBUG_PRINT_TIMER(what)
#define DEBUG_PRINT_TIMER_TOTAL(what)
#endif



// Definitions
////////////////

enum eSPCopyColor{
	spccQuadParams,
	spccMipMapLevel,
	spccTCClamp
};

enum eSPMinMapMipMap{
	spmmmmTCClamp,
	spmmmmMipMapLevel,
	spmmmmSplitPos
};

enum eSPBApplyReflections{
	spbarEnvMapMatrixInfluence,
	spbarEnvMapMatrixReflBox,
	spbarEnvMapInfluenceBorder,
	spbarEnvMapInfluenceScale,
	spbarEnvMapPosition,
	spbarEnvMapRoughness,
	spbarEnvMapPriority,
	spbarEnvMapCount
};

enum pSPBReflection{
	spbr2BlendFactors,
	spbr2EnvMapLodLevel,
	spbr2LayerCount,
	spbr2EnvMapPosLayer
};

enum eSPCubeMap2EquiMap{
	spcm2emLevel
};

enum eSPBuildEnvMap{
	spbemQuadParams,
	spbemBlendWeights,
	spbemMipMapLevel
};

enum eSPEnvMapMask{
	speemMatrixMV,
	speemMatrixP,
	speemMatrixReflectionBox,
	speemMatrixMaskBox
};

enum eSPCopyMaterial{
	spcmPosTransform,
	spcmPosTransform2,
	spcmSCTransform,
	spcmTCTransform,
	spcmMatrixPosition,
	spcmMatrixNormal
};



// Class deoglRenderReflection
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderReflection::deoglRenderReflection( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread )
{
	const bool indirectMatrixAccessBug = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken();
	const bool renderFSQuadStereoVSLayer = renderThread.GetChoices().GetRenderFSQuadStereoVSLayer();
	const bool useInverseDepth = renderThread.GetChoices().GetUseInverseDepth();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const deoglExtensions &extensions = renderThread.GetExtensions();
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	const deoglShaderSources *sources;
	
	pRenderTask = NULL;
	pAddToRenderTask = NULL;
	
	pUseEquiEnvMap = true;
	pEnvMap = NULL;
	pEnvMapEqui = NULL;
	
	pDirectEnvMapActive  = NULL;

	pDirectEnvMapFading = NULL;
	
	pUseEquiEnvMap = config.GetEnvMapUseEqui() || ! extensions.GetHasArrayCubeMap();
	//printf( "EQUI: %i %i %i\n", pUseEquiEnvMap?1:0, config.GetEnvMapUseEqui()?1:0, extensions.GetHasArrayCubeMap()?1:0 );
	
	try{
		renderThread.GetShader().SetCommonDefines( commonDefines );
		
		
		// copy color
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, false, false );
		pipconf.SetEnableScissorTest( true );
		
		defines = commonDefines;
		defines.SetDefines( "INPUT_ARRAY_TEXTURE" );
		sources = shaderManager.GetSourcesNamed( "DefRen Copy Color" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineCopyColor = pipelineManager.GetWith( pipconf );
		
		defines.SetDefines( "MIPMAP" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineCopyColorMipMap = pipelineManager.GetWith( pipconf );
		
		defines = commonDefines;
		defines.SetDefines( "INPUT_ARRAY_TEXTURE" );
		defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen Copy Color Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineCopyColorStereo = pipelineManager.GetWith( pipconf );
		
		defines.SetDefines( "MIPMAP" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineCopyColorMipMapStereo = pipelineManager.GetWith( pipconf );
		
		
		// min max
		pipconf.Reset();
		pipconf.SetEnableScissorTest( true );
		
		if( deoglDRDepthMinMax::USAGE_VERSION == 0 ){
			pipconf.SetMasks( true, true, false, false, false );
		
			defines = commonDefines;
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection MinMap MipMap 2" );
			defines.SetDefines( "NO_TEXCOORD", "INITIAL" );
			pipconf.SetShader( renderThread, sources, defines );
			pPipelineMinMaxMipMapInitial = pipelineManager.GetWith( pipconf );
			
			defines = commonDefines;
			defines.SetDefines( "NO_TEXCOORD", "DOWNSAMPLE" );
			pipconf.SetShader( renderThread, sources, defines );
			pPipelineMinMaxMipMapDownsample = pipelineManager.GetWith( pipconf );
			
		}else if( deoglDRDepthMinMax::USAGE_VERSION == 1 ){
			pipconf.SetMasks( true, true, true, false, true );
			pipconf.EnableDepthTestAlways();
			
			defines = commonDefines;
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection MinMap MipMap" );
			defines.SetDefines( "NO_TEXCOORD", "CLAMP_TC", "FUNC_MIN" );
			pipconf.SetShader( renderThread, sources, defines );
			pPipelineMinMaxMipMapMin = pipelineManager.GetWith( pipconf );
			
			defines = commonDefines;
			defines.SetDefines( "NO_TEXCOORD", "CLAMP_TC", "FUNC_MAX" );
			pipconf.SetShader( renderThread, sources, defines );
			pPipelineMinMaxMipMapMax = pipelineManager.GetWith( pipconf );
			
		}else if( deoglDRDepthMinMax::USAGE_VERSION == 2 ){
			pipconf.SetMasks( true, true, true, false, true );
			pipconf.EnableDepthTestAlways();
			
			defines = commonDefines;
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection MinMap MipMap" );
			defines.SetDefines( "NO_TEXCOORD", "CLAMP_TC", "SPLIT_VERSION", "SPLIT_SHIFT_TC" );
			pipconf.SetShader( renderThread, sources, defines );
			pPipelineMinMaxMipMapInitial = pipelineManager.GetWith( pipconf );
			
			defines = commonDefines;
			defines.SetDefines( "NO_TEXCOORD", "SPLIT_VERSION" );
			pipconf.SetShader( renderThread, sources, defines );
			pPipelineMinMaxMipMapDownsample = pipelineManager.GetWith( pipconf );
		}
		
		
		
		// NOTE
		// Best solution is using screen-space stepping version (SSR_VERSION=1) but with z-position instead of depth.
		// The depth is too similar in pixels not very close to the camera resulting in stepping artifacts in the
		// narrow-phase. With using z-position these artifacts are not appearing (SSR_VERSION=2). view-space stepping
		// alone though has less quality than screen-space stepping and is less controllable.
		// 
		// For the time being view-space stepping is used until the position texture is back in use. Then the final
		// version will be used using screen-space stepping with z-position
		//
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, false, false );
		pipconf.SetEnableScissorTest( true );
		// pipconf.SetEnableStencilTest( true );
		// pipconf.SetStencil( GL_EQUAL, plan.GetStencilRefValue(), ~0, 0 );
		// pipconf.SetStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection ScreenSpace" );
		if( pUseEquiEnvMap ){
			defines.SetDefines( "ENVMAP_EQUI" );
		}
		if( useInverseDepth ){
			defines.SetDefines( "INVERSE_DEPTH" );
		}
		
		if( config.GetSSRMethod() == 0 ){ // 0 = groundTruth
			defines.SetDefine( "SSR_VERSION", 0 );
			
		}else if( config.GetSSRMethod() == 1 ){ // 1 = stepedSS
			defines.SetDefine( "SSR_VERSION", 1 );
			//defines.SetDefine( "RESULT_AFTER_FIRST_LOOP", true ); // this yields wrong results (moving reflections)
				defines.SetDefines( "NESTED_LOOP" ); // enabled slows down on Radeon 4870 but can't do better quality
				defines.SetDefines( "MULTI_STEPPING" );
				// integrated seems worse with SSR_VERSION=1 but required as otherwise NaN/Inf polutes the rendering
				//defines.SetDefine( "INTEGRATED_THRESHOLD_TEST", true );
			
			if( deoglDRDepthMinMax::USAGE_VERSION != -1 ){
				defines.SetDefines( "USE_DEPTH_MIPMAP" );
			}
		}
		
		//defines.SetDefine( "ROUGHNESS_TAPPING", true );
		
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineScreenSpace = pipelineManager.GetWith( pipconf );
		
		defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection ScreenSpace Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineScreenSpaceStereo = pipelineManager.GetWith( pipconf );
		
		
		// apply reflections
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, false, false );
		pipconf.SetEnableScissorTest( true );
		pipconf.EnableBlendAdd();
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection ApplyReflections" );
		if( pUseEquiEnvMap ){
			defines.SetDefines( "ENVMAP_EQUI" );
		}
		if( useInverseDepth ){
			defines.SetDefines( "INVERSE_DEPTH" );
		}
		//defines.SetDefine( "HACK_NO_SSR", true ); // set to 1 to examine env-map reflection only
		
		if( config.GetEnvMapMethod() == deoglConfiguration::eemmSingle ){
			defines.SetDefines( "ENVMAP_SINGLE" ); // ENVMAP_SINGLE, not ENVMAP_BOX_PROJECTION
			
		}else if( config.GetEnvMapMethod() == deoglConfiguration::eemmMultipleBoxProjection ){
			defines.SetDefines( "ENVMAP_BOX_PROJECTION" ); // ENVMAP_BOX_PROJECTION, not ENVMAP_SINGLE
			
		//}else{ // not ENVMAP_SINGLE, not ENVMAP_BOX_PROJECTION
		}
		
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineApplyReflections = pipelineManager.GetWith( pipconf );
		
		if( renderThread.GetChoices().GetRenderFSQuadStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection ApplyReflections Stereo" );
			defines.SetDefines( "GS_RENDER_STEREO" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineApplyReflectionsStereo = pipelineManager.GetWith( pipconf );
		
		
		// copy material
		// input is framebuffer normal format (usually float). output is hard-coded shifted-int
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, false, false );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen EnvMap Material Copy" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineCopyMaterial = pipelineManager.GetWith( pipconf );
		
		
		// env map copy
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, true, false );
		
		sources = shaderManager.GetSourcesNamed( "DefRen EnvMap Light GI" );
		if( pUseEquiEnvMap ){
			defines.SetDefines( "ENVMAP_EQUI" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineEnvMapCopy = pipelineManager.GetWith( pipconf );
		
		// env map light gi
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, true, false );
		
		defines.SetDefines( "WITH_GI" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineEnvMapLightGI = pipelineManager.GetWith( pipconf );
		
		
		// reflection
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, false, false );
		pipconf.SetEnableScissorTest( true );
		pipconf.EnableBlendAdd();
		// pipconf.SetEnableStencilTest( true );
		// pipconf.SetStencil( GL_EQUAL, plan.GetStencilRefValue(), ~0, 0 );
		// pipconf.SetStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
	
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection" );
		//if( pUseEquiEnvMap ){
			defines.SetDefines( "ENVMAP_EQUI" );
		//}
		if( useInverseDepth ){
			defines.SetDefines( "INVERSE_DEPTH" );
		}
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineReflection = pipelineManager.GetWith( pipconf );
		
		defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineReflectionStereo = pipelineManager.GetWith( pipconf );
		
		
		// cube map to equi rect map
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, true, false );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection CubeMap 2 EquiMap" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineCubeMap2EquiMap = pipelineManager.GetWith( pipconf );
		
		
		// build env map
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, true, false );
		
		defines = commonDefines;
		if( pUseEquiEnvMap ){
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection Build EnvMap Equi" );
			defines.SetDefines( "ENVMAP_EQUI" );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection Build EnvMap" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineBuildEnvMap = pipelineManager.GetWith( pipconf );
		
		
		// env map mask
		pipconf.Reset();
		pipconf.SetMasks( false, false, false, true, false );
		pipconf.EnableBlend( GL_SRC_COLOR, GL_ZERO );
		pipconf.EnableCulling( false );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection EnvMap Mask" );
		//defines.SetDefines( "FULLSCREENQUAD" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineEnvMapMask = pipelineManager.GetWith( pipconf );
		
		
		
		deoglSPBlockUBO::Ref ubo( deoglSPBlockUBO::Ref::New( new deoglSPBlockUBO( renderThread ) ) );
		ubo->SetRowMajor( ! indirectMatrixAccessBug );
		ubo->SetParameterCount( 4 );
		ubo->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2 pBlendFactors
		ubo->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float pEnvMapLodLevel
		ubo->GetParameterAt( 2 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int pLayerCount
		ubo->GetParameterAt( 3 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 100 ); // vec3 pEnvMapPosLayer[ 100 ]
		ubo->MapToStd140();
		ubo->SetBindingPoint( 1 );
		pRenderParamBlockSingleUse.TakeOver( new deoglSPBSingleUse( renderThread, ubo ) );
		
		pRenderTask = new deoglRenderTask( renderThread );
		pAddToRenderTask = new deoglAddToRenderTask( renderThread, *pRenderTask );
		
		if( config.GetEnvMapMethod() == deoglConfiguration::eemmSingle ){
			if( pUseEquiEnvMap ){
				pEnvMapEqui = new deoglTexture( renderThread );
				pEnvMapEqui->SetSize( config.GetEnvMapSize() * 4, config.GetEnvMapSize() * 2 );
				//pEnvMapEqui->SetFBOFormat( 3
				pEnvMapEqui->SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG11B10F );
				pEnvMapEqui->SetMipMapped( true );
				
			}else{
				pEnvMap = new deoglCubeMap( renderThread );
				pEnvMap->SetSize( config.GetEnvMapSize() );
				//pEnvMap->SetFBOFormat( 3
				pEnvMap->SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG11B10F );
				pEnvMap->SetMipMapped( true );
			}
		}
		
		
		ubo.TakeOver( new deoglSPBlockUBO( renderThread ) );
		ubo->SetRowMajor( ! indirectMatrixAccessBug );
		ubo->SetParameterCount( 8 );
		ubo->GetParameterAt( spbarEnvMapMatrixInfluence ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 8 );
		ubo->GetParameterAt( spbarEnvMapMatrixReflBox ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 8 );
		ubo->GetParameterAt( spbarEnvMapInfluenceBorder ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 8 );
		ubo->GetParameterAt( spbarEnvMapInfluenceScale ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 8 );
		ubo->GetParameterAt( spbarEnvMapPosition ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 8 );
		ubo->GetParameterAt( spbarEnvMapRoughness ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 8 );
		ubo->GetParameterAt( spbarEnvMapPriority ).SetAll( deoglSPBParameter::evtInt, 1, 1, 8 );
		ubo->GetParameterAt( spbarEnvMapCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
		ubo->MapToStd140();
		ubo->SetBindingPoint( 1 );
		pEnvMapsParamBlockSingleUse.TakeOver( new deoglSPBSingleUse( renderThread, ubo ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderReflection::~deoglRenderReflection(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderReflection::ConvertCubeMap2EquiMap( deoglCubeMap &cubemap, deoglTexture *equimap ){
	if( ! equimap ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Reflection.ConvertCubeMap2EquiMap" );
	deoglFramebuffer * const oldfbo = renderThread.GetFramebuffer().GetActive();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const int height = equimap->GetHeight();
	const int width = equimap->GetWidth();
	deoglFramebuffer *fbo = NULL;
	deoglShaderCompiled *shader;
	
	pPipelineCubeMap2EquiMap->Activate();
	
	shader = &pPipelineCubeMap2EquiMap->GetGlShader();
	
	tsmgr.EnableCubeMap( 0, cubemap, GetSamplerClampNearest() ); // GetSamplerClampLinear()
	
	try{
		fbo = renderThread.GetFramebuffer().GetManager().GetFBOWithResolution( width, height );
		renderThread.GetFramebuffer().Activate( fbo );
		
		fbo->DetachAllImages();
		fbo->AttachColorTexture( 0, equimap );
		fbo->Verify();
		
		const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		shader->SetParameterFloat( spcm2emLevel, 0.0f );
		
		RenderFullScreenQuadVAO();
		
		renderThread.GetFramebuffer().Activate( oldfbo );
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		
	}catch( const deException & ){
		renderThread.GetFramebuffer().Activate( oldfbo );
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		throw;
	}
	
	equimap->CreateMipMaps();
}

void deoglRenderReflection::RenderEnvMapMask( deoglRenderPlan &plan, deoglEnvironmentMap &envmap, int cubeMapFace ){
	const int maskShapeCount = envmap.GetReflectionMaskBoxMatrixCount();
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Reflection.RenderEnvMapMask" );
	
	pPipelineEnvMapMask->Activate();
	
	// clear the alpha value to 1
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	// the rest happens only if there are mask shapes
	if( maskShapeCount > 0 ){
		deoglShapeBox &renderShape = *( ( deoglShapeBox* )renderThread.GetBufferObject().GetShapeManager().GetShapeAt( deoglRTBufferObject::esBox ) );
		const int size = envmap.GetSize();
		deoglShaderCompiled *shader;
		decDMatrix matrixMaskBox;
		decDMatrix matrixCamera;
		int i;
		
		SetViewport( size, size );
		
		shader = &pPipelineEnvMapMask->GetGlShader();
		
		shader->SetParameterMatrix4x4( speemMatrixP, plan.GetProjectionMatrix() );
		
		deoglCubeMap::CreateMatrixForFace( matrixCamera, envmap.GetPosition(), cubeMapFace );
		matrixCamera *= decDMatrix::CreateScale( 1.0, -1.0, 1.0 );
		
		const decDMatrix matrixReflBox = ( envmap.GetMatrixReflectionBox() * matrixCamera ).Invert();
		shader->SetParameterDMatrix4x3( speemMatrixReflectionBox, matrixReflBox );
		
		// for each mask box
		renderShape.ActivateVAO();
		
		for( i=0; i<maskShapeCount; i++ ){
			matrixMaskBox = envmap.GetReflectionMaskBoxMatrixAt( i ) * matrixCamera;
			shader->SetParameterDMatrix4x3( speemMatrixMV, matrixMaskBox );
			shader->SetParameterDMatrix4x3( speemMatrixMaskBox, matrixMaskBox.Invert() );
			
			renderShape.RenderFaces();
		}
	}
}

void deoglRenderReflection::UpdateEnvMap( deoglRenderPlan &plan ){
	DEBUG_RESET_TIMERS;
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Reflection.UpdateEnvMap" );
	
	if( plan.GetDirectEnvMapFader().GetActiveEnvMap() ){
		pDirectEnvMapActive = plan.GetDirectEnvMapFader().GetActiveEnvMap();
		
	}else{
		pDirectEnvMapActive = NULL;
	}
	
	if( plan.GetDirectEnvMapFader().GetFadingEnvMap() ){
		pDirectEnvMapFading = plan.GetDirectEnvMapFader().GetFadingEnvMap();
		
	}else{
		pDirectEnvMapFading = NULL;
	}
	
	if( renderThread.GetConfiguration().GetEnvMapMethod() != deoglConfiguration::eemmSingle ){
		return;
	}
	
	if( pEnvMapEqui ){
		pEnvMapEqui->CreateTexture();
		
	}else{
		pEnvMap->CreateCubeMap();
	}
	
	const deoglTexture *blendEnvMapEqui[ 4 ] = { NULL, NULL, NULL, NULL };
	const deoglCubeMap *blendEnvMap[ 4 ] = { NULL, NULL, NULL, NULL };
	const decDVector &cameraPosition = plan.GetCameraPosition();
	deoglTexSamplerConfig *blendSampler[ 4 ];
	float blendWeights[ 4 ] = { 1.0f, 0.0f, 0.0f, 0.0f };
	deoglShaderCompiled *shader = NULL;
	decDVector envMapPosition[ 4 ];
	deoglEnvironmentMap *envmap;
	int i, count;
	
	for( i=0; i<4; i++ ){
		blendSampler[ i ] = &GetSamplerClampNearestMipMap();
	}
	
	count = 0;
	
	for( i=0; i<plan.GetEnvMapCount(); i++ ){
		if( i == 4 ){
			break;
		}
		
		envmap = plan.GetEnvMapAt( i ).GetEnvMap();
		if( ! envmap || ! envmap->GetReady() ){
			continue;
		}
		
		if( pEnvMapEqui ){
			if( envmap->GetEquiEnvMap() ){
				blendEnvMapEqui[ count ] = envmap->GetEquiEnvMap();
				
			}else{
				blendEnvMapEqui[ count ] = renderThread.GetDefaultTextures().GetEmissivity();
			}
			
			if( blendEnvMapEqui[ count ]->GetWidth() == pEnvMapEqui->GetWidth()
			&& blendEnvMapEqui[ count ]->GetHeight() == pEnvMapEqui->GetHeight() ){
				blendSampler[ count ] = &GetSamplerClampNearestMipMap();
				
			}else{
				blendSampler[ count ] = &GetSamplerClampLinearMipMapNearest();
			}
			
		}else{
			if( envmap->GetEnvironmentMap() ){
				blendEnvMap[ count ] = envmap->GetEnvironmentMap();
				
			}else{
				blendEnvMap[ count ] = renderThread.GetDefaultTextures().GetEnvMap();
			}
			
			if( blendEnvMap[ count ]->GetSize() != pEnvMap->GetSize() ){
				blendSampler[ count ] = &GetSamplerClampNearestMipMap();
				
			}else{
				blendSampler[ count ] = &GetSamplerClampLinearMipMapNearest();
			}
		}
		
		envMapPosition[ count ] = envmap->GetPosition();
		count++;
	}
	
	// calculate the weights using barycentric coordinates on a line, triangle or tetrahedra depending
	// the number of environment maps found. also fill up the environment maps depending on what we need
	const double borderSize = 1.0; // border size in meters
	
	if( count == 4 ){
		const decDVector v12( envMapPosition[ 1 ] - envMapPosition[ 0 ] );
		const decDVector v13( envMapPosition[ 2 ] - envMapPosition[ 0 ] );
		const decDVector v14( envMapPosition[ 3 ] - envMapPosition[ 0 ] );
		const decDVector v23( envMapPosition[ 2 ] - envMapPosition[ 1 ] );
		const decDVector v24( envMapPosition[ 3 ] - envMapPosition[ 1 ] );
		const decDVector v34( envMapPosition[ 3 ] - envMapPosition[ 2 ] );
		const double vl12 = v12.Length();
		const double vl13 = v13.Length();
		const double vl14 = v14.Length();
		const double vl23 = v23.Length();
		const double vl24 = v24.Length();
		const double vl34 = v34.Length();
		const double d12 = ( vl12 + borderSize ) * 0.5;
		const double d13 = ( vl13 + borderSize ) * 0.5;
		const double d14 = ( vl14 + borderSize ) * 0.5;
		const double d23 = ( vl23 + borderSize ) * 0.5;
		const double d24 = ( vl24 + borderSize ) * 0.5;
		const double d34 = ( vl34 + borderSize ) * 0.5;
		const double c12 = ( cameraPosition - envMapPosition[ 0 ] ) * ( v12 / vl12 );
		const double c13 = ( cameraPosition - envMapPosition[ 0 ] ) * ( v13 / vl13 );
		const double c14 = ( cameraPosition - envMapPosition[ 0 ] ) * ( v14 / vl14 );
		const double c23 = ( cameraPosition - envMapPosition[ 1 ] ) * ( v23 / vl23 );
		const double c24 = ( cameraPosition - envMapPosition[ 1 ] ) * ( v24 / vl24 );
		const double c34 = ( cameraPosition - envMapPosition[ 2 ] ) * ( v34 / vl34 );
		const float w12 = ( float )( ( d12 - c12 ) / borderSize );
		const float w13 = ( float )( ( d13 - c13 ) / borderSize );
		const float w14 = ( float )( ( d14 - c14 ) / borderSize );
		const float w21 = 1.0f - w12;
		const float w23 = ( float )( ( d23 - c23 ) / borderSize );
		const float w24 = ( float )( ( d24 - c24 ) / borderSize );
		const float w31 = 1.0f - w13;
		const float w32 = 1.0f - w23;
		const float w34 = ( float )( ( d34 - c34 ) / borderSize );
		
		blendWeights[ 0 ] = w12;
		if( w13 < blendWeights[ 0 ] ){
			blendWeights[ 0 ] = w13;
		}
		if( w14 < blendWeights[ 0 ] ){
			blendWeights[ 0 ] = w14;
		}
		if( blendWeights[ 0 ] < 0.0f ){
			blendWeights[ 0 ] = 0.0f;
		}
		if( blendWeights[ 0 ] > 1.0f ){
			blendWeights[ 0 ] = 1.0f;
		}
		
		blendWeights[ 1 ] = w21;
		if( w23 < blendWeights[ 1 ] ){
			blendWeights[ 1 ] = w23;
		}
		if( w24 < blendWeights[ 1 ] ){
			blendWeights[ 1 ] = w24;
		}
		if( blendWeights[ 1 ] < 0.0f ){
			blendWeights[ 1 ] = 0.0f;
		}
		if( blendWeights[ 1 ] > 1.0f ){
			blendWeights[ 1 ] = 1.0f;
		}
		
		blendWeights[ 2 ] = w31;
		if( w32 < blendWeights[ 2 ] ){
			blendWeights[ 2 ] = w32;
		}
		if( w34 < blendWeights[ 2 ] ){
			blendWeights[ 2 ] = w34;
		}
		if( blendWeights[ 2 ] < 0.0f ){
			blendWeights[ 2 ] = 0.0f;
		}
		if( blendWeights[ 2 ] > 1.0f ){
			blendWeights[ 2 ] = 1.0f;
		}
		
		const float blendWeightFactor = 1.0f / ( blendWeights[ 0 ] + blendWeights[ 1 ] + blendWeights[ 2 ] );
		
		blendWeights[ 0 ] *= blendWeightFactor;
		blendWeights[ 1 ] *= blendWeightFactor;
		blendWeights[ 2 ] *= blendWeightFactor;
		blendWeights[ 3 ] = 1.0f - blendWeights[ 0 ] - blendWeights[ 1 ] - blendWeights[ 2 ];
		
	}else if( count == 3 ){
		const decDVector v12( envMapPosition[ 1 ] - envMapPosition[ 0 ] );
		const decDVector v13( envMapPosition[ 2 ] - envMapPosition[ 0 ] );
		const decDVector v23( envMapPosition[ 2 ] - envMapPosition[ 1 ] );
		const double vl12 = v12.Length();
		const double vl13 = v13.Length();
		const double vl23 = v23.Length();
		const double d12 = ( vl12 + borderSize ) * 0.5;
		const double d13 = ( vl13 + borderSize ) * 0.5;
		const double d23 = ( vl23 + borderSize ) * 0.5;
		const double c12 = ( cameraPosition - envMapPosition[ 0 ] ) * ( v12 / vl12 );
		const double c13 = ( cameraPosition - envMapPosition[ 0 ] ) * ( v13 / vl13 );
		const double c23 = ( cameraPosition - envMapPosition[ 1 ] ) * ( v23 / vl23 );
		const float w12 = ( float )( ( d12 - c12 ) / borderSize );
		const float w13 = ( float )( ( d13 - c13 ) / borderSize );
		const float w21 = 1.0f - w12;
		const float w23 = ( float )( ( d23 - c23 ) / borderSize );
		
		blendWeights[ 0 ] = w12;
		if( w13 < blendWeights[ 0 ] ){
			blendWeights[ 0 ] = w13;
		}
		if( blendWeights[ 0 ] < 0.0f ){
			blendWeights[ 0 ] = 0.0f;
		}
		if( blendWeights[ 0 ] > 1.0f ){
			blendWeights[ 0 ] = 1.0f;
		}
		
		blendWeights[ 1 ] = w21;
		if( w23 < blendWeights[ 1 ] ){
			blendWeights[ 1 ] = w23;
		}
		if( blendWeights[ 1 ] < 0.0f ){
			blendWeights[ 1 ] = 0.0f;
		}
		if( blendWeights[ 1 ] > 1.0f ){
			blendWeights[ 1 ] = 1.0f;
		}
		
		const float blendWeightFactor = 1.0f / ( blendWeights[ 0 ] + blendWeights[ 1 ] );
		
		blendWeights[ 0 ] *= blendWeightFactor;
		blendWeights[ 1 ] *= blendWeightFactor;
		blendWeights[ 2 ] = 1.0f - blendWeights[ 0 ] - blendWeights[ 1 ];
		
	}else if( count == 2 ){
		const decDVector v12( envMapPosition[ 1 ] - envMapPosition[ 0 ] );
		const double vl12 = v12.Length();
		const double d12 = ( vl12 + borderSize ) * 0.5;
		const double c12 = ( cameraPosition - envMapPosition[ 0 ] ) * ( v12 / vl12 );
		const float w12 = ( float )( ( d12 - c12 ) / borderSize );
		
		blendWeights[ 0 ] = w12;
		if( blendWeights[ 0 ] < 0.0f ){
			blendWeights[ 0 ] = 0.0f;
		}
		if( blendWeights[ 0 ] > 1.0f ){
			blendWeights[ 0 ] = 1.0f;
		}
		
		blendWeights[ 1 ] = 1.0f - blendWeights[ 0 ];
		
	}else if( count == 1 ){
		// a single environment map found. use it without blending. a copy is faster
		
	}else{
		// no environment maps found. use the sky environment map instead if existing.
		// use it without blending. a copy is faster.
		deoglEnvironmentMap * const envmapSky = plan.GetWorld()->GetSkyEnvironmentMap();
		
		if( pEnvMapEqui ){
			if( envmapSky ){
				blendEnvMapEqui[ 0 ] = envmapSky->GetEquiEnvMap();
			}
			
			if( ! blendEnvMapEqui[ 0 ] ){
				blendEnvMapEqui[ 0 ] = renderThread.GetDefaultTextures().GetEmissivity();
			}
		
		}else{
			if( envmapSky ){
				blendEnvMap[ 0 ] = envmapSky->GetEnvironmentMap();
			}
			
			if( ! blendEnvMap[ 0 ] ){
				blendEnvMap[ 0 ] = renderThread.GetDefaultTextures().GetEnvMap();
			}
		}
	}
	
	// for the sake of simplicity we use the shader always even for simple cases. there is no real speed
	// gain in trying to use a copy especially since envmaps can have different dimensions
	pPipelineBuildEnvMap->Activate();
	
	shader = &pPipelineBuildEnvMap->GetGlShader();
	
	// for the time beeing we need all textures set
	for( i=1; i<4; i++ ){
		if( pEnvMapEqui ){
			if( ! blendEnvMapEqui[ i ] ){
				blendEnvMapEqui[ i ] = blendEnvMapEqui[ i - 1 ];
			}
			
		}else{
			if( ! blendEnvMap[ i ] ){
				blendEnvMap[ i ] = blendEnvMap[ i - 1 ];
			}
		}
	}
	
	DEBUG_PRINT_TIMER( "Reflection: Update Env Map: Prepare" );
	
	// build the environment map
	if( shader ){
		deoglFramebuffer * const oldfbo = renderThread.GetFramebuffer().GetActive();
		deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
		deoglFramebuffer *fbo = NULL;
		int height, width;
		
		if( pEnvMapEqui ){
			height = pEnvMapEqui->GetHeight();
			width = pEnvMapEqui->GetWidth();
			
		}else{
			height = pEnvMap->GetSize();
			width = pEnvMap->GetSize();
		}
		
		try{
			fbo = renderThread.GetFramebuffer().GetManager().GetFBOWithResolution( width, height );
			renderThread.GetFramebuffer().Activate( fbo );
			
			fbo->DetachAllImages();
			
			OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
			
			if( pEnvMapEqui ){
				const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
				OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
				
			}else{
				const GLenum buffers[ 6 ] = {
					GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
					GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
				OGL_CHECK( renderThread, pglDrawBuffers( 6, buffers ) );
			}
			
			if( pEnvMapEqui ){
				tsmgr.EnableTexture( 1, *blendEnvMapEqui[ 1 ], *blendSampler[ 1 ] );
				tsmgr.EnableTexture( 2, *blendEnvMapEqui[ 2 ], *blendSampler[ 2 ] );
				tsmgr.EnableTexture( 3, *blendEnvMapEqui[ 3 ], *blendSampler[ 3 ] );
				
			}else{
				tsmgr.EnableCubeMap( 1, *blendEnvMap[ 1 ], *blendSampler[ 1 ] );
				tsmgr.EnableCubeMap( 2, *blendEnvMap[ 2 ], *blendSampler[ 2 ] );
				tsmgr.EnableCubeMap( 3, *blendEnvMap[ 3 ], *blendSampler[ 3 ] );
			}
			
			if( pEnvMapEqui ){
				shader->SetParameterFloat( spbemQuadParams, 0.5f, 0.5f, 0.5f, 0.5f );
				
			}else{
				shader->SetParameterFloat( spbemQuadParams, 1.0f, 1.0f, 0.0f, 0.0f );
			}
			shader->SetParameterFloat( spbemBlendWeights, blendWeights[ 0 ],
				blendWeights[ 1 ], blendWeights[ 2 ], blendWeights[ 3 ] );
			
			if( pEnvMapEqui ){
				fbo->AttachColorTexture( 0, pEnvMapEqui );
				
			}else{
				for( i=0; i<6; i++ ){
					fbo->AttachColorCubeMapFace( i, pEnvMap, i );
				}
			}
			fbo->Verify();
			
			SetViewport( width, height );
			
			if( pEnvMapEqui ){
				tsmgr.EnableTexture( 0, *blendEnvMapEqui[ 0 ], *blendSampler[ 0 ] );
				
			}else{
				tsmgr.EnableCubeMap( 0, *blendEnvMap[ 0 ], *blendSampler[ 0 ] );
			}
			
			shader->SetParameterFloat( spbemMipMapLevel, 0.0f );
			
			DEBUG_PRINT_TIMER( "Reflection: Update Env Map: Prepare" );
			RenderFullScreenQuadVAO();
			DEBUG_PRINT_TIMER( "Reflection: Update Env Map: Render" );
			
			renderThread.GetFramebuffer().Activate( oldfbo );
			if( fbo ){
				fbo->DecreaseUsageCount();
			}
			
			if( pEnvMapEqui ){
				pEnvMapEqui->CreateMipMaps();
				
			}else{
				pEnvMap->CreateMipMaps();
			}
			
		}catch( const deException & ){
			renderThread.GetFramebuffer().Activate( oldfbo );
			if( fbo ){
				fbo->DecreaseUsageCount();
			}
			throw;
		}
		DEBUG_PRINT_TIMER( "Reflection: Update Env Map: Build EnvMap" );
		
	}else{
		if( pEnvMapEqui ){
			pEnvMapEqui->CopyFrom( *blendEnvMapEqui[ 0 ], blendEnvMapEqui[ 0 ]->GetMipMapped() );
			
		}else{
			DETHROW_INFO( deeInvalidAction, "CopyFrom not yet implemented on CubeMap" );
			//pEnvMap->CopyFrom( *blendEnvMap[ 0 ], blendEnvMap[ 0 ]->GetMipMapped() );
		}
		DEBUG_PRINT_TIMER( "Reflection: Update Env Map: Copy EnvMap" );
	}
	DEBUG_PRINT_TIMER_TOTAL( "Reflection: Update Env Map" );
}



void deoglRenderReflection::RenderReflections( deoglRenderPlan &plan ){
	if( deoglSkinShader::REFLECTION_TEST_MODE != deoglSkinShader::ertmOwnPassReflection ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Reflection.RenderReflections" );
	
	deoglEnvMapSlotManager &envMapSlotMgr = renderThread.GetEnvMapSlotManager();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTexture *envMapSky = NULL;
	
	// prepare
	DEBUG_RESET_TIMERS;
	UpdateEnvMapSlots( plan );
	DEBUG_PRINT_TIMER( "Reflection: Update Env Map Slots" );
	UpdateRenderParameterBlock( plan );
	DEBUG_PRINT_TIMER( "Reflection: Update Render Param Blocks" );
	
	// determine the sky environment map if existing
	if( plan.GetWorld()->GetSkyEnvironmentMap() ){
		envMapSky = plan.GetWorld()->GetSkyEnvironmentMap()->GetEquiEnvMap();
	}
	if( ! envMapSky ){
		envMapSky = renderThread.GetDefaultTextures().GetEmissivity();
	}
	
	( plan.GetRenderStereo() ? pPipelineReflectionStereo : pPipelineReflection )->Activate();
	
	SetViewport( plan );
	
	// activate fbo
//defren.ActivateFBOTemporary1( false );
	defren.ActivateFBOColor( false, false );
	DEBUG_PRINT_TIMER( "Reflection: Activate FBO" );
	
	// activate shader and set the parameters
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	pRenderParamBlock->Activate();
	DEBUG_PRINT_TIMER( "Reflection: Activate Shader" );
	
	// set textures
	if( renderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 4, *defren.GetTextureRoughness(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 5, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		
	}else{
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 4, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
	}
	
	tsmgr.EnableArrayTexture( 6, *envMapSlotMgr.GetArrayTexture(), GetSamplerRepeatLinearMipMap() );
	tsmgr.EnableTexture( 7, *envMapSky, GetSamplerRepeatLinearMipMap() );
	DEBUG_PRINT_TIMER( "Reflection: Enable Textures" );
	
	RenderFullScreenQuadVAO( plan );
	DEBUG_PRINT_TIMER( "Reflection: Render" );
}

void deoglRenderReflection::UpdateEnvMapSlots( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	
	const deoglEnvironmentMapList &list = plan.GetCollideList().GetEnvironmentMapList();
	deoglEnvMapSlotManager &envMapSlotMgr = renderThread.GetEnvMapSlotManager();
	const int envMapCount = list.GetCount();
	int envMapIndex;
	
	envMapSlotMgr.MarkSlotsUnused();
	for( envMapIndex=0; envMapIndex<envMapCount; envMapIndex++ ){
		envMapSlotMgr.AddEnvironmentMap( list.GetAt( envMapIndex ) );
	}
	envMapSlotMgr.UpdateUsedSlots();
}

void deoglRenderReflection::UpdateRenderParameterBlock( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglEnvMapSlotManager &envMapSlotMgr = renderThread.GetEnvMapSlotManager();
	const decDMatrix &matrixCamera = plan.GetCameraMatrix();
	const int count = decMath::min( envMapSlotMgr.GetUsedSlotCount(), 100 );
	int i;
	
	pRenderParamBlock = ( deoglSPBlockUBO* )pRenderParamBlockSingleUse->Next();
	const deoglSPBMapBuffer mapped( pRenderParamBlock );
	
	// we use a blend zone of width 1m
	pRenderParamBlock->SetParameterDataVec2( spbr2BlendFactors,
		1.0f / ( 2.0f * /*blendWidth[m]=*/1.0f ), 0.5f );
	
	// the env map lod level is used like this in the shader:
	//    envMapLodLevel = log2( 1.0 + pEnvMapLodLevel * roughness )
	// for cube maps pEnvMapLodLevel has to be the size of the cube map. for equi-maps
	// though this is different. there the height is across 360 degrees. We want a split
	// into 4 zones as the minimum. this means we want two lod levels less than what a
	// cube map of the same size wants (down to 4 pixels not 1). this can be achieved
	// by shifting first the height right by 2 which equals a division by 4
	pRenderParamBlock->SetParameterDataFloat( spbr2EnvMapLodLevel,
		( float )( envMapSlotMgr.GetHeight() >> 2 ) );
	
	// set the environment map data array
	pRenderParamBlock->SetParameterDataInt( spbr2LayerCount, count );
	
	for( i=0; i<count; i++ ){
		const int slotIndex = envMapSlotMgr.GetUsedSlotIndexAt( i );
		const deoglEnvironmentMap &envMap = *envMapSlotMgr.GetSlotAt( slotIndex ).GetEnvMap();
		
		pRenderParamBlock->SetParameterDataArrayVec4( spbr2EnvMapPosLayer, i,
			matrixCamera * envMap.GetPosition(), ( double )slotIndex );
	}
}



void deoglRenderReflection::RenderDepthMinMaxMipMap( deoglRenderPlan &plan ){
	if( deoglDRDepthMinMax::USAGE_VERSION == -1 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Reflection.RenderDepthMinMaxMipMap" );
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglDRDepthMinMax &depthMinMap = defren.GetDepthMinMax();
	deoglShaderCompiled *shader;
	int height, width, i;
	
	DEBUG_RESET_TIMERS;
	
	if( deoglDRDepthMinMax::USAGE_VERSION == 0 ){
		const int mipMapLevelCount = depthMinMap.GetTexture()->GetRealMipMapLevelCount();
		
		// create initial min-max texture from the current depth texture
		pPipelineMinMaxMipMapInitial->Activate();
		
		renderThread.GetFramebuffer().Activate( depthMinMap.GetFBOAt( 0 ) );
		
		height = depthMinMap.GetHeight();
		width = depthMinMap.GetWidth();
		
		SetViewport( width, height );
		
		shader = &pPipelineMinMaxMipMapInitial->GetGlShader();
		
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		
		shader->SetParameterInt( spmmmmTCClamp, defren.GetWidth() - 1, defren.GetHeight() - 1 );
		
		RenderFullScreenQuadVAO();
		DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Initial Pass" );
		
		// downsample up to the max level. the first level has been done already by the initial pass
		pPipelineMinMaxMipMapDownsample->Activate();
		
		shader = &pPipelineMinMaxMipMapDownsample->GetGlShader();
		
		tsmgr.EnableArrayTexture( 0, *depthMinMap.GetTexture(), GetSamplerClampNearest() );
		
		for( i=1; i<mipMapLevelCount; i++ ){
			renderThread.GetFramebuffer().Activate( depthMinMap.GetFBOAt( i ) );
			
			shader->SetParameterInt( spmmmmTCClamp, width - 1, height - 1 );
			shader->SetParameterInt( spmmmmMipMapLevel, i - 1 );
			
			width = decMath::max( width >> 1, 1 );
			height = decMath::max( height >> 1, 1 );
			SetViewport( width, height );
			
			RenderFullScreenQuadVAO();
			DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Downsample Pass" );
		}
		
		
		
	}else if( deoglDRDepthMinMax::USAGE_VERSION == 1 ){
		const int mipMapLevelCount = depthMinMap.GetMaxLevelCount();
		pPipelineMinMaxMipMapMin->Activate();
		
		// create minimum texture
		shader = &pPipelineMinMaxMipMapMin->GetGlShader();
		
		height = depthMinMap.GetHeight();
		width = depthMinMap.GetWidth();
		
		for( i=0; i<mipMapLevelCount; i++ ){
			renderThread.GetFramebuffer().Activate( depthMinMap.GetFBOMinAt( i ) );
			
			if( i == 0 ){
				shader->SetParameterInt( spmmmmTCClamp, defren.GetWidth() - 1, defren.GetHeight() - 1 );
				shader->SetParameterInt( spmmmmMipMapLevel, 0 );
				
				tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
				
			}else{
				shader->SetParameterInt( spmmmmTCClamp, width - 1, height - 1 );
				shader->SetParameterInt( spmmmmMipMapLevel, i - 1 );
				
				if( i == 1 ){
					tsmgr.EnableArrayTexture( 0, *depthMinMap.GetTextureMin(), GetSamplerClampNearest() );
				}
				
				width >>= 1;
				if( width < 1 ){
					width = 1;
				}
				height >>= 1;
				if( height < 1 ){
					height = 1;
				}
			}
			
			SetViewport( width, height );
			
			RenderFullScreenQuadVAO();
			DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Min Pass" );
		}
			
		// create maximum texture
		pPipelineMinMaxMipMapMax->Activate();
		
		shader = &pPipelineMinMaxMipMapMax->GetGlShader();
		
		height = depthMinMap.GetHeight();
		width = depthMinMap.GetWidth();
		
		for( i=0; i<mipMapLevelCount; i++ ){
			renderThread.GetFramebuffer().Activate( depthMinMap.GetFBOMaxAt( i ) );
			
			if( i == 0 ){
				shader->SetParameterInt( spmmmmTCClamp, defren.GetWidth() - 1, defren.GetHeight() - 1 );
				shader->SetParameterInt( spmmmmMipMapLevel, 0 );
				
				tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
				
			}else{
				shader->SetParameterInt( spmmmmTCClamp, width - 1, height - 1 );
				shader->SetParameterInt( spmmmmMipMapLevel, i - 1 );
				
				if( i == 1 ){
					tsmgr.EnableArrayTexture( 0, *depthMinMap.GetTextureMax(), GetSamplerClampNearest() );
				}
				
				width >>= 1;
				if( width < 1 ){
					width = 1;
				}
				height >>= 1;
				if( height < 1 ){
					height = 1;
				}
			}
			
			SetViewport( width, height );
			
			RenderFullScreenQuadVAO();
			DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Max Pass" );
		}
		
		
		
	}else if( deoglDRDepthMinMax::USAGE_VERSION == 2 ){
		const int mipMapLevelCount = depthMinMap.GetMaxLevelCount();
		
		pPipelineMinMaxMipMapInitial->Activate();
		
		// create min-max texture
		height = depthMinMap.GetHeight();
		width = depthMinMap.GetWidth();
		
		renderThread.GetFramebuffer().Activate( depthMinMap.GetFBOAt( 0 ) );
		SetViewport( width << 1, height );
		
		shader = &pPipelineMinMaxMipMapInitial->GetGlShader();
		
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		
		shader->SetParameterInt( spmmmmTCClamp, defren.GetWidth() - 1, defren.GetHeight() - 1 );
		shader->SetParameterInt( spmmmmMipMapLevel, 0 );
		shader->SetParameterInt( spmmmmSplitPos, width );
		
		RenderFullScreenQuadVAO();
		DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Initial Pass" );
		
		// downsample up to the max level. the first level has been done already by the initial pass
		pPipelineMinMaxMipMapDownsample->Activate();
		
		shader = &pPipelineMinMaxMipMapDownsample->GetGlShader();
		
		tsmgr.EnableArrayTexture( 0, *depthMinMap.GetTexture(), GetSamplerClampNearest() );
		
		for( i=1; i<mipMapLevelCount; i++ ){
			renderThread.GetFramebuffer().Activate( depthMinMap.GetFBOAt( i ) );
			
			shader->SetParameterInt( spmmmmTCClamp, width - 1, height - 1 );
			shader->SetParameterInt( spmmmmMipMapLevel, i - 1 );
			
			width >>= 1;
			if( width < 1 ){
				width = 1;
			}
			height >>= 1;
			if( height < 1 ){
				height = 1;
			}
			
			shader->SetParameterInt( spmmmmSplitPos, width );
			
			SetViewport( width << 1, height );
			
			RenderFullScreenQuadVAO();
			DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Downsample Pass" );
		}
	}
}

void deoglRenderReflection::CopyColorToTemporary1( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Reflection.CopyColorToTemporary1" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglShaderCompiled *shader;
	
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineCopyColorStereo : *pPipelineCopyColor;
	pipeline.Activate();
	
	defren.ActivateFBOTemporary1Level( 0 );
	
	GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	shader = &pipeline.GetGlShader();
	
	defren.SetShaderParamFSQuad( *shader, spccQuadParams );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	
	RenderFullScreenQuadVAO( plan );
	
	// downsample the mip-map chain. the hardware solution should not introduce problems since the screen space
	// reflections do not sample near the border and since it should simply cut off the superflous pixel if
	// one is present
//	defren.GetTextureTemporary1()->CreateMipMaps();
}

void deoglRenderReflection::CopyMaterial( deoglRenderPlan &plan, bool solid ){
	if( ! plan.GetFBOMaterial() || ! solid ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Reflection.CopyMaterial" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	pPipelineCopyMaterial->Activate();
	
	renderThread.GetFramebuffer().Activate( plan.GetFBOMaterial() );
	
	deoglShaderCompiled &shader = pPipelineCopyMaterial->GetGlShader();
	shader.SetParameterVector4( spcmPosTransform, plan.GetDepthToPosition() );
	shader.SetParameterVector2( spcmPosTransform2, plan.GetDepthToPosition2() );
	shader.SetParameterMatrix4x3( spcmMatrixPosition, plan.GetFBOMaterialMatrix() );
	shader.SetParameterMatrix3x3( spcmMatrixNormal,
		plan.GetInverseCameraMatrix().GetRotationMatrix().QuickInvert() );
	
	if( plan.GetUpsideDown() ){
		shader.SetParameterFloat( spcmSCTransform, 1.0f, 1.0f, 0.0f, 0.0f );
		defren.SetShaderParamFSQuad( shader, spcmTCTransform );
		
	}else{
		shader.SetParameterFloat( spcmSCTransform, 1.0f, -1.0f, 0.0f, 0.0f );
		defren.SetShaderParamFSQuadUpsideDown( shader, spcmTCTransform );
	}
	
	SetViewport( plan );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
	
	RenderFullScreenQuadVAO();
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
}

void deoglRenderReflection::RenderGIEnvMaps( deoglRenderPlan &plan ){
	if( plan.GetNoReflections() ){
		return;
	}
	
	const deoglGIState * const giState = plan.GetRenderGIState();
	if( ! giState ){
		return;
	}
	
	DEBUG_RESET_TIMERS;
	
	// determine envmap to update. updating envmap takes around 350-400ys including rendering
	// and mipmap generation. doing this 8 times per frame costs up to 3ms which is a lot.
	// to reduce this cost only one envmap is updated per frame update. to ensure all envmaps
	// are regularily updated and none missed a last update counter is used. each frame update
	// the counter is incremented by one. then the envmap with the largest value is picked.
	// upon rerendeing the envmap the counter is set to the maximum value to give it highest
	// priority. envmaps with maximum priority are always updated.
	deoglEnvironmentMap *lightEnvMap = NULL;
	const int count = plan.GetEnvMapCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglEnvironmentMap * const envmap = plan.GetEnvMapAt( i ).GetEnvMap();
		if( ! envmap || ! envmap->GetReady() || ! envmap->GetMaterialReady() || envmap->GetSkyOnly() ){
			continue;
		}
		
		envmap->IncLastGILightUpdate();
		if( envmap->IsLastGILightUpdateAtMax() ){
			continue; // will be always updated
		}
		
		if( ! lightEnvMap || envmap->GetLastGILightUpdate() > lightEnvMap->GetLastGILightUpdate() ){
			lightEnvMap = envmap;
		}
	}
	
	// update envmap
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Reflection.RenderGIEnvMaps" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglRenderGI &renderGI = renderThread.GetRenderers().GetLight().GetRenderGI();
	const deoglRestoreFramebuffer restoreFbo( renderThread );
	
	deoglFramebuffer &fbo = renderThread.GetFramebuffer().GetEnvMap();
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	
	pPipelineEnvMapLightGI->Activate();
	
	renderThread.GetFramebuffer().Activate( &fbo );
	fbo.DetachAllImages();
	OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	
	tsmgr.EnableArrayTexture( 4, giState->GetTextureProbeIrradiance(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 5, giState->GetTextureProbeDistance(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 6, giState->GetTextureProbeOffset(), GetSamplerClampLinear() );
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
// 	renderThread.GetLogger().LogInfoFormat( "RenderGIEnvMaps: %d envmaps", count );
	for( i=0; i<count; i++ ){
		deoglEnvironmentMap * const envmap = plan.GetEnvMapAt( i ).GetEnvMap();
		if( ! envmap || ! envmap->GetReady() || ! envmap->GetMaterialReady() || envmap->GetSkyOnly() ){
			continue;
		}
		
		if( ! envmap->IsLastGILightUpdateAtMax() && envmap != lightEnvMap ){
			continue;
		}
		
		if( pUseEquiEnvMap ){
			continue; // not supported
		}
		
// 		renderThread.GetLogger().LogInfoFormat( "- LightEnvMap: %p (%g,%g,%g)", envmap,
// 			envmap->GetPosition().x, envmap->GetPosition().y, envmap->GetPosition().z );
		
		fbo.DetachAllImages();
		fbo.AttachColorCubeMap( 0, envmap->GetEnvironmentMap() );
		fbo.Verify();
		
		OGL_CHECK( renderThread, glViewport( 0, 0, envmap->GetSize(), envmap->GetSize() ) );
		
		tsmgr.EnableArrayTexture( 0, *envmap->GetEnvironmentMapPosition(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 1, *envmap->GetEnvironmentMapDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 2, *envmap->GetEnvironmentMapNormal(), GetSamplerClampNearest() ); // int-shifted
		tsmgr.EnableArrayTexture( 3, *envmap->GetEnvironmentMapEmissive(), GetSamplerClampNearest() );
		
		renderGI.PrepareUBORenderLight( *giState, envmap->GetPosition() );
		renderGI.GetUBORenderLight().Activate();
		
		// WARNING always non-stereo!
		// WARNING do not move this outside of the loop or the GPU may freeze/crash!
		renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
		
		RenderFullScreenQuadVAO();
		
		envmap->GetEnvironmentMap()->CreateMipMaps();
		
		envmap->ResetLastGILightUpdate();
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	
	DEBUG_PRINT_TIMER( "Reflection RenderGIEnvMaps: Render" );
}

void deoglRenderReflection::CopyEnvMap( deoglArrayTexture &source, deoglCubeMap &target ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Reflection.CopyEnvMap" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const deoglRestoreFramebuffer restoreFbo( renderThread );
	
	deoglFramebuffer &fbo = renderThread.GetFramebuffer().GetEnvMap();
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	
	pPipelineEnvMapCopy->Activate();
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	renderThread.GetFramebuffer().Activate( &fbo );
	fbo.DetachAllImages();
	fbo.AttachColorCubeMap( 0, &target );
	OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	fbo.Verify();
	
	SetViewport( target.GetSize(), target.GetSize() );
	
	tsmgr.EnableArrayTexture( 3, source, GetSamplerClampNearest() ); // texEmissive
	
	RenderFullScreenQuadVAO();
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
}

void deoglRenderReflection::RenderScreenSpace( deoglRenderPlan &plan ){
	if( plan.GetNoReflections() ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	if( ! config.GetSSREnable() ){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace( renderThread, "Reflection.RenderScreenSpace" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	DEBUG_RESET_TIMERS;
	
	// copy color to temporary1 and create mip-map chain unless we are in transparency pass because
	// there this has been already created for us.
	if( plan.GetRenderPassNumber() == 1 ){
		CopyColorToTemporary1( plan );
		DEBUG_PRINT_TIMER( "Reflection ScreenSpace: Copy Color To Temporary1" );
	}
	
	// render screen space reflection
	( plan.GetRenderStereo() ? pPipelineScreenSpaceStereo : pPipelineScreenSpace )->Activate();
	
	//defren.GetDepthTexture1()->CreateMipMaps();
	
	defren.ActivateFBOTemporary2( false );
	
	SetViewport( plan );
	
	GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	if( renderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		if( deoglDRDepthMinMax::USAGE_VERSION != -1 ){
			tsmgr.EnableArrayTexture( 1, *defren.GetDepthMinMax().GetTexture(), GetSamplerClampNearestMipMap() );
		}
		tsmgr.EnableArrayTexture( 2, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 3, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 4, *defren.GetTextureRoughness(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 5, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		
	}else{
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		if( deoglDRDepthMinMax::USAGE_VERSION != -1 ){
			tsmgr.EnableArrayTexture( 1, *defren.GetDepthMinMax().GetTexture(), GetSamplerClampNearestMipMap() );
		}
		tsmgr.EnableArrayTexture( 2, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 3, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 4, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
	}
	
	RenderFullScreenQuadVAO( plan );
	DEBUG_PRINT_TIMER( "Reflection ScreenSpace: Render Screen Space Reflection" );
	
	//defren.GetTextureTemporary2()->CreateMipMaps();
	
	// this fails on nVidia if NV_copy_image is only supported since this is a copy from RGBA to RGB format and
	// only ARB_copy_image supports this not NV_copy_image
	//defren.GetTextureColor()->CopyFrom( *defren.GetTextureTemporary1(), false, defren.GetWidth(), defren.GetHeight(), 0, 0, 0, 0 );
	
	
	// prepare vbo for applying reflections
	int i, envMapCount = 0;
	const deoglTexture *envMapEquis[ 8 ] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	const deoglCubeMap *envMapCubes[ 8 ] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	const float envMapRoughnessBase = ( float )PI * 0.5f;
	
	pEnvMapsParamBlock = ( deoglSPBlockUBO* )pEnvMapsParamBlockSingleUse->Next();
	{
		const deoglSPBMapBuffer mapped( pEnvMapsParamBlock );
		for( i=0; envMapCount<8 && i<plan.GetEnvMapCount(); i++ ){
			deoglEnvironmentMap * const envmap = plan.GetEnvMapAt( i ).GetEnvMap();
			if( ! envmap || ! envmap->GetReady() ){
				continue;
			}
			
			decDMatrix matrixReflBox = decDMatrix::CreateSRT( decDVector( 1e6, 1e6, 1e6 ), decDVector(), decDVector() ).Invert();
			decDMatrix matrixInfluence = matrixReflBox;// decDMatrix::CreateScale( 0.0, 0.0, 0.0 ); //matrixReflBox;
			decDVector envMapPosition;
			decVector influenceBorder = decVector( 1e5f, 1e5f, 1e5f );
			decVector influenceScale = decVector( 1.0f, 1.0f, 1.0f );
			
			if( ! envmap->GetSkyOnly() ){
				envMapPosition = plan.GetCameraMatrix() * envmap->GetPosition();
			}
			
			if( envmap->GetHasInfluenceBox() ){
				matrixInfluence = ( envmap->GetMatrixInfluenceBox() * plan.GetCameraMatrix() ).Invert();
				influenceBorder = envmap->GetInfluenceBoxBorderFactor();
				influenceScale = envmap->GetInfluenceBoxScale();
			}
			
			if( envmap->GetHasReflectionBox() ){
				matrixReflBox = ( envmap->GetMatrixReflectionBox() * plan.GetCameraMatrix() ).Invert();
			}
			
			pEnvMapsParamBlock->SetParameterDataArrayMat4x3( spbarEnvMapMatrixInfluence,
				envMapCount, matrixInfluence );
			pEnvMapsParamBlock->SetParameterDataArrayMat4x3( spbarEnvMapMatrixReflBox,
				envMapCount, matrixReflBox );
			pEnvMapsParamBlock->SetParameterDataArrayVec3( spbarEnvMapInfluenceScale,
				envMapCount, influenceScale );
			pEnvMapsParamBlock->SetParameterDataArrayVec3( spbarEnvMapInfluenceBorder,
				envMapCount, influenceBorder );
			pEnvMapsParamBlock->SetParameterDataArrayVec3( spbarEnvMapPosition,
				envMapCount, envMapPosition );
			pEnvMapsParamBlock->SetParameterDataArrayFloat( spbarEnvMapRoughness,
				envMapCount, envMapRoughnessBase * ( float )envmap->GetSize() );
			pEnvMapsParamBlock->SetParameterDataArrayInt( spbarEnvMapPriority,
				envMapCount, envmap->GetInfluencePriority() );
			
			if( pUseEquiEnvMap ){
				envMapEquis[ envMapCount ] = envmap->GetEquiEnvMap();
				
			}else{
				envMapCubes[ envMapCount ] = envmap->GetEnvironmentMap();
			}
			
			envMapCount++;
		}
		
		pEnvMapsParamBlock->SetParameterDataInt( spbarEnvMapCount, envMapCount );
	}
	
	// apply reflections
	( plan.GetRenderStereo() ? pPipelineApplyReflectionsStereo : pPipelineApplyReflections )->Activate();
	
	defren.ActivateFBOColor( false, false );
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	pEnvMapsParamBlock->Activate();
	
	/*
	decDMatrix matrixReflectionBox, matrixReflectionBoxNormal;
	decDVector envMapPosition;
	int hasReflectionBox = 0;
	
	//printf( "reflectionBox=%i [%f,%f,%f,%f][%f,%f,%f,%f][%f,%f,%f,%f]\n", pHasReflectionBox?1:0, pMatrixReflectionBox.a11, pMatrixReflectionBox.a12, pMatrixReflectionBox.a13, pMatrixReflectionBox.a14, pMatrixReflectionBox.a21, pMatrixReflectionBox.a22, pMatrixReflectionBox.a23, pMatrixReflectionBox.a24, pMatrixReflectionBox.a31, pMatrixReflectionBox.a32, pMatrixReflectionBox.a33, pMatrixReflectionBox.a34 );
	if( pHasReflectionBox ){
		envMapPosition = plan.GetCameraMatrix() * pEnvMapPosition;
		matrixReflectionBox = ( pMatrixReflectionBox * plan.GetCameraMatrix() ).Invert();
		matrixReflectionBoxNormal = ( pMatrixReflectionBox * plan.GetCameraMatrix() ).GetRotationMatrix().Invert().Transpose();
		hasReflectionBox = 1;
	}
	
	shader->SetParameterDMatrix4x3( sparMatrixReflectionBox, matrixReflectionBox );
	shader->SetParameterDMatrix3x3( sparMatrixReflectionBoxNormal, matrixReflectionBoxNormal );
	shader->SetParameterDVector3( sparEnvMapPosition, envMapPosition );
	shader->SetParameterInt( sparHasReflectionBox, hasReflectionBox );
	*/
	
	if( renderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 1, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 2, *defren.GetTextureTemporary2(), GetSamplerClampLinear() );
		tsmgr.EnableArrayTexture( 3, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 4, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 5, *defren.GetTextureRoughness(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 6, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		
	}else{
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 1, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 2, *defren.GetTextureTemporary2(), GetSamplerClampLinear() );
		tsmgr.EnableArrayTexture( 3, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 4, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 6, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
	}
	
	tsmgr.EnableArrayTexture( 7, *defren.GetTextureTemporary1(), GetSamplerClampLinearMipMap() );
	
	if( config.GetEnvMapMethod() == deoglConfiguration::eemmSingle ){
		if( pEnvMapEqui ){
			tsmgr.EnableTexture( 8, *pEnvMapEqui, GetSamplerRepeatLinearMipMap() );
			
		}else{
			tsmgr.EnableCubeMap( 8, *pEnvMap, GetSamplerClampLinearMipMap() );
		}
		
	}else{
		for( i=0; i<envMapCount; i++ ){
			if( pUseEquiEnvMap ){
				tsmgr.EnableTexture( 8 + i, *envMapEquis[ i ], GetSamplerRepeatLinearMipMap() );
				
			}else{
				tsmgr.EnableCubeMap( 8 + i, *envMapCubes[ i ], GetSamplerClampLinearMipMap() );
			}
		}
	}
	
	RenderFullScreenQuadVAO( plan );
	DEBUG_PRINT_TIMER( "Reflection ScreenSpace: Apply Reflections" );
}



// Private Functions
//////////////////////

void deoglRenderReflection::pCleanUp(){
	if( pEnvMapEqui ){
		delete pEnvMapEqui;
	}
	if( pEnvMap ){
		delete pEnvMap;
	}
	
	if( pAddToRenderTask ){
		delete pAddToRenderTask;
	}
	if( pRenderTask ){
		delete pRenderTask;
	}
}
