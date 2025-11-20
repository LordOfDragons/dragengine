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

#include "deoglRenderDepthPass.h"
#include "deoglRenderGeometry.h"
#include "deoglRenderOcclusion.h"
#include "deoglRenderVR.h"
#include "deoglRenderWorld.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlanDebug.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanMasked.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglRenderTask.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideListLight.h"
#include "../collidelist/deoglCollideListManager.h"
#include "../component/deoglRComponent.h"
#include "../debug/deoglDebugInformation.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../debug/deoglDebugSnapshot.h"
#include "../debug/debugSnapshot.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../light/deoglRLight.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../occquery/deoglOcclusionQuery.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shapes/deoglShape.h"
#include "../shapes/deoglShapeManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../vao/deoglVAO.h"

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

enum eSPDepthDownsample{
	spddsTCClamp,
	spddsMipMapLevel
};



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



// Class deoglRenderDepthPass
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderDepthPass::deoglRenderDepthPass( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread )
{
	const bool renderFSQuadStereoVSLayer = renderThread.GetChoices().GetRenderFSQuadStereoVSLayer();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const bool useInverseDepth = renderThread.GetChoices().GetUseInverseDepth();
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	const deoglShaderSources *sources;
	
	
	renderThread.GetShader().SetCommonDefines( commonDefines );
	if(useInverseDepth){
		defines.SetDefines("INVERSE_DEPTH");
	}
	
	
	// depth downsample
	pipconf.Reset();
	pipconf.SetMasks( false, false, false, false, true );
	pipconf.EnableDepthTestAlways();
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed( "DefRen Depth Downsample" );
	
	defines.SetDefines( "NO_TEXCOORD" );
	defines.SetDefines( "USE_MIN_FUNCTION" ); // so it works for SSR. should also work for SSAO
	pAsyncGetPipeline(pPipelineDepthDownsample, pipconf, sources, defines);
	
	
	// depth downsample stereo
	defines = commonDefines;
	defines.SetDefines( "NO_TEXCOORD" );
	defines.SetDefines( "USE_MIN_FUNCTION" ); // so it works for SSR. should also work for SSAO
	
	defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
	if(renderFSQuadStereoVSLayer){
		defines.SetDefines("VS_RENDER_LAYER");
	}
	if( ! renderFSQuadStereoVSLayer ){
		sources = shaderManager.GetSourcesNamed( "DefRen Depth Downsample Stereo" );
	}
	
	pAsyncGetPipeline(pPipelineDepthDownsampleStereo, pipconf, sources, defines);
}

deoglRenderDepthPass::~deoglRenderDepthPass(){
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



void deoglRenderDepthPass::RenderSolidDepthPass( deoglRenderPlan &plan,
const deoglRenderPlanMasked *mask, bool xray ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "DepthPass.RenderSolidDepthPass" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	deoglPipelineState &state = renderThread.GetPipelineManager().GetState();
	
	DebugTimer1Reset( plan, true );
	
	// clear depth texture
	pPipelineClearBuffers->Activate();
	defren.ActivateFBODepth();
	
	const GLfloat clearDepth = renderThread.GetChoices().GetClearDepthValueRegular();
	OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		// NOTE: Haiku MESA 17.1.10 fails to properly clear. No idea why
	
	// render depth geometry
	state.StencilMask( 0 );
	state.StencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
	
	if( mask ){
		state.StencilFunc( GL_EQUAL, 0x1, 0x1 );
		
	}else{
		state.StencilFunc( GL_ALWAYS, 0x1, 0x0 );
	}
	
	RenderDepth( plan, mask, true, false, false, xray ); // +solid, -maskedOnly, -reverseDepthTest
	
	// now is a good time to do occlusion queries for all lights having passed
	// the quick rejection test as well as all components which are costly
	// enough to justify wasting an occlusion query on them.
	if( ! xray ){
		RenderOcclusionQueryPass( plan, mask );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryOcclusion, true );
	}
	
	// if we are rendering a solid pass and we have transparency render the
	// transparency counter pass to determine how many layers we need
	if( plan.GetHasTransparency() ){
		renderThread.GetRenderers().GetTransparencyCounter().CountTransparency( plan, mask );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryTranspCounter, true );
	}
}



