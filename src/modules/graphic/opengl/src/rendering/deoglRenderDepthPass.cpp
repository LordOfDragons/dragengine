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
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	const deoglShaderSources *sources;
	
	
	renderThread.GetShader().SetCommonDefines( commonDefines );
	
	
	// depth downsample
	pipconf.Reset();
	pipconf.SetMasks( false, false, false, false, true );
	pipconf.EnableDepthTestAlways();
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed( "DefRen Depth Downsample" );
	if( useInverseDepth ){
		defines.SetDefines( "INVERSE_DEPTH" );
	}
	
	defines.SetDefines( "NO_TEXCOORD" );
	defines.SetDefines( "USE_MIN_FUNCTION" ); // so it works for SSR. should also work for SSAO
	pipconf.SetShader( renderThread, sources, defines );
	pPipelineDepthDownsample = pipelineManager.GetWith( pipconf );
	
	
	// depth downsample stereo
	defines = commonDefines;
	if( useInverseDepth ){
		defines.SetDefines( "INVERSE_DEPTH" );
	}
	defines.SetDefines( "NO_TEXCOORD" );
	defines.SetDefines( "USE_MIN_FUNCTION" ); // so it works for SSR. should also work for SSAO
	
	defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
	if( ! renderFSQuadStereoVSLayer ){
		sources = shaderManager.GetSourcesNamed( "DefRen Depth Downsample Stereo" );
	}
	
	pipconf.SetShader( renderThread, sources, defines );
	pPipelineDepthDownsampleStereo = pipelineManager.GetWith( pipconf );
}

deoglRenderDepthPass::~deoglRenderDepthPass(){
}



// Rendering
//////////////

#define DO_QUICK_DEBUG 1

#ifdef DO_QUICK_DEBUG
#define QUICK_DEBUG_START( lower, upper ) \
	if( renderThread.GetConfiguration().GetQuickDebug() > upper \
	|| renderThread.GetConfiguration().GetQuickDebug() < lower ){
#define QUICK_DEBUG_END }
#else
#define QUICK_DEBUG_START( lower, upper )
#define QUICK_DEBUG_END
#endif



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
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapDepthPassBuffers ){
		deoglDebugSnapshot snapshot( renderThread );
		snapshot.SetEnableDepth( true );
		snapshot.SetName( "solid/depthpass-depth" );
		snapshot.TakeSnapshot();
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	
	QUICK_DEBUG_START( 14, 19 )
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
	
	QUICK_DEBUG_END
}



void deoglRenderDepthPass::RenderDepth( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask,
bool solid, bool maskedOnly, bool reverseDepthTest, bool xray ){
DBG_ENTER_PARAM3("RenderDepthPass", "%p", mask, "%d", solid, "%d", maskedOnly)
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "DepthPass.RenderDepth" );
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglConfiguration &config = renderThread.GetConfiguration();
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
	
	deoglRenderTask *renderTask;
	
	if( solid ){
		deoglRenderPlanTasks &tasks = plan.GetTasks();
		tasks.WaitFinishBuildingTasksDepth();
		
		renderTask = xray ? &tasks.GetSolidDepthXRayTask() : &tasks.GetSolidDepthTask();
		renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		
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
		addToRenderTask.SetFilterDecal( true );
		addToRenderTask.SetDecal( false );
		addToRenderTask.AddComponents( collideList );
		addToRenderTask.SetFilterDecal( false );
		
		// billboards
		addToRenderTask.AddBillboards( collideList );
		
		// prop fields
		addToRenderTask.AddPropFields( collideList, false );
		addToRenderTask.AddPropFields( collideList, true );
		
		// height terrains
		addToRenderTask.AddHeightTerrains( collideList, true );
		
		// particles
		if( renderThread.GetChoices().GetRealTransparentParticles() ){
			addToRenderTask.AddParticles( collideList );
		}
		
		renderTask->PrepareForRender();
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthTask, true );
	}
	
	if( renderTask->GetPipelineCount() > 0 ){
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
		
		if( config.GetDebugSnapshot() == edbgsnapDepthPassRenTask ){
			renderThread.GetLogger().LogInfo( "RenderWorld.pRenderDepthPass: render task" );
			renderTask->DebugPrint( renderThread.GetLogger() );
		}
		
		rengeom.RenderTask( *renderTask );
		
		if( solid ){
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryDepthRender, true );
			
		}else{
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthRender, true );
		}
	}
	
	
	// outline
	const deoglDebugTraceGroup debugTraceOutline( renderThread, "DepthPass.RenderDepth.Outline" );
	if( solid ){
		deoglRenderPlanTasks &tasks = plan.GetTasks();
		renderTask = xray ? &tasks.GetSolidDepthOutlineXRayTask() : &tasks.GetSolidDepthOutlineTask();
		renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		
	}else{
		DebugTimer1Reset( plan, true );
		renderTask = renworld.GetRenderTask();
		deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
		
		renderTask->Clear();
		renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		renderTask->SetRenderVSStereo( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() );
		
		addToRenderTask.Reset();
		addToRenderTask.SetOutline( true );
		addToRenderTask.SetFilterDecal( true );
		addToRenderTask.SetDecal( false );
		addToRenderTask.SetSolid( solid );
		addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
		addToRenderTask.SetNoRendered( mask );
		if( xray ){
			addToRenderTask.SetFilterXRay( true );
			addToRenderTask.SetXRay( xray );
		}
		addToRenderTask.SetSkinPipelineType( pipelineType );
		addToRenderTask.SetSkinPipelineModifier( pipelineModifier );
		
		addToRenderTask.AddComponents( collideList );
		
		renderTask->PrepareForRender();
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthTask, true );
	}
	
	if( renderTask->GetPipelineCount() > 0 ){
		rengeom.RenderTask( *renderTask );
		
		if( solid ){
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryDepthRender, true );
			
		}else{
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthRender, true );
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
	
	deoglShaderCompiled &shader = pipeline.GetGlShader();
	
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
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 61 ){
		decString text;
		
		for( i=0; i<=mipMapLevelCount; i++ ){
			text.Format( "downsample_depth-pass%i", i );
			renderThread.GetDebug().GetDebugSaveTexture().SaveDepthArrayTextureLevel(
				*defren.GetDepthTexture1(), i, text.GetString(), deoglDebugSaveTexture::edtDepth );
		}
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
