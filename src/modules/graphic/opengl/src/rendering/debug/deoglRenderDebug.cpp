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

#include "deoglRenderDebug.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../../capabilities/deoglCapsTextureFormat.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../deoglDebugFont.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTBufferObject.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shapes/deoglShape.h"
#include "../../shapes/deoglShapeManager.h"
#include "../../texture/arraytexture/deoglArrayTexture.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../utils/collision/deoglDCollisionBox.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUTF8Decoder.h>



// Definitions
////////////////

enum eSPRender{
	sprMatrixMVP,
	sprColor,
	sprSCToDTC
};

enum eSPDebug{
	spdMatrixMVP,
	spdMatrixMV,
	spdScreenTransform,
	spdColorBegin,
	spdColorEnd,
	spdRadius
};

enum eSPOutTex{
	spotPosTransform,
	spotTCTransform,
	spotColor,
	spotGamma,
	spotLevel,
	spotLayer
};

enum eSPRectangle{
	sprectPosTransform,
	sprectColor
};



// Class deoglRenderDebug
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderDebug::deoglRenderDebug( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread ),

pShaderXRay( NULL ),
pShaderSolid( NULL ),

pShaderSphere( NULL ),

pShaderOutTex( NULL ),
pShaderOutTexLayer( NULL ),
pShaderOutArrTex( NULL ),

pShaderRenderText( NULL ),
pShaderRectangle( NULL ),

