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

#include <stdio.h>
#include <stdlib.h>

#include "deoglRenderToneMap.h"
#include "deoglRenderWorld.h"
#include "defren/deoglDeferredRendering.h"
#include "light/deoglRenderLight.h"
#include "plan/deoglRenderPlan.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTChoices.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../sky/deoglRSkyInstance.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../vao/deoglVAO.h"
#include "../world/deoglRCamera.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>



//#define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer timerTotal;
static decTimer timer;

#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
#define DEBUG_PRINT_TIMER(what)			renderThread.GetLogger().LogInfoFormat( "Timer: %s = %iys", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) )
#define DEBUG_PRINT_TIMER_TOTAL(what)	renderThread.GetLogger().LogInfoFormat( "Timer-Total %s = %iys", what, ( int )( timerTotal.GetElapsedTime() * 1000000.0 ) )
#else
#define DEBUG_RESET_TIMERS
#define DEBUG_PRINT_TIMER(what)
#define DEBUG_PRINT_TIMER_TOTAL(what)
#endif



// Definitions
////////////////

/*
== Notes ==
===========

Tone mapping is done using a modified Reinhard operator. The parameters are
stored in a 1x1 texture which is exchanged on each run with the current
texture stored in the camera.

To calculate the scene key the color texture is first sampled using a color
two logarithmic luminance shader and then down sampled to a 2x2 texture.
To avoid creating additional textures the diffuse and normal texture are
used in ping-pong mode. The result is stored in the 1x1 parameter texture.
The diffuse and normal texture are not used anymore.

For blooming the color texture is first processed using a bright pass shader.
Here two the diffuse and specularity texture are used for ping-pong down
sampling the filtered image. The bloomed result is stored in the normal
texture. This texture has to stay untouched until the final tone map shader
has run.

The tone mapping shader evnetually renders the tone mapped image using the
1x1 parameter texture, the original color texture and the bloom texture.
The result is stored in the diffuse texture as required by the upcoming
stages of the render code.

== Texture Assignment ==
diffuse: final tone mapped image ( blocked after tone map pass )
normal: bloom texture ( blocked after the bloom pass )

*/



// Definitions
////////////////

enum eSPColor2LogLum{
	spc2llParam1,
	spc2llParam2
};

enum eSPAverageLogLum{
	spallTCTransform,
	spallOffsets
};

enum eSPParameters{
	sppAvgLogLumTCs
};

enum eSPBloomReduce{
	spbrParam1,
	spbrParam2
};

enum eSPBloomBlur{
	spbbTCTransform,
	spbbOffsets1,
	spbbOffsets2,
	spbbOffsets3,
	spbbOffsets4,
	spbbOffsets5,
	spbbWeights1,
	spbbWeights2,
	spbbClamp,
	spbbLevel,
	spbbScaleColor
};

enum eSPBloomAdd{
	spbaTCTransform,
	spbaLevel
};

enum eSPToneMap{
	sptmTCBloomTransform,
	sptmTCBloomClamp
};

enum eSPFinalize{
	spfinTCTransform,
	spfinGamma,
	spfinBrightness,
	spfinContrast
};

enum eSPBloomDownSample{
	spmdsLevel
};



