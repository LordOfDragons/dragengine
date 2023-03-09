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
#include "deoglRenderGeometryPass.h"
#include "deoglRenderSky.h"
#include "deoglRenderWorld.h"
#include "defren/deoglDeferredRendering.h"
#include "light/deoglRenderLight.h"
#include "light/deoglRenderGI.h"
#include "plan/deoglRenderPlan.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglRenderTask.h"
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideList.h"
#include "../debug/debugSnapshot.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTShader.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

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



// Class deoglRenderGeometryPass
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderGeometryPass::deoglRenderGeometryPass( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread ){
}

deoglRenderGeometryPass::~deoglRenderGeometryPass(){
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



void deoglRenderGeometryPass::RenderDecals( deoglRenderPlan &plan, bool xray ){
DBG_ENTER("RenderDecals")
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GeometryPass.RenderDecals" );
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	
	// render using render task
	deoglRenderPlanTasks &tasks = plan.GetTasks();
	tasks.WaitFinishBuildingTasksGeometry();
	
	deoglRenderTask &renderTask = xray ? tasks.GetSolidDecalsXRayTask() : tasks.GetSolidDecalsTask();
	renderTask.SetRenderParamBlock( renworld.GetRenderPB() );
	rengeom.RenderTask( renderTask );
DBG_EXIT("RenderDecals")
}

void deoglRenderGeometryPass::RenderSolidGeometryPass( deoglRenderPlan &plan,
const deoglRenderPlanMasked *mask, bool xray ){
DBG_ENTER_PARAM("RenderSolidGeometryPass", "%p", mask)
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "GeometryPass.RenderSolidGeometryPass" );
	const bool useComputeRenderTask = renderThread.GetChoices().GetUseComputeRenderTask();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglRenderDepthPass &rendepth = renderThread.GetRenderers().GetDepthPass();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	deoglConfiguration &config = renderThread.GetConfiguration();
	deoglPipelineState &state = renderThread.GetPipelineManager().GetState();
	
	// render pre-pass depth. this includes rendering depth, occlusion testing and transparency counting
	rendepth.RenderSolidDepthPass( plan, mask, xray );
	
	QUICK_DEBUG_START( 14, 19 )
	
	DebugTimer1Reset( plan, true );
	
	// geometry pass. render first sky which uses FBO Depth+Color.
	// sky always clears color buffer.
	// 
	// TODO change the sky rendering so it can use FBO Material (render to color only). this
	//      would avoid the need to switch FBO attachment. maybe glDrawBuffers switching
	//      temporarily can help? needs first some research if glDrawBuffers has similar
	//      caveats performance wise as switching FBOs has
	SetViewport( plan );
// 	defren.ActivateFBOMaterialColor();
	if( ! xray ){
		QUICK_DEBUG_START( 16, 19 )
		renderThread.GetRenderers().GetSky().RenderSky( plan, mask );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometrySky, true );
		QUICK_DEBUG_END
	}
	
	
	// activate material fbo and clear all color attachments except color and depth buffer
	pPipelineClearBuffers->Activate();
	defren.ActivateFBOMaterialColor();
	
	const GLfloat clearDiffuse[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat clearNormal[ 4 ] = { 0.5f, 0.5f, 1.0f, 0.0f };
	const GLfloat clearReflectivity[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	if( renderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
		const GLfloat clearRoughness[ 4 ] = { 1.0f, 0.0f, 0.0f, 0.0f };
		const GLfloat clearAOSolidity[ 4 ] = { 1.0f, 1.0f, 0.0f, 0.0f };
		const GLfloat clearSubSurface[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearDiffuse[ 0 ] ) );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 1, &clearNormal[ 0 ] ) );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 2, &clearReflectivity[ 0 ] ) );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 3, &clearRoughness[ 0 ] ) );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 4, &clearAOSolidity[ 0 ] ) );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 5, &clearSubSurface[ 0 ] ) );
		
	}else{
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearDiffuse[ 0 ] ) );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 1, &clearNormal[ 0 ] ) );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 2, &clearReflectivity[ 0 ] ) );
	}
	
	
	// render geometry
	state.StencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
	state.StencilMask( plan.GetStencilWriteMask() );
	
	if( mask ){
		state.StencilFunc( GL_EQUAL, plan.GetStencilRefValue(), 0x1 );
		
	}else{
		state.StencilFunc( GL_ALWAYS, plan.GetStencilRefValue(), 0x0 );
	}
	
	
	deoglRenderPlanTasks &tasks = plan.GetTasks();
	tasks.WaitFinishBuildingTasksGeometry();
	
	deoglRenderTask *renderTask = nullptr;
	deoglComputeRenderTask *computeRenderTask = nullptr;
	
	// height terrain has to come first since it has to be handled differently
	deoglDebugTraceGroup debugTraceHT( renderThread, "GeometryPass.RenderSolidGeometryPass.HeightTerrain" );
	renderTask = xray ? &tasks.GetSolidGeometryHeight1XRayTask() : &tasks.GetSolidGeometryHeight1Task();
	if( renderTask->GetPipelineCount() > 0 ){
		renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		rengeom.RenderTask( *renderTask );
	}
	
	renderTask = xray ? &tasks.GetSolidGeometryHeight2XRayTask() : &tasks.GetSolidGeometryHeight2Task();
	if( renderTask->GetPipelineCount() > 0 ){
		renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		rengeom.RenderTask( *renderTask );
	}
	
	// other content
	deoglDebugTraceGroup debugTraceOther( debugTraceHT, "GeometryPass.RenderSolidGeometryPass.Geometry" );
	if( useComputeRenderTask ){
		computeRenderTask = xray ? tasks.GetCRTSolidGeometryXRay() : tasks.GetCRTSolidGeometry();
																												computeRenderTask->DebugSimple(renderThread.GetLogger(), false);
		if( computeRenderTask->GetStepCount() > 0 ){
			computeRenderTask->SetRenderParamBlock( renworld.GetRenderPB() );
			computeRenderTask->Render();
		}
		
	}else{
		renderTask = xray ? &tasks.GetSolidGeometryXRayTask() : &tasks.GetSolidGeometryTask();
		if( renderTask->GetPipelineCount() > 0 ){
			renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
			rengeom.RenderTask( *renderTask );
		}
	}
	
	// outline
	if( ! useComputeRenderTask ){
		deoglDebugTraceGroup debugTraceOutline( debugTraceOther, "GeometryPass.RenderSolidGeometryPass.Outline" );
		renderTask = xray ? &tasks.GetSolidGeometryOutlineXRayTask() : &tasks.GetSolidGeometryOutlineTask();
		if( renderTask->GetPipelineCount() > 0 ){
			renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
			rengeom.RenderTask( *renderTask );
		}
		
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryRender, true );
	}
	debugTraceOther.Close();
	
	// decals
	if( ! useComputeRenderTask ){
		RenderDecals( plan, xray );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryDecals, true );
	}
	QUICK_DEBUG_END
	
	
	// downsample depth into mip-map levels if some later feature requires this. this can be done only after the
	// geometry pass since this writes to the stencil mask of the depth texture. writing to the stencil part
	// trashes mip map levels although no depth writing is enabled
	if( config.GetSSAOEnable() && ! plan.GetNoReflections() ){
		rendepth.DownsampleDepth( plan );
		SetViewport( plan );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryDownsampleDepth, true );
	}
	
	// screen space ambient occlusion
	if( ! plan.GetNoReflections() ){
		renderThread.GetRenderers().GetLight().RenderAO( plan, true );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometrySSAO, true );
	}
