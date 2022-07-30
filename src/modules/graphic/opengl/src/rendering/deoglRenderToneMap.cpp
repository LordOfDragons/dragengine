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

#include <stdio.h>
#include <stdlib.h>

#include "deoglRenderToneMap.h"
#include "deoglRenderWorld.h"
#include "defren/deoglDeferredRendering.h"
#include "light/deoglRenderLight.h"
#include "plan/deoglRenderPlan.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugSaveTexture.h"
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
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
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

#define DEBUG_SNAPSHOT_TONEMAP 50



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
	spbbClamp
};

enum eSPBloomAdd{
	spbaPosToTC
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



// Debug Checks
/////////////////

static void DebugNanCheck( deoglRenderThread &renderThread, deoglDeferredRendering &defren, const deoglArrayTexture &texture ){
	const int defrenHeight = defren.GetHeight();
	const int defrenWidth = defren.GetWidth();
	const int defrenLayers = defren.GetLayerCount();
	const int texHeight = texture.GetHeight();
	const int texWidth = texture.GetWidth();
	
	deoglPixelBuffer pixelBuffer( deoglPixelBuffer::epfFloat4, texWidth, texHeight, 1 );
	
	texture.GetPixelsLevel( 0, pixelBuffer );
	decColor * const dummy = ( decColor* )pixelBuffer.GetPointer();
	
	int nanCulprits = 0;
	int nanCulpritsR = 0;
	int nanCulpritsG = 0;
	int nanCulpritsB = 0;
	int nanCulpritsA = 0;
	int infCulprits = 0;
	int infCulpritsR = 0;
	int infCulpritsG = 0;
	int infCulpritsB = 0;
	int infCulpritsA = 0;
	int x, y, l;
	
	for( l=0; l<defrenLayers; l++ ){
		for( y=0; y<defrenHeight; y++ ){
			for( x=0; x<defrenWidth; x++ ){
				const int i = ( texWidth * texHeight ) * l + ( texHeight - 1 - y ) * texWidth + x;
				
				if( isnan( dummy[i].r ) ){
					nanCulpritsR++;
				}
				if( isnan( dummy[i].g ) ){
					nanCulpritsG++;
				}
				if( isnan( dummy[i].b ) ){
					nanCulpritsB++;
				}
				if( isnan( dummy[i].a ) ){
					nanCulpritsA++;
				}
				
				if( isinf( dummy[i].r ) ){
					infCulpritsR++;
				}
				if( isinf( dummy[i].g ) ){
					infCulpritsG++;
				}
				if( isinf( dummy[i].b ) ){
					infCulpritsB++;
				}
				if( isinf( dummy[i].a ) ){
					infCulpritsA++;
				}
				
				if( isnan( dummy[i].r ) || isnan( dummy[i].g ) || isnan( dummy[i].b ) || isnan( dummy[i].a ) ){
					nanCulprits++;
				}
				if( isinf( dummy[i].r ) || isinf( dummy[i].g ) || isinf( dummy[i].b ) || isinf( dummy[i].a ) ){
					infCulprits++;
				}
			}
		}
	}
	
	renderThread.GetLogger().LogInfoFormat( "nan/inf check rendered image: nan(%i,%i,%i,%i|%i) inf(%i,%i,%i,%i|%i)",
		nanCulpritsR, nanCulpritsG, nanCulpritsB, nanCulpritsA, nanCulprits,
		infCulpritsR, infCulpritsG, infCulpritsB, infCulpritsA, infCulprits );
	
	delete [] dummy;
}

static void DebugAvgSceneColor( deoglRenderThread &renderThread, const deoglArrayTexture &texture, int width, int height ){
	const int texHeight = texture.GetHeight();
	const int texWidth = texture.GetWidth();
	int totallyBlack = 0;
	
	deoglPixelBuffer pixelBuffer( deoglPixelBuffer::epfFloat4, texWidth, texHeight, 1 );
	
	texture.GetPixelsLevel( 0, pixelBuffer );
	decColor * const dummy = ( decColor* )pixelBuffer.GetPointer();
	
	const int count = width * height;
	float avg = 0.0f;
	int x, y;
	
	for( y=0; y<height; y++ ){
		for( x=0; x<width; x++ ){
			const float value = dummy[ ( texHeight - 1 - y ) * texWidth + x ].r;
			
			avg += value;
			
			if( value < -9.2f ){
				totallyBlack++;
			}
			//if( width <= 4 && height <= 4 ){
			//	ogl.LogInfoFormat( "average scene color: (%ix%i) value (%i,%i) = %g", width, height, x, y, value );
			//}
		}
	}
	
	renderThread.GetLogger().LogInfoFormat( "average scene color: (%ix%i) %g (%i totallyBlack)", width, height, expf( avg / ( float )count ), totallyBlack );
	
	delete [] dummy;
}



// Class deoglRenderToneMap
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderToneMap::deoglRenderToneMap( deoglRenderThread &renderThread ) : deoglRenderBase( renderThread ){
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderDefines defines, commonDefines;
	deoglShaderSources *sources;
	
	pFBOToneMapParams = NULL;
	pTextureToneMapParams = NULL;
	
	try{
		renderThread.GetShader().AddCommonDefines( commonDefines );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "ToneMap Color2LogLum" );
		defines.AddDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		pShaderColor2LogLum = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "ToneMap Color2LogLum Stereo" );
		defines.AddDefines( "GS_RENDER_STEREO" );
		pShaderColor2LogLumStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "ToneMap Average LogLum" );
		defines.AddDefines( "NO_POSTRANSFORM" );
		pShaderAvgLogLum = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "ToneMap Average LogLum Stereo" );
		defines.AddDefines( "GS_RENDER_STEREO" );
		pShaderAvgLogLumStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "ToneMap Parameters" );
		defines.AddDefines( "NO_POSTRANSFORM", "NO_TEXCOORD" );
		pShaderParameters = shaderManager.GetProgramWith( sources, defines );
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "ToneMap Bright-Pass" );
		defines.AddDefines( "NO_POSTRANSFORM", "FULLSCREENQUAD" );
		pShaderBrightPass = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "ToneMap Bright-Pass Stereo" );
		defines.AddDefines( "GS_RENDER_STEREO" );
		pShaderBrightPassStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "ToneMap Bloom Reduce" );
		pShaderBloomReduce = shaderManager.GetProgramWith( sources, defines ); // not used
		
		sources = shaderManager.GetSourcesNamed( "ToneMap Bloom Add" );
		pShaderBloomAdd = shaderManager.GetProgramWith( sources, defines ); // not used
		
		
		sources = shaderManager.GetSourcesNamed( "ToneMap Bloom Blur" );
		defines.AddDefines( "NO_POSTRANSFORM" );
		pShaderBloomBlur = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "ToneMap Bloom Blur Stereo" );
		defines.AddDefines( "GS_RENDER_STEREO" );
		pShaderBloomBlurStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "ToneMap Tone Mapping" );
		defines.AddDefines( "NO_POSTRANSFORM", "NO_TCTRANSFORM" );
		pShaderToneMap = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "ToneMap Tone Mapping Stereo" );
		defines.AddDefines( "GS_RENDER_STEREO" );
		pShaderToneMapStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "DefRen Finalize" );
		defines.AddDefines( "NO_POSTRANSFORM" );
		pShaderFinalize = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "DefRen Finalize Stereo" );
		defines.AddDefines( "GS_RENDER_STEREO" );
		pShaderFinalizeStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "ToneMap Luminance Prepare" );
		defines.AddDefines( "NO_POSTRANSFORM", "NO_TEXCOORD" );
		pShaderLumPrepare = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "ToneMap Luminance Prepare Stereo" );
		defines.AddDefines( "GS_RENDER_STEREO" );
		pShaderLumPrepareStereo = shaderManager.GetProgramWith( sources, defines );
		
		
		pFBOToneMapParams = new deoglFramebuffer( renderThread, false );
		
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
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const int height = defren.GetHeight();
	const int width = defren.GetWidth();
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	defren.ActivateFBOLuminance();
	
	OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, width, height ) );
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	
	renderThread.GetShader().ActivateShader( plan.GetRenderStereo() ? pShaderLumPrepareStereo : pShaderLumPrepare );
	
	renderThread.GetRenderers().GetWorld().ActivateRenderPB( plan );
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
DEBUG_PRINT_TIMER_TOTAL( "LuminancePrepare" );
}

