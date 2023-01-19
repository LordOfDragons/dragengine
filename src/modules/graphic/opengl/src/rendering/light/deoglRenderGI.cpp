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

#include "deoglRenderLight.h"
#include "deoglRenderGI.h"
#include "../deoglRenderWorld.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglRenderTaskTexture.h"
#include "../task/deoglAddToRenderTaskGIMaterial.h"
#include "../task/deoglRenderTaskPipeline.h"
#include "../task/shared/deoglRenderTaskSharedInstance.h"
#include "../task/shared/deoglRenderTaskSharedTexture.h"
#include "../task/shared/deoglRenderTaskSharedVAO.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../debug/deoglDebugSaveTexture.h"
#include "../../debug/deoglDebugTraceGroup.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../gi/deoglGI.h"
#include "../../gi/deoglGIBVH.h"
#include "../../gi/deoglGICascade.h"
#include "../../gi/deoglGITraceRays.h"
#include "../../gi/deoglGIState.h"
#include "../../gi/deoglGIMaterials.h"
#include "../../model/deoglModelLOD.h"
#include "../../model/deoglRModel.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTBufferObject.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/deoglSPBParameter.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/deoglImageStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../tbo/deoglDynamicTBOFloat16.h"
#include "../../tbo/deoglDynamicTBOFloat32.h"
#include "../../tbo/deoglDynamicTBOUInt16.h"
#include "../../tbo/deoglDynamicTBOUInt32.h"
#include "../../vao/deoglVAO.h"
#include "../../vbo/deoglSharedVBOBlock.h"
#include "../../world/deoglRWorld.h"
#include "../../utils/collision/deoglDCollisionFrustum.h"

#include <dragengine/common/exceptions.h>


// Definitions
////////////////

enum eUBORenderLightParameters{
	euprlMatrix,
	euprlMatrixNormal,
	euprlProbeCount,
	euprlOcclusionMapSize,
	euprlProbeClamp,
	euprlDistanceMapSize,
	euprlProbeSpacing,
	euprlProbeSpacingInv,
	euprlNormalBias,
	euprlPositionClamp,
	euprlOcclusionMapScale,
	euprlDistanceMapScale,
	euprlGridCoordShift,
	euprlIrradianceGamma,
	euprlSelfShadowBias,
	euprlGridOrigin,
	euprlGridCoordUnshift
};

enum eSPMaterial{
	espmQuadParams
};

enum eSPDebugProbe{
	spdpMatrixNormal,
	spdpMatrixMV,
	spdpMatrixMVP,
	spdpMatrixP,
	spdpGIDebugCascade
};

enum eSPDebugProbeOffset{
	spdpoMatrixMVP,
	spdpoGIDebugCascade
};

enum eSPDebugProbeUpdate{
	spdpuPosTransform,
	spdpuTCTransform,
	spdpuGIDebugCascade,
	spdpuParams,
	spdpuPlaneLeft,
	spdpuPlaneRight,
	spdpuPlaneTop,
	spdpuPlaneBottom,
	spdpuPlaneNear
};



