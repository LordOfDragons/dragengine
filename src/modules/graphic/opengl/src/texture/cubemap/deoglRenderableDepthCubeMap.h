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

#ifndef _DEOGLRENDERABLEDEPTHCUBEMAP_H_
#define _DEOGLRENDERABLEDEPTHCUBEMAP_H_

class deoglRenderThread;
class deoglCubeMap;



/**
 * Renderable Depth CubeMap.
 * Stores a renderable cubemap of any kind. Renderable cubemaps can not be
 * used by more than one entity at the same time. They can also not change
 * the size of format once created. They can though be reused if the entity
 * holding the cubemap gives it up.
 */
class deoglRenderableDepthCubeMap{
private:
	int pSize;
	bool pUseFloat;
	bool pInUse;
	
	deoglCubeMap *pCubeMap;
	
	int pMemoryUsageGPU;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable cubemap. */
	deoglRenderableDepthCubeMap( deoglRenderThread &renderThread, int size, bool useFloat );
	/** Cleans up the renderable cubemap. */
	~deoglRenderableDepthCubeMap();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the size. */
	inline int GetSize() const{ return pSize; }
	
	/** Use float depth. */
	inline bool GetUseFloat() const{ return pUseFloat; }
	
	/** Determines if the format matches the given format. */
	bool Matches( int size, bool useFloat ) const;
	
	/** Determines if the shadow map is in use. */
	inline bool GetInUse() const{ return pInUse; }
	/** Sets if the shadow map is in use. */
	void SetInUse( bool inUse );
	
	/** Retrieves the cubemap. */
	inline deoglCubeMap *GetCubeMap() const{ return pCubeMap; }
	
	/** Retrieves the GPU memory usage. */
	inline int GetMemoryUsageGPU() const{ return pMemoryUsageGPU; }
	/** Updates the GPU memory consumption. */
	void UpdateMemoryUsage();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
