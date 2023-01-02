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
#include "deoglRenderParticles.h"
#include "deoglRenderReflection.h"
#include "deoglRenderTransparentPasses.h"
#include "deoglRenderWorld.h"
#include "defren/deoglDeferredRendering.h"
#include "light/deoglRenderLight.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanLight.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglAddToRenderTaskParticles.h"
#include "task/deoglRenderTask.h"
#include "task/deoglRenderTaskParticles.h"
#include "../capabilities/deoglCapabilities.h"
#include "../debug/deoglDebugSnapshot.h"
#include "../debug/debugSnapshot.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../particle/deoglParticleSorter.h"
#include "../particle/deoglRParticleEmitterInstance.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../texture/deoglTextureStageManager.h"

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

enum eSPCopyDepth{
	spcdQuadParams
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



// Class deoglRenderTransparentPasses
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTransparentPasses::deoglRenderTransparentPasses( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread )
{
	const bool renderFSQuadStereoVSLayer = renderThread.GetChoices().GetRenderFSQuadStereoVSLayer();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const bool useInverseDepth = renderThread.GetChoices().GetUseInverseDepth();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	deoglShaderSources *sources;
	
	renderThread.GetShader().SetCommonDefines( commonDefines );
	
	
	// copy depth color
	pipconf.Reset();
	pipconf.SetMasks( true, true, true, true, true );
	pipconf.EnableDepthTest( renderThread.GetChoices().GetDepthCompareFuncRegular() );
	pipconf.EnableDynamicStencilTest();
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed( "DefRen Copy Depth" );
	
	defines.SetDefines( "DEPTH_TEST", "COPY_COLOR" );
	if( useInverseDepth ){
		defines.SetDefine( "SHADOW_INVERSE_DEPTH", true );
	}
	pipconf.SetShader( renderThread, sources, defines );
	pPipelineCopyDepthColor = pipelineManager.GetWith( pipconf );
	
	// copy depth color stereo
	defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
	if( ! renderFSQuadStereoVSLayer ){
		sources = shaderManager.GetSourcesNamed( "DefRen Copy Depth Stereo" );
	}
	pipconf.SetShader( renderThread, sources, defines );
	pPipelineCopyDepthColorStereo = pipelineManager.GetWith( pipconf );
	
	
	// copy depth limit
	pipconf.Reset();
	pipconf.SetMasks( false, false, false, false, true );
	pipconf.EnableDepthTest( renderThread.GetChoices().GetDepthCompareFuncReversed() );
	pipconf.EnableDynamicStencilTest();
	
	defines = commonDefines;
	defines.SetDefine( "DEPTH_TEST", true );
	if( ! useInverseDepth ){
		defines.SetDefine( "SHADOW_INVERSE_DEPTH", true );
	}
	pipconf.SetShader( renderThread, sources, defines );
	pPipelineCopyDepthLimit = pipelineManager.GetWith( pipconf );
	
	// copy depth limit stereo
	defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
	pipconf.SetShader( renderThread, sources, defines );
	pPipelineCopyDepthLimitStereo = pipelineManager.GetWith( pipconf );
	
	
	// copy color
	pipconf.Reset();
	pipconf.SetMasks( true, true, true, true, false );
	pipconf.SetEnableScissorTest( true );
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed( "DefRen Copy Color" );
	defines.SetDefine( "INPUT_ARRAY_TEXTURE", true );
	pipconf.SetShader( renderThread, sources, defines );
	pPipelineCopyColor = pipelineManager.GetWith( pipconf );
	
	// copy color stereo
	defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
	if( ! renderFSQuadStereoVSLayer ){
		sources = shaderManager.GetSourcesNamed( "DefRen Copy Color Stereo" );
	}
	pipconf.SetShader( renderThread, sources, defines );
	pPipelineCopyColorStereo = pipelineManager.GetWith( pipconf );
}

deoglRenderTransparentPasses::~deoglRenderTransparentPasses(){
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



void deoglRenderTransparentPasses::RenderTransparentPasses( deoglRenderPlan &plan,
const deoglRenderPlanMasked *mask, bool xray ){
DBG_ENTER_PARAM("RenderTransparentPasses", "%p", mask)
	// to handle pixel correct transparency a depth peeling approach is used but different
	// than the conventional depth peeling approach. in the conventional approach peeling
	// works from the front to the back using an occlusion query after every pass. this is
	// slow and thus requires a better solution. this approach here though uses a pre depth
	// counter pass that determines the number of depth passes required. no occlusion queries
	// are required with this approach during transparency rendering. also peeling works from
	// the back to the front. only this solution provides correct transparency in the case
	// of distortion transparency. optmizations are still possible.
	// 
	// the algorithm works the following way basically. for each layer the following is done:
	// 
	// 1) swap depth textures. this allows to use the first depth texture for depth testing
	//    in the depth shader. the second depth texture is used throughout the rest now
	// 
	// 2) clear depth/stenctil/diffuse texture. the depth is cleared to 0 instead of 1 since
	//    we peel from the front to the back. the stencil clear is required since the second
	//    depth texture receives the mask of the written pixels to speed up the upcoming
	//    render steps. the diffuse texture clear is required since the following lighting
	//    pass uses the alpha value to discard pixels not written by the geometry pass.
	//    this should be done with the stencil mask but some drivers are bugged and do not
	//    work correctly with a depth texture attached to the FBO and used as texture although
	//    the depth/stencil writing is disabled (ATI is prone of this bug). the diffuse alpha
	//    discard hack is the best solution around this problem
	// 
	// 3) render depth. the depth is rendered using greater-than depth testing. this keeps the
	//    back most fragment. the first depth texture is used to peel away fragments behind
	//    already rendered geometry. this peels layer by layer from the back to the front.
	//    furthermore stencil is set to keep-keep-replace with the current plan reference value.
	//    the reference value is thus written for every fragment in the current peeling layer.
	// 
	// 4) volumetric pass is rendered. these are particles or other volumetric effects. they
	//    do not write to the depth buffer but use the greater-than depth testing to clip
	//    against the current depth layer depths and the first depth texture to clip against
	//    the last layer depths. this renders only volumetric fragments between pixels of
	//    this layer and the previous layer. everything else will be written after all layers
	//    have been rendered. stencil is set to keep-keep-keep with equal testing against the
	//    reference value. the color is written directly to the color texture.
	// 
	// 5) render geometry. uses equal depth testing to write only the pixels in this layer.
	//    stencil is set the same as for the volumetric pass. the color is though written to
	//    the temporary texture since the color texture (including volumetric updates) is
	//    used for transparency
	// 
	// 6) swap textures. the first depth texture has to be updated now.
	// 
	// 7) copy depth/color. the first depth texture is updated with the depths of this layer
	//    as well as the color texture is updated with the colors written into the temporary
	//    texture. since the stencil mask from the previous steps is no more available (it
	//    is attached to the second depth texture) it has to be recreated from the copied
	//    depth values. the shader discards all fragments with a depth of 0. stencil is set
	//    to keep-keep-replace writing the reference value and using equal with the mask
	//    reference value. this makes mask working. the worst that can happen is that some
	//    fragments have been calculated that are dropped now.
	// 
	// after all layers are done the volumetric pass is rendered again. this time no back
	// peeling is required. only the depth test is enough. this renders now all volumetric
	// fragments that are totally in front of all solid and transparent geometry
	
	DebugTimer1Reset( plan, false );
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "TransparentPasses.RenderTransparentPasses" );
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	//deoglTextureStageManager &tsmgr = *renderThread.GetTexture().GetStages();
	//deoglShaderCompiled *shader;
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspPasses ){
		deoglDebugSnapshot snapshot( renderThread );
		snapshot.SetEnableDepthStencil( true );
		snapshot.SetEnableColor( true );
		snapshot.SetEnableMaterialBuffers( true );
		snapshot.SetName( "transparency/enter/" );
		snapshot.TakeSnapshot();
	}
	
