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

#ifndef _DEOGLGI_H_
#define _DEOGLGI_H_

#include "deoglGIBVH.h"
#include "deoglGIRays.h"
#include "deoglGIMaterials.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../texture/deoglRenderbuffer.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../texture/texture2d/deoglTexture.h"

class deoglRenderThread;
class deoglROcclusionMesh;


//#define ENABLE_GI 1


/**
 * \brief Global illumination.
 * 
 * Shared parameters and data for global illumination.
 */
class deoglGI{
public:
	/** \brief UBO Parameters. */
	enum eUBOParameters{
		eupSampleImageScale, // vec2: scale factor for sample image size
		eupProbeCount, // int: count of probes to update
		eupRaysPerProbe, // int: count of rays per probe
		eupGridProbeCount, // ivec3: count of probes in grid
		eupProbesPerLine, // int: count of probes per line
		eupGridProbeSpacing, // vec3: spacing of probes in grid
		eupBVHInstanceRootNode, // int
		eupOcclusionMapScale, // vec2: scale factor for occlusion map
		eupDistanceMapScale, // vec2: scale factor for distance map
		eupOcclusionMapSize, // int: size of occlusion map
		eupDistanceMapSize, // int: size of distance map
		eupMaxProbeDistance, // float
		eupDepthSharpness, // float
		eupFieldOrigin, // vec3
		eupProbeIndex, // ivec4[]: group of 4 probes to trace
		eupProbePosition, // vec3[]: probe position and ray origin
		eupRayDirection // vec3[]: ray direction
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglGIBVH pBVH;
	deoglGIRays pRays;
	deoglGIMaterials pMaterials;
	
	deObjectReference pUBO;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create global illumination. */
	deoglGI( deoglRenderThread &renderThread );
	
	/** \brief Clean up global illumination. */
	~deoglGI();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief BVH. */
	inline deoglGIBVH &GetBVH(){ return pBVH; }
	inline const deoglGIBVH &GetBVH() const{ return pBVH; }
	
	/** \brief Rays. */
	inline deoglGIRays &GetRays(){ return pRays; }
	inline const deoglGIRays &GetRays() const{ return pRays; }
	
	/** \brief Materials. */
	inline deoglGIMaterials &GetMaterials(){ return pMaterials; }
	inline const deoglGIMaterials &GetMaterials() const{ return pMaterials; }
	
	/** \brief UBO. */
	inline deoglSPBlockUBO &GetUBO() const{ return ( deoglSPBlockUBO& )( deObject& )pUBO; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateUBO();
};

#endif
