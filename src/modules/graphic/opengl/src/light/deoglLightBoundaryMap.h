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

#ifndef _DEOGLLIGHTBOUNDARYMAP_H_
#define _DEOGLLIGHTBOUNDARYMAP_H_

#include "../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>

class deoglFramebuffer;
class deoglRenderThread;
class deoglTexture;



/**
 * Light Boundary Map.
 * Boundary map used for light boundary box calculation from a shadow map. The boundary map
 * stores a Z-Pyramid of the minimum and maximum boundary of a shadow map. For each lod level
 * in the boundary map an own framebuffer is used.
 */
class deoglLightBoundaryMap{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexture *pTextureMin, *pTextureMax;
	decObjectList pFBOs;
	
	const deoglPixelBuffer::Ref pPixBufBoundaryMin, pPixBufBoundaryMax;
	
	int pSize;
	int pLevelCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new boundary map. */
	deoglLightBoundaryMap( deoglRenderThread &renderThread, int size );
	/** Cleans up the boundary map. */
	~deoglLightBoundaryMap();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the size of the base level. */
	inline int GetSize() const{ return pSize; }
	/** Retrieves the level count. */
	inline int GetLevelCount() const{ return pLevelCount; }
	/** Retrieves the base level for the given size. */
	int GetBaseLevel( int baseSize );
	
	/** Retrieves the minimum texture. */
	inline deoglTexture *GetTextureMin() const{ return pTextureMin; }
	/** Retrieves the maximum texture. */
	inline deoglTexture *GetTextureMax() const{ return pTextureMax; }
	/** Retrieves the fbo for a level. */
	deoglFramebuffer *GetFBOAt(int level);
	
	/** Retrieve the result. */
	void GetResult( decVector &boundaryMin, decVector &boundaryMax );
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateTextures();
	void pCreateFBOs();
};

#endif
