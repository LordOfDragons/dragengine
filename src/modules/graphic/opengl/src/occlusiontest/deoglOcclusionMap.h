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

#ifndef _DEOGLOCCLUSIONMAP_H_
#define _DEOGLOCCLUSIONMAP_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglArrayTexture;
class deoglFramebuffer;



/**
 * Occlusion map used for occlusion testing. The occlusion map stores a Z-Pyramid
 * of the depth in the scene used for testing. For each lod level in the occlusion
 * map an own framebuffer is used.
 */
class deoglOcclusionMap{
private:
	deoglRenderThread &pRenderThread;
	
	deoglArrayTexture *pTexture;
	deoglFramebuffer **pFBOs;
	
	int pWidth;
	int pHeight;
	int pLayerCount;
	int pLevelCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create occlusion map. */
	deoglOcclusionMap( deoglRenderThread &renderThread, int width, int height, int layerCount );
	
	/** Clean up occlusion map. */
	~deoglOcclusionMap();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Width of the base level. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Height of the base level. */
	inline int GetHeight() const{ return pHeight; }
	
	/** Layer count. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Level count. */
	inline int GetLevelCount() const{ return pLevelCount; }
	
	/** Texture. */
	inline deoglArrayTexture *GetTexture() const{ return pTexture; }
	
	/** FBO for level. */
	deoglFramebuffer *GetFBOAt( int level );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateTextures();
	void pCreateFBOs();
};

#endif
