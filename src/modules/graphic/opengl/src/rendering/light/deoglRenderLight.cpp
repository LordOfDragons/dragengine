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
#include "deoglRenderLightSpot.h"
#include "deoglRenderLightSky.h"
#include "deoglRenderLightPoint.h"
#include "deoglRenderLightParticles.h"
#include "deoglRenderGI.h"
#include "../deoglRenderWorld.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglAddToRenderTask.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugSaveTexture.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../gi/deoglGIState.h"
#include "../../gi/deoglGICascade.h"
#include "../../light/deoglRLight.h"
#include "../../light/shader/deoglLightShader.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shadow/deoglShadowMapper.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eSPAOBlur{
	spaobQuadParams,
	spaobOffsets1,
	spaobOffsets2,
	spaobOffsets3,
	spaobOffsets4,
	spaobOffsets5,
	spaobOffsets6,
	spaobOffsets7,
	spaobWeights1,
	spaobWeights2,
	spaobClamp,
	spaobDepthTransform
};

enum pSPDebugAO{
	spdaoPosTransform,
	spdaoTCTransform,
	spdaoColor,
	spdaoGamma,
	spdaoLevel,
	spdaoLayer
};



// Class deoglRenderLight
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderLight::deoglRenderLight( deoglRenderThread &renderThread,
deoglRTRenderers &renderers ) :
deoglRenderLightBase( renderThread ),

pRenderLightSpot( NULL ),
pRenderLightSky( NULL ),
pRenderLightPoint( NULL ),
pRenderLightParticles( NULL ),
pRenderGI( NULL ),

