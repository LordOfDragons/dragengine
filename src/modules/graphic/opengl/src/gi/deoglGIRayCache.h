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

#ifndef _DEOGLGIRAYCACHE_H_
#define _DEOGLGIRAYCACHE_H_

#include "../texture/arraytexture/deoglArrayTexture.h"

class deoglRenderThread;


/**
 * Global illumination rays.
 */
class deoglGIRayCache{
private:
	deoglRenderThread &pRenderThread;
	
	int pRaysPerProbe;
	int pProbesPerLine;
	int pProbeCount;
	int pLayerCount;
	decVector2 pRayMapScale;
	
	deoglArrayTexture pTexDistance;
	deoglArrayTexture pTexNormal;
	deoglArrayTexture pTexDiffuse;
	deoglArrayTexture pTexReflectivity;
	deoglArrayTexture pTexLight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination ray tracing. */
	deoglGIRayCache( deoglRenderThread &renderThread, int raysPerProbe, int probeCount, int layerCount );
	
	/** Clean up global illumination ray tracing. */
	~deoglGIRayCache();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Rays per probe. */
	inline int GetRaysPerProbe() const{ return pRaysPerProbe; }
	
	/** Set rays per probe. */
	void SetRaysPerProbe( int raysPerProbe );
	
	/** Probes per line in result textures. */
	inline int GetProbesPerLine() const{ return pProbesPerLine; }
	
	/** Probe count. */
	inline int GetProbeCount() const{ return pProbeCount; }
	
	/** Set probe count. FBO and textures are valid only after this call. */
	void SetProbeCount( int count );
	
	/** Layer count. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Set layer count. FBO and textures are valid only after this call. */
	void SetLayerCount( int count );
	
	/** Ray map scale. */
	inline const decVector2 &GetRayMapScale() const{ return pRayMapScale; }
	
	
	
	/** Distance texture. */
	inline deoglArrayTexture &GetTextureDistance(){ return pTexDistance; }
	inline const deoglArrayTexture &GetTextureDistance() const{ return pTexDistance; }
	
	/** Normal texture. */
	inline deoglArrayTexture &GetTextureNormal(){ return pTexNormal; }
	inline const deoglArrayTexture &GetTextureNormal() const{ return pTexNormal; }
	
	/** Diffuse/TintMask texture. */
	inline deoglArrayTexture &GetTextureDiffuse(){ return pTexDiffuse; }
	inline const deoglArrayTexture &GetTextureDiffuse() const{ return pTexDiffuse; }
	
	/** Reflectivity/Roughness texture. */
	inline deoglArrayTexture &GetTextureReflectivity(){ return pTexReflectivity; }
	inline const deoglArrayTexture &GetTextureReflectivity() const{ return pTexReflectivity; }
	
	/** Light texture. */
	inline deoglArrayTexture &GetTextureLight(){ return pTexLight; }
	inline const deoglArrayTexture &GetTextureLight() const{ return pTexLight; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateFBO();
};

#endif