void deoglRenderDepthPass::RenderDepth( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask,
bool solid, bool maskedOnly, bool reverseDepthTest, bool xray ){
DBG_ENTER_PARAM3("RenderDepthPass", "%p", mask, "%d", solid, "%d", maskedOnly)
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "DepthPass.RenderDepth" );
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglCollideList &collideList = plan.GetCollideList();
	deoglRenderPlanDebug * const planDebug = plan.GetDebug();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	
	// debug
	if( planDebug ){
		if( solid ){
			planDebug->IncrementRenderPasses();
			
		}else{
			planDebug->IncrementTransparentPasses();
		}
	}
	
	// depth pass
	if( solid && ! mask ){
		renderThread.GetRenderers().GetVR().RenderHiddenArea( plan, false );
	}
	
	deoglSkinTexturePipelines::eTypes pipelineType = deoglSkinTexturePipelines::etDepth;
	if( mask && mask->GetUseClipPlane() ){
		if( reverseDepthTest ){
			pipelineType = deoglSkinTexturePipelines::etDepthClipPlaneReversed;
			
		}else{
			pipelineType = deoglSkinTexturePipelines::etDepthClipPlane;
		}
		
	}else{
		if( reverseDepthTest ){
			pipelineType = deoglSkinTexturePipelines::etDepthReversed;
			
		}else{
			pipelineType = deoglSkinTexturePipelines::etDepth;
		}
	}
	
	int pipelineModifier = 0;
	if( plan.GetFlipCulling() ){
		pipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
	}
	if( plan.GetRenderStereo() ){
		pipelineModifier |= deoglSkinTexturePipelines::emStereo;
	}
	
	deoglRenderTask *renderTask = nullptr;
	deoglComputeRenderTask *computeRenderTask = nullptr;
	
	if( solid ){
		deoglRenderPlanTasks &tasks = plan.GetTasks();
		tasks.WaitFinishBuildingTasksDepth();
		
		if( renderThread.GetChoices().GetUseComputeRenderTask() ){
			// TEMP HACK
			if( collideList.GetHTSClusterCount() > 0 ){
				deoglRenderTask &rt = *renworld.GetRenderTask();
				deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
				rt.Clear();
				rt.SetRenderParamBlock( renworld.GetRenderPB() );
				rt.SetRenderVSStereo( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() );
				addToRenderTask.Reset();
				addToRenderTask.SetSolid( true );
				addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
				addToRenderTask.SetNoRendered( mask );
				if( xray ){
					addToRenderTask.SetFilterXRay( true );
					addToRenderTask.SetXRay( true );
				}
				addToRenderTask.SetSkinPipelineType( pipelineType );
				addToRenderTask.SetSkinPipelineModifier( pipelineModifier );
				addToRenderTask.AddHeightTerrainSectorClusters( collideList, true );
				rt.PrepareForRender();
				rengeom.RenderTask( rt );
			}
			// TEMP HACK
			
			computeRenderTask = xray ? tasks.GetCRTSolidDepthXRay() : tasks.GetCRTSolidDepth();
			computeRenderTask->SetRenderParamBlock( renworld.GetRenderPB() );
			
		}else{
			renderTask = xray ? &tasks.GetSolidDepthXRayTask() : &tasks.GetSolidDepthTask();
			renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		}
		
	}else{
		DebugTimer1Reset( plan, true );
		renderTask = renworld.GetRenderTask();
		deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
		
		// build render task
		renderTask->Clear();
		renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		renderTask->SetRenderVSStereo( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() );
		
		addToRenderTask.Reset();
		addToRenderTask.SetSolid( solid );
		addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
		addToRenderTask.SetNoRendered( mask );
		if( xray ){
			addToRenderTask.SetFilterXRay( true );
			addToRenderTask.SetXRay( true );
		}
		addToRenderTask.SetSkinPipelineType( pipelineType );
		addToRenderTask.SetSkinPipelineModifier( pipelineModifier );
		
		// components
		addToRenderTask.SetFilterDecal(true);
		addToRenderTask.SetDecal(false);
		addToRenderTask.AddComponents(collideList);
		
		addToRenderTask.SetDecal(true);
		addToRenderTask.AddComponents(collideList);
		addToRenderTask.SetFilterDecal(false);
		
		// decals
		addToRenderTask.AddDecals(collideList);
		
		// billboards
		addToRenderTask.AddBillboards( collideList );
		
		// prop fields
		addToRenderTask.AddPropFields( collideList, false );
		addToRenderTask.AddPropFields( collideList, true );
		addToRenderTask.AddPropFieldClusters( collideList, false );
		addToRenderTask.AddPropFieldClusters( collideList, true );
		
		// height terrains
		addToRenderTask.AddHeightTerrains( collideList, true );
		addToRenderTask.AddHeightTerrainSectorClusters( collideList, true );
		
		// particles
		if( renderThread.GetChoices().GetRealTransparentParticles() ){
			addToRenderTask.AddParticles( collideList );
		}
		
		// outline
		addToRenderTask.SetOutline( true );
		addToRenderTask.SetFilterDecal( true );
		addToRenderTask.SetDecal( false );
		
		addToRenderTask.AddComponents( collideList );
		
		// prepare render task
		renderTask->PrepareForRender();
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthTask, true );
	}
	
	if( ( renderTask && renderTask->GetPipelineCount() > 0 )
	|| ( computeRenderTask && computeRenderTask->GetStepCount() > 0 ) ){
		if( planDebug && plan.GetRenderPassNumber() == 1 ){
			const int componentCount = collideList.GetComponentCount();
			deoglEnvironmentMapList envMapList;
			int i;
			
			for( i=0; i<componentCount; i++ ){
				const deoglCollideListComponent &clistComponent = *collideList.GetComponentAt( i );
				const deoglRComponent &component = *clistComponent.GetComponent();
				const deoglModelLOD &modelLOD = component.GetModel()->GetLODAt( clistComponent.GetLODLevel() );
				
				planDebug->IncrementRenderedObjects();
				planDebug->IncrementRenderedTriangles( modelLOD.GetFaceCount() );
				planDebug->IncrementLODTriangles( component.GetModel()->GetLODAt( 0 ).GetFaceCount() );
				/*
				if( component.GetRenderEnvMap() ){
					envMapList.AddIfMissing( component.GetRenderEnvMap() );
				}
				if( component.GetRenderEnvMapFade() ){
					envMapList.AddIfMissing( component.GetRenderEnvMapFade() );
				}
				*/
			}
			
			planDebug->IncrementRenderedEnvMaps( envMapList.GetCount() );
		}
		
		if( computeRenderTask ){
			computeRenderTask->Render();
			
		}else{
			rengeom.RenderTask( *renderTask );
		}
		
		if( solid ){
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryDepthRender, true );
			
		}else{
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthRender, true );
		}
	}
	
	
	// outline
	if( ! renderThread.GetChoices().GetUseComputeRenderTask() && solid ){
		const deoglDebugTraceGroup debugTraceOutline( renderThread, "DepthPass.RenderDepth.Outline" );
		
		deoglRenderPlanTasks &tasks = plan.GetTasks();
		renderTask = xray ? &tasks.GetSolidDepthOutlineXRayTask() : &tasks.GetSolidDepthOutlineTask();
		renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		
		if( renderTask->GetPipelineCount() > 0 ){
			rengeom.RenderTask( *renderTask );
			
			if( solid ){
				DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryDepthRender, true );
				
			}else{
				DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthRender, true );
			}
		}
	}
	
	
	
	// TODO
	// optimize render passes later on. each pass has to reconstruct the depth from the depth texture.
	// this boils down in general to these instructions:
	// 
	//    vec3 position = vec3( texelFetch( texDepth, ivec2( gl_FragCoord.xy ), 0 ).r );
	//    position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	//    #ifdef FULLSCREENQUAD
	//       position.xy = vScreenCoord * pPosTransform.zw * position.zz;
	//    #else
	//       position.xy = vLightVolumePos.xy * position.zz / vLightVolumePos.zz;
	//    #endif
	// 
	// rendering directly to an RGB16 color texture is slow since we loose the double speed writing
	// possible with depth textures. using an additional render pass though this problem can be
	// worked around. the depth is rendered as above into a conventional depth texture. afterwards
	// an RGB16 texture is rendered with the depth texture as input. the used shader does nothing
	// else than reconstructing the view position from the input depth texture as done by the other
	// passes writing it out to the RGB16 texture. there is no double write in this scenario but
	// due to the short shader and the sequential access due to the fullscreen quad the cost is
	// not any worse than a fullscreen post processing effect pass. as a result though all following
	// passes do not require to reconstruct the position over and over again saving shader processing
	// time. only downside is the memory consumption. for the old way two depth textures are required
	// each consuming 4 bytes per pixel for a total of 8 bytes per pixel. for the new way one depth
	// texture is required (for the stencil mask) consuming 4 bytes per pixel and two color position
	// textures of RGB16 format each consuming 6 bytes per pixel for a total of 16 bytes per pixel.
	// thus the new version consumes double the GPU memory compared to the old one but speeds up the
	// rendering. furthermore it allows to better use stencil masks to improve later render passes
	// without requiring costly re-rendering or copying of the stencil masks between depth textures.
	// 
	// NOTE
	// the light shaders use a different calculation for the light-volume case. it has to be checked
	// if this works correctly with these shaders or if the position there is a different one
