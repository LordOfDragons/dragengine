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

#ifndef _DEOGLMEMORYCONSUMPTIONTEXTUREUSE_H_
#define _DEOGLMEMORYCONSUMPTIONTEXTUREUSE_H_

#include "deoglMemoryConsumptionGPUUse.h"

#include <dragengine/common/math/decMath.h>


class deoglMemoryConsumptionTexture;
#ifdef BACKEND_OPENGL
class deoglCapsTextureFormat;
#elif defined BACKEND_VULKAN
class devkFormat;
#endif


/**
 * Texture memory consumption usage.
 */
class deoglMemoryConsumptionTextureUse{
public:
	deoglMemoryConsumptionGPUUse colorCompressed;
	deoglMemoryConsumptionGPUUse colorUncompressed;
	
	deoglMemoryConsumptionGPUUse depthCompressed;
	deoglMemoryConsumptionGPUUse depthUncompressed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create memory consumption usage. */
	deoglMemoryConsumptionTextureUse(deoglMemoryConsumptionTexture &tracker);
	
	/** Clean up vbo memory consumption. */
	~deoglMemoryConsumptionTextureUse();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Total consumption. */
	unsigned long long Total() const;
	
	/** Total compressed consumption. */
	unsigned long long TotalCompressed() const;
	
	/** Total uncompressed consumption. */
	unsigned long long TotalUncompressed() const;
	
	/** Clear all consumptions. */
	void Clear();
	
	/** Set consumption. */
	void Set(unsigned long long consumption, bool depth, bool compressed);
	
#ifdef BACKEND_OPENGL
	/** Set compressed consumption. */
	void SetCompressed(unsigned long long consumption, const deoglCapsTextureFormat &format);
	
	/** Set uncompressed consumption. */
	void SetUncompressed(const deoglCapsTextureFormat &format, int width, int height,
		int depth, int mipMapLevels);
	
	/** Calculate base uncompressed consumption. */
	unsigned long long BaseConsumption(const deoglCapsTextureFormat &format,
		int width, int height, int depth) const;
	
#elif defined BACKEND_VULKAN
	/** Set compressed consumption. */
	void SetCompressed(unsigned long long consumption, const devkFormat &format);
	
	/** Set uncompressed consumption. */
	void SetUncompressed(const devkFormat &format, int width, int height, int depth, int mipMapLevels);
	
	/** Calculate base uncompressed consumption. */
	unsigned long long BaseConsumption(const devkFormat &format, int width, int height, int depth) const;
#endif
	
	/** Calculate mip mapped uncompressed consumption. */
	unsigned long long MipMappedConsumption(int levels, int width, int height,
		unsigned long long baseConsumption) const;
	/*@}*/
};

#endif
