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

#include <stdlib.h>

#include "deoglDebugStateSnapshot.h"
#include "../deoglBasics.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Helper stuff
/////////////////

struct sNameTableEntry{
	const int value;
	const char *name;
};

static sNameTableEntry vGLConstantNameTable[] = {
	{ GL_ZERO, "GL_ZERO"},
	{ GL_ONE, "GL_ONE"},
	{ GL_SRC_COLOR, "GL_SRC_COLOR"},
	{ GL_ONE_MINUS_SRC_COLOR, "GL_ONE_MINUS_SRC_COLOR"},
	{ GL_DST_COLOR, "GL_DST_COLOR"},
	{ GL_ONE_MINUS_DST_COLOR, "GL_ONE_MINUS_DST_COLOR"},
	{ GL_SRC_ALPHA, "GL_SRC_ALPHA"},
	{ GL_ONE_MINUS_SRC_ALPHA, "GL_ONE_MINUS_SRC_ALPHA"},
	{ GL_DST_ALPHA, "GL_DST_ALPHA"},
	{ GL_ONE_MINUS_DST_ALPHA, "GL_ONE_MINUS_DST_ALPHA"},
	{ GL_CONSTANT_COLOR, "GL_CONSTANT_COLOR"},
	{ GL_ONE_MINUS_CONSTANT_COLOR, "GL_ONE_MINUS_CONSTANT_COLOR"},
	{ GL_CONSTANT_ALPHA, "GL_CONSTANT_ALPHA"},
	{ GL_ONE_MINUS_CONSTANT_ALPHA, "GL_ONE_MINUS_CONSTANT_ALPHA"},
	{ GL_SRC_ALPHA_SATURATE, "GL_SRC_ALPHA_SATURATE"},
	{ GL_SRC1_COLOR, "GL_SRC1_COLOR"},
	{ GL_ONE_MINUS_SRC1_COLOR, "GL_ONE_MINUS_SRC1_COLOR"},
	{ GL_SRC1_ALPHA, "GL_SRC1_ALPHA"},
	{ GL_ONE_MINUS_SRC1_ALPHA, "GL_ONE_MINUS_SRC1_ALPHA"},
	
	{ GL_FRONT, "GL_FRONT"},
	{ GL_BACK, "GL_BACK"},
	{ GL_FRONT_AND_BACK, "GL_FRONT_AND_BACK"},
	
	{ GL_NEVER, "GL_NEVER"},
	{ GL_LESS, "GL_LESS"},
	{ GL_EQUAL, "GL_EQUAL"},
	{ GL_LEQUAL, "GL_LEQUAL"},
	{ GL_GREATER, "GL_GREATER"},
	{ GL_NOTEQUAL, "GL_NOTEQUAL"},
	{ GL_GEQUAL, "GL_GEQUAL"},
	{ GL_ALWAYS, "GL_ALWAYS"},
	
	{ GL_POINT, "GL_POINT"},
	{ GL_LINE, "GL_LINE"},
	{ GL_FILL, "GL_FILL"},
	
	{ GL_KEEP, "GL_KEEP"},
	{ GL_REPLACE, "GL_REPLACE"},
	{ GL_INCR, "GL_INCR"},
	{ GL_INCR_WRAP, "GL_INCR_WRAP"},
	{ GL_DECR, "GL_DECR"},
	{ GL_DECR_WRAP, "GL_DECR_WRAP"},
	{ GL_INVERT, "GL_INVERT"},
	
	{ GL_FUNC_ADD, "GL_FUNC_ADD"},
	{ GL_FUNC_SUBTRACT, "GL_FUNC_SUBTRACT"},
	{ GL_FUNC_REVERSE_SUBTRACT, "GL_FUNC_REVERSE_SUBTRACT"},
	{ GL_MIN, "GL_MIN"},
	{ GL_MAX, "GL_MAX"},
	
	{ 0, NULL }
};



