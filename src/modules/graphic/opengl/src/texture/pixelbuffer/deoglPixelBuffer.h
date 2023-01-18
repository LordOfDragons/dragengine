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

#ifndef _DEOGLPIXELBUFFER_H_
#define _DEOGLPIXELBUFFER_H_

#include "../../deoglBasics.h"



/**
 * Pixel Buffer.
 * Stores image data. Used to transfer pixels from or to an OpenGL texture.
 */
class deoglPixelBuffer{
public:
	/** Pixel formats. */
	enum ePixelFormats{
		/** 1 component byte format (sByte1). */
		epfByte1,
		/** 2 component byte format (sByte2). */
		epfByte2,
		/** 3 component byte format (sByte3). */
		epfByte3,
		/** 4 component byte format (sByte4). */
		epfByte4,
		/** 1 component float format (sFloat1). */
		epfFloat1,
		/** 2 component float format (sFloat2). */
		epfFloat2,
		/** 3 component float format (sFloat3). */
		epfFloat3,
		/** 4 component float format (sFloat4). */
		epfFloat4,
		/** Depth format (sDepth). */
		epfDepth,
		/** Stencil format (sStencil). */
		epfStencil,
		/** Depth combined with stencil format (sDepthStencil). */
		epfDepthStencil,
		/** DXT1 compression format (sDXT1). */
		epfDXT1,
		/** DXT3 compression format (sDXT3). */
		epfDXT3,
		/** Dummy entry for the number of pixel formats. */
		EPF_COUNT
	};
	
	/** 1 component byte format. */
	struct sByte1{
		GLubyte r;
	};
	
	/** 2 component byte format. */
	struct sByte2{
		GLubyte r;
		GLubyte g;
	};
	
	/** 3 component byte format. */
	struct sByte3{
		GLubyte r;
		GLubyte g;
		GLubyte b;
	};
	
	/** 4 component byte format. */
	struct sByte4{
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};
	
	/** 1 component float format. */
	struct sFloat1{
		GLfloat r;
	};
	
	/** 2 component float format. */
	struct sFloat2{
		GLfloat r;
		GLfloat g;
	};
	
	/** 3 component float format. */
	struct sFloat3{
		GLfloat r;
		GLfloat g;
		GLfloat b;
	};
	
	/** 4 component float format. */
	struct sFloat4{
		GLfloat r;
		GLfloat g;
		GLfloat b;
		GLfloat a;
	};
	
	/** Depth format. */
	struct sDepth{
		GLfloat depth;
	};
	
	/** Stencil format. */
	struct sStencil{
		GLubyte stencil;
	};
	
	/**
	 * Depth combined with stencil format. The first 24 bits (high bits) are the depth
	 * encoded to int. The last 8 bits (low bits) are the stencil.
	 */
	struct sDepthStencil{
		GLuint depthStencil;
	};
	
	/**
	 * DXT1 compression format. Block of 4x4 compressed pixels. The colors color1 and color2
	 * are stored as 5-6-5 RGB values. The codes contains the codes for all 16 pixels from
	 * top left to bottom right each code consuming 2 bits with the first pixel starting
	 * at bits 30-31 with all others following down to the lowest bit.
	 */
	struct sDXT1{
		GLushort color1;
		GLushort color2;
		GLuint codes;
	};
	
	/**
	 * DXT3 compression format. Block of 4x4 compressed pixels. The colors color1 and color2
	 * are stored as 5-6-5 RGB values. The codes contains the codes for all 16 pixels from
	 * top left to bottom right each code consuming 2 bits with the first pixel starting
	 * at bits 30-31 with all others following down to the lowest bit. The alpha1 and alpha2
	 * values contain the alpha values for all pixels in the same order consuming 4 bits each.
	 * Here too the first alpha value is located at alpha0 bits 28-31 with all others following
	 * towards the lowest bit. This covers the first 8 pixels. Alpha2 works similar for the
	 * next 8 pixels.
	 */
	struct sDXT3{
		GLuint alpha1;
		GLuint alpha2;
		GLushort color1;
		GLushort color2;
		GLuint codes;
	};
	
private:
	int pWidth;
	int pHeight;
	int pDepth;
	ePixelFormats pFormat;
	int pUnitSize;
	int pStrideLine;
	int pStrideLayer;
	int pImageSize;
	bool pCompressed;
	GLenum pGLPixelFormat;
	GLenum pGLPixelType;
	void *pPixels;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new pixel buffer. */
	deoglPixelBuffer( ePixelFormats format, int width, int height, int depth );
	
