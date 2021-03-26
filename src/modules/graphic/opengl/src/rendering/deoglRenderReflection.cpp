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
#include "debug/deoglRenderDebug.h"
#include "defren/deoglDeferredRendering.h"
#include "defren/deoglDRDepthMinMax.h"
#include "plan/deoglRenderPlanDebug.h"
#include "plan/deoglRenderPlanEnvMap.h"
#include "plan/deoglRenderPlan.h"
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
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTLogger.h"
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
	spssQuadTCTransform,
	spssPosTransform,
	spssMatrixP,
	spssMatrixBackProjection,
	spssClipReflDirNearDist,
	spssPixelSize,
	spssStepCount,
	spssSubStepCount,
	spssMaxRayLength,
	spssMinMaxTCFactor,
	spssCoverageFactor1,
	spssCoverageFactor2,
	spssRoughnessMaxTaps,
	spssRoughnessTapCountScale
};

enum pSPApplyReflections{
	sparQuadTCTransform,
	sparPosTransform,
	sparMatrixEnvMap,
	sparEnvMapLodLevel,
	sparMipMapLevelParams,
	sparMipMapTCClamp,
	//sparMatrixReflectionBox,
	//sparMatrixReflectionBoxNormal,
	//sparEnvMapPosition,
	//sparHasReflectionBox
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
	spbr2MatrixEnvMap,
	spbr2QuadTCTransform,
	spbr2PosTransform,
	spbr2BlendFactors,
	spbr2EnvMapLodLevel,
	spbr2LayerCount,
	spbr2EnvMapPosLayer
};

enum eSPIndexPass{
	spipQuadTCTransform,
	spipPosTransform,
	spipScaleDistance,
	spipMatrixMVP,
	spipMatrixMV,
	spipEnvMapPosition,
	spipEnvMapIndex
};

/*
enum eSPEnvMapPass{
	spempQuadTCTransform,
	spempPosTransform,
	spempScaleDistance,
	spempBlendFactors,
	spempMatrixEnvMap,
	spempEnvMapLodLevel,
	spempMatrixMVP,
	spempMatrixMV,
	spempEnvMapPosition,
	spempEnvMapIndex
};
*/

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

