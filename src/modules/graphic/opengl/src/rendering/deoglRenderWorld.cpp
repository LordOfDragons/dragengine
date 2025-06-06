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

#include "deoglRenderDevMode.h"
#include "deoglRenderGeometry.h"
#include "deoglRenderGeometryPass.h"
#include "deoglRenderReflection.h"
#include "deoglRenderToneMap.h"
#include "deoglRenderTransparentPasses.h"
#include "deoglRenderTranspCounting.h"
#include "deoglRenderWorld.h"
#include "deoglRenderVR.h"
#include "debug/deoglRenderDebugDrawer.h"
#include "defren/deoglDeferredRendering.h"
#include "defren/deoglDRDepthMinMax.h"
#include "light/deoglRenderLight.h"
#include "light/deoglRenderGI.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanMasked.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglAddToRenderTaskParticles.h"
#include "task/deoglRenderTask.h"
#include "task/deoglRenderTaskParticles.h"
#include "../component/deoglRComponent.h"
#include "../debug/deoglDebugSnapshot.h"
#include "../debug/deoglDebugInformation.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../effects/render/deoglREffect.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../gi/deoglGIState.h"
#include "../gi/deoglGICascade.h"
#include "../particle/deoglParticleSorter.h"
#include "../pipeline/deoglPipelineConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
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
#include "../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../sky/deoglRSky.h"
#include "../sky/deoglRSkyInstance.h"
#include "../sky/deoglRSkyLayer.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../world/deoglRCamera.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eSPDepthOnly{
	spdoMatrixMVP,
	spdoMatrixMV,
	spdoMatrixDiffuse,
	spdoMaterialGamma,
	spdoClipPlane,
	spdoViewport,
	spdoPFMatrix
};

enum eSPTraCountMaxCount{
	sptcmcPosTransform,
	sptcmcTCTransform,
	sptcmcClampTC,
	sptcmcOffsets1,
	sptcmcOffsets2,
	sptcmcOffsets3,
	sptcmcOffsets4
};

enum eSPTraCountGetCount{
	sptcgcPosTransform,
	sptcgcThresholdTransform
};

enum eSPFinalize{
	spfinTCTransform,
	spfinGamma,
	spfinBrightness,
	spfinContrast
};

enum eSPCopyDepth{
	spcdQuadParams
};

enum eSPDepthDownsample{
	spddsTCClamp,
	spddsMipMapLevel
};

//#define REAL_TRANSP_PARTICLES		1

#define DEBUG_SNAPSHOT_TRANSPARENCY_COUNTING 10
#define DEBUG_SNAPSHOT_TRANSPARENCY_PASSES   20



// #define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer dtimer;

#define DEBUG_RESET_TIMER \
	dtimer.Reset();
#define DEBUG_PRINT_TIMER(what) \
	renderThread.GetLogger().LogInfoFormat( "World %s = %iys", what, \
		( int )( dtimer.GetElapsedTime() * 1000000.0 ) );\
	dtimer.Reset();
#else
#define DEBUG_RESET_TIMER
#define DEBUG_PRINT_TIMER(what)
#endif



// Class deoglRenderWorld
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderWorld::deoglRenderWorld( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread ),

pRenderTask( nullptr ),
pAddToRenderTask( nullptr ),
pParticleSorter( nullptr ),
pRenderTaskParticles( nullptr ),
pAddToRenderTaskParticles( nullptr ),

