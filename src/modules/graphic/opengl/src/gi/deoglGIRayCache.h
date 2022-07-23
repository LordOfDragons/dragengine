/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOGLGIRAYCACHE_H_
#define _DEOGLGIRAYCACHE_H_

#include "../framebuffer/deoglFramebuffer.h"
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
	deoglFramebuffer pFBOResult;
	
	
	
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
	
	/** Result FBO. */
	inline deoglFramebuffer &GetFBOResult(){ return pFBOResult; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateFBO();
};

#endif