void deoglRenderToneMap::ToneMap( deoglRenderPlan &plan ){
DEBUG_RESET_TIMERS;
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	
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
	
	if( config.GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
DEBUG_PRINT_TIMER_TOTAL( "Tone-Mapping" );
}

void deoglRenderToneMap::CalculateSceneKey( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
//	const float pixelSizeS = defren.GetPixelSizeU();
//	const float pixelSizeT = defren.GetPixelSizeV();
	deoglShaderCompiled *shader;
	bool modeTarget;
	
	if( config.GetDebugSnapshot() == 55 ){
		DebugNanCheck( renderThread, defren, *defren.GetTextureLuminance() );
	}
	
	if( config.GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
// 		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture( *defren.GetTextureColor(), "tonemap_input_color" );
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture( *defren.GetTextureLuminance(), "tonemap_input_luminance" );
	}
	
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
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	
	defren.ActivateFBOTemporary1( false );
	OGL_CHECK( renderThread, glViewport( 0, 0, curWidth, curHeight ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, curWidth, curHeight ) );
// 	tsmgr.EnableTexture( 0, *defren.GetTextureColor(), GetSamplerClampLinear() );
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureLuminance(), GetSamplerClampNearest() );
	
	deoglShaderProgram *program = plan.GetRenderStereo() ? pShaderColor2LogLumStereo : pShaderColor2LogLum;
	renderThread.GetShader().ActivateShader( program );
	shader = program->GetCompiled();
	
	renderThread.GetRenderers().GetWorld().ActivateRenderPB( plan );
	
	shader->SetParameterFloat( spc2llParam1, tcOffsetU, 0.0f, 0.0f, tcOffsetV );
	shader->SetParameterFloat( spc2llParam2, tcOffsetU, tcOffsetV, clampU, clampV );
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	if( config.GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureLevelConversion( *defren.GetTextureTemporary1(),
			0, "tonemap_loglum", deoglDebugSaveTexture::ecLogIntensity );
	}
DEBUG_PRINT_TIMER( "ToneMap: LogLum" );
	
	// average the log luminances
	program = plan.GetRenderStereo() ? pShaderAvgLogLumStereo : pShaderAvgLogLum;
	renderThread.GetShader().ActivateShader( program );
	shader = program->GetCompiled();
	
	renderThread.GetRenderers().GetWorld().ActivateRenderPB( plan );
	
	modeTarget = false;
	
	if( useTextureBarrier ){
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary1(), GetSamplerClampLinear() );
	}
	
	int round = 0;
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
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		
		if( config.GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
			decString text;
			text.Format( "tonemap_avg_lumlog_%i_%ix%i_to_%ix%i", round++, lastWidth, lastHeight, curWidth, curHeight );
			if( modeTarget || useTextureBarrier ){
				renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureLevelConversion( *defren.GetTextureTemporary1(),
					0, text.GetString(), deoglDebugSaveTexture::ecLogIntensity );
			}else{
				renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTextureLevelConversion( *defren.GetTextureTemporary2(),
					0, text.GetString(), deoglDebugSaveTexture::ecLogIntensity );
			}
		}
		
		if( config.GetDebugSnapshot() == 55 ){
			if( modeTarget || useTextureBarrier ){
				DebugAvgSceneColor( renderThread, *defren.GetTextureTemporary1(), curWidth, curHeight );
			}else{
				DebugAvgSceneColor( renderThread, *defren.GetTextureTemporary2(), curWidth, curHeight );
			}
		}
		
		modeTarget = ! modeTarget;
	}
