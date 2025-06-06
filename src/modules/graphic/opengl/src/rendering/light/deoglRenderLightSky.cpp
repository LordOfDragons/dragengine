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

#include "deoglRenderLight.h"
#include "deoglRenderLightSky.h"
#include "deoglRenderGI.h"
#include "../deoglRenderGeometry.h"
#include "../deoglRenderWorld.h"
#include "../debug/deoglRenderDebug.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../plan/deoglRenderPlanSkyLight.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglAddToRenderTask.h"
#include "../lod/deoglLODCalculator.h"
#include "../deoglRenderOcclusion.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListManager.h"
#include "../../collidelist/deoglCollideListHTSector.h"
#include "../../collidelist/deoglCollideListHTSCluster.h"
#include "../../collidelist/deoglCollideListPropField.h"
#include "../../collidelist/deoglCollideListPropFieldType.h"
#include "../../collidelist/deoglCollideListPropFieldCluster.h"
#include "../../component/deoglComponentLOD.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugSaveTexture.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../debug/deoglDebugSnapshot.h"
#include "../../debug/debugSnapshot.h"
#include "../../debug/deoglDebugTraceGroup.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../gi/deoglGI.h"
#include "../../gi/deoglGIRayCache.h"
#include "../../gi/deoglGIState.h"
#include "../../light/deoglRLight.h"
#include "../../light/shader/deoglLightShader.h"
#include "../../light/volume/deoglLightVolume.h"
#include "../../model/deoglModelLOD.h"
#include "../../model/deoglRModel.h"
#include "../../occlusiontest/deoglOcclusionTest.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../../shadow/deoglSCSolid.h"
#include "../../shadow/deoglSCTransparent.h"
#include "../../shadow/deoglShadowCaster.h"
#include "../../shadow/deoglShadowMapper.h"
#include "../../shapes/deoglShape.h"
#include "../../shapes/deoglShapeManager.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../sky/deoglRSky.h"
#include "../../sky/deoglRSkyLayer.h"
#include "../../sky/deoglRSkyInstance.h"
#include "../../sky/deoglRSkyInstanceLayer.h"
#include "../../sky/deoglSkyLayerGICascade.h"
#include "../../texture/arraytexture/deoglArrayTexture.h"
#include "../../texture/arraytexture/deoglRenderableDepthArrayTexture.h"
#include "../../texture/arraytexture/deoglRenderableDepthArrayTextureManager.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../../texture/texture2d/deoglRenderableDepthTextureManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../utils/convexhull/deoglConvexHull2D.h"
#include "../../visitors/deoglVolumeShape.h"
#include "../../world/deoglRWorld.h"
#include "../../world/deoglWorldOctree.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../utils/collision/deoglDCollisionFrustum.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../utils/collision/deoglDCollisionSphere.h"
#include "../../utils/collision/deoglDCollisionVolume.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>



// Definitions
////////////////


enum eSPLight{
	splQuadParams,
	splPosTransform,
	splLightView,
	splLightColor,
	splAmbientColor,
	splLayerBorder,
	splShadowMatrix1,
	splShadowMatrix2,
	splShadowMatrix3,
	splShadowMatrix4,
	splShaMap1Params
};

enum eSPLightAmbient{
	splaQuadParams,
	splaPosTransform,
	splaLightView,
	splaAmbientColor
};



// Class deoglRenderLightSky
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderLightSky::deoglRenderLightSky( deoglRenderThread &renderThread ) :
deoglRenderLightBase( renderThread ),

pColList2( NULL ),

