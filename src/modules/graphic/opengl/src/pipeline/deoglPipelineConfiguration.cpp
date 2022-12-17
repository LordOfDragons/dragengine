/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "deoglPipelineConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>



// Class deoglPipelineConfiguration
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPipelineConfiguration::deoglPipelineConfiguration() :
pEnableRasterizerDiscard( false ),
pPolygonMode( GL_FILL ),
pCullFace( GL_BACK ),
pEnablePolygonOffset( false ),
pPolygonOffsetFactor( 0.0f ),
pPolygonOffsetBias( 0.0f ),
pEnableDepthTest( false ),
pDepthFunc( GL_LESS ),
pDepthMask( true ),
pEnableStencilTest( false ),
pStencilOpFailFront( GL_KEEP ),
pStencilOpZFailFront( GL_KEEP ),
pStencilOpZPassFront( GL_KEEP ),
pStencilOpFailBack( GL_KEEP ),
pStencilOpZFailBack( GL_KEEP ),
pStencilOpZPassBack( GL_KEEP ),
pStencilFunc( GL_ALWAYS ),
pStencilRef( 0 ),
pStencilRefMask( ~0 ),
pStencilMask( ~0 ),
pStencilSameFrontBack( true ),
pBlendFuncSource( GL_ONE ),
pBlendFuncDest( GL_ZERO ),
pDynamicCullFace( false ),
pDynamicPolygonOffset( false ),
pDynamicStencil( false )
{
	pColorMask[ 0 ] = true;
	pColorMask[ 1 ] = true;
	pColorMask[ 2 ] = true;
	pColorMask[ 3 ] = true;
}

deoglPipelineConfiguration::deoglPipelineConfiguration( const deoglPipelineConfiguration &config ) :
pEnableRasterizerDiscard( false ),
pPolygonMode( GL_FILL ),
pCullFace( GL_BACK ),
pEnablePolygonOffset( false ),
pPolygonOffsetFactor( 0.0f ),
pPolygonOffsetBias( 0.0f ),
pEnableDepthTest( false ),
pDepthFunc( GL_LESS ),
pDepthMask( true ),
pEnableStencilTest( false ),
pStencilOpFailFront( GL_KEEP ),
pStencilOpZFailFront( GL_KEEP ),
pStencilOpZPassFront( GL_KEEP ),
pStencilOpFailBack( GL_KEEP ),
pStencilOpZFailBack( GL_KEEP ),
pStencilOpZPassBack( GL_KEEP ),
pStencilFunc( GL_ALWAYS ),
pStencilRef( 0 ),
pStencilRefMask( ~0 ),
pStencilMask( ~0 ),
pBlendFuncSource( GL_ONE ),
pBlendFuncDest( GL_ZERO ),
pDynamicCullFace( false ),
pDynamicPolygonOffset( false ),
pDynamicStencil( false )
{
	pColorMask[ 0 ] = true;
	pColorMask[ 1 ] = true;
	pColorMask[ 2 ] = true;
	pColorMask[ 3 ] = true;
	*this = config;
}



// Management
///////////////

void deoglPipelineConfiguration::SetShader( const deoglShaderProgramUsage &shader ){
	pShader = shader;
}

void deoglPipelineConfiguration::SetEnableRasterizerDiscard( bool enable ){
	pEnableRasterizerDiscard = enable;
}

void deoglPipelineConfiguration::SetPolygonMode( GLenum mode ){
	pPolygonMode = mode;
}

void deoglPipelineConfiguration::SetCullFace( GLenum mode ){
	pCullFace = mode;
}

void deoglPipelineConfiguration::SetEnablePolygonOffset( bool enable ){
	pEnablePolygonOffset = enable;
}

void deoglPipelineConfiguration::SetPolygonOffsetFactor( float factor ){
	pPolygonOffsetFactor = factor;
}

void deoglPipelineConfiguration::SetPolygonOffsetBias( float bias ){
	pPolygonOffsetBias = bias;
}

void deoglPipelineConfiguration::SetEnableDepthTest( bool enable ){
	pEnableDepthTest = enable;
}

void deoglPipelineConfiguration::SetDepthFunc( GLenum mode ){
	pDepthFunc = mode;
}

void deoglPipelineConfiguration::SetDepthMask( bool mask ){
	pDepthMask = mask;
}

void deoglPipelineConfiguration::SetEnableStencilTest( bool enable ){
	pEnableStencilTest = enable;
}

