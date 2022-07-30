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
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShaderDefines defines, commonDefines;
	deoglShaderSources *sources;
	
	renderThread.GetShader().AddCommonDefines( commonDefines );
	
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed( "DefRen Copy Depth" );
	
	defines.AddDefine( "DEPTH_TEST", true );
	defines.AddDefine( "COPY_COLOR", true );
	if( defren.GetUseInverseDepth() ){
		defines.AddDefine( "SHADOW_INVERSE_DEPTH", true );
	}
	pShaderCopyDepthColor = shaderManager.GetProgramWith( sources, defines );
	
	defines = commonDefines;
	defines.AddDefine( "DEPTH_TEST", true );
	if( ! defren.GetUseInverseDepth() ){
		defines.AddDefine( "SHADOW_INVERSE_DEPTH", true );
	}
	pShaderCopyDepthLimit = shaderManager.GetProgramWith( sources, defines );
	
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed( "DefRen Copy Depth Stereo" );
	
	defines.AddDefine( "GS_RENDER_STEREO", true );
	defines.AddDefine( "DEPTH_TEST", true );
	defines.AddDefine( "COPY_COLOR", true );
	if( defren.GetUseInverseDepth() ){
		defines.AddDefine( "SHADOW_INVERSE_DEPTH", true );
	}
	pShaderCopyDepthColorStereo = shaderManager.GetProgramWith( sources, defines );
	
	defines = commonDefines;
	defines.AddDefine( "GS_RENDER_STEREO", true );
	defines.AddDefine( "DEPTH_TEST", true );
	if( ! defren.GetUseInverseDepth() ){
		defines.AddDefine( "SHADOW_INVERSE_DEPTH", true );
	}
	pShaderCopyDepthLimitStereo = shaderManager.GetProgramWith( sources, defines );
	
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed( "DefRen Copy Color" );
	defines.AddDefine( "INPUT_ARRAY_TEXTURE", true );
	pShaderCopyColor = shaderManager.GetProgramWith( sources, defines );
	
	sources = shaderManager.GetSourcesNamed( "DefRen Copy Color Stereo" );
	defines.AddDefine( "GS_RENDER_STEREO", true );
	pShaderCopyColorStereo = shaderManager.GetProgramWith( sources, defines );
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