pSolidShadowMap( NULL )
{
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	
	try{
		renderThread.GetShader().SetCommonDefines(commonDefines);
		
		// clear depth
		pipconf.Reset();
		pipconf.SetMasks( false, false, false, false, true );
		pipconf.EnableDepthTestLessEqual();
		pipconf.EnableCulling( false );
		pipconf.SetStencil( GL_NOTEQUAL, 0, 0xff, 0xff );
		pipconf.SetStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		pipconf.SetEnableStencilTest( true );
		
		defines = commonDefines;
		pAsyncGetPipeline(pPipelineClearDepth, pipconf, "DefRen Clear Depth", defines);
		
		
		// occlusion mesh
		pipconf.Reset();
		pipconf.SetMasks( false, false, false, false, true );
		pipconf.EnableDepthTestLessEqual();
		pipconf.SetDepthClamp( true );
		
		AddSharedSPBDefines( defines );
		defines.SetDefines( "WITH_SHADOWMAP" );
		defines.SetDefines( "DEPTH_ORTHOGONAL" );
		defines.SetDefines( "DEPTH_OFFSET" );
		pipconf.SetSPBInstanceIndexBase(0);
		pAsyncGetPipeline(pPipelineOccMesh, pipconf, "DefRen Occlusion OccMap", defines, true);
		
		
		
		pColList2 = new deoglCollideList;
		
		
		
		// debug information
		const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
		const decColor colorBgUp( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBg( 0.0f, 0.0f, 0.25f, 0.75f );
		const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
		const decColor colorBgSub3( 0.15f, 0.15f, 0.15f, 0.75f );
		
		pDebugInfoSolid.TakeOver( new deoglDebugInformation( "Sky", colorText, colorBgUp ) );
		pDebugInfoTransparent.TakeOver( new deoglDebugInformation( "Sky", colorText, colorBgUp ) );
		
		
		
		pDebugInfoSolidDetail.TakeOver( new deoglDebugInformation( "Light Sky Solid", colorText, colorBg ) );
		
		pDebugInfoSolidShadow.TakeOver( new deoglDebugInformation( "Shadow", colorText, colorBgSub ) );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidShadow );
		
		pDebugInfoSolidShadowOcclusion.TakeOver( new deoglDebugInformation( "Occlusion", colorText, colorBgSub2 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowOcclusion );
		
		pDebugInfoSolidShadowSplit.TakeOver( new deoglDebugInformation( "Splits", colorText, colorBgSub2 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowSplit );
		
		pDebugInfoSolidShadowSplitContent.TakeOver( new deoglDebugInformation( "Add Elements", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitContent );
		
		pDebugInfoSolidShadowSplitLODLevels.TakeOver( new deoglDebugInformation( "LOD Levels", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitLODLevels );
		
		pDebugInfoSolidShadowSplitClear.TakeOver( new deoglDebugInformation( "Clear", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitClear );
		
		pDebugInfoSolidShadowSplitTask.TakeOver( new deoglDebugInformation( "Task", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitTask );
		
		pDebugInfoSolidShadowSplitRender.TakeOver( new deoglDebugInformation( "Render", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitRender );
		
		pDebugInfoSolidShadowGI.TakeOver( new deoglDebugInformation( "Shadow GI", colorText, colorBgSub ) );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidShadowGI );
		
		pDebugInfoSolidLight.TakeOver( new deoglDebugInformation( "Light", colorText, colorBgSub ) );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidLight );
		
		
		
		pDebugInfoTransparentDetail.TakeOver( new deoglDebugInformation( "Light Sky Transp", colorText, colorBg ) );
		
		pDebugInfoTransparentLight.TakeOver( new deoglDebugInformation( "Light", colorText, colorBgSub ) );
		pDebugInfoTransparentDetail->GetChildren().Add( pDebugInfoTransparentLight );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderLightSky::~deoglRenderLightSky(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderLightSky::RenderLights( deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask, bool xray ){
// 	if( ! solid ){
// 		return;
// 	}
	
	if( solid && ! xray ){
		// this situation is annoying. to build sky shadow render tasks these actions are required:
		// - find sky light content (parallel task)
		// - render occlusion tests => requires view occlusion map to be rendered
		// - resolve occlusion tests and remove invisible components
		// - build render tasks (2 parallel tasks)
		// 
		// the problem here is the dependency on the view occlusion map to be rendered paired
		// with the time consumption of finding sky light content. due to this rendering the
		// occlusion tests can be done earliest at the end of the prepare render plan stage.
		// but to build render plans in parallel we need to first wait for the occlusion test
		// results to come in. by this time we are often close to rendering the sky light
		// and the effect is voided.
		// 
		// a solution would be to use glFenceSync() and periodically check if a render process
		// finished to start a following one as soon as possible. this is though quite complex
		// and possibly not helping. kept for later.
		// 
		// what is done here is starting the parellel tasks here. while rendering the GI shadow
		// is rendered first in the hope to hide the building of the render tasks there
		plan.SkyLightsStartBuildRT();
	}
	
	const int count = plan.GetSkyLightCount();
	if( count == 0 ){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace( GetRenderThread(), "LightSky.RenderLights" );
	DebugTimersReset( plan, false );
	
	//RenderAO( plan );
	RestoreFBO( plan );
	
	int i;
	for( i=0; i<count; i++ ){
		RenderLight( *plan.GetSkyLightAt( i ), solid, mask, xray );
	}
	
	if( solid ){
		DebugTimer1Sample( plan, *pDebugInfoSolid, true );
		
	}else{
		DebugTimer1Sample( plan, *pDebugInfoTransparent, true );
	}
}



void deoglRenderLightSky::RenderAO( deoglRenderPlan &plan ){
#if 0
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
//	deoglRenderGeometry &rengeom = *ogl.GetRenderGeometry();
//	const decMatrix &cameraMatrix = plan.GetCameraMatrix();
	
	float znear = plan.GetCameraImageDistance();
	float zfar = plan.GetCameraViewDistance();
	float fov = plan.GetCameraFov();
	float fovRatio = plan.GetCameraFovRatio();
	float aspectRatio = plan.GetAspectRatio();
	
	// attach ao texture
	defren.ActivateFBOTemporary1( false );
	tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	
	// clear the ao texture to full bright
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	SetCullMode( ! plan.GetFlipCulling() );
	
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_FALSE, GL_FALSE ) );
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	// set shader
	renderThread.GetShader().ActivateShader( pPipelineAO );
	
	pPipelineAO->GetGlShader().SetParameterFloat( 0, defren.GetPixelSizeU(), defren.GetPixelSizeV(), 0.0f, 0.0f );
	
	// mapping (0,1-sv) => (-1,-1) and (su,1) => (1,1)
	// (x-c)*s = x*s-c*s
	float scaleU = 2.0f / defren.GetScalingU(); // s = 1/(su/2) = 2/su
	float scaleV = 2.0f / defren.GetScalingV(); // s = 1/(sv/2) = 2/sv
	float offsetU = -1.0f; // c = su/2 ; c*s = su/2 * 1/(su/2) = 1
	float offsetV = 1.0f - scaleV; // c = 1-sv/2 ; c*s = (1-sv/2) * 1/(sv/2) = 1/(sv/2)-1 = s-1
	
	// u=0; 0*(2/su) + (-1) = -1 (ok)
	// u=su; su*(2/su) + (-1) = 2-1 = 1 (ok)
	// v=1-sv; (1-sv)*(2/sv) + (1-2/sv) = 2/sv-2 + 1-2/sv = -1 (ok)
	// v=1; 1*(2/sv) + (1-2/sv) = 2/sv + 1-2/sv = 1 (ok)
	pPipelineAO->GetGlShader().SetParameterFloat( 1, scaleU, scaleV, offsetU, offsetV );
	
	float q = zfar / ( zfar - znear );
	pPipelineAO->GetGlShader().SetParameterVector4( 2, plan.GetDepthToPosition() );
	
	// render probes
//	rengeom.RenderSkyLOProbes( &plan, plan.GetCollideList(), plan.GetCameraMatrix() );
#endif
}

void deoglRenderLightSky::RenderLight( deoglRenderPlanSkyLight &plan, bool solid,
const deoglRenderPlanMasked *mask, bool xray ){
	if( ! plan.GetUseLight() ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "LightSky.RenderLight" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	//deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglGIState * const giState = plan.GetPlan().GetUpdateGIState();
	const deoglLightPipeline *pipeline = nullptr;
	
	const bool useShadow = plan.GetUseShadow();
	int shadowMapSize = plan.GetPlan().GetShadowSkySize();
	int passCount = 1;
	
	// render shadow map
	passCount = plan.GetShadowLayerCount();
	
	if( ( useShadow || giState ) && solid && ! xray ){
		RenderShadows( plan, solid, mask );
		RestoreFBO( plan.GetPlan() );
		DebugTimer2SampleCount( plan.GetPlan(), *pDebugInfoSolidShadow, 1, true );
	}
	
	int pipelineModifiers = 0;
	if( ! solid ){
		pipelineModifiers |= deoglLightPipelines::emTransparent;
	}
	if( plan.GetPlan().GetRenderStereo() ){
		pipelineModifiers |= deoglLightPipelines::emStereo;
	}
	
	// activate pipeline
	deoglRSkyInstanceLayer &skyLayer = *plan.GetLayer();
	if( useShadow ){
		pipeline = &skyLayer.GetPipelines().GetWithRef( deoglLightPipelines::etSolid1, pipelineModifiers );
		
	}else if( skyLayer.GetHasLightDirect() ){
		pipeline = &skyLayer.GetPipelines().GetWithRef( deoglLightPipelines::etNoShadow, pipelineModifiers );
		
	}else{
		pipeline = &skyLayer.GetPipelines().GetWithRef( deoglLightPipelines::etAmbient, pipelineModifiers );
	}
	
	pipeline->GetPipeline()->Activate();
	
	SetViewport( plan.GetPlan() );
	
	// set program parameters
	deoglSPBlockUBO &spbInstance = skyLayer.GetInstanceParameterBlock();
	deoglSPBlockUBO &spbLight = skyLayer.GetLightParameterBlock();
	
	const deoglLightShader &lightShader = pipeline->GetShader();
	UpdateLightParamBlock( lightShader, spbLight, plan );
	UpdateInstanceParamBlock( lightShader, spbInstance, plan, shadowMapSize, passCount );
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	spbLight.Activate();
	spbInstance.Activate();
	
	// set textures
	int target;
	
	target = lightShader.GetTextureTarget( deoglLightShader::ettShadow1SolidDepth );
	if( target != -1 ){
		tsmgr.EnableArrayTexture( target, *pSolidShadowMap->GetArrayTexture(), GetSamplerShadowClampLinear() );
	}
	
	target = lightShader.GetTextureTarget( deoglLightShader::ettNoise );
	if( target != -1 ){
		tsmgr.EnableTexture( target, *renderThread.GetDefaultTextures().GetNoise2D(), GetSamplerRepeatNearest() );
	}
	
	target = lightShader.GetTextureTarget( deoglLightShader::ettLightDepth1 );
	if( target != -1 ){
		tsmgr.EnableArrayTexture( target, *pSolidShadowMap->GetArrayTexture(), GetSamplerClampLinear() );
	}
	
	// set the ao texture
	//tsmgr.EnableTexture( 5, *defren.GetTemporaryTexture(), deoglTextureStageManager::etfNearest, GL_CLAMP );
	
	// render quad
	RenderFullScreenQuadVAO( plan.GetPlan() );
	
	if( solid ){
		DebugTimer2SampleCount( plan.GetPlan(), *pDebugInfoSolidLight, 1, true );
		
	}else{
		DebugTimer2SampleCount( plan.GetPlan(), *pDebugInfoTransparentLight, 1, true );
	}
	
	// GI rays
	if( ! mask && solid && ! xray && giState ){
		const deoglSkyLayerGICascade * const slgc = plan.GetLayer()->GetGICascade( giState->GetSkyShadowCascade() );
		const deoglSCSolid * const scsolid = slgc ? &slgc->GetShadowCaster().GetSolid() : NULL;
		
		RestoreFBOGITraceRays( *giState );
		
		deoglTexture *texture1 = renderThread.GetDefaultTextures().GetShadowMap();
		deoglTexture *texture2 = texture1;
		pipeline = nullptr;
		
		if( scsolid ){
			if( scsolid->GetStaticMap() ){
				texture1 = scsolid->GetStaticMap();
				
				if( scsolid->GetDynamicMap() ){
					texture2 = scsolid->GetDynamicMap();
					pipeline = skyLayer.GetPipelines().GetWith( deoglLightPipelines::etGIRaySolid2, 0 );
					
				}else{
					pipeline = skyLayer.GetPipelines().GetWith( deoglLightPipelines::etGIRaySolid1, 0 );
				}
				
			}else if( scsolid->GetDynamicMap() ){
				texture1 = scsolid->GetDynamicMap();
				pipeline = skyLayer.GetPipelines().GetWith( deoglLightPipelines::etGIRaySolid1, 0 );
				
			}else{
				pipeline = skyLayer.GetPipelines().GetWith( deoglLightPipelines::etGIRayNoShadow, 0 );
			}
		}
		
		if( pipeline ){
			pipeline->GetPipeline()->Activate();
			
			// WARNING always non-stereo!
			renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
			
			spbLight.Activate();
			spbInstance.Activate();
			
			const deoglLightShader &lightShaderGI = pipeline->GetShader();
			target = lightShaderGI.GetTextureTarget( deoglLightShader::ettShadow1SolidDepth );
			if( target != -1 ){
				tsmgr.EnableTexture( target, *texture1, GetSamplerShadowClampLinear() );
			}
			
			target = lightShaderGI.GetTextureTarget( deoglLightShader::ettShadow2SolidDepth );
			if( target != -1 ){
				tsmgr.EnableTexture( target, *texture2, GetSamplerShadowClampLinear() );
			}
			
			target = lightShaderGI.GetTextureTarget( deoglLightShader::ettNoise );
			if( target != -1 ){
				tsmgr.EnableTexture( target, *renderThread.GetDefaultTextures().GetNoise2D(), GetSamplerRepeatNearest() );
			}
			
			RenderFullScreenQuadVAO();
		}
	}
}



void deoglRenderLightSky::RenderShadows( deoglRenderPlanSkyLight &plan, bool solid,
const deoglRenderPlanMasked *mask ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	
	if( renderThread.GetChoices().GetUseComputeRenderTask() ){
		plan.FinishReadBackComputeRenderTasks();
	}
	
	if( solid && ! mask ){
		RenderGIShadows( plan, shadowMapper );
	}
	
	if( plan.GetUseShadow() ){
		RenderShadowMap( plan, shadowMapper );
	}
}

// #define SKY_SHADOW_LAYERED_RENDERING 1
#define SKY_SHADOW_FILTERED 1
#define SKY_SHADOW_OCCMAP_VISIBLE 1
// #define SKY_SHADOW_DEBUG_TIME 1

#if defined SKY_SHADOW_LAYERED_RENDERING && defined SKY_SHADOW_FILTERED
	#undef SKY_SHADOW_FILTERED
#endif

#ifdef SKY_SHADOW_DEBUG_TIME
	#define SSDTPF(...) printf(__VA_ARGS__)
	#define SSDTLOG(...) renderThread.GetLogger().LogInfoFormat(__VA_ARGS__)
#else
	#define SSDTPF(...)
	#define SSDTLOG(...)
#endif

void deoglRenderLightSky::RenderShadowMap( deoglRenderPlanSkyLight &plan, deoglShadowMapper &shadowMapper ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "LightSky.RenderShadowMap" );
	const deoglRWorld &world = *plan.GetPlan().GetWorld();
#ifdef SKY_SHADOW_LAYERED_RENDERING
	deoglSPBlockUBO *renderParamBlock = renderThread.GetRenderers().GetLight().GetShadowCascadedPB();
#else
	deoglSPBlockUBO *renderParamBlock = renderThread.GetRenderers().GetLight().NextShadowPB();
#endif
	const bool bugClearEntireArrTex = renderThread.GetCapabilities().GetClearEntireArrayTexture().Broken();
// 	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
// 	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	const decDVector &referencePosition = world.GetReferencePosition();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglPipelineState &state = renderThread.GetPipelineManager().GetState();
	deoglRSkyInstanceLayer &skyLayer = *plan.GetLayer();
	decMatrix matrixCamera;
	
	const decDMatrix &matCamInv = plan.GetPlan().GetInverseCameraMatrix();
	
	const decDVector &camPos = plan.GetPlan().GetCameraPosition();
	const bool clearBackFaceFragments = false; // problems. see GIShadow for the reason why
// 	int c, componentCount;
	deoglDCollisionBox box;
	decVector position;
	int i;
	
	// occlusion test the content to remove as much as possible. rendering the tests has
	// been done ahead of time in the render plan sky light to avoid stalling
#ifdef SKY_SHADOW_OCCMAP_VISIBLE
	// done by deoglRenderPlanSkyLight
// 	planSkyLight.GetOcclusionTest()->UpdateResults();
// 	collideList.RemoveCulledElements();
// 	DebugTimer3Sample( plan, *pDebugInfoSolidShadowOcclusion, false );
#endif
	
	// get light properties
	const decMatrix matLig = decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * skyLayer.GetMatrix();
	const decMatrix matLigInv = matLig.Invert();
	const decMatrix matCL = matCamInv.GetRotationMatrix().ToMatrix() * matLigInv;
	
	// set up stencil mask. this is used to mark back facing fragments (see after rendering)
	if( clearBackFaceFragments ){
		state.StencilOpFront( GL_KEEP, GL_KEEP, GL_ZERO );
		state.StencilOpBack( GL_KEEP, GL_KEEP, GL_REPLACE );
		state.StencilMask( ~0 );
		
	}else{
		state.StencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		state.StencilMask( 0x0 );
	}
	state.StencilFunc( GL_ALWAYS, ~0, ~0 );
	
	// get shadow map
	const int shadowMapSize = plan.GetPlan().GetShadowSkySize();
	const int layerCount = plan.GetShadowLayerCount();
	
	if( pSolidShadowMap && ( pSolidShadowMap->GetWidth() != shadowMapSize
			|| pSolidShadowMap->GetLayerCount() != layerCount ) ){
		pSolidShadowMap->SetInUse( false );
		pSolidShadowMap = NULL;
	}
	if( ! pSolidShadowMap ){
		pSolidShadowMap = renderThread.GetTexture().GetRenderableDepthArrayTexture()
			.GetWith( shadowMapSize, shadowMapSize, layerCount, true, false );
	}
	
	// clear shadow map
	shadowMapper.SetForeignSolidDepthArrayTexture( pSolidShadowMap->GetArrayTexture() );
	
#ifdef SKY_SHADOW_LAYERED_RENDERING
	if( bugClearEntireArrTex ){
		pPipelineClearBuffers->Activate();
		for( i=0; i<layerCount; i++ ){
			shadowMapper.ActivateSolidArrayTextureLayer( shadowMapSize, layerCount, i, true );
			OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, 1.0f, 0xff ) );
		}
		shadowMapper.ActivateSolidArrayTexture( shadowMapSize, layerCount, true );
		
	}else{
		shadowMapper.ActivateSolidArrayTexture( shadowMapSize, layerCount, true );
		OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, 1.0f, 0xff ) );
	}
#else
	if( ! bugClearEntireArrTex ){
		pPipelineClearBuffers->Activate();
		shadowMapper.ActivateSolidArrayTexture( shadowMapSize, layerCount, true );
		if( clearBackFaceFragments ){
			OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, 1.0f, 0xff ) );
			
		}else{
			const GLfloat clearDepth = 1.0f;
			OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		}
	}
#endif
	
	// render all layers into the shadow map
	DebugTimer3Reset( plan.GetPlan(), true );
	DebugTimer4Reset( plan.GetPlan(), true );
	
#ifndef SKY_SHADOW_FILTERED
		#ifdef SKY_SHADOW_DEBUG_TIME
		decTimer timer;
		#endif
	// render task "all in one". calculating the optimal LOD to use would require using
	// SetComponentLODOrtho() on the lowest split the component is visible in. since this
	// is calculated on the GPU this can not be done here without expensive CPU calculations.
	// as a makeshift solution the LOD is calculated the same way as with the camera view
// 	deoglLODCalculator lodCalculator[ 4 ];
// 	float lodBoxWidth[ 4 ], lodBoxHeight[ 4 ];
// 	
// 	for( i=0; i<layerCount; i++ ){
// 		lodCalculator[ i ].SetMaxPixelError( config.GetLODMaxPixelError() );
// 		lodCalculator[ i ].SetMaxErrorPerLevel( config.GetLODMaxErrorPerLevel() );
// 		
// 		const deoglRenderPlanSkyLight::sShadowLayer &sl = planSkyLight.GetShadowLayerAt( i );
// 		lodBoxWidth[ i ] = sl.maxExtend.x - sl.minExtend.x;
// 		lodBoxHeight[ i ] = sl.maxExtend.y - sl.minExtend.y;
// 	}
	
	const int componentCount = collideList.GetComponentCount();
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &cl = *collideList.GetComponentAt( i );
		deoglRComponent &component = *cl.GetComponent();
// 		const int mask = component.GetSkyShadowSplitMask();
// 		
// 		if( mask ){
// 			int lowest;
// 			for( lowest=0; lowest<layerCount && ( mask & ( 1 << lowest ) ) == 0; lowest++ );
// 			lodCalculator[ lowest ].SetComponentLODOrtho( cl, lodBoxWidth[ lowest ],
// 				lodBoxHeight[ lowest ], shadowMapSize, shadowMapSize );
// 			
// 		}else{
// 			cl.SetLODLevel( component.GetLODCount() - 1 );
// 		}
		
		component.SetSpecialFlagsFromSkyShadowLayerMask( cl.GetCascadeMask() );
	}
	
	
	deoglLODCalculator lodCalculator;
	
	lodCalculator.SetMaxPixelError( config.GetLODMaxPixelError() );
	lodCalculator.SetMaxErrorPerLevel( config.GetLODMaxErrorPerLevel() );
	
	lodCalculator.SetComponentLODProjection( collideList, plan.GetCameraPosition(),
		plan.GetInverseCameraMatrix().TransformView(), plan.GetCameraFov(),
		plan.GetCameraFov() * plan.GetCameraFovRatio(), plan.GetViewportWidth(),
		plan.GetViewportHeight() );
			SSDT("SkyLight %d: Prepare %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
	
	
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renderParamBlock );
	
	addToRenderTask.Reset();
#ifdef SKY_SHADOW_LAYERED_RENDERING
	renderTask.SetUseSPBInstanceFlags( true );
	addToRenderTask.SetUseSpecialParamBlock( true );
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentShadowOrthogonalCascaded );
#else
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentShadowOrthogonal );
#endif
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoShadowNone( true );
	addToRenderTask.SetForceDoubleSided( true );
	addToRenderTask.AddComponents( collideList );
			SSDT("SkyLight %d: RTBuild %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
	
	renderTask.PrepareForRender( renderThread );
	DebugTimer4Sample( plan, *pDebugInfoSolidShadowSplitTask, true );
			SSDT("SkyLight %d: RTPrepare %dys (s=%d t=%d v=%d i=%d si=%d)\n", i,
				(int)(timer.GetElapsedTime()*1e6f), renderTask.GetShaderCount(),
				renderTask.GetTotalTextureCount(), renderTask.GetTotalVAOCount(),
				renderTask.GetTotalInstanceCount(), renderTask.GetTotalSubInstanceCount());
#endif
	
	// render layers
	
#ifdef SKY_SHADOW_LAYERED_RENDERING
	// render solid content. two different depth offsets for front and back faces are used. double sided always
	// counts as front facing. this way all can be rendered in one go
	{
		const deoglSPBMapBuffer mapped( renderParamBlock );
		
		renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, decMatrix() );
		
		for( i=0; i<layerCount; i++ ){
			deoglRenderPlanSkyLight::sShadowLayer &sl = planSkyLight.GetShadowLayerAt( i );
			
			// determine the shadow limits
			position.x = ( sl.minExtend.x + sl.maxExtend.x ) * 0.5f;
			position.y = ( sl.minExtend.y + sl.maxExtend.y ) * 0.5f;
			position.z = ( sl.minExtend.z + sl.maxExtend.z ) * 0.5f;
			//position.z = sl.minExtend.z;
			
			sl.position = ( camPos - referencePosition ).ToVector() + ( matLig * position );
			sl.scale.x = 1.0f / ( sl.maxExtend.x - sl.minExtend.x );
			sl.scale.y = 1.0f / ( sl.maxExtend.y - sl.minExtend.y );
			sl.scale.z = 1.0f / ( sl.maxExtend.z - sl.minExtend.z );
			
			// setup matrix
			sl.matrix = matCL * decMatrix::CreateTranslation( -sl.minExtend ) * decMatrix::CreateScale( sl.scale );
			
			matrixCamera = decMatrix::CreateCamera( sl.position, matLig.TransformView(), matLig.TransformUp() )
				* decMatrix::CreateScale( sl.scale * 2.0f );
			
			renderParamBlock->SetParameterDataArrayMat4x3( deoglSkinShader::erutMatrixV, i, matrixCamera );
			renderParamBlock->SetParameterDataArrayMat4x4( deoglSkinShader::erutMatrixVP, i, matrixCamera );
			renderParamBlock->SetParameterDataArrayMat3x3( deoglSkinShader::erutMatrixVn,
				i, matrixCamera.GetRotationMatrix().Invert() );
			renderParamBlock->SetParameterDataArrayVec4( deoglSkinShader::erutDepthOffset,
				i, sl.zscale, sl.zoffset, -sl.zscale, -sl.zoffset );
		}
	}
			SSDT("SkyLight %d: ParamBlock %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
	
	state.StencilOpFront( GL_KEEP, GL_KEEP, GL_ZERO );
	state.StencilOpBack( GL_KEEP, GL_KEEP, GL_REPLACE );
	state.StencilFunc( GL_ALWAYS, ~0, ~0 );
	
	rengeom.RenderTask( renderTask );
	
	// clear back facing fragments. back facing fragments cause a lot of troubles to shadow
	// casting. the idea here is to consider back facing fragments hit before any front facing
	// fragments to belong to the outer hull of a map. this is similar to a fragment blocking
	// the sky light right at the near end of the shadow map. by clearing back facing fragments
	// to 0 artifacts for back faces are eliminated altogether. this though won't help with
	// issues on front facing fragments but makes underground scenes safe
	pPipelineClearDepth->Activate();
	
	pPipelineClearDepth->GetGlShader().SetParameterFloat( 0, -1.0f );
	
	RenderFullScreenQuadVAO();
			SSDT("SkyLight %d: Render %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
	
	DebugTimer4Sample( plan, *pDebugInfoSolidShadowSplitRender, true );
	DebugTimer3SampleCount( plan, *pDebugInfoSolidShadowSplit, 1, false );
	
#else
	for( i=0; i<layerCount; i++ ){
		deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetShadowLayerAt( i );
			#ifdef SKY_SHADOW_DEBUG_TIME
			decTimer timer;
			#endif
		
#ifdef SKY_SHADOW_FILTERED
		if( ! renderThread.GetChoices().GetUseComputeRenderTask() ){
			if( i == 0 ){
				plan.WaitFinishedBuildRT1();
				
			}else if( i == 3 ){
				plan.WaitFinishedBuildRT2();
			}
		}
		DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowSplitContent, false );
#endif
			SSDTPF("SkyLight %d: CollideList %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
		
		// determine the shadow limits
		position.x = ( sl.minExtend.x + sl.maxExtend.x ) * 0.5f;
		position.y = ( sl.minExtend.y + sl.maxExtend.y ) * 0.5f;
		position.z = ( sl.minExtend.z + sl.maxExtend.z ) * 0.5f;
		//position.z = sl.minExtend.z;
		
		sl.position = ( camPos - referencePosition ).ToVector() + ( matLig * position );
		sl.scale.x = 1.0f / ( sl.maxExtend.x - sl.minExtend.x );
		sl.scale.y = 1.0f / ( sl.maxExtend.y - sl.minExtend.y );
		sl.scale.z = 1.0f / ( sl.maxExtend.z - sl.minExtend.z );
		
		// calculate lod levels to use
#ifdef SKY_SHADOW_FILTERED
		/* done in parallel task
		const float lodBoxWidth = sl.maxExtend.x - sl.minExtend.x;
		const float lodBoxHeight = sl.maxExtend.y - sl.minExtend.y;
		
		deoglLODCalculator lodCalculator;
		lodCalculator.SetMaxPixelError( 2 );
		
		lodCalculator.SetComponentLODOrtho( *pColList2, lodBoxWidth, lodBoxHeight, shadowMapSize, shadowMapSize );
		DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowSplitLODLevels, false );
		*/
#endif
		
		// setup matrix
		sl.matrix = matCL * decMatrix::CreateTranslation( -sl.minExtend ) * decMatrix::CreateScale( sl.scale );
		
		matrixCamera = decMatrix::CreateCamera( sl.position, matLig.TransformView(), matLig.TransformUp() )
			* decMatrix::CreateScale( sl.scale * 2.0f );
		
		if( clearBackFaceFragments ){
			state.StencilOpFront( GL_KEEP, GL_KEEP, GL_ZERO );
			state.StencilOpBack( GL_KEEP, GL_KEEP, GL_REPLACE );
			state.StencilFunc( GL_ALWAYS, ~0, ~0 );
		}
		
		// activate shadow map. since objects mostly reside in only one shadow texture
		// we attach the individual layers instead of the entire texture. avoids the
		// need for a geometry program to be used.
		shadowMapper.ActivateSolidArrayTextureLayer( shadowMapSize, layerCount, i, true );
		if( bugClearEntireArrTex ){
			pPipelineClearBuffers->Activate();
			OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, 1.0f, 0xff ) );
			DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowSplitClear, true );
		}
			SSDTPF("SkyLight %d: Prepare %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
		
		// render solid content. two different depth offsets for front and back faces are used. double sided always
		// counts as front facing. this way all can be rendered in one go
		if( i > 0 ){
			renderParamBlock = renderThread.GetRenderers().GetLight().NextShadowPB();
		}
		{
			const deoglSPBMapBuffer mapped( *renderParamBlock );
			
			renderParamBlock->SetParameterDataMat4x3( deoglSkinShader::erutMatrixV, matrixCamera );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, decMatrix() );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixVP, matrixCamera );
			renderParamBlock->SetParameterDataMat3x3( deoglSkinShader::erutMatrixVn,
				matrixCamera.GetRotationMatrix().Invert() );
			renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
				sl.zscale, sl.zoffset, -sl.zscale, -sl.zoffset );
			
			renderParamBlock->SetParameterDataBVec4( deoglSkinShader::erutConditions1, false, false, false, false );
		}
			SSDTPF("SkyLight %d: ParamBlock %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
		
		if( renderThread.GetChoices().GetUseComputeRenderTask() ){
#ifdef SKY_SHADOW_FILTERED
			sl.computeRenderTask->SetRenderParamBlock( renderParamBlock );
			DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowSplitTask, true );
#endif
			rengeom.RenderTask( sl.computeRenderTask );
			
		}else{
#ifdef SKY_SHADOW_FILTERED
			sl.renderTask->SetRenderParamBlock( renderParamBlock );
			sl.addToRenderTask->SetForceDoubleSided( true );
#endif
			rengeom.RenderTask( *sl.renderTask );
		}
		
		// clear back facing fragments. back facing fragments cause a lot of troubles to shadow
		// casting. the idea here is to consider back facing fragments hit before any front facing
		// fragments to belong to the outer hull of a map. this is similar to a fragment blocking
		// the sky light right at the near end of the shadow map. by clearing back facing fragments
		// to 0 artifacts for back faces are eliminated altogether. this though won't help with
		// issues on front facing fragments but makes underground scenes safe
		if( clearBackFaceFragments ){
			pPipelineClearDepth->Activate();
			pPipelineClearDepth->GetShader().SetParameterFloat( 0, -1.0f );
			RenderFullScreenQuadVAO();
		}
			SSDTPF("SkyLight %d: Render %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
		
		DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowSplitRender, true );
	}