DBG_EXIT("RenderDepthPass")
}



void deoglRenderDepthPass::DownsampleDepth( deoglRenderPlan &plan ){
DBG_ENTER("DownsampleDepth")
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "DepthPass.DownsampleDepth" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglArrayTexture &texture = *defren.GetDepthTexture1();
	int height, width, i;
	
	const int mipMapLevelCount = texture.GetRealMipMapLevelCount();
	const GLfloat clearDepth = renderThread.GetChoices().GetClearDepthValueRegular();
	
	height = defren.GetHeight();
	width = defren.GetWidth();
	
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineDepthDownsampleStereo : *pPipelineDepthDownsample;
	pipeline.Activate();
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	deoglShaderCompiled &shader = pipeline.GetShader();
	
	tsmgr.EnableArrayTexture( 0, texture, GetSamplerClampNearest() );
	
	for( i=1; i<=mipMapLevelCount; i++ ){
		defren.ActivateFBODepthLevel( i );
		
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		
		shader.SetParameterInt( spddsTCClamp, width - 1, height - 1 );
		shader.SetParameterInt( spddsMipMapLevel, i - 1 );
		
		width = decMath::max( width >> 1, 1 );
		height = decMath::max( height >> 1, 1 );
		
		SetViewport( width, height );
		
		RenderFullScreenQuad( plan );
	}
DBG_EXIT("DownsampleDepth")
}