// Class deoglDebugStateSnapshot
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugStateSnapshot::deoglDebugStateSnapshot( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pStatesKeyWidth( 35 )
{
	pEnabledBlend = glIsEnabled( GL_BLEND );
	pEnabledCullFace = glIsEnabled( GL_CULL_FACE );
	pEnabledDepthTest = glIsEnabled( GL_DEPTH_TEST );
	pEnabledPolygonOffsetFill = glIsEnabled( GL_POLYGON_OFFSET_FILL );
	pEnabledScissorTest = glIsEnabled( GL_SCISSOR_TEST );
	pEnabledStencilTest = glIsEnabled( GL_STENCIL_TEST );
	
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_BLEND_SRC_RGB, &pBlendSrcRgb ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_BLEND_SRC_ALPHA, &pBlendSrcAlpha ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_BLEND_DST_RGB, &pBlendDstRgb ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_BLEND_DST_ALPHA, &pBlendDstAlpha ) );
	
	OGL_CHECK( pRenderThread, glGetBooleanv( GL_COLOR_WRITEMASK, &pColorWriteMask[ 0 ] ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_CULL_FACE_MODE, &pCullFaceMode ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_DEPTH_FUNC, &pDepthFunc ) );
	OGL_CHECK( pRenderThread, glGetBooleanv( GL_DEPTH_WRITEMASK, &pDepthWriteMask ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_POLYGON_MODE, &pPolygonMode ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_SCISSOR_BOX, &pScissorBox[ 0 ] ) );
	
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_BITS, &pStencilBits ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_FUNC, &pStencilFunc ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_VALUE_MASK, &pStencilValueMask ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_REF, &pStencilRef ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_BACK_FUNC, &pStencilBackFunc ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_BACK_VALUE_MASK, &pStencilBackValueMask ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_BACK_REF, &pStencilBackRef ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_WRITEMASK, &pStencilWriteMask ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_BACK_WRITEMASK, &pStencilBackWriteMask ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_FAIL, &pStencilFail ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_PASS_DEPTH_PASS, &pStencilPassDepthPass ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_PASS_DEPTH_FAIL, &pStencilPassDepthFail ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_BACK_FAIL, &pStencilBackFail ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_BACK_PASS_DEPTH_PASS, &pStencilBackPassDepthPass ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_STENCIL_BACK_PASS_DEPTH_FAIL, &pStencilBackPassDepthFail ) );
	
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_VIEWPORT, &pViewport[ 0 ] ) );
	
	OGL_CHECK( pRenderThread, glGetFloatv( GL_BLEND_COLOR, &pBlendColor[ 0 ] ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_BLEND_EQUATION_RGB, &pBlendEquationRgb ) );
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_BLEND_EQUATION_ALPHA, &pBlendEquationAlpha ) );
	
	OGL_CHECK( pRenderThread, glGetFloatv( GL_POLYGON_OFFSET_FACTOR, &pPolygonOffsetFactor ) );
	OGL_CHECK( pRenderThread, glGetFloatv( GL_POLYGON_OFFSET_UNITS, &pPolygonOffsetUnits ) );
}

deoglDebugStateSnapshot::~deoglDebugStateSnapshot(){
}



// Management
///////////////

