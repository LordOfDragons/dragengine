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
#include "deoglRenderTranspCounting.h"
#include "deoglRenderWorld.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanMasked.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglRenderTask.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideList.h"
#include "../component/deoglRComponent.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../debug/debugSnapshot.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../occquery/deoglOcclusionQuery.h"
#include "../occquery/deoglOcclusionQueryManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTChoices.h"
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
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../texture/deoglTextureStageManager.h"
#include "../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eSPTraCountMaxCount{
	sptcmcClampTC,
	sptcmcOffsets1,
	sptcmcOffsets2,
	sptcmcOffsets3,
	sptcmcOffsets4
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



// Class deoglRenderTranspCounting
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTranspCounting::deoglRenderTranspCounting( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread ),
pOccQuery( NULL ),
pHasCount( false ),
pCount( 0 )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderDefines defines, commonDefines;
	deoglShaderSources *sources;
	
	renderThread.GetShader().SetCommonDefines( commonDefines );
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed( "DefRen Transparency Max Count" );
	defines.SetDefines( "NO_POSTRANSFORM", "NO_TCTRANSFORM" );
	pShaderTraCountMaxCount = shaderManager.GetProgramWith( sources, defines );
	
	if( renderThread.GetChoices().GetRenderFSQuadStereoVSLayer() ){
		defines.SetDefines( "VS_RENDER_STEREO" );
		
	}else{
		sources = shaderManager.GetSourcesNamed( "DefRen Transparency Max Count Stereo" );
		defines.SetDefine( "GS_RENDER_STEREO", true );
	}
	pShaderTraCountMaxCountStereo = shaderManager.GetProgramWith( sources, defines );
	
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed( "DefRen Transparency Get Count" );
	defines.SetDefines( "NO_POSTRANSFORM", "NO_TEXCOORD" );
	pShaderTraCountGetCount = shaderManager.GetProgramWith( sources, defines );
	
	if( renderThread.GetChoices().GetRenderFSQuadStereoVSLayer() ){
		defines.SetDefines( "VS_RENDER_STEREO" );
		
	}else{
		sources = shaderManager.GetSourcesNamed( "DefRen Transparency Get Count Stereo" );
		defines.SetDefine( "GS_RENDER_STEREO", true );
	}
	pShaderTraCountGetCountStereo = shaderManager.GetProgramWith( sources, defines );
	
	
	pOccQuery = new deoglOcclusionQuery( renderThread );
}