// Class deoglRenderToneMap
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderToneMap::deoglRenderToneMap( deoglRenderThread &renderThread ) : deoglRenderBase( renderThread ){
	const bool renderFSQuadStereoVSLayer = renderThread.GetChoices().GetRenderFSQuadStereoVSLayer();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	const deoglShaderSources *sources;
	
	pTextureToneMapParams = NULL;
	
	try{
		renderThread.GetShader().SetCommonDefines( commonDefines );
		
		
		pipconf.Reset();
		pipconf.SetMasks( true, false, false, false, false );
		pipconf.SetEnableScissorTest( true );
		
		// color to loglum
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		sources = shaderManager.GetSourcesNamed( "ToneMap Color2LogLum" );
		pAsyncGetPipeline(pPipelineColor2LogLum, pipconf, sources, defines);
		
		// color to loglum stereo
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if(renderFSQuadStereoVSLayer){
			defines.SetDefines("VS_RENDER_LAYER");
		}
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "ToneMap Color2LogLum Stereo" );
		}
		pAsyncGetPipeline(pPipelineColor2LogLumStereo, pipconf, sources, defines);
		
		
		// average loglum
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM" );
		sources = shaderManager.GetSourcesNamed( "ToneMap Average LogLum" );
		pAsyncGetPipeline(pPipelineAvgLogLum, pipconf, sources, defines);
		
		// average loglum stereo
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if(renderFSQuadStereoVSLayer){
			defines.SetDefines("VS_RENDER_LAYER");
		}
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "ToneMap Average LogLum Stereo" );
		}
		pAsyncGetPipeline(pPipelineAvgLogLumStereo, pipconf, sources, defines);
		
		
		// parameters
		pipconf.SetMasks(true, true, true, true, false);
		
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM", "NO_TEXCOORD" );
		sources = shaderManager.GetSourcesNamed( "ToneMap Parameters" );
		pAsyncGetPipeline(pPipelineParameters, pipconf, sources, defines);
		
		defines.SetDefines( "SAMPLE_STEREO" );
		pAsyncGetPipeline(pPipelineParametersStereo, pipconf, sources, defines);
		
		
		// bright pass
		pipconf.SetMasks(true, true, true, false, false);
		pipconf.SetEnableBlend(false);
		
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		sources = shaderManager.GetSourcesNamed( "ToneMap Bright-Pass" );
		pAsyncGetPipeline(pPipelineBrightPass, pipconf, sources, defines);
		
		
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if(renderFSQuadStereoVSLayer){
			defines.SetDefines("VS_RENDER_LAYER");
		}
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "ToneMap Bright-Pass Stereo" );
		}
		pAsyncGetPipeline(pPipelineBrightPassStereo, pipconf, sources, defines);
		
		
		// bloom downsample
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "ToneMap Bloom Down-Sample" );
		pAsyncGetPipeline(pPipelineBloomDownSample, pipconf, sources, defines);
		
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if(renderFSQuadStereoVSLayer){
			defines.SetDefines("VS_RENDER_LAYER");
		}
		if( ! renderThread.GetChoices().GetRenderFSQuadStereoVSLayer() ){
			sources = shaderManager.GetSourcesNamed( "ToneMap Bloom Down-Sample Stereo" );
		}
		pAsyncGetPipeline(pPipelineBloomDownSampleStereo, pipconf, sources, defines);
		
		
		// bloom blur
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM" );
		sources = shaderManager.GetSourcesNamed( "ToneMap Bloom Blur" );
		pAsyncGetPipeline(pPipelineBloomBlur, pipconf, sources, defines);
		
		// bloom blur stereo
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if(renderFSQuadStereoVSLayer){
			defines.SetDefines("VS_RENDER_LAYER");
		}
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "ToneMap Bloom Blur Stereo" );
		}
		pAsyncGetPipeline(pPipelineBloomBlurStereo, pipconf, sources, defines);
		
		
		// bloom add
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM" );
		sources = shaderManager.GetSourcesNamed( "ToneMap Bloom Add" );
		pipconf.EnableBlend( GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR );
		//pipconf.SetBlendColor( decColor( 0.5f, 0.5f, 0.5f, 0.5f ) );
		pipconf.SetBlendColor( decColor( 0.75f, 0.75f, 0.75f, 0.75f ) );
		pAsyncGetPipeline(pPipelineBloomAdd, pipconf, sources, defines);
		
		// bloom add stereo
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if(renderFSQuadStereoVSLayer){
			defines.SetDefines("VS_RENDER_LAYER");
		}
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "ToneMap Bloom Add Stereo" );
		}
		pAsyncGetPipeline(pPipelineBloomAddStereo, pipconf, sources, defines);
		
		
		// tone map
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM", "NO_TCTRANSFORM" );
		sources = shaderManager.GetSourcesNamed( "ToneMap Tone Mapping" );
		pipconf.SetMasks(true, true, true, true, false);
		pipconf.SetEnableBlend(false);
		pAsyncGetPipeline(pPipelineToneMap, pipconf, sources, defines);
		
		defines.SetDefines( "WITH_TONEMAP_CURVE" );
		pAsyncGetPipeline(pPipelineToneMapCustom, pipconf, sources, defines);
		defines.RemoveDefines( "WITH_TONEMAP_CURVE" );
		
		// tone map stereo
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if(renderFSQuadStereoVSLayer){
			defines.SetDefines("VS_RENDER_LAYER");
		}
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "ToneMap Tone Mapping Stereo" );
		}
		pAsyncGetPipeline(pPipelineToneMapStereo, pipconf, sources, defines);
		
		defines.SetDefines( "WITH_TONEMAP_CURVE" );
		pAsyncGetPipeline(pPipelineToneMapCustomStereo, pipconf, sources, defines);
		defines.RemoveDefines( "WITH_TONEMAP_CURVE" );
		
		
		// ldr
		pipconf.Reset();
		pipconf.SetMasks(true, true, true, true, false);
		pipconf.SetEnableScissorTest(true);
		
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM" );
		sources = shaderManager.GetSourcesNamed( "DefRen Finalize" );
		pAsyncGetPipeline(pPipelineLdr, pipconf, sources, defines);
		
		// ldr stereo
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if(renderFSQuadStereoVSLayer){
			defines.SetDefines("VS_RENDER_LAYER");
		}
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "DefRen Finalize Stereo" );
		}
		pAsyncGetPipeline(pPipelineLdrStereo, pipconf, sources, defines);
		
		
		// lum prepare
		pipconf.Reset();
		pipconf.SetMasks( true, false, false, false, false );
		pipconf.SetEnableScissorTest( true );
		
		defines = commonDefines;
		defines.SetDefines( "NO_POSTRANSFORM", "NO_TEXCOORD" );
		sources = shaderManager.GetSourcesNamed( "ToneMap Luminance Prepare" );
		pAsyncGetPipeline(pPipelineLumPrepare, pipconf, sources, defines);
		
		// lum prepare stereo
		defines.SetDefine("LAYERED_RENDERING", deoglSkinShaderConfig::elrmStereo);
		if(renderFSQuadStereoVSLayer){
			defines.SetDefines("VS_RENDER_LAYER");
		}
		if( ! renderFSQuadStereoVSLayer ){
			sources = shaderManager.GetSourcesNamed( "ToneMap Luminance Prepare Stereo" );
		}
		pAsyncGetPipeline(pPipelineLumPrepareStereo, pipconf, sources, defines);
		
		
		pFBOToneMapParams.TakeOverWith(renderThread, false);
		
		pTextureToneMapParams = new deoglTexture( renderThread );
		pTextureToneMapParams->SetSize( 1, 1 );
		pTextureToneMapParams->SetFBOFormat( 4, true );
		pTextureToneMapParams->CreateTexture();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderToneMap::~deoglRenderToneMap(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderToneMap::LuminancePrepare( deoglRenderPlan &plan ){
DEBUG_RESET_TIMERS;
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "ToneMap.LuminancePrepare" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	defren.ActivateFBOLuminance();
	
	SetViewport( plan );
	renderThread.GetTexture().GetStages().EnableArrayTexture(
		0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	
	( plan.GetRenderStereo() ? pPipelineLumPrepareStereo : pPipelineLumPrepare )->Activate();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	RenderFullScreenQuadVAO( plan );
DEBUG_PRINT_TIMER_TOTAL( "LuminancePrepare" );
}

void deoglRenderToneMap::ToneMap( deoglRenderPlan &plan ){
DEBUG_RESET_TIMERS;
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "ToneMap.ToneMap" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	if( plan.GetUseToneMap() && renderThread.GetConfiguration().GetUseHDRR() && plan.GetCamera() ){
		int bloomWidth = 0;
		int bloomHeight = 0;
		
		CalculateSceneKey( plan );
		RenderBloomPass( plan, bloomWidth, bloomHeight );
		RenderToneMappingPass( plan, bloomWidth, bloomHeight );
		
	}else{
		RenderLDR( plan );
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
DEBUG_PRINT_TIMER_TOTAL( "Tone-Mapping" );
}

void deoglRenderToneMap::CalculateSceneKey( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "ToneMap.CalculateSceneKey" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
//	const float pixelSizeS = defren.GetPixelSizeU();
//	const float pixelSizeT = defren.GetPixelSizeV();
	deoglShaderCompiled *shader;
	bool modeTarget;
	
	// convert color to log luminance. to allow for proper averaging the output image is reduced
	// to the largest power of four size fitting inside for upcoming downsampling. if the found
	// dimensions are less than factor two of the real size the dimension is halved. this is
	// required to ensure that there is enough texture space left for texture barrier driven
	// down sampling
	const float pixelSizeU = defren.GetPixelSizeU();
	const float pixelSizeV = defren.GetPixelSizeV();
	int viewportPingPongOffset, tcPingPongOffset;
	float tcOffsetU, tcOffsetV, clampU, clampV;
	const int realHeight = defren.GetHeight();
	const int realWidth = defren.GetWidth();
	bool useTextureBarrier = pglTextureBarrier && false;
	int lastWidth, lastHeight;
	int curWidth, curHeight;
	int pingPongOffset;
	
	for( curWidth=4; (curWidth<<2)<realWidth; curWidth<<=2 );
	for( curHeight=4; (curHeight<<2)<realHeight; curHeight<<=2 );
	
	if( curWidth * 2 > realWidth ){
		curWidth >>= 1;
	}
	if( curHeight * 2 > realHeight ){
		curHeight >>= 1;
	}
	
	if( useTextureBarrier ){
		pingPongOffset = curWidth + 32;
		
	}else{
		pingPongOffset = 0;
		//pingPongOffset = curWidth + 32;
	}
	
	tcOffsetU = pixelSizeU * ( ( ( float )realWidth / ( float )curWidth ) * 0.5f );
	tcOffsetV = pixelSizeV * ( ( ( float )realHeight / ( float )curHeight ) * 0.5f );
	clampU = pixelSizeU * ( ( float )realWidth - 0.5f );
	clampV = pixelSizeV * ( ( float )realHeight - 0.5f );
	
	defren.ActivateFBOTemporary1( false );
	OGL_CHECK( renderThread, glViewport( 0, 0, curWidth, curHeight ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, curWidth, curHeight ) );
// 	tsmgr.EnableTexture( 0, *defren.GetTextureColor(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureLuminance(), GetSamplerClampNearest() );
	
	const deoglPipeline *pipeline = plan.GetRenderStereo() ? pPipelineColor2LogLumStereo : pPipelineColor2LogLum;
	pipeline->Activate();
	shader = &pipeline->GetShader();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	shader->SetParameterFloat( spc2llParam1, tcOffsetU, 0.0f, 0.0f, tcOffsetV );
	shader->SetParameterFloat( spc2llParam2, tcOffsetU, tcOffsetV, clampU, clampV );
	
	RenderFullScreenQuad( plan );
DEBUG_PRINT_TIMER( "ToneMap: LogLum" );
	
	// average the log luminances
	pipeline = plan.GetRenderStereo() ? pPipelineAvgLogLumStereo : pPipelineAvgLogLum;
	pipeline->Activate();
	shader = &pipeline->GetShader();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	modeTarget = false;
	
	if( useTextureBarrier ){
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary1(), GetSamplerClampLinear() );
	}
	
	while( curWidth > 4 || curHeight > 4 ){
		lastWidth = curWidth;
		lastHeight = curHeight;
		
		if( curWidth > 4 ){
			curWidth >>= 2;
			tcOffsetU = pixelSizeU;
			
		}else{
			tcOffsetU = 0.0f;
		}
		
		if( curHeight > 4 ){
			curHeight >>= 2;
			tcOffsetV = pixelSizeV;
			
		}else{
			tcOffsetV = 0.0f;
		}
		
		if( modeTarget ){
			viewportPingPongOffset = 0;
			tcPingPongOffset = pingPongOffset;
			
		}else{
			viewportPingPongOffset = pingPongOffset;
			tcPingPongOffset = 0;
		}
		
		OGL_CHECK( renderThread, glViewport( viewportPingPongOffset, 0, curWidth, curHeight ) );
		OGL_CHECK( renderThread, glScissor( viewportPingPongOffset, 0, curWidth, curHeight ) );
		
		if( useTextureBarrier ){
			pglTextureBarrier();
			
		}else{
			if( modeTarget ){
				defren.ActivateFBOTemporary1( false );
				tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary2(), GetSamplerClampLinear() );
				
			}else{
				defren.ActivateFBOTemporary2( false );
				tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary1(), GetSamplerClampLinear() );
			}
		}
		
		defren.SetShaderParamFSQuad( *shader, spallTCTransform, ( float )tcPingPongOffset, 0.0f,
			( float )( tcPingPongOffset + lastWidth ), ( float )lastHeight );
		shader->SetParameterFloat( spallOffsets, -tcOffsetU, tcOffsetU, -tcOffsetV, tcOffsetV );
		
		RenderFullScreenQuad( plan );
		
		modeTarget = ! modeTarget;
	}
DEBUG_PRINT_TIMER( "ToneMap: Average" );
	
	// determine tone map parameters to use for this scene
	renderThread.GetFramebuffer().Activate(pFBOToneMapParams);
	
	deoglRCamera &oglCamera = *plan.GetCamera();
	deoglTexture * const lastParams = oglCamera.GetToneMapParamsTexture();
	
	oglCamera.SetForceToneMapAdaption( false );
	
	pipeline = plan.GetRenderStereo() ? pPipelineParametersStereo : pPipelineParameters;
	pipeline->Activate();
	shader = &pipeline->GetShader();
	
	// WARNING we have to use the non-stereo version always even if we sample stereo. the reason
	//         is that to use the stereo render param block we have to use the LAYERED_RENDERING
	//         shader define to get the right block layout. this in turn though would cause
	//         the vertex shader to use a geometry shader which this shader is not using.
	//         to avoid problems the LAYERED_RENDERING is not used, which requires us to always
	//         use the non-stereo block. instead a special define SAMPLE_STEREO is used
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	if( modeTarget ){
		viewportPingPongOffset = 0;
		tcPingPongOffset = pingPongOffset;
		
	}else{
		viewportPingPongOffset = pingPongOffset;
		tcPingPongOffset = 0;
	}
	
	if( curWidth == 4 ){
		tcOffsetU = pixelSizeU * ( ( float )tcPingPongOffset + 3.0f );
		
	}else{ // curWidth == 2
		tcOffsetU = pixelSizeU * ( ( float )tcPingPongOffset + 1.0f );
	}
	if( curHeight == 4 ){
		tcOffsetV = pixelSizeV * 3.0f;
		
	}else{ // curHeight == 2
		tcOffsetV = pixelSizeV * 1.0f;
	}
	
	if( useTextureBarrier ){
		pglTextureBarrier();
	}
	
	shader->SetParameterFloat( sppAvgLogLumTCs, pixelSizeU * ( ( float )tcPingPongOffset + 1.0f ), pixelSizeV, tcOffsetU, tcOffsetV );
	
	oglCamera.ResetElapsedToneMapAdaption();
	
	pFBOToneMapParams->DetachAllImages();
	pFBOToneMapParams->AttachColorTexture(0, pTextureToneMapParams);
	const GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
	OGL_CHECK(renderThread, pglDrawBuffers(1, buffers));
	OGL_CHECK(renderThread, glReadBuffer(GL_COLOR_ATTACHMENT0));
	pFBOToneMapParams->Verify();
	
	OGL_CHECK( renderThread, glViewport( 0, 0, 1, 1 ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, 1, 1 ) );
	
	if( modeTarget && ! useTextureBarrier ){
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary2(), GetSamplerClampLinear() );
		
	}else{
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary1(), GetSamplerClampLinear() );
	}
	tsmgr.EnableTexture( 1, *lastParams, GetSamplerClampNearest() );
	if( plan.GetWorld()->GetSkyEnvironmentMap() && plan.GetWorld()->GetSkyEnvironmentMap()->GetEnvironmentMap() ){
		tsmgr.EnableCubeMap( 2, *plan.GetWorld()->GetSkyEnvironmentMap()->GetEnvironmentMap(), GetSamplerClampLinear() );
	}else{
		tsmgr.EnableCubeMap( 2, *renderThread.GetDefaultTextures().GetEnvMap(), GetSamplerClampLinear() );
	}
	
	RenderFullScreenQuad();
	
	oglCamera.SetToneMapParamsTexture( pTextureToneMapParams );
	pTextureToneMapParams = lastParams;
