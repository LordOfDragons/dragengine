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
#include "plan/deoglRenderPlan.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglRenderTask.h"
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideList.h"
#include "../debug/debugSnapshot.h"
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
deoglRenderBase( renderThread )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	sources = shaderManager.GetSourcesNamed( "DefRen Skin Debug" );
	pShaderDEBUG.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
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



void deoglRenderGeometryPass::RenderDecals( deoglRenderPlan &plan ){
DBG_ENTER("RenderDecals")
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const deoglCollideList &collideList = plan.GetCollideList();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderTask &renderTask = *renworld.GetRenderTask();
	deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
	
	// set opengl states
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	
	OGL_CHECK( renderThread, glEnable( GL_POLYGON_OFFSET_FILL ) );
	
	if( defren.GetUseInverseDepth() ){
		OGL_CHECK( renderThread, pglPolygonOffset( -config.GetDecalOffsetScale(), -config.GetDecalOffsetBias() ) );
		
	}else{
		OGL_CHECK( renderThread, pglPolygonOffset( config.GetDecalOffsetScale(), config.GetDecalOffsetBias() ) );
	}
	
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	
	// render using render task
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renworld.GetRenderPB() );
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estDecalGeometry );
	
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( true );
	addToRenderTask.SetSolid( true );
	addToRenderTask.AddComponents( collideList );
	addToRenderTask.SetSolid( false );
	addToRenderTask.AddComponents( collideList );
	addToRenderTask.SetFilterDecal( false );
	
	addToRenderTask.AddDecals( collideList );
	
	renderTask.PrepareForRender( renderThread );
	rengeom.RenderTask( renderTask );
	
	// cleanup
	OGL_CHECK( renderThread, glDisable( GL_POLYGON_OFFSET_FILL ) );
DBG_EXIT("RenderDecals")
}

