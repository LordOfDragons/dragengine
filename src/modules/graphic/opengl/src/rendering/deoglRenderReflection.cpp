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
#include "task/deoglRenderTaskShader.h"
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugSaveTexture.h"
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
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBParameter.h"
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

enum pSPScreenSpace{
	spssClipReflDirNearDist,
	spssStepCount,
	spssSubStepCount,
	spssMaxRayLength,
	spssMinMaxTCFactor,
	spssCoverageFactor1,
	spssCoverageFactor2,
	spssRoughnessMaxTaps,
	spssRoughnessTapCountScale
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
	const bool bugUBODirectLinkDeadloop = renderThread.GetCapabilities().GetUBODirectLinkDeadloop().Broken();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const deoglExtensions &extensions = renderThread.GetExtensions();
	deoglShaderDefines defines, commonDefines;
	deoglShaderSources *sources;
	
	pRenderParamBlock = NULL;
	pRenderTask = NULL;
	pAddToRenderTask = NULL;
	
	pUseEquiEnvMap = true;
	pEnvMap = NULL;
	pEnvMapEqui = NULL;
	pEnvMapsParamBlock = NULL;
	
	pDirectEnvMapActive  = NULL;
	pDirectEnvMapFading = NULL;
	
	pUseEquiEnvMap = config.GetEnvMapUseEqui() || ! extensions.GetHasArrayCubeMap();
	//printf( "EQUI: %i %i %i\n", pUseEquiEnvMap?1:0, config.GetEnvMapUseEqui()?1:0, extensions.GetHasArrayCubeMap()?1:0 );
	