/*
static const int vCubeFaces[] = {
	deoglCubeMap::efPositiveX, deoglCubeMap::efNegativeX,
	deoglCubeMap::efPositiveY, deoglCubeMap::efNegativeY,
	deoglCubeMap::efPositiveZ, deoglCubeMap::efNegativeZ
};
*/



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
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	pShaderIndexPass1 = NULL;
	pShaderIndexPass2 = NULL;
	/*
	pShaderRenderEnvMaps = NULL;
	*/
	
	pShaderCopyColor = NULL;
	pShaderCopyColorMipMap = NULL;
	pShaderMinMaxMipMapMin = NULL;
	pShaderMinMaxMipMapMax = NULL;
	pShaderMinMaxMipMapInitial = NULL;
	pShaderMinMaxMipMapDownsample = NULL;
	pShaderScreenSpace = NULL;
	pShaderApplyReflections = NULL;
	
	pShaderReflection = NULL;
	pShaderCubeMap2EquiMap = NULL;
	pShaderBuildEnvMap = NULL;
	pShaderEnvMapMask = NULL;
	
	pRenderParamBlock = NULL;
	pRenderTask = NULL;
	pAddToRenderTask = NULL;
	
	pUseEquiEnvMap = true;
	pEnvMap = NULL;
	pEnvMapEqui = NULL;
	pEnvMapsParamBlock = NULL;
	
	pTextureIndices = NULL;
	pTextureDistance1 = NULL;
	pTextureDistance2 = NULL;
	pFBOIndexPass1 = NULL;
	pFBOIndexPass2 = NULL;
	
	pDirectEnvMapActive  = NULL;
	pDirectEnvMapFading = NULL;
	
	pUseEquiEnvMap = config.GetEnvMapUseEqui() || ! extensions.GetHasArrayCubeMap();
	//printf( "EQUI: %i %i %i\n", pUseEquiEnvMap?1:0, config.GetEnvMapUseEqui()?1:0, extensions.GetHasArrayCubeMap()?1:0 );
	
	try{
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection Index Pass" );
		if( config.GetDefRenEncDepth() ){
			defines.AddDefine( "DECODE_IN_DEPTH", "1" );
		}
		if( indirectMatrixAccessBug ){
			defines.AddDefine( "UBO_IDMATACCBUG", "1" );
		}
		if( bugUBODirectLinkDeadloop ){
			defines.AddDefine( "BUG_UBO_DIRECT_LINK_DEAD_LOOP", "1" );
		}
		defines.AddDefine( "FULLSCREENQUAD", "1" );
		pShaderIndexPass1 = shaderManager.GetProgramWith( sources, defines );
		
		defines.AddDefine( "CLAMP_AGAINST_DISTANCE", "1" );
		pShaderIndexPass2 = shaderManager.GetProgramWith( sources, defines );
		
		defines.RemoveAllDefines();
		
		
		/*
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection EnvMap Pass" );
		if( config.GetDefRenEncDepth() ){
			defines.AddDefine( "DECODE_IN_DEPTH", "1" );
		}
		if( indirectMatrixAccessBug ){
			defines.AddDefine( "UBO_IDMATACCBUG", "1" );
		}
		if( bugUBODirectLinkDeadloop ){
			defines.AddDefine( "BUG_UBO_DIRECT_LINK_DEAD_LOOP", "1" );
		}
		defines.AddDefine( "FULLSCREENQUAD", "1" );
		defines.AddDefine( "MATERIAL_NORMAL_DEC_INTBASIC", "1" );
		pShaderRenderEnvMaps = shaderManager.GetProgramWith( sources, defines );
		*/
		
		sources = shaderManager.GetSourcesNamed( "DefRen Copy Color" );
		pShaderCopyColor = shaderManager.GetProgramWith( sources, defines );
		
		defines.AddDefine( "MIPMAP", "1" );
		pShaderCopyColorMipMap = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		if( deoglDRDepthMinMax::USAGE_VERSION == 0 ){
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection MinMap MipMap 2" );
			if( ! sources ){
				DETHROW( deeInvalidParam );
			}
			if( config.GetDefRenEncDepth() ){
				defines.AddDefine( "DECODE_IN_DEPTH", "1" );
			}
			defines.AddDefine( "NO_TEXCOORD", "1" );
			defines.AddDefine( "INITIAL", "1" );
			pShaderMinMaxMipMapInitial = shaderManager.GetProgramWith( sources, defines );
			defines.RemoveAllDefines();
			
			if( config.GetDefRenEncDepth() ){
				defines.AddDefine( "DECODE_IN_DEPTH", "1" );
			}
			defines.AddDefine( "NO_TEXCOORD", "1" );
			defines.AddDefine( "DOWNSAMPLE", "1" );
			pShaderMinMaxMipMapDownsample = shaderManager.GetProgramWith( sources, defines );
			defines.RemoveAllDefines();
			
		}else if( deoglDRDepthMinMax::USAGE_VERSION == 1 ){
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection MinMap MipMap" );
			if( ! sources ){
				DETHROW( deeInvalidParam );
			}
			if( config.GetDefRenEncDepth() ){
				defines.AddDefine( "DECODE_IN_DEPTH", "1" );
			}
			defines.AddDefine( "NO_TEXCOORD", "1" );
			defines.AddDefine( "CLAMP_TC", "1" );
			defines.AddDefine( "FUNC_MIN", "1" );
			pShaderMinMaxMipMapMin = shaderManager.GetProgramWith( sources, defines );
			defines.RemoveAllDefines();
			
			if( config.GetDefRenEncDepth() ){
				defines.AddDefine( "DECODE_IN_DEPTH", "1" );
			}
			defines.AddDefine( "NO_TEXCOORD", "1" );
			defines.AddDefine( "CLAMP_TC", "1" );
			defines.AddDefine( "FUNC_MAX", "1" );
			pShaderMinMaxMipMapMax = shaderManager.GetProgramWith( sources, defines );
			defines.RemoveAllDefines();
			
		}else{ // deoglDRDepthMinMax::USAGE_VERSION == 2
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection MinMap MipMap" );
			if( ! sources ){
				DETHROW( deeInvalidParam );
			}
			if( config.GetDefRenEncDepth() ){
				defines.AddDefine( "DECODE_IN_DEPTH", "1" );
			}
			defines.AddDefine( "NO_TEXCOORD", "1" );
			defines.AddDefine( "CLAMP_TC", "1" );
			defines.AddDefine( "SPLIT_VERSION", "1" );
			defines.AddDefine( "SPLIT_SHIFT_TC", "1" );
			pShaderMinMaxMipMapInitial = shaderManager.GetProgramWith( sources, defines );
			defines.RemoveAllDefines();
			
			if( config.GetDefRenEncDepth() ){
				defines.AddDefine( "DECODE_IN_DEPTH", "1" );
			}
			defines.AddDefine( "NO_TEXCOORD", "1" );
			defines.AddDefine( "SPLIT_VERSION", "1" );
			pShaderMinMaxMipMapDownsample = shaderManager.GetProgramWith( sources, defines );
			defines.RemoveAllDefines();
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
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection ScreenSpace" );
		if( config.GetDefRenEncDepth() ){
			defines.AddDefine( "DECODE_IN_DEPTH", "1" );
		}
		defines.AddDefine( "MATERIAL_NORMAL_DEC_INTBASIC", "1" );
		if( pUseEquiEnvMap ){
			defines.AddDefine( "ENVMAP_EQUI", "1" );
		}
		if( defren.GetUseInverseDepth() ){
			defines.AddDefine( "INVERSE_DEPTH", "1" );
		}
		
		if( config.GetSSRMethod() == 0 ){ // 0 = groundTruth
			defines.AddDefine( "SSR_VERSION", "0" );
			
		}else if( config.GetSSRMethod() == 1 ){ // 1 = stepedSS
			defines.AddDefine( "SSR_VERSION", "1" );
			//defines.AddDefine( "RESULT_AFTER_FIRST_LOOP", "1" ); // this yields wrong results (moving reflections)
				defines.AddDefine( "NESTED_LOOP", "1" ); // enabled slows down on Radeon 4870 but can't do better quality
				defines.AddDefine( "MULTI_STEPPING", "1" );
				// integrated seems worse with SSR_VERSION=1 but required as otherwise NaN/Inf polutes the rendering
				//defines.AddDefine( "INTEGRATED_THRESHOLD_TEST", "1" );
			
			if( deoglDRDepthMinMax::USAGE_VERSION != -1 ){
				defines.AddDefine( "USE_DEPTH_MIPMAP", "1" );
			}
		}
		
		//defines.AddDefine( "ROUGHNESS_TAPPING", "1" );
		
		pShaderScreenSpace = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection ApplyReflections" );
		if( indirectMatrixAccessBug ){
			defines.AddDefine( "UBO_IDMATACCBUG", "1" );
		}
		if( bugUBODirectLinkDeadloop ){
			defines.AddDefine( "BUG_UBO_DIRECT_LINK_DEAD_LOOP", "1" );
		}
		if( config.GetDefRenEncDepth() ){
			defines.AddDefine( "DECODE_IN_DEPTH", "1" );
		}
		defines.AddDefine( "MATERIAL_NORMAL_DEC_INTBASIC", "1" );
		if( pUseEquiEnvMap ){
			defines.AddDefine( "ENVMAP_EQUI", "1" );
		}
		if( defren.GetUseInverseDepth() ){
			defines.AddDefine( "INVERSE_DEPTH", "1" );
		}
		//defines.AddDefine( "HACK_NO_SSR", "1" ); // set to 1 to examine env-map reflection only
		
		if( config.GetEnvMapMethod() == deoglConfiguration::eemmSingle ){
			defines.AddDefine( "ENVMAP_SINGLE", "1" ); // ENVMAP_SINGLE, not ENVMAP_BOX_PROJECTION
			
		}else if( config.GetEnvMapMethod() == deoglConfiguration::eemmMultipleBoxProjection ){
			defines.AddDefine( "ENVMAP_BOX_PROJECTION", "1" ); // ENVMAP_BOX_PROJECTION, not ENVMAP_SINGLE
			
		//}else{ // not ENVMAP_SINGLE, not ENVMAP_BOX_PROJECTION
		}
		
		pShaderApplyReflections = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection" );
		if( config.GetDefRenEncDepth() ){
			defines.AddDefine( "DECODE_IN_DEPTH", "1" );
		}
		if( indirectMatrixAccessBug ){
			defines.AddDefine( "UBO_IDMATACCBUG", "1" );
		}
		if( bugUBODirectLinkDeadloop ){
			defines.AddDefine( "BUG_UBO_DIRECT_LINK_DEAD_LOOP", "1" );
		}
		//if( pUseEquiEnvMap ){
			defines.AddDefine( "ENVMAP_EQUI", "1" );
		//}
		if( defren.GetUseInverseDepth() ){
			defines.AddDefine( "INVERSE_DEPTH", "1" );
		}
		defines.AddDefine( "MATERIAL_NORMAL_DEC_INTBASIC", "1" );
		pShaderReflection = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection CubeMap 2 EquiMap" );
		pShaderCubeMap2EquiMap = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		if( pUseEquiEnvMap ){
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection Build EnvMap Equi" );
			defines.AddDefine( "ENVMAP_EQUI", "1" );
			
		}else{
			sources = shaderManager.GetSourcesNamed( "DefRen Reflection Build EnvMap" );
		}
		pShaderBuildEnvMap = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Reflection EnvMap Mask" );
		//defines.AddDefine( "FULLSCREENQUAD", "1" );
		pShaderEnvMapMask = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		pRenderParamBlock = new deoglSPBlockUBO( renderThread );
		pRenderParamBlock->SetRowMajor( ! indirectMatrixAccessBug );
		pRenderParamBlock->SetParameterCount( 7 );
		pRenderParamBlock->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 3, 3, 1 ); // mat3 pMatrixEnvMap
		pRenderParamBlock->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4 pQuadTCTransform
		pRenderParamBlock->GetParameterAt( 2 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4 pPosTransform
		pRenderParamBlock->GetParameterAt( 3 ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2 pBlendFactors
		pRenderParamBlock->GetParameterAt( 4 ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float pEnvMapLodLevel
		pRenderParamBlock->GetParameterAt( 5 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int pLayerCount
		pRenderParamBlock->GetParameterAt( 6 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 100 ); // vec3 pEnvMapPosLayer[ 100 ]
		pRenderParamBlock->MapToStd140();
		pRenderParamBlock->SetBindingPoint( 0 );
		
		pRenderTask = new deoglRenderTask;
		pAddToRenderTask = new deoglAddToRenderTask( renderThread, *pRenderTask );
		
		if( config.GetEnvMapMethod() == deoglConfiguration::eemmSingle ){
			if( pUseEquiEnvMap ){
				pEnvMapEqui = new deoglTexture( renderThread );
				pEnvMapEqui->SetSize( config.GetEnvMapSize() * 4, config.GetEnvMapSize() * 2 );
				//pEnvMapEqui->SetFBOFormat( 3, true );
				pEnvMapEqui->SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG11B10F );
				pEnvMapEqui->SetMipMapped( true );
				
			}else{
				pEnvMap = new deoglCubeMap( renderThread );
				pEnvMap->SetSize( config.GetEnvMapSize() );
				//pEnvMap->SetFBOFormat( 3, true );
				pEnvMap->SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG11B10F );
				pEnvMap->SetMipMapped( true );
			}
		}
		
		/*
		// create textures
		pIndexTextureWidth = 512;
		pIndexTextureHeight = 512;
		
		// create indices
		pTextureIndices = new deoglTexture( ogl );
		pTextureIndices->SetSize( pIndexTextureWidth, pIndexTextureHeight );
		pTextureIndices->SetFBOFormatIntegral( 2, 8, true );
		pTextureIndices->CreateTexture();
		
		// create distances
		pTextureDistance1 = new deoglTexture( ogl );
		pTextureDistance1->SetSize( pIndexTextureWidth, pIndexTextureHeight );
		pTextureDistance1->SetDepthFormat( false );
		pTextureDistance1->CreateTexture();
		
		pTextureDistance2 = new deoglTexture( ogl );
		pTextureDistance2->SetSize( pIndexTextureWidth, pIndexTextureHeight );
		pTextureDistance2->SetDepthFormat( false );
		pTextureDistance2->CreateTexture();
		
		// create fbo index pass 1
		pFBOIndexPass1 = new deoglFramebuffer( ogl, false );
		
		renderThread.GetFramebuffer().Activate( pFBOIndexPass1 );
		
		pFBOIndexPass1->AttachDepthTexture( pTextureDistance1 );
		pFBOIndexPass1->AttachColorTexture( 0, pTextureIndices );
		
		const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		pFBOIndexPass1->Verify();
		
		// create fbo index pass 2
		pFBOIndexPass2 = new deoglFramebuffer( ogl, false );
		
		renderThread.GetFramebuffer().Activate( pFBOIndexPass2 );
		
		pFBOIndexPass2->AttachDepthTexture( pTextureDistance2 );
		pFBOIndexPass2->AttachColorTexture( 0, pTextureIndices );
		
		const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		pFBOIndexPass2->Verify();
		*/
		
		
		
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
		pEnvMapsParamBlock->SetBindingPoint( 0 );
		
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
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 63 ){
		return;
	}
	
	if( renderThread.GetConfiguration().GetEnvMapMethod() != deoglConfiguration::eemmSingle ){
		return;
	}
	
	
	
	if( pEnvMapEqui ){
		pEnvMapEqui->CreateTexture();
		
	}else{
		pEnvMap->CreateCubeMap();
	}
	
	deoglTexture *blendEnvMapEqui[ 4 ] = { NULL, NULL, NULL, NULL };
	deoglCubeMap *blendEnvMap[ 4 ] = { NULL, NULL, NULL, NULL };
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
	
#if 0
	const deoglEnvironmentMapList &list = plan.GetCollideList().GetEnvironmentMapList();
	deoglEnvironmentMap *closestEnvMap[ 4 ] = { NULL, NULL, NULL, NULL };
	float closestDistance[ 4 ] = { 1000.0f, 1000.0f, 1000.0f, 1000.0f };
	int i, j, k, count;
	
	count = list.GetCount();
	
	for( i=0; i<4; i++ ){
		for( j=0; j<count; j++ ){
			envmap = list.GetAt( j );
			
			//for( k=0; envmap!=closestEnvMap[ k ] && k<i; k++ );
			for( k=0; ( envmap->GetPosition() - envMapPosition[ k ] ).Length()>FLOAT_SAFE_EPSILON && k<i; k++ );
			
			if( k == i && ! envmap->GetSkyOnly() ){
				const float distance = ( float )( ( envmap->GetPosition() - cameraPosition ).Length() );
				
				if( /*distance > nearDistLimit && */ ( ! closestEnvMap[ i ] || distance < closestDistance[ i ] ) ){
					closestEnvMap[ i ] = envmap;
					closestDistance[ i ] = distance;
					envMapPosition[ i ] = envmap->GetPosition();
					
					if( envmap->GetEquiEnvMap() ){
						blendEnvMap[ i ] = envmap->GetEquiEnvMap();
						
					}else{
						blendEnvMap[ i ] = ogl.GetTextureDefaultEmissivity();
					}
				}
			}
		}
		
		if( ! closestEnvMap[ i ] ){
			break;
		}
	}
#endif
	
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
	
	/*
	if( count > 1 ){
		renderThread.GetShader().ActivateShader( pShaderBuildEnvMap );
		shader = pShaderBuildEnvMap->GetCompiled();
	}
	
	if( count < 2 && ! pglCopyImageSubData ){
		// nVidia has a bug with the copy somewhere. most probably it can not correctly copy the RG11B10 format.
		// in this case the rendering would be broken due to the resulting env-map containing garbage. on ATI
		// this works without a problem
		renderThread.GetShader().ActivateShader( pShaderBuildEnvMap );
		shader = pShaderBuildEnvMap->GetCompiled();
	}
	*/
	// for the sake of simplicity we use the shader always even for simple cases. there is no real speed
	// gain in trying to use a copy especially since envmaps can have different dimensions
	renderThread.GetShader().ActivateShader( pShaderBuildEnvMap );
	shader = pShaderBuildEnvMap->GetCompiled();
	
	/*
	if( pEnvMapEqui ){
		printf( "envmapequi %p %p %p %p\n", blendEnvMapEqui[ 0 ], blendEnvMapEqui[ 1 ], blendEnvMapEqui[ 2 ], blendEnvMapEqui[ 3 ] );
	}else{
		printf( "envmap %p %p %p %p\n", blendEnvMap[ 0 ], blendEnvMap[ 1 ], blendEnvMap[ 2 ], blendEnvMap[ 3 ] );
	}
	printf( "weight %f %f %f %f\n", blendWeights[ 0 ], blendWeights[ 1 ], blendWeights[ 2 ], blendWeights[ 3 ] );
	*/
	
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
	
#if 0
	if( blendEnvMap[ 3 ] ){
		const decDVector q = cameraPosition - envMapPosition[ 3 ];
		const decDVector q1 = envMapPosition[ 0 ] - envMapPosition[ 3 ];
		const decDVector q2 = envMapPosition[ 1 ] - envMapPosition[ 3 ];
		const decDVector q3 = envMapPosition[ 2 ] - envMapPosition[ 3 ];
		decDMatrix matrix;
		
		renderThread.GetShader().ActivateShader( pShaderBuildEnvMap );
		shader = pShaderBuildEnvMap->GetCompiled();
		
		/*
		double det = matrix.a13 * ( matrix.a22 * matrix.a31 + matrix.a21 * matrix.a32 )
			+ matrix.a12 * ( matrix.a23 * matrix.a31 - matrix.a21 * matrix.a33 )
			+ matrix.a11 * ( matrix.a22 * matrix.a33 - matrix.a23 * matrix.a32 );
		*/
		
		matrix.a11 = q1.x; matrix.a12 = q1.y; matrix.a13 = q1.z;
		matrix.a21 = q2.x; matrix.a22 = q2.y; matrix.a23 = q2.z;
		matrix.a31 = q3.x; matrix.a32 = q3.y; matrix.a33 = q3.z;
		double factor = matrix.Determinant();
		if( factor != 0.0 ){
			factor = 1.0 / factor;
		}
		
		matrix.a11 = q.x; matrix.a12 = q.y; matrix.a13 = q.z;
		blendWeights[ 0 ] = ( float )( matrix.Determinant() * factor );
		
		matrix.a11 = q1.x; matrix.a12 = q1.y; matrix.a13 = q1.z;
		matrix.a21 = q.x; matrix.a22 = q.y; matrix.a23 = q.z;
		blendWeights[ 1 ] = ( float )( matrix.Determinant() * factor );
		
		matrix.a21 = q2.x; matrix.a22 = q2.y; matrix.a23 = q2.z;
		matrix.a31 = q.x; matrix.a32 = q.y; matrix.a33 = q.z;
		blendWeights[ 2 ] = ( float )( matrix.Determinant() * factor );
		
		blendWeights[ 3 ] = 1.0 - blendWeights[ 0 ] - blendWeights[ 1 ] - blendWeights[ 2 ];
		
	}else if( blendEnvMap[ 2 ] ){
		// three environment maps found. use barycentric coordinates on a triangle
		renderThread.GetShader().ActivateShader( pShaderBuildEnvMap );
		shader = pShaderBuildEnvMap->GetCompiled();
		
		const decDVector temp = envMapPosition[ 2 ] - envMapPosition[ 1 ];
		decDVector vn = ( envMapPosition[ 1 ] - envMapPosition[ 0 ] ) % temp;
		const double area = vn.Length();
		vn /= area * area;
		const decDVector cp = cameraPosition + vn * ( ( envMapPosition[ 1 ] - cameraPosition ) * vn );
		
		blendWeights[ 0 ] = ( float )( ( temp % ( cp - envMapPosition[ 1 ] ) ) * vn );
		if( blendWeights[ 0 ] > 1.0f ){
			blendWeights[ 0 ] = 1.0f;
		}
		if( blendWeights[ 0 ] < 0.0f ){
			blendWeights[ 0 ] = 0.0f;
		}
		
		blendWeights[ 1 ] = ( float )( ( ( envMapPosition[ 0 ] - envMapPosition[ 2 ] ) % ( cp - envMapPosition[ 2 ] ) ) * vn );
		if( blendWeights[ 0 ] > 1.0f ){
			blendWeights[ 0 ] = 1.0f;
		}
		if( blendWeights[ 0 ] < 0.0f ){
			blendWeights[ 0 ] = 0.0f;
		}
		
		blendWeights[ 2 ] = 1.0 - blendWeights[ 0 ] - blendWeights[ 1 ];
		if( blendWeights[ 2 ] > 1.0f ){
			blendWeights[ 2 ] = 1.0f;
		}
		if( blendWeights[ 2 ] < 0.0f ){
			blendWeights[ 2 ] = 0.0f;
		}
		// this doesn't work shit since values can go beyond [0..1] ... U_U
		
		printf( "blend %f %f %f\n", blendWeights[ 0 ], blendWeights[ 1 ], blendWeights[ 2 ] );
		
		blendEnvMap[ 3 ] = blendEnvMap[ 2 ]; // for the time being we need all textures set
		
	}else if( blendEnvMap[ 1 ] ){
		// two environment maps found. use barycentric coordinates on a line
		const decDVector q = cameraPosition - envMapPosition[ 1 ];
		const decDVector q1 = envMapPosition[ 0 ] - envMapPosition[ 1 ];
		const double len = q1.Length();
		
		renderThread.GetShader().ActivateShader( pShaderBuildEnvMap );
		shader = pShaderBuildEnvMap->GetCompiled();
		
		blendWeights[ 0 ] = ( float )( ( q * q1 ) / ( len * len ) );
		if( blendWeights[ 0 ] > 1.0f ){
			// degenrated into the case of a single environment map
			blendWeights[ 0 ] = 1.0f;
			shader = NULL;
			
		}else{
			blendWeights[ 1 ] = 1.0f - blendWeights[ 0 ];
			
			blendEnvMap[ 2 ] = blendEnvMap[ 1 ]; // for the time being we need all textures set
			blendEnvMap[ 3 ] = blendEnvMap[ 1 ]; // for the time being we need all textures set
		}
		
	}else if( blendEnvMap[ 0 ] ){
		// a single environment map found. use it without blending. a copy is faster
		
	}else{
		// no environment maps found. use the sky environment map instead if existing.
		// use it without blending. a copy is faster
		deoglRSkyInstance * const sky = plan.GetWorld()->GetSky();
		
		if( sky && sky->GetEnvironmentMap() ){
			blendEnvMap[ 0 ] = sky->GetEnvironmentMap()->GetEquiEnvMap();
		}
		
		if( ! blendEnvMap[ 0 ] ){
			blendEnvMap[ 0 ] = ogl.GetTextureDefaultEmissivity();
		}
	}
#endif
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
			
//#define RENDER_MIPMAP_LEVELS 1
#ifdef RENDER_MIPMAP_LEVELS
			int j;
			
			if( pEnvMapEqui ){
				count = pEnvMapEqui->GetRealMipMapLevelCount();
				
			}else{
				count = pEnvMap->GetRealMipMapLevelCount();
			}
			
			for( i=0; i<count; i++ ){
				if( pEnvMapEqui ){
					fbo->AttachColorTextureLevel( 0, pEnvMapEqui, i );
					
				}else{
					for( j=0; j<6; j++ ){
						fbo->AttachColorCubeMapLevel( j, pEnvMap, i, i );
					}
				}
				fbo->Verify();
				
				OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
				
				if( pEnvMapEqui ){
					tsmgr.EnableTexture( 0, *blendEnvMapEqui[ 0 ], blendFiltering[ 0 ], GL_CLAMP_TO_EDGE );
					
				}else{
					tsmgr.EnableCubeMap( 0, *blendEnvMap[ 0 ], blendFiltering[ 0 ] );
				}
				
				shader->SetParameterFloat( spbemMipMapLevel, ( float )i );
				
				defren.RenderFSQuadVAO();
				
				width >>= 1;
				if( width < 1 ){
					width = 1;
				}
				height >>= 1;
				if( height < 1 ){
					height = 1;
				}
			}
#else
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
#endif
			
			renderThread.GetFramebuffer().Activate( oldfbo );
			if( fbo ){
				fbo->DecreaseUsageCount();
			}
			
#ifndef RENDER_MIPMAP_LEVELS
			if( pEnvMapEqui ){
				pEnvMapEqui->CreateMipMaps();
				
			}else{
				pEnvMap->CreateMipMaps();
			}
#endif
			
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
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 65 ){
		decString text;
		
		if( pEnvMapEqui ){
			count = pEnvMapEqui->GetRealMipMapLevelCount();
			for( i=0; i<count; i++ ){
				text.Format( "envmap_equimap_l%.2i", i );
				renderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion(
					*pEnvMapEqui, i, text.GetString(), deoglDebugSaveTexture::ecColorLinearToneMapsRGB );
				text.Format( "envmap_equimap_source1_l%.2i", i );
				renderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion(
					*blendEnvMapEqui[0], i, text.GetString(), deoglDebugSaveTexture::ecColorLinearToneMapsRGB );
			}
			
		}else{
			count = pEnvMap->GetRealMipMapLevelCount();
			for( i=0; i<count; i++ ){
				text.Format( "envmap_l%.2i", i );
				renderThread.GetDebug().GetDebugSaveTexture().SaveCubeMapLevelConversion(
					*pEnvMap, i, text.GetString(), true, deoglDebugSaveTexture::ecColorLinearToneMapsRGB );
				text.Format( "envmap_source1_l%.2i", i );
				renderThread.GetDebug().GetDebugSaveTexture().SaveCubeMapLevelConversion(
					*blendEnvMap[0], i, text.GetString(), true, deoglDebugSaveTexture::ecColorLinearToneMapsRGB );
			}
		}
		
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
}



void deoglRenderReflection::RenderReflections( deoglRenderPlan &plan ){
if( deoglSkinShader::REFLECTION_TEST_MODE == 1 ){
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
	
	// HACK
	/*if( renderThread.GetConfiguration()->GetDebugSnapshot() != 62 ){
		RenderIndices( plan );
	}*/
	// HACK
	
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
	renderThread.GetShader().ActivateShader( pShaderReflection );
	
	pRenderParamBlock->Activate();
	DEBUG_PRINT_TIMER( "Reflection: Activate Shader" );
	
	// set textures
	if( renderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
		tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 4, *defren.GetTextureRoughness(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		
	}else{
		tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 4, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
	}
	
	tsmgr.EnableArrayTexture( 6, *envMapSlotMgr.GetArrayTexture(), GetSamplerRepeatLinearMipMap() );
	tsmgr.EnableTexture( 7, *envMapSky, GetSamplerRepeatLinearMipMap() );
	DEBUG_PRINT_TIMER( "Reflection: Enable Textures" );
	
	defren.RenderFSQuadVAO();
	DEBUG_PRINT_TIMER( "Reflection: Render" );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 60 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *defren.GetTextureTemporary1(), "refl_reflection" );
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
}

#if 0
	if( renderThread.GetConfiguration()->GetDebugSnapshot() == 61 ){
		return;
	}
	
	// resize the textures. for testing purpose we use the same size as the render plan size
	/*
	const int vpWidth = plan.GetViewportWidth();
	const int vpHeight = plan.GetViewportHeight();
	
	if( vpWidth > pIndexTextureWidth || vpHeight > pIndexTextureHeight ){
		pTextureIndices->SetSize( vpWidth, vpHeight );
		pTextureIndices->CreateTexture();
		pTextureDistance1->SetSize( vpWidth, vpHeight );
		pTextureDistance1->CreateTexture();
		pTextureDistance2->SetSize( vpWidth, vpHeight );
		pTextureDistance2->CreateTexture();
		pIndexTextureWidth = vpWidth;
		pIndexTextureHeight = vpHeight;
	}
	*/
	
	DEBUG_RESET_TIMERS;
	
	RenderIndices( plan );
	RenderEnvMaps( plan );
	RenderScreenSpace( plan );
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	
	DEBUG_PRINT_TIMER_TOTAL( "RenderReflections Total" );
	
	if( renderThread.GetConfiguration()->GetDebugSnapshot() == 60 ){
		renderThread.GetConfiguration()->SetDebugSnapshot( 0 );
	}
#endif
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
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglEnvMapSlotManager &envMapSlotMgr = renderThread.GetEnvMapSlotManager();
	const decDMatrix &matrixCamera = plan.GetCameraMatrix();
	const int count = decMath::min( envMapSlotMgr.GetUsedSlotCount(), 100 );
	int i;
	
	pRenderParamBlock->MapBuffer();
	
	try{
		// general parameters required for the full screen quad and position reconstruction
		pRenderParamBlock->SetParameterDataMat3x3( spbr2MatrixEnvMap,
			plan.GetRefPosCameraMatrix().GetRotationMatrix().Invert() );
		defren.SetShaderParamFSQuad( *pRenderParamBlock, spbr2QuadTCTransform );
		pRenderParamBlock->SetParameterDataVec4( spbr2PosTransform, plan.GetDepthToPosition() );
		
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



void deoglRenderReflection::RenderIndices( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglEnvironmentMapList &list = plan.GetWorld()->GetEnvMapList();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const decDMatrix &matrixCamera = plan.GetCameraMatrix();
	const GLfloat clearDepth = defren.GetClearDepthValueRegular();
	GLuint clearColor[ 4 ] = { 0, 0, 0, 0 };
	const int count = list.GetCount();
	deoglShaderCompiled *shader;
	int i;
	
	// set states
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	//OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	//SetCullMode( false );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT, GL_ZERO_TO_ONE );
	}
	
	OGL_CHECK( renderThread, glEnable( GL_POLYGON_OFFSET_FILL ) );
	OGL_CHECK( renderThread, pglPolygonOffset( 0.0f, 1.0f ) );
	
	DEBUG_PRINT_TIMER( "Index Pass 1: Set States" );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, pIndexTextureWidth, pIndexTextureHeight ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, pIndexTextureWidth, pIndexTextureHeight ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	DEBUG_PRINT_TIMER( "Index Pass 1: Viewport/Scissor" );
	
	// bind full screen quad for testing purpose. later on this will be improved
	// with using only the bounding box around the environment map
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	DEBUG_PRINT_TIMER( "Index Pass 1: Bind Full-Screen VAO" );
	
	
	
	// activate first pass fbo
	renderThread.GetFramebuffer().Activate( pFBOIndexPass1 );
	DEBUG_PRINT_TIMER( "Index Pass 1: Activate FBO" );
	
	// clear the first distance texture and the indices texture
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	clearColor[ 0 ] = ( 1 << 8 ) - 1; // ( 1 << 16 ) - 1;
	clearColor[ 1 ] = clearColor[ 0 ];
	clearColor[ 2 ] = clearColor[ 0 ];
	clearColor[ 3 ] = clearColor[ 0 ];
	OGL_CHECK( renderThread, pglClearBufferuiv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	DEBUG_PRINT_TIMER( "Index Pass 1: Clear" );
	
	// activate first pass shader and set common parameters
	renderThread.GetShader().ActivateShader( pShaderIndexPass1 );
	shader = pShaderIndexPass1->GetCompiled();
	
	defren.SetShaderParamFSQuad( *shader, spipQuadTCTransform );
	shader->SetParameterVector4( spipPosTransform, plan.GetDepthToPosition() );
	
	//defren.SetShaderParamFSQuad( *shader, spipPosTransform );
	shader->SetParameterFloat( spipScaleDistance, 1.0f / 20.0f /*( float )plan.GetCameraImageDistance()*/ );
	DEBUG_PRINT_TIMER( "Index Pass 1: Activate Shader" );
	
	tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	DEBUG_PRINT_TIMER( "Index Pass 1: Enable Depth Texture" );
	
	for( i=0; i<count; i++ ){
		const deoglEnvironmentMap &envmap = *list.GetAt( i );
		
		if( ! envmap.GetSkyOnly() ){
			shader->SetParameterMatrix4x4( spipMatrixMVP, decMatrix() );
			shader->SetParameterMatrix4x3( spipMatrixMV, decMatrix() );
			shader->SetParameterDVector3( spipEnvMapPosition, matrixCamera * envmap.GetPosition() );
			shader->SetParameterInt( spipEnvMapIndex, i );
			//DEBUG_PRINT_TIMER( "Index Pass 1: Level Set Uniform" );
			
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
			//DEBUG_PRINT_TIMER( "Index Pass 1: Level Draw Quad" );
		}
	}
	DEBUG_PRINT_TIMER( "Index Pass 1: Draw Env Maps" );
	
	
	
	// activate second pass fbo
	renderThread.GetFramebuffer().Activate( pFBOIndexPass2 );
	DEBUG_PRINT_TIMER( "Index Pass 2: Activate FBO" );
	
	// clear the second dinstance texture 
	OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_TRUE, GL_FALSE, GL_FALSE ) );
	DEBUG_PRINT_TIMER( "Index Pass 2: Clear" );
	
	// activate second pass shader and set common parameters
	renderThread.GetShader().ActivateShader( pShaderIndexPass2 );
	shader = pShaderIndexPass2->GetCompiled();
	
	defren.SetShaderParamFSQuad( *shader, spipQuadTCTransform );
	shader->SetParameterVector4( spipPosTransform, plan.GetDepthToPosition() );
	
	//defren.SetShaderParamFSQuad( *shader, spipPosTransform );
	shader->SetParameterFloat( spipScaleDistance, 1.0f / 20.0f /*( float )plan.GetCameraImageDistance()*/ );
	DEBUG_PRINT_TIMER( "Index Pass 2: Activate Shader" );
	
	tsmgr.EnableTexture( 1, *pTextureDistance1, GetSamplerClampNearest() );
	DEBUG_PRINT_TIMER( "Index Pass 2: Enable Distance 1 Texture" );
	
	for( i=0; i<count; i++ ){
		const deoglEnvironmentMap &envmap = *list.GetAt( i );
		
		if( ! envmap.GetSkyOnly() ){
			shader->SetParameterMatrix4x4( spipMatrixMVP, decMatrix() );
			shader->SetParameterMatrix4x3( spipMatrixMV, decMatrix() );
			shader->SetParameterDVector3( spipEnvMapPosition, matrixCamera * envmap.GetPosition() );
			shader->SetParameterInt( spipEnvMapIndex, i );
			//DEBUG_PRINT_TIMER( "Index Pass 2: Level Set Uniform" );
			
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
			//DEBUG_PRINT_TIMER( "Index Pass 2: Level Draw Quad" );
		}
	}
	DEBUG_PRINT_TIMER( "Index Pass 2: Draw Env Maps" );
	
	
	
	// restore opengl states
	OGL_CHECK( renderThread, glDisable( GL_POLYGON_OFFSET_FILL ) );
	
	// debug
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 60 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTexture(
			*pTextureDistance1, "refl_pass1_distance", deoglDebugSaveTexture::edtLinear );
		renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *pTextureIndices, "refl_pass1_indices" );
		renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTexture(
			*pTextureDistance2, "refl_pass2_distance", deoglDebugSaveTexture::edtLinear );
		renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *pTextureIndices, "refl_pass2_indices" );
	}
}