	if( xray ? plan.GetHasXRayTransparency() : plan.GetHasTransparency() ){
		// limit depth if required. can modify plan transparency layer count
		RenderTransparentLimitDepth( plan, mask, xray );

		// render
		const int passCount = plan.GetTransparencyLayerCount();
		int p;
		
		for( p=0; p<passCount; p++ ){
			// determine the stencil parameters. due to the limited stencil bits available we have
			// to force a clearing of the stencil buffer render pass bits whenever we run beyond a
			// given number of passes. currently we use at most 4 bits for the render pass bits.
			// this allows for 15 layers of transparency before we wrap around requiring to clear.
			plan.SetCurrentTransparencyLayer( p );
			plan.SetStencilPrevRefValue( plan.GetStencilRefValue() );
			plan.SetRenderPassNumber( plan.GetRenderPassNumber() + 1 );
			
			if( plan.GetRenderPassNumber() == 16 ){
				plan.SetStencilPrevRefValue( 0 );
				plan.SetRenderPassNumber( 1 );
				plan.SetClearStencilPassBits( true );
			}
			
			if( mask ){
				plan.SetStencilRefValue( ( plan.GetRenderPassNumber() << 1 ) | 0x01 );
				plan.SetStencilWriteMask( 0xfe );
				
			}else{
				plan.SetStencilRefValue( plan.GetRenderPassNumber() << 1 );
				plan.SetStencilWriteMask( 0xff );
			}
			
			// render the transparent layer
			RenderTransparentGeometryPass( plan, mask, xray );
			
			if( ! mask ){
				renderThread.GetRenderers().GetReflection().CopyMaterial( plan, false );
			}
			
			if( ! plan.GetDisableLights() ){
				renderThread.GetRenderers().GetLight().RenderLights( plan, false, mask, xray );
				DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentLights, true );
			}
			
			renderThread.GetRenderers().GetReflection().RenderScreenSpace( plan );
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentSSR, true );
		}
		
		// required so the following volumetric pass knows if there had been transparent layers
		plan.SetCurrentTransparencyLayer( passCount );
		
	}else{
		plan.SetCurrentTransparencyLayer( 0 );
	}
	
	plan.SetStencilPrevRefValue( plan.GetStencilRefValue() );
	// this volumetric pass happens no matter if there have been transparent layers or not. the code expects
	// depth2 to contain the latest depth texture hence the depth textures have to be switch prior and after.
	// in contrary to the volumetric passes rendered between transparent layers this one here does not
	// require two depth tests (false at the end)
	RenderVolumetricPass( plan, mask, false );
	DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentVolumetric, true );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspPasses ){
		deoglDebugSnapshot snapshot( renderThread );
		snapshot.SetEnableDepthStencil( true );
		snapshot.SetEnableColor( true );
		snapshot.SetEnableMaterialBuffers( true );
		snapshot.SetName( "transparency/exit/" );
		snapshot.TakeSnapshot();
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	
	renworld.GetRenderTaskParticles()->RemoveAllSteps();
DBG_EXIT("RenderTransparentPasses")
}