pLightPB( NULL ),
pLightStereoPB( NULL ),
pShadowPB( NULL ),
pShadowCascadedPB( nullptr ),
pShadowCubePB( NULL ),
pOccMapPB( NULL ),
pRenderTask( NULL ),
pAddToRenderTask( NULL )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		pLightPB = deoglLightShader::CreateSPBRender( renderThread );
		pLightStereoPB = deoglLightShader::CreateSPBRenderStereo( renderThread );
		pShadowPB = deoglSkinShader::CreateSPBRender( renderThread );
		pShadowCascadedPB = deoglSkinShader::CreateSPBRenderCascaded( renderThread );
		pShadowCubePB = deoglSkinShader::CreateSPBRenderCubeMap( renderThread );
		pOccMapPB = deoglSkinShader::CreateSPBOccMap( renderThread );
		
		pRenderTask = new deoglRenderTask( renderThread );
		pAddToRenderTask = new deoglAddToRenderTask( renderThread, *pRenderTask );
		
		pRenderLightSky = new deoglRenderLightSky( renderThread );
		pRenderLightSpot = new deoglRenderLightSpot( renderThread, renderers );
		pRenderLightPoint = new deoglRenderLightPoint( renderThread, renderers );
		pRenderLightParticles = new deoglRenderLightParticles( renderThread );
		pRenderGI = new deoglRenderGI( renderThread );
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen ScreenSpace SubSurface Scattering" );
		defines.AddDefines( "FULLSCREENQUAD", "NO_POSTRANSFORM" );
		pShaderSSSSS = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "DefRen ScreenSpace SubSurface Scattering Stereo" );
		defines.AddDefine( "GS_RENDER_STEREO", true );
		pShaderSSSSSStereo = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen AmbientOcclusion Local" );
		defines.AddDefine( "SSAO_RESOLUTION_COUNT", 1 ); // 1-4
		defines.AddDefines( "FULLSCREENQUAD", "NO_POSTRANSFORM" );
		pShaderAOLocal = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "DefRen AmbientOcclusion Local Stereo" );
		defines.AddDefine( "GS_RENDER_STEREO", true );
		pShaderAOLocalStereo = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		sources = shaderManager.GetSourcesNamed( "Gauss Separable Fixed" );
		defines.AddDefine( "TAP_COUNT", 9 );
		defines.AddDefine( "DEPTH_DIFFERENCE_WEIGHTING", true );
		defines.AddDefine( "OUT_DATA_SIZE", 1 );
		defines.AddDefine( "TEX_DATA_SIZE", 1 );
		defines.AddDefine( "TEX_DATA_SWIZZLE", "g" );
		defines.AddDefine( "INPUT_ARRAY_TEXTURES", true );
		pShaderAOBlur1 = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		defines.AddDefine( "TAP_COUNT", 9 );
		defines.AddDefine( "DEPTH_DIFFERENCE_WEIGHTING", true );
		defines.AddDefine( "OUT_DATA_SIZE", 3 );
		defines.AddDefine( "OUT_DATA_SWIZZLE", "g" );
		defines.AddDefine( "TEX_DATA_SIZE", 1 );
		defines.AddDefine( "INPUT_ARRAY_TEXTURES", true );
		pShaderAOBlur2 = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "Debug Display Texture" );
		defines.AddDefine( "TEXTURELEVEL", 1 );
		defines.AddDefine( "OUT_COLOR_SIZE", 3 );
		defines.AddDefine( "TEX_DATA_SWIZZLE", "ggg" );
		pShaderDebugAO = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		// debug information
		const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
		const decColor colorBg( 0.0f, 0.0f, 0.25f, 0.75f );
		const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
		
		pDebugInfoSolid.TakeOver( new deoglDebugInformation( "Lights Solid", colorText, colorBg ) );
		
		pDebugInfoSolidCopyDepth.TakeOver( new deoglDebugInformation( "Copy Depth", colorText, colorBgSub ) );
		pDebugInfoSolid->GetChildren().Add( pDebugInfoSolidCopyDepth );
		
		pDebugInfoSolid->GetChildren().Add( pRenderLightSky->GetDebugInfoSolid() );
		pDebugInfoSolid->GetChildren().Add( pRenderLightPoint->GetDebugInfoSolid() );
		pDebugInfoSolid->GetChildren().Add( pRenderLightSpot->GetDebugInfoSolid() );
		
		pDebugInfoSolidParticle.TakeOver( new deoglDebugInformation( "Particle", colorText, colorBgSub ) );
		pDebugInfoSolid->GetChildren().Add( pDebugInfoSolidParticle );
		
		pDebugInfoSolidSSSSS.TakeOver( new deoglDebugInformation( "SSSSS", colorText, colorBgSub ) );
		pDebugInfoSolid->GetChildren().Add( pDebugInfoSolidSSSSS );
		
		
		
		pDebugInfoTransparent.TakeOver( new deoglDebugInformation( "Lights Transp", colorText, colorBg ) );
		
		pDebugInfoTransparentCopyDepth.TakeOver( new deoglDebugInformation( "Copy Depth", colorText, colorBgSub ) );
		pDebugInfoTransparent->GetChildren().Add( pDebugInfoTransparentCopyDepth );
		
		pDebugInfoTransparent->GetChildren().Add( pRenderLightSky->GetDebugInfoTransparent() );
		pDebugInfoTransparent->GetChildren().Add( pRenderLightPoint->GetDebugInfoTransparent() );
		pDebugInfoTransparent->GetChildren().Add( pRenderLightSpot->GetDebugInfoTransparent() );
		
		pDebugInfoTransparentSSSSS.TakeOver( new deoglDebugInformation( "SSSSS", colorText, colorBgSub ) );
		pDebugInfoTransparent->GetChildren().Add( pDebugInfoTransparentSSSSS );
		
		
		
		DevModeDebugInfoChanged();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderLight::~deoglRenderLight(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderLight::RenderLights( deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const bool sssssEnable = config.GetSSSSSEnable();
	
	if( config.GetQuickDebug() == 20 ){
		return;
	}
	
	DebugTimersReset( plan, false );
	
	// copy the depth. this is only required if we are not using encoded depth
	if( renderThread.GetDebug().GetDeveloperMode().GetDebugEnableLightDepthStencil() ){
		renderThread.GetDeferredRendering().CopyFirstDepthToSecond( true, true );
		
		if( solid ){
			DebugTimer2Sample( plan, *pDebugInfoSolidCopyDepth, true );
			
		}else{
			DebugTimer2Sample( plan, *pDebugInfoTransparentCopyDepth, true );
		}
	}
	
	// render lights
	const bool hasGIStateUpdate = plan.GetUpdateGIState() != NULL;
	const bool hasGIStateRender = plan.GetRenderGIState() != NULL;
	
	if( solid && ! mask && hasGIStateUpdate ){
		pRenderGI->ClearProbes( plan );
	}
	
	RestoreFBO( plan );
	
	if( sssssEnable ){
		deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
		GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		
		defren.ActivateFBOTemporary2( false );
		
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		
		OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	}
	
	PrepareRenderParamBlockLight( plan );
	if( hasGIStateRender ){
		pRenderGI->PrepareUBORenderLight( plan );
	}
	
	pRenderLightPoint->RenderLights( plan, solid, mask );
	pRenderLightSpot->RenderLights( plan, solid, mask );
	
	// sky light requires large render tasks that can be expensive to build. to do this as
	// fast as possible the render task building is done using parallel tasks. by rendering
	// sky light as last light type before GI reduces the time waiting for the parallel
	// tasks to finish
	pRenderLightSky->RenderLights( plan, solid, mask );
	
	if( solid && ! mask && hasGIStateUpdate ){
		if( hasGIStateRender ){
			pRenderGI->RenderLightGIRay( plan );
		}
		pRenderGI->UpdateProbes( plan );
		pRenderGI->MoveProbes( plan );
	}
	if( hasGIStateRender ){
		pRenderGI->RenderLight( plan, solid );
	}
	
	DebugTimer2Reset( plan, false );
	
	if( solid ){
		pRenderLightParticles->RenderLights( plan );
		DebugTimer2Sample( plan, *pDebugInfoSolidParticle, true );
	}
	
	// apply screen space sub surface scattering
	if( sssssEnable ){
		RenderSSSSS( plan, solid );
		if( solid ){
			DebugTimer2Sample( plan, *pDebugInfoSolidSSSSS, true );
			
		}else{
			DebugTimer2Sample( plan, *pDebugInfoTransparentSSSSS, true );
		}
	}
	
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	shadowMapper.DropTextures();
	shadowMapper.DropCubeMaps();
	
	// do not remove this line. without the rendering of upcoming light passes
	// (even across frames) is seriously broken
	renderThread.GetTexture().GetStages().DisableAllStages();
	
	if( solid ){
		DebugTimer1Sample( plan, *pDebugInfoSolid, true );
		
	}else{
		DebugTimer1SampleCount( plan, *pDebugInfoTransparent, 1, true );
	}
}



void deoglRenderLight::RenderAO( deoglRenderPlan &plan, bool solid ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	
	if( ! config.GetSSAOEnable() ){
		return;
	}
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeveloperMode &devmode = renderThread.GetDebug().GetDeveloperMode();
	deoglShaderCompiled *shader;
	
	const int width = defren.GetWidth();
	const int height = defren.GetHeight();
	const float pixelSizeU = defren.GetPixelSizeU();
	const float pixelSizeV = defren.GetPixelSizeV();
	
	
	// render SSAO into green channel
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
	//OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
	//OGL_CHECK( renderThread, glStencilMask( 0 ) );
	//OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
	//OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), ~0 ) );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
	
	defren.ActivateFBOAOSolidity( false );
	
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_TRUE, GL_FALSE, GL_FALSE ) );
	
	deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderAOLocalStereo : pShaderAOLocal;
	renderThread.GetShader().ActivateShader( program );
	
	renderThread.GetRenderers().GetWorld().ActivateRenderPB( plan );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearestMipMap() );
	tsmgr.EnableArrayTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 61 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture(
			*defren.GetTextureAOSolidity(), "ao_local" );
	}
	
	
	
	// gaussian blur with 9 taps (17 pixels size): pass 1
	const float blurOffsets[ 4 ] = { 1.411765f, 3.294118f, 5.176471f, 7.058824f };
	const float blurWeights[ 4 ] = { 2.967529e-1f, 9.442139e-2f, 1.037598e-2f, 2.593994e-4f };
	const float edgeBlurThreshold = config.GetSSAOEdgeBlurThreshold();
	
	defren.ActivateFBOTemporary3();
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	const GLfloat clearColor[ 4 ] = { 1.0f, 1.0f, 1.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	renderThread.GetShader().ActivateShader( pShaderAOBlur1 );
	shader = pShaderAOBlur1->GetCompiled();
	
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureAOSolidity(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 1, *defren.GetDepthTexture1(), GetSamplerClampLinear() );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, width, height ) );
	
	defren.SetShaderParamFSQuad( *shader, spaobQuadParams );
	shader->SetParameterFloat( spaobClamp,
		pixelSizeU * ( ( float )width - 0.5f ), pixelSizeV * ( ( float )height - 0.5f ) );
	shader->SetParameterFloat( spaobDepthTransform,
		plan.GetDepthToPosition().x, plan.GetDepthToPosition().y, edgeBlurThreshold );
	
	shader->SetParameterFloat( spaobWeights1,
		0.1963806f, blurWeights[ 0 ], blurWeights[ 1 ], blurWeights[ 2 ] );
	shader->SetParameterFloat( spaobWeights2, blurWeights[ 3 ], 0.0f, 0.0f, 0.0f );
	
	shader->SetParameterFloat( spaobOffsets1,
		blurOffsets[ 0 ] * pixelSizeU, 0.0f, -blurOffsets[ 0 ] * pixelSizeU, 0.0f );
	shader->SetParameterFloat( spaobOffsets2,
		blurOffsets[ 1 ] * pixelSizeU, 0.0f, -blurOffsets[ 1 ] * pixelSizeU, 0.0f );
	shader->SetParameterFloat( spaobOffsets3,
		blurOffsets[ 2 ] * pixelSizeU, 0.0f, -blurOffsets[ 2 ] * pixelSizeU, 0.0f );
	shader->SetParameterFloat( spaobOffsets4,
		blurOffsets[ 3 ] * pixelSizeU, 0.0f, -blurOffsets[ 3 ] * pixelSizeU, 0.0f );
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 61 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture(
			*defren.GetTextureTemporary3(), "ao_local_blur1" );
	}
	
	
	
	// gaussian blur with 9 taps (17 pixels size): pass 2
	defren.ActivateFBOAOSolidity( false );
	
	renderThread.GetShader().ActivateShader( pShaderAOBlur2 );
	shader = pShaderAOBlur2->GetCompiled();
	
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary3(), GetSamplerClampLinear() );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, width, height ) );
	
	defren.SetShaderParamFSQuad( *shader, spaobQuadParams );
	shader->SetParameterFloat( spaobClamp,
		pixelSizeU * ( ( float )width - 0.5f ), pixelSizeV * ( ( float )height - 0.5f ) );
	shader->SetParameterFloat( spaobDepthTransform,
		plan.GetDepthToPosition().x, plan.GetDepthToPosition().y, edgeBlurThreshold );
	
	shader->SetParameterFloat( spaobWeights1,
		0.1963806f, blurWeights[ 0 ], blurWeights[ 1 ], blurWeights[ 2 ] );
	shader->SetParameterFloat( spaobWeights2, blurWeights[ 3 ], 0.0f, 0.0f, 0.0f );
	
	shader->SetParameterFloat( spaobOffsets1,
		0.0f, blurOffsets[ 0 ] * pixelSizeV, 0.0f, -blurOffsets[ 0 ] * pixelSizeV );
	shader->SetParameterFloat( spaobOffsets2,
		0.0f, blurOffsets[ 1 ] * pixelSizeV, 0.0f, -blurOffsets[ 1 ] * pixelSizeV );
	shader->SetParameterFloat( spaobOffsets3,
		0.0f, blurOffsets[ 2 ] * pixelSizeV, 0.0f, -blurOffsets[ 2 ] * pixelSizeV );
	shader->SetParameterFloat( spaobOffsets4,
		0.0f, blurOffsets[ 3 ] * pixelSizeV, 0.0f, -blurOffsets[ 3 ] * pixelSizeV );
	
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_TRUE, GL_FALSE, GL_FALSE ) );
	
	//OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	//OGL_CHECK( renderThread, pglBlendEquation( GL_MIN ) );
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	//OGL_CHECK( renderThread, pglBlendEquation( GL_FUNC_ADD ) );
	//OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	// debug output
	if( devmode.GetShowSSAO() ){
		renderThread.GetFramebuffer().Activate( devmode.GetFBODebugImageWith( width, height ) );
		
		renderThread.GetShader().ActivateShader( pShaderDebugAO );
		shader = pShaderDebugAO->GetCompiled();
		
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, width, height ) );
		
		shader->SetParameterFloat( spdaoPosTransform, 1.0f, 1.0f, 0.0f, 0.0f );
		defren.SetShaderParamFSQuad( *shader, spdaoTCTransform );
		
		shader->SetParameterFloat( spdaoColor, 1.0f, 1.0f, 1.0f, 1.0f );
		shader->SetParameterFloat( spdaoGamma, 2.2f, 2.2f, 2.2f, 1.0f );
		shader->SetParameterFloat( spdaoLevel, 0.0f );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	}
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 61 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture(
			*defren.GetTextureAOSolidity(), "ao_local_blur2" );
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
}