#if 0
void deoglRenderReflection::RenderEnvMaps( deoglRenderPlan &plan ){
	const deoglEnvironmentMapList &list = plan.GetWorld()->GetEnvMapList();
	deoglTextureStageManager &tsmgr = *renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = *renderThread.GetDeferredRendering();
	const decDMatrix &matrixCamera = plan.GetCameraMatrix();
//	GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const int count = list.GetCount();
	deoglShaderCompiled *shader;
	int i;
	
	// set states
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
//	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
	DEBUG_PRINT_TIMER( "EnvMap Pass: Set States" );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	DEBUG_PRINT_TIMER( "EnvMap Pass: Viewport/Scissor" );
	
	// activate temporary 2 fbo
//	defren.ActivateFBOTemporary2( false );
	defren.ActivateFBOColor( false, false );
	DEBUG_PRINT_TIMER( "EnvMap Pass: Activate FBO" );
	
	// clear the temporary 2 texture
//	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
//	DEBUG_PRINT_TIMER( "EnvMap Pass: Clear" );
	
	// activate envmap pass shader and set common parameters
	renderThread.GetShader().ActivateShader( pShaderRenderEnvMaps );
	shader = pShaderRenderEnvMaps->GetCompiled();
	
	const float znear = plan.GetCameraImageDistance();
	const float zfar = plan.GetCameraViewDistance();
	const float aspectRatio = plan.GetAspectRatio();
	const float fovRatio = plan.GetCameraFovRatio();
	const float fov = plan.GetCameraFov();
	const float q = zfar / ( zfar - znear );
	
	shader->SetParameterFloat( spipQuadTCTransform, 0.5f, 0.5f, 0.5f, 0.5f );
	shader->SetParameterVector4( spempPosTransform, plan.GetDepthToPosition() );
	
	//defren.SetShaderParamFSQuad( *shader, spipPosTransform );
	shader->SetParameterFloat( spempScaleDistance, 20.0f /*( float )plan.GetCameraImageDistance()*/ );
	shader->SetParameterFloat( spempBlendFactors, 1.0f / ( 2.0f * /*blendWidth[m]=*/1.0f ), 0.5f );
	shader->SetParameterMatrix3x3( spempMatrixEnvMap, plan.GetRefPosCameraMatrix().GetRotationMatrix().Invert() );
	DEBUG_PRINT_TIMER( "EnvMap Pass: Activate Shader" );
	
	tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 1, *defren.GetTextureNormal(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 2, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 3, *defren.GetTextureRoughness(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 4, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 5, *pTextureIndices, GetSamplerClampNearest() );
	tsmgr.EnableTexture( 6, *pTextureDistance1, deoglTextureStageManager::etfLinear, GL_CLAMP_TO_EDGE );
	tsmgr.EnableTexture( 7, *pTextureDistance2, deoglTextureStageManager::etfLinear, GL_CLAMP_TO_EDGE );
	DEBUG_PRINT_TIMER( "EnvMap Pass: Enable Textures" );
	
	for( i=0; i<count; i++ ){
		const deoglEnvironmentMap &envmap = *list.GetAt( i );
		
		if( ! envmap.GetSkyOnly() ){
			shader->SetParameterFloat( spempEnvMapLodLevel, ( float )envmap.GetEnvironmentMap()->GetSize() * 1.0f );
			
			shader->SetParameterMatrix4x4( spempMatrixMVP, decMatrix() );
			shader->SetParameterMatrix4x3( spempMatrixMV, decMatrix() );
			shader->SetParameterDVector3( spempEnvMapPosition, matrixCamera * envmap.GetPosition() );
			shader->SetParameterInt( spempEnvMapIndex, i );
			//DEBUG_PRINT_TIMER( "EnvMap Pass: Level Set Uniform" );
			
			tsmgr.EnableCubeMap( 7, *envmap.GetEnvironmentMap(), deoglTextureStageManager::etfLinearMipMap );
			
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
			//DEBUG_PRINT_TIMER( "EnvMap Pass: Level Draw Quad" );
		}
	}
	DEBUG_PRINT_TIMER( "EnvMap Pass: Draw Env Maps" );
	
	// restore opengl states
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	DEBUG_PRINT_TIMER( "EnvMap Pass: Disable VAO" );
	
	// debug
	if( renderThread.GetConfiguration()->GetDebugSnapshot() == 60 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *defren.GetTextureTemporary2(), "refl_envmappass", false );
	}
}

