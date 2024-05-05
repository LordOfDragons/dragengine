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

#ifndef _DEOGLRENDERABLECOLORARRAYTEXTURE_H_
#define _DEOGLRENDERABLECOLORARRAYTEXTURE_H_

class deoglRenderThread;
class deoglArrayTexture;



/**
 * Stores a renderable texture of any kind. Renderable textures can not be
 * used by more than one entity at the same time. They can also not change
 * the size of format once created. They can though be reused if the entity
 * holding the texture gives it up.
 */
class deoglRenderableColorArrayTexture{
private:
	int pWidth;
	int pHeight;
	int pLayerCount;
	int pComponentCount;
	bool pIsFloat;
	bool pInUse;
	
	deoglArrayTexture *pArrayTexture;
	
	int pMemoryUsageGPU;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderable texture. */
	deoglRenderableColorArrayTexture( deoglRenderThread &renderThread, int width,
		int height, int layerCount, int componentCount, bool isFloat );
	
	/** Clean up renderable texture. */
	~deoglRenderableColorArrayTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Height. */
	inline int GetHeight() const{ return pHeight; }
	
	/** Layer count. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Count of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Use float format. */
	inline bool GetIsFloat() const{ return pIsFloat; }
	
	/** Format matches. */
	bool Matches( int width, int height, int layers, int componentCount, bool isFloat ) const;
	
	/** Texture is in use. */
	inline bool GetInUse() const{ return pInUse; }
	
	/** Set if texture is in use. */
	void SetInUse( bool inUse );
	
	/** Texture. */
	inline deoglArrayTexture *GetArrayTexture() const{ return pArrayTexture; }
	
	/** GPU memory usage. */
	inline int GetMemoryUsageGPU() const{ return pMemoryUsageGPU; }
	
	/** Update GPU memory consumption. */
	void UpdateMemoryUsage();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