pDebugInfo( renderThread )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const bool useFSQuadStereoVSLayer = renderThread.GetChoices().GetRenderFSQuadStereoVSLayer();
	deoglShaderDefines defines, commonDefines;
	const deoglShaderSources *sources;
	deoglPipelineConfiguration pipconf, pipconf2;
	
	try{
		renderThread.GetShader().SetCommonDefines( commonDefines );
		
		pRenderPBSingleUse.TakeOver( new deoglSPBSingleUse( renderThread,
			deoglSkinShader::CreateSPBRender( renderThread ) ) );
		
		pRenderTask = new deoglRenderTask( renderThread );
		pAddToRenderTask = new deoglAddToRenderTask( renderThread, *pRenderTask );
		
		pParticleSorter = new deoglParticleSorter;
		pRenderTaskParticles = new deoglRenderTaskParticles;
		pAddToRenderTaskParticles = new deoglAddToRenderTaskParticles( renderThread, pRenderTaskParticles );
		
		
		
		// finlize
		pipconf.Reset();
		pipconf.SetDepthMask( false );
		pipconf.SetEnableScissorTest( true );
		
		sources = shaderManager.GetSourcesNamed( "DefRen Finalize" );
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM" );
		pAsyncGetPipeline(pPipelineFinalize, pipconf, sources, defines);
		
		pipconf2 = pipconf;
		pipconf2.EnableBlendBlend();
		pAsyncGetPipeline(pPipelineFinalizeBlend, pipconf2, sources, defines);
		
		// finalize stereo
		defines.SetDefines( useFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! useFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen Finalize Stereo" );
		}
		pAsyncGetPipeline(pPipelineFinalizeStereo, pipconf, sources, defines);
		
		pipconf2 = pipconf;
		pipconf2.EnableBlendBlend();
		pAsyncGetPipeline(pPipelineFinalizeBlendStereo, pipconf2, sources, defines);
		
		// finalize split
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM", "SPLIT_LAYERS" );
		pAsyncGetPipeline(pPipelineFinalizeSplit, pipconf, "DefRen Finalize Split", defines);
		
		
		
		DevModeDebugInfoChanged();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderWorld::~deoglRenderWorld(){
	pCleanUp();
}



// Rendering
//////////////

//#define ENABLE_DEBUG_ENTER_EXIT 1
#ifdef OS_ANDROID
// 	#define ENABLE_DEBUG_ENTER_EXIT 1
#endif

#ifdef ENABLE_DEBUG_ENTER_EXIT
#define DBG_ENTER(x) GetRenderThread().GetLogger().LogInfo("RenderWorld." x ": ENTER");
#define DBG_ENTER_PARAM(x,pt,pv) GetRenderThread().GetLogger().LogInfoFormat("RenderWorld." x ": ENTER [" pt "]", pv);
#define DBG_ENTER_PARAM2(x,pt1,pv1,pt2,pv2) GetRenderThread().GetLogger().LogInfoFormat("RenderWorld." x ": ENTER [" pt1 "," pt2 "]", pv1, pv2);
#define DBG_ENTER_PARAM3(x,pt1,pv1,pt2,pv2,pt3,pv3) GetRenderThread().GetLogger().LogInfoFormat("RenderWorld." x ": ENTER [" pt1 "," pt2 "," pt3 "]", pv1, pv2, pv3);
#define DBG_EXIT(x) GetRenderThread().GetLogger().LogInfo("RenderWorld." x ": EXIT");
#else
#define DBG_ENTER(x)
#define DBG_ENTER_PARAM(x,pt,pv)
#define DBG_ENTER_PARAM2(x,pt1,pv1,pt2,pv2)
#define DBG_ENTER_PARAM3(x,pt1,pv1,pt2,pv2,pt3,pv3)
#define DBG_EXIT(x)
#endif


void deoglRenderWorld::RenderBlackScreen( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglDebugTraceGroup debugTrace( renderThread, "World.RenderBlackScreen" );
	
	defren.InitPostProcessTarget();
	defren.ActivatePostProcessFBO( true );
	
	pPipelineClearBuffers->Activate();
	SetViewport( plan );
	
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	// swap so the caller finds the final image to work with in GetPostProcessTexture
	defren.SwapPostProcessTarget();
}

void deoglRenderWorld::RenderWorld( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask ){
DBG_ENTER_PARAM("RenderWorld", "%p", mask)
DEBUG_RESET_TIMER
//	bool maskedRendering = info->GetUseRenderMask();
//	deoglRenderableColorTexture *maskRenderTexture = info->GetMaskRenderTexture();
	
	const bool debugMainPass = ! mask;
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, mask ? "World.RenderWorld(Masked)" : "World.RenderWorld" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
//	deoglCollideList &colList = plan.GetCollideList();
	deoglDeveloperMode &devMode = renderThread.GetDebug().GetDeveloperMode();
	deoglRTRenderers &renderers = GetRenderThread().GetRenderers();
	deoglRWorld &world = *plan.GetWorld();
	deoglShaderCompiled *shader;
//	int c, componentCount;
	
	// debug if enabled in plan and this is the main pass
	if( debugMainPass && plan.GetDebugTiming() ){
		pDebugInfo.ClearAll();
		renderers.GetLight().ResetDebugInfo();
		DebugTimersReset( plan, true );
	}
	
	// prepare for rendering
	const bool disableLights = plan.GetDisableLights();
	
	if( disableLights ){
		renderers.GetGeometry().SetAmbient( decColor( 1.0f, 1.0f, 1.0f ) );
		
	}else{
		decColor ambient = world.GetAmbientLight();
		ambient.r = powf( ambient.r, OGL_RENDER_GAMMA );
		ambient.g = powf( ambient.g, OGL_RENDER_GAMMA );
		ambient.b = powf( ambient.b, OGL_RENDER_GAMMA );
		
		const decVector vector( world.GetLightColorMatrix() * decVector( ambient.r, ambient.g, ambient.b ) );
		renderers.GetGeometry().SetAmbient( decColor( vector.x, vector.y, vector.z ) );
	}
	
	// set up the render parameter shader parameter block
	PrepareRenderParamBlock( plan, mask );
	
	// this part is done only for the main render pass
	SetViewport( plan );
	
	// set the stencil mask
	plan.SetRenderPassNumber( 1 );
	
	if( mask ){
		// if there is a mask set the stencil write mask to only affect the
		// bits of the render pass number and use the mask to filter
		plan.SetStencilRefValue( 0x03 ); // ( renderPassNumber << 1 ) | 0x01
		plan.SetStencilWriteMask( 0xfe );
		
	}else{
		// if there is no mask we set the mask bit to 0 and write to all stencil
		// bits. this way the mask bit is already cleared for the first masked
		// render level we encounter. no filtering for the mask is used
		plan.SetStencilRefValue( 0x02 ); // ( renderPassNumber << 1 ) | 0x00
		plan.SetStencilWriteMask( 0xff );
	}
	
	// render masked stuff. works before calling ClearScreen since it handles clearing the
	// depth buffer internally. after finishing rendering the mask passes we have to do
	// ClearScreen but protecting color otherwise the masks are overwritten
	if( mask ){
// 		ClearScreen( plan, false, true );
		
	}else{
		RenderMaskedPass( plan );
		DebugTimer2Sample( plan, *pDebugInfo.infoPassMasked, true );
		
// 		ClearScreen( plan, false, false );
	}
	
	// prepare for one render turn with this camera
	plan.PrepareRenderOneTurn();
	
	// trace global illumination rays if in main render pass
	if( ! mask ){
		deoglRenderGI &renderGI = renderThread.GetRenderers().GetLight().GetRenderGI();
		if( plan.GetUpdateGIState() ){
			renderGI.TraceRays( plan );
			DebugTimer2Sample( plan, *pDebugInfo.infoGITraceRays, true );
			
			// calculate probe offset and extends. done here to avoid stalling since the results
			// are read during the next frame update. for offsets the result is also used later
			// on to update offset texture when not used anymore. this has no stalling problem
			// since the rendering happens sequentially on the GPU
			renderGI.ProbeOffset( plan );
			renderGI.ProbeExtends( plan );
			SetViewport( plan );
			
			// start transfer data to client memory asynchronously for use in the next update
			plan.GetUpdateGIState()->StartReadBack();
		}
	}
	
	// solid pass
	//if( ! plan->GetFBOTarget() )
	if( renderThread.GetChoices().GetUseComputeRenderTask() ){
		plan.GetTasks()->FinishReadBackComputeRenderTasks( mask );
	}
	renderers.GetGeometryPass().RenderSolidGeometryPass( plan, mask, false );
	if( debugMainPass ){
		DebugTimer2Sample( plan, *pDebugInfo.infoSolidGeometry, true );
	}
	
	DBG_ENTER("RenderDepthMinMaxMipMap")
	renderers.GetReflection().RenderDepthMinMaxMipMap( plan );
	DBG_EXIT("RenderDepthMinMaxMipMap")
	
	if( deoglSkinShader::REFLECTION_TEST_MODE == deoglSkinShader::ertmOwnPassReflection ){
		// NOTE actually this requires updated GI probes but this happens below during RenderLights().
		//      this is though not that much of a trouble. it only causes environment maps to be
		//      GI light one frame behind
		DBG_ENTER("RenderGIEnvMaps")
		renderers.GetReflection().RenderGIEnvMaps( plan );
		DBG_EXIT("RenderGIEnvMaps")
		
		DBG_ENTER("RenderReflections")
		renderers.GetReflection().RenderReflections( plan );
		DBG_EXIT("RenderReflections")
	}
	if( debugMainPass ){
		DebugTimer2Sample( plan, *pDebugInfo.infoReflection, true );
	}
	
	// plan transparency. this affects lighting thus it comes earlier than RenderTransparentPasses
	if( plan.GetHasTransparency() ){
		#ifdef WITH_OPENGLES
		plan.PlanTransparency( plan.GetTransparencyLayerCount() );
		#else
		plan.PlanTransparency( renderers.GetTransparencyCounter().GetCount() );
		#endif
	}else{
		plan.PlanTransparency( 0 );
	}
	
	//if( ! plan->GetFBOTarget() )
	if( ! plan.GetDisableLights() ){
		if( ! mask ){
			renderers.GetToneMap().LuminancePrepare( plan );
// 			renderers.GetGeometryPass().RenderLuminanceOnly( plan );
			DebugTimer2Sample( plan, *pDebugInfo.infoLuminancePrepare, true );
		}
		
		if( ! mask ){
			DBG_ENTER("EnvMapCopyMaterials")
			renderers.GetReflection().CopyMaterial( plan, true );
			DBG_EXIT("EnvMapCopyMaterials")
		}
		
		DBG_ENTER("RenderLights")
		renderers.GetLight().RenderLights( plan, true, mask, false );
		DBG_EXIT("RenderLights")
		if( debugMainPass ){
			DebugTimer2Sample( plan, *pDebugInfo.infoSolidGeometryLights, true );
		}
	}
	
	if( deoglSkinShader::REFLECTION_TEST_MODE != deoglSkinShader::ertmOwnPassReflection ){
		DBG_ENTER("RenderGIEnvMaps")
		renderers.GetReflection().RenderGIEnvMaps( plan );
		DBG_EXIT("RenderGIEnvMaps")
	}
	
	DBG_ENTER("RenderReflectionScreenSpace")
	renderers.GetReflection().RenderScreenSpace( plan );
	DBG_EXIT("RenderReflectionScreenSpace")
	if( debugMainPass ){
		DebugTimer2Sample( plan, *pDebugInfo.infoSSR, true );
	}
	
	// transparenc passes
// 	if( ! plan.GetFBOTarget() ){
		renderers.GetTransparentPasses().RenderTransparentPasses( plan, mask, false );
		if( debugMainPass ){
			DebugTimer2Sample( plan, *pDebugInfo.infoTransparent, true );
		}
// 	}
	
	// xray pass
	if( plan.GetTasks()->GetSolidDepthXRayTask().GetPipelineCount() > 0
	|| plan.GetTasks()->GetSolidDepthOutlineXRayTask().GetPipelineCount() > 0
	|| plan.GetHasXRayTransparency() ){
		// - switch texture but not with secondary depth texture but with third depth texture.
		//   required for depth textures to stay usable for upcoming render steps.
		//   
		//   possible new texture properties:
		//   - xray.color  // default black
		//   - xray.color.tint   // default white
		//   - xray.color.tint.mask   // default white
		//   - xray.color.blend  // blend factor between non-xray color (0) and xray color (1).
		//                       // hence to just darken xray geometry just use blend since
		//                       // color is black by default.
		//                       // default value 0
		//   
		//   - xray.emissivity  // default 0
		//   - xray.emissivity.tint  // default white
		//   - xray.emissivity.intensity  // default 0
		//   - xray.emissivity.blend  // blend factor between non-xray emissivity (0) and xray emissivity (1)
		//   
		//   - xray.outline  // apply xray to outline. 'xray' only applies to non-outline
		//   
		//   - xray.outline.color  // default black
		//   - xray.outline.color.tint  // default black
		//   
		//   - xray.outline.emissivity  // emissivity to add to outline emissivity
		// - clear the depth texture
		// - render below
		// - switch texture with third depth texture. this restores the depth texture for
		//   upcoming post processing (and future VR motion texture support). ignoring the
		//   XRay depth is fine since this only renders what is hidden and does not change
		//   the front most depth
		const deoglDebugTraceGroup debugTraceXRay( renderThread, "World.XRay" );
		
		// render solid geometry pass. this clears the depth texture
		renderers.GetGeometryPass().RenderSolidGeometryPass( plan, mask, true );
		if( debugMainPass ){
			DebugTimer2Sample( plan, *pDebugInfo.infoSolidGeometry, true );
		}
		
		// reflections
		renderers.GetReflection().RenderDepthMinMaxMipMap( plan );
		
		if( deoglSkinShader::REFLECTION_TEST_MODE == deoglSkinShader::ertmOwnPassReflection ){
			renderers.GetReflection().RenderReflections( plan );
			if( debugMainPass ){
				DebugTimer2Sample( plan, *pDebugInfo.infoReflection, true );
			}
		}
		
		// lighting
		if( plan.GetHasXRayTransparency() ){
			#ifdef WITH_OPENGLES
			plan.PlanTransparency( plan.GetTransparencyLayerCount() );
			#else
			plan.PlanTransparency( renderers.GetTransparencyCounter().GetCount() );
			#endif
		}else{
			plan.PlanTransparency( 0 );
		}
		
		if( ! plan.GetDisableLights() ){
			if( ! mask ){
				renderers.GetToneMap().LuminancePrepare( plan );
				DebugTimer2Sample( plan, *pDebugInfo.infoLuminancePrepare, true );
				
				renderers.GetReflection().CopyMaterial( plan, true );
			}
			
			renderers.GetLight().RenderLights( plan, true, mask, true );
			if( debugMainPass ){
				DebugTimer2Sample( plan, *pDebugInfo.infoSolidGeometryLights, true );
			}
		}
		
		// reflections
		renderers.GetReflection().RenderScreenSpace( plan );
		if( debugMainPass ){
			DebugTimer2Sample( plan, *pDebugInfo.infoSSR, true );
		}
		
		// transparency
		if( plan.GetHasXRayTransparency() ){
			renderers.GetTransparentPasses().RenderTransparentPasses( plan, mask, true );
			if( debugMainPass ){
				DebugTimer2Sample( plan, *pDebugInfo.infoTransparent, true );
			}
		}
	}
	
	// this happens only in the main pass
	if( ! mask ){
		// tone mapping
		if( disableLights || ! plan.GetUseToneMap() ){
			const deoglDebugTraceGroup debugTraceToneMap( renderThread, "World.ToneMap" );
			const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineFinalizeStereo : *pPipelineFinalize;
			pipeline.Activate();
			
			defren.ActivateFBOTemporary2( false );
			tsmgr.EnableArrayTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
			
			pRenderPB->Activate();
			
			shader = &pipeline.GetShader();
			defren.SetShaderParamFSQuad( *shader, spfinTCTransform );
			shader->SetParameterFloat( spfinGamma, 1.0f, 1.0f, 1.0f, 1.0f );
			shader->SetParameterFloat( spfinBrightness, 0.0f, 0.0f, 0.0f, 0.0f );
			shader->SetParameterFloat( spfinContrast, 1.0f, 1.0f, 1.0f, 1.0f );
			
			RenderFullScreenQuadVAO( plan );
			
		}else{
			DBG_ENTER("RenderToneMap")
			renderers.GetToneMap().ToneMap( plan );
			DBG_EXIT("RenderToneMap")
			DebugTimer2Sample( plan, *pDebugInfo.infoToneMapping, true );
		}
		
		// TODO due to the deferred rendering using transparency the depth buffer itself is
		// unusable. the depth is stored in the encoded ( or depth ) texture. the upcoming
		// shaders working with the depth values have to be modified to use the depth
		// texture instead of the depth unit. another solution would be to write a pass in
		// which only the depth texture is written to the depth populating the depth texture
		// ( and therefore the depth unit ) with the proper values. solution pending.
		
		// render shadeless objects
		/*
		pRenderShadelessPass( plan, mask );
		*/
		
		// post processing
		const deoglDebugTraceGroup debugTracePostProcess( renderThread, "World.PostProcess" );
		
		//if( ! disableLights && maskedRenderTexture ){
		//	plan->InitRenderTextures( defren.GetColorTexture(), defren.GetDiffuseTexture() );
		//	
		//}else{
			defren.InitPostProcessTarget();
		//}
		defren.ActivatePostProcessFBO( true );
		
		RenderAntiAliasingPass();
		
		if( plan.GetRenderDebugPass() ){
			RenderDebugDrawers( plan );
			renderers.GetLight().GetRenderGI().RenderDebugOverlay( plan );
			
			//pRenderDebugging( info );
			DebugTimer2Sample( plan, *pDebugInfo.infoDebugDrawers, true );
		}
		
		// effects
		RenderEffects( plan );
		DebugTimer2Sample( plan, *pDebugInfo.infoPostProcessing, true );
		
		// rendering time measuring finishes here since RenderDevMode uses it
		DebugTimer1Sample( plan, *pDebugInfo.infoWorld, true );
		
		// debug information
		if( plan.GetDebugTiming() && devMode.GetEnabled() ){
			// only in canvas rendering and main pass or VR depending if VR is used
			if( ( renderThread.GetVRCamera() && plan.GetRenderVR() != deoglRenderPlan::ervrNone )
			|| ( ! renderThread.GetVRCamera() && ! plan.GetFBOTarget() ) ){
				// measuring the time required for rendering developer mode is tricky. the reason is
				// that we need to sample the time across this function call. the sampling is present
				// only after the function call returns which draws the measurements. we can not
				// simply delay the result and show it the next time since we also need to clear
				// at some time. to solve this a temporary measurement container is used. before
				// showing the result the temporary measurement is copied to the real measurement
				// which is then displayed while the temporary one is clear
				pDebugInfo.infoDeveloperMode->CopyResults( *pDebugInfo.infoDeveloperModeTemp );
				pDebugInfo.infoDeveloperModeTemp->Clear();
				renderers.GetDevMode().RenderDevMode( plan );
				DebugTimer1Sample( plan, *pDebugInfo.infoDeveloperModeTemp, true );
			}
		}
		
		// swap one last time so the caller finds the final image to work with in GetPostProcessTexture
		defren.SwapPostProcessTarget();
		DEBUG_PRINT_TIMER( "Total" )
	}
DBG_EXIT("RenderWorld")
}



void deoglRenderWorld::PrepareRenderParamBlock( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask ){
DBG_ENTER_PARAM("PrepareRenderParamBlock", "%p", mask)
	deoglRenderThread &renderThread = GetRenderThread();
	const decDMatrix matrixEnvMap( plan.GetRefPosCameraMatrix().GetRotationMatrix().Invert() );
	const decColor &ambient = renderThread.GetRenderers().GetGeometry().GetAmbient();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const decDMatrix &matrixProjection = plan.GetProjectionMatrix();
	const decDMatrix &matrixCamera = plan.GetRefPosCameraMatrix();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const decMatrix matrixSkyBody( matrixCamera.GetRotationMatrix() * matrixProjection );
	const int height = defren.GetHeight();
	const int width = defren.GetWidth();
	float envMapLodLevel = 1.0f;
	
	// sharpness indicates the cone angle from 0 to 90 degrees. at 45 degrees a single cube map face is required
	// to be sampled. hence 0.5 sharpness has to pick the max lod level from the environment map. to determine
	// the lod level the log2 from the size times the double-sharpness can be used. to avoid a bad look at very
	// dull values the lod level is clamped to the max level minus one.
	deoglEnvironmentMap * const envmapSky = plan.GetWorld()->GetSkyEnvironmentMap();
	if( envmapSky ){
		//envMapLodLevel = ( float )envmapSky->GetMaxMipMapLevel() * 2.0f;
		//envMapLodLevel = ( float )( envmapSky->GetSize() - 1 ) * 2.0f;
		envMapLodLevel = ( float )envmapSky->GetSize() * 1.0f;
	}
	
	// clip plane if used
	decVector clipPlaneNormal( 0.0f, 0.0f, 1.0f );
	decVector clipPlaneNormalStereo( 0.0f, 0.0f, 1.0f );
	float clipPlaneDistance = 0.0f;
	float clipPlaneDistanceStereo = 0.0f;
	
	if( mask && mask->GetUseClipPlane() ){
		clipPlaneNormal = mask->GetClipNormal();
		clipPlaneDistance = mask->GetClipDistance();
		clipPlaneNormalStereo = mask->GetClipNormalStereo();
		clipPlaneDistanceStereo = mask->GetClipDistanceStereo();
	}
	
	// particle light hack
	decColor particleLight = GetRenderThread().GetRenderers().GetGeometry().GetAmbient();
	
	const int skyCount = plan.GetSkyInstanceCount();
	int i;
	for( i=0; i<skyCount; i++ ){
		deoglRSky &sky = *plan.GetSkyInstanceAt( i )->GetRSky();
		const int layerCount = sky.GetLayerCount();
		int j;
		
		for( j=0; j<layerCount; j++ ){
			const deoglRSkyLayer &layer = sky.GetLayerAt( j );
			particleLight += layer.GetLightColor() * layer.GetLightIntensity();
		}
	}
	
	// mip map params
	int mipmapMaxLevel = 0;
	{
	int mipMapHeight = height;
	int mipMapWidth = width;
	while( mipMapWidth > 1 && mipMapHeight > 1 ){
		mipMapWidth = decMath::max( mipMapWidth >> 1, 1 );
		mipMapHeight = decMath::max( mipMapHeight >> 1, 1 );
		mipmapMaxLevel++;
	}
	}
	const float mipMapFactor = ( float )plan.GetProjectionMatrix().a11 * 0.5f;
	const float mipMapPixelSizeU = mipMapFactor * width;
	const float mipMapPixelSizeV = mipMapFactor * height;
	const float mipMapMaxScale = ( float )( 1 << mipmapMaxLevel );
	
	// ssao
	const float ssaoRandomAngleConstant = 6.2831853f * config.GetSSAOTurnCount(); // 2 * pi * turns
	const float ssaoSelfOcclusion = 1.0f - cosf( config.GetSSAOSelfOcclusionAngle() * DEG2RAD );
	const float ssaoEpsilon = 1e-5f;
	const float ssaoScale = 2.0f; // sigma * 2.0
	const float ssaoTapCount = ( float )config.GetSSAOTapCount();
	const float ssaoRadius = config.GetSSAORadius();
	const float ssaoInfluenceRadius = config.GetSSAORadius();
	const float ssaoRadiusLimit = config.GetSSAORadiusLimit();
	const float ssaoRadiusFactor = ( float )plan.GetProjectionMatrix().a11 * 0.5f;
	const int ssaoMaxSize = ( width > height ) ? width : height;
	const float ssaoMipMapMaxLevel = floorf( log2f( ( float )ssaoMaxSize ) - 3.0f );
	const float ssaoMipMapBase = log2f( config.GetSSAOMipMapBase() );
	
	// sssss
	const float sssssLargestPixelSize = decMath::max( defren.GetPixelSizeU(), defren.GetPixelSizeV() );
	const float sssssDropSubSurfaceThreshold = 0.001f; // config
	const int sssssTapCount = 18; //9; //18; // config: 9-18
	const int sssssTurnCount = 7; //5; //7; // config
	const float sssssTapRadiusLimit = 0.5f; // 50% of screen size
	const float sssssTapRadiusFactor = ( float )plan.GetProjectionMatrix().a11 * 0.5f;
	const float sssssTapDropRadiusThreshold = sssssLargestPixelSize * 1.5f; // 1 pixel radius (1.44 at square boundary)
	
	// ssr
	const float ssrInvCoverageEdgeSize = 1.0f / config.GetSSRCoverageEdgeSize();
	const decVector2 ssrCoverageFactor( -ssrInvCoverageEdgeSize, ssrInvCoverageEdgeSize * 0.5f );
	const float ssrPowerEdge = config.GetSSRCoveragePowerEdge();
	const float ssrPowerRayLength = config.GetSSRCoveragePowerRayLength();
	const float ssrClipReflDirNearDist = plan.GetCameraImageDistance() * 0.9f;
	const int ssrRoughnessTapMax = 5; //20;
	const float ssrRoughnessTapRange = 0.1f;
	const float ssrRoughnessTapCountScale = ( float )ssrRoughnessTapMax / ssrRoughnessTapRange;
	const int ssrStepCount = config.GetSSRStepCount();
	const int ssrMaxRayLength = decMath::max( ssrStepCount, ( int )(
		config.GetSSRMaxRayLength() * decMath::max( width, height ) ) );
	const int ssrSubStepCount = int( floorf( log2f( ( float )ssrMaxRayLength / ( float )ssrStepCount ) ) ) + 1;
	decVector2 ssrMinMaxTCFactor;
	
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
		ssrMinMaxTCFactor.x = 0.5f * ( float )defren.GetRealWidth() / ( float )defren.GetDepthMinMax().GetWidth();
		ssrMinMaxTCFactor.y = 0.5f * ( float )defren.GetRealHeight() / ( float )defren.GetDepthMinMax().GetHeight();
	}
	
	// lighting
	const deoglGIState * const giState = plan.GetRenderGIState();
	decDMatrix giMatrix, giMatrixNormal;
	int giHighestCascade = 0;
	
	if( giState ){
		giMatrix = decDMatrix::CreateTranslation( giState->GetActiveCascade().GetPosition() ) * plan.GetCameraMatrix();
		giMatrixNormal = giMatrix.GetRotationMatrix().QuickInvert();
		giHighestCascade = giState->GetCascadeCount() - 1;
	}
	
	// tone mapping
	const deoglRCamera * const oglCamera = plan.GetCamera();
	float toneMapWhiteScale = 2.0f;
	float toneMapBloomIntensity = 1.0f;
	float toneMapBloomStrength = 0.5f;
	float toneMapBloomBlend = 1.0f;
	float toneMapAdaptationTime = 0.0f;
	float toneMapExposure = 1.0f;
	float toneMapLowInt = 0.0f;
	float toneMapHighInt = 1.0f;
	
	if( oglCamera ){
		toneMapAdaptationTime = oglCamera->GetAdaptionTime(); // 0.1 for good lighting condition ( 0.4 for very bad )
		
		if( toneMapAdaptationTime < 0.001f || oglCamera->GetForceToneMapAdaption() ){
			//toneMapAdaptationTime = 4.0; //1.0f; // required for the time being for the longer darkness adjust hack
			toneMapAdaptationTime = 100.0f; // hack for the time being. shader does clamp(value*0.25,0,1) in the worst case
			
		}else{
			toneMapAdaptationTime = 1.0f - expf( -oglCamera->GetElapsedToneMapAdaption() / toneMapAdaptationTime );
			//toneMapAdaptationTime = oglCamera->GetElapsedToneMapAdaption() * adaptationTime;
		}
		
		toneMapExposure = oglCamera->GetExposure();
		toneMapLowInt = oglCamera->GetLowestIntensity();
		toneMapHighInt = oglCamera->GetHighestIntensity();
		toneMapWhiteScale = ( oglCamera->UseCustomToneMapCurve() ? 1.0f : 3.0f ) / oglCamera->GetWhiteIntensity();
		toneMapBloomIntensity = oglCamera->GetBloomIntensity();
		toneMapBloomStrength = oglCamera->GetBloomStrength();
		toneMapBloomBlend = oglCamera->GetBloomBlend();
		// oglCamera->GetBloomSize();
	}
	
	// render all debug shapes with a z-offset to avoid z-fighting for shapes overlapping rendered
	// geometry. doing this by default is okay since debug drawers are supposed to be rendered
	// after world geometry and thus winning over world geometry feels logic.
	// pglPolygonOffset can not be used since depth has to be compared in the fragment shader. this
	// is the case since the depth comes from a texture due to the depth buffer being undefined.
	// to solve this problem the shift is placed into the camera-projection matrix after the
	// projection. the z value has to be shifted slightly into the negative direction. the shift
	// has to be some depth units which is 1/(1<<precision) where precision is depth-buffer-buts
	// minus some bits to counter rounding errors. shifting by 1 or 2 depth bits should help.
	// too much and geometry pops up behind geometry
	const double debugDepthScale = 1.0 - 1.0 / ( double )( 1 << 22 ); // 24bits minus 2 bits
	const double debugDepthShift = -1.0 / ( double )( 1 << 21 ); // 24bits minus 3 bits
	
	// stereo rendering
	const decMatrix &cameraStereoMatrix = plan.GetCameraStereoMatrix();
	
	// conditions, aka specializations
	const bool condClipPlane = mask && mask->GetUseClipPlane();
	
	// fill parameter blocks
	pRenderPB = ( deoglSPBlockUBO* )pRenderPBSingleUse->Next();
	
	deoglSPBlockUBO * const spbBlocks[ 1 ] = { pRenderPB };
	
	for( i=0; i<1; i++ ){
		deoglSPBlockUBO &spb = *spbBlocks[ i ];
		const deoglSPBMapBuffer mapped( spb );
		
		spb.SetParameterDataVec4( deoglSkinShader::erutAmbient, ambient, 1.0f );
		spb.SetParameterDataMat3x3( deoglSkinShader::erutMatrixEnvMap, matrixEnvMap );
		
		spb.SetParameterDataArrayMat4x3( deoglSkinShader::erutMatrixV, 0, matrixCamera );
		spb.SetParameterDataArrayMat4x4( deoglSkinShader::erutMatrixP, 0, matrixProjection );
		spb.SetParameterDataArrayMat4x4( deoglSkinShader::erutMatrixVP, 0, matrixCamera * matrixProjection );
		spb.SetParameterDataArrayMat3x3( deoglSkinShader::erutMatrixVn, 0, matrixCamera.GetRotationMatrix().Invert() );
		spb.SetParameterDataArrayMat4x4( deoglSkinShader::erutMatrixSkyBody, 0, matrixSkyBody );
		spb.SetParameterDataArrayVec4( deoglSkinShader::erutDepthToPosition, 0, plan.GetDepthToPosition() );
		spb.SetParameterDataArrayVec2( deoglSkinShader::erutDepthToPosition2, 0, plan.GetDepthToPosition2() );
		
		spb.SetParameterDataFloat( deoglSkinShader::erutEnvMapLodLevel, envMapLodLevel );
		spb.SetParameterDataFloat( deoglSkinShader::erutNorRoughCorrStrength, config.GetNormalRoughnessCorrectionStrength() );
		
		spb.SetParameterDataBool( deoglSkinShader::erutSkinDoesReflections, ! config.GetSSREnable() );
		spb.SetParameterDataBool( deoglSkinShader::erutFlipCulling, plan.GetFlipCulling() );
		spb.SetParameterDataFloat( deoglSkinShader::erutClearDepthValue, renderThread.GetChoices().GetClearDepthValueRegular() );
		
		defren.SetShaderViewport( spb, deoglSkinShader::erutViewport, true );
		spb.SetParameterDataIVec4( deoglSkinShader::erutViewportImage, 0, 0, width - 1, height - 1 );
		spb.SetParameterDataArrayVec4( deoglSkinShader::erutClipPlane, 0, clipPlaneNormal, clipPlaneDistance );
		spb.SetParameterDataArrayVec4( deoglSkinShader::erutClipPlane, 1, clipPlaneNormalStereo, clipPlaneDistanceStereo );
		spb.SetParameterDataVec4( deoglSkinShader::erutScreenSpace,
			defren.GetScalingU(), defren.GetScalingV(), defren.GetPixelSizeU(), defren.GetPixelSizeV() );
		spb.SetParameterDataVec4( deoglSkinShader::erutDepthOffset, 0.0f, 0.0f, 0.0f, 0.0f );
		
		spb.SetParameterDataVec2( deoglSkinShader::erutRenderSize, ( float )width, ( float )height );
		spb.SetParameterDataUVec2( deoglSkinShader::erutRenderSizeCompute, width, height );
		
		spb.SetParameterDataVec4( deoglSkinShader::erutMipMapParams, mipMapPixelSizeU, mipMapPixelSizeV, ( float )mipmapMaxLevel, mipMapMaxScale );
		
		spb.SetParameterDataVec3( deoglSkinShader::erutParticleLightHack, particleLight );
		
		spb.SetParameterDataFloat( deoglSkinShader::erutBillboardZScale, tanf( plan.GetCameraFov() * 0.5f ) );
		
		spb.SetParameterDataVec2( deoglSkinShader::erutCameraRange, plan.GetCameraImageDistance(), plan.GetCameraViewDistance() );
		
		if( plan.GetDisableLights() ){
			spb.SetParameterDataFloat( deoglSkinShader::erutCameraAdaptedIntensity, 1.0f );
			
		}else if( plan.GetCamera() ){
			spb.SetParameterDataFloat( deoglSkinShader::erutCameraAdaptedIntensity,
				plan.GetCamera()->GetLastAverageLuminance() / config.GetHDRRSceneKey() );
			
		}else{
			spb.SetParameterDataFloat( deoglSkinShader::erutCameraAdaptedIntensity,
				plan.GetCameraAdaptedIntensity() );
		}
		
		spb.SetParameterDataVec2( deoglSkinShader::erutDepthSampleOffset, plan.GetDepthSampleOffset() );
		spb.SetParameterDataVec4( deoglSkinShader::erutFSTexCoordToScreenCoord,
			2.0f / defren.GetScalingU(), 2.0f / defren.GetScalingV(), -1.0f, -1.0f );
		defren.SetShaderParamFSQuad( spb, deoglSkinShader::erutFSScreenCoordToTexCoord );
		spb.SetParameterDataVec4( deoglSkinShader::erutFSFragCoordToTexCoord,
			defren.GetScalingU() / ( float )width, defren.GetScalingV() / ( float )height,
			0.5f / ( float )width, 0.5f / ( float )height );
		spb.SetParameterDataVec4( deoglSkinShader::erutFSFragCoordToScreenCoord,
			2.0f / ( float )width, 2.0f / ( float )height,
			1.0f / ( float )width - 1.0f, 1.0f / ( float )height - 1.0f );
		
		const float znear = plan.GetCameraImageDistance();
		const float zfar = plan.GetCameraViewDistance();
		const float fadeRange = ( zfar - znear ) * 0.001f; // for example 1m on 1km
		spb.SetParameterDataVec3( deoglSkinShader::erutFadeRange, zfar - fadeRange, zfar, 1.0f / fadeRange );
		
		// ssao
		spb.SetParameterDataVec4( deoglSkinShader::erutSSAOParams1,
			ssaoSelfOcclusion, ssaoEpsilon, ssaoScale, ssaoRandomAngleConstant );
		spb.SetParameterDataVec4( deoglSkinShader::erutSSAOParams2,
			ssaoTapCount, ssaoRadius, ssaoInfluenceRadius, ssaoRadiusLimit );
		spb.SetParameterDataVec3( deoglSkinShader::erutSSAOParams3,
			ssaoRadiusFactor, ssaoMipMapBase, ssaoMipMapMaxLevel );
		
		// sssss
		spb.SetParameterDataVec4( deoglSkinShader::erutSSSSSParams1, sssssDropSubSurfaceThreshold,
			sssssTapRadiusFactor, sssssTapRadiusLimit, sssssTapDropRadiusThreshold );
		spb.SetParameterDataIVec2( deoglSkinShader::erutSSSSSParams2, sssssTapCount, sssssTurnCount );
		
		// ssr
		spb.SetParameterDataVec4( deoglSkinShader::erutSSRParams1,
			ssrCoverageFactor.x, ssrCoverageFactor.y, ssrPowerEdge, ssrPowerRayLength );
		spb.SetParameterDataVec4( deoglSkinShader::erutSSRParams2, ssrClipReflDirNearDist,
			ssrRoughnessTapCountScale, ssrMinMaxTCFactor.x, ssrMinMaxTCFactor.y );
		spb.SetParameterDataIVec4( deoglSkinShader::erutSSRParams3,
			ssrStepCount, ssrSubStepCount, ssrMaxRayLength, ssrRoughnessTapMax );
		
		// lighting
		spb.SetParameterDataVec2( deoglSkinShader::erutAOSelfShadow, config.GetAOSelfShadowEnable() ? 0.1f : 1.0f,
			1.0f / ( DEG2RAD * config.GetAOSelfShadowSmoothAngle() ) );
		
		spb.SetParameterDataVec2( deoglSkinShader::erutLumFragCoordScale,
			( float )width / ( float )defren.GetTextureLuminance()->GetWidth(),
			( float )height / ( float )defren.GetTextureLuminance()->GetHeight() );
		
		// global illumination
		spb.SetParameterDataMat4x3( deoglSkinShader::erutGIRayMatrix, giMatrix );
		spb.SetParameterDataMat3x3( deoglSkinShader::erutGIRayMatrixNormal, giMatrixNormal );
		spb.SetParameterDataInt( deoglSkinShader::erutGIHighestCascade, giHighestCascade );
		
		// tone mapping
		spb.SetParameterDataVec2( deoglSkinShader::erutToneMapSceneKey, toneMapExposure, toneMapWhiteScale );
		spb.SetParameterDataVec3( deoglSkinShader::erutToneMapAdaption, toneMapLowInt, toneMapHighInt, toneMapAdaptationTime );
		spb.SetParameterDataVec3( deoglSkinShader::erutToneMapBloom, toneMapBloomStrength, toneMapBloomIntensity, toneMapBloomBlend );
		
		// debug depth transform
		spb.SetParameterDataVec2( deoglSkinShader::erutDebugDepthTransform, debugDepthScale, debugDepthShift );
		
		// specializations
		spb.SetParameterDataBVec4( deoglSkinShader::erutConditions1, condClipPlane, false, false, false );
		
		// stereo rendering
		if( plan.GetRenderStereo() ){
			const decDMatrix matrixCameraStereo( matrixCamera * cameraStereoMatrix );
			const decDMatrix &matrixProjectionStereo = plan.GetProjectionMatrixStereo();
			const decMatrix matrixSkyBodyStereo( matrixCameraStereo.GetRotationMatrix() * matrixProjectionStereo );
			
			spb.SetParameterDataArrayMat4x3( deoglSkinShader::erutMatrixV, 1, matrixCameraStereo );
			spb.SetParameterDataArrayMat4x4( deoglSkinShader::erutMatrixP, 1, matrixProjectionStereo );
			spb.SetParameterDataArrayMat4x4( deoglSkinShader::erutMatrixVP, 1, matrixCameraStereo * matrixProjectionStereo );
			spb.SetParameterDataArrayMat3x3( deoglSkinShader::erutMatrixVn, 1, matrixCameraStereo.GetRotationMatrix().Invert() );
			spb.SetParameterDataArrayMat4x4( deoglSkinShader::erutMatrixSkyBody, 1, matrixSkyBodyStereo );
			spb.SetParameterDataArrayVec4( deoglSkinShader::erutDepthToPosition, 1, plan.GetDepthToPositionStereo() );
			spb.SetParameterDataArrayVec2( deoglSkinShader::erutDepthToPosition2, 1, plan.GetDepthToPositionStereo2() );
			spb.SetParameterDataMat4x3( deoglSkinShader::erutCameraStereoMatrix, cameraStereoMatrix );
			
		}else{
			spb.SetParameterDataMat4x3( deoglSkinShader::erutCameraStereoMatrix, decMatrix() );
		}
	}
DBG_EXIT("PrepareRenderParamBlock")
}



