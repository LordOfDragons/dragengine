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

#include "deoglREffectFilterKernel.h"
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
	speTCTransform,
	speOptions,
	speKernel1,
	speKernel2,
	speKernel3
};

enum eSPEffectDownsample{
	spedsTCTransform,
	spedsTCClamp
};

// Class deoglREffectFilterKernel
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglREffectFilterKernel::deoglREffectFilterKernel( deoglRenderThread &renderThread ) :
deoglREffect( renderThread ),
pKernel( NULL ),
pKernelRows( 0 ),
pKernelCols( 0 ),
pScale( 1.0f )
{
	LEAK_CHECK_CREATE( renderThread, EffectFilterKernel );
}

deoglREffectFilterKernel::~deoglREffectFilterKernel(){
	LEAK_CHECK_FREE( GetRenderThread(), EffectFilterKernel );
	if( pKernel ){
		delete [] pKernel;
	}
}



// Management
///////////////

void deoglREffectFilterKernel::SetKernelSize( int rows, int cols ){
	if( rows < 1 || cols < 1 ){
		DETHROW( deeInvalidParam );
	}
	if( ( rows % 2 ) == 0 || ( cols % 2 ) == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pKernel ){
		delete [] pKernel;
	}
	pKernel = new float[ rows * cols ];
	pKernelRows = rows;
	pKernelCols = cols;
}

float deoglREffectFilterKernel::GetKernelValueAt( int row, int col ) const{
	if( row < 0 || row >= pKernelRows ){
		DETHROW( deeInvalidParam );
	}
	if( col < 0 || col >= pKernelCols ){
		DETHROW( deeInvalidParam );
	}
	return pKernel[ row * pKernelCols + col ];
}

void deoglREffectFilterKernel::SetKernelValueAt( int row, int col, float value ){
	if( row < 0 || row >= pKernelRows ){
		DETHROW( deeInvalidParam );
	}
	if( col < 0 || col >= pKernelCols ){
		DETHROW( deeInvalidParam );
	}
	pKernel[ row * pKernelCols + col ] = value;
}

void deoglREffectFilterKernel::SetScale( float scale ){
	pScale = scale;
}



const deoglPipeline *deoglREffectFilterKernel::GetPipeline(){
	if( ! pPipeline ){
		deoglPipelineManager &pipelineManager = GetRenderThread().GetPipelineManager();
		deoglPipelineConfiguration pipconf;
		deoglShaderDefines defines;
		
		GetRenderThread().GetShader().SetCommonDefines( defines );
		
		pipconf.SetDepthMask( false );
		pipconf.SetEnableScissorTest( true );
		
		defines.SetDefines( "NO_POSTRANSFORM" );
		pipconf.SetShader( GetRenderThread(), "Effect Filter Kernel", defines );
		pPipeline = pipelineManager.GetWith( pipconf );
	}
	
	return pPipeline;
}

const deoglPipeline *deoglREffectFilterKernel::GetPipelineStereo(){
	if( ! pPipelineStereo ){
		deoglPipelineManager &pipelineManager = GetRenderThread().GetPipelineManager();
		deoglPipelineConfiguration pipconf;
		deoglShaderDefines defines;
		
		GetRenderThread().GetShader().SetCommonDefines( defines );
		
		defines.SetDefines( "NO_POSTRANSFORM" );
		
		pipconf.SetDepthMask( false );
		pipconf.SetEnableScissorTest( true );
		
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if( GetRenderThread().GetChoices().GetRenderFSQuadStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_LAYER" );
			pipconf.SetShader( GetRenderThread(), "Effect Filter Kernel", defines );
			
		}else{
			pipconf.SetShader( GetRenderThread(), "Effect Filter Kernel Stereo", defines );
		}
		
		pPipelineStereo = pipelineManager.GetWith( pipconf );
	}
	
	return pPipelineStereo;
}

const deoglPipeline *deoglREffectFilterKernel::GetPipelineDownsample(){
	if( ! pPipeline ){
		deoglPipelineManager &pipelineManager = GetRenderThread().GetPipelineManager();
		deoglPipelineConfiguration pipconf;
		deoglShaderDefines defines;
		
		GetRenderThread().GetShader().SetCommonDefines( defines );
		
		pipconf.SetDepthMask( false );
		pipconf.SetEnableScissorTest( true );
		
		defines.SetDefines( "NO_POSTRANSFORM" );
		pipconf.SetShader( GetRenderThread(), "Effect Filter Kernel DownSample", defines );
		pPipelineDownsample = pipelineManager.GetWith( pipconf );
	}
	
	return pPipelineDownsample;
}

