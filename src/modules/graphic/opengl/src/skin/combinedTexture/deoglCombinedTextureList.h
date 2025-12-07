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

#ifndef _DEOGLCOMBINEDTEXTURELIST_H_
#define _DEOGLCOMBINEDTEXTURELIST_H_

#include <dragengine/common/math/decMath.h>

class deoglCombinedTexture;
class deoglRenderThread;
class deoglRImage;



/**
 * Combined Texture List.
 */
class deoglCombinedTextureList{
private:
	deoglRenderThread &pRenderThread;
	
	deoglCombinedTexture *pRoot;
	deoglCombinedTexture *pTail;
	int pCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create list. */
	deoglCombinedTextureList(deoglRenderThread &pRenderThread);
	
	/** Clean up list. */
	~deoglCombinedTextureList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Number of combined textures in the list. */
	inline int GetCount() const{ return pCount; }
	
	/** Root combined texture. */
	inline deoglCombinedTexture *GetRoot() const{ return pRoot; }
	
	/**
	 * Combined texture with parameters.
	 * \details Creates combined texture if not existing. This adds a usage to the combined
	 *          texture. To remove a usage use the RemoveUsage function on the returned
	 *          combined texture.
	 */
	deoglCombinedTexture *GetWith(const decColor &color, deoglRImage *images[4]);
	
	/**
	 * Remove combined texture.
	 * \details For use by deoglCombinedTexture only.
	 */
	void Remove(deoglCombinedTexture *combinedTexture);
	/*@}*/
};

#endif