void deoglRenderWorld::RenderMaskedPass( deoglRenderPlan &plan ){
DBG_ENTER("RenderMaskedPass")
	const int maskedCount = plan.GetMaskedPlanCount();
	if( maskedCount == 0 ){
DBG_EXIT("RenderMaskedPass(early)")
		return;
	}
	
	// NOTE rendering masked passes is called in the main thread before clearing. this works
	//      since each rendered mask pass activates depth FBO only and clears it. while calling
	//      RenderWorld the rest is then cleared
	
	// render each masked object
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "World.RenderMaskedPass" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderGeometry &rengeom = GetRenderThread().GetRenderers().GetGeometry();
	deoglPipelineState &state = renderThread.GetPipelineManager().GetState();
	bool clearColor = plan.GetClearColor();
	int m;
	
	for( m=0; m<maskedCount; m++ ){
		deoglRenderPlanMasked * const maskedPlan = plan.GetMaskedPlanAt( m );
		
		// clear depth texture
		deoglDebugTraceGroup debugTrace2( renderThread, "World.RenderMaskedPass.Mask" );
		
		pPipelineClearBuffers->Activate();
		defren.ActivateFBODepth();
		
		state.StencilMask( ~0 );
		OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0,
			renderThread.GetChoices().GetClearDepthValueRegular(), 0 ) );
		
		// render the mask
		state.StencilMask( ~0 );
		state.StencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
		state.StencilMask( 0x1 );
		state.StencilFunc( GL_ALWAYS, 0x1, 0x1 );
		
		// render solid content
		if( m > 0 ){ // already prepared before the first mask
			PrepareRenderParamBlock( plan, nullptr );
		}
		
		int pipelineModifier = 0;
		if( plan.GetFlipCulling() ){
			pipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
		}
		if( plan.GetRenderStereo() ){
			pipelineModifier |= deoglSkinTexturePipelines::emStereo;
		}
		
		pRenderTask->Clear();
		pRenderTask->SetRenderParamBlock( pRenderPB );
		pRenderTask->SetRenderVSStereo( plan.GetRenderStereo()
			&& renderThread.GetChoices().GetRenderStereoVSLayer() );
		
		pAddToRenderTask->Reset();
		pAddToRenderTask->SetSolid( true );
		pAddToRenderTask->SetSkinPipelineType( deoglSkinTexturePipelines::etMask );
		pAddToRenderTask->SetSkinPipelineModifier( pipelineModifier );
		
		pAddToRenderTask->AddComponentFaces( maskedPlan->GetComponent()->GetLODAt( 0 ),
			maskedPlan->GetComponentTexture(), 0 );
		
		pRenderTask->PrepareForRender();
		rengeom.RenderTask( *pRenderTask );
		
		// render vr hidden mesh clearing the mask
		debugTrace2.Close();
		
		renderThread.GetRenderers().GetVR().RenderHiddenArea( plan, true );
		
		// render the world using this mask
		deoglRenderPlan &maskedPlanPlan = *maskedPlan->GetPlan();
		maskedPlanPlan.SetClearColor( clearColor );
		RenderWorld( maskedPlanPlan, maskedPlan );
		//RenderFinalizeContext( maskedPlanPlan );  // this is wrong
		clearColor = maskedPlanPlan.GetClearColor();
	}
	
	plan.SetClearColor( clearColor );
	
	// restore the render parameter shader parameter block
	PrepareRenderParamBlock( plan, nullptr );
	
	// the occlusion maps are trashed now. the best solution would be to use a set of occlusion
	// maps for each masked plan avoiding the trashing of the main occlusion maps
	
	//if( deoglSkinShader::REFLECTION_TEST_MODE == deoglSkinShader::ertmSingleBlenderEnvMap ){
	//	ogl.GetRenderReflection().UpdateEnvMap( plan );
	//}
