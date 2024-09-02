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
#include <stdlib.h>
#include <string.h>

#include "deoglREffectOverlayImage.h"
#include "../../rendering/deoglRenderWorld.h"
#include "../../rendering/defren/deoglDeferredRendering.h"
#include "../../rendering/plan/deoglRenderPlan.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../texture/deoglRImage.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../debug/deoglDebugTraceGroup.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eSPEffect{
	speGamma,
	speColor
};

// Class deoglREffectOverlayImage
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglREffectOverlayImage::deoglREffectOverlayImage( deoglRenderThread &renderThread ) :
deoglREffect( renderThread )
{
	LEAK_CHECK_CREATE( renderThread, EffectOverlayImage );
}

deoglREffectOverlayImage::~deoglREffectOverlayImage(){
	LEAK_CHECK_FREE( GetRenderThread(), EffectOverlayImage );
}



// Management
///////////////

void deoglREffectOverlayImage::SetTransparency( float transparency ){
	pTransparency = decMath::clamp( transparency, 0.0f, 1.0f );
}

void deoglREffectOverlayImage::SetImage( deoglRImage *image ){
	pImage = image;
}



const deoglPipeline *deoglREffectOverlayImage::GetPipeline(){
	if( ! pPipeline ){
		deoglPipelineManager &pipelineManager = GetRenderThread().GetPipelineManager();
		deoglPipelineConfiguration pipconf;
		deoglShaderDefines defines;
		
		GetRenderThread().GetShader().SetCommonDefines( defines );
		
		pipconf.SetDepthMask( false );
		pipconf.SetEnableScissorTest( true );
		pipconf.EnableBlendBlend();
		
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD", "TEXCOORD_FLIP_Y" );
		pipconf.SetShader( GetRenderThread(), "Effect Overlay", defines );
		pPipeline = pipelineManager.GetWith( pipconf );
	}
	
	return pPipeline;
}

const deoglPipeline *deoglREffectOverlayImage::GetPipelineStereo(){
	if( ! pPipelineStereo ){
		deoglPipelineManager &pipelineManager = GetRenderThread().GetPipelineManager();
		deoglPipelineConfiguration pipconf;
		deoglShaderDefines defines;
		
		GetRenderThread().GetShader().SetCommonDefines( defines );
		
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD", "TEXCOORD_FLIP_Y" );
		
		pipconf.SetDepthMask( false );
		pipconf.SetEnableScissorTest( true );
		pipconf.EnableBlendBlend();
		
		if( GetRenderThread().GetChoices().GetRenderFSQuadStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			pipconf.SetShader( GetRenderThread(), "Effect Overlay", defines );
			
		}else{
			defines.SetDefines( "GS_RENDER_STEREO" );
			pipconf.SetShader( GetRenderThread(), "Effect Overlay Stereo", defines );
		}
		
		pPipelineStereo = pipelineManager.GetWith( pipconf );
	}
	
	return pPipelineStereo;
}

void deoglREffectOverlayImage::PrepareForRender(){
	if( pImage ){
		pImage->PrepareForRender();
	}
}

void deoglREffectOverlayImage::Render( deoglRenderPlan &plan ){
	if( ! pImage ){
		return;
	}
	
	deoglTexture * const texture = pImage->GetTexture();
	if( ! texture ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "EffectOverlayImage.Render" );
	const deoglRenderWorld &renderWorld = renderThread.GetRenderers().GetWorld();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRTShader &rtshader = renderThread.GetShader();
	
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *GetPipelineStereo() : *GetPipeline();
	pipeline.Activate();
	
	renderWorld.SetViewport( plan );
	
	defren.ActivatePostProcessFBO( false );
	tsmgr.EnableTexture( 0, *texture, *rtshader.GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
	
	renderWorld.GetRenderPB()->Activate();
	
	// [-1,1] * su/2 + su/2 = [0,su]
	// [-1,1] * sv/2 + sv/2 = [0,sv]
	deoglShaderCompiled &shader = pipeline.GetGlShader();
	shader.SetParameterFloat( speGamma, OGL_RENDER_GAMMA, OGL_RENDER_GAMMA, OGL_RENDER_GAMMA, 1.0 );
	shader.SetParameterFloat( speColor, 1.0f, 1.0f, 1.0f, pTransparency );
	
	renderWorld.RenderFullScreenQuadVAO( plan );
}
