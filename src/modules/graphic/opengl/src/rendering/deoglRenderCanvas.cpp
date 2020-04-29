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
#include "../devmode/deoglDeveloperMode.h"
#include "../debug/deoglDebugInformation.h"
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
	spcTCClamp
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

pShaderCanvasColor( NULL ),
pShaderCanvasImage( NULL ),

pDebugInfoCanvas( NULL ),
pDebugInfoCanvasView( NULL ),
pDebugInfoCanvasImage( NULL ),
pDebugInfoCanvasPaint( NULL ),
pDebugInfoCanvasRenderWorld( NULL ),
pDebugInfoCanvasText( NULL ),
pDebugInfoCanvasVideoPlayer( NULL ),
pDebugInfoCanvasCanvasView( NULL ),

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
pDebugCountCanvasCanvasView( 0 ),

pDebugInfoPlanPrepare( NULL ),
pDebugInfoPlanPrepareCollect( NULL ),
pDebugInfoPlanPrepareCulling( NULL ),
pDebugInfoPlanPrepareEnvMaps( NULL ),
pDebugInfoPlanPrepareHTViewVBOs( NULL ),
pDebugInfoPlanPrepareComponents( NULL ),
pDebugInfoPlanPrepareComponentsVBO( NULL ),
pDebugInfoPlanPrepareComponentsRenderables( NULL ),
pDebugInfoPlanPrepareSort( NULL ),
pDebugInfoPlanPrepareBuildPlan( NULL ),
pDebugInfoPlanPrepareLights( NULL )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		pCreateShapesVAO();
		
		sources = shaderManager.GetSourcesNamed( "Canvas" );
		pShaderCanvasColor = shaderManager.GetProgramWith( sources, defines );
		
		defines.AddDefine( "WITH_TEXTURE", "1" );
		pShaderCanvasImage = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		// debug information
		const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
		const decColor colorBg( 0.0f, 0.0f, 0.0f, 0.75f );
		const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
		
		pDebugInfoCanvas = new deoglDebugInformation( "Canvas", colorText, colorBg );
		
		pDebugInfoCanvasView = new deoglDebugInformation( "View", colorText, colorBgSub );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasView );
		
		pDebugInfoCanvasImage = new deoglDebugInformation( "Image", colorText, colorBgSub );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasImage );
		
		pDebugInfoCanvasPaint = new deoglDebugInformation( "Paint", colorText, colorBgSub );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasPaint );
		
		pDebugInfoCanvasRenderWorld = new deoglDebugInformation( "Render World", colorText, colorBgSub );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasRenderWorld );
		
		pDebugInfoCanvasText = new deoglDebugInformation( "Text", colorText, colorBgSub );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasText );
		
		pDebugInfoCanvasVideoPlayer = new deoglDebugInformation( "Video Player", colorText, colorBgSub );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasVideoPlayer );
		
		pDebugInfoCanvasCanvasView = new deoglDebugInformation( "Canvas View", colorText, colorBgSub );
		pDebugInfoCanvas->GetChildren().Add( pDebugInfoCanvasCanvasView );
		
		
		
		pDebugInfoPlanPrepare = new deoglDebugInformation( "Plan Prepare", colorText, colorBg );
		
		pDebugInfoPlanPrepareCollect = new deoglDebugInformation( "Collect", colorText, colorBgSub );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareCollect );
		
		pDebugInfoPlanPrepareCulling = new deoglDebugInformation( "Culling", colorText, colorBgSub );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareCulling );
		
		pDebugInfoPlanPrepareEnvMaps = new deoglDebugInformation( "Env-Maps", colorText, colorBgSub );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareEnvMaps );
		
		pDebugInfoPlanPrepareHTViewVBOs = new deoglDebugInformation( "HT-View VBOs", colorText, colorBgSub );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareHTViewVBOs );
		
		pDebugInfoPlanPrepareComponents = new deoglDebugInformation( "Components", colorText, colorBgSub );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareComponents );
			
			pDebugInfoPlanPrepareComponentsVBO = new deoglDebugInformation( "VBO", colorText, colorBgSub2 );
			pDebugInfoPlanPrepareComponents->GetChildren().Add( pDebugInfoPlanPrepareComponentsVBO );
			
			pDebugInfoPlanPrepareComponentsRenderables = new deoglDebugInformation( "Renderables", colorText, colorBgSub2 );
			pDebugInfoPlanPrepareComponents->GetChildren().Add( pDebugInfoPlanPrepareComponentsRenderables );
		
		pDebugInfoPlanPrepareSort = new deoglDebugInformation( "Sort", colorText, colorBgSub );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareSort );
		
		pDebugInfoPlanPrepareBuildPlan = new deoglDebugInformation( "Build Plan", colorText, colorBgSub );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareBuildPlan );
		
		pDebugInfoPlanPrepareLights = new deoglDebugInformation( "Lights", colorText, colorBgSub );
		pDebugInfoPlanPrepare->GetChildren().Add( pDebugInfoPlanPrepareLights );
		
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
	
	renderThread.GetBufferObject().GetSharedVBOListForType(
		deoglRTBufferObject::esvbolCanvasPaint ).PrepareVBOs();
	
	// disable all textures to start with a fresh slate
	renderThread.GetTexture().GetStages().DisableAllStages();
	
	// set viewport and scissor box
	const decPoint &viewportOffset = context.GetViewportOffset();
	const decPoint &viewportSize = context.GetViewportSize();
	
	OGL_CHECK( renderThread, glViewport( viewportOffset.x, viewportOffset.y, viewportSize.x, viewportSize.y ) );
	
	OGL_CHECK( renderThread, glScissor( viewportOffset.x, viewportOffset.y, viewportSize.x, viewportSize.y ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	
	// disable depth test. render order takes care of overlap
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	// disable culling. allows flipped canvas without extra work
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	// enable blending with basic src-alpha blend function
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	pBlendSrc = GL_SRC_ALPHA;
	pBlendDest = GL_ONE_MINUS_SRC_ALPHA;
	
	// enable color writing to all channels
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	// enable the shapes vao
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
	
	decTexMatrix2 transform( context.GetTransform() );
	const float transparency = context.GetTransparency();
	const float thickness = decMath::max( 0.0f, canvas.GetThickness() );
	
	pSetBlendMode( canvas.GetBlendSrc(), canvas.GetBlendDest() );
	
	renderThread.GetShader().ActivateShader( pShaderCanvasColor );
	deoglShaderCompiled &shader = *pShaderCanvasColor->GetCompiled();
	
	shader.SetParameterFloat( spcClipRect,
		( context.GetClipMin().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMin().y + 1.0f ) * context.GetClipFactor().y,
		( context.GetClipMax().x + 1.0f ) * context.GetClipFactor().x,
		( context.GetClipMax().y + 1.0f ) * context.GetClipFactor().y );
	
	shader.SetParameterFloat( spcTCClamp,
		context.GetTCClampMinimum().x, context.GetTCClampMinimum().y,
		context.GetTCClampMaximum().x, context.GetTCClampMaximum().y );
	
	shader.SetParameterFloat( spcGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	
	shader.SetParameterTexMatrix3x2( spcTransform, transform );
	
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
	pSetBlendMode( canvas.GetBlendSrc(), canvas.GetBlendDest() );
	
	tsmgr.EnableTexture( 0, *image->GetTexture(), GetSamplerRepeatLinear() );
	
	const decTexMatrix2 &transform = context.GetTransform();
	const decTexMatrix2 &tctransform = canvas.GetTCTransform();
	const float transparency = context.GetTransparency();
	const decColorMatrix colorTransform( decColorMatrix::CreateScaling(
		1.0f, 1.0f, 1.0f, transparency ) * context.GetColorTransform() );
	
	const decTexMatrix2 billboardTransform( decTexMatrix2::CreateScale( canvas.GetSize() ) );
	
	renderThread.GetShader().ActivateShader( pShaderCanvasImage );
	deoglShaderCompiled &shader = *pShaderCanvasImage->GetCompiled();
	
	shader.SetParameterTexMatrix3x2( spcTransform, billboardTransform * transform );
	shader.SetParameterTexMatrix3x2( spcTCTransform, tctransform );
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
	pSetBlendMode( canvas.GetBlendSrc(), canvas.GetBlendDest() );
	
	tsmgr.EnableTexture( 0, *renderTarget->GetTexture(), GetSamplerRepeatLinear() );
	
	const decTexMatrix2 &transform = context.GetTransform();
	const decTexMatrix2 &tctransform = canvas.GetTCTransform();
	const float transparency = context.GetTransparency();
	const decColorMatrix colorTransform( decColorMatrix::CreateScaling( 1.0f, 1.0f, 1.0f, transparency ) * context.GetColorTransform() );
	
	const decTexMatrix2 billboardTransform( decTexMatrix2::CreateScale( canvas.GetSize() ) );
	
	renderThread.GetShader().ActivateShader( pShaderCanvasImage );
	deoglShaderCompiled &shader = *pShaderCanvasImage->GetCompiled();
	
	shader.SetParameterTexMatrix3x2( spcTransform, billboardTransform * transform );
	shader.SetParameterTexMatrix3x2( spcTCTransform, tctransform );
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
	pSetBlendMode( canvas.GetBlendSrc(), canvas.GetBlendDest() );
	
	tsmgr.EnableTexture( 0, *videoPlayer->GetTexture(), GetSamplerRepeatLinear() );
	
	const decTexMatrix2 &transform = context.GetTransform();
	const decTexMatrix2 &tctransform = canvas.GetTCTransform();
	const float transparency = context.GetTransparency();
	const decColorMatrix colorTransform( decColorMatrix::CreateScaling( 1.0f, 1.0f, 1.0f, transparency ) * context.GetColorTransform() );
	
	const decTexMatrix2 billboardTransform( decTexMatrix2::CreateScale( canvas.GetSize() ) );
	
	renderThread.GetShader().ActivateShader( pShaderCanvasImage );
	deoglShaderCompiled &shader = *pShaderCanvasImage->GetCompiled();
	
	shader.SetParameterTexMatrix3x2( spcTransform, billboardTransform * transform );
	shader.SetParameterTexMatrix3x2( spcTCTransform, tctransform );
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
	pSetBlendMode( canvas.GetBlendSrc(), canvas.GetBlendDest() );
	
	// set texture
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	tsmgr.EnableTexture( 0, *image->GetTexture(), GetSamplerClampNearest() );
	
	// set shader
	renderThread.GetShader().ActivateShader( pShaderCanvasImage );
	deoglShaderCompiled &shader = *pShaderCanvasImage->GetCompiled();
	
	// set color
	const decTexMatrix2 &transform = context.GetTransform();
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
	
	// render text
	const deoglRFont::sGlyph * const oglGlyphs = font->GetGlyphs();
	decUTF8Decoder utf8Decoder;
	float curx = 0.0f;
	float cury = 0.0f;
	
	utf8Decoder.SetString( canvas.GetText() );
	const int len = utf8Decoder.GetLength();
	const float fontScale = canvas.GetFontSize() / ( float )font->GetLineHeight();
	
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
	
	pActiveVAO = 0; // usually this will be trashed
	
	// determine the render size
	const decVector2 &size = canvas.GetSize();
	const decPoint intSize( size );
	int rwidth = intSize.x;
	int rheight = intSize.y;
	pWorldRenderSize( rwidth, rheight );
	
	// if zero do not try to render otherwise deoglDeferredRendering throws an exception
	if( rwidth == 0 || rheight == 0 ){
		return;
	}
	
	// render using render plan
	deoglRenderPlan &plan = camera->GetPlan();
	
	plan.SetViewport( 0, 0, rwidth, rheight );
	plan.SetUpscaleSize( intSize.x, intSize.y );
	plan.SetUseUpscaling( rwidth != intSize.x || rheight != intSize.y );
	plan.SetUpsideDown( false );
	
	const deoglDeveloperMode &devmode = renderThread.GetDebug().GetDeveloperMode();
	plan.SetDebugTiming( ! context.GetFBO() && devmode.GetEnabled() && devmode.GetShowDebugInfo() );
	
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	renworld.DebugTimer1Reset( plan, true );
	plan.PrepareRender();
	renworld.DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoPrepare, true );
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	defren.Resize( rwidth, rheight );
	plan.Render();
	
	// revert back to 2d rendering
	// TODO time finalize pass too
	renderThread.GetFramebuffer().Activate( context.GetFBO() );
	
	Prepare( context );
	
	// render finalize pass into canvas space with all the bells and whistles
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	tsmgr.EnableTexture( 0, *defren.GetPostProcessTexture(), GetSamplerClampLinear() );
	
	const decTexMatrix2 &transform = context.GetTransform();
	const decTexMatrix2 tctransform( decTexMatrix2::CreateST( defren.GetScalingU(), -defren.GetScalingV(), 0.0f, defren.GetScalingV() ) );
	const float transparency = context.GetTransparency();
	
	const decTexMatrix2 billboardTransform( decTexMatrix2::CreateScale( size ) );
	
	renderThread.GetShader().ActivateShader( pShaderCanvasImage );
	deoglShaderCompiled &shader = *pShaderCanvasImage->GetCompiled();
	
	shader.SetParameterTexMatrix3x2( spcTransform, billboardTransform * transform );
	shader.SetParameterTexMatrix3x2( spcTCTransform, tctransform );
	
	// color correction from configuration applied over canvas color transformation
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const float gamma = 1.0f / ( OGL_RENDER_GAMMA * config.GetGammaCorrection() );
	
	const decColorMatrix colorTransform(
		decColorMatrix::CreateContrast( config.GetContrast() ) *
		decColorMatrix::CreateBrightness( config.GetBrightness() ) *
		decColorMatrix::CreateScaling( 1.0f, 1.0f, 1.0f, transparency ) *
		context.GetColorTransform() );
	
	shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransform );
	shader.SetParameterFloat( spcGamma, gamma, gamma, gamma, 1.0f );
	
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
	pSetBlendModeForce( canvas.GetBlendSrc(), canvas.GetBlendDest() );
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, OFFSET_RECT, COUNT_RECT ) );
	
	// clean up
	tsmgr.DisableStage( 0 ); // TODO is this really required?
	
	if( pDebugInfoCanvas->GetVisible() ){
		// ATTENTION RenderWorld also does printing developer mode information. The mearures time
		//           for Canvas and RenderThread are going to be higher my 4-6ms due to this
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
	pDebugInfoPlanPrepareCollect->Clear();
	pDebugInfoPlanPrepareCulling->Clear();
	pDebugInfoPlanPrepareEnvMaps->Clear();
	pDebugInfoPlanPrepareHTViewVBOs->Clear();
	pDebugInfoPlanPrepareComponents->Clear();
	pDebugInfoPlanPrepareComponentsVBO->Clear();
	pDebugInfoPlanPrepareComponentsRenderables->Clear();
	pDebugInfoPlanPrepareSort->Clear();
	pDebugInfoPlanPrepareBuildPlan->Clear();
	pDebugInfoPlanPrepareLights->Clear();
	
	DebugTimersReset( plan, false );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepare( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer1Sample( plan, *pDebugInfoPlanPrepare, false );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareCollect( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareCollect, false );
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

void deoglRenderCanvas::SampleDebugInfoPlanPrepareHTViewVBOs( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareHTViewVBOs, true );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareComponents( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareComponents, true );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareComponentsVBO( deoglRenderPlan& plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer3SampleCount( plan, *pDebugInfoPlanPrepareComponentsVBO, 1, true );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareComponentsRenderables( deoglRenderPlan& plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer3SampleCount( plan, *pDebugInfoPlanPrepareComponentsRenderables, 1, true );
}

void deoglRenderCanvas::SampleDebugInfoPlanPrepareSort( deoglRenderPlan &plan ){
	if( ! plan.GetDebugTiming() || ! pDebugInfoPlanPrepare->GetVisible() ){
		return;
	}
	DebugTimer2Sample( plan, *pDebugInfoPlanPrepareSort, false );
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
	if( pShaderCanvasImage ){
		pShaderCanvasImage->RemoveUsage();
	}
	if( pShaderCanvasColor ){
		pShaderCanvasColor->RemoveUsage();
	}
	
	if( pVAOShapes ){
		pglDeleteVertexArrays( 1, &pVAOShapes );
	}
	if( pVBOShapes ){
		pglDeleteBuffers( 1, &pVBOShapes );
	}
	
	if( pDebugInfoCanvas ){
		GetRenderThread().GetDebug().GetDebugInformationList().RemoveIfPresent( pDebugInfoCanvas );
		pDebugInfoCanvas->FreeReference();
	}
	
	if( pDebugInfoCanvasView ){
		pDebugInfoCanvasView->FreeReference();
	}
	if( pDebugInfoCanvasImage ){
		pDebugInfoCanvasImage->FreeReference();
	}
	if( pDebugInfoCanvasPaint ){
		pDebugInfoCanvasPaint->FreeReference();
	}
	if( pDebugInfoCanvasRenderWorld ){
		pDebugInfoCanvasRenderWorld->FreeReference();
	}
	if( pDebugInfoCanvasText ){
		pDebugInfoCanvasText->FreeReference();
	}
	if( pDebugInfoCanvasVideoPlayer ){
		pDebugInfoCanvasVideoPlayer->FreeReference();
	}
	if( pDebugInfoCanvasCanvasView ){
		pDebugInfoCanvasCanvasView->FreeReference();
	}
	
	if( pDebugInfoPlanPrepare ){
		GetRenderThread().GetDebug().GetDebugInformationList().RemoveIfPresent( pDebugInfoPlanPrepare );
		pDebugInfoPlanPrepare->FreeReference();
	}
	if( pDebugInfoPlanPrepareCollect ){
		pDebugInfoPlanPrepareCollect->FreeReference();
	}
	if( pDebugInfoPlanPrepareCulling ){
		pDebugInfoPlanPrepareCulling->FreeReference();
	}
	if( pDebugInfoPlanPrepareEnvMaps ){
		pDebugInfoPlanPrepareEnvMaps->FreeReference();
	}
	if( pDebugInfoPlanPrepareHTViewVBOs ){
		pDebugInfoPlanPrepareHTViewVBOs->FreeReference();
	}
	if( pDebugInfoPlanPrepareComponents ){
		pDebugInfoPlanPrepareComponents->FreeReference();
	}
	if( pDebugInfoPlanPrepareSort ){
		pDebugInfoPlanPrepareSort->FreeReference();
	}
	if( pDebugInfoPlanPrepareBuildPlan ){
		pDebugInfoPlanPrepareBuildPlan->FreeReference();
	}
	if( pDebugInfoPlanPrepareLights ){
		pDebugInfoPlanPrepareLights->FreeReference();
	}
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

void deoglRenderCanvas::pSetBlendMode( GLenum blendSrc, GLenum blendDest ){
	if( blendSrc != pBlendSrc || blendDest != pBlendDest ){
		pSetBlendModeForce( blendSrc, blendDest );
	}
}

void deoglRenderCanvas::pSetBlendModeForce( GLenum blendSrc, GLenum blendDest ){
	OGL_CHECK( GetRenderThread(), glBlendFunc( blendSrc, blendDest ) );
	pBlendSrc = blendSrc;
	pBlendDest = blendDest;
}

void deoglRenderCanvas::pActivateVAOShapes(){
	if( pActiveVAO == pVAOShapes ){
		return;
	}
	
	OGL_CHECK( GetRenderThread(), pglBindVertexArray( pVAOShapes ) );
	pActiveVAO = pVAOShapes;
}
