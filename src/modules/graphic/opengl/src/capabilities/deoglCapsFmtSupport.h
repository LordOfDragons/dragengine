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

#ifndef _DEOGLCAPFMTSUPPORT_H_
#define _DEOGLCAPFMTSUPPORT_H_

#include "deoglCapsTextureFormatList.h"
#include "../deoglBasics.h"

#include <dragengine/common/string/decString.h>

class deoglCapabilities;
class deoglCapsTextureFormat;



/**
 * OpenGL capabilities texture format support.
 */
class deoglCapsFmtSupport{
public:
	/**
	 * 2D-Texture format configuration.
	 * 
	 * The constants are encoded like this: eutfR{G{B{A}}}{8,16,32}{I}{UI}{_C}.
	 * R{G{B{A}}} stands for the number of components, {8,16,32} for the bit depth,
	 * {I} for integral, {UI} for unsigned integral, {_C} for compressed (not
	 * compressed if missing) and {_S} for signed normalized.
	 */
	enum eUseTextureFormats{
		eutfR8,
		eutfR16,
		eutfR8_C,
		eutfR16F,
		eutfR32F,
		eutfR8I,
		eutfR16I,
		eutfR8UI,
		eutfR16UI,
		eutfR8_S,
		eutfR16_S,
		
		eutfRG8,
		eutfRG16,
		eutfRG8_C,
		eutfRG16F,
		eutfRG32F,
		eutfRG8I,
		eutfRG16I,
		eutfRG8UI,
		eutfRG16UI,
		eutfRG8_S,
		eutfRG16_S,
		
		eutfR3G3B2,
		eutfRGB4,
		eutfRGB5,
		eutfRGB8,
		eutfRGB16,
		eutfRGB8_C,
		eutfRG11B10F,
		eutfRGB16F,
		eutfRGB32F,
		eutfRGB8I,
		eutfRGB16I,
		eutfRGB8UI,
		eutfRGB16UI,
		eutfRGB8_S,
		eutfRGB16_S,
		
		eutfRGBA2,
		eutfRGBA4,
		eutfRGB5A1,
		eutfRGBA8,
		eutfRGB10A2,
		eutfRGBA16,
		eutfRGBA8_C,
		eutfRGBA16F,
		eutfRGBA32F,
		eutfRGBA8I,
		eutfRGBA16I,
		eutfRGBA8UI,
		eutfRGBA16UI,
		eutfRGBA8_S,
		eutfRGBA16_S,
		
		eutfDepth,
		eutfDepth_Stencil,
		eutfStencil,
		eutfDepth16,
		eutfDepthF,
		eutfDepthF_Stencil
	};
	const static int UseTextureFormatCount = eutfDepthF_Stencil + 1;
	
	
	
private:
	deoglCapabilities &pCapabilities;
	
	deoglCapsTextureFormatList pFoundTex2DFormats;
	deoglCapsTextureFormatList pFoundTexCubeFormats;
	deoglCapsTextureFormatList pFoundArrTexFormats;
	deoglCapsTextureFormatList pFoundFBOTex2DFormats;
	deoglCapsTextureFormatList pFoundFBOTexCubeFormats;
	deoglCapsTextureFormatList pFoundFBOArrTexFormats;
	
	const deoglCapsTextureFormat *pUseTex2DFormats[UseTextureFormatCount];
	const deoglCapsTextureFormat *pUseTexCubeFormats[UseTextureFormatCount];
	const deoglCapsTextureFormat *pUseArrTexFormats[UseTextureFormatCount];
	const deoglCapsTextureFormat *pUseFBOTex2DFormats[UseTextureFormatCount];
	const deoglCapsTextureFormat *pUseFBOTexCubeFormats[UseTextureFormatCount];
	const deoglCapsTextureFormat *pUseFBOArrTexFormats[UseTextureFormatCount];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create capabilities format support. */
	deoglCapsFmtSupport(deoglCapabilities &capabilities);
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** List of found texture 2d formats. */
	const deoglCapsTextureFormatList &GetFoundTex2DFormats() const{ return pFoundTex2DFormats; }
	deoglCapsTextureFormatList &GetFoundTex2DFormats(){return pFoundTex2DFormats;}
	
	/** List of found texture cube formats. */
	const deoglCapsTextureFormatList &GetFoundTexCubeFormats() const{ return pFoundTexCubeFormats; }
	deoglCapsTextureFormatList &GetFoundTexCubeFormats(){return pFoundTexCubeFormats;}
	
	/** List of found array texture formats. */
	const deoglCapsTextureFormatList &GetFoundArrayTexFormats() const{ return pFoundArrTexFormats; }
	deoglCapsTextureFormatList &GetFoundArrayTexFormats(){return pFoundArrTexFormats;}
	