#endif
	
	shadowMapper.DropForeignArrayTextures();
	DebugTimer3SampleCount( plan.GetPlan(), *pDebugInfoSolidShadowSplit, layerCount, true );
}

void deoglRenderLightSky::RenderGIShadows( deoglRenderPlanSkyLight &plan,
deoglShadowMapper &shadowMapper ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglGIState * const giState = plan.GetPlan().GetUpdateGIState();
	if( ! giState ){
		return;
	}
	
	DebugTimer4Reset( plan.GetPlan(), true );
	plan.WaitFinishedGIUpdateRT();
	
	deoglSkyLayerGICascade * const slgc = plan.GetLayer()->GetGICascade( giState->GetSkyShadowCascade() );
	if( ! slgc ){
		return;
	}
	
	// shadow layer
	const decMatrix matLig( decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * plan.GetLayer()->GetMatrix() );
	
	deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetGIShadowLayer();
	
	const decDVector &worldRefPos = plan.GetPlan().GetWorld()->GetReferencePosition();
	const decVector position( ( sl.minExtend + sl.maxExtend ) * 0.5f );
	sl.position = ( slgc->GetPosition() - worldRefPos ).ToVector() + ( matLig * position );
	sl.scale.x = 1.0f / ( sl.maxExtend.x - sl.minExtend.x );
	sl.scale.y = 1.0f / ( sl.maxExtend.y - sl.minExtend.y );
	sl.scale.z = 1.0f / ( sl.maxExtend.z - sl.minExtend.z );
// 		renderThread.GetLogger().LogInfoFormat("ShadowMapGI: (%g,%g,%g)",
// 			sl.maxExtend.x - sl.minExtend.x, sl.maxExtend.y - sl.minExtend.y, sl.maxExtend.z - sl.minExtend.z);
	
	// prepare what is used by both
	deoglShadowCaster &shadowCaster = slgc->GetShadowCaster();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	
	shadowCaster.SetShadowLayerCount( 1 );
	deoglShadowCaster::sShadowLayer &scsl = shadowCaster.GetShadowLayerAt( 0 );
	scsl.layerBorder = sl.layerBorder;
	scsl.scale = sl.scale;
	scsl.matrix = sl.matrix;
	
	// the calculation here is a bit complicated. the fragment position is transformed from
	// GI space to camera space to allow the same light shader code to light it. the GI shadow
	// map though is calculated relative to GI space. the GI and camera space are slightly
	// different from each other. for this reason we have to convert the position first from
	// camera spaces back to GI space. from there we can convert to light space
	// 
	// NOTE the rotation is not the problem but the position. the sl.position calculation
	//      above uses for camera plan->GetCameraPosition() instead of giState->GetPosition().
	//      this offset has to be corrected
	const decDMatrix matrixCamera( decDMatrix::CreateCamera( sl.position, matLig.TransformView(), matLig.TransformUp() ) );
	
	sl.matrix = ( plan.GetPlan().GetInverseCameraMatrix() * matrixCamera ).ToMatrix()
		* decMatrix::CreateTranslation( -sl.minExtend ) * decMatrix::CreateScale( sl.scale );
	
	const decMatrix matrixGI( matrixCamera.ToMatrix() * decMatrix::CreateScale( sl.scale * 2.0f ) );
			SSDTLOG("RenderGIShadowMap Prepare2 %d", (int)(timer.GetElapsedTime() * 1e6f));
	
	// the depth offset has to be scaled to be correct. this is required since the depth
	// offset from the configuration is in meters but the depth offset to apply in the
	// shader has to be in the range from 0 to 1
	// 
	// NOTE depth offset scaling is not working correctly. the shader uses dxdF/dydF for
	//      this which fails to work properly across different triangles. the correct
	//      solution requires using a geometry shader which calculates the triangle
	//      depth slope using the vZDepth calculated by the vertex shader. this are
	//      in the correct space and thus would allow to calculate the correct offset.
	//      
	//      actually a geometry shader would be required anyway to calculate and apply
	//      the depth offset without needing to manipulate the gl_FragDepth in the
	//      fragment shader. not touching gl_FragDepth is faster. using a geometry
	//      shader does make rendering more expensive but not touching gl_FragDepth
	//      should allow double-write speed to be used and should compensate
	
	// static shadow map
	const int shadowMapSize = plan.GetGIShadowSize();
	
	if( plan.GetGIShadowUpdateStatic() ){
		// parameter block
		deoglSPBlockUBO &renderParamBlock = renderThread.GetRenderers().GetLight().NextShadowPB();
		{
			const deoglSPBMapBuffer mapped( renderParamBlock );
			
			renderParamBlock.SetParameterDataMat4x3( deoglSkinShader::erutMatrixV, matrixGI );
			renderParamBlock.SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, decMatrix() );
			renderParamBlock.SetParameterDataMat4x4( deoglSkinShader::erutMatrixVP, matrixGI );
			renderParamBlock.SetParameterDataMat3x3( deoglSkinShader::erutMatrixVn,
				matrixGI.GetRotationMatrix().QuickInvert() );
			
			renderParamBlock.SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
				sl.zscale, sl.zoffset, -sl.zscale, -sl.zoffset );
			
			renderParamBlock.SetParameterDataBVec4( deoglSkinShader::erutConditions1, false, false, false, false );
		}
				SSDTLOG("RenderGIShadowMap RenderParamBlock %d", (int)(timer.GetElapsedTime() * 1e6f));
		
		// render
		shadowMapper.SetForeignSolidDepthTexture( scsolid.ObtainStaticMapWithSize( shadowMapSize, true, false ) );
		
		if( renderThread.GetChoices().GetUseComputeRenderTask() ){
			plan.GetCRTShadowGIStatic()->SetRenderParamBlock( &renderParamBlock );
			
			RenderGIShadowMap( shadowMapper, plan.GetCRTShadowGIStatic(), shadowMapSize, false /*true*/ );
			
		}else{
			plan.GetGIRenderTaskStatic().SetRenderParamBlock( &renderParamBlock );
			plan.GetGIRenderTaskAddStatic().SetForceDoubleSided( true );
			
			RenderGIShadowMap( shadowMapper, plan.GetGIRenderTaskStatic(), shadowMapSize, false /*true*/ );
		}
		
		shadowMapper.DropForeignTextures();
	}
	
	// dynamic shadow map. parameter block
	deoglSPBlockUBO &renderParamBlock = renderThread.GetRenderers().GetLight().NextShadowPB();
	{
		const deoglSPBMapBuffer mapped( renderParamBlock );
		
		// this is the same as above and could be shared
		renderParamBlock.SetParameterDataMat4x3( deoglSkinShader::erutMatrixV, matrixGI );
		renderParamBlock.SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, decMatrix() );
		renderParamBlock.SetParameterDataMat4x4( deoglSkinShader::erutMatrixVP, matrixGI );
		renderParamBlock.SetParameterDataMat3x3( deoglSkinShader::erutMatrixVn, matrixGI.GetRotationMatrix().QuickInvert() );
		
		// this though is different and requires an own parameter block to be used.
		// in general only dynamic shadow map is rendered so the situation of this
		// parameter blockt to be updated twice (and stalling) is little
		renderParamBlock.SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
			sl.zscale, sl.zoffset, -sl.zscale, -sl.zoffset );
		
		renderParamBlock.SetParameterDataBVec4( deoglSkinShader::erutConditions1, false, false, false, false );
	}
			SSDTLOG("RenderGIShadowMap RenderParamBlock %d", (int)(timer.GetElapsedTime() * 1e6f));
	
	// render
	if( scsolid.GetDynamicMap() && scsolid.GetDynamicMap()->GetWidth() != shadowMapSize ){
		scsolid.DropDynamic();
	}
	
	shadowMapper.SetForeignSolidDepthTexture( scsolid.ObtainDynamicMapWithSize( shadowMapSize, true, false ) );
	
	if( renderThread.GetChoices().GetUseComputeRenderTask() ){
		plan.GetCRTShadowGIDynamic()->SetRenderParamBlock( &renderParamBlock );
		
		RenderGIShadowMap( shadowMapper, plan.GetCRTShadowGIDynamic(), shadowMapSize, false );
		
	}else{
		plan.GetGIRenderTaskDynamic().SetRenderParamBlock( &renderParamBlock );
		plan.GetGIRenderTaskAddDynamic().SetForceDoubleSided( true );
		
		RenderGIShadowMap( shadowMapper, plan.GetGIRenderTaskDynamic(), shadowMapSize, false );
	}
	
	shadowMapper.DropForeignTextures();
	
	// clean up
	DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowGI, true );
}