void deoglRenderTransparentPasses::RenderTransparentGeometryPass( deoglRenderPlan &plan,
const deoglRenderPlanMasked *mask, bool xray ){
DBG_ENTER_PARAM("RenderTransparentGeometryPass", "%p", mask)
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "TransparentPasses.RenderTransparentGeometryPass" );
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	//deoglRenderDecal &rendecal = renderThread.GetRenderers().GetDecal();
	deoglRenderDepthPass &rendepth = renderThread.GetRenderers().GetDepthPass();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglCollideList &collideList = plan.GetCollideList();
	deoglRenderTask &renderTask = *renworld.GetRenderTask();
	deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
	
	DebugTimer2Reset( plan, false );
	
	// render process of transparency in a nut shell. the goal is to obtain fully valid textures color, diffuse,
	// normal, specularity and depth after the transparency pass is finished. to get this properly working across
	// all the different parts the following process is used:
	// 
	// - clear diffuse to 0, depth2 to 0 and stencil to 0. 0 in depth2 and diffuse marks not written pixels
	//   (notes) diffuse clear is not required anymore if stencil mask works properly for lighting
	// 
	// - render depth to depth2 using GEQUAL without stencil using depth1 as shader back limit. depth is written.
	//   stencil is written with current layer stencil reference value
	// 
	// (status) depth2 contains current layer depth and stencil. depth1 contains last layer depth
	// 
	// - optionally copy color to temporary without mask if particles require a color copy
	// 
	// (status) (optionally) temporary contains last layer color
	// 
	// - render volumetrics to color using depth2 as depth test front limit with GEQUAL and stencil mask, depth1
	//   as shader test back limit and optionally temporary as last layer color. no depth or stencil is written.
	//   (notes) requires back-test in shader for geometry skin shader (currently only in depth skin shader)
	// 
	// (status) color contains last layer color plus particles plus simple transparency components.
	// 
	// - clear depth1 stencil mask if required due to too many layers
	// 
	// - copy depth2 to depth1 and color to temporary using stencil mask and shader test to discard not written
	//   pixels (depth = 0). stencil is written with current layer stencil reference value
	// 
	// (status) depth1 contains last layer depth plus this layer depth changes as well as mask stencil updated with
	//          the current layer pixels written. depth2 is obsolete now. temporary contains last layer color
	// 
	// (status) temporary contains last layer color plus particles plus simple transparency components
	// 
	// - render geometry pass to depth1 using EQUAL with stencil mask and temporary with last layer color. no depth
	//   and stencil written. color, normal and specularity written for light pass
	// 
	// (status) color, normal, specularity, depth1 and stencil ready for light pass
	// 
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspPasses ){
		deoglDebugSnapshot snapshot( renderThread );
		snapshot.SetEnableStates( true );
		snapshot.SetEnableDepthStencil( true );
		snapshot.SetEnableColor( true );
		snapshot.SetEnableMaterialBuffers( true );
		decString name;
		name.Format( "transparency/pass%d/enter/", plan.GetCurrentTransparencyLayer() );
		snapshot.SetName( name );
		snapshot.TakeSnapshot();
	}
	
	// clear diffuse to 0, depth2 to 0 and stencil to 0. 0 in depth2 and diffuse marks not written pixels
	// (notes) diffuse clear is not required anymore if stencil mask works properly for lighting
	pPipelineClearBuffers->Activate();
	defren.SwapDepthTextures();
	defren.ActivateFBODiffuse( true );
	
	OGL_CHECK( renderThread, glStencilMask( ~0 ) );
	OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0,
		renderThread.GetChoices().GetClearDepthValueReversed(), 0 ) );
	
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentClear, true );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspPasses ){
		deoglDebugSnapshot snapshot( renderThread );
		snapshot.SetEnableStates( true );
		snapshot.SetEnableDepthStencil( true );
		snapshot.SetEnableColor( true );
		decString name;
		name.Format( "transparency/pass%d/clear/", plan.GetCurrentTransparencyLayer() );
		snapshot.SetName( name );
		snapshot.TakeSnapshot();
	}
	
	
	// render depth to depth2 using GEQUAL without stencil using depth1 as shader back limit.
	// depth is written. stencil is written with current layer stencil reference value
	defren.ActivateFBODepth();
	
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE ) );
	OGL_CHECK( renderThread, glStencilMask( plan.GetStencilWriteMask() ) );
	OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, plan.GetStencilRefValue(), 0x0 ) );
	
	rendepth.RenderDepth( plan, mask, false, false, false, xray ); // -solid, -maskedOnly, -reverseDepthTest
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspPasses ){
		deoglDebugSnapshot snapshot( renderThread );
		snapshot.SetEnableDepthStencil( true );
		decString name;
		name.Format( "transparency/pass%d/depth_pass/", plan.GetCurrentTransparencyLayer() );
		snapshot.SetName( name );
		snapshot.TakeSnapshot();
	}
	
	// optionally copy color to temporary without mask if particles require a color copy
	// NOTE perhaps glCopyTexSubImage works better. activate FBO containing the texture to copy. then use
	//      glCopyTexSubImage to copy to a different texture. speed test required
	const bool requiresColorCopy = true;
	
	if( requiresColorCopy ){
		CopyColorToTemporary( plan );
		
		if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspPasses ){
			deoglDebugSnapshot snapshot( renderThread );
			snapshot.SetEnableColor( true );
			decString name;
			name.Format( "transparency/pass%d/copy_color/", plan.GetCurrentTransparencyLayer() );
			snapshot.SetName( name );
			snapshot.TakeSnapshot();
		}
	}
	
	// render volumetrics to color using depth2 as depth test front limit with GEQUAL and stencil mask, depth1
	// as shader test back limit and optionally temporary as last layer color. no depth or stencil is written.
	// (notes) requires back-test in shader for geometry skin shader (currently only in depth skin shader)
	RenderVolumetricPass( plan, mask, true );
	DebugTimer2Sample( plan, *renworld.GetDebugInfo().infoTransparentVolumetric, true );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspPasses ){
		deoglDebugSnapshot snapshot( renderThread );
		snapshot.SetEnableColor( true );
		decString name;
		name.Format( "transparency/pass%d/volumetric_pass/", plan.GetCurrentTransparencyLayer() );
		snapshot.SetName( name );
		snapshot.TakeSnapshot();
	}
	
	// clear depth1 stencil mask if required due to too many layers
	defren.SwapDepthTextures();
	
	if( plan.GetClearStencilPassBits() ){
		pPipelineClearBuffers->Activate();
		defren.ActivateFBODepth();
		
		OGL_CHECK( renderThread, glStencilMask( ~1 ) ); // mask bit has to stay intact
		const GLint clearStencil = 0;
		OGL_CHECK( renderThread, pglClearBufferiv( GL_STENCIL, 0, &clearStencil ) );
		
		plan.SetClearStencilPassBits( false );
	}
	
	// copy depth2 to depth1 and color to temporary using stencil mask and shader test to
	// discard not written pixels (depth = 0 or 1 depending on inverse mode). stencil is
	// written with current layer stencil reference value
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineCopyDepthColorStereo : *pPipelineCopyDepthColor;
	pipeline.Activate();
	defren.ActivateFBOTemporary1( true );
	
	OGL_CHECK( renderThread, glStencilMask( plan.GetStencilWriteMask() ) );
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE ) );
	
	if( mask ){
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), 0x1 ) );
		
	}else{
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, plan.GetStencilRefValue(), 0x0 ) );
	}
	
	deoglShaderCompiled &shader = *pipeline.GetGlShader()->GetCompiled();
	
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture2(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 1, *defren.GetTextureColor(), GetSamplerClampNearest() );
	
	defren.SetShaderParamFSQuad( shader, spcdQuadParams );
	RenderFullScreenQuadVAO( plan );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspPasses ){
		deoglDebugSnapshot snapshot( renderThread );
		snapshot.SetEnableDepthStencil( true );
		snapshot.SetEnableColor( true );
		decString name;
		name.Format( "transparency/pass%d/copy_depth_color/", plan.GetCurrentTransparencyLayer() );
		snapshot.SetName( name );
		snapshot.TakeSnapshot();
	}
	DebugTimer2Sample( plan, *renworld.GetDebugInfo().infoTransparentCopyDepth, true );
	
	
	// render geometry pass to depth1 using EQUAL with stencil mask and temporary with last layer
	// color. no depth and stencil written. color, normal and specularity written for light pass
	deoglDebugTraceGroup debugTraceGeometry( renderThread, "TransparentPasses.RenderTransparentPasses.Geometry" );
	pPipelineClearBuffers->Activate();
	defren.ActivateFBOMaterialColor();
	
	OGL_CHECK( renderThread, glStencilMask( 0 ) );
	
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
	
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
	OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), ~0 ) );
	
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renworld.GetRenderPB() );
	renderTask.SetRenderVSStereo( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() );
	
	int pipelineModifier = 0;
	if( plan.GetRenderStereo() ){
		pipelineModifier |= deoglSkinTexturePipelines::emStereo;
	}
	if( plan.GetFlipCulling() ){
		pipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
	}
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid( false );
	addToRenderTask.SetNoRendered( mask );
	addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
	addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etGeometry );
	addToRenderTask.SetSkinPipelineModifier( pipelineModifier );
	
	addToRenderTask.AddComponents( collideList );
	addToRenderTask.AddBillboards( collideList );
	addToRenderTask.AddPropFields( collideList, false );
	addToRenderTask.AddPropFields( collideList, true );
	if( renderThread.GetChoices().GetRealTransparentParticles() ){
		addToRenderTask.AddParticles( collideList );
	}
	
	renderTask.PrepareForRender();
	DebugTimer2Sample( plan, *renworld.GetDebugInfo().infoTransparentTask, true );
	
	rengeom.RenderTask( renderTask );
	DebugTimer2Sample( plan, *renworld.GetDebugInfo().infoTransparentRender, true );
	
	
	// outline
	deoglDebugTraceGroup debugTraceOutline( debugTraceGeometry, "TransparentPasses.RenderTransparentPasses.Outline" );
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renworld.GetRenderPB() );
	renderTask.SetRenderVSStereo( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() );
	
	addToRenderTask.Reset();
	addToRenderTask.SetOutline( true );
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.SetSolid( false );
	addToRenderTask.SetNoRendered( true );
	addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etGeometry );
	addToRenderTask.SetSkinPipelineModifier( pipelineModifier );
	
	addToRenderTask.AddComponents( collideList );
	
	renderTask.PrepareForRender();
	DebugTimer2Sample( plan, *renworld.GetDebugInfo().infoTransparentTask, true );
	
	if( renderTask.GetPipelineCount() > 0 ){
		rengeom.RenderTask( renderTask );
		DebugTimer2Sample( plan, *renworld.GetDebugInfo().infoTransparentRender, true );
	}
	
	
	//addToRenderTask.SetFilterHoles( false );
	//rendecal.RenderDecals( renderParams ); // hm... what about transparent decals on transparent objects?
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspPasses ){
		deoglDebugSnapshot snapshot( renderThread );
		snapshot.SetEnableDepthStencil( true );
		snapshot.SetEnableColor( true );
		snapshot.SetEnableMaterialBuffers( true );
		decString name;
		name.Format( "transparency/pass%d/geometry_pass/", plan.GetCurrentTransparencyLayer() );
		snapshot.SetName( name );
		snapshot.TakeSnapshot();
	}
