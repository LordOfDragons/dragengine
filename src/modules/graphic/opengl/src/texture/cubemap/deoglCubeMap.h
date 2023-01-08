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

#ifndef _DEOGLCUBEMAP_H_
#define _DEOGLCUBEMAP_H_

#include "../../deoglBasics.h"
#include "../../capabilities/deoglCapsFmtSupport.h"
#include "../../memory/consumption/deoglMemoryConsumptionTextureUse.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deoglPixelBuffer;
class deoglRenderThread;
class deoglCapsTextureFormat;



/**
 * @brief Cube Map.
 */
class deoglCubeMap{
public:
	/** Cube map faces. */
	enum eFaces{
		efPositiveX,
		efNegativeX,
		efPositiveY,
		efNegativeY,
		efPositiveZ,
		efNegativeZ
	};
	
public:
	deoglRenderThread &pRenderThread;
	
	GLuint pTexture;
	
	int pSize;
	const deoglCapsTextureFormat *pFormat;
	bool pMipMapped;
	int pMipMapLevelCount;
	int pRealMipMapLevelCount;
	
	deoglMemoryConsumptionTextureUse pMemUse;
	decString pDebugObjectLabel;
	
	
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new cube map. */
	deoglCubeMap( deoglRenderThread &renderThread );
	/** Cleans up the cube map. */
	~deoglCubeMap();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the texture handle. */
	inline GLuint GetTexture() const{ return pTexture; }
	/** Retrieves the size of the cube map. */
	inline int GetSize() const{ return pSize; }
	/** Sets the size of the cube map destroying the old cube map if present. */
	void SetSize( int size );
	/** Retrieves the texture format. */
	inline const deoglCapsTextureFormat *GetFormat() const{ return pFormat; }
	/** Sets the texture format. */
	void SetFormat( const deoglCapsTextureFormat *format );
	/** Sets the texture format by number from the list of mapping texture formats to use. */
	void SetFormatMappingByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber );
	/** Sets the texture format by number from the list of fbo texture formats to use. */
	void SetFormatFBOByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber );
	/** Determines if mip mapping has to be used on this cube map. */
	inline bool GetMipMapped() const{ return pMipMapped; }
	/** Sets if mip mapping has to be used on this cube map. */
	void SetMipMapped( bool mipmapped );
	/** Retrieves the mip map level count or 0 to let the hardware auto-generate them. */
	inline int GetMipMapLevelCount() const{ return pMipMapLevelCount; }
	/** Sets the mip map level count or 0 to let the hardware auto-generate them. */
	void SetMipMapLevelCount( int count );
	/** Retrieves the real mip map level count which is either mipMapLevelCount or the real texture mip map level count. */
	inline int GetRealMipMapLevelCount() const{ return pRealMipMapLevelCount; }
	
	/** Creates the cube map if not created yet. */
	void CreateCubeMap();
	/** Destroys the cube map if existing. */
	void DestroyCubeMap();
	/** Copies pixel data from the given pixel buffer into the cube map. */
	void SetPixels( const deoglPixelBuffer &pixels );
	/** Sets texture level pixels from a pixel buffer without touching other mip map levels. */
	void SetPixelsLevel( int level, const deoglPixelBuffer &pixels );
	
	/** \brief Copy pixels from first level into pixel buffer. */
	void GetPixels( deoglPixelBuffer &pixelBuffer ) const;
	
	/** \brief Copy pixels from level into pixel buffer. */
	void GetPixelsLevel( int level, deoglPixelBuffer &pixelBuffer ) const;
	
	/** Retrieves the size of a mip map level. */
	int GetLevelSize( int level ) const;
	
	/** Force mip map creation if the cube map is mip mapped. */
	void CreateMipMaps();
	
	/** Copy from another cubemap to this texture. */
	void CopyFrom( const deoglCubeMap &cubemap, bool withMipMaps );
	/** Copy area from cubemap texture to this texture. */
	void CopyFrom( const deoglCubeMap &cubemap, bool withMipMaps, int size, int srcX, int srcY, int destX, int destY );
	
	/** Memory consumption. */
	inline const deoglMemoryConsumptionTextureUse &GetMemoryConsumption() const{ return pMemUse; }
	
	/** Update memory usage. */
	void UpdateMemoryUsage();
	/*@}*/
	
	/** @name Helper Functions */
	/*@{*/
	/** Sets the texture format suitable for texture mapping according to the provided texture description. */
	void SetMapingFormat( int channels, bool useFloat, bool compressed );
	/** Sets the texture format suitable for attaching as FBO render target. */
	void SetFBOFormat( int channels, bool useFloat );
	
	/** Sets the suitable depth texture format. */
	void SetDepthFormat( bool useFloat );
	
	/** Creates a camera matrix for a cube map face. */
	static void CreateMatrixForFace( decMatrix &matrix, const decVector &position, int face );
	static void CreateMatrixForFace( decDMatrix &matrix, const decDVector &position, int face );
	
	/** Set debug object label. */
	void SetDebugObjectLabel( const char *name );
	/*@}*/
	
private:
	void pUpdateDebugObjectLabel();
};

#endif
