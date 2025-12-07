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

#ifndef _DEOGLSCSOLID_H_
#define _DEOGLSCSOLID_H_

#include "../memory/consumption/deoglMemoryConsumptionGPUUse.h"

class deoglRenderThread;
class deoglArrayTexture;
class deoglTexture;
class deoglCubeMap;
class deoglRenderableDepthTexture;
class deoglRenderableDepthCubeMap;
class deoglRenderableDepthArrayTexture;



/**
 * Shadow caster solid faces.
 */
class deoglSCSolid{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexture *pStaticMap;
	deoglCubeMap *pStaticCubeMap;
	deoglArrayTexture *pStaticArrayMap;
	int pLastUseStatic;
	bool pHasStatic;
	
	deoglTexture *pDynamicMap;
	deoglCubeMap *pDynamicCubeMap;
	deoglArrayTexture *pDynamicArrayMap;
	int pLastUseDynamic;
	bool pHasDynamic;
	bool pDirtyDynamic;
	
	deoglRenderableDepthTexture *pTemporaryMap;
	deoglRenderableDepthCubeMap *pTemporaryCubeMap;
	deoglRenderableDepthArrayTexture *pTemporaryArrayMap;
	
	int pLastSizeStatic;
	int pNextSizeStatic;
	int pLastSizeDynamic;
	int pNextSizeDynamic;
	
	deoglMemoryConsumptionGPUUse pMemUseStaMap;
	deoglMemoryConsumptionGPUUse pMemUseStaCube;
	deoglMemoryConsumptionGPUUse pMemUseStaArray;
	deoglMemoryConsumptionGPUUse pMemUseDynMap;
	deoglMemoryConsumptionGPUUse pMemUseDynCube;
	deoglMemoryConsumptionGPUUse pMemUseDynArray;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create solid shadow caster. */
	deoglSCSolid(deoglRenderThread &renderThread);
	
	/** Clean up solid shadow caster. */
	~deoglSCSolid();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Static shadow map if present or NULL otherwise. */
	inline deoglTexture *GetStaticMap() const{return pStaticMap;}
	
	/** Request static shadow map with size if absent. */
	deoglTexture *ObtainStaticMapWithSize(int size, bool withStencil, bool useFloat);
	
	/** Static shadow cube map if present or NULL otherwise. */
	inline deoglCubeMap *GetStaticCubeMap() const{return pStaticCubeMap;}
	
	/** Request static cube map with size if absent. */
	deoglCubeMap *ObtainStaticCubeMapWithSize(int size, bool useFloat);
	
	/** Static array shadow map if present or NULL otherwise. */
	inline deoglArrayTexture *GetStaticArrayMap() const{return pStaticArrayMap;}
	
	/** Request static array shadow map with size if absent. */
	deoglArrayTexture *ObtainStaticArrayMapWithSize(int size, int layers, bool useFloat);
	
	/** Drop static shadow map if present. */
	void DropStatic();
	
	/** Number of frames elapsed since the last time static shadow map has been used. */
	inline int GetLastUseStatic() const{return pLastUseStatic;}
	
	/** Increment last use static shadow map counter by one. */
	void IncrementLastUseStatic();
	
	/** Reset last use static shadow map counter. */
	void ResetLastUseStatic();
	
	
	
	/** Dynamic shadow map if present or NULL otherwise. */
	inline deoglTexture *GetDynamicMap() const{return pDynamicMap;}
	
	/** Obtain dynamic shadow map with size if absent. */
	deoglTexture *ObtainDynamicMapWithSize(int size, bool withStencil, bool useFloat);
	
	/** Dynamic shadow cube map if present or NULL otherwise. */
	inline deoglCubeMap *GetDynamicCubeMap() const{return pDynamicCubeMap;}
	
	/** Obtain dynamic shadow cube map with size if absent. */
	deoglCubeMap *ObtainDynamicCubeMapWithSize(int size, bool useFloat);
	
	/** Dynamic array shadow map if present or NULL otherwise. */
	inline deoglArrayTexture *GetDynamicArrayMap() const{return pDynamicArrayMap;}
	
	/** Obtain dynamic array shadow map with size if absent. */
	deoglArrayTexture *ObtainDynamicArrayMapWithSize(int size, int layers, bool useFloat);
	
	/** Drop dynamic shadow map if present. */
	void DropDynamic();
	
	/** Number of frames elapsed since the last time dynamic shadow map has been used. */
	inline int GetLastUseDynamic() const{return pLastUseDynamic;}
	
	/** Increment last use dynamic shadow map counter by one. */
	void IncrementLastUseDynamic();
	
	/** Reset last use dynamic shadow map counter. */
	void ResetLastUseDynamic();
	
	/** Dynamic shadow map is dirty. */
	inline bool GetDirtyDynamic() const{return pDirtyDynamic;}
	
	/** Set dynamic shadow map dirty. */
	void SetDirtyDynamic(bool dirty);
	
	
	
	/** Temporary shadow map if present or NULL otherwise. */
	inline deoglRenderableDepthTexture *GetTemporaryMap() const{return pTemporaryMap;}
	
	/** Obtain temporary shadow map with size if absent. */
	deoglRenderableDepthTexture *ObtainTemporaryMapWithSize(int size, bool withStencil, bool useFloat);
	
	/** Temporary shadow cube map if present or NULL otherwise. */
	inline deoglRenderableDepthCubeMap *GetTemporaryCubeMap() const{return pTemporaryCubeMap;}
	
	/** Obtain temporary shadow cube map with size if absent. */
	deoglRenderableDepthCubeMap *ObtainTemporaryCubeMapWithSize(int size, bool useFloat);
	
	/** Temporary array shadow map if present or NULL otherwise. */
	inline deoglRenderableDepthArrayTexture *GetTemporaryArrayMap() const{return pTemporaryArrayMap;}
	
	/** Obtain temporary array shadow map with size if absent. */
	deoglRenderableDepthArrayTexture *ObtainTemporaryArrayMapWithSize(int size, int layers, bool useFloat);
	
	/** Drop temporary shadow map if present. */
	void DropTemporary();
	
	
	
	/** Last frame static size or 0. */
	inline int GetLastSizeStatic() const{return pLastSizeStatic;}
	
	/** Next frame static size or 0. */
	inline int GetNextSizeStatic() const{return pNextSizeStatic;}
	
	/** Set next frame static size to largest value. */
	void SetLargestNextSizeStatic(int size);
	
	/** Last frame dynamic size or 0. */
	inline int GetLastSizeDynamic() const{return pLastSizeDynamic;}
	
	/** Next frame dynamic size or 0. */
	inline int GetNextSizeDynamic() const{return pNextSizeDynamic;}
	
	/** Set next frame dynamic size to largest value. */
	void SetLargestNextSizeDynamic(int size);
	
	
	
	/** Check if static maps have not been used recently removing them. */
	void Update();
	
	/** Shadow caster requires update. True if timers are running to drop textures. */
	bool RequiresUpdate() const;
	
	/** Drop all maps. */
	void Clear();
	/*@}*/
};

#endif