DEBUG_PRINT_TIMER( "ToneMap: Average" );
	
	if( config.GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
		if( plan.GetWorld()->GetSkyEnvironmentMap() && plan.GetWorld()->GetSkyEnvironmentMap()->GetEnvironmentMap() ){
			renderThread.GetDebug().GetDebugSaveTexture().SaveCubeMapLevel(
				*plan.GetWorld()->GetSkyEnvironmentMap()->GetEnvironmentMap(), 8, "tonemap_envmap", false );
		}
	}
	
	// determine tone map parameters to use for this scene
	renderThread.GetFramebuffer().Activate( pFBOToneMapParams );
	
	deoglRCamera &oglCamera = *plan.GetCamera();
	deoglTexture * const lastParams = oglCamera.GetToneMapParamsTexture();
	
	oglCamera.SetForceToneMapAdaption( false );
	
	renderThread.GetShader().ActivateShader( pShaderParameters );
	shader = pShaderParameters->GetCompiled();
	
	renderThread.GetRenderers().GetWorld().ActivateRenderPB( plan );
	
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
	pFBOToneMapParams->AttachColorTexture( 0, pTextureToneMapParams );
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOToneMapParams->Verify();
	
	OGL_CHECK( renderThread, glViewport( 0, 0, 1, 1 ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, 1, 1 ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
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
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	oglCamera.SetToneMapParamsTexture( pTextureToneMapParams );
	pTextureToneMapParams = lastParams;
DEBUG_PRINT_TIMER( "ToneMap: Determine Parameters" );
	
	if( config.GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
		deoglPixelBuffer pixelBuffer( deoglPixelBuffer::epfFloat4, 1, 1, 1 );
		oglCamera.GetToneMapParamsTexture()->GetPixelsLevel( 0, pixelBuffer );
		const deoglPixelBuffer::sFloat4 avgSceneColor = *pixelBuffer.GetPointerFloat4();
		renderThread.GetLogger().LogInfoFormat( "tone map params: %g %g %g %g", avgSceneColor.r, avgSceneColor.g, avgSceneColor.b, avgSceneColor.a );
	}
}

void deoglRenderToneMap::RenderBloomPass( deoglRenderPlan &plan, int &bloomWidth, int &bloomHeight ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const float pixelSizeU = defren.GetPixelSizeU();
	const float pixelSizeV = defren.GetPixelSizeV();
	deoglRCamera *oglCamera = plan.GetCamera();
	int realWidth = defren.GetWidth();
	int realHeight = defren.GetHeight();
	deoglShaderCompiled *shader;
	int lastWidth, lastHeight;
	int curWidth, curHeight;
	//bool modeTarget;
	int i;
	
	// 1/16 downscale??
	
	/*float offset = 1.5f;
	float weights[ 2 ];
	weights[ 0 ] = 1.0f / sqrtf( 2.0f * PI );
	weights[ 1 ] = expf( -( offset * offset ) / 2.0f ) / sqrtf( 2.0f * PI );*/
	
	//float g = 1.0f / sqrtf( 2.0f * D3DX_PI * rho * rho ); // rho = 1.0
	//g *= expf( -(x*x + y*y)/(2*rho*rho) );
	
	// set opengl states
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE ) );
	
	// convert color to bright values. to allow for proper bluring the output image is reduced
	// to the largest power of two size fitting inside
	for( curWidth=1; (curWidth<<1)<realWidth; curWidth<<= 1 );
	for( curHeight=1; (curHeight<<1)<realHeight; curHeight<<=1 );
	/*
	if( curWidth * 2 > realWidth ){
		curWidth >>= 1;
	}
	if( curHeight * 2 > realHeight ){
		curHeight >>= 1;
	}
	*/
	
	bloomWidth = curWidth;
	bloomHeight = curHeight;
	
	renderThread.GetShader().ActivateShader( plan.GetRenderStereo() ? pShaderBrightPassStereo : pShaderBrightPass );
	
	renderThread.GetRenderers().GetWorld().ActivateRenderPB( plan );
	
	defren.ActivateFBOTemporary1( false );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, curWidth, curHeight ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, curWidth, curHeight ) );
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureColor(), GetSamplerClampLinear() );
	tsmgr.EnableTexture( 1, *oglCamera->GetToneMapParamsTexture(), GetSamplerClampNearest() );
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	if( config.GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture( *defren.GetTextureTemporary1(), "tonemap_bright" );
	}
	
	// determine the number of blur passes. depends right now on the size of the bright image.
	// right now on a 1024 image 4 blur passes are done (1024, 512, 256, 128). hence the
	// number of passes is given be the number of times the bright image size can be halved
	// before it falls below 128. usually the width is larger than the height but both cases
	// are taken into consideration. hence the largest of both values is used to determine
	// the required number of passes
	int blurPassCount = 0;
	
	if( curWidth > curHeight ){
		lastWidth = curWidth;
		
	}else{
		lastWidth = curHeight;
	}
	
	for( blurPassCount=0; lastWidth>=128; lastWidth>>=1, blurPassCount++ );
	
	blurPassCount = 1;
	//modeTarget = false;
	
	lastWidth = curWidth;
	lastHeight = curHeight;
	
	// apply a blur filter according to 'tonemapping' in the 'doc' directory
	const float blurTCOffsets[] = { 1.354203f, 3.343485f, 5.329522f, 7.304296f, 9.266765f };
	const float blurWeights[] = { 1.232953f, 3.278228e-1f, 8.461847e-2f, 1.874333e-2f, 3.136081e-3f };
	
	deoglShaderProgram *program = plan.GetRenderStereo() ? pShaderBloomBlurStereo : pShaderBloomBlur;
	renderThread.GetShader().ActivateShader( program );
	shader = program->GetCompiled();
	
	renderThread.GetRenderers().GetWorld().ActivateRenderPB( plan );
	
	defren.SetShaderParamFSQuad( *shader, spbbTCTransform, 0.0f, 0.0f, ( float )lastWidth, ( float )lastHeight );
	shader->SetParameterFloat( spbbClamp, pixelSizeU * ( ( float )lastWidth - 0.5f ), pixelSizeV * ( ( float )lastHeight - 0.5f ) );
	
	shader->SetParameterFloat( spbbWeights1, 1.0f, blurWeights[ 0 ], blurWeights[ 1 ], blurWeights[ 2 ] );
	shader->SetParameterFloat( spbbWeights2, blurWeights[ 3 ], blurWeights[ 4 ], 0.0f, 0.0f );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, curWidth, curHeight ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, curWidth, curHeight ) );
	
	for( i=0; i<blurPassCount; i++ ){
		// blur in x direction
		defren.ActivateFBOTemporary2( false );
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary1(), GetSamplerClampLinear() );
		
		shader->SetParameterFloat( spbbOffsets1, blurTCOffsets[ 0 ] * pixelSizeU, 0.0f, -blurTCOffsets[ 0 ] * pixelSizeU, 0.0f );
		shader->SetParameterFloat( spbbOffsets2, blurTCOffsets[ 1 ] * pixelSizeU, 0.0f, -blurTCOffsets[ 1 ] * pixelSizeU, 0.0f );
		shader->SetParameterFloat( spbbOffsets3, blurTCOffsets[ 2 ] * pixelSizeU, 0.0f, -blurTCOffsets[ 2 ] * pixelSizeU, 0.0f );
		shader->SetParameterFloat( spbbOffsets4, blurTCOffsets[ 3 ] * pixelSizeU, 0.0f, -blurTCOffsets[ 3 ] * pixelSizeU, 0.0f );
		shader->SetParameterFloat( spbbOffsets5, blurTCOffsets[ 4 ] * pixelSizeU, 0.0f, -blurTCOffsets[ 4 ] * pixelSizeU, 0.0f );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		
		if( config.GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
			decString text;
			text.Format( "tonemap_bloom_%i_blur_x", i );
			renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture( *defren.GetTextureTemporary2(), text.GetString() ); // temporary2
		}
		
		// blur in y direction
		defren.ActivateFBOTemporary1( false );
		tsmgr.EnableArrayTexture( 0, *defren.GetTextureTemporary2(), GetSamplerClampLinear() );
		
		shader->SetParameterFloat( spbbOffsets1, 0.0f, blurTCOffsets[ 0 ] * pixelSizeV, 0.0f, -blurTCOffsets[ 0 ] * pixelSizeV );
		shader->SetParameterFloat( spbbOffsets2, 0.0f, blurTCOffsets[ 1 ] * pixelSizeV, 0.0f, -blurTCOffsets[ 1 ] * pixelSizeV );
		shader->SetParameterFloat( spbbOffsets3, 0.0f, blurTCOffsets[ 2 ] * pixelSizeV, 0.0f, -blurTCOffsets[ 2 ] * pixelSizeV );
		shader->SetParameterFloat( spbbOffsets4, 0.0f, blurTCOffsets[ 3 ] * pixelSizeV, 0.0f, -blurTCOffsets[ 3 ] * pixelSizeV );
		shader->SetParameterFloat( spbbOffsets5, 0.0f, blurTCOffsets[ 4 ] * pixelSizeV, 0.0f, -blurTCOffsets[ 4 ] * pixelSizeV );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		
		if( config.GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
			decString text;
			text.Format( "tonemap_bloom_%i_blur_y", i );
			renderThread.GetDebug().GetDebugSaveTexture().SaveArrayTexture( *defren.GetTextureTemporary1(), text.GetString() );
		}
	}
	
