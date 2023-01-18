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

#ifndef _DEOGLGITRACERAYS_H_
#define _DEOGLGITRACERAYS_H_

#include "../framebuffer/deoglFramebuffer.h"
#include "../texture/texture2d/deoglTexture.h"

class deoglRenderThread;
class deoglConfiguration;

#define GI_MAX_RAYS_PER_PROBE 256

// 4096 is the maximum supported in smallest UBO size
#define GI_MAX_PROBE_COUNT 2048


/**
 * Stores temporary result of tracing global illumination rays.
 */
class deoglGITraceRays{
private:
	deoglRenderThread &pRenderThread;
	
	int pRaysPerProbe;
	int pProbesPerLine;
	int pProbeCount;
	
	deoglTexture pTexPosition;
	deoglTexture pTexNormal;
	deoglTexture pTexDiffuse;
	deoglTexture pTexReflectivity;
	deoglTexture pTexLight;
	deoglFramebuffer pFBOResult;
	deoglFramebuffer pFBODistance;
	deoglFramebuffer pFBOLight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination ray tracing. */
	deoglGITraceRays( deoglRenderThread &renderThread );
	
	/** Clean up global illumination ray tracing. */
	~deoglGITraceRays();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Rays per probe. */
	inline int GetRaysPerProbe() const{ return pRaysPerProbe; }
	
	/** Probes per line in result textures. */
	inline int GetProbesPerLine() const{ return pProbesPerLine; }
	
	/** Probe count in result textures. */
	inline int GetProbeCount() const{ return pProbeCount; }
	
	/** Get rays per probe matching configuration quality. */
	static int ConfigRaysPerProbe( const deoglConfiguration &config );
	
	/** Get probe count matching configuration quality. */
	static int ConfigProbeCount( const deoglConfiguration &config );
	
	/** Update from configuration. */
	void UpdateFromConfig();
	
	
	
	/** Position texture (also used for distance using a component only). */
	inline deoglTexture &GetTexturePosition(){ return pTexPosition; }
	inline const deoglTexture &GetTexturePosition() const{ return pTexPosition; }
	
	/** Normal texture. */
	inline deoglTexture &GetTextureNormal(){ return pTexNormal; }
	inline const deoglTexture &GetTextureNormal() const{ return pTexNormal; }
	
	/** Diffuse/TintMask texture. */
	inline deoglTexture &GetTextureDiffuse(){ return pTexDiffuse; }
	inline const deoglTexture &GetTextureDiffuse() const{ return pTexDiffuse; }
	
	/** Reflectivity/Roughness texture. */
	inline deoglTexture &GetTextureReflectivity(){ return pTexReflectivity; }
	inline const deoglTexture &GetTextureReflectivity() const{ return pTexReflectivity; }
	
	/** Light texture. */
	inline deoglTexture &GetTextureLight(){ return pTexLight; }
	inline const deoglTexture &GetTextureLight() const{ return pTexLight; }
	
	/** Result FBO. */
	inline deoglFramebuffer &GetFBOResult(){ return pFBOResult; }
	
	/** Distance FBO. */
	inline deoglFramebuffer &GetFBODistance(){ return pFBODistance; }
	
	/** Light FBO. */
	inline deoglFramebuffer &GetFBOLight(){ return pFBOLight; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateFBORay();
};

#endif