void deoglRenderGeometryPass::RenderSolidGeometryPass( deoglRenderPlan &plan, deoglRenderPlanMasked *mask ){
DBG_ENTER_PARAM("RenderSolidGeometryPass", "%p", mask)
	deoglRenderThread &renderThread = GetRenderThread();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglRenderDepthPass &rendepth = renderThread.GetRenderers().GetDepthPass();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	const deoglCollideList &collideList = plan.GetCollideList();
	deoglConfiguration &config = renderThread.GetConfiguration();
	deoglRenderTask &renderTask = *renworld.GetRenderTask();
	deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
	
	// switch to wireframe mode if required
	if( renderThread.GetConfiguration().GetDebugWireframe() ){
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	
	
	// render pre-pass depth. this includes rendering depth, occlusion testing and transparency counting
	rendepth.RenderSolidDepthPass( plan, mask );
	
	QUICK_DEBUG_START( 14, 19 )
	
	DebugTimer1Reset( plan, true );
	
	// geometry pass. render first sky which uses FBO Depth+Color. sky always clears color
	// bufffer.
	// 
	// TODO change the sky rendering so it can use FBO Material (render to color only). this
	//      would avoid the need to switch FBO attachment. maybe glDrawBuffers switching
	//      temporarily can help? needs first some research if glDrawBuffers has similar
	//      caveats performance wise as switching FBOs has
// 	defren.ActivateFBOMaterialColor();
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	SetCullMode( plan.GetFlipCulling() );
	
	if( mask ){
		OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, 0x01, 0x01 ) );
		
	}else{
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	}
	
	QUICK_DEBUG_START( 16, 19 )
	GetRenderThread().GetRenderers().GetSky().RenderSky( plan );
	DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometrySky, true );
	QUICK_DEBUG_END
	
	
	
	// activate material fbo and clear all color attachments except color and depth buffer
	defren.ActivateFBOMaterialColor();
	
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		// required since sky pass needs TTTF
	
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
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
	
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	
	// render geometry
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_EQUAL ) );
	
	OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE ) );
	OGL_CHECK( renderThread, glStencilMask( plan.GetStencilWriteMask() ) );
	
	if( mask ){
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), 0x01 ) );
		
	}else{
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, plan.GetStencilRefValue(), 0x00 ) );
	}
	
	
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renworld.GetRenderPB() );
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
	
	// height terrain has to come first since it has to be handled differently
	if( collideList.GetHTSectorCount() > 0 ){
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapGeometry );
		addToRenderTask.AddHeightTerrains( collideList, true );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		renderTask.PrepareForRender( renderThread );
		if( config.GetDebugSnapshot() == edbgsnapDepthPassRenTask ){
			renderThread.GetLogger().LogInfo( "RenderWorld.pRenderGeometryPass: render task height terrain pass 1" );
			renderTask.DebugPrint( renderThread.GetLogger() );
		}
		rengeom.RenderTask( renderTask );
		
		renderTask.Clear();
		renderTask.SetRenderParamBlock( renworld.GetRenderPB() );
		addToRenderTask.AddHeightTerrains( collideList, false );
		OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
		OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
		renderTask.PrepareForRender( renderThread );
		if( config.GetDebugSnapshot() == edbgsnapDepthPassRenTask ){
			renderThread.GetLogger().LogInfo( "RenderWorld.pRenderGeometryPass: render task height terrain pass 2" );
			renderTask.DebugPrint( renderThread.GetLogger() );
		}
		rengeom.RenderTask( renderTask );
		
		renderTask.Clear();
		renderTask.SetRenderParamBlock( renworld.GetRenderPB() );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	}
	
	// build render task
	if( defren.GetUseFadeOutRange() && false /* alpha blend problem */ ){
		OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
		OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
		
	}else{
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	}
	
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentGeometry );
	addToRenderTask.AddComponents( collideList );
	addToRenderTask.SetFilterDecal( false );
	
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estBillboardGeometry );
	addToRenderTask.AddBillboards( collideList );
	
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldGeometry );
	addToRenderTask.AddPropFields( collideList, false );
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldImposterGeometry );
	addToRenderTask.AddPropFields( collideList, true );
	
	if( renderThread.GetChoices().GetRealTransparentParticles() ){
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estParticleGeometry );
		addToRenderTask.SetSkinShaderTypeRibbon( deoglSkinTexture::estParticleRibbonGeometry );
		addToRenderTask.SetSkinShaderTypeBeam( deoglSkinTexture::estParticleBeamGeometry );
		addToRenderTask.AddParticles( collideList );
	}
	
	renderTask.PrepareForRender( renderThread );
	
	if( config.GetDebugSnapshot() == edbgsnapDepthPassRenTask ){
		renderThread.GetLogger().LogInfo( "RenderWorld.pRenderGeometryPass: render task" );
		renderTask.DebugPrint( renderThread.GetLogger() );
		config.SetDebugSnapshot( 0 );
	}
	DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryTask, true );
	
	rengeom.RenderTask( renderTask );
	DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryRender, true );
	
	
	// outline
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renworld.GetRenderPB() );
	
	addToRenderTask.Reset();
	addToRenderTask.SetOutline( true );
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
	
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estOutlineGeometry );
	addToRenderTask.AddComponents( collideList );
	
	renderTask.PrepareForRender( renderThread );
	DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryTask, true );
	
	if( renderTask.GetShaderCount() > 0 ){
		SetCullMode( ! plan.GetFlipCulling() );
		rengeom.RenderTask( renderTask );
		SetCullMode( plan.GetFlipCulling() );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryRender, true );
	}
	
	
	// decals
	RenderDecals( plan );
// 	OGL_CHECK( renderThread, glDepthFunc( GL_EQUAL ) );  // WARNING! this can disable z-optimizations if switched, really necessary???????????
	DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryDecals, true );
	QUICK_DEBUG_END
	
	
	// downsample depth into mip-map levels if some later feature requires this. this can be done only after the
	// geometry pass since this writes to the stencil mask of the depth texture. writing to the stencil part
	// trashes mip map levels although no depth writing is enabled
	if( config.GetSSAOEnable() && ! plan.GetNoReflections() ){
		rendepth.DownsampleDepth();
		//OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
		//OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
		//OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryDownsampleDepth, true );
	}
	
	// screen space ambient occlusion
	if( ! plan.GetNoReflections() ){
		renderThread.GetRenderers().GetLight().RenderAO( plan, true );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometrySSAO, true );
	}
	
	// if we used wireframe mode disable it now
	if( renderThread.GetConfiguration().GetDebugWireframe() ){
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
DBG_EXIT("RenderSolidGeometryPass")
}