DBG_EXIT("RenderTransparentGeometryPass")
}

void deoglRenderTransparentPasses::RenderTransparentLimitDepth(
deoglRenderPlan &plan, const deoglRenderPlanMasked *mask, bool xray ){
DBG_ENTER_PARAM("RenderTransparentLimitDepth", "%p", mask)
	deoglRenderThread &renderThread = GetRenderThread();
	deoglRenderDepthPass &rendepth = renderThread.GetRenderers().GetDepthPass();
	const int layerLimit = renderThread.GetConfiguration().GetTranspLayerLimit();
	if( plan.GetTransparencyLayerCount() <= layerLimit ){
		DBG_EXIT("RenderTransparentLimitDepth: Layer count below trashold")
		return;
	}
	
	// due to the limited stencil bits available we would have to force a clearing of the
	// stencil buffer render pass bits whenever we run beyond a given number of passes.
	// currently we use at most 4 bits for the render pass bits. this allows for 15 layers
	// of transparency before we wrap around requiring to clear. to avoid this the maximum
	// number of layers is forced to stay below 15
	const deoglDebugTraceGroup debugTrace( renderThread, "TransparentPasses.RenderTransparentLimitDepth.Outline" );
	const int prevStencilRefValue = plan.GetStencilRefValue();
	const int maskRefValue = mask ? 0x01 : 0;
	
	plan.SetStencilWriteMask( mask ? 0xfe : 0xff );
	
	// similar to RenderTransparentGeometryPass but doing only depth passes to obtain the depth
	// at the maximum number of layers allowed and doing it inverse. RenderTransparentGeometryPass
	// starts at the back most depth (the solid one) peeling layers moving forward to the front
	// most. for the depth limit though we start at the front most layer and peeling layers moving
	// backwards. this way only the front most N layers are kept. the solid pass depth buffer is
	// stored aside for depth testing only
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	// from here on the solid texture is called the first texture (depth1) and the texture
	// accumulating the depth is called the second texture (depth2). the third texture is
	// used as the working texture. depth textures are swap multiple times to ensure this
	// ordering is correct. this is required for TUCs since they use the second depth texture
	// for depth testing
	
	// clear second texture. this will be accumulate the limit
	pPipelineClearBuffers->Activate();
	defren.SwapDepthTextures(); // solid depth is now depth2
	defren.ActivateFBODepth();
	
	OGL_CHECK( renderThread, glStencilMask( 0x0 ) );
	const GLfloat clearDepth = renderThread.GetChoices().GetClearDepthValueReversed();
	OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	
	defren.SwapDepthTextures(); // solid depth is now depth1
	
	// calculate limit. the last layer is processed differently then all others
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	int l;
	
	for( l=0; l<layerLimit-1; l++ ){
		plan.SetCurrentTransparencyLayer( l );
		plan.SetStencilPrevRefValue( plan.GetStencilRefValue() );
		plan.SetRenderPassNumber( 1 + l );
		plan.SetStencilRefValue( ( ( 1 + l ) << 1 ) | maskRefValue );
		
		// clear depth3 to 0 and stencil to 0
		pPipelineClearBuffers->Activate();
		defren.ActivateFBODepth3();
		
		OGL_CHECK( renderThread, glStencilMask( ~0 ) );
		OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0,
			renderThread.GetChoices().GetClearDepthValueRegular(), 0 ) );
		
		// render depth to depth3 using reversed depth compare with depth tested against depth2
		// with withstencil. depth is written. stencil is written with current layer stencil
		// reference value.
		// 
		// we have to swap textures to move the solid depth to depth1 for all but the last layer
		// otherwise it is used for depth testing producing wrong results. the swap is undone
		// right after if applied
		OGL_CHECK( renderThread, glStencilMask( plan.GetStencilWriteMask() ) );
		OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE ) );
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, plan.GetStencilRefValue(), 0x0 ) );
		
		rendepth.RenderDepth( plan, mask, false, false, true, xray ); // -solid, -maskedOnly, +reverseDepthTest
		
		// copy depth3 to depth2 using stencil mask and shader test to discard not
		// written pixels. stencil is written with current layer stencil reference value
		const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineCopyDepthLimitStereo : *pPipelineCopyDepthLimit;
		pipeline.Activate();
		defren.SwapDepthTextures(); // solid depth is now depth2
		defren.ActivateFBODepth();
		
		OGL_CHECK( renderThread, glStencilMask( plan.GetStencilWriteMask() ) );
		
		if( mask ){
			OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), 0x1 ) );
			
		}else{
			OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, plan.GetStencilRefValue(), 0x0 ) );
		}
		
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture3(), GetSamplerClampNearest() );
		defren.SetShaderParamFSQuad( *pipeline.GetGlShader()->GetCompiled(), spcdQuadParams );
		
		RenderFullScreenQuadVAO( plan );
		
		defren.SwapDepthTextures(); // solid depth is now depth1
		
		if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspPasses ){
			deoglDebugSnapshot snapshot( renderThread );
			snapshot.SetEnableDepth3( true );
			decString name;
			name.Format( "transparency/depthlimit/pass%d-", l );
			snapshot.SetName( name );
			snapshot.TakeSnapshot();
		}
	}
	
	// for the last layer activate the solid depth to update with the last layer
	defren.ActivateFBODepth();
	
	plan.SetTransparencyLayerCount( layerLimit );
	plan.SetCurrentTransparencyLayer( 0 );
	plan.SetStencilPrevRefValue( prevStencilRefValue );
	plan.SetRenderPassNumber( 1 );
	plan.SetStencilRefValue( prevStencilRefValue );
	
	OGL_CHECK( renderThread, glStencilMask( plan.GetStencilWriteMask() ) );
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
	
	if( mask ){
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, prevStencilRefValue, 0x1 ) );
		
	}else{
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, prevStencilRefValue, 0x0 ) );
	}
	
	rendepth.RenderDepth( plan, mask, false, false, true, xray ); // -solid, -maskedOnly, +reverseDepthTest
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapTranspPasses ){
		deoglDebugSnapshot snapshot( renderThread );
		snapshot.SetEnableDepth( true );
		snapshot.SetName( "transparency/depthlimit/result-" );
		snapshot.TakeSnapshot();
	}