deoglRenderTranspCounting::~deoglRenderTranspCounting(){
	pCleanUp();
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



void deoglRenderTranspCounting::CountTransparency( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask ){
DBG_ENTER_PARAM("deoglRenderTranspCounting::CountTransparency", "%p", mask)
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "TranspCounting.CountTransparency" );
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglCollideList &collideList = plan.GetCollideList();
	deoglRenderTask &renderTask = *renworld.GetRenderTask();
	int realWidth = defren.GetWidth();
	int realHeight = defren.GetHeight();
	int curWidth, curHeight;
	bool useTexture1;
	int nextSize;
	
	
	// drop cached count
	pHasCount = false;
	pCount = 0;
	
	
	// attach the first counter texture to store the count of layers per pixel
	defren.ActivateFBODiffuse( true );
	OGL_CHECK( renderThread, glViewport( 0, 0, realWidth, realHeight ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, realWidth, realHeight ) );
	
	// set opengl states
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	//OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_CONSTANT_COLOR, GL_ONE ) );
	OGL_CHECK( renderThread, pglBlendColor( 1.0f / 255.0f, 0.0f, 0.0f, 0.0f ) );
	
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( plan.GetFlipCulling() );
	
	if( mask ){
		OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, 0x01, 0x01 ) );
		
	}else{
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	}
	
	// clear the counter texture to 0
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	
	
	// render transparent scene elements
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renworld.GetRenderPB() );
	renderTask.SetRenderVSStereo( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() );
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid( false );
	addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
	
	// components
	if( mask && mask->GetUseClipPlane() ){
		addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
			? deoglSkinTexture::estStereoComponentCounterClipPlane
			: deoglSkinTexture::estComponentCounterClipPlane );
		
	}else{
		addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
			? deoglSkinTexture::estStereoComponentCounter
			: deoglSkinTexture::estComponentCounter );
	}
	
	addToRenderTask.AddComponents( collideList );
	
	// billboards
	if( mask && mask->GetUseClipPlane() ){
		addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
			? deoglSkinTexture::estStereoBillboardCounterClipPlane
			: deoglSkinTexture::estBillboardCounterClipPlane );
		
	}else{
		addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
			? deoglSkinTexture::estStereoBillboardCounter
			: deoglSkinTexture::estBillboardCounter );
	}
	
	addToRenderTask.AddBillboards( collideList );
	
	// particles
	if( renderThread.GetChoices().GetRealTransparentParticles() ){
		if( mask && mask->GetUseClipPlane() ){
			addToRenderTask.SetSkinShaderType(
				deoglSkinTexture::estParticleCounterClipPlane );
			addToRenderTask.SetSkinShaderTypeRibbon( plan.GetRenderStereo()
				? deoglSkinTexture::estStereoParticleRibbonCounterClipPlane
				: deoglSkinTexture::estParticleRibbonCounterClipPlane );
			addToRenderTask.SetSkinShaderTypeBeam( plan.GetRenderStereo()
				? deoglSkinTexture::estStereoParticleBeamCounterClipPlane
				: deoglSkinTexture::estParticleBeamCounterClipPlane );
			
		}else{
			addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
				? deoglSkinTexture::estStereoParticleCounter
				: deoglSkinTexture::estParticleCounter );
			addToRenderTask.SetSkinShaderTypeRibbon( plan.GetRenderStereo()
				? deoglSkinTexture::estStereoParticleRibbonCounter
				: deoglSkinTexture::estParticleRibbonCounter );
			addToRenderTask.SetSkinShaderTypeBeam( plan.GetRenderStereo()
				? deoglSkinTexture::estStereoParticleBeamCounter
				: deoglSkinTexture::estParticleBeamCounter );
		}
		addToRenderTask.AddParticles( collideList );
	}
	
	// render
	renderTask.PrepareForRender();
	rengeom.RenderTask( renderTask );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspCounting ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureConversion( *defren.GetTextureDiffuse(),
			"transp_count", deoglDebugSaveTexture::ecNoConversion );
		//renderThread.GetConfiguration()->SetDebugSnapshot( 0 );
		renderTask.DebugPrint( renderThread.GetLogger() );
	}
	
	
	// outline
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renworld.GetRenderPB() );
	renderTask.SetRenderVSStereo( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() );
	
	addToRenderTask.Reset();
	addToRenderTask.SetOutline( true );
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.SetSolid( false );
	addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
	
	if( mask && mask->GetUseClipPlane() ){
		addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
			? deoglSkinTexture::estStereoOutlineCounterClipPlane
			: deoglSkinTexture::estOutlineCounterClipPlane );
		
	}else{
		addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
			? deoglSkinTexture::estStereoOutlineCounter
			: deoglSkinTexture::estOutlineCounter );
	}
	
	addToRenderTask.AddComponents( collideList );
	
	if( renderTask.GetShaderCount() > 0 ){
		renderTask.PrepareForRender();
		SetCullMode( ! plan.GetFlipCulling() );
		rengeom.RenderTask( renderTask );
		SetCullMode( plan.GetFlipCulling() );
	}
	
	
	// calculate the maximum layer count. uses ping pong between diffuse and reflectivity buffer.
	deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderTraCountMaxCountStereo : pShaderTraCountMaxCount;
	renderThread.GetShader().ActivateShader( program );
	deoglShaderCompiled * const shader = program->GetCompiled();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) ); // not required from here on
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	useTexture1 = true;
	curWidth = realWidth;
	curHeight = realHeight;
	
	while( curWidth > 1 || curHeight > 1 ){
		// reduce in x direction
		if( curWidth > 1 ){
			nextSize = ( curWidth - 1 ) / 8 + 1;
			
			if( useTexture1 ){
				defren.ActivateFBOReflectivity( false );
				tsmgr.EnableArrayTexture( 0, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
				
			}else{
				defren.ActivateFBODiffuse( false );
				tsmgr.EnableArrayTexture( 0, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
			}
			OGL_CHECK( renderThread, glViewport( 0, 0, nextSize, curHeight ) );
// 			OGL_CHECK( renderThread, glScissor( 0, 0, nextSize, curHeight ) );
			
			OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
			
			shader->SetParameterInt( sptcmcClampTC, 0, 0, curWidth - 1, curHeight - 1 );
			shader->SetParameterInt( sptcmcOffsets1, 1, 0, 2, 0 );
			shader->SetParameterInt( sptcmcOffsets2, 3, 0, 4, 0 );
			shader->SetParameterInt( sptcmcOffsets3, 5, 0, 6, 0 );
			shader->SetParameterInt( sptcmcOffsets4, 7, 0, 8, 1 );
			
			RenderFullScreenQuad( plan );
			
			if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspCounting ){
				decString text;
				text.Format( "transp_count_max_u_%ix%i_%i", curWidth, curHeight, nextSize );
				if( useTexture1 ){
					//defren.ActivateFBODiffuse( true );
					renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture( *defren.GetTextureReflectivity(), text );
				}else{
					//defren.ActivateFBOReflectivity( true );
					renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture( *defren.GetTextureDiffuse(), text );
				}
			}
			
			useTexture1 = ! useTexture1;
			curWidth = nextSize;
		}
		
		// reduce in y direction
		if( curHeight > 1 ){
			nextSize = ( curHeight - 1 ) / 8 + 1;
			
			if( useTexture1 ){
				defren.ActivateFBOReflectivity( false );
				tsmgr.EnableArrayTexture( 0, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
				
			}else{
				defren.ActivateFBODiffuse( false );
				tsmgr.EnableArrayTexture( 0, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
			}
			OGL_CHECK( renderThread, glViewport( 0, 0, curWidth, nextSize ) );
// 			OGL_CHECK( renderThread, glScissor( 0, 0, curWidth, nextSize ) );
			
			OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
			
			shader->SetParameterInt( sptcmcClampTC, 0, 0, curWidth - 1, curHeight - 1 );
			shader->SetParameterInt( sptcmcOffsets1, 0, 1, 0, 2 );
			shader->SetParameterInt( sptcmcOffsets2, 0, 3, 0, 4 );
			shader->SetParameterInt( sptcmcOffsets3, 0, 5, 0, 6 );
			shader->SetParameterInt( sptcmcOffsets4, 0, 7, 1, 8 );
			
			RenderFullScreenQuad( plan );
			
			if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspCounting ){
				decString text;
				text.Format( "transp_count_max_v_%ix%i_%i", curWidth, curHeight, nextSize );
				if( useTexture1 ){
					//defren.ActivateFBODiffuse( true );
					renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture( *defren.GetTextureReflectivity(), text );
				}else{
					//defren.ActivateFBOReflectivity( true );
					renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture( *defren.GetTextureDiffuse(), text );
				}
			}
			
			useTexture1 = ! useTexture1;
			curHeight = nextSize;
		}
	}
	
	
	// start the occlusion query to determine the count. occlusion queries always have a little
	// delay so we fetch the result after the solid pass
	//OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	#ifdef OS_ANDROID
		// OpenGL ES does not support counting queries only any samples passed type queries.
		// we are forced to do a slower glReadPixels on the last set frame buffer attachment
		GLbyte queryResult[ 4 ];
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		OGL_CHECK( renderThread, glReadPixels( 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, ( GLvoid* )&queryResult ) );
		
		plan.SetTransparencyLayerCount( queryResult[ 0 ] );
		
	#else
		if( useTexture1 ){
			defren.ActivateFBOReflectivity( false );
			tsmgr.EnableArrayTexture( 0, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
			
		}else{
			defren.ActivateFBODiffuse( false );
			tsmgr.EnableArrayTexture( 0, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		}
		OGL_CHECK( renderThread, glViewport( 0, 0, 100, 1 ) );
// 		OGL_CHECK( renderThread, glScissor( 0, 0, 100, 1 ) );
		
		renderThread.GetShader().ActivateShader( plan.GetRenderStereo() ? pShaderTraCountGetCountStereo : pShaderTraCountGetCount );
		
		renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
		
		const GLfloat clearColor2[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor2[ 0 ] ) );
		
		pOccQuery->BeginQuery( deoglOcclusionQuery::eqtCount );
		RenderFullScreenQuad( plan );
		pOccQuery->EndQuery();
		
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	#endif
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspCounting ){
		pOccQuery->GetResult();
		tsmgr.DisableStage( 0 );
		if( useTexture1 ){
			renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture(
				*defren.GetTextureReflectivity(), "transp_count_get" );
		}else{
			renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture(
				*defren.GetTextureDiffuse(), "transp_count_get" );
		}
	}
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspCounting ){
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	
	// invalidate buffer. it is not needed anymore
	renderThread.GetFramebuffer().GetActive()->InvalidateColor( 0 );
	
	// reset
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
DBG_EXIT("deoglRenderTranspCounting::CountTransparency")
}



int deoglRenderTranspCounting::GetCount(){
	if( ! pHasCount ){
		pCount = pOccQuery->GetResult();
		pHasCount = true;
	}
	
	return pCount;
}



// Private Functions
//////////////////////

void deoglRenderTranspCounting::pCleanUp(){
	if( pOccQuery ){
		delete pOccQuery;
	}
}