DBG_EXIT("RenderSolidGeometryPass")
}

#if 0
void deoglRenderGeometryPass::RenderLuminanceOnly( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	const deoglCollideList &collideList = plan.GetCollideList();
	deoglRenderTask &renderTask = *renworld.GetRenderTask();
	deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
	const int width = defren.GetTextureLuminance()->GetWidth();
	const int height = defren.GetTextureLuminance()->GetHeight();
	
	// clear textures
	defren.ActivateFBOLuminanceNormal();
	
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
	if( renderThread.GetChoices().GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT, GL_ZERO_TO_ONE );
	}
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
	
	const GLfloat clearDepth = renderThread.GetChoices().GetClearDepthValueRegular();
	const GLfloat clearLuminance[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat clearNormal[ 4 ] = { 0.5f, 0.5f, 1.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearLuminance[ 0 ] ) );
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 1, &clearNormal[ 0 ] ) );
	
	// render sky. clears targets
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	SetCullMode( plan.GetFlipCulling() );
// 	GetRenderThread().GetRenderers().GetSky().RenderSky( plan );
	
	// render geometry
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDepthFunc( renderThread.GetChoices().GetDepthCompareFuncRegular() ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renworld.GetRenderLuminancePB() );
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
	
	if( collideList.GetHTSectorCount() > 0 ){
		// height terrain has to come first since it has to be handled differently
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapLuminance );
		addToRenderTask.AddHeightTerrains( collideList, true );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		renderTask.PrepareForRender( renderThread );
		rengeom.RenderTask( renderTask );
		
		renderTask.Clear();
		renderTask.SetRenderParamBlock( renworld.GetRenderLuminancePB() );
		addToRenderTask.AddHeightTerrains( collideList, false );
		OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
		OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
		renderTask.PrepareForRender( renderThread );
		rengeom.RenderTask( renderTask );
		
		renderTask.Clear();
		renderTask.SetRenderParamBlock( renworld.GetRenderLuminancePB() );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	}
	
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentLuminance );
	addToRenderTask.AddComponentsHighestLod( collideList );
	
	renderTask.PrepareForRender( renderThread );
	rengeom.RenderTask( renderTask );
}
#endif
