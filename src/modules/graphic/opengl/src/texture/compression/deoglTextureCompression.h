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
	inline deoglPixelBuffer *GetDecompressedData() const{return pDecompressedData;}
	/** Sets the pixel buffer containing the decompressed data or NULL if not set. */
	void SetDecompressedData(deoglPixelBuffer *pixelBuffer);
	/** Retrieves the pixel buffer containing the compressed data or NULL if not set. */
	inline deoglPixelBuffer *GetCompressedData() const{return pCompressedData;}
	/** Sets the pixel buffer containing the compressed data or NULL if not set. */
	void SetCompressedData(deoglPixelBuffer *pixelBuffer);
	
	/** Retrieves the pixel buffer mip map containing the decompressed data or NULL if not set. */
	inline deoglPixelBufferMipMap *GetDecompressedDataMipMap() const{return pDecompressedDataMipMap;}
	/** Sets the pixel buffer mip map containing the decompressed data or NULL if not set. */
	void SetDecompressedDataMipMap(deoglPixelBufferMipMap *pixelBufferMipMap);
	/** Retrieves the pixel buffer mip map containing the compressed data or NULL if not set. */
	inline deoglPixelBufferMipMap *GetCompressedDataMipMap() const{return pCompressedDataMipMap;}
	/** Sets the pixel buffer mip map containing the compressed data or NULL if not set. */
	void SetCompressedDataMipMap(deoglPixelBufferMipMap *pixelBufferMipMap);
	
	/** Determines if fast compression with lower quality is used. */
	inline bool GetFastCompression() const{return pFastCompression;}
	/** Sets if fast compression with lower quality is used. */
	void SetFastCompression(bool fastCompression);
	
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
	void pCompressSquish(const deoglPixelBuffer &pixelBufferFrom, deoglPixelBuffer &pixelBufferTo, int flags);
};

#endif
