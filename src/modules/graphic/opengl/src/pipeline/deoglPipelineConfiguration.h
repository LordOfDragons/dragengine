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

#ifndef _DEOGLPIPELINECONFIGURATION_H_
#define _DEOGLPIPELINECONFIGURATION_H_

#include "../deoglBasics.h"
#include "../shaders/deoglShaderProgramUsage.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglShaderSources;
class deoglShaderDefines;


/**
 * Pipeline configuration for OpenGL.
 * 
 * Default values:
 * - EnableStencilTest: false
 * - EnableRasterizerDiscard: false
 * - PolygonMode: GL_FILL
 * - EnableCullFace: false
 * - CullFace: GL_BACK
 * - EnablePolygonOffset: false
 * - PolygonOffsetFactor: 0
 * - PolygonOffsetBias: 0
 * - EnableDepthTest: false
 * - DepthFunc: GL_LESS
 * - DepthMask: true
 * - EnableStencilTest: false
 * - StencilOpFailFront: GL_KEEP
 * - StencilOpZFailFront: GL_KEEP
 * - StencilOpZPassFront: GL_KEEP
 * - StencilOpFailBack: GL_KEEP
 * - StencilOpZFailBack: GL_KEEP
 * - StencilOpZPassBack: GL_KEEP
 * - StencilFunc: GL_ALWAYS
 * - StencilRef: 0
 * - StencilRefMask: ~0
 * - StencilMask: ~0
 * - StencilSameFrontBack: true
 * - BlendFuncSource: GL_ONE
 * - BlendFuncDest: GL_ZERO
 * - ClipControl:
 *   - true: GL_LOWER_LEFT, GL_ZERO_TO_ONE
 *   - false: GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE
 * - DynamicCullFace: false
 * - DynamicPolygonOffset: false
 * - DynamicStencil: false
 */
class deoglPipelineConfiguration{
private:
	deoglShaderProgramUsage pShader;
	
	bool pColorMask[ 4 ];
	
	bool pEnableScissorTest;
	
	bool pEnableRasterizerDiscard;
	
	GLenum pPolygonMode;
	bool pEnableCullFace;
	GLenum pCullFace;
	
	bool pEnablePolygonOffset;
	float pPolygonOffsetFactor;
	float pPolygonOffsetBias;
	
	bool pEnableDepthTest;
	GLenum pDepthFunc;
	bool pDepthMask;
	
	bool pEnableStencilTest;
	GLenum pStencilOpFailFront;
	GLenum pStencilOpZFailFront;
	GLenum pStencilOpZPassFront;
	GLenum pStencilOpFailBack;
	GLenum pStencilOpZFailBack;
	GLenum pStencilOpZPassBack;
	GLenum pStencilFunc;
	int pStencilRef;
	int pStencilRefMask;
	int pStencilMask;
	bool pStencilSameFrontBack;
	
	bool pEnableBlend;
	decColor pBlendColor;
	GLenum pBlendFuncSource;
	GLenum pBlendFuncDest;
	
	bool pClipControl;
	
	bool pDynamicCullFace;
	bool pDynamicPolygonOffset;
	bool pDynamicStencilTest;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline configuration. */
	deoglPipelineConfiguration();
	
	deoglPipelineConfiguration( const deoglPipelineConfiguration &configuration );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Shader program. */
	inline const deoglShaderProgramUsage &GetShader() const{ return pShader; }
	
	void SetShader( const deoglShaderProgramUsage &shader );
	
	void SetShader( deoglRenderThread &renderThread,
		const char *sources, const deoglShaderDefines &defines );
	
	void SetShader( deoglRenderThread &renderThread,
		deoglShaderSources *sources, const deoglShaderDefines &defines );
	
	/** Ensure RTS shader exists. */
	void EnsureRTSShader();
	void EnsureRTSShader( int drawIDOffset );
	
	
	
	/** Color mask. */
	bool GetColorMask( int component ) const;
	void SetColorMask( bool red, bool green, bool blue, bool alpha );
	
	/** Set color and depth mask. */
	void SetMasks( bool red, bool green, bool blue, bool alpha, bool depth );
	
	
	
	/** Enable scissor test. */
	inline bool GetEnableScissorTest() const{ return pEnableScissorTest; }
	void SetEnableScissorTest( bool enable );
	
	
	
	/** Enable rasterizer discard. */
	inline bool GetEnableRasterizerDiscard() const{ return pEnableRasterizerDiscard; }
	void SetEnableRasterizerDiscard( bool enable );
	
	/** Enable rasterizer discard and set color and depth masks to false. */
	void EnableRasterizerDiscard();
	
	
	
	/** Polygon mode. */
	inline GLenum GetPolygonMode() const{ return pPolygonMode; }
	void SetPolygonMode( GLenum mode );
	
	/** Enable cull face. */
	inline bool GetEnableCullFace() const{ return pEnableCullFace; }
	void SetEnableCullFace( bool enable );
	
	/** Cull face. */
	inline GLenum GetCullFace() const{ return pCullFace; }
	void SetCullFace( GLenum mode );
	
	/** Enabl culling. */
	void EnableCulling( bool renderBackFaces );
	
	
	
	/** Enable polygon offset. */
	inline bool GetEnablePolygonOffset() const{ return pEnablePolygonOffset; }
	void SetEnablePolygonOffset( bool enable );
	
	/** Polygon offset factor. */
	inline float GetPolygonOffsetFactor() const{ return pPolygonOffsetFactor; }
	void SetPolygonOffsetFactor( float factor );
	
	/** Polygon offset bias. */
	inline float GetPolygonOffsetBias() const{ return pPolygonOffsetBias; }
	void SetPolygonOffsetBias( float bias );
	
	
	