	try{
		renderThread.GetShader().SetCommonDefines( commonDefines );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Copy Color" );
		defines.SetDefine( "INPUT_ARRAY_TEXTURE", true );
		pShaderCopyColor = shaderManager.GetProgramWith( sources, defines );
		
		defines.SetDefine( "MIPMAP", true );
		pShaderCopyColorMipMap = shaderManager.GetProgramWith( sources, defines );
		
		defines = commonDefines;
		defines.SetDefine( "INPUT_ARRAY_TEXTURE", true );
		if( renderThread.GetChoices().GetRenderStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Copy Color Stereo" );
			defines.SetDefine( "GS_RENDER_STEREO", true );
		}
		
		pShaderCopyColorStereo = shaderManager.GetProgramWith( sources, defines );
		
		defines.SetDefine( "MIPMAP", true );
		pShaderCopyColorMipMapStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		
		if( deoglDRDepthMinMax::USAGE_VERSION == 0 ){
			defines = commonDefines;
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection MinMap MipMap 2" );
			if( ! sources ){
				DETHROW( deeInvalidParam );
			}
			defines.SetDefine( "NO_TEXCOORD", true );
			defines.SetDefine( "INITIAL", true );
			pShaderMinMaxMipMapInitial = shaderManager.GetProgramWith( sources, defines );
			
			defines = commonDefines;
			defines.SetDefine( "NO_TEXCOORD", true );
			defines.SetDefine( "DOWNSAMPLE", true );
			pShaderMinMaxMipMapDownsample = shaderManager.GetProgramWith( sources, defines );
			
		}else if( deoglDRDepthMinMax::USAGE_VERSION == 1 ){
			defines = commonDefines;
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection MinMap MipMap" );
			if( ! sources ){
				DETHROW( deeInvalidParam );
			}
			defines.SetDefine( "NO_TEXCOORD", true );
			defines.SetDefine( "CLAMP_TC", true );
			defines.SetDefine( "FUNC_MIN", true );
			pShaderMinMaxMipMapMin = shaderManager.GetProgramWith( sources, defines );
			
			defines = commonDefines;
			defines.SetDefine( "NO_TEXCOORD", true );
			defines.SetDefine( "CLAMP_TC", true );
			defines.SetDefine( "FUNC_MAX", true );
			pShaderMinMaxMipMapMax = shaderManager.GetProgramWith( sources, defines );
			
		}else if( deoglDRDepthMinMax::USAGE_VERSION == 2 ){
			defines = commonDefines;
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection MinMap MipMap" );
			if( ! sources ){
				DETHROW( deeInvalidParam );
			}
			defines.SetDefine( "NO_TEXCOORD", true );
			defines.SetDefine( "CLAMP_TC", true );
			defines.SetDefine( "SPLIT_VERSION", true );
			defines.SetDefine( "SPLIT_SHIFT_TC", true );
			pShaderMinMaxMipMapInitial = shaderManager.GetProgramWith( sources, defines );
			
			defines = commonDefines;
			defines.SetDefine( "NO_TEXCOORD", true );
			defines.SetDefine( "SPLIT_VERSION", true );
			pShaderMinMaxMipMapDownsample = shaderManager.GetProgramWith( sources, defines );
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
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection ScreenSpace" );
		if( pUseEquiEnvMap ){
			defines.SetDefine( "ENVMAP_EQUI", true );
		}
		if( defren.GetUseInverseDepth() ){
			defines.SetDefine( "INVERSE_DEPTH", true );
		}
		
		if( config.GetSSRMethod() == 0 ){ // 0 = groundTruth
			defines.SetDefine( "SSR_VERSION", "0" );
			
		}else if( config.GetSSRMethod() == 1 ){ // 1 = stepedSS
			defines.SetDefine( "SSR_VERSION", true );
			//defines.SetDefine( "RESULT_AFTER_FIRST_LOOP", true ); // this yields wrong results (moving reflections)
				defines.SetDefine( "NESTED_LOOP", true ); // enabled slows down on Radeon 4870 but can't do better quality
				defines.SetDefine( "MULTI_STEPPING", true );
				// integrated seems worse with SSR_VERSION=1 but required as otherwise NaN/Inf polutes the rendering
				//defines.SetDefine( "INTEGRATED_THRESHOLD_TEST", true );
			
			if( deoglDRDepthMinMax::USAGE_VERSION != -1 ){
				defines.SetDefine( "USE_DEPTH_MIPMAP", true );
			}
		}
		
		//defines.SetDefine( "ROUGHNESS_TAPPING", true );
		
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		pShaderScreenSpace = shaderManager.GetProgramWith( sources, defines );
		
		if( renderThread.GetChoices().GetRenderStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection ScreenSpace Stereo" );
			defines.SetDefine( "GS_RENDER_STEREO", true );
		}
		pShaderScreenSpaceStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection ApplyReflections" );
		if( indirectMatrixAccessBug ){
			defines.SetDefine( "UBO_IDMATACCBUG", true );
		}
		if( bugUBODirectLinkDeadloop ){
			defines.SetDefine( "BUG_UBO_DIRECT_LINK_DEAD_LOOP", true );
		}
		if( pUseEquiEnvMap ){
			defines.SetDefine( "ENVMAP_EQUI", true );
		}
		if( defren.GetUseInverseDepth() ){
			defines.SetDefine( "INVERSE_DEPTH", true );
		}
		//defines.SetDefine( "HACK_NO_SSR", true ); // set to 1 to examine env-map reflection only
		
		if( config.GetEnvMapMethod() == deoglConfiguration::eemmSingle ){
			defines.SetDefine( "ENVMAP_SINGLE", true ); // ENVMAP_SINGLE, not ENVMAP_BOX_PROJECTION
			
		}else if( config.GetEnvMapMethod() == deoglConfiguration::eemmMultipleBoxProjection ){
			defines.SetDefine( "ENVMAP_BOX_PROJECTION", true ); // ENVMAP_BOX_PROJECTION, not ENVMAP_SINGLE
			
		//}else{ // not ENVMAP_SINGLE, not ENVMAP_BOX_PROJECTION
		}
		
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		pShaderApplyReflections = shaderManager.GetProgramWith( sources, defines );
		
		if( renderThread.GetChoices().GetRenderStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection ApplyReflections Stereo" );
			defines.SetDefine( "GS_RENDER_STEREO", true );
		}
		pShaderApplyReflectionsStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		
		// input is framebuffer normal format (usually float). output is hard-coded shifted-int
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen EnvMap Material Copy" );
		pShaderCopyMaterial = shaderManager.GetProgramWith( sources, defines );
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen EnvMap Light GI" );
		if( pUseEquiEnvMap ){
			defines.SetDefine( "ENVMAP_EQUI", true );
		}
		pShaderEnvMapCopy = shaderManager.GetProgramWith( sources, defines );
		
		defines.SetDefine( "WITH_GI", true );
		pShaderEnvMapLightGI = shaderManager.GetProgramWith( sources, defines );
		
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection" );
		if( indirectMatrixAccessBug ){
			defines.SetDefine( "UBO_IDMATACCBUG", true );
		}
		if( bugUBODirectLinkDeadloop ){
			defines.SetDefine( "BUG_UBO_DIRECT_LINK_DEAD_LOOP", true );
		}
		//if( pUseEquiEnvMap ){
			defines.SetDefine( "ENVMAP_EQUI", true );
		//}
		if( defren.GetUseInverseDepth() ){
			defines.SetDefine( "INVERSE_DEPTH", true );
		}
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		pShaderReflection = shaderManager.GetProgramWith( sources, defines );
		
		if( renderThread.GetChoices().GetRenderStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection Stereo" );
			defines.SetDefine( "GS_RENDER_STEREO", true );
		}
		pShaderReflectionStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection CubeMap 2 EquiMap" );
		pShaderCubeMap2EquiMap = shaderManager.GetProgramWith( sources, defines );
		
		
		
		defines = commonDefines;
		if( pUseEquiEnvMap ){
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection Build EnvMap Equi" );
			defines.SetDefine( "ENVMAP_EQUI", true );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection Build EnvMap" );
		}
		pShaderBuildEnvMap = shaderManager.GetProgramWith( sources, defines );
		
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection EnvMap Mask" );
		//defines.SetDefine( "FULLSCREENQUAD", true );
		pShaderEnvMapMask = shaderManager.GetProgramWith( sources, defines );
		
		
		
		pRenderParamBlock = new deoglSPBlockUBO( renderThread );
		pRenderParamBlock->SetRowMajor( ! indirectMatrixAccessBug );
		pRenderParamBlock->SetParameterCount( 4 );
		pRenderParamBlock->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2 pBlendFactors
		pRenderParamBlock->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float pEnvMapLodLevel
		pRenderParamBlock->GetParameterAt( 2 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int pLayerCount
		pRenderParamBlock->GetParameterAt( 3 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 100 ); // vec3 pEnvMapPosLayer[ 100 ]
		pRenderParamBlock->MapToStd140();
		pRenderParamBlock->SetBindingPoint( 1 );
		
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
		
		
		pEnvMapsParamBlock = new deoglSPBlockUBO( renderThread );
		pEnvMapsParamBlock->SetRowMajor( ! indirectMatrixAccessBug );
		pEnvMapsParamBlock->SetParameterCount( 8 );
		pEnvMapsParamBlock->GetParameterAt( spbarEnvMapMatrixInfluence ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 8 );
		pEnvMapsParamBlock->GetParameterAt( spbarEnvMapMatrixReflBox ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 8 );
		pEnvMapsParamBlock->GetParameterAt( spbarEnvMapInfluenceBorder ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 8 );
		pEnvMapsParamBlock->GetParameterAt( spbarEnvMapInfluenceScale ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 8 );
		pEnvMapsParamBlock->GetParameterAt( spbarEnvMapPosition ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 8 );
		pEnvMapsParamBlock->GetParameterAt( spbarEnvMapRoughness ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 8 );
		pEnvMapsParamBlock->GetParameterAt( spbarEnvMapPriority ).SetAll( deoglSPBParameter::evtInt, 1, 1, 8 );
		pEnvMapsParamBlock->GetParameterAt( spbarEnvMapCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
		pEnvMapsParamBlock->MapToStd140();
		pEnvMapsParamBlock->SetBindingPoint( 1 );
		
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
	deoglFramebuffer * const oldfbo = renderThread.GetFramebuffer().GetActive();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const int height = equimap->GetHeight();
	const int width = equimap->GetWidth();
	deoglFramebuffer *fbo = NULL;
	deoglShaderCompiled *shader;
	
	// set states
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
	renderThread.GetShader().ActivateShader( pShaderCubeMap2EquiMap );
	shader = pShaderCubeMap2EquiMap->GetCompiled();
	
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
		
		defren.RenderFSQuadVAO();
		
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
	
	// set state we need at least for clearing
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE ) );
	
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
		
		// set states
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
		OGL_CHECK( renderThread, glBlendFunc( GL_SRC_COLOR, GL_ZERO ) );
		OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
		SetCullMode( false );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, size, size ) );
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		renderThread.GetShader().ActivateShader( pShaderEnvMapMask );
		shader = pShaderEnvMapMask->GetCompiled();
		
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
	renderThread.GetShader().ActivateShader( pShaderBuildEnvMap );
	shader = pShaderBuildEnvMap->GetCompiled();
	
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
		deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
		deoglFramebuffer *fbo = NULL;
		int height, width;
		