#if 0
	// blur brightness additive to obtain bloom
	const float offsetS = pixelSizeU * offset;
	const float offsetT = pixelSizeV * offset;
	int tcOffsetS, tcOffsetT;
	int tcScaleS, tcScaleT;
	
	for( i=0; i<blurPassCount; i++ ){
		lastWidth = curWidth;
		lastHeight = curHeight;
		
		// reduce the size by half if not the first image (which is already reduced)
		if( i > 0 ){
			tcScaleS = 1;
			tcScaleT = 1;
			tcOffsetS = 0;
			tcOffsetT = 0;
			
			if( curWidth > 2 ){
				curWidth >>= 1;
				tcScaleS = 2;
				tcOffsetS = 1;
			}
			if( curHeight > 2 ){
				curHeight >>= 1;
				tcScaleT = 2;
				tcOffsetT = 1;
			}
			
			renderThread.GetShader().ActivateShader( pShaderBloomReduce );
			shader = pShaderBloomReduce->GetCompiled();
			shader->SetParameterInt( spbrParam1, tcScaleS, tcScaleT, tcOffsetS, 0 );
			shader->SetParameterInt( spbrParam2, 0, tcOffsetT, tcOffsetS, tcOffsetT );
			
			if( modeTarget ){
				defren.ActivateFBOTemporary2( false );
				tsmgr.EnableTexture( 0, *defren.GetSpecularityTexture(), GetSamplerClampLinear() );
				
			}else{
				defren.ActivateFBOSpecularity();
				tsmgr.EnableTexture( 0, *defren.GetTemporary2Texture(), GetSamplerClampLinear() );
			}
			OGL_CHECK( renderThread, glViewport( 0, 0, curWidth, curHeight ) );
			OGL_CHECK( renderThread, glScissor( 0, 0, curWidth, curHeight ) );
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
			
			if( renderThread.GetConfiguration()->GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
				decString text;
				text.Format( "tonemap_bloom_%i_reduce_%ix%i_to_%ix%i", i, lastWidth, lastHeight, curWidth, curHeight );
				if( modeTarget ){
					renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *defren.GetTemporary2Texture(), text.GetString(), false );
				}else{
					renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *defren.GetSpecularityTexture(), text.GetString(), false );
				}
			}
			
			modeTarget = ! modeTarget;
		}
		
		// blur filter in x direction
		renderThread.GetShader().ActivateShader( pShaderBloomBlur );
		shader = pShaderBloomBlur->GetCompiled();
		
		defren.SetShaderParamFSQuad( *shader, spbbPosToTC, curWidth, curHeight );
		shader->SetParameterFloat( spbbOffsets, offsetS, 0.0f, -offsetS, 0.0f );
		shader->SetParameterFloat( spbbWeights, weights[ 0 ], weights[ 1 ] );
		shader->SetParameterFloat( spbbClamp, pixelSizeU * ( float )( lastWidth - 1 ), pixelSizeV * ( float )( lastHeight - 1 ) );
		
		if( modeTarget ){
			defren.ActivateFBOTemporary2( false );
			tsmgr.EnableTexture( 0, *defren.GetSpecularityTexture(), GetSamplerClampLinear() );
			
		}else{
			defren.ActivateFBOSpecularity();
			tsmgr.EnableTexture( 0, *defren.GetTemporary2Texture(), GetSamplerClampLinear() );
		}
		OGL_CHECK( renderThread, glViewport( 0, 0, curWidth, curHeight ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, curWidth, curHeight ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		
		if( renderThread.GetConfiguration()->GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
			decString text;
			text.Format( "tonemap_bloom_%i_blur_x", i );
			if( modeTarget ){
				renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *defren.GetTemporary2Texture(), text.GetString(), false );
			}else{
				renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *defren.GetSpecularityTexture(), text.GetString(), false );
			}
		}
		
		modeTarget = ! modeTarget;
		
		// blur filter in y direction
		shader->SetParameterFloat( spbbOffsets, 0.0f, offsetT, 0.0f, -offsetT );
		
		if( modeTarget ){
			defren.ActivateFBOTemporary2( false );
			tsmgr.EnableTexture( 0, *defren.GetSpecularityTexture(), GetSamplerClampLinear() );
			
		}else{
			defren.ActivateFBOSpecularity();
			tsmgr.EnableTexture( 0, *defren.GetTemporary2Texture(), GetSamplerClampLinear() );
		}
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		
		if( renderThread.GetConfiguration()->GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
			decString text;
			text.Format( "tonemap_bloom_%i_blur_y", i );
			if( modeTarget ){
				renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *defren.GetTemporary2Texture(), text.GetString(), false );
			}else{
				renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *defren.GetSpecularityTexture(), text.GetString(), false );
			}
		}
		
		// add to bloom texture
		renderThread.GetShader().ActivateShader( pShaderBloomAdd );
		shader = pShaderBloomAdd->GetCompiled();
		defren.SetShaderParamFSQuad( *shader, spbaPosToTC, curWidth, curHeight );
		
		defren.ActivateFBOTemporary( false );
		OGL_CHECK( renderThread, glViewport( 0, 0, bloomWidth, bloomHeight ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, bloomWidth, bloomHeight ) );
		if( modeTarget ){
			tsmgr.EnableTexture( 0, *defren.GetTemporary2Texture(), GetSamplerClampLinear() );
			
		}else{
			tsmgr.EnableTexture( 0, *defren.GetSpecularityTexture(), GetSamplerClampLinear() );
		}
		
		if( i > 0 ){
			OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
		}
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		
		if( renderThread.GetConfiguration()->GetDebugSnapshot() == DEBUG_SNAPSHOT_TONEMAP ){
			decString text;
			text.Format( "tonemap_bloom_%i_result", i );
			renderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *defren.GetTemporaryTexture(), text.GetString(), false );
		}
	}