void deoglRenderReflection::RenderScreenSpace( deoglRenderPlan &plan ){
}
#endif

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
		
		tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		
		shader->SetParameterInt( spmmmmTCClamp, defren.GetWidth() - 1, defren.GetHeight() - 1 );
		
		defren.RenderFSQuadVAO();
		DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Initial Pass" );
		
		// downsample up to the max level. the first level has been done already by the initial pass
		renderThread.GetShader().ActivateShader( pShaderMinMaxMipMapDownsample );
		shader = pShaderMinMaxMipMapDownsample->GetCompiled();
		
		tsmgr.EnableTexture( 0, *depthMinMap.GetTexture(), GetSamplerClampNearest() );
		
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
			renderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion(
				*depthMinMap.GetTexture(), 0, text.GetString(), defren.GetUseInverseDepth() ?
					deoglDebugSaveTexture::ecDepthBufferInverse : deoglDebugSaveTexture::ecNoConversion );
			
			for( i=1; i<mipMapLevelCount; i++ ){
				width = decMath::max( width >> 1, 1 );
				height = decMath::max( height >> 1, 1 );
				
				text.Format( "depth_minmax_level%i_%ix%i", i, width, height );
				renderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion(
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
				
				tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
				
			}else{
				shader->SetParameterInt( spmmmmTCClamp, width - 1, height - 1 );
				shader->SetParameterInt( spmmmmMipMapLevel, i - 1 );
				
				if( i == 1 ){
					tsmgr.EnableTexture( 0, *depthMinMap.GetTextureMin(), GetSamplerClampNearest() );
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
				renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTextureLevel(
					*depthMinMap.GetTextureMin(), i, text.GetString(), deoglDebugSaveTexture::edtDepth );
				if( i > 0 ){
					tsmgr.EnableTexture( 0, *depthMinMap.GetTextureMin(), GetSamplerClampNearest() );
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
				
				tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
				
			}else{
				shader->SetParameterInt( spmmmmTCClamp, width - 1, height - 1 );
				shader->SetParameterInt( spmmmmMipMapLevel, i - 1 );
				
				if( i == 1 ){
					tsmgr.EnableTexture( 0, *depthMinMap.GetTextureMax(), GetSamplerClampNearest() );
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
				renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTextureLevel(
					*depthMinMap.GetTextureMax(), i, text.GetString(), deoglDebugSaveTexture::edtDepth );
				if( i > 0 ){
					tsmgr.EnableTexture( 0, *depthMinMap.GetTextureMax(), GetSamplerClampNearest() );
				}
			}
		}
		
		
		
	}else{ // deoglDRDepthMinMax::USAGE_VERSION == 2
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
		
		tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		
		shader->SetParameterInt( spmmmmTCClamp, defren.GetWidth() - 1, defren.GetHeight() - 1 );
		shader->SetParameterInt( spmmmmMipMapLevel, 0 );
		shader->SetParameterInt( spmmmmSplitPos, width );
		
		defren.RenderFSQuadVAO();
		DEBUG_PRINT_TIMER( "Reflection Depth Min-Max: Initial Pass" );
		if( renderThread.GetConfiguration().GetDebugSnapshot() == 62 ){
			decString text;
			text.Format( "depth_minmax_level0_%ix%i", width, height );
			renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTextureLevel(
				*depthMinMap.GetTexture(), 0, text.GetString(), deoglDebugSaveTexture::edtDepth );
		}
		
		// downsample up to the max level. the first level has been done already by the initial pass
		renderThread.GetShader().ActivateShader( pShaderMinMaxMipMapDownsample );
		shader = pShaderMinMaxMipMapDownsample->GetCompiled();
		
		tsmgr.EnableTexture( 0, *depthMinMap.GetTexture(), GetSamplerClampNearest() );
		
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
				renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTextureLevel(
					*depthMinMap.GetTexture(), i, text.GetString(), deoglDebugSaveTexture::edtDepth );
				tsmgr.EnableTexture( 0, *depthMinMap.GetTexture(), GetSamplerClampNearest() );
			}
		}
	}
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 62 ){
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
}

