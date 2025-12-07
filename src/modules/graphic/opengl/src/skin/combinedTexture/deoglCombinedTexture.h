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

#ifndef _DEOGLCOMBINEDTEXTURE_H_
#define _DEOGLCOMBINEDTEXTURE_H_

#include "../../texture/deoglRImage.h"

#include <dragengine/common/math/decMath.h>

class deoglPixelBuffer;
class deoglRenderThread;
class deoglTexture;



/**
 * Combined texture.
 * 
 * Stores a texture composed of 2 or more static colors or images.
 */
class deoglCombinedTexture{
private:
	deoglRenderThread &pRenderThread;
	
	decColor pColor;
	deoglRImage::Ref pImages[4];
	
	deoglTexture *pTexture;
	
	int pUsageCount;
	unsigned int pHashCode;
	deoglCombinedTexture *pLLPrev;
	deoglCombinedTexture *pLLNext;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create combined texture. */
	deoglCombinedTexture(deoglRenderThread &renderThread, const decColor &color, deoglRImage *images[4]);
	
	/** Clean up combined texture. */
	~deoglCombinedTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	
	
	/** Static color. */
	inline const decColor &GetColor() const{return pColor;}
	
	/** Image for component or nullptr to use the static color component. */
	const deoglRImage::Ref &GetImageAt(int component) const;
	
	
	
	/** Texture or nullptr if not existing. */
	inline deoglTexture *GetTexture() const{return pTexture;}
	
	/** Set texture or nullptr if not existing. */
	void SetTexture(deoglTexture *texture);
	
	
	
	/** Combined texture matches another combined texture. */
	bool Equals(const deoglCombinedTexture &combinedTexture) const;
	
	/** Combined texture matches another combined texture. */
	bool Equals(const decColor &color, deoglRImage *images[4]) const;
	
	
	
	/** Usage count. */
	inline int GetUsageCount() const{return pUsageCount;}
	
	/** Increment usage count by one. */
	void AddUsage();
	
	/**
	 * Decrement usage count by one.
	 * \details If usage count reaches 0 the combined texture is removed from the
	 *          combined texture manager and deleted.
	 */
	void RemoveUsage();
	
	
	
	/** Hash code. */
	inline unsigned int GetHashCode() const{return pHashCode;}
	
	/** Calculate hash code. */
	void CalcHashCode();
	
	/** Calculate hash code for combination. */
	static unsigned int CalcHashCodeFor(const decColor &color, const deoglRImage::Ref *images);
	
	
	
	/** Previous entry in the linked list. */
	inline deoglCombinedTexture *GetLLPrev() const{return pLLPrev;}
	
	/** Set previous entry in the linked list. */
	void SetLLPrev(deoglCombinedTexture *entry);
	
	/** Next entry in the linked list. */
	inline deoglCombinedTexture *GetLLNext() const{return pLLNext;}
	
	/** Set next entry in the linked list. */
	void SetLLNext(deoglCombinedTexture *entry);
	/*@}*/
};

#endif
