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
#include <string.h>

#include "deoglRenderCanvas.h"
#include "deoglRenderCanvasContext.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlan.h"
#include "../canvas/render/deoglRCanvasCanvasView.h"
#include "../canvas/render/deoglRCanvasImage.h"
#include "../canvas/render/deoglRCanvasPaint.h"
#include "../canvas/render/deoglRCanvasRenderWorld.h"
#include "../canvas/render/deoglRCanvasText.h"
#include "../canvas/render/deoglRCanvasVideoPlayer.h"
#include "../canvas/render/deoglRCanvasView.h"
#include "../configuration/deoglConfiguration.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../debug/deoglDebugInformation.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../font/deoglRFont.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../rendering/deoglRenderWorld.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglSkinTexture.h"
#include "../target/deoglRenderTarget.h"
#include "../texture/deoglRImage.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../video/deoglRVideoPlayer.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglSharedVBOListList.h"
#include "../vr/deoglVR.h"
#include "../world/deoglRCamera.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUTF8Decoder.h>



// Definitions
////////////////

#define OFFSET_POINT	0
#define COUNT_POINT		1

#define OFFSET_LINE		1
#define COUNT_LINE		2

#define OFFSET_RECT		3
#define COUNT_RECT		4

enum eSPCanvas{
	spcTransform,
	spcTCTransform,
	spcColorTransform,
	spcColorTransform2,
	spcGamma,
	spcClipRect,
	spcTCClamp,
	spcTCTransformMask
};



// Class deoglRenderCanvas
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderCanvas::deoglRenderCanvas( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread ),

pVBOShapes( 0 ),
pVAOShapes( 0 ),
pActiveVAO( 0 ),