void deoglRenderLight::RenderSSSSS( deoglRenderPlan &plan, bool solid ){
	deoglRenderThread &renderThread = GetRenderThread();
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
	
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE ) );
	
	//OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
	//OGL_CHECK( renderThread, glStencilMask( 0 ) );
	//OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
	//OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), ~0 ) );
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	defren.ActivateFBOColor( false, false );
	
	deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderSSSSSStereo : pShaderSSSSS;
	renderThread.GetShader().ActivateShader( program );
	
	renderThread.GetRenderers().GetWorld().ActivateRenderPB( plan );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 1122 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureConversion( *defren.GetTextureDiffuse(),
			"sssss_texture1", deoglDebugSaveTexture::ecColorLinear2sRGB );
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureConversion( *defren.GetTextureSubSurface(),
			"sssss_subsurface", deoglDebugSaveTexture::ecNoConversion );
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureConversion( *defren.GetTextureTemporary2(),
			"sssss_temporary2", deoglDebugSaveTexture::ecColorLinearToneMapsRGB );
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 2, *defren.GetTextureSubSurface(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 3, *defren.GetTextureTemporary2(), GetSamplerClampLinear() );
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
}



void deoglRenderLight::PrepareRenderParamBlockLight( deoglRenderPlan &plan ){
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	deoglDeferredRendering &defren = GetRenderThread().GetDeferredRendering();
	
	// regular parameter block
	pLightPB->MapBuffer();
	try{
		pLightPB->SetParameterDataVec4( deoglLightShader::erutDepthToPosition, plan.GetDepthToPosition() );
		pLightPB->SetParameterDataVec2( deoglLightShader::erutDepthToPosition2, plan.GetDepthToPosition2() );
		pLightPB->SetParameterDataVec2( deoglLightShader::erutDepthSampleOffset, plan.GetDepthSampleOffset() );
		
		pLightPB->SetParameterDataVec2( deoglLightShader::erutAOSelfShadow,
			config.GetAOSelfShadowEnable() ? 0.1 : 1.0,
			1.0f / ( DEG2RAD * config.GetAOSelfShadowSmoothAngle() ) );
		
		pLightPB->SetParameterDataVec2( deoglLightShader::erutLumFragCoordScale,
			( float )defren.GetWidth() / ( float )defren.GetTextureLuminance()->GetWidth(),
			( float )defren.GetHeight() / ( float )defren.GetTextureLuminance()->GetHeight() );
		
		// global illumination
		const deoglGIState * const giState = plan.GetRenderGIState();
		if( giState ){
			// ray
			const decDMatrix matrix( decDMatrix::CreateTranslation( giState->GetActiveCascade().GetPosition() )
				* plan.GetCameraMatrix() );
			
			pLightPB->SetParameterDataMat4x3( deoglLightShader::erutGIRayMatrix, matrix );
			pLightPB->SetParameterDataMat3x3( deoglLightShader::erutGIRayMatrixNormal,
				matrix.GetRotationMatrix().QuickInvert() );
			pLightPB->SetParameterDataMat4x4( deoglLightShader::erutGICameraProjection,
				plan.GetProjectionMatrix() );
			
			// general
			pLightPB->SetParameterDataInt( deoglLightShader::erutGIHighestCascade, giState->GetCascadeCount() - 1 );
		}
		
	}catch( const deException & ){
		pLightPB->UnmapBuffer();
		throw;
	}
	pLightPB->UnmapBuffer();
	
	// stereo parameter block
	pLightStereoPB->MapBuffer();
	try{
		pLightStereoPB->SetParameterDataArrayVec4( deoglLightShader::erutDepthToPosition, 0, plan.GetDepthToPosition() );
		pLightStereoPB->SetParameterDataArrayVec2( deoglLightShader::erutDepthToPosition2, 0 , plan.GetDepthToPosition2() );
		
		pLightStereoPB->SetParameterDataArrayVec4( deoglLightShader::erutDepthToPosition, 1, plan.GetDepthToPositionStereo() );
		pLightStereoPB->SetParameterDataArrayVec2( deoglLightShader::erutDepthToPosition2, 1 , plan.GetDepthToPositionStereo2() );
		
		pLightStereoPB->SetParameterDataVec2( deoglLightShader::erutDepthSampleOffset, plan.GetDepthSampleOffset() );
		
		pLightStereoPB->SetParameterDataVec2( deoglLightShader::erutAOSelfShadow,
			config.GetAOSelfShadowEnable() ? 0.1 : 1.0,
			1.0f / ( DEG2RAD * config.GetAOSelfShadowSmoothAngle() ) );
		
		pLightStereoPB->SetParameterDataVec2( deoglLightShader::erutLumFragCoordScale,
			( float )defren.GetWidth() / ( float )defren.GetTextureLuminance()->GetWidth(),
			( float )defren.GetHeight() / ( float )defren.GetTextureLuminance()->GetHeight() );
		
		// global illumination
		const deoglGIState * const giState = plan.GetRenderGIState();
		if( giState ){
			// ray
			const decDMatrix matrix( decDMatrix::CreateTranslation( giState->GetActiveCascade().GetPosition() )
				* plan.GetCameraMatrix() );
			
			pLightStereoPB->SetParameterDataMat4x3( deoglLightShader::erutGIRayMatrix, matrix );
			pLightStereoPB->SetParameterDataMat3x3( deoglLightShader::erutGIRayMatrixNormal,
				matrix.GetRotationMatrix().QuickInvert() );
			pLightStereoPB->SetParameterDataMat4x4( deoglLightShader::erutGICameraProjection,
				plan.GetProjectionMatrix() );
			
			// general
			pLightStereoPB->SetParameterDataInt( deoglLightShader::erutGIHighestCascade, giState->GetCascadeCount() - 1 );
		}
		
	}catch( const deException & ){
		pLightStereoPB->UnmapBuffer();
		throw;
	}
	pLightStereoPB->UnmapBuffer();
}