		if( pEnvMapEqui ){
			height = pEnvMapEqui->GetHeight();
			width = pEnvMapEqui->GetWidth();
			
		}else{
			height = pEnvMap->GetSize();
			width = pEnvMap->GetSize();
		}
		
		// set states
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
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
			
			OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
			
			if( pEnvMapEqui ){
				tsmgr.EnableTexture( 0, *blendEnvMapEqui[ 0 ], *blendSampler[ 0 ] );
				
			}else{
				tsmgr.EnableCubeMap( 0, *blendEnvMap[ 0 ], *blendSampler[ 0 ] );
			}
			
			shader->SetParameterFloat( spbemMipMapLevel, 0.0f );
			
			DEBUG_PRINT_TIMER( "Reflection: Update Env Map: Prepare" );
			defren.RenderFSQuadVAO();
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
	if( deoglSkinShader::REFLECTION_TEST_MODE != 1 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 61 ){
		return;
	}
	
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
	
	// set states
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
//OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	//OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
	//OGL_CHECK( renderThread, glStencilMask( 0 ) );
	//OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
	//OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), ~0 ) );
OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	DEBUG_PRINT_TIMER( "Reflection: Set States" );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	DEBUG_PRINT_TIMER( "Reflection: Viewport/Scissor" );
	