void deoglRenderLightSky::RenderGIShadowMap( deoglShadowMapper &shadowMapper,
deoglRenderTask &renderTask, int shadowMapSize, bool clearBackFaceFragments ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "LightSky.RenderGIShadowMap" );
	
			#ifdef SKY_SHADOW_DEBUG_TIME
			decTimer timer;
			#endif
	
	// set up stencil mask. this is used to mark back facing fragments (see after rendering)
	// front faces set 0x0 as stencil value and back faces 0xff
	/*
	if( clearBackFaceFragments ){
		state.StencilOpFront( GL_KEEP, GL_KEEP, GL_ZERO );
		state.StencilOpBack( GL_KEEP, GL_KEEP, GL_REPLACE );
		state.StencilMask( ~0 );
		
	}else{
		state.StencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		state.StencilMask( 0x0 );
	}
	state.StencilFunc( GL_ALWAYS, ~0, ~0 );
	*/
	
	// get shadow map
			SSDTLOG("RenderGIShadowMap Prepare %d", (int)(timer.GetElapsedTime() * 1e6f));
	
	// clear shadow map
	pPipelineClearBuffers->Activate();
	shadowMapper.ActivateSolidTexture( shadowMapSize, false, true );
	
	if( clearBackFaceFragments ){
		OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, 1.0f, ~0 ) );
		
	}else{
		const GLfloat clearDepth = 1.0f;
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	}
			SSDTLOG("RenderGIShadowMap Clear %d", (int)(timer.GetElapsedTime() * 1e6f));
	
	// render shadow map. only solid content without holes is rendered at the highest lod level
	renderThread.GetRenderers().GetGeometry().RenderTask( renderTask );
			SSDTLOG("RenderGIShadowMap Render %d (s=%d t=%d v=%d i=%d si=%d)",
				(int)(timer.GetElapsedTime() * 1e6f), renderTask.GetShaderCount(), renderTask.GetTotalTextureCount(),
				renderTask.GetTotalVAOCount(), renderTask.GetTotalInstanceCount(), renderTask.GetTotalSubInstanceCount() );
	
	if( clearBackFaceFragments ){
		// clear back facing fragments. back facing fragments cause a lot of troubles to shadow
		// casting. the idea here is to consider back facing fragments hit before any front facing
		// fragments to belong to the outer hull of a map. this is similar to a fragment blocking
		// the sky light right at the near end of the shadow map. by clearing back facing fragments
		// to 0 artifacts for back faces are eliminated altogether. this though won't help with
		// issues on front facing fragments but makes underground scenes safe
		pPipelineClearDepth->Activate();
		pPipelineClearDepth->GetShader().SetParameterFloat( 0, -1.0f );
		RenderFullScreenQuadVAO();
				SSDTLOG("RenderGIShadowMap BackFaceClear %d", (int)(timer.GetElapsedTime() * 1e6f));
	}
}