void deoglRenderLight::ResetDebugInfo(){
	pDebugInfoSolid->Clear();
	pDebugInfoSolidCopyDepth->Clear();
	pDebugInfoSolidParticle->Clear();
	pDebugInfoSolidSSSSS->Clear();
	
	pDebugInfoTransparent->Clear();
	pDebugInfoTransparentCopyDepth->Clear();
	pDebugInfoTransparentSSSSS->Clear();
	
	pRenderLightSky->ResetDebugInfo();
	pRenderLightPoint->ResetDebugInfo();
	pRenderLightSpot->ResetDebugInfo();
	pRenderGI->ResetDebugInfo();
}

void deoglRenderLight::AddTopLevelDebugInfo(){
	deoglDebugInformationList &list = GetRenderThread().GetDebug().GetDebugInformationList();
	
	list.Add( pDebugInfoSolid );
	pRenderLightSky->AddTopLevelDebugInfoSolid();
	pRenderLightPoint->AddTopLevelDebugInfoSolid();
	pRenderLightSpot->AddTopLevelDebugInfoSolid();
	
	list.Add( pDebugInfoTransparent );
	pRenderLightSky->AddTopLevelDebugInfoTransparent();
	pRenderLightPoint->AddTopLevelDebugInfoTransparent();
	pRenderLightSpot->AddTopLevelDebugInfoTransparent();
	
	pRenderGI->AddTopLevelDebugInfo();
}