	// activate fbo
//defren.ActivateFBOTemporary1( false );
	defren.ActivateFBOColor( false, false );
	DEBUG_PRINT_TIMER( "Reflection: Activate FBO" );
	
	// activate shader and set the parameters
	renderThread.GetShader().ActivateShader( plan.GetRenderStereo() ? pShaderReflectionStereo : pShaderReflection );
	
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
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 60 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture( *defren.GetTextureTemporary1(), "refl_reflection" );
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
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
	
	if( GetRenderThread().GetConfiguration().GetDebugSnapshot() == 60 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture( *envMapSlotMgr.GetArrayTexture(), "refl_envmapslotmgr" );
	}
}

void deoglRenderReflection::UpdateRenderParameterBlock( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglEnvMapSlotManager &envMapSlotMgr = renderThread.GetEnvMapSlotManager();
	const decDMatrix &matrixCamera = plan.GetCameraMatrix();
	const int count = decMath::min( envMapSlotMgr.GetUsedSlotCount(), 100 );
	int i;
	
	pRenderParamBlock->MapBuffer();
	
	try{
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
		
	}catch( const deException & ){
		pRenderParamBlock->UnmapBuffer();
		throw;
	}
	
	pRenderParamBlock->UnmapBuffer();
}



void deoglRenderReflection::RenderDepthMinMaxMipMap( deoglRenderPlan &plan ){
	if( deoglDRDepthMinMax::USAGE_VERSION == -1 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 61 ){
		return;
	}
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglDRDepthMinMax &depthMinMap = defren.GetDepthMinMax();
	deoglShaderCompiled *shader;
	int height, width, i;
	
	DEBUG_RESET_TIMERS;
	
	if( deoglDRDepthMinMax::USAGE_VERSION == 0 ){
		const int mipMapLevelCount = depthMinMap.GetTexture()->GetRealMipMapLevelCount();
		
		// create initial min-max texture from the current depth texture
		renderThread.GetFramebuffer().Activate( depthMinMap.GetFBOAt( 0 ) );
		
		/*if( renderThread.GetConfiguration().GetDebugSnapshot() == 62 ){
			renderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion(
				*defren.GetDepthTexture1(), 0, "depth_minmax_original", deoglDebugSaveTexture::ecDepthBuffer );
		}*/
		
		height = depthMinMap.GetHeight();
		width = depthMinMap.GetWidth();
		
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_FALSE, GL_FALSE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
		
		renderThread.GetShader().ActivateShader( pShaderMinMaxMipMapInitial );
		shader = pShaderMinMaxMipMapInitial->GetCompiled();
		
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		
		shader->SetParameterInt( spmmmmTCClamp, defren.GetWidth() - 1, defren.GetHeight() - 1 );
		
		defren.RenderFSQuadVAO();
		DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Initial Pass" );
		
		// downsample up to the max level. the first level has been done already by the initial pass
		renderThread.GetShader().ActivateShader( pShaderMinMaxMipMapDownsample );
		shader = pShaderMinMaxMipMapDownsample->GetCompiled();
		
		tsmgr.EnableArrayTexture( 0, *depthMinMap.GetTexture(), GetSamplerClampNearest() );
		
		for( i=1; i<mipMapLevelCount; i++ ){
			renderThread.GetFramebuffer().Activate( depthMinMap.GetFBOAt( i ) );
			
			shader->SetParameterInt( spmmmmTCClamp, width - 1, height - 1 );
			shader->SetParameterInt( spmmmmMipMapLevel, i - 1 );
			
			width = decMath::max( width >> 1, 1 );
			height = decMath::max( height >> 1, 1 );
			
			OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
			
			defren.RenderFSQuadVAO();
			DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Downsample Pass" );
		}
		
		if( renderThread.GetConfiguration().GetDebugSnapshot() == 62 ){
			decString text;
			
			height = depthMinMap.GetHeight();
			width = depthMinMap.GetWidth();
			
			text.Format( "depth_minmax_level0_%ix%i", width, height );
			renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureLevelConversion(
				*depthMinMap.GetTexture(), 0, text.GetString(), defren.GetUseInverseDepth() ?
					deoglDebugSaveTexture::ecDepthBufferInverse : deoglDebugSaveTexture::ecNoConversion );
			
			for( i=1; i<mipMapLevelCount; i++ ){
				width = decMath::max( width >> 1, 1 );
				height = decMath::max( height >> 1, 1 );
				
				text.Format( "depth_minmax_level%i_%ix%i", i, width, height );
				renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureLevelConversion(
					*depthMinMap.GetTexture(), i, text.GetString(), defren.GetUseInverseDepth() ?
						deoglDebugSaveTexture::ecDepthBufferInverse : deoglDebugSaveTexture::ecNoConversion	);
			}
		}
		
		
		
	}else if( deoglDRDepthMinMax::USAGE_VERSION == 1 ){
		const int mipMapLevelCount = depthMinMap.GetMaxLevelCount();
		
		OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		// create minimum texture
		renderThread.GetShader().ActivateShader( pShaderMinMaxMipMapMin );
		shader = pShaderMinMaxMipMapMin->GetCompiled();
		
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
			
			OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
			
			defren.RenderFSQuadVAO();
			DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Min Pass" );
			if( renderThread.GetConfiguration().GetDebugSnapshot() == 62 ){
				decString text;
				text.Format( "depth_minmax_min_level%i_%ix%i", i, width, height );
				renderThread.GetDebug().GetDebugSaveTexture().SaveDepthArrayTextureLevel(
					*depthMinMap.GetTextureMin(), i, text.GetString(), deoglDebugSaveTexture::edtDepth );
				if( i > 0 ){
					tsmgr.EnableArrayTexture( 0, *depthMinMap.GetTextureMin(), GetSamplerClampNearest() );
				}
			}
		}
			
		// create maximum texture
		renderThread.GetShader().ActivateShader( pShaderMinMaxMipMapMax );
		shader = pShaderMinMaxMipMapMax->GetCompiled();
		
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
			
			OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
			
			defren.RenderFSQuadVAO();
			DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Max Pass" );
			if( renderThread.GetConfiguration().GetDebugSnapshot() == 62 ){
				decString text;
				text.Format( "depth_minmax_max_level%i_%ix%i", i, width, height );
				renderThread.GetDebug().GetDebugSaveTexture().SaveDepthArrayTextureLevel(
					*depthMinMap.GetTextureMax(), i, text.GetString(), deoglDebugSaveTexture::edtDepth );
				if( i > 0 ){
					tsmgr.EnableArrayTexture( 0, *depthMinMap.GetTextureMax(), GetSamplerClampNearest() );
				}
			}
		}
		
		
		
	}else if( deoglDRDepthMinMax::USAGE_VERSION == 2 ){
		const int mipMapLevelCount = depthMinMap.GetMaxLevelCount();
		
		OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		// create min-max texture
		height = depthMinMap.GetHeight();
		width = depthMinMap.GetWidth();
		
		renderThread.GetFramebuffer().Activate( depthMinMap.GetFBOAt( 0 ) );
		OGL_CHECK( renderThread, glViewport( 0, 0, width << 1, height ) );
		
		renderThread.GetShader().ActivateShader( pShaderMinMaxMipMapInitial );
		shader = pShaderMinMaxMipMapInitial->GetCompiled();
		
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		
		shader->SetParameterInt( spmmmmTCClamp, defren.GetWidth() - 1, defren.GetHeight() - 1 );
		shader->SetParameterInt( spmmmmMipMapLevel, 0 );
		shader->SetParameterInt( spmmmmSplitPos, width );
		
		defren.RenderFSQuadVAO();
		DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Initial Pass" );
		if( renderThread.GetConfiguration().GetDebugSnapshot() == 62 ){
			decString text;
			text.Format( "depth_minmax_level0_%ix%i", width, height );
			renderThread.GetDebug().GetDebugSaveTexture().SaveDepthArrayTextureLevel(
				*depthMinMap.GetTexture(), 0, text.GetString(), deoglDebugSaveTexture::edtDepth );
		}
		
		// downsample up to the max level. the first level has been done already by the initial pass
		renderThread.GetShader().ActivateShader( pShaderMinMaxMipMapDownsample );
		shader = pShaderMinMaxMipMapDownsample->GetCompiled();
		
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
			
			OGL_CHECK( renderThread, glViewport( 0, 0, width << 1, height ) );
			
			defren.RenderFSQuadVAO();
			DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Downsample Pass" );
			if( renderThread.GetConfiguration().GetDebugSnapshot() == 62 ){
				decString text;
				text.Format( "depth_minmax_level%i_%ix%i", i, width, height );
				renderThread.GetDebug().GetDebugSaveTexture().SaveDepthArrayTextureLevel(
					*depthMinMap.GetTexture(), i, text.GetString(), deoglDebugSaveTexture::edtDepth );
				tsmgr.EnableArrayTexture( 0, *depthMinMap.GetTexture(), GetSamplerClampNearest() );
			}
		}
	}
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 62 ){
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
}

