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
#include "../texture/deoglRenderbuffer.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/deObjectReference.h>

class deoglRenderThread;
class deoglSPBlockUBO;


/**
 * \brief Global illumination ray casting.
 */
class deoglGIRays{
private:
	deoglRenderThread &pRenderThread;
	
	int pRaysPerProbe;
	int pProbesPerLine;
	int pProbeCount;
	
	deoglRenderbuffer pRBufDepth;
	deoglTexture pTexPosition;
	deoglTexture pTexNormal;
	deoglTexture pTexDiffuse;
	deoglTexture pTexLight;
	deoglFramebuffer pFBOResult;
	deoglFramebuffer pFBOLight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create global illumination ray tracing. */
	deoglGIRays( deoglRenderThread &renderThread );
	
	/** \brief Clean up global illumination ray tracing. */
	~deoglGIRays();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Rays per probe. */
	inline int GetRaysPerProbe() const{ return pRaysPerProbe; }
	
	/** \brief Probes per line in result textures. */
	inline int GetProbesPerLine() const{ return pProbesPerLine; }
	
	/** \brief Probe count in result textures. */
	inline int GetProbeCount() const{ return pProbeCount; }
	
	
	
	/** \brief Position texture. */
	inline deoglTexture &GetTexturePosition(){ return pTexPosition; }
	
	/** \brief Normal texture. */
	inline deoglTexture &GetTextureNormal(){ return pTexNormal; }
	
	/** \brief Diffuse texture. */
	inline deoglTexture &GetTextureDiffuse(){ return pTexDiffuse; }
	
	/** \brief Light texture. */
	inline deoglTexture &GetTextureLight(){ return pTexLight; }
	
	/** \brief Ray tracing result FBO. */
	inline deoglFramebuffer &GetFBORayResult(){ return pFBOResult; }
	
	/** \brief Ray light FBO. */
	inline deoglFramebuffer &GetFBORayLight(){ return pFBOLight; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateFBORay();
};

#endif