DBG_EXIT("RenderTransparentLimitDepth")
}



void deoglRenderTransparentPasses::RenderVolumetricPass( deoglRenderPlan &plan,
const deoglRenderPlanMasked *mask, bool inbetween ){
	deoglRenderThread &renderThread = GetRenderThread();
	if( renderThread.GetChoices().GetRealTransparentParticles() ){
		return;
	}
	
DBG_ENTER_PARAM2("RenderVolumetricPass", "%p", mask, "%d", inbetween)
	const deoglDebugTraceGroup debugTrace( renderThread, "TransparentPasses.RenderVolumetricPass" );
	const deoglParticleEmitterInstanceList &particleEmitterList = plan.GetCollideList().GetParticleEmitterList();
	const int particleEmitterCount = particleEmitterList.GetCount();
	
	if( particleEmitterCount == 0 ){
		return;
	}
	
	//bool enableLights = ! plan->GetDisableLights();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderParticles &renpart = GetRenderThread().GetRenderers().GetParticles();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	const decDVector &cameraPosition = plan.GetCameraPosition();
	const decVector cameraView = plan.GetInverseCameraMatrix().TransformView().ToVector();
	deoglParticleSorter &particleSorter = *renworld.GetParticleSorter();
	deoglRenderTaskParticles &renderTaskParticles = *renworld.GetRenderTaskParticles();
	deoglAddToRenderTaskParticles &addToRenderTaskParticles = *renworld.GetAddToRenderTaskParticles();
	int e, p;
	
	// sort particles only if this is the first layer. for all other layers we can reuse the result
	if( plan.GetCurrentTransparencyLayer() == 0 ){
		particleSorter.Clear();
		
		for( e=0; e<particleEmitterCount; e++ ){
			deoglRParticleEmitterInstance * const instance = particleEmitterList.GetAt( e );
			const int particleCount = instance->GetParticleCount();
			
			if( particleCount > 0 ){
				const decVector emitCamPos( instance->GetPosition() - cameraPosition );
				
				for( p=0; p<particleCount; p++ ){
					particleSorter.AddParticle( instance, p,
						( emitCamPos + instance->GetParticlePositionAt( p ) ) * cameraView );
				}
			}
		}
		
		particleSorter.Sort();
//DEBUG_PRINT_TIMER( "RenderWorld: Volumetric Pass Sort" );
	}
	
	// create the render task only if this is the first layer or we are not in between.
	// for all other passes we can reuse the last result
	if( ! inbetween || plan.GetCurrentTransparencyLayer() == 0 ){
		for( e=0; e<particleEmitterCount; e++ ){
			particleEmitterList.GetAt( e )->ClearIBO();
		}
		
		int pipelineModifier = 0;
		if( plan.GetRenderStereo() ){
			pipelineModifier |= deoglSkinTexturePipelines::emStereo;
		}
		if( plan.GetFlipCulling() ){
			pipelineModifier |= deoglSkinTexturePipelines::emFlipCullFace;
		}
		
		renderTaskParticles.Clear();
		renderTaskParticles.SetRenderParamBlock( renworld.GetRenderPB() );
// 		renderTaskParticles.SetRenderVSStereo( plan.GetRenderStereo() && renderThread.GetChoices().GetVRRenderStereo() );
		
		addToRenderTaskParticles.SetSkinPipelineType( inbetween
			? deoglSkinTexturePipelines::etGeometryDepthTest : deoglSkinTexturePipelines::etGeometry );
		addToRenderTaskParticles.SetSkinPipelineModifier( pipelineModifier );
		
		particleSorter.AddToRenderTask( addToRenderTaskParticles );
		
		for( e=0; e<particleEmitterCount; e++ ){
			particleEmitterList.GetAt( e )->UpdateIBO();
		}
		
		//renderTaskParticles.DebugPrint( renderThread.GetLogger() );
//DEBUG_PRINT_TIMER( "RenderWorld: Volumetric Pass Add To Render Task" );
	}
	
	// if this is not in-between we have to copy color texture first so it is present in temporary
// 	if( ! inbetween ){
// 		CopyColorToTemporary();
// 		// only required if config.SetTextureRenderColor(true). this though does not work
// 		// since the blend mode is (1,1-alpha).
// 	}
	
	// render particles back to front. the stencil mask settings is important. for the first
	// transparency layer the entire mask has to be rendered. this works since the depth has
	// been cleared to 0 and the test is GEQUAL. for all other layers the mask has to be the
	// reference value mask to render only behind the pixels affecting the current layer.
	defren.ActivateFBOColor( true, false );
	
	OGL_CHECK( renderThread, glStencilMask( 0 ) );
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
	if( inbetween ){
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), ~0 ) );
		
	}else if( mask ){
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, 0x1, 0x1 ) );
		
	}else{
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, 0x0, 0x0 ) );
	}
	
	renpart.RenderTaskParticles( renderTaskParticles );
//DEBUG_PRINT_TIMER( "RenderWorld: Volumetric Pass Render" );
DBG_EXIT("RenderVolumetricPass")
}



void deoglRenderTransparentPasses::CopyColorToTemporary( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "TransparentPasses.RenderTransparentLimitDepth.CopyColorToTemporary" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineCopyColorStereo : *pPipelineCopyColor;
	pipeline.Activate();
	defren.ActivateFBOTemporary1( false );
	
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	renderThread.GetTexture().GetStages().EnableArrayTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	
	defren.SetShaderParamFSQuad( *pipeline.GetGlShader()->GetCompiled(), spcdQuadParams );
	RenderFullScreenQuadVAO( plan );
}