void deoglRenderReflection::CopyColorToTemporary1( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglShaderCompiled *shader;
	int height = defren.GetHeight();
	int width = defren.GetWidth();
	
	// copy base level
	defren.ActivateFBOTemporary1Level( 0 );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
	
	deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderCopyColorStereo : pShaderCopyColor;
	renderThread.GetShader().ActivateShader( program );
	shader = program->GetCompiled();
	
	defren.SetShaderParamFSQuad( *shader, spccQuadParams );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	
	RenderFullScreenQuadVAO( plan );
	
	// downsample the mip-map chain. the hardware solution should not introduce problems since the screen space
	// reflections do not sample near the border and since it should simply cut off the superflous pixel if
	// one is present
//	defren.GetTextureTemporary1()->CreateMipMaps();
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 64 ){
		decString text;
		int level = 0;
		
		width = defren.GetWidth();
		height = defren.GetHeight();
		level = 0;
		
		text.Format( "refl_ssr_copycolor_level0_%ix%i", width, height );
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureLevelConversion( *defren.GetTextureTemporary1(), 0,
			text.GetString(), deoglDebugSaveTexture::ecColorLinear2sRGB );
		
		while( width > 1 && height > 1 ){
			width >>= 1;
			if( width < 1 ){
				width = 1;
			}
			height >>= 1;
			if( height < 1 ){
				height = 1;
			}
			level++;
			
			text.Format( "refl_ssr_copycolor_level%i_%ix%i", level, width, height );
			renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureLevelConversion( *defren.GetTextureTemporary1(), level,
				text.GetString(), deoglDebugSaveTexture::ecColorLinear2sRGB );
		}
		
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
}