pDebugFont( NULL ),
pVBORenderTextData( NULL ),
pVBORenderTextDataCount( 0 ),
pVBORenderTextDataSize( 0 ),
pVBORenderText( 0 ),
pVAORenderText( 0 )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		sources = shaderManager.GetSourcesNamed( "DefRen Debug Color-Only" );
		if( defren.GetUseEncodedDepth() ){
			defines.AddDefine( "ENCODE_DEPTH", "1" );
		}
		if( defren.GetUseInverseDepth() ){
			defines.AddDefine( "INVERSE_DEPTH", "1" );
		}
		pShaderXRay = shaderManager.GetProgramWith( sources, defines );
		
		defines.AddDefine( "WITH_DEPTH", "1" );
		pShaderSolid = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Debug Sphere" );
		pShaderSphere = shaderManager.GetProgramWith( sources, defines );
		
		
		
		sources = shaderManager.GetSourcesNamed( "Debug Display Texture" );
		pShaderOutTex = shaderManager.GetProgramWith( sources, defines );
		
		defines.AddDefine( "TEXTURELEVEL", "1" );
		pShaderOutTexLayer = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		defines.AddDefine( "ARRAYTEXTURE", "1" );
		pShaderOutArrTex = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "Debug Render Text" );
		pShaderRenderText = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "Debug Rectangle" );
		defines.AddDefine( "NO_TCTRANSFORM", "1" );
		defines.AddDefine( "NO_TEXCOORD", "1" );
		pShaderRectangle = shaderManager.GetProgramWith( sources, defines );
		
		
		
		// create debug font
		pDebugFont = new deoglDebugFont( renderThread );
		
		// create render text vbo and vao
		OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBORenderText ) );
		if( ! pVBORenderText ){
			DETHROW( deeOutOfMemory );
		}
		
		OGL_CHECK( renderThread, pglGenVertexArrays( 1, &pVAORenderText ) );
		if( ! pVAORenderText ){
			DETHROW( deeOutOfMemory );
		}
		
		OGL_CHECK( renderThread, pglBindVertexArray( pVAORenderText ) );
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBORenderText ) );
		
		OGL_CHECK( renderThread, pglEnableVertexAttribArray( 0 ) ); // position
		OGL_CHECK( renderThread, pglVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE,
			sizeof( GLfloat ) * 8, ( const GLvoid * )( sizeof( GLfloat ) * 0 ) ) );
		
		OGL_CHECK( renderThread, pglEnableVertexAttribArray( 1 ) ); // texcoord
		OGL_CHECK( renderThread, pglVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE,
			sizeof( GLfloat ) * 8, ( const GLvoid * )( sizeof( GLfloat ) * 2 ) ) );
		
		OGL_CHECK( renderThread, pglEnableVertexAttribArray( 2 ) ); // color
		OGL_CHECK( renderThread, pglVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE,
			sizeof( GLfloat ) * 8, ( const GLvoid * )( sizeof( GLfloat ) * 4 ) ) );
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderDebug::~deoglRenderDebug(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderDebug::DisplayTexture( deoglRenderPlan &plan, deoglTexture *texture, bool gammaCorrect ){
	DisplayTextureLevel( plan, texture, 0, gammaCorrect );
}

void deoglRenderDebug::DisplayTextureLevel( deoglRenderPlan &plan, deoglTexture *texture, int level, bool gammaCorrect ){
	if( ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	int texWidth = texture->GetWidth();
	int texHeight = texture->GetHeight();
	int i;
	
	for( i=0; i<level; i++ ){
		texWidth >>= 1;
		if( texWidth < 1 ){
			texWidth = 1;
		}
		
		texHeight >>= 1;
		if( texHeight < 1 ){
			texHeight = 1;
		}
	}
	
	const int viewWidth = plan.GetViewportWidth();
	const int viewHeight = plan.GetViewportHeight();
	const float scaleX = ( float )texWidth / ( float )viewWidth;
	const float scaleY = ( float )texHeight / ( float )viewHeight;
	
	// about scaling:
	// input = [-1..1][-1..1]
	// output.x = -1 + ( input.x + 1 ) * ( tex.width / view.width )
	// output.x = ( input.x + 1 ) * ( tex.width / view.width ) - 1
	// output.x = input.x * ( tex.width / view.width ) + ( tex.width / view.width ) - 1
	// => scale.x = tex.width / view.width
	// => offset.x = ( tex.width / view.width ) - 1 = scale.x - 1
	
	// set opengl states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	// set shader and options
	renderThread.GetShader().ActivateShader( pShaderOutTexLayer );
	deoglShaderCompiled &shader = *pShaderOutTexLayer->GetCompiled();
	
	shader.SetParameterFloat( spotPosTransform, scaleX, scaleY, scaleX - 1.0f, scaleY - 1.0f );
	shader.SetParameterFloat( spotTCTransform, 0.5f, 0.5f, 0.5f, 0.5f );
	shader.SetParameterFloat( spotColor, 1.0f, 1.0f, 1.0f, 1.0f );
	shader.SetParameterFloat( spotLevel, ( float )level );
	
	if( gammaCorrect ){
		if( texture->GetFormat()->GetIsDepth() ){
			shader.SetParameterFloat( spotGamma, 1000.0f, 1000.0f, 1000.0f, 1.0f );
			
		}else{
			shader.SetParameterFloat( spotGamma, OGL_RENDER_GAMMA, OGL_RENDER_GAMMA, OGL_RENDER_GAMMA, 1.0f );
		}
		
	}else{
		shader.SetParameterFloat( spotGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	}
	
	// set texture
	tsmgr.EnableTexture( 0, *texture, GetSamplerClampNearestMipMap() );
	
	// render full screen quad
	defren.RenderFSQuadVAO();
	
	// cleanup
	tsmgr.DisableStage( 0 );
}

void deoglRenderDebug::DisplayArrayTextureLayer( deoglRenderPlan &plan, deoglArrayTexture *texture, int layer, bool gammaCorrect ){
	if( ! texture || layer < 0 || layer >= texture->GetLayerCount() ) DETHROW( deeInvalidParam );
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	int viewWidth = plan.GetViewportWidth();
	int viewHeight = plan.GetViewportHeight();
	int texWidth = texture->GetWidth();
	int texHeight = texture->GetHeight();
	float scaleX = ( float )texWidth / ( float )viewWidth;
	float scaleY = ( float )texHeight / ( float )viewHeight;
	
	// about scaling:
	// input = [-1..1][-1..1]
	// output.x = -1 + ( input.x + 1 ) * ( tex.width / view.width )
	// output.x = ( input.x + 1 ) * ( tex.width / view.width ) - 1
	// output.x = input.x * ( tex.width / view.width ) + ( tex.width / view.width ) - 1
	// => scale.x = tex.width / view.width
	// => offset.x = ( tex.width / view.width ) - 1 = scale.x - 1
	
	// set opengl states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	// set shader and options
	renderThread.GetShader().ActivateShader( pShaderOutArrTex );
	deoglShaderCompiled &shader = *pShaderOutArrTex->GetCompiled();
	
	shader.SetParameterFloat( spotPosTransform, scaleX, scaleY, scaleX - 1.0f, scaleY - 1.0f );
	shader.SetParameterFloat( spotTCTransform, 0.5f, 0.5f, 0.5f, 0.5f );
	shader.SetParameterFloat( spotColor, 1.0f, 1.0f, 1.0f, 1.0f );
	
	if( gammaCorrect ){
		shader.SetParameterFloat( spotGamma, OGL_RENDER_GAMMA, OGL_RENDER_GAMMA, OGL_RENDER_GAMMA, 1.0f );
		
	}else{
		shader.SetParameterFloat( spotGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	}
	
	shader.SetParameterFloat( spotLayer, ( float )layer );
	
	// set texture
	tsmgr.EnableArrayTexture( 0, *texture, GetSamplerClampNearest() );
	
	// render full screen quad
	defren.RenderFSQuadVAO();
	
	// cleanup
	tsmgr.DisableStage( 0 );
}

void deoglRenderDebug::RenderComponentsStatic( sRenderParameters &params ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const deoglCollideList &clist = *params.collideList;
	int c, componentCount;
	
	// set states
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	// prepare depth testing
	tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	
	// render component boxes
	componentCount = clist.GetComponentCount();
	
	for( c=0; c<componentCount; c++ ){
		deoglRComponent &component = *clist.GetComponentAt( c )->GetComponent();
		
		if( component.GetRenderStatic() ){
			RenderComponentBox( params, component, decColor( 0.0f, 0.5f, 0.0f, 1.0f ) );
			
		}else{
			RenderComponentBox( params, component, decColor( 1.0f, 0.0f, 0.0f, 1.0f ) );
		}
	}
}

void deoglRenderDebug::RenderComponentBox( sRenderParameters &params, deoglRComponent &component, const decColor &color ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglShapeManager &shapeManager = renderThread.GetBufferObject().GetShapeManager();
	deoglShape &shapeBox = *shapeManager.GetShapeAt( deoglRTBufferObject::esBox );
	
	decDMatrix matrixCP( params.matrixCamera * params.matrixProjection );
	deoglShaderCompiled *shader = NULL;
	decDMatrix matrixMVP;
	deoglDCollisionBox box;
	decColor edgeColor;
	
	// set states
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	// prepare depth testing
	tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	
	// select shader
	renderThread.GetShader().ActivateShader( pShaderXRay );
	shader = pShaderXRay->GetCompiled();
	
	edgeColor.r = powf( color.r, OGL_RENDER_GAMMA );
	edgeColor.g = powf( color.g, OGL_RENDER_GAMMA );
	edgeColor.b = powf( color.b, OGL_RENDER_GAMMA );
	
	// set matrix
	box.SetFromExtends( component.GetMinimumExtend(), component.GetMaximumExtend() );
	
	matrixMVP = decDMatrix::CreateScale( box.GetHalfSize() ) * decDMatrix::CreateTranslation( box.GetCenter() ) * matrixCP;
	
	shader->SetParameterDMatrix4x4( sprMatrixMVP, matrixMVP );
	
	// render
	shader->SetParameterColor4( sprColor, edgeColor );
	shapeBox.ActivateVAO();
	shapeBox.RenderLines();
	pglBindVertexArray( 0 );
}

void deoglRenderDebug::RenderText( deoglRenderPlan &plan, const char *text, int x, int y, const decColor &color ){
	if( ! text ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	const deoglDebugFont::sGlyph * const glyphs = pDebugFont->GetGlyphs();
	float texCoordWidth, texCoordHeight, quadWidth, quadHeight;
	int curx, x1, y1, x2, y2, cw, adv;
	const float fontScale = 1.0f;
	decUTF8Decoder utf8Decoder;
	int character, len;
	
	const float scalePosition1X = 1.0f / ( float )plan.GetViewportWidth();
	const float scalePosition1Y = -1.0f / ( float )plan.GetViewportHeight();
	const float scalePosition2X = 2.0f / ( float )plan.GetViewportWidth();
	const float scalePosition2Y = -2.0f / ( float )plan.GetViewportHeight();
	const float offsetPositionX = scalePosition2X * 0.375f - 1.0f;
	const float offsetPositionY = scalePosition2Y * 0.375f + 1.0f;
	
	// set texture
	tsmgr.EnableTexture( 0, *pDebugFont->GetTexture(), GetSamplerClampNearest() );
	
	// set shader
	renderThread.GetShader().ActivateShader( pShaderOutTex );
	deoglShaderCompiled &shader = *pShaderOutTex->GetCompiled();
	
	shader.SetParameterFloat( spotColor, color.r, color.g, color.b, color.a );
	shader.SetParameterFloat( spotGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	
	// render text
	curx = x;
	
	utf8Decoder.SetString( text );
	len = strlen( text );
	if( len > utf8Decoder.GetLength() ){
		len = utf8Decoder.GetLength();
	}
	
	while( utf8Decoder.GetPosition() < len ){
		character = utf8Decoder.DecodeNextCharacter();
		if( character < 0 || character > 255 ){
			continue; // temp hack: not working for unicode
		}
		
		const deoglDebugFont::sGlyph &glyph = glyphs[ character ];
		
		// calculate positions
		x1 = curx;
		y1 = y;
		cw = ( int )( ( float )glyph.width * fontScale );
		x2 = x1 + cw;
		y2 = y1 + ( int )( ( float )glyph.height * fontScale );
		adv = ( int )( ( float )glyph.advance * fontScale );
		
		quadWidth = ( float )( x2 - x1 );
		quadHeight = ( float )( y2 - y1 );
		texCoordWidth = glyph.x2 - glyph.x1;
		texCoordHeight = glyph.y2 - glyph.y1;
		
		// render char
		shader.SetParameterFloat( spotPosTransform,
			scalePosition1X * quadWidth, scalePosition1Y * quadHeight,
			scalePosition2X * ( ( float )( x1 ) + quadWidth * 0.5f ) + offsetPositionX,
			scalePosition2Y * ( ( float )( y1 ) + quadHeight * 0.5f ) + offsetPositionY );
		shader.SetParameterFloat( spotTCTransform,
			texCoordWidth * 0.5f, texCoordHeight * 0.5f, glyph.x1 + texCoordWidth * 0.5f, glyph.y1 + texCoordHeight * 0.5f );
		
		defren.RenderFSQuadVAO();
		
		// next round
		curx += adv;
	}
	
	tsmgr.DisableStage( 0 );
}

void deoglRenderDebug::BeginRenderText(){
	pVBORenderTextDataCount = 0;
}

void deoglRenderDebug::AddRenderText( deoglRenderPlan &plan, const char *text, int x, int y, const decColor &color ){
	if( ! text ){
		DETHROW( deeInvalidParam );
	}
	
	const deoglDebugFont::sGlyph * const glyphs = pDebugFont->GetGlyphs();
	const float fontScale = 1.0f;
	
	const float scalePosition1X = 1.0f / ( float )plan.GetViewportWidth();
	const float scalePosition1Y = -1.0f / ( float )plan.GetViewportHeight();
	const float scalePosition2X = 2.0f / ( float )plan.GetViewportWidth();
	const float scalePosition2Y = -2.0f / ( float )plan.GetViewportHeight();
	const float offsetPositionX = scalePosition2X * 0.375f - 1.0f;
	const float offsetPositionY = scalePosition2Y * 0.375f + 1.0f;
	
	// render text
	int curx = x;
	
	decUTF8Decoder utf8Decoder;
	utf8Decoder.SetString( text );
	const int len = decMath::min( strlen( text ), utf8Decoder.GetLength() );
	
	while( utf8Decoder.GetPosition() < len ){
		const int character = utf8Decoder.DecodeNextCharacter();
		if( character < 0 || character > 255 ){
			continue; // temp hack: not working for unicode
		}
		
		const deoglDebugFont::sGlyph &glyph = glyphs[ character ];
		
		// calculate positions
		const int x1 = curx;
		const int y1 = y;
		const int cw = ( int )( ( float )glyph.width * fontScale );
		const int x2 = x1 + cw;
		const int y2 = y1 + ( int )( ( float )glyph.height * fontScale );
		const int adv = ( int )( ( float )glyph.advance * fontScale );
		
		const float quadWidth = ( float )( x2 - x1 );
		const float quadHeight = ( float )( y2 - y1 );
		const float texCoordWidth = glyph.x2 - glyph.x1;
		const float texCoordHeight = glyph.y2 - glyph.y1;
		
		// add to VBO data
		if( pVBORenderTextDataCount + 6 > pVBORenderTextDataSize ){
			const int newSize = pVBORenderTextDataCount * 3 / 2 + 6;
			sVBODataGlyph * const newArray = new sVBODataGlyph[ newSize ];
			if( pVBORenderTextData ){
				memcpy( newArray, pVBORenderTextData, sizeof( sVBODataGlyph ) * pVBORenderTextDataCount );
				delete [] pVBORenderTextData;
			}
			pVBORenderTextData = newArray;
			pVBORenderTextDataSize = newSize;
		}
		
		// position = inPosition * pPosTransform.xy + pPosTransform.zw
		// texCoord = inPosition * pTCTransform.xy + pTCTransform.zw
		const float posTransform[ 4 ] = { scalePosition1X * quadWidth, scalePosition1Y * quadHeight,
			scalePosition2X * ( ( float )( x1 ) + quadWidth * 0.5f ) + offsetPositionX,
			scalePosition2Y * ( ( float )( y1 ) + quadHeight * 0.5f ) + offsetPositionY };
		
		const float tcTransform[ 4 ] = { texCoordWidth * 0.5f, texCoordHeight * 0.5f,
			glyph.x1 + texCoordWidth * 0.5f, glyph.y1 + texCoordHeight * 0.5f };
		
		sVBODataGlyph &p1 = pVBORenderTextData[ pVBORenderTextDataCount++ ];
		p1.position.x = -posTransform[ 0 ] + posTransform[ 2 ];
		p1.position.y = -posTransform[ 1 ] + posTransform[ 3 ];
		p1.texCoord.x = -tcTransform[ 0 ] + tcTransform[ 2 ];
		p1.texCoord.y = -tcTransform[ 1 ] + tcTransform[ 3 ];
		p1.color = color;
		
		sVBODataGlyph &p2 = pVBORenderTextData[ pVBORenderTextDataCount++ ];
		p2.position.x = posTransform[ 0 ] + posTransform[ 2 ];
		p2.position.y = -posTransform[ 1 ] + posTransform[ 3 ];
		p2.texCoord.x = tcTransform[ 0 ] + tcTransform[ 2 ];
		p2.texCoord.y = -tcTransform[ 1 ] + tcTransform[ 3 ];
		p2.color = color;
		
		sVBODataGlyph &p3 = pVBORenderTextData[ pVBORenderTextDataCount++ ];
		p3.position.x = -posTransform[ 0 ] + posTransform[ 2 ];
		p3.position.y = posTransform[ 1 ] + posTransform[ 3 ];
		p3.texCoord.x = -tcTransform[ 0 ] + tcTransform[ 2 ];
		p3.texCoord.y = tcTransform[ 1 ] + tcTransform[ 3 ];
		p3.color = color;
		
		pVBORenderTextData[ pVBORenderTextDataCount++ ] = p3;
		pVBORenderTextData[ pVBORenderTextDataCount++ ] = p2;
		
		sVBODataGlyph &p4 = pVBORenderTextData[ pVBORenderTextDataCount++ ];
		p4.position.x = posTransform[ 0 ] + posTransform[ 2 ];
		p4.position.y = posTransform[ 1 ] + posTransform[ 3 ];
		p4.texCoord.x = tcTransform[ 0 ] + tcTransform[ 2 ];
		p4.texCoord.y = tcTransform[ 1 ] + tcTransform[ 3 ];
		p4.color = color;
		
		// next round
		curx += adv;
	}
}

void deoglRenderDebug::EndRenderText(){
	if( pVBORenderTextDataCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBORenderText ) );
	OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER,
		sizeof( sVBODataGlyph ) * pVBORenderTextDataCount, NULL, GL_STREAM_DRAW ) );
	OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER,
		sizeof( sVBODataGlyph ) * pVBORenderTextDataCount, pVBORenderTextData, GL_STREAM_DRAW ) );
	
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	tsmgr.EnableTexture( 0, *pDebugFont->GetTexture(), GetSamplerClampNearest() );
	
	renderThread.GetShader().ActivateShader( pShaderRenderText );
	
	OGL_CHECK( renderThread, pglBindVertexArray( pVAORenderText ) );
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, pVBORenderTextDataCount ) );
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	
	tsmgr.DisableStage( 0 );
	pVBORenderTextDataCount = 0;
}



