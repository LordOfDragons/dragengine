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
#include "../rendering/task/shared/deoglRenderTaskSharedShader.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"

#include <dragengine/common/exceptions.h>



// Class deoglPipelineConfiguration
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPipelineConfiguration::deoglPipelineConfiguration() :
pEnableScissorTest( false ),
pEnableRasterizerDiscard( false ),
pPolygonMode( GL_FILL ),
pEnableCullFace( false ),
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
pEnableBlend( false ),
pBlendFuncSource( GL_ONE ),
pBlendFuncDest( GL_ZERO ),
pClipControl( false ),
pSPBInstanceIndexBase( -1 ),
pDrawIDOffset( -1 ),
pDynamicCullFace( false ),
pDynamicPolygonOffset( false ),
	pDynamicStencilTest ( false )
{
	pColorMask[ 0 ] = true;
	pColorMask[ 1 ] = true;
	pColorMask[ 2 ] = true;
	pColorMask[ 3 ] = true;
}

deoglPipelineConfiguration::deoglPipelineConfiguration( const deoglPipelineConfiguration &config ) :
pEnableScissorTest( false ),
pEnableRasterizerDiscard( false ),
pPolygonMode( GL_FILL ),
pEnableCullFace( false ),
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
pEnableBlend( false ),
pBlendFuncSource( GL_ONE ),
pBlendFuncDest( GL_ZERO ),
pClipControl( false ),
pSPBInstanceIndexBase( -1 ),
pDrawIDOffset( -1 ),
pDynamicCullFace( false ),
pDynamicPolygonOffset( false ),
	pDynamicStencilTest ( false )
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

void deoglPipelineConfiguration::SetShader( deoglRenderThread &renderThread,
const char *sources, const deoglShaderDefines &defines ){
	deoglShaderManager &sm = renderThread.GetShader().GetShaderManager();
	SetShader( sm.GetProgramWith( sm.GetSourcesNamed( sources ), defines ) );
}

void deoglPipelineConfiguration::SetShader( deoglRenderThread &renderThread,
deoglShaderSources *sources, const deoglShaderDefines &defines ){
	SetShader( renderThread.GetShader().GetShaderManager().GetProgramWith( sources, defines ) );
}

void deoglPipelineConfiguration::EnsureRTSShader(){
	DEASSERT_NOTNULL( pShader );
	
	pShader->EnsureRTSShader();
	pShader->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
}

