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
#include "../../debug/deoglDebugTraceGroup.h"
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
#include "../../renderthread/deoglRTChoices.h"
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
	spaobDepthDifferenceThreshold
};

enum pSPDebugAO{
	spdaoPosTransform,
	spdaoTCTransform,
	spdaoColor,
	spdaoGamma,
	spdaoLevel,
	spdaoLayer
};

enum eSPCopyDepth{
	spcdQuadParams
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

pShadowPB( NULL ),
pOccMapPB( NULL ),
pRenderTask( NULL ),
pAddToRenderTask( NULL )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	const bool renderFSQuadStereoVSLayer = renderThread.GetChoices().GetRenderFSQuadStereoVSLayer();
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	deoglShaderSources *sources;
	
	try{
		renderThread.GetShader().SetCommonDefines( commonDefines );
		
		pShadowPB = deoglSkinShader::CreateSPBRender( renderThread );
		pOccMapPB = deoglSkinShader::CreateSPBOccMap( renderThread );
		
		pRenderTask = new deoglRenderTask( renderThread );
		pAddToRenderTask = new deoglAddToRenderTask( renderThread, *pRenderTask );
		
		pRenderLightSky = new deoglRenderLightSky( renderThread );
		pRenderLightSpot = new deoglRenderLightSpot( renderThread, renderers );
		pRenderLightPoint = new deoglRenderLightPoint( renderThread, renderers );
		pRenderLightParticles = new deoglRenderLightParticles( renderThread );
		pRenderGI = new deoglRenderGI( renderThread );
		
		
		
		// sssss
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, false, false );
		pipconf.EnableBlendAdd();
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen ScreenSpace SubSurface Scattering" );
		defines.SetDefines( "FULLSCREENQUAD", "NO_POSTRANSFORM" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineSSSSS = pipelineManager.GetWith( pipconf );
		
		defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen ScreenSpace SubSurface Scattering Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineSSSSSStereo = pipelineManager.GetWith( pipconf );
		
		
		
		// ambient occlusion
		pipconf.Reset();
		pipconf.SetMasks( false, true, false, false, false );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen AmbientOcclusion Local" );
		defines.SetDefine( "SSAO_RESOLUTION_COUNT", 1 ); // 1-4
		defines.SetDefines( "FULLSCREENQUAD", "NO_POSTRANSFORM" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineAOLocal = pipelineManager.GetWith( pipconf );
		
		// ambient occlusion stereo
		defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen AmbientOcclusion Local Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineAOLocalStereo = pipelineManager.GetWith( pipconf );
		
		
		
		// ambient occlusion blur phase 1
		pipconf.SetMasks( true, false, false, false, false );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "Gauss Separable Fixed" );
		defines.SetDefine( "TAP_COUNT", 9 );
		defines.SetDefine( "OUT_DATA_SIZE", 1 );
		defines.SetDefine( "TEX_DATA_SIZE", 1 );
		defines.SetDefine( "TEX_DATA_SWIZZLE", "g" );
		defines.SetDefines( "DEPTH_DIFFERENCE_WEIGHTING", "INPUT_ARRAY_TEXTURES" );
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineAOBlur1 = pipelineManager.GetWith( pipconf );
		
		// ambient occlusion blur phase 1 stereo
		defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "Gauss Separable Fixed Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineAOBlur1Stereo = pipelineManager.GetWith( pipconf );
		
		
		// ambient occlusion blur phase 2
		pipconf.SetMasks( false, true, false, false, false );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "Gauss Separable Fixed" );
		defines.SetDefine( "TAP_COUNT", 9 );
		defines.SetDefine( "OUT_DATA_SIZE", 3 );
		defines.SetDefine( "OUT_DATA_SWIZZLE", "g" );
		defines.SetDefine( "TEX_DATA_SIZE", 1 );
		defines.SetDefines( "DEPTH_DIFFERENCE_WEIGHTING", "INPUT_ARRAY_TEXTURES" );
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineAOBlur2 = pipelineManager.GetWith( pipconf );
		