void deoglPipelineConfiguration::SetStencilOpFailFront( GLenum mode ){
	pStencilOpFailFront = mode;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetStencilOpZFailFront( GLenum mode ){
	pStencilOpZFailFront = mode;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetStencilOpZPassFront( GLenum mode ){
	pStencilOpZPassFront = mode;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetStencilOpFailBack( GLenum mode ){
	pStencilOpFailBack = mode;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetStencilOpZFailBack( GLenum mode ){
	pStencilOpZFailBack = mode;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetStencilOpZPassBack( GLenum mode ){
	pStencilOpZPassBack = mode;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetStencilFunc( GLenum mode ){
	pStencilFunc = mode;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetStencilRef( int ref ){
	pStencilRef = ref;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetStencilRefMask( int mask ){
	pStencilRefMask = mask;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetStencilMask( int mask ){
	pStencilMask = mask;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetBlendColor( const decColor &color ){
	pBlendColor = color;
}

void deoglPipelineConfiguration::SetBlendFuncSource( GLenum mode ){
	pBlendFuncSource = mode;
}

void deoglPipelineConfiguration::SetBlendFuncDest( GLenum mode ){
	pBlendFuncDest = mode;
}

bool deoglPipelineConfiguration::GetColorMask( int component ) const{
	DEASSERT_TRUE( component >= 0 && component <= 3 )
	return pColorMask[ component ];
}

void deoglPipelineConfiguration::SetColorMask( bool red, bool green, bool blue, bool alpha ){
	pColorMask[ 0 ] = red;
	pColorMask[ 1 ] = green;
	pColorMask[ 2 ] = blue;
	pColorMask[ 3 ] = alpha;
}



void deoglPipelineConfiguration::Activate( deoglRenderThread &renderThread ){
	#define ENABLE_GL_STATE(flag,state) \
		if( flag ){ \
			OGL_CHECK( renderThread, glEnable( state ) ); \
		} else { \
			OGL_CHECK( renderThread, glDisable( state ) ); \
		}
	
	renderThread.GetShader().ActivateShader( pShader );
	
	ENABLE_GL_STATE( pEnableRasterizerDiscard, GL_RASTERIZER_DISCARD )
	
	OGL_CHECK( renderThread, glPolygonMode( GL_FRONT_AND_BACK, pPolygonMode ) );
	if( ! pDynamicCullFace ){
		OGL_CHECK( renderThread, glCullFace( pCullFace ) );
	}
	
	ENABLE_GL_STATE( pEnablePolygonOffset, GL_POLYGON_OFFSET_FILL )
	if( pEnablePolygonOffset && ! pDynamicPolygonOffset ){
		OGL_CHECK( renderThread, pglPolygonOffset( pPolygonOffsetFactor, pPolygonOffsetBias ) );
	}
	
	ENABLE_GL_STATE( pEnableDepthTest, GL_DEPTH_TEST )
	if( pEnableDepthTest ){
		OGL_CHECK( renderThread, glDepthFunc( pDepthFunc ) );
		OGL_CHECK( renderThread, glDepthMask( pDepthMask ) );
	}
	
	ENABLE_GL_STATE( pEnableStencilTest, GL_STENCIL_TEST )
	if( pEnableStencilTest && ! pDynamicStencil ){
		if( pStencilSameFrontBack ){
			OGL_CHECK( renderThread, glStencilOp(
				pStencilOpFailFront, pStencilOpZFailFront, pStencilOpZPassFront ) );
			
		}else{
			OGL_CHECK( renderThread, pglStencilOpSeparate( GL_FRONT,
				pStencilOpFailFront, pStencilOpZFailFront, pStencilOpZPassFront ) );
			OGL_CHECK( renderThread, pglStencilOpSeparate( GL_BACK,
				pStencilOpFailBack, pStencilOpZFailBack, pStencilOpZPassBack ) );
		}
		
		OGL_CHECK( renderThread, glStencilFunc( pStencilFunc, pStencilRef, pStencilRefMask ) );
		OGL_CHECK( renderThread, glStencilMask( pStencilMask ) );
	}
	
	OGL_CHECK( renderThread, pglBlendColor( pBlendColor.r, pBlendColor.g, pBlendColor.b, pBlendColor.a ) );
	OGL_CHECK( renderThread, glBlendFunc( pBlendFuncSource, pBlendFuncDest ) );
	OGL_CHECK( renderThread, glColorMask( pColorMask[ 0 ], pColorMask[ 1 ], pColorMask[ 2 ], pColorMask[ 3 ] ) );
	
	#undef ENABLE_GL_STATE
}



// Operators
//////////////

deoglPipelineConfiguration &deoglPipelineConfiguration::operator=( const deoglPipelineConfiguration &config ){
	pShader = config.pShader;
	pEnableRasterizerDiscard = config.pEnableRasterizerDiscard;
	pPolygonMode = config.pPolygonMode;
	pCullFace = config.pCullFace;
	pEnablePolygonOffset = config.pEnablePolygonOffset;
	pPolygonOffsetFactor = config.pPolygonOffsetFactor;
	pPolygonOffsetBias = config.pPolygonOffsetBias;
	pEnableDepthTest = config.pEnableDepthTest;
	pDepthFunc = config.pDepthFunc;
	pDepthMask = config.pDepthMask;
	pEnableStencilTest = config.pEnableStencilTest;
	pStencilOpFailFront = config.pStencilOpFailFront;
	pStencilOpZFailFront = config.pStencilOpZFailFront;
	pStencilOpZPassFront = config.pStencilOpZPassFront;
	pStencilOpFailBack = config.pStencilOpFailBack;
	pStencilOpZFailBack = config.pStencilOpZFailBack;
	pStencilOpZPassBack = config.pStencilOpZPassBack;
	pStencilFunc = config.pStencilFunc;
	pStencilRef = config.pStencilRef;
	pStencilRefMask = config.pStencilRefMask;
	pStencilMask = config.pStencilMask;
	pBlendColor = config.pBlendColor;
	pBlendFuncSource = config.pBlendFuncSource;
	pBlendFuncDest = config.pBlendFuncDest;
	pColorMask[ 0 ] = config.pColorMask[ 0 ];
	pColorMask[ 1 ] = config.pColorMask[ 1 ];
	pColorMask[ 2 ] = config.pColorMask[ 2 ];
	pColorMask[ 3 ] = config.pColorMask[ 3 ];
	pDynamicCullFace = config.pDynamicCullFace;
	pDynamicPolygonOffset = config.pDynamicPolygonOffset;
	pDynamicStencil = config.pDynamicStencil;
	return *this;
}

bool deoglPipelineConfiguration::operator==( const deoglPipelineConfiguration &config ) const{
	return pShader == config.pShader
		&& pEnableRasterizerDiscard == config.pEnableRasterizerDiscard
		&& pPolygonMode == config.pPolygonMode
		&& pCullFace == config.pCullFace
		&& pEnablePolygonOffset == config.pEnablePolygonOffset
		&& pPolygonOffsetFactor == config.pPolygonOffsetFactor
		&& pPolygonOffsetBias == config.pPolygonOffsetBias
		&& pEnableDepthTest == config.pEnableDepthTest
		&& pDepthFunc == config.pDepthFunc
		&& pDepthMask == config.pDepthMask
		&& pEnableStencilTest == config.pEnableStencilTest
		&& pStencilOpFailFront == config.pStencilOpFailFront
		&& pStencilOpZFailFront == config.pStencilOpZFailFront
		&& pStencilOpZPassFront == config.pStencilOpZPassFront
		&& pStencilOpFailBack == config.pStencilOpFailBack
		&& pStencilOpZFailBack == config.pStencilOpZFailBack
		&& pStencilOpZPassBack == config.pStencilOpZPassBack
		&& pStencilFunc == config.pStencilFunc
		&& pStencilRef == config.pStencilRef
		&& pStencilRefMask == config.pStencilRefMask
		&& pStencilMask == config.pStencilMask
		&& pBlendColor.IsEqualTo( config.pBlendColor )
		&& pBlendFuncSource == config.pBlendFuncSource
		&& pBlendFuncDest == config.pBlendFuncDest
		&& pColorMask[ 0 ] == config.pColorMask[ 0 ]
		&& pColorMask[ 1 ] == config.pColorMask[ 1 ]
		&& pColorMask[ 2 ] == config.pColorMask[ 2 ]
		&& pColorMask[ 3 ] == config.pColorMask[ 3 ]
		&& pDynamicCullFace == config.pDynamicCullFace
		&& pDynamicPolygonOffset == config.pDynamicPolygonOffset
		&& pDynamicStencil == config.pDynamicStencil;
}



// Protected Functions
////////////////////////

void deoglPipelineConfiguration::pUpdateStencilSameFrontBack(){
	pStencilSameFrontBack = pStencilOpFailFront == pStencilOpFailBack
		&& pStencilOpZFailFront == pStencilOpZFailBack
		&& pStencilOpZPassFront == pStencilOpZPassBack;
}