void deoglRenderLightSky::RenderGIShadowMap( deoglShadowMapper &shadowMapper,
deoglComputeRenderTask &renderTask, int shadowMapSize, bool clearBackFaceFragments ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "LightSky.RenderGIShadowMapCRT" );
	
	// clear shadow map
	pPipelineClearBuffers->Activate();
	shadowMapper.ActivateSolidTexture( shadowMapSize, false, true );
	
	if( clearBackFaceFragments ){
		OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, 1.0f, ~0 ) );
		
	}else{
		const GLfloat clearDepth = 1.0f;
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	}
	
	// render shadow map. only solid content without holes is rendered at the highest lod level
	renderTask.Render();
	
	if( clearBackFaceFragments ){
		pPipelineClearDepth->Activate();
		pPipelineClearDepth->GetShader().SetParameterFloat( 0, -1.0f );
		RenderFullScreenQuadVAO();
	}
}



void deoglRenderLightSky::UpdateLightParamBlock( const deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlanSkyLight &plan ){
	int target;
	
	// get light properties
	const deoglRSkyInstanceLayer &skyLayer = *plan.GetLayer();
	const float ambientIntensity = skyLayer.GetAmbientIntensity();
	const float lightIntensity = skyLayer.GetLightIntensity();
	decColor lightColor = skyLayer.GetLightColor();
	
	lightColor.r = powf( lightColor.r, OGL_RENDER_GAMMA );
	lightColor.g = powf( lightColor.g, OGL_RENDER_GAMMA );
	lightColor.b = powf( lightColor.b, OGL_RENDER_GAMMA );
	lightColor = TransformColor( plan.GetPlan().GetWorld()->GetLightColorMatrix(), lightColor );
	
	// set values
	const deoglSPBMapBuffer mapped( paramBlock );
	const bool hasGIState = plan.GetPlan().GetRenderGIState() != NULL;
	
	target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightColor );
	if( target != -1 ){
		paramBlock.SetParameterDataVec3( target, lightColor * lightIntensity );
	}
	
	target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightColorAmbient );
	if( target != -1 ){
		if( hasGIState || plan.GetPlan().GetNoAmbientLight() ){
			paramBlock.SetParameterDataVec3( target, 0.0f, 0.0f, 0.0f );
			
		}else{
			paramBlock.SetParameterDataVec3( target, lightColor * ambientIntensity );
			// this happens usually only for env-maps. better be dark than wrong lit
			//paramBlock.SetParameterDataVec3( target, 0.0f, 0.0f, 0.0f );
		}
	}
	
	target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightColorAmbientGI );
	if( target != -1 ){
		if( plan.GetPlan().GetNoAmbientLight() ){
			paramBlock.SetParameterDataVec3( target, 0.0f, 0.0f, 0.0f );
			
		}else{
			paramBlock.SetParameterDataVec3( target, lightColor * ambientIntensity );
		}
	}
	
	// NOTE sky light is currently using a special handling which has to be replaced with the
	//      one used by point and spot lights. due to this pLightAmbientRatio can not be
	//      used in the shader since it is not defined here. to work with the new version the
	//      following changes have to be done (once the shader is up to it)
	//      
	//      - LightColor = lightColor * ( lightIntensity + ambientIntensity )
	//      - pLightAmbientRatio = ambientIntensity / ( lightIntensity + ambientIntensity )
	//      - pLightColorAmbient = not used anymore
	//      
	//      better would be to change the sky light definition to use ambient-ratio as
	//      lights do. in this case the solution becomes as simple as in the other lights:
	//      
	//      - LightColor = lightColor * lightIntensity
	//      - pLightAmbientRatio = lightAmbientRatio
	//      - pLightColorAmbient = not used anymore
	//      
	target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightAmbientRatio );
	if( target != -1 ){
		paramBlock.SetParameterDataFloat( target, 0.0f );
	}
}

