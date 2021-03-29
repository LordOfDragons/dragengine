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

#ifndef _DEOGLGIRAYS_H_
#define _DEOGLGIRAYS_H_

#include "../framebuffer/deoglFramebuffer.h"
#include "../texture/texture2d/deoglTexture.h"

class deoglRenderThread;


/**
 * Global illumination rays.
 */
class deoglGIRays{
private:
	deoglRenderThread &pRenderThread;
	
	int pRaysPerProbe;
	int pProbesPerLine;
	int pProbeCount;
	decVector2 pRayMapScale;
	
	deoglTexture pTexDistance;
	deoglTexture pTexNormal;
	deoglTexture pTexDiffuse;
	deoglTexture pTexReflectivity;
	deoglTexture pTexLight;
	deoglFramebuffer pFBOResult;
	
	deoglTexture pTexDistanceLimit;
	deoglFramebuffer pFBODistanceLimit;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination ray tracing. */
	deoglGIRays( deoglRenderThread &renderThread, int raysPerProbe, int probeCount );
	
	/** Clean up global illumination ray tracing. */
	~deoglGIRays();
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
	
	/** Ray map scale. */
	inline const decVector2 &GetRayMapScale() const{ return pRayMapScale; }
	
	
	
	/** Distance texture. */
	inline deoglTexture &GetTextureDistance(){ return pTexDistance; }
	
	/** Normal texture. */
	inline deoglTexture &GetTextureNormal(){ return pTexNormal; }
	
	/** Diffuse/TintMask texture. */
	inline deoglTexture &GetTextureDiffuse(){ return pTexDiffuse; }
	
	/** Reflectivity/Roughness texture. */
	inline deoglTexture &GetTextureReflectivity(){ return pTexReflectivity; }
	
	/** Light texture. */
	inline deoglTexture &GetTextureLight(){ return pTexLight; }
	
	/** Result FBO. */
	inline deoglFramebuffer &GetFBOResult(){ return pFBOResult; }
	
	
	
	/** Distance limit texture. */
	inline deoglTexture &GetTextureDistanceLimit(){ return pTexDistanceLimit; }
	
	/** Distance limit FBO. */
	inline deoglFramebuffer &GetFBODistanceLimit(){ return pFBODistanceLimit; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateFBO();
};

#endif