	/** Enable depth test. */
	inline bool GetEnableDepthTest() const{ return pEnableDepthTest; }
	void SetEnableDepthTest( bool enable );
	
	/** Depth test function. */
	inline GLenum GetDepthFunc() const{ return pDepthFunc; }
	void SetDepthFunc( GLenum mode );
	
	/** Depth mask. */
	inline bool GetDepthMask() const{ return pDepthMask; }
	void SetDepthMask( bool mask );
	
	/** Enable depth test with function always. */
	void EnableDepthTestAlways();
	
	/** Enable depth test with function less-equal. */
	void EnableDepthTestLessEqual();
	
	
	
	/** Enable stencil test. */
	inline bool GetEnableStencilTest() const{ return pEnableStencilTest; }
	void SetEnableStencilTest( bool enable );
	
	/** Stencil op fail front. */
	inline GLenum GetStencilOpFailFront() const{ return pStencilOpFailFront; }
	void SetStencilOpFailFront( GLenum mode );
	
	/** Stencil op zfail front. */
	inline GLenum GetStencilOpZFailFront() const{ return pStencilOpZFailFront; }
	void SetStencilOpZFailFront( GLenum mode );
	
	/** Stencil op zpass front. */
	inline GLenum GetStencilOpZPassFront() const{ return pStencilOpZPassFront; }
	void SetStencilOpZPassFront( GLenum mode );
	
	/** Stencil op fail back. */
	inline GLenum GetStencilOpFailBack() const{ return pStencilOpFailBack; }
	void SetStencilOpFailBack( GLenum mode );
	
	/** Stencil op zfail back. */
	inline GLenum GetStencilOpZFailBack() const{ return pStencilOpZFailBack; }
	void SetStencilOpZFailBack( GLenum mode );
	
	/** Stencil op zpass back. */
	inline GLenum GetStencilOpZPassBack() const{ return pStencilOpZPassBack; }
	void SetStencilOpZPassBack( GLenum mode );
	
	/** Stencil function. */
	inline GLenum GetStencilFunc() const{ return pStencilFunc; }
	void SetStencilFunc( GLenum mode );
	
	/** Stencil reference value. */
	inline int GetStencilRef() const{ return pStencilRef; }
	void SetStencilRef( int ref );
	
	/** Stencil reference mask. */
	inline int GetStencilRefMask() const{ return pStencilRefMask; }
	void SetStencilRefMask( int mask );
	
	/** Stencil mask. */
	inline int GetStencilMask() const{ return pStencilMask; }
	void SetStencilMask( int mask );
	
	/** Set stencil parameters. */
	void SetStencil( GLenum func, int ref, int refMask, int mask );
	
	/** Set stencil parameters for front and back. */
	void SetStencilOp( GLenum opFail, GLenum opZFail, GLenum opZPass );
	
	/** Set stencil parameters for front. */
	void SetStencilOpFront( GLenum opFail, GLenum opZFail, GLenum opZPass );
	
	/** Set stencil parameters for back. */
	void SetStencilOpBack( GLenum opFail, GLenum opZFail, GLenum opZPass );
	
	/** Enable dynamic stencil test. */
	void EnableDynamicStencilTest();
	
	
	
	/** Clip control. */
	inline bool GetClipControl() const{ return pClipControl; }
	void SetClipControl( bool clipControl );
	
	
	
	/** Enable blend. */
	inline bool GetEnableBlend() const{ return pEnableBlend; }
	void SetEnableBlend( bool enable );
	
	/** Blend color. */
	inline const decColor &GetBlendColor() const{ return pBlendColor; }
	void SetBlendColor( const decColor &color );
	
	/** Source blend function. */
	inline GLenum GetBlendFuncSource() const{ return pBlendFuncSource; }
	void SetBlendFuncSource( GLenum mode );
	
	/** Destination blend function. */
	inline GLenum GetBlendFuncDest() const{ return pBlendFuncDest; }
	void SetBlendFuncDest( GLenum mode );
	
	/** Enable blend with function GL_ONE / GL_ZERO. */
	void EnableBlendReplace();
	
	/** Enable blend with function GL_SRC_ALPHA / GL_ONE_MINUS_SRC_ALPHA. */
	void EnableBlendBlend();
	
	/** Enable blend with function GL_ONE / GL_ONE. */
	void EnableBlendAdd();
	
	/** Enable blend with function GL_SRC_ALPHA / GL_ONE. */
	void EnableBlendTranspAdd();
	
	/** Enable blend with function. */
	void EnableBlend( GLenum source, GLenum dest );
	
	
	
	/** Dynamic cull face. */
	inline bool GetDynamicCullFace() const{ return pDynamicCullFace; }
	void SetDynamicCullFace( bool dynamic );
	
	/** Dynamic polygon offset. */
	inline bool GetDynamicPolygonOffset() const{ return pDynamicPolygonOffset; }
	void SetDynamicPolygonOffset( bool dynamic );
	
	/** Dynamic stencil. */
	inline bool GetDynamicStencilTest() const{ return pDynamicStencilTest; }
	void SetDynamicStencilTest( bool dynamic );
	
	
	
	/** Reset to default parameters. */
	void Reset();
	
	
	
	/** Activate pipeline. Sets shader and states that are not dynamic. */
	void Activate( deoglRenderThread &renderThread ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==( const deoglPipelineConfiguration &configuration ) const;
	
	/** Copy configuration. */
	deoglPipelineConfiguration &operator=( const deoglPipelineConfiguration &configuration );
	/*@}*/
	
	
	
protected:
	void pUpdateStencilSameFrontBack();
};

#endif