DEBUG_PRINT_TIMER( "ToneMap: Determine Parameters" );
}

void deoglRenderToneMap::RenderBloomPass( deoglRenderPlan &plan, int &bloomWidth, int &bloomHeight ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "ToneMap.RenderBloomPass" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglRCamera &oglCamera = *plan.GetCamera();
	const float pixelSizeU = defren.GetPixelSizeU();
	const float pixelSizeV = defren.GetPixelSizeV();
	const int realWidth = defren.GetWidth();
	const int realHeight = defren.GetHeight();
	deoglShaderCompiled *shader;
	int curWidth, curHeight;
	
	// convert color to bright values. to allow for proper bluring the output image is reduced
	// to the largest power of two size fitting inside
	for( curWidth=1; (curWidth<<1)<realWidth; curWidth<<= 1 );
	for( curHeight=1; (curHeight<<1)<realHeight; curHeight<<=1 );
	
	const int referenceSize = decMath::max( curWidth, curHeight );
	
	const deoglPipeline *pipeline = plan.GetRenderStereo() ? pPipelineBrightPassStereo : pPipelineBrightPass;
	pipeline->Activate();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	defren.ActivateFBOTemporary1( false );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, curWidth, curHeight ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, curWidth, curHeight ) );
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureColor(), GetSamplerClampLinear() );
	tsmgr.EnableTexture( 1, *oglCamera.GetToneMapParamsTexture(), GetSamplerClampNearest() );
	
	RenderFullScreenQuad( plan );
	
	
	// determine the number of blur passes. tries to finds the level where the required pixel
	// size (blendSize * mipMapWidth) is at most 21 (pixel size of gauss kernel)
	const int brightWidth = curWidth;
	const int brightHeight = curHeight;
	const float bloomSize = oglCamera.GetBloomSize();
	
	const int realMipMapCount = defren.GetTextureTemporary1()->GetRealMipMapLevelCount();
	int i;
	
	/*
	const int blurStep = 31; // 21 + 2 (2 because of downsample)
	float blurLevelScale = 1.0f;
	int blurSize = 0, topMipMapLevel = 0;
	
	for( i=0; i<realMipMapCount; i++ ){
		const int requiredBlurSize = ( int )( bloomSize * ( float )curWidth );
		
		blurSize += blurStep;
		curWidth /= 2;
		
		if( blurSize >= requiredBlurSize ){
			blurLevelScale = ( float )requiredBlurSize / ( float )blurStep;
			topMipMapLevel = i;
			break;
		}
	}
	*/
	
	const float level = decMath::max( log2f( ( float )referenceSize )
		- log2f( 21.0f / decMath::max( bloomSize, 0.01f ) ), 0.0f );
	
	const int topMipMapLevel = ( int )ceil( level );
	
	const int buildMipMapLevelCount = decMath::min( topMipMapLevel + 1, realMipMapCount );
	
	
	// create mipmap levels
	{
	const deoglDebugTraceGroup debugTrace2( renderThread, "DownSample" );
	pipeline = plan.GetRenderStereo() ? pPipelineBloomDownSampleStereo : pPipelineBloomDownSample;
	pipeline->Activate();
	
	shader = &pipeline->GetShader();
	
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary1(), GetSamplerClampNearest() );
	
	bloomWidth = brightWidth;
	bloomHeight = brightHeight;
	
	for( i=1; i<buildMipMapLevelCount; i++ ){
		bloomWidth >>= 1;
		bloomHeight >>= 1;
		
		defren.ActivateFBOTemporary1Level( i );
		OGL_CHECK( renderThread, glViewport( 0, 0, bloomWidth, bloomHeight ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, bloomWidth, bloomHeight ) );
		
		shader->SetParameterInt( spmdsLevel, i - 1 );
		
		RenderFullScreenQuad( plan );
	}
	}
	
	
	// NOTE applying gauss blur multiple time is not working
	// NOTE on 1920 screen a 25% blur requires 480 pixels
	// NOTE for something like depth-of-field requiring variable blurring the mip-map
	//      level has to be chosen at shader time
	
	// apply a blur filter according to 'tonemapping' in the 'doc' directory
	const float blurTCOffsets[ 5 ] = { 1.354203f, 3.343485f, 5.329522f, 7.304296f, 9.266765f };
	//const float blurWeights[ 6 ] = { 2.050781e-1f, 1.171875e-1f, 4.394531e-2f, 9.765625e-3f, 9.765625e-4f };
	const float blurWeights[ 6 ] = { 3.549092e-1f, 2.373966e-1f, 6.387397e-2f, 1.676156e-2f, 3.835649e-3f, 6.776054e-4f };
	
	bloomWidth = brightWidth / ( 1 << topMipMapLevel );
	bloomHeight = brightHeight / ( 1 << topMipMapLevel );
	
	const float levelDiffScale = powf( 2.0f, level - ceilf( level ) );
	
	
	for( i=topMipMapLevel; i>=0; i-- ){
		const deoglDebugTraceGroup debugTrace2( renderThread, "Blur Pass" );
		pipeline = plan.GetRenderStereo() ? pPipelineBloomBlurStereo : pPipelineBloomBlur;
		pipeline->Activate();
		shader = &pipeline->GetShader();
		
		shader->SetParameterFloat( spbbWeights1, blurWeights[ 0 ], blurWeights[ 1 ], blurWeights[ 2 ], blurWeights[ 3 ] );
		shader->SetParameterFloat( spbbWeights2, blurWeights[ 4 ], blurWeights[ 5 ], 0.0f, 0.0f );
		
		renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
		
		OGL_CHECK( renderThread, glViewport( 0, 0, bloomWidth, bloomHeight ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, bloomWidth, bloomHeight ) );
		
		// blur in x direction
		//const float curLevel = ( float )i;
		const float curLevel = i == topMipMapLevel ? level : ( float )i;
		float tcScaleLevel = powf( 2.0f, curLevel ); // * blurLevelScale;
		defren.SetShaderParamFSQuad( *shader, spbbTCTransform, brightWidth, brightHeight );
		shader->SetParameterFloat( spbbClamp, pixelSizeU * ( ( float )brightWidth - 0.5f - tcScaleLevel ),
			pixelSizeV * ( ( float )brightHeight - 0.5f - tcScaleLevel ) );
		
		defren.ActivateFBOTemporary2( false );
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary1(), GetSamplerClampLinearMipMap() );
		
		shader->SetParameterFloat( spbbOffsets1, blurTCOffsets[ 0 ] * pixelSizeU * tcScaleLevel, 0.0f,
			-blurTCOffsets[ 0 ] * pixelSizeU * tcScaleLevel, 0.0f );
		shader->SetParameterFloat( spbbOffsets2, blurTCOffsets[ 1 ] * pixelSizeU * tcScaleLevel, 0.0f,
			-blurTCOffsets[ 1 ] * pixelSizeU * tcScaleLevel, 0.0f );
		shader->SetParameterFloat( spbbOffsets3, blurTCOffsets[ 2 ] * pixelSizeU * tcScaleLevel, 0.0f,
			-blurTCOffsets[ 2 ] * pixelSizeU * tcScaleLevel, 0.0f );
		shader->SetParameterFloat( spbbOffsets4, blurTCOffsets[ 3 ] * pixelSizeU * tcScaleLevel, 0.0f,
			-blurTCOffsets[ 3 ] * pixelSizeU * tcScaleLevel, 0.0f );
		shader->SetParameterFloat( spbbOffsets5, blurTCOffsets[ 4 ] * pixelSizeU * tcScaleLevel, 0.0f,
			-blurTCOffsets[ 4 ] * pixelSizeU * tcScaleLevel, 0.0f );
		shader->SetParameterFloat( spbbLevel, curLevel );
		shader->SetParameterFloat( spbbScaleColor, 1.0f );
		
		RenderFullScreenQuad( plan );
		
		
		// blur in y direction
		defren.SetShaderParamFSQuad( *shader, spbbTCTransform, bloomWidth, bloomHeight );
		shader->SetParameterFloat( spbbClamp, pixelSizeU * ( ( float )bloomWidth - 0.5f ),
			pixelSizeV * ( ( float )bloomHeight - 0.5f ) );
		
		defren.ActivateFBOTemporary1Level( i );
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary2(), GetSamplerClampLinear() );
		
		//tcScaleLevel = blurLevelScale;
		tcScaleLevel = i == topMipMapLevel ? levelDiffScale : 1.0f;
		shader->SetParameterFloat( spbbOffsets1, 0.0f, blurTCOffsets[ 0 ] * pixelSizeV * tcScaleLevel,
			0.0f, -blurTCOffsets[ 0 ] * pixelSizeV * tcScaleLevel );
		shader->SetParameterFloat( spbbOffsets2, 0.0f, blurTCOffsets[ 1 ] * pixelSizeV * tcScaleLevel,
			0.0f, -blurTCOffsets[ 1 ] * pixelSizeV * tcScaleLevel );
		shader->SetParameterFloat( spbbOffsets3, 0.0f, blurTCOffsets[ 2 ] * pixelSizeV * tcScaleLevel,
			0.0f, -blurTCOffsets[ 2 ] * pixelSizeV * tcScaleLevel );
		shader->SetParameterFloat( spbbOffsets4, 0.0f, blurTCOffsets[ 3 ] * pixelSizeV * tcScaleLevel,
			0.0f, -blurTCOffsets[ 3 ] * pixelSizeV * tcScaleLevel );
		shader->SetParameterFloat( spbbOffsets5, 0.0f, blurTCOffsets[ 4 ] * pixelSizeV * tcScaleLevel,
			0.0f, -blurTCOffsets[ 4 ] * pixelSizeV * tcScaleLevel );
		shader->SetParameterFloat( spbbLevel, 0.0f );
		shader->SetParameterFloat( spbbScaleColor, 1.0 );
		
		RenderFullScreenQuad( plan );
		
		
		// add to previous level
		const int nextBloomWidth = bloomWidth * 2;
		const int nextBloomHeight = bloomHeight * 2;
		
		if( i > 0 ){
			pipeline = plan.GetRenderStereo() ? pPipelineBloomAddStereo : pPipelineBloomAdd;
			pipeline->Activate();
			shader = &pipeline->GetShader();
			
			renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
			
			OGL_CHECK( renderThread, glViewport( 0, 0, nextBloomWidth, nextBloomHeight ) );
			OGL_CHECK( renderThread, glScissor( 0, 0, nextBloomWidth, nextBloomHeight ) );
			
			const int shift = 1 << i;
			defren.SetShaderParamFSQuad( *shader, spbbTCTransform, brightWidth - shift, brightHeight - shift );
			shader->SetParameterFloat( spbaLevel, ( float )i );
			
			defren.ActivateFBOTemporary1Level( i - 1 );
			tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary1(), GetSamplerClampLinearMipMapNearest() );
			
			RenderFullScreenQuad( plan );
			
			bloomWidth = nextBloomWidth;
			bloomHeight = nextBloomHeight;
		}
	}
