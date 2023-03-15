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