// Class deoglRenderGI
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderGI::deoglRenderGI( deoglRenderThread &renderThread ) :
deoglRenderLightBase( renderThread )
{
	const bool renderFSQuadStereoVSLayer = renderThread.GetChoices().GetRenderFSQuadStereoVSLayer();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	deoglPipelineConfiguration pipconf, pipconf2;
	deoglShaderDefines defines, commonDefines;
	const deoglShaderSources *sources;
	
	try{
		renderThread.GetShader().SetCommonDefines( commonDefines );
		
		pCreateUBORenderLight();
		
		
		// resize materials
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, true, false );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen GI Resize Materials" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineResizeMaterials = pipelineManager.GetWith( pipconf );
		
		
		// trace rays
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, true, false );
		pipconf.SetEnableScissorTest( true );
		
		defines.SetDefine( "GI_CLEAR_PROBES_COUNT", ( 32 * 32 * 8 ) / 32 / 4 );
		
		#ifdef GI_RENDERDOC_DEBUG
		defines.SetDefines( "GI_RENDERDOC_DEBUG" );
		#endif
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Trace Rays" );
		#ifdef GI_USE_RAY_CACHE
			defines.SetDefines( "GI_USE_RAY_CACHE" );
		#endif
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineTraceRays = pipelineManager.GetWith( pipconf );
		defines.RemoveDefine( "GI_USE_RAY_CACHE" );
		
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineTraceRaysCache = pipelineManager.GetWith( pipconf );
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Copy Ray Cache" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineCopyRayCache = pipelineManager.GetWith( pipconf );
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Init From Ray Cache" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineInitFromRayCache = pipelineManager.GetWith( pipconf );
		
		
		// clear probes
		pipconf.Reset();
		pipconf.SetType( deoglPipelineConfiguration::etCompute );
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Clear Probes" );
		defines.SetDefines( "MAP_IRRADIANCE" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineClearProbeIrradiance = pipelineManager.GetWith( pipconf );
		defines.RemoveDefine( "MAP_IRRADIANCE" );
		
		defines.SetDefines( "MAP_DISTANCE" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineClearProbeDistance = pipelineManager.GetWith( pipconf );
		defines.RemoveDefine( "MAP_DISTANCE" );
		
		
		// update probes
		pipconf.Reset();
		pipconf.SetType( deoglPipelineConfiguration::etCompute );
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Update Probes" );
		defines.SetDefines( "MAP_IRRADIANCE" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineUpdateProbeIrradiance = pipelineManager.GetWith( pipconf );
		defines.RemoveDefine( "MAP_IRRADIANCE" );
		
		defines.SetDefines( "MAP_DISTANCE" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineUpdateProbeDistance = pipelineManager.GetWith( pipconf );
		defines.RemoveDefine( "MAP_DISTANCE" );
		
		
		// probe dynamic states
		pipconf.Reset();
		pipconf.SetType( deoglPipelineConfiguration::etCompute );
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Probe Dynamic States" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineProbeDynamicStates = pipelineManager.GetWith( pipconf );
		
		
		// probe offset
		pipconf.Reset();
		pipconf.SetType( deoglPipelineConfiguration::etCompute );
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Probe Offset" );
		if( renderThread.GetChoices().GetGIMoveUsingCache() ){
			defines.SetDefines( "WITH_RAY_CACHE" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineProbeOffset = pipelineManager.GetWith( pipconf );
		defines.RemoveDefine( "WITH_RAY_CACHE" );
		
		
		// probe extends
		pipconf.Reset();
		pipconf.SetType( deoglPipelineConfiguration::etCompute );
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Probe Extends" );
		if( renderThread.GetChoices().GetGIMoveUsingCache() ){
			defines.SetDefines( "WITH_RAY_CACHE" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineProbeExtends = pipelineManager.GetWith( pipconf );
		defines.RemoveDefine( "WITH_RAY_CACHE" );
		
		
		// debug
		pipconf.Reset();
		
		pipconf2 = pipconf;
		pipconf2.SetEnableDepthTest( true );
		pipconf2.SetDepthFunc( renderThread.GetChoices().GetDepthCompareFuncRegular() );
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Debug Probe" );
		pipconf2.SetShader( renderThread, sources, defines );
		pPipelineDebugProbe = pipelineManager.GetWith( pipconf2 );
		
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineDebugProbeXRay = pipelineManager.GetWith( pipconf );
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Debug Probe Offset" );
		pipconf2.SetShader( renderThread, sources, defines );
		pPipelineDebugProbeOffset = pipelineManager.GetWith( pipconf2 );
		
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineDebugProbeOffsetXRay = pipelineManager.GetWith( pipconf );
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Debug Probe Update" );
		pipconf2.SetShader( renderThread, sources, defines );
		pPipelineDebugProbeUpdatePass1 = pipelineManager.GetWith( pipconf2 );
		
		defines.SetDefines( "PASS2" );
		pipconf2.SetShader( renderThread, sources, defines );
		pPipelineDebugProbeUpdatePass2 = pipelineManager.GetWith( pipconf2 );
		
		
		// render light
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, true, false );
		pipconf.SetEnableScissorTest( true );
		pipconf.EnableDynamicStencilTest();
		
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		sources = shaderManager.GetSourcesNamed( "DefRen Light GI" );
		pipconf.EnableBlendAdd();
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineLight = pipelineManager.GetWith( pipconf );
		
		pipconf.EnableBlendTranspAdd();
		pPipelineLightTransp = pipelineManager.GetWith( pipconf );
		
		// render light stereo
		defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen Light GI Stereo" );
		}
		pipconf.EnableBlendAdd();
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineLightStereo = pipelineManager.GetWith( pipconf );
		
		pipconf.EnableBlendTranspAdd();
		pPipelineLightTranspStereo = pipelineManager.GetWith( pipconf );
		
		
		// render light gi ray
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, true, false );
		pipconf.EnableBlendAdd();
		
		sources = shaderManager.GetSourcesNamed( "DefRen Light GI" );
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD", "GI_RAY" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineLightGIRay = pipelineManager.GetWith( pipconf );
		
		
		// debug information
		const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
		const decColor colorBg( 0.0f, 0.0f, 0.25f, 0.75f );
		const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
		
		pDebugInfoGI.TakeOver( new deoglDebugInformation( "GI", colorText, colorBg ) );
		
		pDebugInfoGITraceRays.TakeOver( new deoglDebugInformation( "Trace Rays", colorText, colorBgSub ) );
		pDebugInfoGI->GetChildren().Add( pDebugInfoGITraceRays );
		
		pDebugInfoGIRenderMaterials.TakeOver( new deoglDebugInformation( "Render Materials", colorText, colorBgSub ) );
		pDebugInfoGI->GetChildren().Add( pDebugInfoGIRenderMaterials );
		
		pDebugInfoGIClearProbes.TakeOver( new deoglDebugInformation( "Clear Probes", colorText, colorBgSub ) );
		pDebugInfoGI->GetChildren().Add( pDebugInfoGIClearProbes );
		
		pDebugInfoGIUpdateProbes.TakeOver( new deoglDebugInformation( "Update Probes", colorText, colorBgSub ) );
		pDebugInfoGI->GetChildren().Add( pDebugInfoGIUpdateProbes );
		
		pDebugInfoGIMoveProbes.TakeOver( new deoglDebugInformation( "Move Probes", colorText, colorBgSub ) );
		pDebugInfoGI->GetChildren().Add( pDebugInfoGIMoveProbes );
		
		pDebugInfoGIRenderLightGIRay.TakeOver( new deoglDebugInformation( "Light Rays", colorText, colorBgSub ) );
		pDebugInfoGI->GetChildren().Add( pDebugInfoGIRenderLightGIRay );
		
		pDebugInfoGIRenderLight.TakeOver( new deoglDebugInformation( "Light Geometry", colorText, colorBgSub ) );
		pDebugInfoGI->GetChildren().Add( pDebugInfoGIRenderLight );
		
		
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderGI::~deoglRenderGI(){
	pCleanUp();
}



// Rendering
//////////////

// #define DO_BVH_TIMING

#ifdef DO_BVH_TIMING
	#define BVH_TIMING_INIT decTimer timerBvh;
	#define BVH_TIMING_PRINT(t) renderThread.GetLogger().LogInfoFormat(t ": %d", (int)(timerBvh.GetElapsedTime() * 1e6f));
#else
	#define BVH_TIMING_INIT
	#define BVH_TIMING_PRINT(t)
#endif

void deoglRenderGI::TraceRays( deoglRenderPlan &plan ){
	deoglGIState * const giState = plan.GetUpdateGIState();
	if( ! giState ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GI.TraceRays" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const deoglGICascade &cascade = giState->GetActiveCascade();
	deoglGI &gi = renderThread.GetGI();
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Reset( plan, true );
	}
	
	// if any ray caches are invalid update them
	#ifdef GI_USE_RAY_CACHE
	if( cascade.GetRayCacheProbeCount() > 0 ){
		deoglGIBVH &bvh = giState->GetBVHStatic();
		
		if( bvh.GetDirty() ){
			bvh.GetRenderTaskMaterial().Clear();
			
			bvh.Clear();
				BVH_TIMING_INIT
			bvh.AddComponents( plan, giState->GetInstances(), false );
				BVH_TIMING_PRINT("Cache BVH Add Components")
			bvh.BuildBVH();
				BVH_TIMING_PRINT("Cache BVH Build")
			
			giState->PrepareUBOStateRayCache(); // has to be done here since it is shared
			
			RenderMaterials( plan, bvh.GetRenderTaskMaterial() );
			
		}else{
			giState->PrepareUBOStateRayCache();
		}
		
		deoglDebugTraceGroup debugTraceCacheTrace( renderThread, "GI.TraceRays.CacheTraceRays" );
		pPipelineTraceRaysCache->Activate();
		pSharedTraceRays( plan );
		pClearTraceRays();
		pActivateGIUBOs();
		pInitTraceTextures( bvh );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		
		// copy traced rays to cache
		deoglDebugTraceGroup debugTraceCachStore( debugTraceCacheTrace, "GI.TraceRays.TraceRays.CacheStore" );
		deoglGIRayCache &rayCache = giState->GetRayCache();
		pPipelineCopyRayCache->Activate();
		renderThread.GetFramebuffer().Activate( &rayCache.GetFBOResult() );
		
		SetViewport( rayCache.GetTextureDistance().GetSize() );
		
		pActivateGIUBOs();
		
		deoglGITraceRays &traceRays = gi.GetTraceRays();
		tsmgr.EnableTexture( 0, traceRays.GetTexturePosition(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 1, traceRays.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 2, traceRays.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 3, traceRays.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 4, traceRays.GetTextureLight(), GetSamplerClampNearest() );
		tsmgr.DisableStagesAbove( 4 );
		
		OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_TRIANGLE_FAN, 0, 4, cascade.GetRayCacheProbeCount() ) );
		
		giState->ValidatedRayCaches(); // comment out for performance test
	}
	#endif
	
	// trace rays
	deoglDebugTraceGroup debugTraceTraceRays( renderThread, "GI.TraceRays.TraceRays" );
	deoglGIBVH &bvh = giState->GetBVHDynamic();
	bvh.GetRenderTaskMaterial().Clear();
	
	bvh.Clear();
		BVH_TIMING_INIT
	#ifdef GI_USE_RAY_CACHE
		bvh.AddComponents( plan, giState->GetInstances(), true );
	#else
		bvh.AddComponents( plan, giState->GetInstances() );
	#endif
		BVH_TIMING_PRINT("Frame BVH Add Components")
	bvh.BuildBVH();
		BVH_TIMING_PRINT("Frame BVH Build")
	//bvh.DebugPrint( giState.GetPosition() );
	
	giState->PrepareUBOState(); // has to be done here since it is shared
	
	RenderMaterials( plan, bvh.GetRenderTaskMaterial() );
	pSharedTraceRays( plan );
	pClearTraceRays();
	
	#ifdef GI_USE_RAY_CACHE
		deoglDebugTraceGroup debugTrace3( renderThread, "GI.TraceRays.RestoreCache" );
		pPipelineInitFromRayCache->Activate();
		pActivateGIUBOs();
		
		deoglGIRayCache &rayCache = giState->GetRayCache();
		tsmgr.EnableArrayTexture( 0, rayCache.GetTextureDistance(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 1, rayCache.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 2, rayCache.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 3, rayCache.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 4, rayCache.GetTextureLight(), GetSamplerClampNearest() );
		tsmgr.DisableStagesAbove( 4 );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		debugTrace3.Close();
	#endif
	
	pPipelineTraceRays->Activate();
	pActivateGIUBOs();
	pInitTraceTextures( bvh );
	
	#ifdef GI_USE_RAY_CACHE
		tsmgr.EnableArrayTexture( 12, rayCache.GetTextureDistance(), GetSamplerClampNearest() );
	#endif
	
	#ifdef GI_RENDERDOC_DEBUG
		SetViewport( 512, 256 );
	#endif
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	// clean up
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	tsmgr.DisableAllStages();
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Sample( plan, *pDebugInfoGITraceRays, true );
	}
}

void deoglRenderGI::PrepareUBORenderLight( deoglRenderPlan &plan ){
	const deoglGIState * const giState = plan.GetRenderGIState();
	if( ! giState ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = giState->GetCascadeCount();
	int i;
	
	deoglSPBlockUBO &ubo = GetUBORenderLight();
	const deoglSPBMapBuffer mapped( ubo );
	
	for( i=0; i<count; i++ ){
		const deoglGICascade &cascade = giState->GetCascadeAt( i );
		
		const decDVector position( cascade.GetPosition() + cascade.GetFieldOrigin() );
		const decDMatrix matrix( plan.GetInverseCameraMatrix() * decDMatrix::CreateTranslation( -position ) );
		
		ubo.SetParameterDataMat4x3( euprlMatrix, i, matrix );
		ubo.SetParameterDataMat3x3( euprlMatrixNormal, i, matrix.GetRotationMatrix().QuickInvert() );
		
		ubo.SetParameterDataIVec3( euprlProbeCount, i, giState->GetProbeCount() );
		ubo.SetParameterDataIVec3( euprlProbeClamp, i, giState->GetGridCoordClamp() );
		ubo.SetParameterDataVec3( euprlProbeSpacing, i, cascade.GetProbeSpacing() );
		ubo.SetParameterDataVec3( euprlProbeSpacingInv, i, cascade.GetProbeSpacingInverse() );
		ubo.SetParameterDataVec3( euprlPositionClamp, i, cascade.GetPositionClamp() );
		ubo.SetParameterDataIVec3( euprlGridCoordShift, i, giState->GetProbeCount() - cascade.GetGridCoordShift() );
		
		ubo.SetParameterDataInt( euprlOcclusionMapSize, i, giState->GetIrradianceMapSize() );
		ubo.SetParameterDataVec2( euprlOcclusionMapScale, i, giState->GetIrradianceMapScale() );
		ubo.SetParameterDataInt( euprlDistanceMapSize, i, giState->GetDistanceMapSize() );
		ubo.SetParameterDataVec2( euprlDistanceMapScale, i, giState->GetDistanceMapScale() );
		ubo.SetParameterDataFloat( euprlNormalBias, i, giState->GetNormalBias() );
		ubo.SetParameterDataFloat( euprlIrradianceGamma, i, giState->GetIrradianceGamma() );
		ubo.SetParameterDataFloat( euprlSelfShadowBias, i, cascade.CalcUBOSelfShadowBias() );
		ubo.SetParameterDataVec3( euprlGridOrigin, i, cascade.GetFieldOrigin() );
		ubo.SetParameterDataIVec3( euprlGridCoordUnshift, i, cascade.GetGridCoordShift() );
	}
}

void deoglRenderGI::PrepareUBORenderLight( const deoglGIState &giState, const decDVector &position ){
	const int count = giState.GetCascadeCount();
	int i;
	
	deoglSPBlockUBO &ubo = GetUBORenderLight();
	const deoglSPBMapBuffer mapped( ubo );
	
	for( i=0; i<count; i++ ){
		const deoglGICascade &cascade = giState.GetCascadeAt( i );
		
		const decDVector cascadePosition( cascade.GetPosition() + cascade.GetFieldOrigin() );
		const decMatrix matrix( decMatrix::CreateTranslation( ( position - cascadePosition ).ToVector() ) );
		
		ubo.SetParameterDataMat4x3( euprlMatrix, i, matrix );
		ubo.SetParameterDataMat3x3( euprlMatrixNormal, i, decMatrix() );
		
		ubo.SetParameterDataIVec3( euprlProbeCount, i, giState.GetProbeCount() );
		ubo.SetParameterDataIVec3( euprlProbeClamp, i, giState.GetGridCoordClamp() );
		ubo.SetParameterDataVec3( euprlProbeSpacing, i, cascade.GetProbeSpacing() );
		ubo.SetParameterDataVec3( euprlProbeSpacingInv, i, cascade.GetProbeSpacingInverse() );
		ubo.SetParameterDataVec3( euprlPositionClamp, i, cascade.GetPositionClamp() );
		ubo.SetParameterDataIVec3( euprlGridCoordShift, i, giState.GetProbeCount() - cascade.GetGridCoordShift() );
		
		ubo.SetParameterDataInt( euprlOcclusionMapSize, i, giState.GetIrradianceMapSize() );
		ubo.SetParameterDataVec2( euprlOcclusionMapScale, i, giState.GetIrradianceMapScale() );
		ubo.SetParameterDataInt( euprlDistanceMapSize, i, giState.GetDistanceMapSize() );
		ubo.SetParameterDataVec2( euprlDistanceMapScale, i, giState.GetDistanceMapScale() );
		ubo.SetParameterDataFloat( euprlNormalBias, i, giState.GetNormalBias() );
		ubo.SetParameterDataFloat( euprlIrradianceGamma, i, giState.GetIrradianceGamma() );
		ubo.SetParameterDataFloat( euprlSelfShadowBias, i, cascade.CalcUBOSelfShadowBias() );
		ubo.SetParameterDataVec3( euprlGridOrigin, i, cascade.GetFieldOrigin() );
		ubo.SetParameterDataIVec3( euprlGridCoordUnshift, i, cascade.GetGridCoordShift() );
	}
}

void deoglRenderGI::RenderMaterials( deoglRenderPlan &plan, const deoglRenderTask &renderTask ){
	deoglGIState * const giState = plan.GetUpdateGIState();
	if( ! giState ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GI.RenderMaterials" );
	deoglGI &gi = renderThread.GetGI();
	
	const int pipelineCount = renderTask.GetPipelineCount();
	if( pipelineCount == 0 ){
		return;
	}
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglGIMaterials &materials = gi.GetMaterials();
	const int width = materials.GetTextureDiffuse().GetWidth();
	const int height = materials.GetTextureDiffuse().GetHeight();
	int i, j;
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Reset( plan, true );
	}
	
	renderThread.GetFramebuffer().Activate( &materials.GetFBOMaterial() );
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	SetViewport( width, height );
	
	const int mapsPerRow = materials.GetMaterialsPerRow();
	const int rowsPerImage = materials.GetRowsPerImage();
	const float scaleU = 1.0f / ( float )mapsPerRow;
	const float scaleV = 1.0f / ( float )rowsPerImage;
	const float offsetScaleU = 2.0f / ( float )mapsPerRow;
	const float offsetScaleV = 2.0f / ( float )rowsPerImage;
	const float offsetBaseU = offsetScaleU * 0.5f - 1.0f;
	const float offsetBaseV = offsetScaleV * 0.5f - 1.0f;
	
	for( i=0; i<pipelineCount; i++ ){
		const deoglRenderTaskPipeline &rtpipeline = *renderTask.GetPipelineAt( i );
		rtpipeline.GetPipeline()->Activate();
		
		const deoglShaderCompiled &shader = rtpipeline.GetPipeline()->GetGlShader();
		
		const int textureCount = rtpipeline.GetTextureCount();
		for( j=0; j<textureCount; j++ ){
			const deoglRenderTaskTexture &rttexture = *rtpipeline.GetTextureAt( j );
			const deoglTexUnitsConfig &tuc = *rttexture.GetTexture()->GetTUC();
			const int matMapIndex = tuc.GetMaterialIndex();
			if( matMapIndex < 1 ){ // no slot (-1), fallback slot (0)
				continue;
			}
			
			tuc.Apply();
			
			const decPoint matMapCoord( matMapIndex % mapsPerRow, matMapIndex / mapsPerRow );
			
			const float offsetU = offsetScaleU * ( float )matMapCoord.x + offsetBaseU;
			const float offsetV = offsetScaleV * ( float )matMapCoord.y + offsetBaseV;
			
			shader.SetParameterFloat( espmQuadParams, scaleU, scaleV, offsetU, offsetV );
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		}
	}
	
	// clean up
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	renderThread.GetTexture().GetStages().DisableAllStages();
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Sample( plan, *pDebugInfoGIRenderMaterials, true );
	}
	
// 	materials.DEBUG();
}

void deoglRenderGI::ResizeMaterials( deoglTexture &texDiffuse, deoglTexture &texReflectivity,
deoglTexture &texEmissivity, int mapsPerRow, int rowsPerImage ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GI.ResizeMaterials" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglGIMaterials &materials = renderThread.GetGI().GetMaterials();
	
	pPipelineResizeMaterials->Activate();
	renderThread.GetFramebuffer().Activate( &materials.GetFBOMaterial() );
	
	SetViewport( materials.GetMaterialMapSize() * materials.GetMaterialsPerRow(),
		materials.GetMaterialMapSize() * materials.GetRowsPerImage() );
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	deoglShaderCompiled &shader = pPipelineResizeMaterials->GetGlShader();
	
	shader.SetParameterInt( 0, materials.GetMaterialsPerRow(),
		materials.GetRowsPerImage(), mapsPerRow, rowsPerImage );
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	tsmgr.EnableTexture( 0, texDiffuse, GetSamplerClampLinear() );
	tsmgr.EnableTexture( 1, texReflectivity, GetSamplerClampLinear() );
	tsmgr.EnableTexture( 2, texEmissivity, GetSamplerClampLinear() );
	tsmgr.DisableStagesAbove( 2 );
	
	OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_TRIANGLE_FAN, 0, 4, mapsPerRow * rowsPerImage ) );
	
	// clean up
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	renderThread.GetTexture().GetStages().DisableAllStages();
}

void deoglRenderGI::ClearProbes( deoglRenderPlan &plan ){
	deoglGIState * const giState = plan.GetUpdateGIState();
	if( ! giState  ){
		DETHROW( deeInvalidParam );
	}
	
	deoglGICascade &cascade = giState->GetActiveCascade();
	if( ! cascade.HasClearProbes() ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GI.ClearProbes" );
	deoglImageStageManager &ismgr = renderThread.GetTexture().GetImageStages();
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Reset( plan, true );
	}
	
	giState->PrepareUBOClearProbes();
	
	// clear probes: irradiance map
	pPipelineClearProbeIrradiance->Activate();
	
	ismgr.DisableAllStages();
	ismgr.Enable( 0, giState->GetTextureProbeIrradiance(), 0, deoglImageStageManager::eaWrite );
	
	pActivateGIUBOs();
	giState->GetUBOClearProbes().Activate();
	
	OGL_CHECK( renderThread, pglDispatchCompute( giState->GetRealProbeCount(), 1, 1 ) );
	
	// clear probes: distance map
	pPipelineClearProbeDistance->Activate();
	
	ismgr.Enable( 0, giState->GetTextureProbeDistance(), 0, deoglImageStageManager::eaWrite );
	
	pActivateGIUBOs();
	giState->GetUBOClearProbes().Activate();
	
	OGL_CHECK( renderThread, pglDispatchCompute( giState->GetRealProbeCount(), 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_TEXTURE_FETCH_BARRIER_BIT ) );
	
	// clean up
	cascade.ClearClearProbes();
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Sample( plan, *pDebugInfoGIClearProbes, true );
	}
}

void deoglRenderGI::UpdateProbes( deoglRenderPlan &plan ){
	deoglGIState * const giState = plan.GetUpdateGIState();
	if( ! giState ){
		DETHROW( deeInvalidParam );
	}
	
	const deoglGICascade &cascade = giState->GetActiveCascade();
	if( cascade.GetUpdateProbeCount() == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GI.UpdateProbes" );
	deoglImageStageManager &ismgr = renderThread.GetTexture().GetImageStages();
	deoglGITraceRays &traceRays = renderThread.GetGI().GetTraceRays();
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Reset( plan, true );
	}
	
	// update irradiance probes. required 1 quadrant to be processed
	pPipelineUpdateProbeIrradiance->Activate();
	
	ismgr.DisableAllStages();
	ismgr.Enable( 0, traceRays.GetTexturePosition(), 0, deoglImageStageManager::eaRead );
	ismgr.Enable( 1, traceRays.GetTextureNormal(), 0, deoglImageStageManager::eaRead );
	ismgr.Enable( 2, traceRays.GetTextureLight(), 0, deoglImageStageManager::eaRead );
	ismgr.Enable( 3, giState->GetTextureProbeIrradiance(), 0, deoglImageStageManager::eaReadWrite );
	
	pActivateGIUBOs();
	
	OGL_CHECK( renderThread, pglDispatchCompute( cascade.GetUpdateProbeCount(), 1, 1 ) );
	
	// update distance probes. requires 4 quadrants to be processed
	pPipelineUpdateProbeDistance->Activate();
	
	ismgr.Enable( 3, giState->GetTextureProbeDistance(), 0, deoglImageStageManager::eaReadWrite );
	
	pActivateGIUBOs();
	
	OGL_CHECK( renderThread, pglDispatchCompute( cascade.GetUpdateProbeCount(), 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT ) );
	
	// clean up
	
	ismgr.DisableAllStages();
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Sample( plan, *pDebugInfoGIUpdateProbes, true );
	}
}