DBG_EXIT("RenderMaskedPass")
}



void deoglRenderWorld::RenderDebugDrawers( deoglRenderPlan &plan ){
DBG_ENTER("RenderDebugDrawers")
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "World.RenderDebugDrawers" );
	
	// attach diffuse texture as this is the output from the tone map pass
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	defren.ActivatePostProcessFBO( false );
	
	GetRenderThread().GetRenderers().GetDebugDrawer().RenderDebugDrawers( plan );
DBG_EXIT("RenderDebugDrawers")
}



void deoglRenderWorld::RenderEffects( deoglRenderPlan &plan ){
DBG_ENTER("RenderEffects")
	deoglRCamera * const camera = plan.GetCamera();
	if( ! camera ){
		return;
	}
	
	const int count = camera->GetEffectCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglREffect &effect = camera->GetEffectAt( i );
		
		if( effect.GetEnabled() ){
			effect.Render( plan );
		}
	}
DBG_EXIT("RenderEffects")
}

void deoglRenderWorld::RenderAntiAliasingPass(){
	// works but the results don't please me yet.
	/*
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager *tsMgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = *renderThread.GetDeferredRendering();
	
	// NOT REWORKED YET! DO NOT USE!!!
	
	// set attachments
	info->SwapRenderTextures();
	defren.AttachColor( info->GetPrimaryRenderTexture(), false, false ); // no depth, no stencil
	
	tsMgr->EnableTexture( 0, info->GetSecondaryRenderTexture(), GetSamplerClampNearest() );
	tsMgr->EnableDepth( 1, defren.GetDepthTexture() );
	tsMgr->EnableTexture( 2, defren.GetNormalTexture(), GetSamplerClampNearest() );
	
	// set states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	// set program
	ogl.pSetProgram( espDrAntiAliasing );
	ogl.pSetProgramParam( espDrAntiAliasing_Options, defren.GetPixelSizeU(),
		defren.GetPixelSizeV(), 0.1f, 0.0f );
	
	// render quad
	defren.RenderFullScreenQuad();
	*/
}

