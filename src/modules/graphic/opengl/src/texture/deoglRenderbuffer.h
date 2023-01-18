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

#ifndef _DEOGLRENDERBUFFER_H_
#define _DEOGLRENDERBUFFER_H_

#include "../deoglBasics.h"
#include "../memory/consumption/deoglMemoryConsumptionRenderBufferUse.h"

class deoglRenderThread;
class deoglCapsTextureFormat;



/**
 * @brief OpenGL Renderbuffer
 * Manages an opengl renderbuffer
 */
class deoglRenderbuffer{
private:
	deoglRenderThread &pRenderThread;
	
	GLuint pRenderbuffer;
	GLint pFormat;
	int pWidth;
	int pHeight;
	int pBitsPerPixel;
	bool pIsDepth;
	
	deoglMemoryConsumptionRenderBufferUse pMemUse;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new opengl renderbuffer. */
	deoglRenderbuffer( deoglRenderThread &renderThread );
	/** Cleans up the opengl renderbuffer. */
	~deoglRenderbuffer();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the renderbuffer. */
	inline GLuint GetRenderbuffer() const{ return pRenderbuffer; }
	/** Retrieves the width of the texture. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height of the texture. */
	inline int GetHeight() const{ return pHeight; }
	/** Sets the size of the texture destroying the old texture if present. */
	void SetSize( int width, int height );
	/** Retrieves the texture format. */
	inline GLint GetFormat() const{ return pFormat; }
	/** Sets the texture format and destroys the old texture if present. */
	void SetFormat( GLint format );
	
	/** Creates the renderbuffer if not created yet. */
	void CreateBuffer();
	/** Destroys the renderbuffer if existing. */
	void DestroyBuffer();
	
	/** Activates the renderbuffer. */
	void Activate();
	/** Deactivates the current render buffer. */
	void Deactivate();
	
	/** Memory consumption. */
	inline const deoglMemoryConsumptionRenderBufferUse &GetMemoryConsumption() const{ return pMemUse; }
	
	/** Update memory usage. */
	void UpdateMemoryUsage();
	/*@}*/
	
	/** \name Helper Functions */
	/*@{*/
	/** Sets the format from a capabilities texture format. */
	void SetFormatFromCaps( const deoglCapsTextureFormat &format );
	/**
	 * Sets the texture format suitable for texture mapping according to the
	 * provided texture description. The currently set compressed value is
	 * taken into account while chosing a format.
	 */
	void SetColorFormat( int bpp, int channels, bool useFloat );
	/**
	 * Sets the depth texture format suitable for texture mapping according
	 * to the provided texture description.
	 */
	void SetDepthFormat( bool packedStencil );
	/** Sets the stencil format. */
	void SetStencilFormat();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