void deoglRenderGI::ProbeOffset( deoglRenderPlan &plan ){
	deoglGIState * const giState = plan.GetUpdateGIState();
	if( ! giState ){
		return;
	}
	
	const deoglGICascade &cascade = giState->GetActiveCascade();
	if( cascade.GetUpdateProbeCount() == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GI.ProbeOffset" );
	deoglImageStageManager &ismgr = renderThread.GetTexture().GetImageStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Reset( plan, true );
	}
	
	
	// calculate dynamic states into temporary SSBO to reduce shared memory usage
	pPipelineProbeDynamicStates->Activate();
	
	pActivateGIUBOs();
	
	const deoglGITraceRays &traceRays = renderThread.GetGI().GetTraceRays();
	ismgr.Enable( 0, traceRays.GetTexturePosition(), 0, deoglImageStageManager::eaRead );
	ismgr.Enable( 1, traceRays.GetTextureNormal(), 0, deoglImageStageManager::eaRead );
	ismgr.DisableStagesAbove( 1 );
	
	giState->GetPBProbeDynamicStates()->Activate();
	
	OGL_CHECK( renderThread, pglDispatchCompute( cascade.GetUpdateProbeCount(), 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_UNIFORM_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT ) );
	
	
	// calculate new offset and state
	pPipelineProbeOffset->Activate();
	
	pActivateGIUBOs();
	
	if( renderThread.GetChoices().GetGIMoveUsingCache() ){
		const deoglGIRayCache &rayCache = giState->GetRayCache();
		ismgr.Enable( 2, rayCache.GetTextureDistance(), 0, deoglImageStageManager::eaRead );
		ismgr.Enable( 3, rayCache.GetTextureNormal(), 0, deoglImageStageManager::eaRead );
	}
	
	ismgr.Enable( 4, giState->GetTextureProbeOffset(), 0, deoglImageStageManager::eaWrite );
	
	giState->GetPBProbeOffsets()->Activate();
	giState->GetPBProbeDynamicStates()->Activate( 1 );
	
	OGL_CHECK( renderThread, pglDispatchCompute( cascade.GetUpdateProbeCount(), 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_UNIFORM_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT ) );
	
	giState->GetPBProbeDynamicStates()->Deactivate( 1 );
	giState->GetPBProbeOffsets()->Deactivate();
	
	giState->ProbesMoved(); // tell state probes moved so it can read it later without stalling
	
	
	// clean up
	ismgr.DisableAllStages();
	
	defren.ActivatePostProcessFBO( true );
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Sample( plan, *pDebugInfoGIMoveProbes, true );
	}
}

