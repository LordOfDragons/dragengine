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

#ifndef _DEOGLTEXTURECOMPRESSION_H_
#define _DEOGLTEXTURECOMPRESSION_H_

class deoglPixelBuffer;
class deoglPixelBufferMipMap;



/**
 * Texture Compression.
 * Provides support to compress and decompress texture data in pixel buffers.
 */
class deoglTextureCompression{
public:
	deoglPixelBuffer *pDecompressedData;
	deoglPixelBuffer *pCompressedData;
	
	deoglPixelBufferMipMap *pDecompressedDataMipMap;
	deoglPixelBufferMipMap *pCompressedDataMipMap;
	
	bool pFastCompression;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new texture compression object. */
	deoglTextureCompression();
	/** Cleans up the texture compression object. */
	~deoglTextureCompression();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the pixel buffer containing the decompressed data or NULL if not set. */
	inline deoglPixelBuffer *GetDecompressedData() const{ return pDecompressedData; }
	/** Sets the pixel buffer containing the decompressed data or NULL if not set. */
	void SetDecompressedData( deoglPixelBuffer *pixelBuffer );
	/** Retrieves the pixel buffer containing the compressed data or NULL if not set. */
	inline deoglPixelBuffer *GetCompressedData() const{ return pCompressedData; }
	/** Sets the pixel buffer containing the compressed data or NULL if not set. */
	void SetCompressedData( deoglPixelBuffer *pixelBuffer );
	
	/** Retrieves the pixel buffer mip map containing the decompressed data or NULL if not set. */
	inline deoglPixelBufferMipMap *GetDecompressedDataMipMap() const{ return pDecompressedDataMipMap; }
	/** Sets the pixel buffer mip map containing the decompressed data or NULL if not set. */
	void SetDecompressedDataMipMap( deoglPixelBufferMipMap *pixelBufferMipMap );
	/** Retrieves the pixel buffer mip map containing the compressed data or NULL if not set. */
	inline deoglPixelBufferMipMap *GetCompressedDataMipMap() const{ return pCompressedDataMipMap; }
	/** Sets the pixel buffer mip map containing the compressed data or NULL if not set. */
	void SetCompressedDataMipMap( deoglPixelBufferMipMap *pixelBufferMipMap );
	
	/** Determines if fast compression with lower quality is used. */
	inline bool GetFastCompression() const{ return pFastCompression; }
	/** Sets if fast compression with lower quality is used. */
	void SetFastCompression( bool fastCompression );
	
	/** Compress texture from the decompressed pixel buffer into the compressed pixel buffer. */
	void Compress();
	/** Compress texture from the decompressed pixel buffer into the compressed pixel buffer using DXT1 format. */
	void CompressDXT1();
	/** Compress texture from the decompressed pixel buffer into the compressed pixel buffer using DXT3 format. */
	void CompressDXT3();
	
	/** Compress textures from the decompressed pixel buffer mip map into the compressed pixel buffer mip map. */
	void CompressMipMap();
	/** Compress textures from the decompressed pixel buffer mip map into the compressed pixel buffer mip map using DXT1 format. */
	void CompressMipMapDXT1();
	/** Compress textures from the decompressed pixel buffer mip map into the compressed pixel buffer mip map DXT3 format. */
	void CompressMipMapDXT3();
	/*@}*/
	
private:
	int pGetQualitySquishFlags();
	void pCompressSquish( const deoglPixelBuffer &pixelBufferFrom, deoglPixelBuffer &pixelBufferTo, int flags );
};

#endif
