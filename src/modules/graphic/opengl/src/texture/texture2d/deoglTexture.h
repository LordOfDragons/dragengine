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

#ifndef _DEOGLTEXTURE_H_
#define _DEOGLTEXTURE_H_

#include "../../deoglBasics.h"
#include "../../capabilities/deoglCapsFmtSupport.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglPixelBuffer;
class deoglCapsTextureFormat;



/**
 * \brief OpenGL 2D texture
 * 
 * Manages an OpenGL texture. The texture is resizable and can be created
 * once required. By default the texture is not created. After changing the
 * size the texture is destroyed too. The property flags like mipmapped are
 * informative flags and do not influence the texture creation. They are
 * stored for external code to know what sort of texture they deal with.
 * Pixel data can be loaded into the texture using pixel buffer objects. Any
 * kind of pixel buffer can be used.
 */
class deoglTexture{
public:
	deoglRenderThread &pRenderThread;
	
	GLuint pTexture;
	
	decPoint pSize;
	const deoglCapsTextureFormat *pFormat;
	int pMipMapLevelCount;
	int pRealMipMapLevelCount;
	bool pMipMapped;
	
	int pMemoryUsageGPU;
	bool pMemoryUsageCompressed;
	bool pMemoryUsageColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create opengl texture. */
	deoglTexture( deoglRenderThread &renderThread );
	
	/** \brief Clean up opengl texture. */
	~deoglTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Texture handle. */
	inline GLuint GetTexture() const{ return pTexture; }
	
	
	
	/** \brief Width in pixels. */
	inline int GetWidth() const{ return pSize.x; }
	
	/** \brief Height in pixels. */
	inline int GetHeight() const{ return pSize.y; }
	
	/** \brief Size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** \brief Set size in pixels destroying opengl texture if present. */
	void SetSize( int width, int height );
	
	/** \brief Set size in pixels destroying opengl texture if present. */
	void SetSize( const decPoint &size );
	
	
	
	/** \brief Texture format. */
	inline const deoglCapsTextureFormat *GetFormat() const{ return pFormat; }
	
	/** \brief Set texture format. */
	void SetFormat( const deoglCapsTextureFormat *format );
	
	/** \brief Set texture format by number from the list of mapping texture formats to use. */
	void SetFormatMappingByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber );
	
	/** \brief Set texture format by number from the list of fbo texture formats to use. */
	void SetFormatFBOByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber );
	
	
	
	/** \brief Mip mapping has to be used on this texture. */
	inline bool GetMipMapped() const{ return pMipMapped; }
	
	/** \brief Set if mip mapping has to be used on this texture. */
	void SetMipMapped( bool mipmapped );
	
	/** \brief Mip map level count or 0 to let the hardware auto-generate them. */
	inline int GetMipMapLevelCount() const{ return pMipMapLevelCount; }
	
	/** \brief Set mip map level count or 0 to let the hardware auto-generate them. */
	void SetMipMapLevelCount( int count );
	
	/**
	 * \brief Real mip map level count.
	 * \details Is either mipMapLevelCount or the real texture mip map level count.
	 */
	inline int GetRealMipMapLevelCount() const{ return pRealMipMapLevelCount; }
	
	
	
	/** \brief Create texture if not created yet. */
	void CreateTexture();
	
	/** \brief Destroy texture if existing. */
	void DestroyTexture();
	
	/** \brief Set base level texture pixels from a pixel buffer. */
	void SetPixels( const deoglPixelBuffer &pixelBuffer );
	
	/** \brief Set texture level pixels from a pixel buffer. */
	void SetPixelsLevel( int level, const deoglPixelBuffer &pixelBuffer );
	
	/** \brief Set texture level pixels from a pixel buffer layer. */
	void SetPixelsLevelLayer( int level, const deoglPixelBuffer &pixelBuffer, int layer );
	
	/** \brief Copy pixels from first level into pixel buffer. */
	void GetPixels( deoglPixelBuffer &pixelBuffer ) const;
	
	/** \brief Copy pixels from level into pixel buffer. */
	void GetPixelsLevel( int level, deoglPixelBuffer &pixelBuffer ) const;
	
	/** \brief Size of a mip map level. */
	void GetLevelSize( int level, int &width, int &height ) const;
	
	/** \brief Create mip maps if texture foramt is mip mapped. */
	void CreateMipMaps();
	
	/** \brief Copy from another texture to this texture. */
	void CopyFrom( const deoglTexture &texture, bool withMipMaps );
	
	/** \brief Copy area from another texture to this texture. */
	void CopyFrom( const deoglTexture &texture, bool withMipMaps,
		int width, int height, int srcX, int srcY, int destX, int destY );
	
	
	
	/** \brief GPU memory usage. */
	inline int GetMemoryUsageGPU() const{ return pMemoryUsageGPU; }
	
	/** \brief GPU memory usage is compressed image data. */
	inline bool GetMemoryUsageCompressed() const{ return pMemoryUsageCompressed; }
	
	/** \brief Update memory usage. */
	void UpdateMemoryUsage();
	/*@}*/
	
	
	
	/** \name Helper Functions */
	/*@{*/
	/** \brief Set texture format suitable for texture mapping. */
	void SetMapingFormat( int channels, bool useFloat, bool compressed );
	
	/** \brief Set texture format suitable for attaching as FBO render target. */
	void SetFBOFormat( int channels, bool useFloat );
	
	/** \brief Set texture format suitable for attaching as FBO render target. */
	void SetFBOFormatFloat32( int channels );
	
	/** \brief Set texture format suitable for rendering to an integral texture using an FBO. */
	void SetFBOFormatIntegral( int channels, int bpp, bool useUnsigned );
	
	/** \brief Set texture format suitable for attaching as FBO render target. */
	void SetFBOFormatSNorm( int channels, int bpp );
	
	/** \brief Set depth texture format suitable for attaching as FBO render target. */
	void SetDepthFormat( bool packedStencil, bool useFloat );
	/*@}*/
};

#endif