void deoglDebugStateSnapshot::LogState() const{
	deoglRTLogger &logger = pRenderThread.GetLogger();
	
	logger.LogInfo( "OpenGL State:" );
	logger.LogInfo( "  glEnable/glDisable: " );
	logger.LogInfo( pLogState( "    GL_BLEND", pEnabledBlend ? "true": "false" ) );
	logger.LogInfo( pLogState( "    GL_CULL_FACE", pEnabledCullFace ? "true": "false" ) );
	logger.LogInfo( pLogState( "    GL_DEPTH_TEST", pEnabledDepthTest ? "true": "false" ) );
	logger.LogInfo( pLogState( "    GL_POLYGON_OFFSET_FILL", pEnabledPolygonOffsetFill ? "true": "false" ) );
	logger.LogInfo( pLogState( "    GL_SCISSOR_TEST", pEnabledScissorTest ? "true": "false" ) );
	logger.LogInfo( pLogState( "    GL_STENCIL_TEST", pEnabledStencilTest ? "true": "false" ) );
	
	logger.LogInfo( "  States:" );
	logger.LogInfo( pLogState( "    GL_BLEND_SRC_RGB", pGLConstantName( pBlendSrcRgb ) ) );
	logger.LogInfo( pLogState( "    GL_BLEND_SRC_ALPHA", pGLConstantName( pBlendSrcAlpha ) ) );
	logger.LogInfo( pLogState( "    GL_BLEND_DST_RGB", pGLConstantName( pBlendDstRgb ) ) );
	logger.LogInfo( pLogState( "    GL_BLEND_DST_ALPHA", pGLConstantName( pBlendDstAlpha ) ) );
	
	decString text;
	text.Format( "%s %s %s %s",
		pColorWriteMask[ 0 ] ? "true": "false",
		pColorWriteMask[ 1 ] ? "true": "false",
		pColorWriteMask[ 2 ] ? "true": "false",
		pColorWriteMask[ 3 ] ? "true": "false" );
	logger.LogInfo( pLogState( "    GL_COLOR_WRITEMASK", text ) );
	
	logger.LogInfo( pLogState( "    GL_CULL_FACE_MODE", pGLConstantName( pCullFaceMode ) ) );
	logger.LogInfo( pLogState( "    GL_DEPTH_FUNC", pGLConstantName( pDepthFunc ) ) );
	logger.LogInfo( pLogState( "    GL_DEPTH_WRITEMASK", pDepthWriteMask ? "true": "false" ) );
	logger.LogInfo( pLogState( "    GL_POLYGON_MODE", pGLConstantName( pPolygonMode ) ) );
	
	text.Format( "x,y=(%d,%d) w,h=(%d,%d)", pScissorBox[ 0 ],
		pScissorBox[ 1 ], pScissorBox[ 2 ], pScissorBox[ 3 ] );
	logger.LogInfo( pLogState( "    GL_SCISSOR_BOX", text ) );
	
	logger.LogInfo( pLogState( "    GL_STENCIL_BITS", pGLConstantName( pStencilBits ) ) );
	logger.LogInfo( pLogState( "    GL_STENCIL_FUNC", pGLConstantName( pStencilFunc ) ) );
	
	text.Format( "%02x (%s)", pStencilValueMask, pBitString( pStencilValueMask, pStencilBits ).GetString() );
	logger.LogInfo( pLogState( "    GL_STENCIL_VALUE_MASK", text ) );
	
	text.Format( "%02x (%s)", pStencilRef, pBitString( pStencilRef, pStencilBits ).GetString() );
	logger.LogInfo( pLogState( "    GL_STENCIL_REF", text ) );
	
	logger.LogInfo( pLogState( "    GL_STENCIL_BACK_FUNC", pGLConstantName( pStencilBackFunc ) ) );
	
	text.Format( "%02x (%s)", pStencilBackValueMask, pBitString( pStencilBackValueMask, pStencilBits ).GetString() );
	logger.LogInfo( pLogState( "    GL_STENCIL_BACK_VALUE_MASK", text ) );
	
	text.Format( "%02x (%s)", pStencilBackRef, pBitString( pStencilBackRef, pStencilBits ).GetString() );
	logger.LogInfo( pLogState( "    GL_STENCIL_BACK_REF", text ) );
	
	text.Format( "%02x (%s)", pStencilWriteMask, pBitString( pStencilWriteMask, pStencilBits ).GetString() );
	logger.LogInfo( pLogState( "    GL_STENCIL_WRITEMASK", text ) );
	
	text.Format( "%02x (%s)", pStencilBackWriteMask, pBitString( pStencilBackWriteMask, pStencilBits ).GetString() );
	logger.LogInfo( pLogState( "    GL_STENCIL_BACK_WRITEMASK", text ) );
	
	logger.LogInfo( pLogState( "    GL_STENCIL_FAIL", pGLConstantName( pStencilFail ) ) );
	logger.LogInfo( pLogState( "    GL_STENCIL_PASS_DEPTH_PASS", pGLConstantName( pStencilPassDepthPass ) ) );
	logger.LogInfo( pLogState( "    GL_STENCIL_PASS_DEPTH_FAIL", pGLConstantName( pStencilPassDepthFail ) ) );
	logger.LogInfo( pLogState( "    GL_STENCIL_BACK_FAIL", pGLConstantName( pStencilBackFail ) ) );
	logger.LogInfo( pLogState( "    GL_STENCIL_BACK_PASS_DEPTH_PASS", pGLConstantName( pStencilBackPassDepthPass ) ) );
	logger.LogInfo( pLogState( "    GL_STENCIL_BACK_PASS_DEPTH_FAIL", pGLConstantName( pStencilBackPassDepthFail ) ) );
	
	text.Format( "x,y=(%d,%d) w,h=(%d,%d)", pViewport[ 0 ], pViewport[ 1 ], pViewport[ 2 ], pViewport[ 3 ] );
	logger.LogInfo( pLogState( "    GL_VIEWPORT", text ) );
	
	text.Format( "(%g, %g, %g, %g)", pBlendColor[ 0 ], pBlendColor[ 1 ], pBlendColor[ 2 ], pBlendColor[ 3 ] );
	logger.LogInfo( pLogState( "    GL_BLEND_COLOR", text ) );
	
	logger.LogInfo( pLogState( "    GL_BLEND_EQUATION_RGB", pGLConstantName( pBlendEquationRgb ) ) );
	logger.LogInfo( pLogState( "    GL_BLEND_EQUATION_ALPHA", pGLConstantName( pBlendEquationAlpha ) ) );
	
	text.Format( "%g", pPolygonOffsetFactor );
	logger.LogInfo( pLogState( "    GL_POLYGON_OFFSET_FACTOR", text ) );
	
	text.Format( "%g", pPolygonOffsetUnits );
	logger.LogInfo( pLogState( "    GL_POLYGON_OFFSET_UNITS", text ) );
}

