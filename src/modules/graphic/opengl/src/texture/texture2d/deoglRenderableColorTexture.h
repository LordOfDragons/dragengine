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

#ifndef _DEOGLRENDERABLECOLORTEXTURE_H_
#define _DEOGLRENDERABLECOLORTEXTURE_H_

class deoglRenderThread;
class deoglTexture;



/**
 * Renderable Color Texture.
 * Stores a renderable texture of any kind. Renderable textures can not be
 * used by more than one entity at the same time. They can also not change
 * the size of format once created. They can though be reused if the entity
 * holding the texture gives it up.
 */
class deoglRenderableColorTexture{
private:
	int pWidth;
	int pHeight;
	int pComponentCount;
	bool pIsFloat;
	bool pInUse;
	
	deoglTexture *pTexture;
	
	int pMemoryUsageGPU;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable texture. */
	deoglRenderableColorTexture( deoglRenderThread &renderThread, int width, int height, int componentCount, bool isFloat );
	/** Cleans up the renderable texture. */
	~deoglRenderableColorTexture();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the width. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height. */
	inline int GetHeight() const{ return pHeight; }
	/** Retrieves the number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	/** Retrieves the texture is of float format. */
	inline bool GetIsFloat() const{ return pIsFloat; }
	
	/** Determines if the format matches the given format. */
	bool Matches( int width, int height, int componentCount, bool isFloat ) const;
	
	/** Determines if the shadow map is in use. */
	inline bool GetInUse() const{ return pInUse; }
	/** Sets if the shadow map is in use. */
	void SetInUse( bool inUse );
	
	/** Retrieves the texture. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** Retrieves the GPU memory usage. */
	inline int GetMemoryUsageGPU() const{ return pMemoryUsageGPU; }
	/** Updates the GPU memory consumption. */
	void UpdateMemoryUsage();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
