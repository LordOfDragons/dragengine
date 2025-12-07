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

#ifndef _DEOGLRWDEPTHMINMAX_H_
#define _DEOGLRWDEPTHMINMAX_H_

#include <dragengine/common/collection/decObjectList.h>

class deoglRenderThread;
class deoglArrayTexture;
class deoglFramebuffer;


/**
 * Deferred Rendering Depth Min-Max.
 * Stores a mip-mapped view of the minimum and maximum depth values inside a depth texture.
 * The maximum number of mipmap levels is limited. For each lod level an own framebuffer
 * is used.
 */
class deoglDRDepthMinMax{
public:
	static int USAGE_VERSION;
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglArrayTexture *pTexture;
	decObjectList pFBOs;
	
	deoglArrayTexture *pTextureMin, *pTextureMax;
	decObjectList pFBOMin, pFBOMax;
	
	int pWidth;
	int pHeight;
	int pLayerCount;
	int pLevelCount;
	int pMaxLevelCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create depth min-max. */
	deoglDRDepthMinMax(deoglRenderThread &renderThread,
		int width, int height, int layerCount, int maxLevelCount);
	
	/** Clean up depth min-max. */
	~deoglDRDepthMinMax();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Width of base level. */
	inline int GetWidth() const{return pWidth;}
	
	/** Height of base level. */
	inline int GetHeight() const{return pHeight;}
	
	/** Layer count. */
	inline int GetLayerCount() const{return pLayerCount;}
	
	/** Level count. */
	inline int GetLevelCount() const{return pLevelCount;}
	
	/** Maximum level count. */
	inline int GetMaxLevelCount() const{return pMaxLevelCount;}
	
	/** Retrieves the texture. */
	inline deoglArrayTexture *GetTexture() const{return pTexture;}
	/** Retrieves the fbo for a level. */
	deoglFramebuffer *GetFBOAt(int level);
	
	/** Retrieves the min texture. */
	inline deoglArrayTexture *GetTextureMin() const{return pTextureMin;}
	/** Retrieves the fbo for a level of the min texture. */
	deoglFramebuffer *GetFBOMinAt(int level);
	/** Retrieves the max texture. */
	inline deoglArrayTexture *GetTextureMax() const{return pTextureMax;}
	/** Retrieves the fbo for a level of the max texture. */
	deoglFramebuffer *GetFBOMaxAt(int level);
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateTextures();
	void pCreateFBOs();
};

#endif