#endif
DEBUG_PRINT_TIMER( "ToneMap: Blooming" );
}

void deoglRenderToneMap::RenderToneMappingPass( deoglRenderPlan &plan, int bloomWidth, int bloomHeight ){
	deoglRenderThread &renderThread = GetRenderThread();
	
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
	
	deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderToneMapStereo : pShaderToneMap;
	renderThread.GetShader().ActivateShader( program );
	shader = program->GetCompiled();
	
	renderThread.GetRenderers().GetWorld().ActivateRenderPB( plan );
	
	defren.SetShaderParamFSQuad( *shader, sptmTCBloomTransform, bloomWidth, bloomHeight );
	shader->SetParameterFloat( sptmTCBloomClamp, clampBloomU, clampBloomV );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 1, *oglCamera->GetToneMapParamsTexture(), GetSamplerClampNearest() );
	tsmgr.EnableArrayTexture( 2, *defren.GetTextureTemporary1(), GetSamplerClampLinear() );
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
}

void deoglRenderToneMap::RenderLDR( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShaderCompiled *shader;
	
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	
	defren.ActivateFBOTemporary2( false );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
	
	deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderFinalizeStereo : pShaderFinalize;
	renderThread.GetShader().ActivateShader( program );
	shader = program->GetCompiled();
	
	renderThread.GetRenderers().GetWorld().ActivateRenderPB( plan );
	
	defren.SetShaderParamFSQuad( *shader, spfinTCTransform );
	shader->SetParameterFloat( spfinGamma, OGL_RENDER_INVGAMMA, OGL_RENDER_INVGAMMA, OGL_RENDER_INVGAMMA, 1.0f );
	shader->SetParameterFloat( spfinBrightness, 0.0f, 0.0f, 0.0f, 0.0f );
	shader->SetParameterFloat( spfinContrast, 1.0f, 1.0f, 1.0f, 1.0f );
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
}



// Private Functions
//////////////////////

void deoglRenderToneMap::pCleanUp(){
	if( pTextureToneMapParams ){
		delete pTextureToneMapParams;
	}
	if( pFBOToneMapParams ){
		delete pFBOToneMapParams;
	}
}
