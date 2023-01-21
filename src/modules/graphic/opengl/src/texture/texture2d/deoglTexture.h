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
#include "../../memory/consumption/deoglMemoryConsumptionTextureUse.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deoglRenderThread;
class deoglPixelBuffer;
class deoglCapsTextureFormat;



/**
 * OpenGL 2D texture
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
	
	deoglMemoryConsumptionTextureUse pMemUse;
	decString pDebugObjectLabel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create opengl texture. */
	deoglTexture( deoglRenderThread &renderThread );
	
	/** Clean up opengl texture. */
	~deoglTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Texture handle. */
	inline GLuint GetTexture() const{ return pTexture; }
	
	
	
	/** Width in pixels. */
	inline int GetWidth() const{ return pSize.x; }
	
	/** Height in pixels. */
	inline int GetHeight() const{ return pSize.y; }
	
	/** Size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** Set size in pixels destroying opengl texture if present. */
	void SetSize( int width, int height );
	
	/** Set size in pixels destroying opengl texture if present. */
	void SetSize( const decPoint &size );
	
	
	
	/** Texture format. */
	inline const deoglCapsTextureFormat *GetFormat() const{ return pFormat; }
	
	/** Set texture format. */
	void SetFormat( const deoglCapsTextureFormat *format );
	
	/** Set texture format by number from the list of mapping texture formats to use. */
	void SetFormatMappingByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber );
	
	/** Set texture format by number from the list of fbo texture formats to use. */
	void SetFormatFBOByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber );
	
	
	
	/** Mip mapping has to be used on this texture. */
	inline bool GetMipMapped() const{ return pMipMapped; }
	
	/** Set if mip mapping has to be used on this texture. */
	void SetMipMapped( bool mipmapped );
	
	/** Mip map level count or 0 to let the hardware auto-generate them. */
	inline int GetMipMapLevelCount() const{ return pMipMapLevelCount; }
	
	/** Set mip map level count or 0 to let the hardware auto-generate them. */
	void SetMipMapLevelCount( int count );
	
	/**
	 * Real mip map level count.
	 * \details Is either mipMapLevelCount or the real texture mip map level count.
	 */
	inline int GetRealMipMapLevelCount() const{ return pRealMipMapLevelCount; }
	
	
	
	/** Create texture if not created yet. */
	void CreateTexture();
	
	/** Destroy texture if existing. */
	void DestroyTexture();
	
	/** Set base level texture pixels from a pixel buffer. */
	void SetPixels( const deoglPixelBuffer &pixelBuffer );
	
	/** Set texture level pixels from a pixel buffer. */
	void SetPixelsLevel( int level, const deoglPixelBuffer &pixelBuffer );
	
	/** Set texture level pixels from a pixel buffer layer. */
	void SetPixelsLevelLayer( int level, const deoglPixelBuffer &pixelBuffer, int layer );
	
	/** Copy pixels from first level into pixel buffer. */
	void GetPixels( deoglPixelBuffer &pixelBuffer ) const;
	
	/** Copy pixels from level into pixel buffer. */
	void GetPixelsLevel( int level, deoglPixelBuffer &pixelBuffer ) const;
	
	/** Size of a mip map level. */
	void GetLevelSize( int level, int &width, int &height ) const;
	
	/** Create mip maps if texture foramt is mip mapped. */
	void CreateMipMaps();
	
	/** Copy from another texture to this texture. */
	void CopyFrom( const deoglTexture &texture, bool withMipMaps );
	
	/** Copy area from another texture to this texture. */
	void CopyFrom( const deoglTexture &texture, bool withMipMaps,
		int width, int height, int srcX, int srcY, int destX, int destY );
	
	
	
	/** Memory consumption. */
	inline const deoglMemoryConsumptionTextureUse &GetMemoryConsumption() const{ return pMemUse; }
	
	/** Update memory usage. */
	void UpdateMemoryUsage();
	/*@}*/
	
	
	
	/** \name Helper Functions */
	/*@{*/
	/** Set texture format suitable for texture mapping. */
	void SetMapingFormat( int channels, bool useFloat, bool compressed );
	
	/** Set texture format suitable for attaching as FBO render target. */
	void SetFBOFormat( int channels, bool useFloat );
	
	/** Set texture format suitable for attaching as FBO render target. */
	void SetFBOFormatFloat32( int channels );
	
	/** Set texture format suitable for rendering to an integral texture using an FBO. */
	void SetFBOFormatIntegral( int channels, int bpp, bool useUnsigned );
	
	/** Set texture format suitable for attaching as FBO render target. */
	void SetFBOFormatSNorm( int channels, int bpp );
	
	/** Set depth texture format suitable for attaching as FBO render target. */
	void SetDepthFormat( bool packedStencil, bool useFloat );
	
	/** Set debug object label. */
	void SetDebugObjectLabel( const char *name );
	/*@}*/
	
private:
	void pUpdateDebugObjectLabel();
};

#endif