void deoglRenderTransparentPasses::RenderTransparentPasses( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask ){
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
	
	if( plan.GetHasTransparency() ){
		// limit depth if required. can modify plan transparency layer count
		RenderTransparentLimitDepth( plan, mask );
		
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
			RenderTransparentGeometryPass( plan, mask );
			
			if( ! mask ){
				renderThread.GetRenderers().GetReflection().CopyMaterial( plan, false );
			}
			
			if( ! plan.GetDisableLights() ){
				renderThread.GetRenderers().GetLight().RenderLights( plan, false, mask );
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



void deoglRenderTransparentPasses::RenderTransparentGeometryPass( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask ){
DBG_ENTER_PARAM("RenderTransparentGeometryPass", "%p", mask)
	deoglRenderThread &renderThread = GetRenderThread();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	//deoglRenderDecal &rendecal = renderThread.GetRenderers().GetDecal();
	deoglRenderDepthPass &rendepth = renderThread.GetRenderers().GetDepthPass();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglCollideList &collideList = plan.GetCollideList();
	deoglRenderTask &renderTask = *renworld.GetRenderTask();
	deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
	deoglShaderCompiled *shader;
	
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
	
	// switch to wireframe mode if required
	if( renderThread.GetConfiguration().GetDebugWireframe() ){
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	
	// clear diffuse to 0, depth2 to 0 and stencil to 0. 0 in depth2 and diffuse marks not written pixels
	// (notes) diffuse clear is not required anymore if stencil mask works properly for lighting
	defren.SwapDepthTextures();
	defren.ActivateFBODiffuse( true );
	
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	OGL_CHECK( renderThread, glStencilMask( ~0 ) );
	OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, defren.GetClearDepthValueReversed(), 0 ) );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentClear, true );
	
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	
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
	
	OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE ) );
	OGL_CHECK( renderThread, glStencilMask( plan.GetStencilWriteMask() ) );
	OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, plan.GetStencilRefValue(), 0 ) );
	
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncReversed() ) );
	rendepth.RenderDepth( plan, mask, false, false, false ); // -solid, -maskedOnly, -reverseDepthTest
	
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
		defren.ActivateFBODepth();
		
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		OGL_CHECK( renderThread, glStencilMask( ~1 ) ); // mask bit has to stay intact
		const GLint clearStencil = 0;
		OGL_CHECK( renderThread, pglClearBufferiv( GL_STENCIL, 0, &clearStencil ) );
		
		OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
		
		plan.SetClearStencilPassBits( false );
		
	}
	
	// copy depth2 to depth1 and color to temporary using stencil mask and shader test to
	// discard not written pixels (depth = 0 or 1 depending on inverse mode). stencil is
	// written with current layer stencil reference value
	defren.ActivateFBOTemporary1( true );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glStencilMask( plan.GetStencilWriteMask() ) );
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE ) );
	
	if( mask ){
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), 1 ) );
		
	}else{
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, plan.GetStencilRefValue(), 0 ) );
	}
	
	deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderCopyDepthColorStereo : pShaderCopyDepthColor;
	renderThread.GetShader().ActivateShader( program );
	shader = program->GetCompiled();
	
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture2(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 1, *defren.GetTextureColor(), GetSamplerClampNearest() );
	
	defren.SetShaderParamFSQuad( *shader, spcdQuadParams );
	defren.RenderFSQuadVAO();
	
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
	defren.ActivateFBOMaterialColor();
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_EQUAL ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( plan.GetFlipCulling() );
	
	if( defren.GetUseFadeOutRange() && false /* alpha blend problem */ ){
		OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
		OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
		
	}else{
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	}
	
	OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glStencilMask( 0 ) );
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
	OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), ~0 ) );
	
	
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
	
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renworld.RenderPB( plan ) );
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid( false );
	addToRenderTask.SetNoRendered( mask );
	addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
	
	addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
		? deoglSkinTexture::estStereoComponentGeometry
		: deoglSkinTexture::estComponentGeometry );
	addToRenderTask.AddComponents( collideList );
	
	addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
		? deoglSkinTexture::estStereoBillboardGeometry
		: deoglSkinTexture::estBillboardGeometry );
	addToRenderTask.AddBillboards( collideList );
	
	addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
		? deoglSkinTexture::estStereoPropFieldGeometry
		: deoglSkinTexture::estPropFieldGeometry );
	addToRenderTask.AddPropFields( collideList, false );
	addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
		? deoglSkinTexture::estStereoPropFieldImposterGeometry
		: deoglSkinTexture::estPropFieldImposterGeometry );
	addToRenderTask.AddPropFields( collideList, true );
	
	if( renderThread.GetChoices().GetRealTransparentParticles() ){
		addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
			? deoglSkinTexture::estStereoParticleGeometry
			: deoglSkinTexture::estParticleGeometry );
		addToRenderTask.SetSkinShaderTypeRibbon( plan.GetRenderStereo()
			? deoglSkinTexture::estStereoParticleRibbonGeometry
			: deoglSkinTexture::estParticleRibbonGeometry );
		addToRenderTask.SetSkinShaderTypeBeam( plan.GetRenderStereo()
			? deoglSkinTexture::estStereoParticleBeamGeometry
			: deoglSkinTexture::estParticleBeamGeometry );
		addToRenderTask.AddParticles( collideList );
	}
	
	renderTask.PrepareForRender();
	DebugTimer2Sample( plan, *renworld.GetDebugInfo().infoTransparentTask, true );
	
	rengeom.RenderTask( renderTask );
	DebugTimer2Sample( plan, *renworld.GetDebugInfo().infoTransparentRender, true );
	
	
	// outline
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renworld.RenderPB( plan ) );
	
	addToRenderTask.Reset();
	addToRenderTask.SetOutline( true );
	addToRenderTask.SetFilterDecal( true );
	addToRenderTask.SetDecal( false );
	addToRenderTask.SetSolid( false );
	addToRenderTask.SetNoRendered( true );
	
	addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
		? deoglSkinTexture::estStereoOutlineGeometry
		: deoglSkinTexture::estOutlineGeometry );
	addToRenderTask.AddComponents( collideList );
	
	renderTask.PrepareForRender();
	DebugTimer2Sample( plan, *renworld.GetDebugInfo().infoTransparentTask, true );
	
	if( renderTask.GetShaderCount() > 0 ){
		SetCullMode( ! plan.GetFlipCulling() );
		rengeom.RenderTask( renderTask );
		SetCullMode( plan.GetFlipCulling() );
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
	
	// if we used wireframe mode disable it now
	if( renderThread.GetConfiguration().GetDebugWireframe() ){
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
DBG_EXIT("RenderTransparentGeometryPass")
}

void deoglRenderTransparentPasses::RenderTransparentLimitDepth(
deoglRenderPlan &plan, const deoglRenderPlanMasked *mask ){
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
	
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE ) );
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	// from here on the solid texture is called the first texture (depth1) and the texture
	// accumulating the depth is called the second texture (depth2). the third texture is
	// used as the working texture. depth textures are swap multiple times to ensure this
	// ordering is correct. this is required for TUCs since they use the second depth texture
	// for depth testing
	
	// clear second texture. this will be accumulate the limit
	defren.SwapDepthTextures(); // solid depth is now depth2
	defren.ActivateFBODepth();
	
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
	const GLfloat clearDepth = defren.GetClearDepthValueReversed();
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
		defren.ActivateFBODepth3();
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		OGL_CHECK( renderThread, glStencilMask( ~0 ) );
		
		OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0,
			defren.GetClearDepthValueRegular(), 0 ) );
		
		// render depth to depth3 using reversed depth compare with depth tested against depth2
		// with withstencil. depth is written. stencil is written with current layer stencil
		// reference value.
		// 
		// we have to swap textures to move the solid depth to depth1 for all but the last layer
		// otherwise it is used for depth testing producing wrong results. the swap is undone
		// right after if applied
		OGL_CHECK( renderThread, glStencilMask( plan.GetStencilWriteMask() ) );
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, plan.GetStencilRefValue(), 0 ) );
		OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
		rendepth.RenderDepth( plan, mask, false, false, true ); // -solid, -maskedOnly, +reverseDepthTest
		
		// copy depth3 to depth2 using stencil mask and shader test to discard not
		// written pixels. stencil is written with current layer stencil reference value
		defren.SwapDepthTextures(); // solid depth is now depth2
		defren.ActivateFBODepth();
		
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) ); // can be modified by RenderDepthPass
		OGL_CHECK( renderThread, glStencilMask( plan.GetStencilWriteMask() ) );
		OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncReversed() ) );
		
		if( mask ){
			OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), 1 ) );
			
		}else{
			OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, plan.GetStencilRefValue(), 0 ) );
		}
		
		OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
		
		deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderCopyDepthLimitStereo : pShaderCopyDepthLimit;
		renderThread.GetShader().ActivateShader( program );
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture3(), GetSamplerClampNearest() );
		defren.SetShaderParamFSQuad( *program->GetCompiled(), spcdQuadParams );
		defren.RenderFSQuadVAO();
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
	if( mask ){
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, prevStencilRefValue, 1 ) );
		
	}else{
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, prevStencilRefValue, 0 ) );
	}
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	rendepth.RenderDepth( plan, mask, false, false, true ); // -solid, -maskedOnly, +reverseDepthTest
	
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
		
		renderTaskParticles.Clear();
		renderTaskParticles.SetRenderParamBlock( renworld.RenderPB( plan ) );
		
		if( inbetween ){
			addToRenderTaskParticles.SetSkinShaderType( plan.GetRenderStereo()
				? deoglSkinTexture::estStereoParticleGeometryDepthTest
				: deoglSkinTexture::estParticleGeometryDepthTest );
			addToRenderTaskParticles.SetSkinShaderTypeRibbon( plan.GetRenderStereo()
				? deoglSkinTexture::estStereoParticleRibbonGeometryDepthTest
				: deoglSkinTexture::estParticleRibbonGeometryDepthTest );
			addToRenderTaskParticles.SetSkinShaderTypeBeam( plan.GetRenderStereo()
				? deoglSkinTexture::estStereoParticleBeamGeometryDepthTest
				: deoglSkinTexture::estParticleBeamGeometryDepthTest );
			
		}else{
			addToRenderTaskParticles.SetSkinShaderType( plan.GetRenderStereo()
				? deoglSkinTexture::estStereoParticleGeometry
				: deoglSkinTexture::estParticleGeometry );
			addToRenderTaskParticles.SetSkinShaderTypeRibbon( plan.GetRenderStereo()
				? deoglSkinTexture::estStereoParticleRibbonGeometry
				: deoglSkinTexture::estParticleRibbonGeometry );
			addToRenderTaskParticles.SetSkinShaderTypeBeam( plan.GetRenderStereo()
				? deoglSkinTexture::estStereoParticleBeamGeometry
				: deoglSkinTexture::estParticleBeamGeometry );
		}
		
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
	
	SetCullMode( plan.GetFlipCulling() );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE/*GL_TRUE*/ ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	
	if( inbetween ){
		OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncReversed() ) );
		
	}else{
		OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	}
	
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA ) );
	
	if( inbetween ){
		OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), ~0 ) );
		
	}else{
		if( mask ){
			OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
			OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
			OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, 1, 1 ) );
			
		}else{
			OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		}
	}
	
	if( renderThread.GetConfiguration().GetDebugWireframe() ){
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	
	renpart.RenderTaskParticles( renderTaskParticles );
//DEBUG_PRINT_TIMER( "RenderWorld: Volumetric Pass Render" );
	
	if( renderThread.GetConfiguration().GetDebugWireframe() ){
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
DBG_EXIT("RenderVolumetricPass")
}



void deoglRenderTransparentPasses::CopyColorToTemporary( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	defren.ActivateFBOTemporary1( false );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	
	deoglShaderProgram *program = plan.GetRenderStereo() ? pShaderCopyColorStereo : pShaderCopyColor;
	renderThread.GetShader().ActivateShader( program );
	deoglShaderCompiled * const shader = program->GetCompiled();
	
	renderThread.GetTexture().GetStages().EnableArrayTexture( 0,
		*defren.GetTextureColor(), GetSamplerClampNearest() );
	
	defren.SetShaderParamFSQuad( *shader, spcdQuadParams );
	defren.RenderFSQuadVAO();
}
