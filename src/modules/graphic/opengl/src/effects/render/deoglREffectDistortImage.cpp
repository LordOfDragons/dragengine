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
#include <stdlib.h>
#include <string.h>

#include "deoglREffectDistortImage.h"
#include "../../debug/deoglDebugTraceGroup.h"
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

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eSPEffect{
	speDistortTransform
};

// Class deoglREffectDistortImage
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglREffectDistortImage::deoglREffectDistortImage( deoglRenderThread &renderThread ) :
deoglREffect( renderThread )
{
	LEAK_CHECK_CREATE( renderThread, EffectDistortImage );
}

deoglREffectDistortImage::~deoglREffectDistortImage(){
	LEAK_CHECK_FREE( GetRenderThread(), EffectDistortImage );
}



// Management
///////////////

void deoglREffectDistortImage::SetStrength( const decVector2 &strength ){
	pStrength = strength;
}

void deoglREffectDistortImage::SetImage( deoglRImage *image ){
	pImage = image;
}



const deoglPipeline *deoglREffectDistortImage::GetPipeline(){
	if( ! pPipeline ){
		deoglPipelineManager &pipelineManager = GetRenderThread().GetPipelineManager();
		deoglPipelineConfiguration pipconf;
		deoglShaderDefines defines;
		
		GetRenderThread().GetShader().SetCommonDefines( defines );
		
		pipconf.SetDepthMask( false );
		pipconf.SetEnableScissorTest( true );
		
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		pipconf.SetShader( GetRenderThread(), "Effect Distort Image", defines );
		pPipeline = pipelineManager.GetWith( pipconf );
	}
	
	return pPipeline;
}

const deoglPipeline *deoglREffectDistortImage::GetPipelineStereo(){
	if( ! pPipelineStereo ){
		deoglPipelineManager &pipelineManager = GetRenderThread().GetPipelineManager();
		deoglPipelineConfiguration pipconf;
		deoglShaderDefines defines;
		
		GetRenderThread().GetShader().SetCommonDefines( defines );
		
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		
		pipconf.SetDepthMask( false );
		pipconf.SetEnableScissorTest( true );
		
		if( GetRenderThread().GetChoices().GetRenderFSQuadStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_STEREO" );
			pipconf.SetShader( GetRenderThread(), "Effect Distort Image", defines );
			
		}else{
			defines.SetDefines( "GS_RENDER_STEREO" );
			pipconf.SetShader( GetRenderThread(), "Effect Distort Image Stereo", defines );
		}
		
		pPipelineStereo = pipelineManager.GetWith( pipconf );
	}
	
	return pPipelineStereo;
}

void deoglREffectDistortImage::PrepareForRender(){
	if( pImage ){
		pImage->PrepareForRender();
	}
}

void deoglREffectDistortImage::Render( deoglRenderPlan &plan ){
	if( ! pImage ){
		return;
	}
	
	deoglTexture * const texture = pImage->GetTexture();
	if( ! texture ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "EffectDistortImage.Render" );
	const deoglRenderWorld &renderWorld = renderThread.GetRenderers().GetWorld();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRTShader &rtshader = renderThread.GetShader();
	
	//int width = defren.GetWidth();
	//int height = defren.GetHeight();
	//int realHeight = defren.GetRealHeight();
	float su = pStrength.x * defren.GetScalingU(); //( float )width;
	float sv = pStrength.y * defren.GetScalingV(); //( float )height;
	
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *GetPipelineStereo() : *GetPipeline();
	pipeline.Activate();
	
	renderWorld.SetViewport( plan );
	
	defren.SwapPostProcessTarget();
	defren.ActivatePostProcessFBO( false );
	tsmgr.EnableArrayTexture( 0, *defren.GetPostProcessTexture(), *rtshader.GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
	tsmgr.EnableTexture( 1, *texture, *rtshader.GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
	
	// set shader program
	// transform image
	// [-1,1] * su/2 + su/2 = [0,su]
	// [-1,1] * sv/2 + sv/2 = [0,sv]
	// transform distort
	// [0,1] * stru*su*2 - stru*su = [-strz*su,stru*su]
	// [0,1] * -strv*sv*2 + strv*sv = [strv*sv,-strv*sv]
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	deoglShaderCompiled &shader = pipeline.GetGlShader();
	shader.SetParameterFloat( speDistortTransform, 2.0f * su, -2.0f * sv, -su, sv );
	
	// TODO correctly we need an explicite clamp in the shader here since the texture
	// can be larger than the area we tap into.
	
	renderWorld.RenderFullScreenQuadVAO( plan.GetRenderStereo()
		&& renderThread.GetChoices().GetRenderFSQuadStereoVSLayer() );
}
