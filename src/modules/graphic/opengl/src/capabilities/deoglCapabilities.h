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
 * OpenGL capabilities.
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
	
	int pMaxTextureSize;
	int pMax3DTextureSize;
	int pMaxDrawBuffers;
	int pUBOMaxSize;
	int pTBOMaxSize;
	int pSSBOMaxSize;
	int pSSBOMaxBlocksVertex;
	int pSSBOMaxBlocksFragment;
	int pSSBOMaxBlocksGeometry;
	int pSSBOMaxBlocksCompute;
	int pUBOOffsetAlignment;
	int pGeometryShaderMaxVertices;
	int pGeometryShaderMaxComponents;
	int pNumProgramBinaryFormats;
	
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
	/** Create capabilities object. */
	deoglCapabilities( deoglRenderThread &renderThread );
	
	/** Clean up capabilities object. */
	~deoglCapabilities();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Texture format support. */
	inline const deoglCapsFmtSupport &GetFormats() const{ return pFormats; }
	
	
	
	/** Maximum texture size. */
	inline int GetMaxTextureSize() const{ return pMaxTextureSize; }
	
	/** Maximum 3D texture size. */
	inline int GetMax3DTextureSize() const{ return pMax3DTextureSize; }
	
	/**
	 * Maximum number of draw buffers available by the hardware.
	 * 
	 * OpenGL Spec requires a minimum value of \em 8.
	 * OpenGL EM Spec requires a minimum value of \em 4.
	 */
	inline int GetMaxDrawBuffers() const{ return pMaxDrawBuffers; }
	
	/** Maximum size of UBOs. OpenGL Spec requires a minimum value of \em 16384. */
	inline int GetUBOMaxSize() const{ return pUBOMaxSize; }
	
	/** Maximum size of TBOs. OpenGL Spec requires a minimum value of \em 128000. */
	inline int GetTBOMaxSize() const{ return pTBOMaxSize; }
	
	/** Maximum size of SSBOs. OpenGL Spec requires a minimum value of \em 16777216. */
	inline int GetSSBOMaxSize() const{ return pSSBOMaxSize; }
	
	/** Maximum number of supported SSBO per vertex shader. */
	inline int GetSSBOMaxBlocksVertex() const{ return pSSBOMaxBlocksVertex; }
	
	/** Maximum number of supported SSBO per fragment shader. */
	inline int GetSSBOMaxBlocksFragment() const{ return pSSBOMaxBlocksFragment; }
	
	/** Maximum number of supported SSBO per geometry shader. */
	inline int GetSSBOMaxBlocksGeometry() const{ return pSSBOMaxBlocksGeometry; }
	
	/** Maximum number of supported SSBO per geometry shader. */
	inline int GetSSBOMaxBlocksCompute() const{ return pSSBOMaxBlocksCompute; }
	
	/** Uniform buffer offset alignment. */
	inline int GetUBOOffsetAlignment() const{ return pUBOOffsetAlignment; }
	
	/**
	 * Maximum number of vertices a geometry shader can emit.
	 * 
	 * OpenGL Spec requires a minimum value of \em 256.
	 */
	inline int GetGeometryShaderMaxVertices() const{ return pGeometryShaderMaxVertices; }
	
	/**
	 * Maximum number of components per invocation a geometry shader can emit.
	 * 
	 * OpenGL Spec requires a minimum value of \em 1024.
	 */
	inline int GetGeometryShaderMaxComponents() const{ return pGeometryShaderMaxComponents; }
	
	/** Count of binary shader formats the driver supports. */
	inline int GetNumProgramBinaryFormats() const{ return pNumProgramBinaryFormats; }
	
	
	
	/** Array texture layer check. */
	inline const deoglCapCheckATLUnbind &ATLUnbind() const{ return pATLUnbind; }
	
	/** Indirect UBO matrix access check. */
	inline const deoglCapCheckUBOIndirectMatrixAccess &GetUBOIndirectMatrixAccess() const{
		return pUBOIndirectMatrixAccess; }
	
	/** Rasterizer discard. */
	inline const deoglCapCheckRasterizerDiscard &GetRasterizerDiscard() const{
		return pRasterizerDiscard; }
	
	/** Clear entire cube map. */
	inline const deoglCapCheckClearEntireCubeMap &GetClearEntireCubeMap() const{
		return pClearEntireCubeMap; }
	
	/** Cleare all layers in an array texture. */
	inline const deoglCapCheckClearEntireArrayTexture &GetClearEntireArrayTexture() const{
		return pClearEntireArrayTexture; }
	
	/** glLayer geometry shaders handling. */
	inline const deoglCapCheckGeometryShaderLayer &GetGeometryShaderLayer() const{
		return pGeometryShaderLayer; }
	
	/** UBO direct linking dead-loop. */
	inline const deoglCapCheckUBODirectLinkDeadloop &GetUBODirectLinkDeadloop() const{
		return pUBODirectLinkDeadloop; }
	
	/** Framebuffer texture single. */
	inline const deoglCapCheckFramebufferTextureSingle &GetFramebufferTextureSingle() const{
		return pFramebufferTextureSingle; }
	
	/** Tests the hardware for its capabilities. */
	void DetectCapabilities();
	
	/** Full-Screen Quad VAO for internal testing only. */
	inline GLuint GetFSQuadVAO() const{ return pFSQuadVAO; }
	
	/*@}*/
	
private:
	#ifdef OS_ANDROID
	void pAndroidTest( deoglFramebuffer *framebuffer );
	#endif
};

#endif
