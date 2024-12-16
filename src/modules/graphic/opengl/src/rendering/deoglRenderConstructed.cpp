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

#include "deoglRenderConstructed.h"
#include "deoglRenderCanvasContext.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlan.h"
#include "../configuration/deoglConfiguration.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../rendering/deoglRenderWorld.h"
#include "../renderthread/deoglRenderThread.h"
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
#include "../skin/state/node/deoglSkinStateCNGroup.h"
#include "../skin/state/node/deoglSkinStateCNImage.h"
#include "../skin/state/node/deoglSkinStateCNShape.h"
#include "../skin/state/node/deoglSkinStateCNText.h"
#include "../skin/deoglSkinTexture.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglSharedVBOListList.h"

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



// Class deoglRenderConstructed
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderConstructed::deoglRenderConstructed( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread ),
pVBOShapes( 0 ),
pVAOShapes( 0 ),
pActiveVAO( 0 )
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
		pipconf.SetShader( renderThread, sources, defines );
		pCreatePipelines( pPipelineCanvasColor, pipconf );
		
		defines.SetDefines( "WITH_TEXTURE" );
		pipconf.SetShader( renderThread, sources, defines );
		pCreatePipelines( pPipelineCanvasImage, pipconf );
		
		defines = commonDefines;
		defines.SetDefines( "WITH_MASK" );
		pipconf.SetShader( renderThread, sources, defines );
		pCreatePipelines( pPipelineCanvasColorMask, pipconf );
		
		defines.SetDefines( "WITH_TEXTURE" );
		pipconf.SetShader( renderThread, sources, defines );
		pCreatePipelines( pPipelineCanvasImageMask, pipconf );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderConstructed::~deoglRenderConstructed(){
	pCleanUp();
}



// Management
///////////////

void deoglRenderConstructed::Prepare( const deoglRenderCanvasContext &context ){
	deoglRenderThread &renderThread = GetRenderThread();
	
	renderThread.GetBufferObject().GetSharedVBOListForType( deoglRTBufferObject::esvbolCanvasPaint ).PrepareVBOs();
	renderThread.GetTexture().GetStages().DisableAllStages();
	SetViewport( context.GetViewportOffset(), context.GetViewportSize() );
	
	OGL_CHECK( renderThread, pglBindVertexArray( pVAOShapes ) );
	pActiveVAO = pVAOShapes;
}



void deoglRenderConstructed::DrawNodeShape( const deoglRenderCanvasContext &context, const deoglSkinStateCNShape &node ){
	if( ! ( node.GetSize() > decPoint3() ) ){
		return; // too small
	}
	if( node.GetDrawCountFill() == 0 && node.GetDrawCountLine() == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	const float transparency = context.GetTransparency();
	const float thickness = decMath::max( 0.0f, node.GetThickness() );
	
	const deoglPipeline &pipeline = context.GetMask()
		? *pPipelineCanvasColorMask[ node.GetCombineMode() ]
		: *pPipelineCanvasColor[ node.GetCombineMode() ];
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
	shader.SetParameterFloat( spcGamma, node.GetGamma(), node.GetGamma(), node.GetGamma(), 1.0f );
	
	deoglSharedVBOBlock &vboBlock = *node.GetVBOBlock();
	const int vboOffset = vboBlock.GetOffset();
	
	const GLuint vao = vboBlock.GetVBO()->GetVAO()->GetVAO();
	OGL_CHECK( GetRenderThread(), pglBindVertexArray( vao ) );
	pActiveVAO = vao;
	
	// fill shape if fill color is not transparent
	const decColor colorFill( node.GetFillColor(), node.GetFillColor().a * transparency );
	const decColorMatrix colorTransformFill( decColorMatrix::CreateScaling( colorFill ) * context.GetColorTransform() );
	
	if( node.GetDrawCountFill() > 0 && colorFill.a > FLOAT_SAFE_EPSILON ){
		shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransformFill );
		
		OGL_CHECK( renderThread, glDrawArrays( node.GetDrawModeFill(),
			vboOffset + node.GetDrawOffsetFill(), node.GetDrawCountFill() ) );
	}
	
	// outline shape if line color is not transparent
	const decColor colorLine( node.GetLineColor(), node.GetLineColor().a * transparency );
	
	if( node.GetDrawCountLine() > 0 && colorLine.a > FLOAT_SAFE_EPSILON && thickness > 0.1f ){
		const decColorMatrix colorTransformLine( decColorMatrix::CreateScaling( colorLine ) * context.GetColorTransform() );
		shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransformLine );
		
		OGL_CHECK( renderThread, glDrawArrays( node.GetDrawModeLine(),
			vboOffset + node.GetDrawOffsetLine(), node.GetDrawCountLine() ) );
	}
}