	/** List of found fbo texture 2d formats. */
	const deoglCapsTextureFormatList &GetFoundFBOTex2DFormats() const{ return pFoundFBOTex2DFormats; }
	deoglCapsTextureFormatList &GetFoundFBOTex2DFormats(){return pFoundFBOTex2DFormats;}
	
	/** List of found fbo texture cube formats. */
	const deoglCapsTextureFormatList &GetFoundFBOTexCubeFormats() const{ return pFoundFBOTexCubeFormats; }
	deoglCapsTextureFormatList &GetFoundFBOTexCubeFormats(){return pFoundFBOTexCubeFormats;}
	
	/** List of found fbo array texture formats. */
	const deoglCapsTextureFormatList &GetFoundFBOArrayTexFormats() const{ return pFoundFBOArrTexFormats; }
	deoglCapsTextureFormatList &GetFoundFBOArrayTexFormats(){return pFoundFBOArrTexFormats;}
	
	
	
	/** Format to use for texture 2d type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseTex2DFormatFor(eUseTextureFormats type) const;
	
	/** Format to use for texture cube type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseTexCubeFormatFor(eUseTextureFormats type) const;
	
	/** Format to use for array texture type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseArrayTexFormatFor(eUseTextureFormats type) const;
	
	/** Format to use for fbo texture 2d type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseFBOTex2DFormatFor(eUseTextureFormats type) const;
	
	/** Format to use for fbo texture cube type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseFBOTexCubeFormatFor(eUseTextureFormats type) const;
	
	/** Format to use for fbo array texture type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseFBOArrayTexFormatFor(eUseTextureFormats type) const;
	
	
	
	/** Format to use for texture 2d type or throws exception if not supported. */
	const deoglCapsTextureFormat &RequireUseTex2DFormatFor(eUseTextureFormats type) const;
	
	/** Format to use for texture cube type or throws exception if not supported. */
	const deoglCapsTextureFormat &RequireUseTexCubeFormatFor(eUseTextureFormats type) const;
	
	/** Format to use for array texture type or throws exception if not supported. */
	const deoglCapsTextureFormat &RequireUseArrayTexFormatFor(eUseTextureFormats type) const;
	
	/** Format to use for fbo texture 2d type or throws exception if not supported. */
	const deoglCapsTextureFormat &RequireUseFBOTex2DFormatFor(eUseTextureFormats type) const;
	
	/** Format to use for fbo texture cube type or throws exception if not supported. */
	const deoglCapsTextureFormat &RequireUseFBOTexCubeFormatFor(eUseTextureFormats type) const;
	
	/** Format to use for fbo array texture type or throws exception if not supported. */
	const deoglCapsTextureFormat &RequireUseFBOArrayTexFormatFor(eUseTextureFormats type) const;
	
	
	/** Get texture format name for logging. */
	const char *GetTextureFormatName(eUseTextureFormats type) const;
	
	
	/** Detect format support. */
	void DetectFormats(GLuint fbo);
	
	
	/** Supported formats. */
	decString SupportedFormatsTex2D() const;
	decString SupportedFormatsTexCube() const;
	decString SupportedFormatsArrayTex() const;
	decString SupportedFormatsFBOTex2D() const;
	decString SupportedFormatsFBOTexCube() const;
	decString SupportedFormatsFBOArrayTex() const;
	/*@}*/
	
	
	
private:
	void pDetectTex2DFormats();
	void pDetectTexCubeFormats();
	void pDetectArrayTexFormats();
	void pDetectFBOTex2DFormats(GLuint fbo);
	void pDetectFBOTexCubeFormats(GLuint fbo);
	void pDetectFBOArrayTexFormats(GLuint fbo);
	
	bool pTestTex2DFormat(GLint format, GLenum pixelFormat, GLenum pixelType,
		int bitsPerPixel, int flags, const char *name);
	bool pTestTexCubeFormat(GLint format, GLenum pixelFormat, GLenum pixelType,
		int bitsPerPixel, int flags, const char *name);
	bool pTestArrayTexFormat(GLint format, GLenum pixelFormat, GLenum pixelType,
		int bitsPerPixel, int flags, const char *name);
	bool pTestFBOTex2DFormat(GLuint fbo, GLint format, GLenum pixelFormat,
		GLenum pixelType, int bitsPerPixel, int flags, const char *name, int what);
	bool pTestFBOTexCubeFormat(GLuint fbo, GLint format, GLenum pixelFormat,
		GLenum pixelType, int bitsPerPixel, int flags, const char *name, int what);
	bool pTestFBOArrayTexFormat(GLuint fbo, GLint format, GLenum pixelFormat,
		GLenum pixelType, int bitsPerPixel, int flags, const char *name, int what);
	
	const deoglCapsTextureFormat &pRequireFormat(const char *categoryName,
		eUseTextureFormats type, const deoglCapsTextureFormat **listUse,
		const deoglCapsTextureFormatList &listSupported) const;
	
	decString pSupportedFormats(const deoglCapsTextureFormatList &listSupported) const;
};

#endif
