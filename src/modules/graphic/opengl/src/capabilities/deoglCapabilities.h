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

#ifndef _DEOGLCAPABILITIES_H_
#define _DEOGLCAPABILITIES_H_

#include "deoglCapsTextureFormatList.h"
#include "deoglCapsFmtSupport.h"
#include "checks/deoglCapCheckATLUnbind.h"
#include "checks/deoglCapCheckUBOIndirectMatrixAccess.h"
#include "checks/deoglCapCheckRasterizerDiscard.h"
#include "checks/deoglCapCheckClearEntireCubeMap.h"
#include "checks/deoglCapCheckClearEntireArrayTexture.h"
#include "checks/deoglCapCheckGeometryShaderLayer.h"
#include "checks/deoglCapCheckUBODirectLinkDeadloop.h"
#include "checks/deoglCapCheckFramebufferTextureSingle.h"
#include "../deoglBasics.h"

class deoglRenderThread;
class decUnicodeString;
class deoglCapsTextureFormat;
class deoglFramebuffer;



/**
 * \brief OpenGL capabilities.
 * 
 * Stores the capabilities of the hardware. The capabilities are tested during
 * start up time using opengl getter calls where possible or else trial-and-error
 * to detect what is working.
 */
class deoglCapabilities{
private:
	deoglRenderThread &pRenderThread;
	
	GLuint pFSQuadVBO;
	GLuint pFSQuadVAO;
	
	deoglCapsFmtSupport pFormats;
	
	int pMaxDrawBuffers;
	int pUBOMaxSize;
	int pSSBOMaxSize;
	int pSSBOMaxBlocksVertex;
	int pSSBOMaxBlocksFragment;
	int pSSBOMaxBlocksGeometry;
	int pUBOOffsetAlignment;
	int pGeometryShaderMaxVertices;
	int pGeometryShaderMaxComponents;
	
	deoglCapCheckATLUnbind pATLUnbind;
	deoglCapCheckUBOIndirectMatrixAccess pUBOIndirectMatrixAccess;
	deoglCapCheckRasterizerDiscard pRasterizerDiscard;
	deoglCapCheckClearEntireCubeMap pClearEntireCubeMap;
	deoglCapCheckClearEntireArrayTexture pClearEntireArrayTexture;
	deoglCapCheckGeometryShaderLayer pGeometryShaderLayer;
	deoglCapCheckUBODirectLinkDeadloop pUBODirectLinkDeadloop;
	deoglCapCheckFramebufferTextureSingle pFramebufferTextureSingle;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create capabilities object. */
	deoglCapabilities( deoglRenderThread &renderThread );
	
	/** \brief Clean up capabilities object. */
	~deoglCapabilities();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Texture format support. */
	inline const deoglCapsFmtSupport &GetFormats() const{ return pFormats; }
	
	
	
	/**
	 * \brief Maximum number of draw buffers available by the hardware.
	 * 
	 * OpenGL Spec requires a minimum value of \em 8.
	 * OpenGL EM Spec requires a minimum value of \em 4.
	 */
	inline int GetMaxDrawBuffers() const{ return pMaxDrawBuffers; }
	
	/**
	 * \brief Maximum size of UBOs.
	 * 
	 * OpenGL Spec requires a minimum value of \em 16384.
	 */
	inline int GetUBOMaxSize() const{ return pUBOMaxSize; }
	
	/**
	 * \brief Maximum size of SSBOs.
	 * 
	 * OpenGL Spec requires a minimum value of \em 16777216.
	 */
	inline int GetSSBOMaxSize() const{ return pSSBOMaxSize; }
	
	/** \brief Maximum number of supported SSBO per vertex shader. */
	inline int GetSSBOMaxBlocksVertex() const{ return pSSBOMaxBlocksVertex; }
	
	/** \brief Maximum number of supported SSBO per fragment shader. */
	inline int GetSSBOMaxBlocksFragment() const{ return pSSBOMaxBlocksFragment; }
	
	/** \brief Maximum number of supported SSBO per geometry shader. */
	inline int GetSSBOMaxBlocksGeometry() const{ return pSSBOMaxBlocksGeometry; }
	
	/** \brief Uniform buffer offset alignment. */
	inline int GetUBOOffsetAlignment() const{ return pUBOOffsetAlignment; }
	
	/**
	 * \brief Maximum number of vertices a geometry shader can emit.
	 * 
	 * OpenGL Spec requires a minimum value of \em 256.
	 */
	inline int GetGeometryShaderMaxVertices() const{ return pGeometryShaderMaxVertices; }
	
	/**
	 * \brief Maximum number of components per invocation a geometry shader can emit.
	 * 
	 * OpenGL Spec requires a minimum value of \em 1024.
	 */
	inline int GetGeometryShaderMaxComponents() const{ return pGeometryShaderMaxComponents; }
	
	
	
	/** \brief Array texture layer check. */
	inline const deoglCapCheckATLUnbind &ATLUnbind() const{ return pATLUnbind; }
	
	/** \brief Indirect UBO matrix access check. */
	inline const deoglCapCheckUBOIndirectMatrixAccess &GetUBOIndirectMatrixAccess() const{
		return pUBOIndirectMatrixAccess; }
	
	/** \brief Rasterizer discard. */
	inline const deoglCapCheckRasterizerDiscard &GetRasterizerDiscard() const{
		return pRasterizerDiscard; }
	
	/** \brief Clear entire cube map. */
	inline const deoglCapCheckClearEntireCubeMap &GetClearEntireCubeMap() const{
		return pClearEntireCubeMap; }
	
	/** \brief Cleare all layers in an array texture. */
	inline const deoglCapCheckClearEntireArrayTexture &GetClearEntireArrayTexture() const{
		return pClearEntireArrayTexture; }
	
	/** \brief glLayer geometry shaders handling. */
	inline const deoglCapCheckGeometryShaderLayer &GetGeometryShaderLayer() const{
		return pGeometryShaderLayer; }
	
	/** \brief UBO direct linking dead-loop. */
	inline const deoglCapCheckUBODirectLinkDeadloop &GetUBODirectLinkDeadloop() const{
		return pUBODirectLinkDeadloop; }
	
	/** \brief Framebuffer texture single. */
	inline const deoglCapCheckFramebufferTextureSingle &GetFramebufferTextureSingle() const{
		return pFramebufferTextureSingle; }
	
	/** Tests the hardware for its capabilities. */
	void DetectCapabilities();
	
	/** \brief Full-Screen Quad VAO for internal testing only. */
	inline GLuint GetFSQuadVAO() const{ return pFSQuadVAO; }
	
	/*@}*/
	
private:
	#ifdef OS_ANDROID
	void pAndroidTest( deoglFramebuffer *framebuffer );
	#endif
};

#endif
