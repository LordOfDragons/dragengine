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
	const deoglFramebuffer::Ref pFBOLight;
	
	
	
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
	
	/** Light FBO. */
	inline const deoglFramebuffer::Ref &GetFBOLight(){ return pFBOLight; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateFBORay();
};

#endif