		// ambient occlusion blur phase 2 stereo
		defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "Gauss Separable Fixed Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineAOBlur2Stereo = pipelineManager.GetWith( pipconf );
		
		
		// ambient occlusion debug
		pipconf.SetMasks( true, true, true, false, false );
		pipconf.SetEnableBlend( false );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "Debug Display Texture" );
		defines.SetDefine( "TEXTURELEVEL", 1 );
		defines.SetDefine( "OUT_COLOR_SIZE", 3 );
		defines.SetDefine( "TEX_DATA_SWIZZLE", "ggg" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineDebugAO = pipelineManager.GetWith( pipconf );
		
		
		
		// copy depth
		pipconf.Reset();
		pipconf.SetMasks( false, false, false, false, true );
		pipconf.EnableDepthTestAlways();
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Copy Depth" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineCopyDepth = pipelineManager.GetWith( pipconf );
		
		// copy depth stereo
		defines.SetDefines( renderFSQuadStereoVSLayer ? "VS_RENDER_STEREO" : "GS_RENDER_STEREO" );
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen Copy Depth Stereo" );
		}
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineCopyDepthStereo = pipelineManager.GetWith( pipconf );
		
		
		
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

void deoglRenderLight::RenderLights( deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask, bool xray ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, solid
		? ( mask ? "Light.RenderLights(Solid,Masked)" : "Light.RenderLights(Solid)" )
		: ( mask ? "Light.RenderLights(Transparent,Masked)" : "Light.RenderLights(Transparent)" ) );
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const bool sssssEnable = config.GetSSSSSEnable();
	
	if( config.GetQuickDebug() == 20 ){
		return;
	}
	
	DebugTimersReset( plan, false );
	
	// copy the depth. this is only required if we are not using encoded depth
	/*
	if( renderThread.GetDebug().GetDeveloperMode().GetDebugEnableLightDepthStencil() ){
		renderThread.GetDeferredRendering().CopyFirstDepthToSecond( true, true );
		
		if( solid ){
			DebugTimer2Sample( plan, *pDebugInfoSolidCopyDepth, true );
			
		}else{
			DebugTimer2Sample( plan, *pDebugInfoTransparentCopyDepth, true );
		}
	}*/
	
	// renderThread.GetDeferredRendering().CopyFirstDepthToSecond( true, false );
	CopyDepth1ToDepth3( plan );
	
	if( solid ){
		DebugTimer2Sample( plan, *pDebugInfoSolidCopyDepth, true );
		
	}else{
		DebugTimer2Sample( plan, *pDebugInfoTransparentCopyDepth, true );
	}
	
	// render lights
	const bool hasGIStateUpdate = plan.GetUpdateGIState() != NULL;
	const bool hasGIStateRender = plan.GetRenderGIState() != NULL;
	
	if( solid && ! mask && ! xray && hasGIStateUpdate ){
		pRenderGI->ClearProbes( plan );
	}
	
	RestoreFBO( plan );
	
	if( sssssEnable ){
		pPipelineClearBuffers->Activate();
		renderThread.GetDeferredRendering().ActivateFBOTemporary2( false );
		
		GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	}
	
	if( hasGIStateRender ){
		pRenderGI->PrepareUBORenderLight( plan );
	}
	
	pRenderLightPoint->RenderLights( plan, solid, mask );
	pRenderLightSpot->RenderLights( plan, solid, mask );
	
	// sky light requires large render tasks that can be expensive to build. to do this as
	// fast as possible the render task building is done using parallel tasks. by rendering
	// sky light as last light type before GI reduces the time waiting for the parallel
	// tasks to finish
	pRenderLightSky->RenderLights( plan, solid, mask, xray );
	
	if( solid && ! mask && hasGIStateUpdate ){
		if( hasGIStateRender ){
			pRenderGI->RenderLightGIRay( plan );
		}
		pRenderGI->UpdateProbes( plan );
		pRenderGI->MoveProbes( plan );
		SetViewport( plan );
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
	const deoglDebugTraceGroup debugTrace( renderThread, solid ? "Light.RenderAO(Solid)" : "Light.RenderAO(Transparent)" );
	const deoglConfiguration &config = renderThread.GetConfiguration();
	
	if( ! config.GetSSAOEnable() || plan.GetDisableLights() ){
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
	const deoglPipeline *pipeline = plan.GetRenderStereo() ? pPipelineAOLocalStereo : pPipelineAOLocal;
	pipeline->Activate();
	
	defren.ActivateFBOAOSolidity( false );
	SetViewport( plan );
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearestMipMap() );
	tsmgr.EnableArrayTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	RenderFullScreenQuad( plan );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 61 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture(
			*defren.GetTextureAOSolidity(), "ao_local" );
	}
	
	
	// gaussian blur with 9 taps (17 pixels size): pass 1
	const float blurOffsets[ 4 ] = { 1.411765f, 3.294118f, 5.176471f, 7.058824f };
	const float blurWeights[ 4 ] = { 2.967529e-1f, 9.442139e-2f, 1.037598e-2f, 2.593994e-4f };
	const float edgeBlurThreshold = config.GetSSAOEdgeBlurThreshold();
	
	pipeline = plan.GetRenderStereo() ? pPipelineAOBlur1Stereo : pPipelineAOBlur1;
	pipeline->Activate();
	
	defren.ActivateFBOTemporary3();
	SetViewport( plan );
	
	const GLfloat clearColor[ 4 ] = { 1.0f, 1.0f, 1.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	shader = pipeline->GetGlShader()->GetCompiled();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureAOSolidity(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 1, *defren.GetDepthTexture1(), GetSamplerClampLinear() );
	
	shader->SetParameterFloat( spaobClamp,
		pixelSizeU * ( ( float )width - 0.5f ), pixelSizeV * ( ( float )height - 0.5f ) );
	shader->SetParameterFloat( spaobDepthDifferenceThreshold, edgeBlurThreshold );
	
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
	
	RenderFullScreenQuad( plan );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 61 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture(
			*defren.GetTextureTemporary3(), "ao_local_blur1" );
	}
	
	
	
	// gaussian blur with 9 taps (17 pixels size): pass 2
	pipeline = plan.GetRenderStereo() ? pPipelineAOBlur2Stereo : pPipelineAOBlur2;
	pipeline->Activate();
	
	defren.ActivateFBOAOSolidity( false );
	SetViewport( plan );
	
	shader = pipeline->GetGlShader()->GetCompiled();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary3(), GetSamplerClampLinear() );
	
	shader->SetParameterFloat( spaobClamp,
		pixelSizeU * ( ( float )width - 0.5f ), pixelSizeV * ( ( float )height - 0.5f ) );
	shader->SetParameterFloat( spaobDepthDifferenceThreshold, edgeBlurThreshold );
	
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
	
	RenderFullScreenQuad( plan );
	
	// debug output
	if( devmode.GetShowSSAO() ){
		pPipelineDebugAO->Activate();
		
		renderThread.GetFramebuffer().Activate( devmode.GetFBODebugImageWith( width, height ) );
		SetViewport( plan );
		
		shader = pPipelineDebugAO->GetGlShader()->GetCompiled();
		
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		
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
	const deoglDebugTraceGroup debugTrace( renderThread, solid ? "Light.RenderSSSSS(Solid)" : "Light.RenderSSSSS(Transparent)" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineSSSSSStereo : *pPipelineSSSSS;
	pipeline.Activate();
	
	defren.ActivateFBOColor( false, false );
	SetViewport( plan );
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
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
	
	RenderFullScreenQuadVAO( plan );
}



void deoglRenderLight::CopyDepth1ToDepth3( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Light.CopyDepth1ToDepth3" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	defren.GetDepthTexture3()->CopyFrom( *defren.GetDepthTexture1(), false );
	
	/*
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineCopyDepthStereo : *pPipelineCopyDepth;
	pipeline.Activate();
	defren.ActivateFBODepth3();
	
	deoglShaderCompiled &shader = *pipeline.GetGlShader()->GetCompiled();
	
	renderThread.GetTexture().GetStages().EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	
	defren.SetShaderParamFSQuad( shader, spcdQuadParams );
	RenderFullScreenQuadVAO( plan );
	*/
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
	if( pShadowPB ){
		pShadowPB->FreeReference();
	}
	
	deoglDebugInformationList &dilist = GetRenderThread().GetDebug().GetDebugInformationList();
	dilist.RemoveIfPresent( pDebugInfoSolid );
	dilist.RemoveIfPresent( pDebugInfoTransparent );
}