void deoglRenderWorld::RenderFinalizeFBO( deoglRenderPlan &plan,
bool withColorCorrection, bool withGammaCorrection ){
DBG_ENTER("RenderFinalizeFBO")
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "World.RenderFinalizeFBO" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	
	const bool upscale = plan.GetUseUpscaling();
	const int upscaleWidth = plan.GetUpscaleWidth();
	const int upscaleHeight = plan.GetUpscaleHeight();
	
	const deoglPipeline &pipeline = plan.GetRenderVR() == deoglRenderPlan::ervrStereo
		? *pPipelineFinalizeSplit : ( plan.GetRenderStereo() ? *pPipelineFinalizeStereo : *pPipelineFinalize );
	pipeline.Activate();
	
	renderThread.GetFramebuffer().Activate( plan.GetFBOTarget() );
	
	const int viewportHeight = upscale ? upscaleHeight : plan.GetViewportHeight();
	const int viewportWidth = upscale ? upscaleWidth : plan.GetViewportWidth();
	
	renderThread.GetTexture().GetStages().DisableAllStages();
	OGL_CHECK( renderThread, glViewport( 0, 0, viewportWidth, viewportHeight ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, viewportWidth, viewportHeight ) );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetPostProcessTexture(),
		plan.GetUseUpscaling() ? GetSamplerClampLinear() : GetSamplerClampNearest() );
	
	pRenderPB->Activate();
	
	deoglShaderCompiled * const shader = &pipeline.GetShader();
	
	if( withGammaCorrection ){
		const float gamma = 1.0f / ( OGL_RENDER_GAMMA * config.GetGammaCorrection() );
		shader->SetParameterFloat( spfinGamma, gamma, gamma, gamma, 1.0f );
		
	}else{
		shader->SetParameterFloat( spfinGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	}
	
	if( withColorCorrection ){
		shader->SetParameterFloat( spfinBrightness, config.GetBrightness(),
			config.GetBrightness(), config.GetBrightness(), 0.0f );
		shader->SetParameterFloat( spfinContrast, config.GetContrast(),
			config.GetContrast(), config.GetContrast(), 1.0f );
		
	}else{
		shader->SetParameterFloat( spfinBrightness, 0.0f, 0.0f, 0.0f, 0.0f );
		shader->SetParameterFloat( spfinContrast, 1.0f, 1.0f, 1.0f, 1.0f );
	}
	
	if( plan.GetUpsideDown() ){
		if( upscale ){
			defren.SetShaderParamFSQuad( *shader, spfinTCTransform,
				plan.GetViewportWidth(), plan.GetViewportHeight() );
			
		}else{
			defren.SetShaderParamFSQuad( *shader, spfinTCTransform );
		}
		
	}else{
		if( upscale ){
			defren.SetShaderParamFSQuadUpsideDown( *shader, spfinTCTransform,
				plan.GetViewportWidth(), plan.GetViewportHeight() );
			
		}else{
			defren.SetShaderParamFSQuadUpsideDown( *shader, spfinTCTransform );
		}
	}
	
	if( plan.GetRenderVR() == deoglRenderPlan::ervrStereo ){
		RenderFullScreenQuadVAO();
		
	}else{
		RenderFullScreenQuadVAO( plan );
	}
	
	
	
	// revert to 2d mode
	tsmgr.DisableAllStages(); // deprecated
	
	// dev mode debug image check
	renderThread.GetDebug().GetDeveloperMode().CheckDebugImageUse();
DBG_EXIT("RenderFinalizeFBO")
}

