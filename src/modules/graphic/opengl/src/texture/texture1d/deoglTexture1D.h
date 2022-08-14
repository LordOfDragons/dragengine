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

#ifndef _DEOGLTEXTURE1D_H_
#define _DEOGLTEXTURE1D_H_

#include "../../deoglBasics.h"
#include "../../capabilities/deoglCapsFmtSupport.h"
#include "../../memory/consumption/deoglMemoryConsumptionTextureUse.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deoglRenderThread;
class deoglPixelBuffer;
class deoglCapsTextureFormat;



/**
 * \brief OpenGL 1D Texture.
 * \details Manages an OpenGL 1D texture. The texture is resizable and can be created once required. By default
 *          the texture is not created. After changing the size the texture is destroyed too. The property flags
 *          like mipmapped are informative flags and do not influence the texture creation. They are stored for
 *          external code to know what sort of texture they deal with. Pixel data can be loaded into the texture
 *          using pixel buffer objects. Any kind of pixel buffer can be used.
 */
class deoglTexture1D{
public:
	deoglRenderThread &pRenderThread;
	
	GLuint pTexture;
	
	int pSize;
	const deoglCapsTextureFormat *pFormat;
	int pMipMapLevelCount;
	int pRealMipMapLevelCount;
	bool pMipMapped;
	
	deoglMemoryConsumptionTextureUse pMemUse;
	decString pDebugObjectLabel;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new opengl texture. */
	deoglTexture1D( deoglRenderThread &renderThread );
	/** \brief Cleans up the opengl texture. */
	~deoglTexture1D();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the texture handle. */
	inline GLuint GetTexture() const{ return pTexture; }
	
	/** \brief Retrieves the size of the texture. */
	inline int GetSize() const{ return pSize; }
	/** \brief Sets the size of the texture destroying the old texture if present. */
	void SetSize( int size );
	/** \brief Retrieves the texture format. */
	inline const deoglCapsTextureFormat *GetFormat() const{ return pFormat; }
	/** \brief Sets the texture format. */
	void SetFormat( const deoglCapsTextureFormat *format );
	/** \brief Sets the texture format by number from the list of mapping texture formats to use. */
	void SetFormatMappingByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber );
	/** \brief Sets the texture format by number from the list of fbo texture formats to use. */
	void SetFormatFBOByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber );
	/** \brief Determines if mip mapping has to be used on this texture. */
	inline bool GetMipMapped() const{ return pMipMapped; }
	/** \brief Sets if mip mapping has to be used on this texture. */
	void SetMipMapped( bool mipmapped );
	/** \brief Retrieves the mip map level count or 0 to let the hardware auto-generate them. */
	inline int GetMipMapLevelCount() const{ return pMipMapLevelCount; }
	/** \brief Sets the mip map level count or 0 to let the hardware auto-generate them. */
	void SetMipMapLevelCount( int count );
	/** \brief Retrieves the real mip map level count which is either mipMapLevelCount or the real texture mip map level count. */
	inline int GetRealMipMapLevelCount() const{ return pRealMipMapLevelCount; }
	
	/** \brief Creates the texture if not created yet. */
	void CreateTexture();
	/** \brief Destroys the texture if existing. */
	void DestroyTexture();
	/** \brief Sets the base level texture pixels from a pixel buffer. */
	void SetPixels( const deoglPixelBuffer &pixelBuffer );
	/** \brief Sets texture level pixels from a pixel buffer. */
	void SetPixelsLevel( int level, const deoglPixelBuffer &pixelBuffer );
	
	/** \brief Copy pixels from first level into pixel buffer. */
	void GetPixels( deoglPixelBuffer &pixelBuffer ) const;
	
	/** \brief Copy pixels from level into pixel buffer. */
	void GetPixelsLevel( int level, deoglPixelBuffer &pixelBuffer ) const;
	
	/** \brief Retrieves the size of a mip map level. */
	int GetLevelSize( int level ) const;
	/** \brief Force mip map creation if the texture is mip mapped. */
	void CreateMipMaps();
	
	/** \brief Copy from another texture to this texture. */
	void CopyFrom( const deoglTexture1D &texture, bool withMipMaps );
	/** \brief Copy area from another texture to this texture. */
	void CopyFrom( const deoglTexture1D &texture, bool withMipMaps, int size, int srcX, int destX );
	
	/** Memory consumption. */
	inline const deoglMemoryConsumptionTextureUse &GetMemoryConsumption() const{ return pMemUse; }
	
	/** \brief Update memory usage. */
	void UpdateMemoryUsage();
	/*@}*/
	
	/** \name Helper Functions */
	/*@{*/
	/** \brief Sets the texture format suitable for texture mapping according to the provided texture description. */
	void SetMapingFormat( int channels, bool useFloat, bool compressed );
	/** \brief Sets the texture format suitable for attaching as FBO render target. */
	void SetFBOFormat( int channels, bool useFloat );
	/** \brief Sets the texture format suitable for rendering to an integral texture using an FBO. */
	void SetFBOFormatIntegral( int channels, int bpp, bool useUnsigned );
	/** \brief Sets the depth texture format suitable for attaching as FBO render target. */
	void SetDepthFormat( bool packedStencil );
	
	/** Set debug object label. */
	void SetDebugObjectLabel( const char *name );
	/*@}*/
	
private:
	void pUpdateDebugObjectLabel();
};

#endif