const deoglPipeline *deoglREffectFilterKernel::GetPipelineDownsampleStereo(){
	if( ! pPipelineDownsampleStereo ){
		deoglPipelineManager &pipelineManager = GetRenderThread().GetPipelineManager();
		deoglPipelineConfiguration pipconf;
		deoglShaderDefines defines;
		
		GetRenderThread().GetShader().SetCommonDefines( defines );
		
		defines.SetDefines( "NO_POSTRANSFORM" );
		
		pipconf.SetDepthMask( false );
		pipconf.SetEnableScissorTest( true );
		
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if( GetRenderThread().GetChoices().GetRenderFSQuadStereoVSLayer() ){
			defines.SetDefines( "VS_RENDER_LAYER" );
			pipconf.SetShader( GetRenderThread(), "Effect Filter Kernel DownSample", defines );
			
		}else{
			pipconf.SetShader( GetRenderThread(), "Effect Filter Kernel DownSample Stereo", defines );
		}
		
		pPipelineDownsampleStereo = pipelineManager.GetWith( pipconf );
	}
	
	return pPipelineDownsampleStereo;
}

void deoglREffectFilterKernel::Render( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "EffectFilterKernel.Render" );
	const deoglRenderWorld &renderWorld = renderThread.GetRenderers().GetWorld();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRTShader &rtshader = renderThread.GetShader();
	
	if( pKernelRows != 3 || pKernelCols != 3 ){
		return;
	}
	
	// source texture in case downsampling is required
	defren.SwapPostProcessTarget();
	deoglArrayTexture *sourceTexture = defren.GetPostProcessTexture();
	
	// down sample if required
	int height = defren.GetHeight();
	int width = defren.GetWidth();
	int downsampleCount = 0;
	float downsampleSize = pScale;
	while( downsampleSize > 1.5f ){
		downsampleCount++;
		downsampleSize *= 0.5f;
	}
	
	if( downsampleCount > 0 ){
		int i;
		
		const deoglPipeline &pipeline = plan.GetRenderStereo() ? *GetPipelineDownsampleStereo() : *GetPipelineDownsample();
		renderWorld.GetRenderPB()->Activate();
		
		deoglShaderCompiled &shaderDownsample = pipeline.GetShader();
		defren.SetShaderParamFSQuad( shaderDownsample, spedsTCTransform );
		
		for( i=0; i<downsampleCount; i++ ){
			tsmgr.EnableArrayTexture( 0, *sourceTexture, *rtshader.GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
			
			if( ( i % 2 ) == 0 ){
				defren.ActivateFBOReflectivity( false );
				sourceTexture = defren.GetTextureReflectivity();
				
			}else{
				defren.ActivateFBOTemporary1( false );
				sourceTexture = defren.GetTextureTemporary1();
			}
			
			shaderDownsample.SetParameterInt( spedsTCClamp, width - 1, height - 1 );
			
			if( width % 2 > 0 ){
				width = ( width >> 1 ) + 1;
				
			}else{
				width >>= 1;
			}
			if( width < 1 ){
				width = 1;
			}
			
			if( height % 2 > 0 ){
				height = ( height >> 1 ) + 1;
				
			}else{
				height >>= 1;
			}
			if( height < 1 ){
				height = 1;
			}
			
			renderWorld.SetViewport( width, height );
			renderWorld.RenderFullScreenQuadVAO( plan.GetRenderStereo()
				&& renderThread.GetChoices().GetRenderFSQuadStereoVSLayer() );
		}
	}
	
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *GetPipelineStereo() : *GetPipeline();
	pipeline.Activate();
	
	renderWorld.SetViewport( plan );
	
	defren.ActivatePostProcessFBO( false );
	tsmgr.EnableArrayTexture( 0, *sourceTexture, *rtshader.GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
	
	deoglShaderCompiled &shader = pipeline.GetShader();
	renderWorld.GetRenderPB()->Activate();
	
	//defren.SetShaderParamFSQuad( shader, speQuadParams );
	defren.SetShaderParamFSQuad( shader, speTCTransform, width, height );
	shader.SetParameterFloat( speOptions, ( float )pKernelRows, ( float )pKernelCols,
		defren.GetPixelSizeU() * downsampleSize/*pScale*/, defren.GetPixelSizeV() * downsampleSize/*pScale*/ );
	shader.SetParameterFloat( speKernel1, GetKernelValueAt( 0, 0 ), GetKernelValueAt( 0, 1 ), GetKernelValueAt( 0, 2 ) );
	shader.SetParameterFloat( speKernel2, GetKernelValueAt( 1, 0 ), GetKernelValueAt( 1, 1 ), GetKernelValueAt( 1, 2 ) );
	shader.SetParameterFloat( speKernel3, GetKernelValueAt( 2, 0 ), GetKernelValueAt( 2, 1 ), GetKernelValueAt( 2, 2 ) );
	
	renderWorld.RenderFullScreenQuadVAO( plan.GetRenderStereo()
		&& renderThread.GetChoices().GetRenderFSQuadStereoVSLayer() );
}