#if 1
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
	
	renderThread.GetShader().ActivateShader( pShaderCopyColor );
	shader = pShaderCopyColor->GetCompiled();
	
	defren.SetShaderParamFSQuad( *shader, spccQuadParams );
	
	tsmgr.EnableTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	
	defren.RenderFSQuadVAO();
	
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
		renderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion( *defren.GetTextureTemporary1(), 0,
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
			renderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion( *defren.GetTextureTemporary1(), level,
				text.GetString(), deoglDebugSaveTexture::ecColorLinear2sRGB );
		}
		
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
}

#else
void deoglRenderReflection::CopyColorToTemporary1( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = *renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = *renderThread.GetTexture().GetStages();
	deoglShaderCompiled *shader;
	int realHeight = defren.GetRealHeight();
	int realWidth = defren.GetRealWidth();
	int height = defren.GetHeight();
	int width = defren.GetWidth();
	int lastWidth, lastHeight;
	float hsu, hsv;
	int level = 0;
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	// copy base level
	defren.ActivateFBOTemporary1Level( 0 );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
	
	renderThread.GetShader().ActivateShader( pShaderCopyColor );
	shader = pShaderCopyColor->GetCompiled();
	
	defren.SetShaderParamFSQuad( *shader, spccQuadParams );
	
	tsmgr.EnableTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	// downsample mip-map levels
	renderThread.GetShader().ActivateShader( pShaderCopyColorMipMap );
	shader = pShaderCopyColorMipMap->GetCompiled();
	
	tsmgr.EnableTexture( 0, *defren.GetTextureTemporary1(), deoglTextureStageManager::etfLinearMipMap, GL_CLAMP_TO_EDGE );
	
	while( width > 1 && height > 1 ){
		// this might look a bit strange but there is a reason for all this here. the mip map dimensions
		// can be odd resulting in a non-matching downsample. to sample the correct values the following
		// calculation is required. the last width and height is first stripped of the last bit. this
		// bit will be lost in a right shift of 1. stripping the last bit is the same as doing first a
		// right shift by 1 followed by a left shift by 1 just cheaper. this is now the size of the
		// rectangle to sample from compared to the real width of the previous mip map level. we can not
		// calculate this in a different way since the shifted dimension looses the last bit but the
		// real size still has the last bit. not doing this calculation can produce an error of a single
		// pixel. this is most probably not bad but it ensures the lost pixel is at the border of the
		// screen where you hardly notice it and not somewhere in the middle of the screen.
		lastWidth = width & ~1;
		if( lastWidth < 1 ){
			lastWidth = 1;
		}
		lastHeight = height & ~1;
		if( lastHeight < 1 ){
			lastHeight = 1;
		}
		
		hsu = 0.5f * ( float )lastWidth / ( float )realWidth;
		hsv = 0.5f * ( float )lastHeight / ( float )realHeight;
		
		realWidth >>= 1;
		if( realWidth < 1 ){
			realWidth = 1;
		}
		realHeight >>= 1;
		if( realHeight < 1 ){
			realHeight = 1;
		}
		
		width >>= 1;
		if( width < 1 ){
			width = 1;
		}
		height >>= 1;
		if( height < 1 ){
			height = 1;
		}
		
		level++;
		
		defren.ActivateFBOTemporary1Level( level );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
		
		shader->SetParameterFloat( spccQuadParams, hsu, hsv, hsu, hsv );
		shader->SetParameterFloat( spccMipMapLevel, ( float )( level - 1 ) );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	
	if( renderThread.GetConfiguration()->GetDebugSnapshot() == 64 ){
		decString text;
		
		width = defren.GetWidth();
		height = defren.GetHeight();
		level = 0;
		
		text.Format( "refl_ssr_copycolor_level0_%ix%i", width, height );
		renderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion( *defren.GetTextureTemporary1(), 0,
			text.GetString(), false, deoglDebugSaveTexture::ecColorLinear2sRGB );
		
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
			renderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion( *defren.GetTextureTemporary1(), level,
				text.GetString(), false, deoglDebugSaveTexture::ecColorLinear2sRGB );
		}
		
		renderThread.GetConfiguration()->SetDebugSnapshot( 0 );
	}
}
#endif