DEBUG_PRINT_TIMER( "ToneMap: Blooming" );
}

void deoglRenderToneMap::RenderToneMappingPass( deoglRenderPlan &plan, int bloomWidth, int bloomHeight ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "ToneMap.RenderToneMappingPass" );
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRCamera *oglCamera = plan.GetCamera();
	float clampBloomU, clampBloomV;
	deoglShaderCompiled *shader;
	
	clampBloomU = defren.GetPixelSizeU() * ( ( float )bloomWidth - 0.5f );
	clampBloomV = defren.GetPixelSizeV() * ( ( float )bloomHeight - 0.5f );
	
	// tone map rendered image
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	
	defren.ActivateFBOTemporary2( false );
	
	const bool useCustom = oglCamera->UseCustomToneMapCurve();
	
	const deoglPipeline &pipeline = useCustom
		? ( plan.GetRenderStereo() ? *pPipelineToneMapCustomStereo : *pPipelineToneMapCustom )
		: ( plan.GetRenderStereo() ? *pPipelineToneMapStereo : *pPipelineToneMap );
	pipeline.Activate();
	shader = &pipeline.GetShader();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	defren.SetShaderParamFSQuad( *shader, sptmTCBloomTransform, bloomWidth, bloomHeight );
	shader->SetParameterFloat( sptmTCBloomClamp, clampBloomU, clampBloomV );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 1, *oglCamera->GetToneMapParamsTexture(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 2, *defren.GetTextureTemporary1(), GetSamplerClampLinear() );
	if( useCustom ){
		tsmgr.EnableTexture( 3, *oglCamera->GetTextureToneMapCurve(), GetSamplerClampLinear() );
	}
	
	RenderFullScreenQuad( plan );
}

void deoglRenderToneMap::RenderLDR( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "ToneMap.RenderLDR" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShaderCompiled *shader;
	
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	
	defren.ActivateFBOTemporary2( false );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	
	const deoglPipeline &pipeline = plan.GetRenderStereo() ? *pPipelineLdrStereo : *pPipelineLdr;
	pipeline.Activate();
	shader = &pipeline.GetShader();
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	
	defren.SetShaderParamFSQuad( *shader, spfinTCTransform );
	shader->SetParameterFloat( spfinGamma, OGL_RENDER_INVGAMMA, OGL_RENDER_INVGAMMA, OGL_RENDER_INVGAMMA, 1.0f );
	shader->SetParameterFloat( spfinBrightness, 0.0f, 0.0f, 0.0f, 0.0f );
	shader->SetParameterFloat( spfinContrast, 1.0f, 1.0f, 1.0f, 1.0f );
	
	RenderFullScreenQuad( plan );
}



// Private Functions
//////////////////////

void deoglRenderToneMap::pCleanUp(){
	if( pTextureToneMapParams ){
		delete pTextureToneMapParams;
	}
}