void deoglRenderLightSky::UpdateInstanceParamBlock( const deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlanSkyLight &plan, int shadowMapSize, int passCount ){
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	const deoglRSkyInstanceLayer &skyLayer = *plan.GetLayer();
	float pixelSize, noiseScale;
	int target;
	
	// calculate matrices
	decMatrix matrixShadow[ 4 ], matrixShadowStereo[ 4 ], giMatrixShadow;
	float layerBorder[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float scaleZ[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	const decDMatrix matrixLayer( decMatrix::CreateFromQuaternion(
		skyLayer.GetLightOrientation() ).QuickMultiply( skyLayer.GetMatrix() ) );
	const decDMatrix matrixLV( matrixLayer.QuickMultiply( plan.GetPlan().GetRefPosCameraMatrix() ) );
	
	// for VR rendering the shadow maps have been calculated against the left eye. when the right
	// eye is rendered the geometry has to be transformed into the left eye for the shadow maps
	// to work correctly.
	decMatrix transformShadow, transformShadowStereo;
	decDMatrix matrixLVStereo;
	
	if( plan.GetPlan().GetRenderStereo() ){
		// using stereo rendering the camera matrix is the left eye camera matrix. the stereo
		// matrix has then to transform from the right eye to the left eye. the stored plan
		// camera stereo matrix transforms from the left eye to the right eye
		matrixLVStereo = matrixLV.QuickMultiply( plan.GetPlan().GetCameraStereoMatrix() );
		transformShadowStereo = plan.GetPlan().GetCameraStereoInverseMatrix();
		
	}else{
		// using non-stereo rendering the situation is slightly different. if the left eye is
		// rendered we are in the correct eye already and do not modify the shadow matrix.
		// if the right eye is rendered the shadow matrix has to be transformed from right eye
		// to left eye. in contrary to stereo rendering this modification has to be applied to
		// the base shadow map not the stereo shadow map
		transformShadow = plan.GetPlan().GetCameraStereoInverseMatrix();
	}
	
	if( plan.GetUseShadow() ){
		// planSkyLight is only properly set up if shadow casting is used
		if( 0 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetShadowLayerAt( 0 );
			matrixShadow[ 0 ] = transformShadow * sl.matrix;
			matrixShadowStereo[ 0 ] = transformShadowStereo * sl.matrix;
			layerBorder[ 0 ] = sl.layerBorder;
			scaleZ[ 0 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
		if( 1 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetShadowLayerAt( 1 );
			matrixShadow[ 1 ] = transformShadow * sl.matrix;
			matrixShadowStereo[ 1 ] = transformShadowStereo * sl.matrix;
			layerBorder[ 1 ] = sl.layerBorder;
			scaleZ[ 1 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
		if( 2 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetShadowLayerAt( 2 );
			matrixShadow[ 2 ] = transformShadow * sl.matrix;
			matrixShadowStereo[ 2 ] = transformShadowStereo * sl.matrix;
			layerBorder[ 2 ] = sl.layerBorder;
			scaleZ[ 2 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
		if( 3 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetShadowLayerAt( 3 );
			matrixShadow[ 3 ] = transformShadow * sl.matrix;
			matrixShadowStereo[ 3 ] = transformShadowStereo * sl.matrix;
			layerBorder[ 3 ] = sl.layerBorder;
			scaleZ[ 3 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
	}
	
	// only properly set up if plan has GI
	if( plan.GetPlan().GetRenderGIState() ){
		giMatrixShadow = plan.GetGIShadowLayer().matrix;
	}
	
	const deoglSPBMapBuffer mapped( paramBlock );
	
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLightView );
	if( target != -1 ){
		paramBlock.SetParameterDataArrayVec3( target, 0, matrixLV.TransformView().Normalized() );
		paramBlock.SetParameterDataArrayVec3( target, 1, matrixLVStereo.TransformView().Normalized() );
	}
	
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix1 );
	if( target != -1 ){
		paramBlock.SetParameterDataArrayMat4x4( target, 0, matrixShadow[ 0 ] );
		paramBlock.SetParameterDataArrayMat4x4( target, 1, matrixShadowStereo[ 0 ] );
	}
	
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix2 );
	if( target != -1 ){
		paramBlock.SetParameterDataArrayMat4x4( target, 0, matrixShadow[ 1 ] );
		paramBlock.SetParameterDataArrayMat4x4( target, 1, matrixShadowStereo[ 1 ] );
	}
	
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix3 );
	if( target != -1 ){
		paramBlock.SetParameterDataArrayMat4x4( target, 0, matrixShadow[ 2 ] );
		paramBlock.SetParameterDataArrayMat4x4( target, 1, matrixShadowStereo[ 2 ] );
	}
	
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix4 );
	if( target != -1 ){
		paramBlock.SetParameterDataArrayMat4x4( target, 0, matrixShadow[ 3 ] );
		paramBlock.SetParameterDataArrayMat4x4( target, 1, matrixShadowStereo[ 3 ] );
	}
	
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLightParams );
	if( target != -1 ){
		paramBlock.SetParameterDataVec4( target, 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLayerBorder );
	if( target != -1 ){
		paramBlock.SetParameterDataVec4( target, layerBorder[ 0 ], layerBorder[ 1 ],
			layerBorder[ 2 ], layerBorder[ 3 ] );
	}
	
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadow1Solid );
	if( target != -1 ){
		pixelSize = 1.0f / ( float )shadowMapSize;
		noiseScale = config.GetShadowCubePCFSize() / ( float )shadowMapSize;
		
		paramBlock.SetParameterDataVec3( target, pixelSize, pixelSize, noiseScale );
	}
	
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowDepthTransform );
	if( target != -1 ){
		paramBlock.SetParameterDataVec4( target, 1.0f, 0.0f, 1.0f / scaleZ[ 0 ], 1.0f / scaleZ[ 1 ] );
	}
	
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowDepthTransform2 );
	if( target != -1 ){
		paramBlock.SetParameterDataVec4( target, 1.0f, 0.0f, 1.0f / scaleZ[ 2 ], 1.0f / scaleZ[ 3 ] );
	}
	
	// global illumination
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutGIShadowMatrix );
	if( target != -1 ){
		paramBlock.SetParameterDataMat4x4( target, giMatrixShadow );
	}
	
	target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutGIShadowParams );
	if( target != -1 ){
		const int giShadowMapSize = plan.GetGIShadowSize();
		pixelSize = 1.0f / ( float )giShadowMapSize;
		noiseScale = config.GetShadowCubePCFSize() / ( float )giShadowMapSize;
		
		paramBlock.SetParameterDataVec3( target, pixelSize, pixelSize, noiseScale );
	}
}