void deoglRenderReflection::RenderScreenSpace( deoglRenderPlan &plan ){
	if( plan.GetNoReflections() ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	
	if( config.GetDebugSnapshot() == 63 ){
		return;
	}
	if( ! config.GetSSREnable() ){
		return;
	}
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglShaderCompiled *shader;
	
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
	
	renderThread.GetShader().ActivateShader( pShaderScreenSpace );
	shader = pShaderScreenSpace->GetCompiled();
	
	defren.SetShaderParamFSQuad( *shader, spssQuadTCTransform );
	shader->SetParameterVector4( spssPosTransform, plan.GetDepthToPosition() );
	
	// NOTE back projection matrix is not used right now. what has it been added for in the first place?
	shader->SetParameterDMatrix4x4( spssMatrixP, plan.GetProjectionMatrix() );
	shader->SetParameterMatrix4x4( spssMatrixBackProjection, decMatrix() ); // plan.GetProjectionMatrix().GetInverse() * lastFrameProjMat
	
	shader->SetParameterFloat( spssClipReflDirNearDist, plan.GetCameraImageDistance() * 0.9f );
	shader->SetParameterFloat( spssPixelSize, ( float )defren.GetRealWidth(), ( float )defren.GetRealHeight() );
	
	const int stepCount = config.GetSSRStepCount();
	const int maxRayLength = decMath::max( stepCount, ( int )(
		( float )decMath::max( defren.GetWidth(), defren.GetHeight() ) * config.GetSSRMaxRayLength() ) );
	const int subStepCount = int( floorf( log2f( ( float )maxRayLength / ( float )stepCount ) ) ) + 1;
	
	shader->SetParameterInt( spssStepCount, stepCount );
	shader->SetParameterInt( spssSubStepCount, subStepCount );
	shader->SetParameterInt( spssMaxRayLength, maxRayLength );
	
	if( deoglDRDepthMinMax::USAGE_VERSION != -1 ){
		// the mip-max texture is the largest factor-of-2 texture size equal to or smaller
		// than the deferred rendering size. the pixels are sampled by factor two which is:
		//   realTC = mipMapTC * 2
		// 
		// to get from realTC back to mipMapTC:
		//   mipMapTC = realTC * 0.5
		// 
		// realTC is in relative texture coordinates. mipMapTC also has to be in relative
		// texture coordinates. this requires an appropriate scaling:
		//   mipMapTC = realTC * ( 0.5 * realSize / mipMapSize )
		shader->SetParameterFloat( spssMinMaxTCFactor,
			0.5f * ( float )defren.GetRealWidth() / ( float )defren.GetDepthMinMax().GetWidth(),
			0.5f * ( float )defren.GetRealHeight() / ( float )defren.GetDepthMinMax().GetHeight() );
		
	}else{
		shader->SetParameterFloat( spssMinMaxTCFactor, 0.0f, 0.0f );
	}
	
	const float invCoverageEdgeSize = 1.0f / config.GetSSRCoverageEdgeSize();
	shader->SetParameterFloat( spssCoverageFactor1, 1.0f / defren.GetScalingU(), 1.0f / defren.GetScalingV(), 1.0f, 0.0f );
	shader->SetParameterFloat( spssCoverageFactor2, -invCoverageEdgeSize, invCoverageEdgeSize * 0.5f,
		config.GetSSRCoveragePowerEdge(), config.GetSSRCoveragePowerRayLength() );
	
	const int roughnessTapMax = 5; //20;
	const float roughnessTapRange = 0.1f;
	shader->SetParameterInt( spssRoughnessMaxTaps, roughnessTapMax );
	shader->SetParameterFloat( spssRoughnessTapCountScale, ( float )roughnessTapMax / roughnessTapRange );
	
	if( renderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
		tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		if( deoglDRDepthMinMax::USAGE_VERSION != -1 ){
			tsmgr.EnableTexture( 1, *defren.GetDepthMinMax().GetTexture(), GetSamplerClampNearestMipMap() );
		}
		tsmgr.EnableTexture( 2, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 3, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 4, *defren.GetTextureRoughness(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		
	}else{
		tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		if( deoglDRDepthMinMax::USAGE_VERSION != -1 ){
			tsmgr.EnableTexture( 1, *defren.GetDepthMinMax().GetTexture(), GetSamplerClampNearestMipMap() );
		}
		tsmgr.EnableTexture( 2, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 3, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 4, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
	}
	
	defren.RenderFSQuadVAO();
	DEBUG_PRINT_TIMER( "Reflection ScreenSpace: Render Screen Space Reflection" );
	
	//defren.GetTextureTemporary2()->CreateMipMaps();
	
	// this fails on nVidia if NV_copy_image is only supported since this is a copy from RGBA to RGB format and
	// only ARB_copy_image supports this not NV_copy_image
	//defren.GetTextureColor()->CopyFrom( *defren.GetTextureTemporary1(), false, defren.GetWidth(), defren.GetHeight(), 0, 0, 0, 0 );
	
	if( config.GetDebugSnapshot() == 60 ){
#if 1
		renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *defren.GetTextureTemporary2(), "refl_ssr" );
		/*
		deoglPixelBuffer pbuf( deoglPixelBuffer::epfFloat3, defren.GetTextureTemporary2()->GetWidth(), defren.GetTextureTemporary2()->GetHeight(), 1 );
		defren.GetTextureTemporary2()->GetPixelsLevel( 0, pbuf );
		const deoglPixelBuffer::sFloat3 * const data = pbuf.GetPointerFloat24();
		const deoglPixelBuffer::sFloat3 &v1 = data[ defren.GetTextureTemporary2()->GetWidth() * ( defren.GetTextureTemporary2()->GetHeight() - 1 - 300 ) + 518 ];
		const deoglPixelBuffer::sFloat3 &v2 = data[ defren.GetTextureTemporary2()->GetWidth() * ( defren.GetTextureTemporary2()->GetHeight() - 1 - 288 ) + 205 ];
		const deoglPixelBuffer::sFloat3 &v3 = data[ defren.GetTextureTemporary2()->GetWidth() * ( defren.GetTextureTemporary2()->GetHeight() - 1 - 233 ) + 570 ];
		const deoglPixelBuffer::sFloat3 &v4 = data[ defren.GetTextureTemporary2()->GetWidth() * ( defren.GetTextureTemporary2()->GetHeight() - 1 - 245 ) + 620 ];
		const deoglPixelBuffer::sFloat3 &v5 = data[ defren.GetTextureTemporary2()->GetWidth() * ( defren.GetTextureTemporary2()->GetHeight() - 1 - 323 ) + 413 ];
		const deoglPixelBuffer::sFloat3 &v6 = data[ defren.GetTextureTemporary2()->GetWidth() * ( defren.GetTextureTemporary2()->GetHeight() - 1 - 321 ) + 413 ];
		if( false ) printf( "v1(%f,%f,%f) v2(%f,%f,%f)\n", v1.r, v1.g, v1.b, v2.r, v2.g, v2.b );
		if( false ) printf( "v3(%f,%f,%f) v4(%f,%f,%f)\n", v3.r, v3.g, v3.b, v4.r, v4.g, v4.b );
		if( true ) printf( "v5(%f,%f,%f) v6(%f,%f,%f)\n", v5.r, v5.g, v5.b, v6.r, v6.g, v6.b );
		*/
		
#else
		decString text;
		int level = 0;
		
		int width = defren.GetWidth();
		int height = defren.GetHeight();
		
		text.Format( "refl_ssr_level0_%ix%i", width, height );
		renderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion( *defren.GetTextureTemporary2(), 0,
			text.GetString(), false, deoglDebugSaveTexture::ecColorLinear2sRGB );
		
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
			
			text.Format( "refl_ssr_level%i_%ix%i", level, width, height );
			renderThread.GetDebug().GetDebugSaveTexture().SaveTextureLevelConversion( *defren.GetTextureTemporary2(), level,
				text.GetString(), false, deoglDebugSaveTexture::ecColorLinear2sRGB );
		}
#endif
		
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	
	
	
	// prepare vbo for applying reflections
	int i, envMapCount = 0;
	deoglTexture *envMapEquis[ 8 ] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	deoglCubeMap *envMapCubes[ 8 ] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
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
	
	renderThread.GetShader().ActivateShader( pShaderApplyReflections );
	shader = pShaderApplyReflections->GetCompiled();
	
	defren.SetShaderParamFSQuad( *shader, sparQuadTCTransform );
	shader->SetParameterVector4( sparPosTransform, plan.GetDepthToPosition() );
	
	float envMapLodLevel = 1.0f;
	deoglEnvironmentMap * const envmapSky = plan.GetWorld()->GetSkyEnvironmentMap();
	if( envmapSky ){
		envMapLodLevel = ( float )envmapSky->GetSize() * 1.0f;
	}
	shader->SetParameterMatrix3x3( sparMatrixEnvMap, plan.GetRefPosCameraMatrix().GetRotationMatrix().Invert() );
	shader->SetParameterFloat( sparEnvMapLodLevel, envMapLodLevel );
	
	const float mmlpFactor1 = plan.GetProjectionMatrix().a11 * 0.5f;
	int height = defren.GetHeight();
	int width = defren.GetWidth();
	int mipmapMaxLevel = 0;
	while( width > 1 && height > 1 ){
		width >>= 1;
		if( width < 1 ){
			width = 1;
		}
		height >>= 1;
		if( height < 1 ){
			height = 1;
		}
		mipmapMaxLevel++;
	}
	
	shader->SetParameterFloat( sparMipMapLevelParams, mmlpFactor1 * ( float)defren.GetWidth(),
		mmlpFactor1 * ( float )defren.GetHeight(), ( float )( 1 << mipmapMaxLevel ) );
	shader->SetParameterFloat( sparMipMapTCClamp, defren.GetPixelSizeU() * ( float )( defren.GetWidth() - 1 ),
		defren.GetPixelSizeV() * ( float )( defren.GetHeight() - 1 ), 1.0f / defren.GetRealWidth(), 1.0f / defren.GetRealHeight() );
	
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
		tsmgr.EnableTexture( 0, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 1, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 2, *defren.GetTextureTemporary2(), GetSamplerClampLinear() );
		tsmgr.EnableTexture( 3, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 4, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *defren.GetTextureRoughness(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 6, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		
	}else{
		tsmgr.EnableTexture( 0, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 1, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 2, *defren.GetTextureTemporary2(), GetSamplerClampLinear() );
		tsmgr.EnableTexture( 3, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 4, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 6, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
	}
	
	tsmgr.EnableTexture( 7, *defren.GetTextureTemporary1(), GetSamplerClampLinearMipMap() );
	
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
	
	defren.RenderFSQuadVAO();
	DEBUG_PRINT_TIMER( "Reflection ScreenSpace: Apply Reflections" );
}



// Private Functions
//////////////////////

void deoglRenderReflection::pCleanUp(){
	if( pFBOIndexPass2 ){
		delete pFBOIndexPass2;
	}
	if( pFBOIndexPass1 ){
		delete pFBOIndexPass1;
	}
	
	if( pTextureDistance2 ){
		delete pTextureDistance2;
	}
	if( pTextureDistance1 ){
		delete pTextureDistance1;
	}
	if( pTextureIndices ){
		delete pTextureIndices;
	}
	
	if( pEnvMapEqui ){
		delete pEnvMapEqui;
	}
	if( pEnvMap ){
		delete pEnvMap;
	}
	
	if( pShaderEnvMapMask ){
		pShaderEnvMapMask->RemoveUsage();
	}
	if( pShaderBuildEnvMap ){
		pShaderBuildEnvMap->RemoveUsage();
	}
	if( pShaderCubeMap2EquiMap ){
		pShaderCubeMap2EquiMap->RemoveUsage();
	}
	if( pShaderReflection ){
		pShaderReflection->RemoveUsage();
	}
	
	if( pShaderApplyReflections ){
		pShaderApplyReflections->RemoveUsage();
	}
	if( pShaderScreenSpace ){
		pShaderScreenSpace->RemoveUsage();
	}
	if( pShaderMinMaxMipMapMax ){
		pShaderMinMaxMipMapMax->RemoveUsage();
	}
	if( pShaderMinMaxMipMapMin ){
		pShaderMinMaxMipMapMin->RemoveUsage();
	}
	
	if( pShaderMinMaxMipMapDownsample ){
		pShaderMinMaxMipMapDownsample->RemoveUsage();
	}
	if( pShaderMinMaxMipMapInitial ){
		pShaderMinMaxMipMapInitial->RemoveUsage();
	}
	
	if( pShaderCopyColorMipMap ){
		pShaderCopyColorMipMap->RemoveUsage();
	}
	if( pShaderCopyColor ){
		pShaderCopyColor->RemoveUsage();
	}
	
	/*
	if( pShaderRenderEnvMaps ){
		pShaderRenderEnvMaps->RemoveUsage();
	}
	*/
	if( pShaderIndexPass2 ){
		pShaderIndexPass2->RemoveUsage();
	}
	if( pShaderIndexPass1 ){
		pShaderIndexPass1->RemoveUsage();
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