void deoglRenderConstructed::DrawNodeImage( const deoglRenderCanvasContext &context, const deoglSkinStateCNImage &node ){
	const deoglRImage::Ref &image = node.GetImage();
	if( ! image || ! image->GetTexture() ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	pActivateVAOShapes();
	
	const deoglPipeline &pipeline = context.GetMask()
		? *pPipelineCanvasImageMask[ node.GetCombineMode() ]
		: *pPipelineCanvasImage[ node.GetCombineMode() ];
	pipeline.Activate();
	
	tsmgr.EnableTexture( 0, *image->GetTexture(), GetSamplerRepeatLinear() );
	if( context.GetMask() ){
		tsmgr.EnableTexture( 1, *context.GetMask(), GetSamplerClampLinear() );
	}
	
	const float transparency = context.GetTransparency();
	const decColorMatrix colorTransform( decColorMatrix::CreateScaling(
		1.0f, 1.0f, 1.0f, transparency ) * context.GetColorTransform() );
	
	const decTexMatrix2 billboardTransform( decTexMatrix2::CreateScale(
		decVector2( node.GetSize().x, node.GetSize().y ) ) );
	
	deoglShaderCompiled &shader = pipeline.GetShader();
	
	shader.SetParameterTexMatrix3x2( spcTransform, billboardTransform * context.GetTransform() );
	shader.SetParameterTexMatrix3x2( spcTCTransform, node.GetTCTransform() );
	shader.SetParameterTexMatrix3x2( spcTCTransformMask, context.GetTCTransformMask() );
	shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransform );
	shader.SetParameterFloat( spcGamma, node.GetGamma(), node.GetGamma(), node.GetGamma(), 1.0f );
	
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
}

void deoglRenderConstructed::DrawNodeText( const deoglRenderCanvasContext &context, const deoglSkinStateCNText &node ){
	const deoglRFont::Ref &font = node.GetFont();
	if( ! font ){
		return;
	}
	
	const deoglRImage::Ref &image = font->GetImage();
	if( ! image || ! image->GetTexture() ){
		return;
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
		? *pPipelineCanvasImageMask[ node.GetCombineMode() ]
		: *pPipelineCanvasImage[ node.GetCombineMode() ];
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
		const decColor color( node.GetColor(), node.GetColor().a * transparency );
		const decColorMatrix colorTransform( decColorMatrix::CreateScaling( color ) * context.GetColorTransform() );
		shader.SetParameterColorMatrix5x4( spcColorTransform, spcColorTransform2, colorTransform );
	}
	
	shader.SetParameterFloat( spcGamma, node.GetGamma(), node.GetGamma(), node.GetGamma(), 1.0f );
	
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
	
	utf8Decoder.SetString( node.GetText() );
	const int len = utf8Decoder.GetLength();
	const float fontScale = node.GetFontSize() / ( float )font->GetLineHeight();
	
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
}



// Private Functions
//////////////////////

void deoglRenderConstructed::pCleanUp(){
	deoglDelayedOperations &dops = GetRenderThread().GetDelayedOperations();
	dops.DeleteOpenGLVertexArray( pVAOShapes );
	dops.DeleteOpenGLBuffer( pVBOShapes );
}

void deoglRenderConstructed::pCreateShapesVAO(){
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

void deoglRenderConstructed::pActivateVAOShapes(){
	if( pActiveVAO == pVAOShapes ){
		return;
	}
	
	OGL_CHECK( GetRenderThread(), pglBindVertexArray( pVAOShapes ) );
	pActiveVAO = pVAOShapes;
}

void deoglRenderConstructed::pCreatePipelines( const deoglPipeline* (&pipelines)[ 2 ], deoglPipelineConfiguration &config ){
	deoglPipelineManager &pipelineManager = GetRenderThread().GetPipelineManager();
	
	config.EnableBlend( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	pipelines[ deSkinPropertyNode::ecmBlend ] = pipelineManager.GetWith( config );
	
	config.EnableBlend( GL_SRC_ALPHA , GL_ONE );
	pipelines[ deSkinPropertyNode::ecmOverlay ] = pipelineManager.GetWith( config );
}