pDebugTimeCanvasView( 0.0f ),
pDebugCountCanvasView( 0 ),
pDebugTimeCanvasImage( 0.0f ),
pDebugCountCanvasImage( 0 ),
pDebugTimeCanvasPaint( 0.0f ),
pDebugCountCanvasPaint( 0 ),
pDebugTimeCanvasRenderWorld( 0.0f ),
pDebugCountCanvasRenderWorld( 0 ),
pDebugTimeCanvasText( 0.0f ),
pDebugCountCanvasText( 0 ),
pDebugTimeCanvasVideoPlayer( 0.0f ),
pDebugCountCanvasVideoPlayer( 0 ),
pDebugTimeCanvasCanvasView( 0.0f ),
pDebugCountCanvasCanvasView( 0 )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	const deoglShaderSources *sources;
	
	try{
		renderThread.GetShader().SetCommonDefines( commonDefines );
		
		pCreateShapesVAO();
		
		pipconf.Reset();
		pipconf.SetEnableScissorTest( true );
		pipconf.EnableBlendBlend();  // this can be dynamic
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "Canvas" );
		pCreatePipelines(pPipelineCanvasColor, pipconf, sources, defines);
		
		defines.SetDefines( "WITH_TEXTURE" );
		pCreatePipelines(pPipelineCanvasImage, pipconf, sources, defines);
		
		defines = commonDefines;
		defines.SetDefines( "WITH_MASK" );
		pCreatePipelines(pPipelineCanvasColorMask, pipconf, sources, defines);
		
		defines.SetDefines( "WITH_TEXTURE" );
		pCreatePipelines(pPipelineCanvasImageMask, pipconf, sources, defines);
		
		defines = commonDefines;
		defines.SetDefines( "WITH_RENDER_WORLD" );
		pCreatePipelines(pPipelineCanvasRenderWorld, pipconf, sources, defines);
		
		defines.SetDefines( "WITH_MASK" );
		pCreatePipelines(pPipelineCanvasRenderWorldMask, pipconf, sources, defines);
		
		
		
		// debug information
		const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
		const decColor colorBg( 0.0f, 0.0f, 0.25f, 0.75f );
		const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
		const decColor colorBgParallel1( 0.05f, 0.025f, 0.05f, 0.75f );
		const decColor colorBgParallel2( 0.025f, 0.05f, 0.05f, 0.75f );
		const decColor colorBgParallel3( 0.05f, 0.05f, 0.025f, 0.75f );
		
		pDebugInfoCanvas.TakeOver( new deoglDebugInformation( "Canvas", colorText, colorBg ) );
		
		pDebugInfoCanvasView.TakeOver( new deoglDebugInformation( "View", colorText, colorBgSub ) );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasView );
		
		pDebugInfoCanvasImage.TakeOver( new deoglDebugInformation( "Image", colorText, colorBgSub ) );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasImage );
		
		pDebugInfoCanvasPaint.TakeOver( new deoglDebugInformation( "Paint", colorText, colorBgSub ) );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasPaint );
		
		pDebugInfoCanvasRenderWorld.TakeOver( new deoglDebugInformation( "Render World", colorText, colorBgSub ) );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasRenderWorld );
		
		pDebugInfoCanvasText.TakeOver( new deoglDebugInformation( "Text", colorText, colorBgSub ) );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasText );
		
		pDebugInfoCanvasVideoPlayer.TakeOver( new deoglDebugInformation( "Video Player", colorText, colorBgSub ) );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasVideoPlayer );
		
		pDebugInfoCanvasCanvasView.TakeOver( new deoglDebugInformation( "Canvas View", colorText, colorBgSub ) );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasCanvasView );
		
		
		
		pDebugInfoPlanPrepare.TakeOver( new deoglDebugInformation( "Plan Prepare", colorText, colorBg ) );
		
		pDebugInfoPlanPrepareEarlyWorld.TakeOver( new deoglDebugInformation( "Early World", colorText, colorBgSub ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareEarlyWorld );
		
		pDebugInfoPlanPrepareFindContent.TakeOver( new deoglDebugInformation( "Find Content", colorText, colorBgParallel1 ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareFindContent );
		
		pDebugInfoPlanPrepareBuildRTs.TakeOver( new deoglDebugInformation( "Build RTs", colorText, colorBgParallel3 ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareBuildRTs );
		
		pDebugInfoPlanPrepareSkyLightFindContent.TakeOver( new deoglDebugInformation( "SL Find Content", colorText, colorBgParallel2 ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareSkyLightFindContent );
		
		pDebugInfoPlanPrepareSkyLightBuildRT.TakeOver( new deoglDebugInformation( "SL Build RT", colorText, colorBgParallel3 ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareSkyLightBuildRT );
		
		pDebugInfoPlanPrepareSkyLightGIFindContent.TakeOver( new deoglDebugInformation( "SL GI Find Content", colorText, colorBgParallel2 ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareSkyLightGIFindContent );
		
		pDebugInfoPlanPrepareSkyLightGIUpdateRenderTask.TakeOver( new deoglDebugInformation( "SL GI Update RT", colorText, colorBgParallel3 ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareSkyLightGIUpdateRenderTask );
		
		pDebugInfoPlanPrepareWorld.TakeOver( new deoglDebugInformation( "World", colorText, colorBgParallel1 ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareWorld );
		
		pDebugInfoPlanPrepareGIUpdate.TakeOver( new deoglDebugInformation( "GI Update", colorText, colorBgParallel1 ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareGIUpdate );
		
		pDebugInfoPlanPrepareCulling.TakeOver( new deoglDebugInformation( "Culling", colorText, colorBgSub ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareCulling );
		
		pDebugInfoPlanPrepareEnvMaps.TakeOver( new deoglDebugInformation( "Env-Maps", colorText, colorBgSub ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareEnvMaps );
		
		pDebugInfoPlanPreparePrepareContent.TakeOver( new deoglDebugInformation( "Prepare Content", colorText, colorBgSub ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPreparePrepareContent );
			
			pDebugInfoPlanPrepareHTViewVBOs.TakeOver( new deoglDebugInformation( "HT-View VBOs", colorText, colorBgSub2 ) );
			pDebugInfoPlanPreparePrepareContent->GetChildren().Add( pDebugInfoPlanPrepareHTViewVBOs );
		
		pDebugInfoPlanPrepareBuildPlan.TakeOver( new deoglDebugInformation( "Build Plan", colorText, colorBgSub ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareBuildPlan );
		
		pDebugInfoPlanPrepareLights.TakeOver( new deoglDebugInformation( "Lights", colorText, colorBgSub ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareLights );
		
		pDebugInfoPlanPrepareFinish.TakeOver( new deoglDebugInformation( "Finish", colorText, colorBgSub ) );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareFinish );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderCanvas::~deoglRenderCanvas(){
	pCleanUp();
}



// Management
///////////////

void deoglRenderCanvas::Prepare( const deoglRenderCanvasContext &context ){
	deoglRenderThread &renderThread = GetRenderThread();
	
	renderThread.GetBufferObject().GetSharedVBOListForType( deoglRTBufferObject::esvbolCanvasPaint ).PrepareVBOs();
	
	renderThread.GetTexture().GetStages().DisableAllStages();
	
	SetViewport( context.GetViewportOffset(), context.GetViewportSize() );
	
	OGL_CHECK( renderThread, pglBindVertexArray( pVAOShapes ) );
	pActiveVAO = pVAOShapes;
}



void deoglRenderCanvas::DrawCanvasPaint( const deoglRenderCanvasContext &context, const deoglRCanvasPaint &canvas ){
	if( ! ( canvas.GetSize() > decVector2() ) ){
		return; // too small
	}
	if( canvas.GetDrawCountFill() == 0 && canvas.GetDrawCountLine() == 0 ){
		return;
	}
	
	if( pDebugInfoCanvas->GetVisible() ){
		GetDebugTimerAt( 0 ).Reset();
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	const float transparency = context.GetTransparency();
	const float thickness = decMath::max( 0.0f, canvas.GetThickness() );
	
	const deoglPipeline &pipeline = context.GetMask()
		? *pPipelineCanvasColorMask[ canvas.GetBlendMode() ]
		: *pPipelineCanvasColor[ canvas.GetBlendMode() ];
	pipeline.Activate();
	
	if( context.GetMask() ){
		tsmgr.EnableTexture( 1, *context.GetMask(), GetSamplerClampLinear() );
	}
	
	deoglShaderCompiled &shader = pipeline.GetShader();
	
	shader.SetParameterFloat( spcClipRect,
		( context.GetClipMin().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMin().y + 1.0f ) * context.GetClipFactor().y,
		( context.GetClipMax().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMax().y + 1.0f ) * context.GetClipFactor().y );
	
	shader.SetParameterFloat( spcTCClamp,
		context.GetTCClampMinimum().x, context.GetTCClampMinimum().y,
		context.GetTCClampMaximum().x, context.GetTCClampMaximum().y );
	
	shader.SetParameterTexMatrix3x2( spcTransform, context.GetTransform() );
	shader.SetParameterTexMatrix3x2( spcTCTransformMask, context.GetTCTransformMask() );
	shader.SetParameterFloat( spcGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	
	deoglSharedVBOBlock &vboBlock = *canvas.GetVBOBlock();
	const int vboOffset = vboBlock.GetOffset();
	
	const GLuint vao = vboBlock.GetVBO()->GetVAO()->GetVAO();
	OGL_CHECK( GetRenderThread(), pglBindVertexArray( vao ) );
	pActiveVAO = vao;
	
	// fill shape if fill color is not transparent
	const decColor colorFill( canvas.GetFillColor(), canvas.GetFillColor().a * transparency );
	const decColorMatrix colorTransformFill( decColorMatrix::CreateScaling( colorFill ) * context.GetColorTransform() );
	
	if( canvas.GetDrawCountFill() > 0 && colorFill.a > FLOAT_SAFE_EPSILON ){
		shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransformFill );
		
		OGL_CHECK( renderThread, glDrawArrays( canvas.GetDrawModeFill(),
			vboOffset + canvas.GetDrawOffsetFill(), canvas.GetDrawCountFill() ) );
	}
	
	// outline shape if line color is not transparent
	const decColor colorLine( canvas.GetLineColor(), canvas.GetLineColor().a * transparency );
	
	if( canvas.GetDrawCountLine() > 0 && colorLine.a > FLOAT_SAFE_EPSILON && thickness > 0.1f ){
		const decColorMatrix colorTransformLine( decColorMatrix::CreateScaling( colorLine ) * context.GetColorTransform() );
		shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransformLine );
		
		OGL_CHECK( renderThread, glDrawArrays( canvas.GetDrawModeLine(),
			vboOffset + canvas.GetDrawOffsetLine(), canvas.GetDrawCountLine() ) );
	}
	
	if( pDebugInfoCanvas->GetVisible() ){
		pDebugTimeCanvasView += GetDebugTimerAt( 0 ).GetElapsedTime();
		pDebugCountCanvasView++;
	}
}

void deoglRenderCanvas::DrawCanvasImage( const deoglRenderCanvasContext &context, const deoglRCanvasImage &canvas ){
	deoglRImage * const image = canvas.GetImage();
	if( ! image || ! image->GetTexture() ){
		return;
	}
	
	if( pDebugInfoCanvas->GetVisible() ){
		GetDebugTimerAt( 0 ).Reset();
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	pActivateVAOShapes();
	
	const deoglPipeline &pipeline = context.GetMask()
		? *pPipelineCanvasImageMask[ canvas.GetBlendMode() ]
		: *pPipelineCanvasImage[ canvas.GetBlendMode() ];
	pipeline.Activate();
	
	tsmgr.EnableTexture( 0, *image->GetTexture(), GetSamplerRepeatLinear() );
	if( context.GetMask() ){
		tsmgr.EnableTexture( 1, *context.GetMask(), GetSamplerClampLinear() );
	}
	
	const float transparency = context.GetTransparency();
	const decColorMatrix colorTransform( decColorMatrix::CreateScaling(
		1.0f, 1.0f, 1.0f, transparency ) * context.GetColorTransform() );
	
	const decTexMatrix2 billboardTransform( decTexMatrix2::CreateScale( canvas.GetSize() ) );
	
	deoglShaderCompiled &shader = pipeline.GetShader();
	
	shader.SetParameterTexMatrix3x2( spcTransform, billboardTransform * context.GetTransform() );
	shader.SetParameterTexMatrix3x2( spcTCTransform, canvas.GetTCTransform() );
	shader.SetParameterTexMatrix3x2( spcTCTransformMask, context.GetTCTransformMask() );
	shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransform );
	shader.SetParameterFloat( spcGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	
	shader.SetParameterFloat( spcClipRect,
		( context.GetClipMin().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMin().y + 1.0f ) * context.GetClipFactor().y,
		( context.GetClipMax().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMax().y + 1.0f ) * context.GetClipFactor().y );
	
	shader.SetParameterFloat( spcTCClamp,
		context.GetTCClampMinimum().x, context.GetTCClampMinimum().y,
		context.GetTCClampMaximum().x, context.GetTCClampMaximum().y );
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, OFFSET_RECT, COUNT_RECT ) );
	
	tsmgr.DisableStage( 0 ); // TODO is this really required?
	
	if( pDebugInfoCanvas->GetVisible() ){
		pDebugTimeCanvasImage += GetDebugTimerAt( 0 ).GetElapsedTime();
		pDebugCountCanvasImage++;
	}
}

void deoglRenderCanvas::DrawCanvasCanvasView( const deoglRenderCanvasContext &context, const deoglRCanvasCanvasView &canvas ){
	deoglRCanvasView * const canvasView = canvas.GetCanvasView();
	if( ! canvasView ){
		return;
	}
	
	deoglRenderTarget * const renderTarget = canvasView->GetRenderTarget();
	if( ! renderTarget || ! renderTarget->GetTexture() ){
		return;
	}
	
	if( pDebugInfoCanvas->GetVisible() ){
		GetDebugTimerAt( 0 ).Reset();
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	pActivateVAOShapes();
	
	const deoglPipeline &pipeline = context.GetMask()
		? *pPipelineCanvasImageMask[ canvas.GetBlendMode() ]
		: *pPipelineCanvasImage[ canvas.GetBlendMode() ];
	pipeline.Activate();
	
	tsmgr.EnableTexture( 0, *renderTarget->GetTexture(), GetSamplerRepeatLinear() );
	if( context.GetMask() ){
		tsmgr.EnableTexture( 1, *context.GetMask(), GetSamplerClampLinear() );
	}
	
	const float transparency = context.GetTransparency();
	const decColorMatrix colorTransform( decColorMatrix::CreateScaling( 1.0f, 1.0f, 1.0f, transparency ) * context.GetColorTransform() );
	
	const decTexMatrix2 billboardTransform( decTexMatrix2::CreateScale( canvas.GetSize() ) );
	
	deoglShaderCompiled &shader = pipeline.GetShader();
	
	shader.SetParameterTexMatrix3x2( spcTransform, billboardTransform * context.GetTransform() );
	shader.SetParameterTexMatrix3x2( spcTCTransform, canvas.GetTCTransform() );
	shader.SetParameterTexMatrix3x2( spcTCTransformMask, context.GetTCTransformMask() );
	shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransform );
	shader.SetParameterFloat( spcGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	
	shader.SetParameterFloat( spcClipRect,
		( context.GetClipMin().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMin().y + 1.0f ) * context.GetClipFactor().y,
		( context.GetClipMax().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMax().y + 1.0f ) * context.GetClipFactor().y );
	
	shader.SetParameterFloat( spcTCClamp,
		context.GetTCClampMinimum().x, context.GetTCClampMinimum().y,
		context.GetTCClampMaximum().x, context.GetTCClampMaximum().y );
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, OFFSET_RECT, COUNT_RECT ) );
	
	tsmgr.DisableStage( 0 ); // TODO is this really required?
	
	if( pDebugInfoCanvas->GetVisible() ){
		pDebugTimeCanvasCanvasView += GetDebugTimerAt( 0 ).GetElapsedTime();
		pDebugCountCanvasCanvasView++;
	}
}

void deoglRenderCanvas::DrawCanvasVideoPlayer( const deoglRenderCanvasContext &context, const deoglRCanvasVideoPlayer &canvas ){
	deoglRVideoPlayer * const videoPlayer = canvas.GetVideoPlayer();
	if( ! videoPlayer || ! videoPlayer->GetTexture() ){
		return;
	}
	
	if( pDebugInfoCanvas->GetVisible() ){
		GetDebugTimerAt( 0 ).Reset();
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	pActivateVAOShapes();
	
	const deoglPipeline &pipeline = context.GetMask()
		? *pPipelineCanvasImageMask[ canvas.GetBlendMode() ]
		: *pPipelineCanvasImage[ canvas.GetBlendMode() ];
	pipeline.Activate();
	
	tsmgr.EnableTexture( 0, *videoPlayer->GetTexture(), GetSamplerRepeatLinear() );
	if( context.GetMask() ){
		tsmgr.EnableTexture( 1, *context.GetMask(), GetSamplerClampLinear() );
	}
	
	const float transparency = context.GetTransparency();
	const decColorMatrix colorTransform( decColorMatrix::CreateScaling( 1.0f, 1.0f, 1.0f, transparency ) * context.GetColorTransform() );
	
	const decTexMatrix2 billboardTransform( decTexMatrix2::CreateScale( canvas.GetSize() ) );
	
	deoglShaderCompiled &shader = pipeline.GetShader();
	
	shader.SetParameterTexMatrix3x2( spcTransform, billboardTransform * context.GetTransform() );
	shader.SetParameterTexMatrix3x2( spcTCTransform, canvas.GetTCTransform() );
	shader.SetParameterTexMatrix3x2( spcTCTransformMask, context.GetTCTransformMask() );
	shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransform );
	shader.SetParameterFloat( spcGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	
	shader.SetParameterFloat( spcClipRect,
		( context.GetClipMin().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMin().y + 1.0f ) * context.GetClipFactor().y,
		( context.GetClipMax().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMax().y + 1.0f ) * context.GetClipFactor().y );
	
	shader.SetParameterFloat( spcTCClamp,
		context.GetTCClampMinimum().x, context.GetTCClampMinimum().y,
		context.GetTCClampMaximum().x, context.GetTCClampMaximum().y );
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, OFFSET_RECT, COUNT_RECT ) );
	
	tsmgr.DisableStage( 0 ); // TODO is this really required?
	
	if( pDebugInfoCanvas->GetVisible() ){
		pDebugTimeCanvasVideoPlayer += GetDebugTimerAt( 0 ).GetElapsedTime();
		pDebugCountCanvasVideoPlayer++;
	}
}

void deoglRenderCanvas::DrawCanvasText( const deoglRenderCanvasContext &context, const deoglRCanvasText &canvas ){
	deoglRFont * const font = canvas.GetFont();
	if( ! font ){
		return;
	}
	
	deoglRImage * const image = font->GetImage();
	if( ! image || ! image->GetTexture() ){
		return;
	}
	
	if( pDebugInfoCanvas->GetVisible() ){
		GetDebugTimerAt( 0 ).Reset();
	}
	
	// scaling and offset
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const float factorU = 1.0f / ( float )image->GetWidth();
	const float factorV = 1.0f / ( float )image->GetHeight();
	const float offsetU = config.GetTextOffsetU() * factorU;
	const float offsetV = config.GetTextOffsetV() * factorV;
	
	pActivateVAOShapes();
	
	const deoglPipeline &pipeline = context.GetMask()
		? *pPipelineCanvasImageMask[ canvas.GetBlendMode() ]
		: *pPipelineCanvasImage[ canvas.GetBlendMode() ];
	pipeline.Activate();
	
	// set texture
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	tsmgr.EnableTexture( 0, *image->GetTexture(), GetSamplerClampNearest() );
	if( context.GetMask() ){
		tsmgr.EnableTexture( 1, *context.GetMask(), GetSamplerClampLinear() );
	}
	
	// set shader
	deoglShaderCompiled &shader = pipeline.GetShader();
	
	// set color
	const float transparency = context.GetTransparency();
	
	if( font->GetIsColorFont() ){
		const decColorMatrix colorTransform( decColorMatrix::CreateScaling( 1.0f, 1.0f, 1.0f, transparency ) * context.GetColorTransform() );
		shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransform );
		
	}else{
		const decColor color( canvas.GetColor(), canvas.GetColor().a * transparency );
		const decColorMatrix colorTransform( decColorMatrix::CreateScaling( color ) * context.GetColorTransform() );
		shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransform );
	}
	
	shader.SetParameterFloat( spcGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	
	// set clipping
	shader.SetParameterFloat( spcClipRect,
		( context.GetClipMin().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMin().y + 1.0f ) * context.GetClipFactor().y,
		( context.GetClipMax().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMax().y + 1.0f ) * context.GetClipFactor().y );
	
	shader.SetParameterFloat( spcTCClamp,
		context.GetTCClampMinimum().x, context.GetTCClampMinimum().y,
		context.GetTCClampMaximum().x, context.GetTCClampMaximum().y );
	
	shader.SetParameterTexMatrix3x2( spcTCTransformMask, context.GetTCTransformMask() );
	
	// render text
	const deoglRFont::sGlyph * const oglGlyphs = font->GetGlyphs();
	decUTF8Decoder utf8Decoder;
	float curx = 0.0f;
	float cury = 0.0f;
	
	utf8Decoder.SetString( canvas.GetText() );
	const int len = utf8Decoder.GetLength();
	const float fontScale = canvas.GetFontSize() / ( float )font->GetLineHeight();
	
	const decTexMatrix2 &transform = context.GetTransform();
	
	while( utf8Decoder.GetPosition() < len ){
		const int character = utf8Decoder.DecodeNextCharacter();
		if( character < 0 ){
			continue; // invalid unicode character
		}
		
		const deoglRFont::sGlyph &oglGlyph = ( character < 256 )
			? oglGlyphs[ character ] : font->GetUndefinedGlyph();
			// temp hack: not working for unicode
		
		// calculate positions
		const float x1 = curx - ( float )oglGlyph.bearing * fontScale;
		const float y1 = cury;
		const float cw = ( float )oglGlyph.width * fontScale;
		const float x2 = x1 + cw;
		const float y2 = y1 + ( float )oglGlyph.height * fontScale;
		const float adv = ( float )oglGlyph.advance * fontScale;
		
		// render char
		const decTexMatrix2 rectTransform( decTexMatrix2::CreateST( x2 - x1, y2 - y1, x1, y1 ) );
		shader.SetParameterTexMatrix3x2( spcTransform, rectTransform * transform );
		
		const decTexMatrix2 tcTransform( decTexMatrix2::CreateST(
			( oglGlyph.x2 - oglGlyph.x1 ) * factorU, ( oglGlyph.y2 - oglGlyph.y1 ) * factorV,
			                                 oglGlyph.x1 * factorU + offsetU, oglGlyph.y1 * factorV + offsetV ) );
		shader.SetParameterTexMatrix3x2( spcTCTransform, tcTransform );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, OFFSET_RECT, COUNT_RECT ) );
		
		// next round
		curx += adv;
	}
	
	// clean up
	tsmgr.DisableStage( 0 ); // TODO is this really required?
	
	if( pDebugInfoCanvas->GetVisible() ){
		pDebugTimeCanvasText += GetDebugTimerAt( 0 ).GetElapsedTime();
		pDebugCountCanvasText++;
	}
}

void deoglRenderCanvas::DrawCanvasRenderWorld( const deoglRenderCanvasContext &context,
const deoglRCanvasRenderWorld &canvas ){
	deoglRCamera * const camera = canvas.GetCamera();
	if( ! camera ){
		return;
	}
	
	deoglRWorld * const world = camera->GetParentWorld();
	if( ! world ){
		return;
	}
	
	if( pDebugInfoCanvas->GetVisible() ){
		GetDebugTimerAt( 0 ).Reset();
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "Canvas.DrawCanvasRenderWorld" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const decVector2 &size = canvas.GetSize();
	deoglVR * const vr = camera->GetVR();
	
	if( vr ){
		vr->Render();
		
	}else{
		pActiveVAO = 0; // usually this will be trashed
		
		// determine the render size
		const decPoint intSize( size );
		int rwidth = intSize.x;
		int rheight = intSize.y;
		pWorldRenderSize( rwidth, rheight );
		
		// if zero do not try to render otherwise deoglDeferredRendering throws an exception
		if( rwidth == 0 || rheight == 0 ){
			return;
		}
		
// #define ENABLE_STEREO_RENDER_TEST 1
		
		// render using render plan
		deoglRenderPlan &plan = camera->GetPlan();
		
		plan.SetRenderVR( deoglRenderPlan::ervrNone );
		plan.SetViewport( rwidth, rheight );
		plan.SetUpscaleSize( intSize.x, intSize.y );
		plan.SetUseUpscaling( rwidth != intSize.x || rheight != intSize.y );
		plan.SetUpsideDown( false );
		plan.SetLodMaxPixelError( config.GetLODMaxPixelError() );
		plan.SetLodLevelOffset( 0 );
		plan.SetRenderStereo( false );
#ifdef ENABLE_STEREO_RENDER_TEST
		plan.SetRenderStereo( true );
		plan.SetCameraStereoMatrix( decDMatrix::CreateTranslation( -0.1, 0, 0 ) );
#endif
		
		const deoglDeveloperMode &devmode = renderThread.GetDebug().GetDeveloperMode();
		plan.SetDebugTiming( ! context.GetFBO() && devmode.GetEnabled() && devmode.GetShowDebugInfo() );
		
		// decTimer timer;
		plan.PrepareRender( context.GetRenderPlanMask() );
		// renderThread.GetLogger().LogInfoFormat("PrepareRender %d", (int)(timer.GetElapsedTime()*1e6f));
		
		defren.Resize( rwidth, rheight );
#ifdef ENABLE_STEREO_RENDER_TEST
		defren.Resize( rwidth, rheight, 2 );
#endif
		plan.Render();
		// pRenderThread.GetLogger().LogInfoFormat("Render %d", (int)(timer.GetElapsedTime()*1e6f));
	}
	
	// revert back to 2d rendering
	// TODO time finalize pass too
	renderThread.GetFramebuffer().Activate( context.GetFBO() );
	
	const deoglPipeline &pipeline = context.GetMask()
		? ( vr
			? *pPipelineCanvasImageMask[ canvas.GetBlendMode() ]
			: *pPipelineCanvasRenderWorldMask[ canvas.GetBlendMode() ] )
		: ( vr
			? *pPipelineCanvasImage[ canvas.GetBlendMode() ]
			: *pPipelineCanvasRenderWorld[ canvas.GetBlendMode() ] );
	pipeline.Activate();
	
	Prepare( context );
	
	// render finalize pass into canvas space with all the bells and whistles
	if( vr ){
		deoglRenderTarget * const rteye = vr->GetLeftEye().GetRenderTarget();
		tsmgr.EnableTexture( 0, rteye ? *rteye->GetTexture()
			: *pRenderThread.GetDefaultTextures().GetColor(), GetSamplerClampLinear() );
		
	}else{
		tsmgr.EnableArrayTexture( 0, *defren.GetPostProcessTexture(), GetSamplerClampLinear() );
	}
	
	if( context.GetMask() ){
		tsmgr.EnableTexture( 1, *context.GetMask(), GetSamplerClampLinear() );
	}
	
	const decTexMatrix2 billboardTransform( decTexMatrix2::CreateScale( size ) );
	const float transparency = context.GetTransparency();
	
	deoglShaderCompiled &shader = pipeline.GetShader();
	
	shader.SetParameterTexMatrix3x2( spcTransform, billboardTransform * context.GetTransform() );
	
	if( vr ){
		const decVector2 &from = vr->GetLeftEye().GetCanvasTCFrom();
		const decVector2 &to = vr->GetLeftEye().GetCanvasTCTo();
		shader.SetParameterTexMatrix3x2( spcTCTransform, decTexMatrix2::CreateST(
			to.x - from.x, from.y - to.y, from.x, 1.0f - from.y ) );
		
	}else{
		shader.SetParameterTexMatrix3x2( spcTCTransform, decTexMatrix2::CreateST(
			defren.GetScalingU(), -defren.GetScalingV(), 0.0f, defren.GetScalingV() ) );
	}
	
	shader.SetParameterTexMatrix3x2( spcTCTransformMask, context.GetTCTransformMask() );
	
	// color correction from configuration applied over canvas color transformation
	const float gamma = 1.0f / ( OGL_RENDER_GAMMA * config.GetGammaCorrection() );
	decColorMatrix colorTransform;
	
	if( ! vr ){
		colorTransform *= decColorMatrix::CreateContrast( config.GetContrast() );
		colorTransform *= decColorMatrix::CreateBrightness( config.GetBrightness() );
	}
	
	colorTransform *= decColorMatrix::CreateScaling( 1.0f, 1.0f, 1.0f, transparency );
	colorTransform *= context.GetColorTransform();
	
	shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransform );
	
	if( ! vr || ! vr->GetLeftEye().GetUseGammaCorrection() ){
		shader.SetParameterFloat( spcGamma, gamma, gamma, gamma, 1.0f );
		
	}else{
		shader.SetParameterFloat( spcGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	}
	
	// set clipping
	shader.SetParameterFloat( spcClipRect,
		( context.GetClipMin().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMin().y + 1.0f ) * context.GetClipFactor().y,
		( context.GetClipMax().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMax().y + 1.0f ) * context.GetClipFactor().y );
	
	shader.SetParameterFloat( spcTCClamp,
		context.GetTCClampMinimum().x, context.GetTCClampMinimum().y,
		context.GetTCClampMaximum().x, context.GetTCClampMaximum().y );
	
	// render finalize of world into canvas
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, OFFSET_RECT, COUNT_RECT ) );
	
	// clean up
	tsmgr.DisableStage( 0 ); // TODO is this really required?
	
	if( pDebugInfoCanvas->GetVisible() ){
		// ATTENTION RenderWorld also does printing developer mode information. The mearures time
		//           for Canvas and RenderThread are going to be higher by 4-6ms due to this
		pDebugTimeCanvasRenderWorld += GetDebugTimerAt( 0 ).GetElapsedTime();
		pDebugCountCanvasRenderWorld++;
	}
}



void deoglRenderCanvas::DebugInfoCanvasReset(){
	if( ! pDebugInfoCanvas->GetVisible() ){
		return;
	}
	
	pDebugTimeCanvasView = 0.0f;
	pDebugCountCanvasView = 0;
	pDebugTimeCanvasImage = 0.0f;
	pDebugCountCanvasImage = 0;
	pDebugTimeCanvasPaint = 0.0f;
	pDebugCountCanvasPaint = 0;
	pDebugTimeCanvasRenderWorld = 0.0f;
	pDebugCountCanvasRenderWorld = 0;
	pDebugTimeCanvasText = 0.0f;
	pDebugCountCanvasText = 0;
	pDebugTimeCanvasVideoPlayer = 0.0f;
	pDebugCountCanvasVideoPlayer = 0;
	pDebugTimeCanvasCanvasView = 0.0f;
	pDebugCountCanvasCanvasView = 0;
}

void deoglRenderCanvas::DebugInfoCanvasUpdate(){
	if( ! pDebugInfoCanvas->GetVisible() ){
		return;
	}
	
	pDebugInfoCanvas->Clear();
	pDebugInfoCanvas->IncrementElapsedTime( pDebugTimeCanvasView + pDebugTimeCanvasImage
		+ pDebugTimeCanvasPaint + pDebugTimeCanvasRenderWorld + pDebugTimeCanvasText
		+ pDebugTimeCanvasVideoPlayer + pDebugTimeCanvasCanvasView );
	
	pDebugInfoCanvasView->Clear();
	pDebugInfoCanvasView->IncrementElapsedTime( pDebugTimeCanvasView );
	pDebugInfoCanvasView->IncrementCounter( pDebugCountCanvasView );
	
	pDebugInfoCanvasImage->Clear();
	pDebugInfoCanvasImage->IncrementElapsedTime( pDebugTimeCanvasImage );
	pDebugInfoCanvasImage->IncrementCounter( pDebugCountCanvasImage );
	
	pDebugInfoCanvasPaint->Clear();
	pDebugInfoCanvasPaint->IncrementElapsedTime( pDebugTimeCanvasPaint );
	pDebugInfoCanvasPaint->IncrementCounter( pDebugCountCanvasPaint );
	
	pDebugInfoCanvasRenderWorld->Clear();
	pDebugInfoCanvasRenderWorld->IncrementElapsedTime( pDebugTimeCanvasRenderWorld );
	pDebugInfoCanvasRenderWorld->IncrementCounter( pDebugCountCanvasRenderWorld );
	
	pDebugInfoCanvasText->Clear();
	pDebugInfoCanvasText->IncrementElapsedTime( pDebugTimeCanvasText );
	pDebugInfoCanvasText->IncrementCounter( pDebugCountCanvasText );
	
	pDebugInfoCanvasVideoPlayer->Clear();
	pDebugInfoCanvasVideoPlayer->IncrementElapsedTime( pDebugTimeCanvasVideoPlayer );
	pDebugInfoCanvasVideoPlayer->IncrementCounter( pDebugCountCanvasVideoPlayer );
	
	pDebugInfoCanvasCanvasView->Clear();
	pDebugInfoCanvasCanvasView->IncrementElapsedTime( pDebugTimeCanvasCanvasView );
	pDebugInfoCanvasCanvasView->IncrementCounter( pDebugCountCanvasCanvasView );
}



void deoglRenderCanvas::ClearAllDebugInfoPlanPrepare( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	
	pDebugInfoPlanPrepare->Clear();
	pDebugInfoPlanPrepareEarlyWorld->Clear();
	pDebugInfoPlanPrepareFindContent->Clear();
	pDebugInfoPlanPrepareBuildRTs->Clear();
	pDebugInfoPlanPrepareSkyLightFindContent->Clear();
	pDebugInfoPlanPrepareSkyLightBuildRT->Clear();
	pDebugInfoPlanPrepareSkyLightGIFindContent->Clear();
	pDebugInfoPlanPrepareSkyLightGIUpdateRenderTask->Clear();
	pDebugInfoPlanPrepareWorld->Clear();
	pDebugInfoPlanPrepareGIUpdate->Clear();
	pDebugInfoPlanPrepareCulling->Clear();
	pDebugInfoPlanPrepareEnvMaps->Clear();
	pDebugInfoPlanPreparePrepareContent->Clear();
	pDebugInfoPlanPrepareHTViewVBOs->Clear();
	pDebugInfoPlanPrepareBuildPlan->Clear();
	pDebugInfoPlanPrepareLights->Clear();
	pDebugInfoPlanPrepareFinish->Clear();
	
	DebugTimersReset( plan, false );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepare( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer1Sample( plan, *pDebugInfoPlanPrepare, false );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareEarlyWorld( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareEarlyWorld, false );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareFindContent( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareFindContent, false );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareFindContent( deoglRenderPlan &plan, float elapsed ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimerIncrement( plan, *pDebugInfoPlanPrepareFindContent, elapsed, 0 );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareBuildRTs( deoglRenderPlan &plan, float elapsed ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimerIncrement( plan, *pDebugInfoPlanPrepareBuildRTs, elapsed, 0 );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareSkyLightFindContent( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareSkyLightFindContent, false );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareSkyLightFindContent( deoglRenderPlan &plan, float elapsed ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimerIncrement( plan, *pDebugInfoPlanPrepareSkyLightFindContent, elapsed, 1 );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareSkyLightBuildRT( deoglRenderPlan &plan, float elapsed ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimerIncrement( plan, *pDebugInfoPlanPrepareSkyLightBuildRT, elapsed, 1 );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareSkyLightGIFindContent( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareSkyLightGIFindContent, false );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareSkyLightGIFindContent( deoglRenderPlan &plan, float elapsed ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimerIncrement( plan, *pDebugInfoPlanPrepareSkyLightGIFindContent, elapsed, 1 );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareSkyLightGIUpdateRenderTask( deoglRenderPlan &plan, float elapsed ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimerIncrement( plan, *pDebugInfoPlanPrepareSkyLightGIUpdateRenderTask, elapsed, 1 );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareWorld( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareWorld, false );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareGIUpdate( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareGIUpdate, true );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareCulling( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareCulling, true );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareEnvMaps( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareEnvMaps, true );
}

void deoglRenderCanvas::SampleDebugInfoPlanPreparePrepareContent( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPreparePrepareContent, true );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareHTViewVBOs( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer3Sample( plan, *pDebugInfoPlanPrepareHTViewVBOs, true );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareBuildPlan( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareBuildPlan, false );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareLights( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareLights, true );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareFinish( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareFinish, true );
}



void deoglRenderCanvas::AddTopLevelDebugInfo(){
	deoglDebugInformationList &list = GetRenderThread().GetDebug().GetDebugInformationList();
	list.Add( pDebugInfoPlanPrepare );
	list.Add( pDebugInfoCanvas );
}

void deoglRenderCanvas::DevModeDebugInfoChanged(){
	const int details = GetRenderThread().GetDebug().GetDeveloperMode().GetDebugInfoDetails();
	
	pDebugInfoCanvas->SetVisible( ( details & deoglDeveloperMode::edimCanvas ) == deoglDeveloperMode::edimCanvas );
	pDebugInfoPlanPrepare->SetVisible( ( details & deoglDeveloperMode::edimPlanPrepare ) == deoglDeveloperMode::edimPlanPrepare );
}



// Private Functions
//////////////////////

void deoglRenderCanvas::pCleanUp(){
	deoglDelayedOperations &dops = GetRenderThread().GetDelayedOperations();
	dops.DeleteOpenGLVertexArray( pVAOShapes );
	dops.DeleteOpenGLBuffer( pVBOShapes );
	
	deoglDebugInformationList &dilist = GetRenderThread().GetDebug().GetDebugInformationList();
	dilist.RemoveIfPresent( pDebugInfoCanvas );
	dilist.RemoveIfPresent( pDebugInfoPlanPrepare );
}

void deoglRenderCanvas::pCreateShapesVAO(){
	OGL_IF_CHECK( deoglRenderThread &renderThread = GetRenderThread() );
	GLfloat vbodata[ 14 ];
	
	// the actual geometry points are obtain by calculating x'=x*scale+offset.
	
	// set up vbo data for a point shape
	vbodata[ 0 ] = 0.0f; // p1.x = x
	vbodata[ 1 ] = 0.0f; // p1.y = y
	
	// set up vbo data for a line shape
	vbodata[ 2 ] = 0.0f; // p1.x = x1
	vbodata[ 3 ] = 0.0f; // p1.y = y1
	
	vbodata[ 4 ] = 1.0f; // p2.x = x2
	vbodata[ 5 ] = 1.0f; // p2.y = y2
	
	// set up vbo data for a rectangular shape
	vbodata[ 6 ] = 0.0f; // p1.x = x1
	vbodata[ 7 ] = 0.0f; // p1.y = y1
	
	vbodata[ 8 ] = 0.0f; // p2.x = x1
	vbodata[ 9 ] = 1.0f; // p2.y = y2
	
	vbodata[ 10 ] = 1.0f; // p3.x = x2
	vbodata[ 11 ] = 1.0f; // p3.y = y2
	
	vbodata[ 12 ] = 1.0f; // p4.x = x2
	vbodata[ 13 ] = 0.0f; // p4.y = y1
	
	// create vbo and vao
	OGL_CHECK( renderThread, pglGenVertexArrays( 1, &pVAOShapes ) );
	if( ! pVAOShapes ){
		DETHROW( deeOutOfMemory );
	}
	OGL_CHECK( renderThread, pglBindVertexArray( pVAOShapes ) );
	
	OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBOShapes ) );
	if( ! pVBOShapes ){
		DETHROW( deeOutOfMemory );
	}
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOShapes ) );
	OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, sizeof( vbodata ), ( const GLvoid * )&vbodata, GL_STATIC_DRAW ) );
	
	OGL_CHECK( renderThread, pglEnableVertexAttribArray( 0 ) );
	OGL_CHECK( renderThread, pglVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, ( const GLvoid * )0 ) );
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
//ogl.LogInfoFormat( "render 2d: size=%i vbo=%u vao=%u", sizeof(vbodata), pVBOShapes, pVAOShapes );
}

void deoglRenderCanvas::pWorldRenderSize( int &width, int &height ) const{
	// adjust render size if half size rendering is used
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	
	width /= config.GetRenderDownScale();
	height /= config.GetRenderDownScale();
	
	// limit the size if requested
	int limitSize = config.GetDefRenSizeLimit();
	
	if( limitSize > 0 ){
		// since anything above 0 is a limit value the user can set some really silly limits.
		// let us avoid anything below 256 as this makes no sense to go that low
		if( limitSize < 256 ){
			limitSize = 256;
		}
		
		if( width > height ){
			if( width > limitSize ){
				height = ( int )( ( float )height * ( ( float )limitSize / ( float )width ) );
				width = limitSize;
			}
			
		}else{
			if( height > limitSize ){
				width = ( int )( ( float )width * ( ( float )limitSize / ( float )height ) );
				height = limitSize;
			}
		}
	}
}

void deoglRenderCanvas::pActivateVAOShapes(){
	if( pActiveVAO == pVAOShapes ){
		return;
	}
	
	OGL_CHECK( GetRenderThread(), pglBindVertexArray( pVAOShapes ) );
	pActiveVAO = pVAOShapes;
}

void deoglRenderCanvas::pCreatePipelines(
const deoglPipeline* (&pipelines)[deoglRCanvas::BlendModeCount], deoglPipelineConfiguration &config,
const deoglShaderSources *sources, const deoglShaderDefines &defines){
	config.EnableBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	pAsyncGetPipeline(pipelines[deCanvas::ebmBlend], config, sources, defines);
	
	config.EnableBlend( GL_SRC_ALPHA , GL_ONE );
	pAsyncGetPipeline(pipelines[deCanvas::ebmAdd], config, sources, defines);
}