void deoglRenderGI::ProbeExtends( deoglRenderPlan &plan ){
	deoglGIState * const giState = plan.GetUpdateGIState();
	if( ! giState ){
		return;
	}
	
	const deoglGICascade &cascade = giState->GetActiveCascade();
	if( cascade.GetRayCacheProbeCount() == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GI.ProbeExtends" );
	deoglImageStageManager &ismgr = renderThread.GetTexture().GetImageStages();
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Reset( plan, true );
	}
	
	pPipelineProbeExtends->Activate();
	pActivateGIUBOs();
	
	#ifdef GI_USE_RAY_CACHE
		ismgr.Enable( 0, giState->GetRayCache().GetTextureDistance(), 0, deoglImageStageManager::eaRead );
	#else
		ismgr.Enable( 0, renderThread.GetGI().GetTraceRays().GetTexturePosition(), 0, deoglImageStageManager::eaRead );
	#endif
	ismgr.DisableStagesAbove( 0 );
	
	giState->GetPBProbeExtends()->Activate();
	
	OGL_CHECK( renderThread, pglDispatchCompute( cascade.GetRayCacheProbeCount(), 1, 1 ) );
	OGL_CHECK( renderThread, pglMemoryBarrier( GL_UNIFORM_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT ) );
	
	giState->GetPBProbeExtends()->Deactivate();
}