void deoglDebugStateSnapshot::LogDifference( const deoglDebugStateSnapshot& state ) const{
	// TODO
}



// Private Functions
//////////////////////

decString deoglDebugStateSnapshot::pLogState( const char* key, const char* value ) const{
	decString text( key );
	text.AppendCharacter( ' ' );
	
	const int len = text.GetLength();
	int i;
	for( i=len; i<=pStatesKeyWidth; i++ ){
		//text.AppendCharacter( ' ' );
		text.AppendCharacter( '.' );
	}
	
	text.AppendCharacter( ' ' );
	text.Append( value );
	return text;
}

decString deoglDebugStateSnapshot::pGLConstantName( int value ) const{
	int i;
	
	for( i=0; vGLConstantNameTable[ i ].name; i++ ){
		if( vGLConstantNameTable[ i ].value == value ){
			return decString( vGLConstantNameTable[ i ].name );
		}
	}
	
	decString text;
	text.Format( "%x", value );
	return text;
}

decString deoglDebugStateSnapshot::pBitString( int value, int maxBits ) const{
	decString text;
	int i;
	
	for( i=1; i<=maxBits; i++ ){
		if( i > 1 && ( i - 1 ) % 4 == 0 ){
			text.AppendCharacter( ' ' );
		}
		
		if( ( value & ( 1 << ( maxBits - i ) ) ) == 0 ){
			text.AppendCharacter( '0' );
			
		}else{
			text.AppendCharacter( '1' );
		}
	}
	
	return text;
}
