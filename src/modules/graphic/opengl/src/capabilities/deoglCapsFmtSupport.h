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

#ifndef _DEOGLCAPFMTSUPPORT_H_
#define _DEOGLCAPFMTSUPPORT_H_

#include "deoglCapsTextureFormatList.h"
#include "../deoglBasics.h"

class deoglCapabilities;
class deoglCapsTextureFormat;



/**
 * \brief OpenGL capabilities texture format support.
 */
class deoglCapsFmtSupport{
public:
	/**
	 * \brief 2D-Texture format configuration.
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
		
		eutfRG8,
		eutfRG16,
		eutfRG8_C,
		eutfRG16F,
		eutfRG32F,
		eutfRG8I,
		eutfRG16I,
		eutfRG8UI,
		eutfRG16UI,
		
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
	deoglCapsTextureFormatList pFoundRenBufFormats;
	
	const deoglCapsTextureFormat *pUseTex2DFormats[ UseTextureFormatCount ];
	const deoglCapsTextureFormat *pUseTexCubeFormats[ UseTextureFormatCount ];
	const deoglCapsTextureFormat *pUseArrTexFormats[ UseTextureFormatCount ];
	const deoglCapsTextureFormat *pUseFBOTex2DFormats[ UseTextureFormatCount ];
	const deoglCapsTextureFormat *pUseFBOTexCubeFormats[ UseTextureFormatCount ];
	const deoglCapsTextureFormat *pUseFBOArrTexFormats[ UseTextureFormatCount ];
	const deoglCapsTextureFormat *pUseRenBufFormats[ UseTextureFormatCount ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create capabilities format support. */
	deoglCapsFmtSupport( deoglCapabilities &capabilities );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List of found texture 2d formats. */
	const deoglCapsTextureFormatList &GetFoundTex2DFormats() const{ return pFoundTex2DFormats; }
	deoglCapsTextureFormatList &GetFoundTex2DFormats(){ return pFoundTex2DFormats; }
	
	/** \brief List of found texture cube formats. */
	const deoglCapsTextureFormatList &GetFoundTexCubeFormats() const{ return pFoundTexCubeFormats; }
	deoglCapsTextureFormatList &GetFoundTexCubeFormats(){ return pFoundTexCubeFormats; }
	
	/** \brief List of found array texture formats. */
	const deoglCapsTextureFormatList &GetFoundArrayTexFormats() const{ return pFoundArrTexFormats; }
	deoglCapsTextureFormatList &GetFoundArrayTexFormats(){ return pFoundArrTexFormats; }
	
	/** \brief List of found fbo texture 2d formats. */
	const deoglCapsTextureFormatList &GetFoundFBOTex2DFormats() const{ return pFoundFBOTex2DFormats; }
	deoglCapsTextureFormatList &GetFoundFBOTex2DFormats(){ return pFoundFBOTex2DFormats; }
	
	/** \brief List of found fbo texture cube formats. */
	const deoglCapsTextureFormatList &GetFoundFBOTexCubeFormats() const{ return pFoundFBOTexCubeFormats; }
	deoglCapsTextureFormatList &GetFoundFBOTexCubeFormats(){ return pFoundFBOTexCubeFormats; }
	
	/** \brief List of found fbo array texture formats. */
	const deoglCapsTextureFormatList &GetFoundFBOArrayTexFormats() const{ return pFoundFBOArrTexFormats; }
	deoglCapsTextureFormatList &GetFoundFBOArrayTexFormats(){ return pFoundFBOArrTexFormats; }
	
	/** \brief List of found render buffer formats. */
	const deoglCapsTextureFormatList &GetFoundRenBufFormats() const{ return pFoundRenBufFormats; }
	deoglCapsTextureFormatList &GetFoundRenBufFormats(){ return pFoundRenBufFormats; }
	
	
	
	/** \brief Format to use for texture 2d type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseTex2DFormatFor( eUseTextureFormats type ) const;
	
	/** \brief Format to use for texture cube type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseTexCubeFormatFor( eUseTextureFormats type ) const;
	
	/** \brief Format to use for array texture type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseArrayTexFormatFor( eUseTextureFormats type ) const;
	
	/** \brief Format to use for fbo texture 2d type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseFBOTex2DFormatFor( eUseTextureFormats type ) const;
	
	/** \brief Format to use for fbo texture cube type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseFBOTexCubeFormatFor( eUseTextureFormats type ) const;
	
	/** \brief Format to use for fbo array texture type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseFBOArrayTexFormatFor( eUseTextureFormats type ) const;
	
	/** \brief Format to use for render buffer texture type or \em NULL if not supported. */
	const deoglCapsTextureFormat *GetUseRenBufFormatFor( eUseTextureFormats type ) const;
	
	
	
	/** \brief Detect format support. */
	void DetectFormats( GLuint fbo );
	/*@}*/
	
	
	
private:
	void pDetectTex2DFormats();
	void pDetectTexCubeFormats();
	void pDetectArrayTexFormats();
	void pDetectFBOTex2DFormats( GLuint fbo );
	void pDetectFBOTexCubeFormats( GLuint fbo );
	void pDetectFBOArrayTexFormats( GLuint fbo );
	void pDetectRenBufFormats( GLuint fbo );
	
	bool pTestTex2DFormat( GLint format, GLenum pixelFormat, GLenum pixelType,
		int bitsPerPixel, int flags, const char *name );
	bool pTestTexCubeFormat( GLint format, GLenum pixelFormat, GLenum pixelType,
		int bitsPerPixel, int flags, const char *name );
	bool pTestArrayTexFormat( GLint format, GLenum pixelFormat, GLenum pixelType,
		int bitsPerPixel, int flags, const char *name );
	bool pTestFBOTex2DFormat( GLuint fbo, GLint format, GLenum pixelFormat,
		GLenum pixelType, int bitsPerPixel, int flags, const char *name, int what );
	bool pTestFBOTexCubeFormat( GLuint fbo, GLint format, GLenum pixelFormat,
		GLenum pixelType, int bitsPerPixel, int flags, const char *name, int what );
	bool pTestFBOArrayTexFormat( GLuint fbo, GLint format, GLenum pixelFormat,
		GLenum pixelType, int bitsPerPixel, int flags, const char *name, int what );
	bool pTestRenBufFormat( GLuint fbo, GLint format, int bitsPerPixel, int flags,
		const char *name, int what );
};

#endif
