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
#include "deoglGITraceRays.h"
#include "deoglGIMaterials.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"

class deoglRenderThread;
class deoglROcclusionMesh;


#define GI_MAX_PROBE_COUNT 4096


// #define GI_ENABLE 1
// #define GI_USE_RAY_LIMIT 1
#define GI_USE_RAY_CACHE 1
#define GI_MOVE_PROBES_RAY_CACHE 1

// #define GI_RENDERDOC_DEBUG 1

#if defined GI_RENDERDOC_DEBUG && defined GI_USE_RAY_CACHE
#undef GI_USE_RAY_CACHE
#endif


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
		eupIrradianceMapScale, // vec2: scale factor for irradiance map
		eupDistanceMapScale, // vec2: scale factor for distance map
		eupIrradianceMapSize, // int: size of irradiance map
		eupDistanceMapSize, // int: size of distance map
		eupMaxProbeDistance, // float
		eupDepthSharpness, // float
		eupGridOrigin, // vec3
		eupGridCoordUnshift, // ivec3
		eupFieldOrigin, // vec3
		eupBlendUpdateProbe, // float
		eupMoveMaxOffset, // vec3
		eupMoveMinDistToSurface, // float
		eupRayMapScale, // vec2: scale factor for ray map
		eupMaterialMapsPerRow, // int
		eupMaterialMapSize // int
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglGIBVH pBVH;
	deoglGITraceRays pTraceRays;
	deoglGIMaterials pMaterials;
	
	deObjectReference pUBOParameter;
	deObjectReference pUBOProbeIndex;
	deObjectReference pUBOProbePosition;
	deObjectReference pUBORayDirection;
	
	
	
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
	
	/** \brief Trace rays. */
	inline deoglGITraceRays &GetTraceRays(){ return pTraceRays; }
	inline const deoglGITraceRays &GetTraceRays() const{ return pTraceRays; }
	
	/** \brief Materials. */
	inline deoglGIMaterials &GetMaterials(){ return pMaterials; }
	inline const deoglGIMaterials &GetMaterials() const{ return pMaterials; }
	
	/** \brief UBO. */
	inline deoglSPBlockUBO &GetUBOParameter() const{ return ( deoglSPBlockUBO& )( deObject& )pUBOParameter; }
	inline deoglSPBlockUBO &GetUBOProbeIndex() const{ return ( deoglSPBlockUBO& )( deObject& )pUBOProbeIndex; }
	inline deoglSPBlockUBO &GetUBOProbePosition() const{ return ( deoglSPBlockUBO& )( deObject& )pUBOProbePosition; }
	inline deoglSPBlockUBO &GetUBORayDirection() const{ return ( deoglSPBlockUBO& )( deObject& )pUBORayDirection; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateUBOParameter();
	void pCreateUBOProbeIndex();
	void pCreateUBOProbePosition();
	void pCreateUBORayDirection();
};

#endif