void deoglRenderLightSky::ResetDebugInfo(){
	pDebugInfoSolid->Clear();
	pDebugInfoTransparent->Clear();
	
	pDebugInfoSolidShadow->Clear();
	pDebugInfoSolidShadowOcclusion->Clear();
	pDebugInfoSolidShadowSplit->Clear();
	pDebugInfoSolidShadowSplitContent->Clear();
	pDebugInfoSolidShadowSplitLODLevels->Clear();
	pDebugInfoSolidShadowSplitClear->Clear();
	pDebugInfoSolidShadowSplitTask->Clear();
	pDebugInfoSolidShadowSplitRender->Clear();
	pDebugInfoSolidShadowGI->Clear();
	pDebugInfoSolidLight->Clear();
	
	pDebugInfoTransparentLight->Clear();
}

void deoglRenderLightSky::AddTopLevelDebugInfoSolid(){
	GetRenderThread().GetDebug().GetDebugInformationList().Add( pDebugInfoSolidDetail );
}

void deoglRenderLightSky::AddTopLevelDebugInfoTransparent(){
	GetRenderThread().GetDebug().GetDebugInformationList().Add( pDebugInfoTransparentDetail );
}

void deoglRenderLightSky::DevModeDebugInfoChanged(){
	const int details = GetRenderThread().GetDebug().GetDeveloperMode().GetDebugInfoDetails();
	const bool show = ( details & ( deoglDeveloperMode::edimLight | deoglDeveloperMode::edimLightSky ) )
		== ( deoglDeveloperMode::edimLight | deoglDeveloperMode::edimLightSky );
	
	pDebugInfoSolidDetail->SetVisible( show );
	//pDebugInfoSolidShadow->SetVisible( true );
	//pDebugInfoSolidShadowOcclusion->SetVisible( true );
	//pDebugInfoSolidShadowSplit->SetVisible( true );
	
	pDebugInfoTransparentDetail->SetVisible( show );
}



// Private Functions
//////////////////////

void deoglRenderLightSky::pCleanUp(){
	if( pSolidShadowMap ){
		pSolidShadowMap->SetInUse( false );
		pSolidShadowMap = NULL;
	}
	if( pSolidShadowMap ){
		pSolidShadowMap->SetInUse( false );
		pSolidShadowMap = NULL;
	}
	
	if( pColList2 ){
		delete pColList2;
	}
}