void deoglRenderDebug::RenderRectangle( deoglRenderPlan &plan, int x1, int y1, int x2, int y2, const decColor &color ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const float scalePosition1X = 1.0f / ( float )plan.GetViewportWidth();
	const float scalePosition1Y = -1.0f / ( float )plan.GetViewportHeight();
	const float scalePosition2X = 2.0f / ( float )plan.GetViewportWidth();
	const float scalePosition2Y = -2.0f / ( float )plan.GetViewportHeight();
	const float offsetPositionX = scalePosition2X * 0.375f - 1.0f;
	const float offsetPositionY = scalePosition2Y * 0.375f + 1.0f;
	
	renderThread.GetShader().ActivateShader( pShaderRectangle );
	deoglShaderCompiled &shader = *pShaderRectangle->GetCompiled();
	
	shader.SetParameterFloat( sprectPosTransform,
		scalePosition1X * ( float )( x2 - x1 ), scalePosition1Y * ( float )( y2 - y1 ),
		scalePosition2X * ( ( float )( x1 + x2 ) * 0.5f ) + offsetPositionX,
		scalePosition2Y * ( ( float )( y1 + y2 ) * 0.5f ) + offsetPositionY );
	
	shader.SetParameterFloat( sprectColor, color.r, color.g, color.b, color.a );
	
	defren.RenderFSQuadVAO();
}



// Private Functions
//////////////////////

void deoglRenderDebug::pCleanUp(){
	if( pVAORenderText ){
		pglDeleteVertexArrays( 1, &pVAORenderText );
	}
	if( pVBORenderText ){
		pglDeleteBuffers( 1, &pVBORenderText );
	}
	if( pVBORenderTextData ){
		delete [] pVBORenderTextData;
	}
	if( pDebugFont ){
		delete pDebugFont;
	}
	
	if( pShaderRectangle ){
		pShaderRectangle->RemoveUsage();
	}
	if( pShaderXRay ){
		pShaderXRay->RemoveUsage();
	}
	if( pShaderSolid ){
		pShaderSolid->RemoveUsage();
	}
	if( pShaderSphere ){
		pShaderSphere->RemoveUsage();
	}
	if( pShaderOutTex ){
		pShaderOutTex->RemoveUsage();
	}
	if( pShaderOutTexLayer ){
		pShaderOutTexLayer->RemoveUsage();
	}
	if( pShaderOutArrTex ){
		pShaderOutArrTex->RemoveUsage();
	}
	if( pShaderRenderText ){
		pShaderRenderText->RemoveUsage();
	}
}