void deoglPipelineConfiguration::EnsureRTSShader( int drawIDOffset ){
	DEASSERT_NOTNULL( pShader );
	
	pShader->EnsureRTSShader();
	pShader->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
	pShader->GetRTSShader()->SetDrawIDOffset( drawIDOffset );
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

void deoglPipelineConfiguration::SetMasks( bool red, bool green, bool blue, bool alpha, bool depth ){
	SetColorMask( red, green, blue, alpha );
	SetDepthMask( depth );
}



void deoglPipelineConfiguration::SetEnableScissorTest( bool enable ){
	pEnableScissorTest = enable;
}



void deoglPipelineConfiguration::SetEnableRasterizerDiscard( bool enable ){
	pEnableRasterizerDiscard = enable;
}

void deoglPipelineConfiguration::EnableRasterizerDiscard(){
	SetColorMask( false, false, false, false );
	SetDepthMask( false );
	SetEnableRasterizerDiscard( true );
}



void deoglPipelineConfiguration::SetPolygonMode( GLenum mode ){
	pPolygonMode = mode;
}

void deoglPipelineConfiguration::SetEnableCullFace( bool enable ){
	pEnableCullFace = enable;
}

void deoglPipelineConfiguration::SetCullFace( GLenum mode ){
	pCullFace = mode;
}

void deoglPipelineConfiguration::EnableCulling( bool renderBackFaces ){
	pEnableCullFace = true;
	pCullFace = renderBackFaces ? GL_FRONT : GL_BACK;
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

void deoglPipelineConfiguration::EnablePolygonOffset( float factor, float bias ){
	pEnablePolygonOffset = true;
	pPolygonOffsetFactor = factor;
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

void deoglPipelineConfiguration::EnableDepthTestAlways(){
	EnableDepthTest( GL_ALWAYS );
}

void deoglPipelineConfiguration::EnableDepthTestLessEqual(){
	EnableDepthTest( GL_LEQUAL );
}

void deoglPipelineConfiguration::EnableDepthTest( GLenum function ){
	pEnableDepthTest = true;
	pDepthFunc = function;
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
}

void deoglPipelineConfiguration::SetStencilRefMask( int mask ){
	pStencilRefMask = mask;
}

void deoglPipelineConfiguration::SetStencilMask( int mask ){
	pStencilMask = mask;
}

void deoglPipelineConfiguration::SetStencil( GLenum func, int ref, int refMask, int mask ){
	pStencilFunc = func;
	pStencilRef = ref;
	pStencilRefMask = refMask;
	pStencilMask = mask;
}

void deoglPipelineConfiguration::SetStencilOp( GLenum opFail, GLenum opZFail, GLenum opZPass ){
	pStencilOpFailBack = pStencilOpFailFront = opFail;
	pStencilOpZFailBack = pStencilOpZFailFront = opZFail;
	pStencilOpZPassBack = pStencilOpZPassFront = opZPass;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetStencilOpFront( GLenum opFail, GLenum opZFail, GLenum opZPass ){
	pStencilOpFailFront = opFail;
	pStencilOpZFailFront = opZFail;
	pStencilOpZPassFront = opZPass;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::SetStencilOpBack( GLenum opFail, GLenum opZFail, GLenum opZPass ){
	pStencilOpFailBack = opFail;
	pStencilOpZFailBack = opZFail;
	pStencilOpZPassBack = opZPass;
	pUpdateStencilSameFrontBack();
}

void deoglPipelineConfiguration::EnableDynamicStencilTest(){
	SetEnableStencilTest( true );
	SetDynamicStencilTest ( true );
}



void deoglPipelineConfiguration::SetEnableBlend( bool enable ){
	pEnableBlend = enable;
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

void deoglPipelineConfiguration::EnableBlendReplace(){
	EnableBlend( GL_ONE, GL_ZERO );
}

void deoglPipelineConfiguration::EnableBlendBlend(){
	EnableBlend( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void deoglPipelineConfiguration::EnableBlendAdd(){
	EnableBlend( GL_ONE, GL_ONE );
}

void deoglPipelineConfiguration::EnableBlendTranspAdd(){
	EnableBlend( GL_SRC_ALPHA, GL_ONE );
}

void deoglPipelineConfiguration::EnableBlend( GLenum source, GLenum dest ){
	pEnableBlend = true;
	pBlendFuncSource = source;
	pBlendFuncDest = dest;
}



void deoglPipelineConfiguration::SetClipControl( bool clipControl ){
	pClipControl = clipControl;
}



void deoglPipelineConfiguration::SetSPBInstanceIndexBase( int parameter ){
	pSPBInstanceIndexBase = parameter;
}

void deoglPipelineConfiguration::SetDrawIDOffset( int parameter ){
	pDrawIDOffset = parameter;
}



void deoglPipelineConfiguration::SetDynamicCullFace( bool dynamic ){
	pDynamicCullFace = dynamic;
}

void deoglPipelineConfiguration::SetDynamicPolygonOffset( bool dynamic ){
	pDynamicPolygonOffset = dynamic;
}

void deoglPipelineConfiguration::SetDynamicStencilTest( bool dynamic ){
	pDynamicStencilTest = dynamic;
}



void deoglPipelineConfiguration::Reset(){
	*this = deoglPipelineConfiguration();
}



void deoglPipelineConfiguration::Activate( deoglRenderThread &renderThread ) const{
	#define ENABLE_GL_STATE(flag,state) \
		if( flag ){ \
			OGL_CHECK( renderThread, glEnable( state ) ); \
		} else { \
			OGL_CHECK( renderThread, glDisable( state ) ); \
		}
	
	renderThread.GetShader().ActivateShader( pShader ); // shader can be null which is allowed
	
	OGL_CHECK( renderThread, glColorMask( pColorMask[ 0 ], pColorMask[ 1 ], pColorMask[ 2 ], pColorMask[ 3 ] ) );
	OGL_CHECK( renderThread, glDepthMask( pDepthMask ) );
	
	ENABLE_GL_STATE( pEnableScissorTest, GL_SCISSOR_TEST )
	
	ENABLE_GL_STATE( pEnableRasterizerDiscard, GL_RASTERIZER_DISCARD )
	
	OGL_CHECK( renderThread, glPolygonMode( GL_FRONT_AND_BACK, pPolygonMode ) );
	
	ENABLE_GL_STATE( pEnableCullFace, GL_CULL_FACE )
	if( pEnableCullFace && ! pDynamicCullFace ){
		OGL_CHECK( renderThread, glCullFace( pCullFace ) );
	}
	
	ENABLE_GL_STATE( pEnablePolygonOffset, GL_POLYGON_OFFSET_FILL )
	if( pEnablePolygonOffset && ! pDynamicPolygonOffset ){
		OGL_CHECK( renderThread, pglPolygonOffset( pPolygonOffsetFactor, pPolygonOffsetBias ) );
	}
	
	ENABLE_GL_STATE( pEnableDepthTest, GL_DEPTH_TEST )
	if( pEnableDepthTest ){
		OGL_CHECK( renderThread, glDepthFunc( pDepthFunc ) );
	}
	
	ENABLE_GL_STATE( pEnableStencilTest, GL_STENCIL_TEST )
	if( pEnableStencilTest && ! pDynamicStencilTest ){
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
	
	ENABLE_GL_STATE( pEnableBlend, GL_BLEND )
	if( pEnableBlend ){
		OGL_CHECK( renderThread, pglBlendColor( pBlendColor.r, pBlendColor.g, pBlendColor.b, pBlendColor.a ) );
		OGL_CHECK( renderThread, glBlendFunc( pBlendFuncSource, pBlendFuncDest ) );
	}
	
	if( pglClipControl ){
		pglClipControl( GL_LOWER_LEFT, pClipControl ? GL_ZERO_TO_ONE : GL_NEGATIVE_ONE_TO_ONE );
	}
	
	#undef ENABLE_GL_STATE
}



// Operators
//////////////

deoglPipelineConfiguration &deoglPipelineConfiguration::operator=( const deoglPipelineConfiguration &config ){
	pShader = config.pShader;
	pColorMask[ 0 ] = config.pColorMask[ 0 ];
	pColorMask[ 1 ] = config.pColorMask[ 1 ];
	pColorMask[ 2 ] = config.pColorMask[ 2 ];
	pColorMask[ 3 ] = config.pColorMask[ 3 ];
	pEnableScissorTest = config.pEnableScissorTest;
	pEnableRasterizerDiscard = config.pEnableRasterizerDiscard;
	pPolygonMode = config.pPolygonMode;
	pEnableCullFace = config.pEnableCullFace;
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
	pEnableBlend = config.pEnableBlend;
	pBlendColor = config.pBlendColor;
	pBlendFuncSource = config.pBlendFuncSource;
	pBlendFuncDest = config.pBlendFuncDest;
	pClipControl = config.pClipControl;
	pSPBInstanceIndexBase = config.pSPBInstanceIndexBase;
	pDrawIDOffset = config.pDrawIDOffset;
	pDynamicCullFace = config.pDynamicCullFace;
	pDynamicPolygonOffset = config.pDynamicPolygonOffset;
	pDynamicStencilTest = config.pDynamicStencilTest;
	return *this;
}

bool deoglPipelineConfiguration::operator==( const deoglPipelineConfiguration &config ) const{
	return pShader == config.pShader
		&& pColorMask[ 0 ] == config.pColorMask[ 0 ]
		&& pColorMask[ 1 ] == config.pColorMask[ 1 ]
		&& pColorMask[ 2 ] == config.pColorMask[ 2 ]
		&& pColorMask[ 3 ] == config.pColorMask[ 3 ]
		&& pEnableScissorTest == config.pEnableScissorTest
		&& pEnableRasterizerDiscard == config.pEnableRasterizerDiscard
		&& pPolygonMode == config.pPolygonMode
		&& pEnableCullFace == config.pEnableCullFace
		&& pCullFace == config.pCullFace
		&& pEnablePolygonOffset == config.pEnablePolygonOffset
		&& fabsf( pPolygonOffsetFactor - config.pPolygonOffsetFactor ) < FLOAT_SAFE_EPSILON
		&& fabsf( pPolygonOffsetBias - config.pPolygonOffsetBias ) < FLOAT_SAFE_EPSILON
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
		&& pEnableBlend == config.pEnableBlend
		&& pBlendColor.IsEqualTo( config.pBlendColor )
		&& pBlendFuncSource == config.pBlendFuncSource
		&& pBlendFuncDest == config.pBlendFuncDest
		&& pClipControl == config.pClipControl
		&& pSPBInstanceIndexBase == config.pSPBInstanceIndexBase
		&& pDrawIDOffset == config.pDrawIDOffset
		&& pDynamicCullFace == config.pDynamicCullFace
		&& pDynamicPolygonOffset == config.pDynamicPolygonOffset
		&& pDynamicStencilTest == config.pDynamicStencilTest;
}



// Protected Functions
////////////////////////

void deoglPipelineConfiguration::pUpdateStencilSameFrontBack(){
	pStencilSameFrontBack = pStencilOpFailFront == pStencilOpFailBack
		&& pStencilOpZFailFront == pStencilOpZFailBack
		&& pStencilOpZPassFront == pStencilOpZPassBack;
}
