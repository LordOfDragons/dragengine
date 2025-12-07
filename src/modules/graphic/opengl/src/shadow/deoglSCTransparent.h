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

#ifndef _DEOGLSCTRANSPARENT_H_
#define _DEOGLSCTRANSPARENT_H_

#include "../memory/consumption/deoglMemoryConsumptionGPUUse.h"

class deoglRenderThread;
class deoglCubeMap;
class deoglTexture;
class deoglRenderableDepthTexture;
class deoglRenderableColorTexture;
class deoglRenderableDepthCubeMap;
class deoglRenderableColorCubeMap;



/**
 * Shadow Caster Transparent.
 */
class deoglSCTransparent{
private:
	deoglRenderThread &pRenderThread;
	
	deoglTexture *pStaticShadowMap;
	deoglTexture *pStaticColorMap;
	deoglCubeMap *pStaticShadowCubeMap;
	deoglCubeMap *pStaticColorCubeMap;
	int pLastUseStatic;
	bool pHasStatic;
	
	deoglTexture *pDynamicShadowMap;
	deoglTexture *pDynamicColorMap;
	deoglCubeMap *pDynamicShadowCubeMap;
	deoglCubeMap *pDynamicColorCubeMap;
	int pLastUseDynamic;
	bool pHasDynamic;
	bool pDirtyDynamic;
	
	deoglRenderableDepthTexture *pTemporaryShadowMap;
	deoglRenderableColorTexture *pTemporaryColorMap;
	deoglRenderableDepthCubeMap *pTemporaryShadowCubeMap;
	deoglRenderableColorCubeMap *pTemporaryColorCubeMap;
	
	int pLastSizeStatic;
	int pNextSizeStatic;
	int pLastSizeDynamic;
	int pNextSizeDynamic;
	
	deoglMemoryConsumptionGPUUse pMemUseStaMapDepth;
	deoglMemoryConsumptionGPUUse pMemUseStaMapColor;
	deoglMemoryConsumptionGPUUse pMemUseStaCubeDepth;
	deoglMemoryConsumptionGPUUse pMemUseStaCubeColor;
	deoglMemoryConsumptionGPUUse pMemUseDynMapDepth;
	deoglMemoryConsumptionGPUUse pMemUseDynMapColor;
	deoglMemoryConsumptionGPUUse pMemUseDynCubeDepth;
	deoglMemoryConsumptionGPUUse pMemUseDynCubeColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create transparent shadow caster. */
	deoglSCTransparent(deoglRenderThread &renderThread);
	
	/** Clean up transparent shadow caster. */
	~deoglSCTransparent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Static shadow map or \em NULL if absent. */
	inline deoglTexture *GetStaticShadowMap() const{return pStaticShadowMap;}
	
	/** Static shadow map with size. */
	deoglTexture *ObtainStaticShadowMapWithSize(int size, bool useFloat);
	
	/** Static color map or \em NULL if absent. */
	inline deoglTexture *GetStaticColorMap() const{return pStaticColorMap;}
	
	/** Static color map with size. */
	deoglTexture *ObtainStaticColorMapWithSize(int size);
	
	/** Static shadow cube map or \em NULL if absent. */
	inline deoglCubeMap *GetStaticShadowCubeMap() const{return pStaticShadowCubeMap;}
	
	/** Static shadow cube map with size. */
	deoglCubeMap *ObtainStaticShadowCubeMapWithSize(int size, bool useFloat);
	
	/** Static color cube map or \em NULL if absent. */
	inline deoglCubeMap *GetStaticColorCubeMap() const{return pStaticColorCubeMap;}
	
	/** Static color cube map with size. */
	deoglCubeMap *ObtainStaticColorCubeMapWithSize(int size);
	
	/** Drop static maps if present. */
	void DropStatic();
	
	/** Number of frames elapsed since the last time static maps have been used. */
	inline int GetLastUseStatic() const{return pLastUseStatic;}
	
	/** Increment last use static maps counter by one. */
	void IncrementLastUseStatic();
	
	/** Reset last use static maps counter. */
	void ResetLastUseStatic();
	
	
	
	/** Dynamic shadow map if present or \em NULL otherwise. */
	inline deoglTexture *GetDynamicShadowMap() const{return pDynamicShadowMap;}
	
	/** Obtain dynamic shadow map with size if absent. */
	deoglTexture *ObtainDynamicShadowMapWithSize(int size, bool useFloat);
	
	/** Dynamic color map if present or \em NULL otherwise. */
	inline deoglTexture *GetDynamicColorMap() const{return pDynamicColorMap;}
	
	/** Obtain dynamic color map with size if absent. */
	deoglTexture *ObtainDynamicColorMapWithSize(int size);
	
	/** Dynamic shadow cube map if present or \em NULL otherwise. */
	inline deoglCubeMap *GetDynamicShadowCubeMap() const{return pDynamicShadowCubeMap;}
	
	/** Obtain dynamic shadow cube map with size if absent. */
	deoglCubeMap *ObtainDynamicShadowCubeMapWithSize(int size, bool useFloat);
	
	/** Dynamic color cube map if present or \em NULL otherwise. */
	inline deoglCubeMap *GetDynamicColorCubeMap() const{return pDynamicColorCubeMap;}
	
	/** Obtain dynamic shadow cube map with size if absent. */
	deoglCubeMap *ObtainDynamicColorCubeMapWithSize(int size);
	
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
	
	
	
	/** Temporary shadow map if present or \em NULL otherwise. */
	inline deoglRenderableDepthTexture *GetTemporaryShadowMap() const{return pTemporaryShadowMap;}
	
	/** Obtain temporary shadow map with size if absent. */
	deoglRenderableDepthTexture *ObtainTemporaryShadowMapWithSize(int size, bool useFloat);
	
	/** Temporary color map if present or \em NULL otherwise. */
	inline deoglRenderableColorTexture *GetTemporaryColorMap() const{return pTemporaryColorMap;}
	
	/** Obtain temporary color map with size if absent. */
	deoglRenderableColorTexture *ObtainTemporaryColorMapWithSize(int size);
	
	/** Temporary shadow cube map if present or \em NULL otherwise. */
	inline deoglRenderableDepthCubeMap *GetTemporaryShadowCubeMap() const{return pTemporaryShadowCubeMap;}
	
	/** Obtain temporary shadow cube map with size if absent. */
	deoglRenderableDepthCubeMap *ObtainTemporaryShadowCubeMapWithSize(int size, bool useFloat);
	
	/** Temporary color cube map if present or \em NULL otherwise. */
	inline deoglRenderableColorCubeMap *GetTemporaryColorCubeMap() const{return pTemporaryColorCubeMap;}
	
	/** Obtain temporary shadow cube map with size if absent. */
	deoglRenderableColorCubeMap *ObtainTemporaryColorCubeMapWithSize(int size);
	
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
	
	/** Clear transparent shadow caster. */
	void Clear();
	/*@}*/
};

#endif
