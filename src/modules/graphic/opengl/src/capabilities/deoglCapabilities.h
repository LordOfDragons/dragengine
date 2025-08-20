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

#ifndef _DEOGLCAPABILITIES_H_
#define _DEOGLCAPABILITIES_H_

#include "deoglCapsTextureFormatList.h"
#include "deoglCapsFmtSupport.h"
#include "checks/deoglCapCheckATLUnbind.h"
#include "checks/deoglCapCheckUBOIndirectMatrixAccess.h"
#include "checks/deoglCapCheckClearEntireArrayTexture.h"
#include "checks/deoglCapCheckUBODirectLinkDeadloop.h"
#include "checks/deoglCapCheckFramebufferTextureSingle.h"
#include "checks/deoglCapCheckStd430.h"
#include "../deoglBasics.h"

#include <dragengine/common/math/decMath.h>

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
	int pMaxComputeWorkGroupInvocations;
	decPoint3 pMaxComputeWorkGroupSize;
	int pMaxTextureImageUnits;
	
	deoglCapCheckATLUnbind pATLUnbind;
	deoglCapCheckUBOIndirectMatrixAccess pUBOIndirectMatrixAccess;
	deoglCapCheckClearEntireArrayTexture pClearEntireArrayTexture;
	deoglCapCheckUBODirectLinkDeadloop pUBODirectLinkDeadloop;
	deoglCapCheckFramebufferTextureSingle pFramebufferTextureSingle;
	deoglCapCheckStd430 pStd430;
	bool pRestrictedImageBufferFormats;
	
	
	
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
	
	/** Maximum compute work group size. */
	inline int GetMaxComputeWorkGroupInvocations() const{ return pMaxComputeWorkGroupInvocations; }
	
	/** Maximum count of compute work group invocations. */
	inline const decPoint3 &GetMaxComputeWorkGroupSize() const{ return pMaxComputeWorkGroupSize; }
	
	/** Maximum texture image units. */
	inline int GetMaxTextureImageUnits() const{ return pMaxTextureImageUnits; }
	
	
	
	/** Array texture layer check. */
	inline const deoglCapCheckATLUnbind &ATLUnbind() const{ return pATLUnbind; }
	
	/** Indirect UBO matrix access check. */
	inline const deoglCapCheckUBOIndirectMatrixAccess &GetUBOIndirectMatrixAccess() const{
		return pUBOIndirectMatrixAccess; }
	
	/** Cleare all layers in an array texture. */
	inline const deoglCapCheckClearEntireArrayTexture &GetClearEntireArrayTexture() const{
		return pClearEntireArrayTexture; }
	
	/** UBO direct linking dead-loop. */
	inline const deoglCapCheckUBODirectLinkDeadloop &GetUBODirectLinkDeadloop() const{
		return pUBODirectLinkDeadloop; }
	
	/** Framebuffer texture single. */
	inline const deoglCapCheckFramebufferTextureSingle &GetFramebufferTextureSingle() const{
		return pFramebufferTextureSingle; }
	
	/** Std430 support. */
	inline const deoglCapCheckStd430 &GetStd430() const{ return pStd430; }
	
	/** Restricted image buffer formats. */
	inline bool GetRestrictedImageBufferFormats() const{ return pRestrictedImageBufferFormats; }
	
	/** Tests the hardware for its capabilities. */
	void DetectCapabilities();
	
	/** Full-Screen Quad VAO for internal testing only. */
	inline GLuint GetFSQuadVAO() const{ return pFSQuadVAO; }
	
	/** Verify capabilities. */
	bool Verify() const;
	/*@}*/
	
private:
	#ifdef OS_ANDROID
	void pAndroidTest( deoglFramebuffer *framebuffer );
	#endif
};

#endif