void deoglRenderDepthPass::RenderOcclusionQueryPass( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask ){
DBG_ENTER_PARAM("RenderOcclusionQueryPass", "%p", mask)
	deoglCollideList &collideList = plan.GetCollideList();
	
	// NOTE: the currently does not fully work and breaks for lights the camera
	// is inside the light box. the same check should be used as for the 180
	// point light case and an occlusion test only done if the camera is outside
	// the light box. for the time being this code is disabled.
	// 
	// NOTE: another note concerning the occlusion query time. currently the
	// light class checks if a result exists and only returns hidden if the
	// query count is zero. this way no time is wasted waiting for a result
	// to arrive. rendering seems though to be faster than the occlusion test
	// can return. not much one can do here except reintroducing the assigning
	// of lights to rooms to cull them this way. solution pending.
	// 
	// NOTE: GI required reordering rendering to support the expensive operation.
	// Now the first light rendering happens rather soon after the depth pass.
	// this causes the first light to request for the occlusion query result nearly
	// directly after the query has been started. this causes delays up to 10ms
	// which is not acceptable. since GI requires light shadows to be rendered
	// even if not visible or outside the view the entire occlusion query is not
	// helping much at all. so dropping it for the time being and improving shadow
	// rendering performance instead
	
	collideList.MarkLightsCulled( false );
	
	DBG_EXIT("RenderOcclusionQueryPass(disabled)")
}