void deoglRenderReflection::CopyMaterial( deoglRenderPlan &plan, bool solid ){
	if( ! plan.GetFBOMaterial() || ! solid ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	renderThread.GetFramebuffer().Activate( plan.GetFBOMaterial() );
	
	renderThread.GetShader().ActivateShader( pShaderCopyMaterial );
	
	deoglShaderCompiled &shader = *pShaderCopyMaterial->GetCompiled();
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
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, plan.GetViewportWidth(), plan.GetViewportHeight() ) );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
	
	defren.RenderFSQuadVAO();
	
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
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglRenderGI &renderGI = renderThread.GetRenderers().GetLight().GetRenderGI();
	const deoglRestoreFramebuffer restoreFbo( renderThread );
	
	deoglFramebuffer &fbo = renderThread.GetFramebuffer().GetEnvMap();
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	
	renderThread.GetFramebuffer().Activate( &fbo );
	fbo.DetachAllImages();
	OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	
	renderThread.GetShader().ActivateShader( pShaderEnvMapLightGI );
	
	tsmgr.EnableArrayTexture( 4, giState->GetTextureProbeIrradiance(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 5, giState->GetTextureProbeDistance(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 6, giState->GetTextureProbeOffset(), GetSamplerClampLinear() );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
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
		
		defren.RenderFSQuadVAO();
		
		envmap->GetEnvironmentMap()->CreateMipMaps();
		
		envmap->ResetLastGILightUpdate();
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	
	DEBUG_PRINT_TIMER( "Reflection RenderGIEnvMaps: Render" );
}

void deoglRenderReflection::CopyEnvMap( deoglArrayTexture &source, deoglCubeMap &target ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const deoglRestoreFramebuffer restoreFbo( renderThread );
	
	deoglFramebuffer &fbo = renderThread.GetFramebuffer().GetEnvMap();
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	renderThread.GetShader().ActivateShader( pShaderEnvMapCopy );
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	renderThread.GetFramebuffer().Activate( &fbo );
	fbo.DetachAllImages();
	fbo.AttachColorCubeMap( 0, &target );
	OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	fbo.Verify();
	
	OGL_CHECK( renderThread, glViewport( 0, 0, target.GetSize(), target.GetSize() ) );
	
	tsmgr.EnableArrayTexture( 3, source, GetSamplerClampNearest() ); // texEmissive
	
	defren.RenderFSQuadVAO();
	
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
	//defren.GetDepthTexture1()->CreateMipMaps();
	
	defren.ActivateFBOTemporary2( false );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	//OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
	//OGL_CHECK( renderThread, glStencilMask( 0 ) );
	//OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
	//OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), ~0 ) );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
	
	renderThread.GetShader().ActivateShader( plan.GetRenderStereo() ? pShaderScreenSpaceStereo : pShaderScreenSpace );
	
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
	
	pEnvMapsParamBlock->MapBuffer();
	
	try{
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
		
	}catch( const deException & ){
		pEnvMapsParamBlock->UnmapBuffer();
		throw;
	}
	
	pEnvMapsParamBlock->UnmapBuffer();
	
	// apply reflections
	defren.ActivateFBOColor( false, false );
	
//OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE ) );
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	
	deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderApplyReflectionsStereo : pShaderApplyReflections;
	renderThread.GetShader().ActivateShader( program );
	
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
	
	if( pEnvMapsParamBlock ){
		pEnvMapsParamBlock->FreeReference();
	}
	
	if( pAddToRenderTask ){
		delete pAddToRenderTask;
	}
	if( pRenderTask ){
		delete pRenderTask;
	}
	if( pRenderParamBlock ){
		pRenderParamBlock->FreeReference();
	}
}
