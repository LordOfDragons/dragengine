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

#ifndef _DEOGLPIPELINESTATE_H_
#define _DEOGLPIPELINESTATE_H_

#include "../deoglBasics.h"

class deoglRenderThread;


/**
 * Pipeline state.
 */
class deoglPipelineState{
private:
	deoglRenderThread &pRenderThread;
	
	bool pColorMask[ 4 ];
	bool pDepthMask;
	bool pEnableScissorTest;
	GLenum pPolygonMode;
	bool pEnableCullFace;
	GLenum pCullFace;
	bool pEnablePolygonOffsetFill;
	float pPolygonOffsetFactor;
	float pPolygonOffsetBias;
	bool pEnableDepthTest;
	GLenum pDepthFunc;
	bool pEnableStencilTest;
	GLenum pStencilOpFailFront;
	GLenum pStencilOpZFailFront;
	GLenum pStencilOpZPassFront;
	GLenum pStencilOpFailBack;
	GLenum pStencilOpZFailBack;
	GLenum pStencilOpZPassBack;
	GLenum pStencilFunc;
	GLint pStencilRef;
	GLuint pStencilRefMask;
	GLuint pStencilMask;
	bool pEnableBlend;
	float pBlendColor[ 4 ];
	GLenum pBlendFuncSource;
	GLenum pBlendFuncDest;
	bool pClipControl;
	bool pEnableDepthClamp;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline state. */
	deoglPipelineState( deoglRenderThread &renderThread );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	void Reset();
	
	void ColorMask( bool red, bool green, bool blue, bool alpha );
	void DepthMask( bool mask );
	void EnableScissorTest( bool enable );
	void PolygonMode( GLenum mode );
	void EnableCullFace( bool enable );
	void CullFace( GLenum cullFace );
	void EnablePolygonOffsetFill( bool enable );
	void PolygonOffset( float factor, float bias );
	void EnableDepthTest( bool enable );
	void DepthFunc( GLenum depthFunc );
	void EnableStencilTest( bool enable );
	void StencilOp( GLenum fail, GLenum zfail, GLenum zpass );
	void StencilOpFront( GLenum fail, GLenum zfail, GLenum zpass );
	void StencilOpBack( GLenum fail, GLenum zfail, GLenum zpass );
	void StencilFunc( GLenum func, GLint ref, GLuint mask );
	void StencilMask( GLuint mask );
	void EnableBlend( bool enable );
	void BlendColor( float red, float green, float blue, float alpha );
	void BlendFunc( GLenum sfactor, GLenum dfactor );
	void ClipControl( bool clipControl );
	void EnableDepthClamp( bool enable );
	/*@}*/
};

#endif
