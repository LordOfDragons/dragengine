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

#include "deoglREffectColorMatrix.h"
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
#include "../../texture/deoglTextureStageManager.h"
#include "../../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eSPEffect{
	speColorMatrix,
	speColorOffset
};

// Class deoglREffectColorMatrix
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglREffectColorMatrix::deoglREffectColorMatrix(deoglRenderThread &renderThread) :
deoglREffect(renderThread)
{
	LEAK_CHECK_CREATE(renderThread, EffectColorMatrix);
}

deoglREffectColorMatrix::~deoglREffectColorMatrix(){
	LEAK_CHECK_FREE(GetRenderThread(), EffectColorMatrix);
}



// Management
///////////////

void deoglREffectColorMatrix::SetColorMatrix(const decColorMatrix &colorMatrix){
	pColorMatrix = colorMatrix;
}



const deoglPipeline *deoglREffectColorMatrix::GetPipeline(){
	if(! pPipeline){
		deoglPipelineManager &pipelineManager = GetRenderThread().GetPipelineManager();
		deoglPipelineConfiguration pipconf;
		deoglShaderDefines defines;
		
		GetRenderThread().GetShader().SetCommonDefines(defines);
		
		pipconf.SetDepthMask(false);
		pipconf.SetEnableScissorTest(true);
		
		defines.SetDefines("NO_POSTRANSFORM", "NO_TEXCOORD");
		pipconf.SetShader(GetRenderThread(), "Effect Color Matrix", defines);
		pPipeline = pipelineManager.GetWith(pipconf);
	}
	
	return pPipeline;
}

const deoglPipeline *deoglREffectColorMatrix::GetPipelineStereo(){
	if(! pPipelineStereo){
		deoglPipelineManager &pipelineManager = GetRenderThread().GetPipelineManager();
		deoglPipelineConfiguration pipconf;
		deoglShaderDefines defines;
		
		GetRenderThread().GetShader().SetCommonDefines(defines);
		
		defines.SetDefines("NO_POSTRANSFORM", "NO_TEXCOORD");
		
		pipconf.SetDepthMask(false);
		pipconf.SetEnableScissorTest(true);
		
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if(GetRenderThread().GetChoices().GetRenderFSQuadStereoVSLayer()){
			defines.SetDefines("VS_RENDER_LAYER");
			pipconf.SetShader(GetRenderThread(), "Effect Color Matrix", defines);
			
		}else{
			pipconf.SetShader(GetRenderThread(), "Effect Color Matrix Stereo", defines);
		}
		
		pPipelineStereo = pipelineManager.GetWith(pipconf);
	}
	
	return pPipelineStereo;
}

void deoglREffectColorMatrix::Render(deoglRenderPlan &plan){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace(renderThread, "EffectColorMatrix.Render");
	const deoglRenderWorld &renderWorld = renderThread.GetRenderers().GetWorld();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRTShader &rtshader = renderThread.GetShader();
	decColorMatrix colorMatrix;
	
	// gamma correct
	/*
	// no good. the matrix values can be as well color values which should
	// be gamma corrected but on the other side they can also be real
	// matrix values which should not be gamma corrected. another problem
	// is that negative values are valid but gamma correction on negative
	// values does not work. so right now we do not do gamma correction as
	// we assume the matrix is a value only matrix. a better solution has
	// to be found.
	
	paramMatrix.a11 = powf(colorMatrix.a11, OGL_RENDER_GAMMA);
	paramMatrix.a12 = powf(colorMatrix.a12, OGL_RENDER_GAMMA);
	paramMatrix.a13 = powf(colorMatrix.a13, OGL_RENDER_GAMMA);
	paramOffset.x = powf(colorMatrix.a15, OGL_RENDER_GAMMA);
	
	paramMatrix.a21 = powf(colorMatrix.a21, OGL_RENDER_GAMMA);
	paramMatrix.a22 = powf(colorMatrix.a22, OGL_RENDER_GAMMA);
	paramMatrix.a23 = powf(colorMatrix.a23, OGL_RENDER_GAMMA);
	paramOffset.y = powf(colorMatrix.a25, OGL_RENDER_GAMMA);
	
	paramMatrix.a31 = powf(colorMatrix.a31, OGL_RENDER_GAMMA);
	paramMatrix.a32 = powf(colorMatrix.a32, OGL_RENDER_GAMMA);
	paramMatrix.a33 = powf(colorMatrix.a33, OGL_RENDER_GAMMA);
	paramOffset.z = powf(colorMatrix.a35, OGL_RENDER_GAMMA);
	
	paramMatrix.a41 = powf(colorMatrix.a41, OGL_RENDER_GAMMA);
	paramMatrix.a42 = powf(colorMatrix.a42, OGL_RENDER_GAMMA);
	paramMatrix.a43 = powf(colorMatrix.a43, OGL_RENDER_GAMMA);
	*/
	
	colorMatrix = pColorMatrix;
	
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *GetPipelineStereo() : *GetPipeline();
	pipeline.Activate();
	
	renderWorld.SetViewport(plan);
	
	defren.SwapPostProcessTarget();
	defren.ActivatePostProcessFBO(false);
	tsmgr.EnableArrayTexture(0, *defren.GetPostProcessTexture(),
		*rtshader.GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	deoglShaderCompiled &shader = pipeline.GetShader();
	shader.SetParameterColorMatrix5x4(speColorMatrix, speColorOffset, colorMatrix);
	
	renderWorld.RenderFullScreenQuadVAO(plan.GetRenderStereo()
		&& renderThread.GetChoices().GetRenderFSQuadStereoVSLayer());
}