void deoglRenderGI::RenderLight( deoglRenderPlan &plan, bool solid ){
	deoglGIState * const giState = plan.GetRenderGIState();
	DEASSERT_NOTNULL( giState )
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GI.RenderLight" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Reset( plan, true );
	}
	
	const deoglPipeline &pipeline = solid
		? ( plan.GetRenderStereo() ? *pPipelineLightStereo : *pPipelineLight )
		: ( plan.GetRenderStereo() ? *pPipelineLightTranspStereo : *pPipelineLightTransp );
	pipeline.Activate();
	
	RestoreFBO( plan );
	SetViewport( plan );
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	pUBORenderLight->Activate();
	
	tsmgr.EnableArrayTexture( 6, giState->GetTextureProbeIrradiance(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 7, giState->GetTextureProbeDistance(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 8, giState->GetTextureProbeOffset(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 8 );
	
	RenderFullScreenQuadVAO( plan );
	
	// clean up
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Sample( plan, *pDebugInfoGIRenderLight, true );
	}
}

void deoglRenderGI::RenderLightGIRay( deoglRenderPlan &plan ){
	deoglGIState * const giStateRender = plan.GetRenderGIState();
	deoglGIState * const giStateUpdate = plan.GetUpdateGIState();
	DEASSERT_NOTNULL( giStateRender )
	DEASSERT_NOTNULL( giStateUpdate )
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GI.RenderLightGIRay" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Reset( plan, true );
	}
	
	pPipelineLightGIRay->Activate();
	RestoreFBOGITraceRays( *giStateUpdate );
	
	// WARNING always non-stereo!
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	pUBORenderLight->Activate();
	
	tsmgr.EnableArrayTexture( 6, giStateRender->GetTextureProbeIrradiance(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 7, giStateRender->GetTextureProbeDistance(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 8, giStateRender->GetTextureProbeOffset(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 8 );
	
	RenderFullScreenQuadVAO();
	
	// clean up
	if( pDebugInfoGI->GetVisible() ){
		DebugTimer1Sample( plan, *pDebugInfoGIRenderLightGIRay, true );
	}
}

void deoglRenderGI::RenderDebugOverlay( deoglRenderPlan &plan ){
	deoglGIState * const giState = plan.GetRenderGIState();
	if( ! giState ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GI.RenderDebugOverlay" );
	const deoglDeveloperMode &devmode = renderThread.GetDebug().GetDeveloperMode();
	if( ! devmode.GetEnabled() || ( ! devmode.GetGIShowProbes() && ! devmode.GetGIShowProbeUpdate() ) ){
		return;
	}
	
	const deoglGICascade &cascade = giState->GetCascadeAt( devmode.GetGIShowCascade() );
	
	const decPoint3 &probeCount = giState->GetProbeCount();
	const decDMatrix matrixC( decDMatrix::CreateTranslation( cascade.GetPosition()
		+ decDVector( cascade.GetFieldOrigin() ) ) * plan.GetCameraMatrix() );
	const decMatrix &matrixP = plan.GetProjectionMatrix();
	const decDMatrix matrixCP( matrixC * decDMatrix( matrixP ) );
	const decDMatrix matrixNormal( matrixC.GetRotationMatrix() ); // transposed to simplify shader
	const bool xray = false;
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	defren.ActivatePostProcessFBO( true );
	
	// probe
	if( devmode.GetGIShowProbes() ){
		const deoglPipeline &pipeline = xray ? *pPipelineDebugProbeXRay : *pPipelineDebugProbe;
		pipeline.Activate();
		OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
		
		deoglShaderCompiled &shaderProbe = pipeline.GetGlShader();
		shaderProbe.SetParameterDMatrix4x3( spdpMatrixNormal, matrixNormal );
		shaderProbe.SetParameterDMatrix4x3( spdpMatrixMV, matrixC );
		shaderProbe.SetParameterDMatrix4x4( spdpMatrixMVP, matrixCP );
		shaderProbe.SetParameterDMatrix4x4( spdpMatrixP, matrixP );
		shaderProbe.SetParameterInt( spdpGIDebugCascade, cascade.GetIndex() );
		
		pUBORenderLight->Activate();
		
		tsmgr.EnableArrayTexture( 0, giState->GetTextureProbeIrradiance(), GetSamplerClampLinear() );
		tsmgr.EnableArrayTexture( 1, giState->GetTextureProbeDistance(), GetSamplerClampLinear() );
		tsmgr.EnableArrayTexture( 2, giState->GetTextureProbeOffset(), GetSamplerClampNearest() );
		tsmgr.DisableStagesAbove( 2 );
		
		OGL_CHECK( renderThread, pglDrawArraysInstanced(
			GL_TRIANGLE_FAN, 0, 4, probeCount.x * probeCount.y * probeCount.z ) );
	}
	
	// offset
	if( devmode.GetGIShowProbes() && devmode.GetGIShowProbeOffsets() ){
		const deoglPipeline &pipeline = xray ? *pPipelineDebugProbeXRay : *pPipelineDebugProbe;
		pipeline.Activate();
		OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
		
		deoglShaderCompiled &shaderOffset = pipeline.GetGlShader();
		shaderOffset.Activate();
		shaderOffset.SetParameterDMatrix4x4( spdpoMatrixMVP, matrixCP );
		shaderOffset.SetParameterInt( spdpoGIDebugCascade, cascade.GetIndex() );
		
		pUBORenderLight->Activate();
		
		tsmgr.EnableArrayTexture( 0, giState->GetTextureProbeOffset(), GetSamplerClampNearest() );
		tsmgr.DisableStagesAbove( 0 );
		
		OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_LINES, 0, 2, probeCount.x * probeCount.y * probeCount.z ) );
	}
	
	// update
	if( devmode.GetGIShowProbeUpdate() ){
		const decPoint3 &count = giState->GetProbeCount();
		const int probeSize = 2;
		const int probeSpacing = 1;
		const int groupSpacing = 4;
		const decPoint position( 5, 300 );
		const decPoint size(
			( probeSize * count.x + probeSpacing * ( count.x - 1 ) ) * count.y
				+ groupSpacing * ( count.y - 1 ),
			( probeSize * count.z + probeSpacing * ( count.z - 1 ) ) );
		
		const decVector2 scale( 1.0f / ( float )plan.GetViewportWidth(), 1.0f / ( float )plan.GetViewportHeight() );
		const decVector2 offset( scale.x * size.x - 1.0f, scale.y * size.y - 1.0f );
		
		// pass 1
		pPipelineDebugProbeUpdatePass1->Activate();
		
		OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
		
		tsmgr.EnableArrayTexture( 0, giState->GetTextureProbeOffset(), GetSamplerClampNearest() );
		tsmgr.DisableStagesAbove( 0 );
		
		OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
		
		deoglShaderCompiled &shader2a = pPipelineDebugProbeUpdatePass1->GetGlShader();
		shader2a.SetParameterFloat( spdpuPosTransform, scale.x * size.x, scale.y * size.y,
			scale.x * position.x * 2.0f + offset.x, scale.y * position.y * 2.0f + offset.y );
		shader2a.SetParameterFloat( spdpuTCTransform, ( float )size.x * 0.5f, ( float )size.y * -0.5f,
			( float )size.x * 0.5, ( float )size.y * 0.5f );
		shader2a.SetParameterInt( spdpuGIDebugCascade, cascade.GetIndex() );
		shader2a.SetParameterInt( spdpuParams, probeSize, probeSpacing, groupSpacing );
		
		const deoglDCollisionFrustum &frustum = *plan.GetUseFrustum();
		
		const decDVector &fnleft = frustum.GetLeftNormal();
		const decDVector &fntop = frustum.GetTopNormal();
		const decDVector &fnright = frustum.GetRightNormal();
		const decDVector &fnbottom = frustum.GetBottomNormal();
		const decDVector &fnnear = frustum.GetNearNormal();
		
		const double fpshift = 0.5 * cascade.GetProbeSpacing().Length();
		const decDVector &fmove = cascade.GetPosition();
		
		const double fdleft = frustum.GetLeftDistance() - fnleft * fmove - fpshift;
		const float fdtop = frustum.GetTopDistance() - fntop * fmove - fpshift;
		const float fdright = frustum.GetRightDistance() - fnright * fmove - fpshift;
		const float fdbottom = frustum.GetBottomDistance() - fnbottom * fmove - fpshift;
		const float fdnear = frustum.GetNearDistance() - fnnear * fmove - fpshift;
		
		shader2a.SetParameterFloat( spdpuPlaneLeft,
			( float )fnleft.x, ( float )fnleft.y, ( float )fnleft.z, ( float )fdleft );
		shader2a.SetParameterFloat( spdpuPlaneRight,
			( float )fnright.x, ( float )fnright.y, ( float )fnright.z, ( float )fdright );
		shader2a.SetParameterFloat( spdpuPlaneTop,
			( float )fntop.x, ( float )fntop.y, ( float )fntop.z, ( float )fdtop );
		shader2a.SetParameterFloat( spdpuPlaneBottom,
			( float )fnbottom.x, ( float )fnbottom.y, ( float )fnbottom.z, ( float )fdbottom );
		shader2a.SetParameterFloat( spdpuPlaneNear,
			( float )fnnear.x, ( float )fnnear.y, ( float )fnnear.z, ( float )fdnear );
		
		pUBORenderLight->Activate();
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		
		// pass 2
		if( giState->GetActiveCascade().GetIndex() == cascade.GetIndex() ){
			pPipelineDebugProbeUpdatePass2->Activate();
			
			deoglShaderCompiled &shader2b = pPipelineDebugProbeUpdatePass2->GetGlShader();
			shader2b.SetParameterFloat( spdpuPosTransform, scale.x * 2.0f, scale.y * 2.0f,
				scale.x * 2.0f * position.x - 1.0f, scale.y * 2.0f * position.y - 1.0f );
			shader2b.SetParameterFloat( spdpuTCTransform, ( float )size.x * 0.5f, ( float )size.y * -0.5f,
				( float )size.x * 0.5, ( float )size.y * 0.5f );
			shader2b.SetParameterInt( spdpuGIDebugCascade, cascade.GetIndex() );
			shader2b.SetParameterInt( spdpuParams, probeSize, probeSpacing, groupSpacing );
			
			pUBORenderLight->Activate();
			GetRenderThread().GetGI().GetUBOProbeIndex().Activate();
			OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_TRIANGLE_FAN, 0, 4, cascade.GetUpdateProbeCount() ) );
		}
	}
	
	
	// clean up
	pglBindVertexArray( 0 );
	tsmgr.DisableAllStages();
}