	/** Create copy of pixel buffer. */
	deoglPixelBuffer( const deoglPixelBuffer &pixelBuffer );
	
	/** Cleans up the opengl array texture. */
	~deoglPixelBuffer();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the width. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height. */
	inline int GetHeight() const{ return pHeight; }
	/** Retrieves the depth. */
	inline int GetDepth() const{ return pDepth; }
	/** Retrieves the pixel format. */
	inline ePixelFormats GetFormat() const{ return pFormat; }
	
	/** Retrieves the unit size in bytes which is the size of the matching pixel data struct. */
	inline int GetUnitSize() const{ return pUnitSize; }
	/** Retrieves the stride of a unit line in bytes which can be a pixel line or a block line. */
	inline int GetLineStride() const{ return pStrideLine; }
	/** Retrieves the stride of a depth layer in bytes. */
	inline int GetLayerStride() const{ return pStrideLayer; }
	/** Retrieves the size of the image data in bytes. */
	inline int GetImageSize() const{ return pImageSize; }
	/** Determines if the pixel buffer contains compressed data. */
	inline bool GetCompressed() const{ return pCompressed; }
	
	/** Retrieves the bare data pointer. */
	inline void *GetPointer() const{ return pPixels; }
	/** Retrieves the sByte1 data pointer if the format is epfByte1 or otherwise throws an exception. */
	sByte1 *GetPointerByte1() const;
	/** Retrieves the sByte2 data pointer if the format is epfByte2 or otherwise throws an exception. */
	sByte2 *GetPointerByte2() const;
	/** Retrieves the sByte3 data pointer if the format is epfByte3 or otherwise throws an exception. */
	sByte3 *GetPointerByte3() const;
	/** Retrieves the sByte4 data pointer if the format is epfByte4 or otherwise throws an exception. */
	sByte4 *GetPointerByte4() const;
	/** Retrieves the sFloat1 data pointer if the format is epfFloat1 or otherwise throws an exception. */
	sFloat1 *GetPointerFloat1() const;
	/** Retrieves the sFloat2 data pointer if the format is epfFloat2 or otherwise throws an exception. */
	sFloat2 *GetPointerFloat2() const;
	/** Retrieves the sFloat3 data pointer if the format is epfFloat3 or otherwise throws an exception. */
	sFloat3 *GetPointerFloat3() const;
	/** Retrieves the sFloat4 data pointer if the format is epfFloat4 or otherwise throws an exception. */
	sFloat4 *GetPointerFloat4() const;
	/** Retrieves the sDepth data pointer if the format is epfDepth or otherwise throws an exception. */
	sDepth *GetPointerDepth() const;
	/** Retrieves the sStencil data pointer if the format is epfStencil or otherwise throws an exception. */
	sStencil *GetPointerStencil() const;
	/** Retrieves the sDepthStencil data pointer if the format is epfDepthStencil or otherwise throws an exception. */
	sDepthStencil *GetPointerDepthStencil() const;
	/** Retrieves the sDXT1 data pointer if the format is epfDXT1 or otherwise throws an exception. */
	sDXT1 *GetPointerDXT1() const;
	/** Retrieves the sDXT3 data pointer if the format is epfDXT3 or otherwise throws an exception. */
	sDXT3 *GetPointerDXT3() const;
	
	/** Sets the pixel data to a uniform color. */
	void SetToIntColor( int red, int green, int blue, int alpha );
	/** Sets the pixel data to a uniform color. */
	void SetToFloatColor( float red, float green, float blue, float alpha );
	
	/** Retrieves the opengl pixel format required to exchange data with an opengl texture. */
	inline GLenum GetGLPixelFormat() const{ return pGLPixelFormat; }
	/** Retrieves the opengl pixel type required to exchange data with an opengl texture. */
	inline GLenum GetGLPixelType() const{ return pGLPixelType; }
	/*@}*/
};

#endif