void deoglRenderLight::DevModeDebugInfoChanged(){
	const int details = GetRenderThread().GetDebug().GetDeveloperMode().GetDebugInfoDetails();
	const bool show = ( details & deoglDeveloperMode::edimLight ) == deoglDeveloperMode::edimLight;
	
	pDebugInfoSolid->SetVisible( show );
	pDebugInfoTransparent->SetVisible( show );
	
	pRenderLightSky->DevModeDebugInfoChanged();
	pRenderLightPoint->DevModeDebugInfoChanged();
	pRenderLightSpot->DevModeDebugInfoChanged();
	pRenderGI->DevModeDebugInfoChanged();
}



// Private Functions
//////////////////////

void deoglRenderLight::pCleanUp(){
	if( pRenderGI ){
		delete pRenderGI;
	}
	if( pRenderLightParticles ){
		delete pRenderLightParticles;
	}
	if( pRenderLightPoint ){
		delete pRenderLightPoint;
	}
	if( pRenderLightSpot ){
		delete pRenderLightSpot;
	}
	if( pRenderLightSky ){
		delete pRenderLightSky;
	}
	
	if( pAddToRenderTask ){
		delete pAddToRenderTask;
	}
	if( pRenderTask ){
		delete pRenderTask;
	}
	if( pOccMapPB ){
		pOccMapPB->FreeReference();
	}
	if( pShadowCubePB ){
		pShadowCubePB->FreeReference();
	}
	if( pShadowCascadedPB ){
		pShadowCascadedPB->FreeReference();
	}
	if( pShadowPB ){
		pShadowPB->FreeReference();
	}
	if( pLightPB ){
		pLightPB->FreeReference();
	}
	if( pLightStereoPB ){
		pLightStereoPB->FreeReference();
	}
	
	deoglDebugInformationList &dilist = GetRenderThread().GetDebug().GetDebugInformationList();
	dilist.RemoveIfPresent( pDebugInfoSolid );
	dilist.RemoveIfPresent( pDebugInfoTransparent );
}