void deoglRenderGI::ResetDebugInfo(){
	pDebugInfoGI->Clear();
	pDebugInfoGITraceRays->Clear();
	pDebugInfoGIRenderMaterials->Clear();
	pDebugInfoGIClearProbes->Clear();
	pDebugInfoGIUpdateProbes->Clear();
	pDebugInfoGIMoveProbes->Clear();
	pDebugInfoGIRenderLight->Clear();
	pDebugInfoGIRenderLightGIRay->Clear();
}

void deoglRenderGI::AddTopLevelDebugInfo(){
	GetRenderThread().GetDebug().GetDebugInformationList().Add( pDebugInfoGI );
}

void deoglRenderGI::DevModeDebugInfoChanged(){
	const int details = GetRenderThread().GetDebug().GetDeveloperMode().GetDebugInfoDetails();
	const bool show = ( details & deoglDeveloperMode::edimGI ) == deoglDeveloperMode::edimGI;
	
	pDebugInfoGI->SetVisible( show );
}



// Private Functions
//////////////////////

void deoglRenderGI::pCleanUp(){
}

void deoglRenderGI::pCreateUBORenderLight(){
	deoglRenderThread &renderThread = GetRenderThread();
	pUBORenderLight.TakeOver( new deoglSPBlockUBO( renderThread ) );
	deoglSPBlockUBO &ubo = pUBORenderLight;
	
	ubo.SetRowMajor( ! renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() );
	ubo.SetParameterCount( euprlGridCoordUnshift + 1 );
	ubo.SetElementCount( GI_MAX_CASCADES );
	
	ubo.GetParameterAt( euprlMatrix ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 ); // mat4x3
	ubo.GetParameterAt( euprlMatrixNormal ).SetAll( deoglSPBParameter::evtFloat, 3, 3, 1 ); // mat3
	ubo.GetParameterAt( euprlProbeCount ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	ubo.GetParameterAt( euprlOcclusionMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( euprlProbeClamp ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	ubo.GetParameterAt( euprlDistanceMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( euprlProbeSpacing ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( euprlProbeSpacingInv ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( euprlPositionClamp ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( euprlNormalBias ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( euprlOcclusionMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo.GetParameterAt( euprlDistanceMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo.GetParameterAt( euprlGridCoordShift ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	ubo.GetParameterAt( euprlIrradianceGamma ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( euprlSelfShadowBias ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( euprlGridOrigin ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( euprlGridCoordUnshift ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	
	ubo.MapToStd140();
	ubo.SetBindingPoint( 1 );
}

void deoglRenderGI::pSharedTraceRays( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	OGL_CHECK( renderThread, pglBindVertexArray(
		renderThread.GetDeferredRendering().GetVAOFullScreenQuad()->GetVAO() ) );
	
	// BVH node count can be 0. in this case pBVHInstanceRootNode is set to -1 instead
	// of the index of the root node. the scripts can deal with this. it is required
	// for the scripts to run even if the BVH is empty since the ray result pixels
	// have to be correctly initialized with the ray direction otherwise the probe
	// update shader fails to work correctly
	renderThread.GetFramebuffer().Activate( &renderThread.GetGI().GetTraceRays().GetFBOResult() );
	
	SetViewport( plan.GetUpdateGIState()->GetSampleImageSize() );
}

void deoglRenderGI::pClearTraceRays(){
	OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread() );
	
	const GLfloat clearPosition[ 4 ] = { 0.0f, 0.0f, 0.0f, 10000.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearPosition[ 0 ] ) );
	
	const GLfloat clearNormal[ 4 ] = { 0.0f, 0.0f, 1.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 1, &clearNormal[ 0 ] ) );
	
	const GLfloat clearDiffuse[ 4 ] = { 1.0f, 1.0f, 1.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 2, &clearDiffuse[ 0 ] ) );
	
	const GLfloat clearReflectivity[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 3, &clearReflectivity[ 0 ] ) );
	
	const GLfloat clearLight[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 4, &clearLight[ 0 ] ) );
}

void deoglRenderGI::pInitTraceTextures( deoglGIBVH &bvh ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const deoglGI &gi = renderThread.GetGI();
	const deoglGIBVHShared &shared = gi.GetBVHShared();
	
	tsmgr.EnableTBO( 0, shared.GetTBONodeBox()->GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 1, shared.GetTBOIndex()->GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 2, bvh.GetTBOInstance()->GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 3, bvh.GetTBOMatrix()->GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 4, shared.GetTBOFace()->GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 5, shared.GetTBOVertex()->GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 6, shared.GetTBOTexCoord()->GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 7, shared.GetTBOMaterial()->GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 8, shared.GetTBOMaterial2()->GetTBO(), GetSamplerClampNearest() );
	
	const deoglGIMaterials &materials = gi.GetMaterials();
	tsmgr.EnableTexture( 9, materials.GetTextureDiffuse(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 10, materials.GetTextureReflectivity(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 11, materials.GetTextureEmissivity(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 12 );
}

void deoglRenderGI::pActivateGIUBOs(){
	const deoglGI &gi = GetRenderThread().GetGI();
	gi.GetUBOParameter().Activate();
	gi.GetUBOProbeIndex().Activate();
	gi.GetUBOProbePosition().Activate();
	gi.GetUBORayDirection().Activate();
}
