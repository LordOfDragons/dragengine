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


/**
 * Pipeline configuration for OpenGL.
 */
class deoglPipelineConfiguration{
private:
	deoglShaderProgramUsage pShader;
	
	bool pEnableRasterizerDiscard;
	
	GLenum pPolygonMode;
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
	
	decColor pBlendColor;
	GLenum pBlendFuncSource;
	GLenum pBlendFuncDest;
	bool pColorMask[ 4 ];
	
	bool pDynamicCullFace;
	bool pDynamicPolygonOffset;
	bool pDynamicStencil;
	
	
	
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
	
	
	
	/** Enable rasterizer discard. */
	inline bool GetEnableRasterizerDiscard() const{ return pEnableRasterizerDiscard; }
	void SetEnableRasterizerDiscard( bool enable );
	
	
	
	/** Polygon mode. */
	inline GLenum GetPolygonMode() const{ return pPolygonMode; }
	void SetPolygonMode( GLenum mode );
	
	/** Cull face. */
	inline GLenum GetCullFace() const{ return pCullFace; }
	void SetCullFace( GLenum mode );
	
	
	
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
	
	
	
	/** Blend color. */
	inline const decColor &GetBlendColor() const{ return pBlendColor; }
	void SetBlendColor( const decColor &color );
	
	/** Source blend function. */
	inline GLenum GetBlendFuncSource() const{ return pBlendFuncSource; }
	void SetBlendFuncSource( GLenum mode );
	
	/** Destination blend function. */
	inline GLenum GetBlendFuncDest() const{ return pBlendFuncDest; }
	void SetBlendFuncDest( GLenum mode );
	
	/** Color mask. */
	bool GetColorMask( int component ) const;
	void SetColorMask( bool red, bool green, bool blue, bool alpha );
	
	
	
	/** Dynamic cull face. */
	inline bool GetDynamicCullFace() const{ return pDynamicCullFace; }
	void SetDynamicCullFace( bool dynamic );
	
	/** Dynamic polygon offset. */
	inline bool GetDynamicPolygonOffset() const{ return pDynamicPolygonOffset; }
	void SetDynamicPolygonOffset( bool dynamic );
	
	/** Dynamic stencil. */
	inline bool GetDynamicStencil() const{ return pDynamicStencil; }
	void SetDynamicStencil( bool dynamic );
	
	
	
	/** Activate pipeline. Sets shader and states that are not dynamic. */
	void Activate( deoglRenderThread &renderThread );
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
