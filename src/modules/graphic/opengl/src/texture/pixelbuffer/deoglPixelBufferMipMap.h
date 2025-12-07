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

#ifndef _DEOGLPIXELBUFFERMIPMAP_H_
#define _DEOGLPIXELBUFFERMIPMAP_H_

#include "deoglPixelBuffer.h"



/**
 * Pixel Buffer Mip Map.
 * Mip map chain of pixel buffers.
 */
class deoglPixelBufferMipMap : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglPixelBufferMipMap> Ref;
	
	
private:
	deoglPixelBuffer::Ref *pPixelBuffers;
	int pPixelBufferCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new pixel buffer mip map. */
	deoglPixelBufferMipMap(deoglPixelBuffer::ePixelFormats format, int width, int height, int depth, int maxLevel);
	
protected:
	/** Cleans up the opengl array texture. */
	virtual ~deoglPixelBufferMipMap();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the number of pixel buffers which is the mip map level count. */
	inline int GetPixelBufferCount() const{ return pPixelBufferCount; }
	/** Retrieves the pixel buffer for a mip map level. */
	const deoglPixelBuffer::Ref &GetPixelBuffer(int level) const;
	
	/** Reduce maximum mip map level count. */
	void ReducePixelBufferCount(int reduceByCount);
	
	/** Create mip maps from the base level using simple box filtering. */
	void CreateMipMaps();
	
	/**
	 * Create mip maps from the base level using simple box filtering.
	 * Only components with their mask set to true are processed.
	 */
	void CreateMipMaps(bool maskRed, bool maskGreen, bool maskBlue, bool maskAlpha);
	
	/** Create mip maps from the base level using maximum filtering. */
	void CreateMipMapsMax();
	
	/**
	 * Create mip maps from the base level using maximum filtering.
	 * Only components with their mask set to true are processed.
	 */
	void CreateMipMapsMax(bool maskRed, bool maskGreen, bool maskBlue, bool maskAlpha);
	
	/** Create mip maps from the base level using minimum filtering. */
	void CreateMipMapsMin();
	
	/**
	 * Create mip maps from the base level using minimum filtering.
	 * Only components with their mask set to true are processed.
	 */
	void CreateMipMapsMin(bool maskRed, bool maskGreen, bool maskBlue, bool maskAlpha);
	
	/**
	 * Create normal mip maps from the base level using simple box filtering. In addition
	 * the variance while downsampling the normals is stored in the alpha channel of the
	 * respective mip map levels.
	 */
	void CreateNormalMipMaps();
	/**
	 * Create roughness mip maps from the base level using box filtering on the roughness
	 * values using the normal values to adjust the roughness at distance. Processes only
	 * the alpha value (for the time being).
	 */
	void CreateRoughnessMipMaps(deoglPixelBufferMipMap &normalPixeBufferMipMap);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pGetTypeParams(int pixelBufferType, int &componentCount, bool &floatData) const;
};

#endif