void deoglRenderWorld::RenderFinalizeContext( deoglRenderPlan &plan ){
DBG_ENTER("RenderFinalizeContext")
//	deoglRenderableColorTexture *maskRenderTexture = info->GetMaskRenderTexture();
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "World.RenderFinalizeContext" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglConfiguration &config = renderThread.GetConfiguration();
	float gamma = 1.0f / ( OGL_RENDER_GAMMA * config.GetGammaCorrection() );
	float contrast = config.GetContrast();
	float brightness = config.GetBrightness();
	float pbn = brightness + ( 1.0f - contrast ) * 0.5f;
	deoglShaderCompiled *shader;
	
	const int viewportHeight = plan.GetViewportHeight();
	const int viewportWidth = plan.GetViewportWidth();
	
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineFinalizeBlendStereo : *pPipelineFinalizeBlend;
	pipeline.Activate();
	
	renderThread.GetTexture().GetStages().DisableAllStages();
	OGL_CHECK( renderThread, glViewport( 0, 0, viewportWidth, viewportHeight ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, viewportWidth, viewportHeight ) );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetPostProcessTexture(), plan.GetUseUpscaling()
		? GetSamplerClampLinear() : GetSamplerClampNearest() );
	
	pRenderPB->Activate();
	
	shader = &pipeline.GetShader();
	shader->SetParameterFloat( spfinGamma, gamma, gamma, gamma, 1.0f );
	shader->SetParameterFloat( spfinBrightness, pbn, pbn, pbn, 0.0f );
	shader->SetParameterFloat( spfinContrast, contrast, contrast, contrast, 1.0f );
	
	if( plan.GetUpsideDown() ){
		if( plan.GetUseUpscaling() ){
			defren.SetShaderParamFSQuad( *shader, spfinTCTransform,
				plan.GetUpscaleWidth(), plan.GetUpscaleHeight() );
			
		}else{
			defren.SetShaderParamFSQuad( *shader, spfinTCTransform );
		}
		
	}else{
		if( plan.GetUseUpscaling() ){
			defren.SetShaderParamFSQuadUpsideDown( *shader, spfinTCTransform,
				plan.GetUpscaleWidth(), plan.GetUpscaleHeight() );
			
		}else{
			defren.SetShaderParamFSQuadUpsideDown( *shader, spfinTCTransform );
		}
	}
	
	RenderFullScreenQuadVAO( plan );
	
	
	
	// revert to 2d mode
	tsmgr.DisableAllStages(); // deprecated
	
	// dev mode debug image check
	renderThread.GetDebug().GetDeveloperMode().CheckDebugImageUse();
DBG_EXIT("RenderFinalizeContext")
}



void deoglRenderWorld::AddTopLevelDebugInfo(){
	pDebugInfo.AddTopLevelDebugInfo();
}

void deoglRenderWorld::DevModeDebugInfoChanged(){
	pDebugInfo.DevModeDebugInfoChanged();
}



// Private Functions
//////////////////////

void deoglRenderWorld::pCleanUp(){
	if( pAddToRenderTaskParticles ){
		delete pAddToRenderTaskParticles;
	}
	if( pRenderTaskParticles ){
		delete pRenderTaskParticles;
	}
	if( pParticleSorter ){
		delete pParticleSorter;
	}
	if( pAddToRenderTask ){
		delete pAddToRenderTask;
	}
	if( pRenderTask ){
		delete pRenderTask;
	}
}
