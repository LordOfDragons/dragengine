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

#include "deoglGIBVHShared.h"
#include "deoglGITraceRays.h"
#include "deoglGIMaterials.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"

class deoglRenderThread;

#define GI_USE_RAY_CACHE 1

// #define GI_RENDERDOC_DEBUG 1

#if defined GI_RENDERDOC_DEBUG && defined GI_USE_RAY_CACHE
#undef GI_USE_RAY_CACHE
#endif


/**
 * Global illumination.
 * 
 * Shared parameters and data for global illumination.
 */
class deoglGI{
public:
	/** UBO Parameters. */
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
		eupFieldSize, // vec3
		eupBlendUpdateProbe, // float
		eupMoveMaxOffset, // vec3
		eupMoveMinDistToSurface, // float
		eupRayMapScale, // vec2: scale factor for ray map
		eupMaterialMapsPerRow, // int
		eupMaterialMapSize, // int
		eupIrradianceGamma, // float
		eupInvIrradianceGamma, // float
		eupSelfShadowBias, // float
		eupCascade, // int
		eupDetectionBox, // vec3
		euppRayCacheProbeCount, // int
		eupBVHOffset // vec3
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglGIBVHShared pBVHShared;
	deoglGITraceRays pTraceRays;
	deoglGIMaterials pMaterials;
	
	deoglSPBlockUBO::Ref pUBOParameter;
	deoglSPBlockUBO::Ref pUBOProbeIndex;
	deoglSPBlockUBO::Ref pUBOProbePosition;
	deoglSPBlockUBO::Ref pUBORayDirection;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination. */
	deoglGI( deoglRenderThread &renderThread );
	
	/** Clean up global illumination. */
	~deoglGI();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** BVH Shared. */
	inline deoglGIBVHShared &GetBVHShared(){ return pBVHShared; }
	inline const deoglGIBVHShared &GetBVHShared() const{ return pBVHShared; }
	
	/** Trace rays. */
	inline deoglGITraceRays &GetTraceRays(){ return pTraceRays; }
	inline const deoglGITraceRays &GetTraceRays() const{ return pTraceRays; }
	
	/** Materials. */
	inline deoglGIMaterials &GetMaterials(){ return pMaterials; }
	inline const deoglGIMaterials &GetMaterials() const{ return pMaterials; }
	
	/** UBO. */
	inline deoglSPBlockUBO &GetUBOParameter() const{ return pUBOParameter; }
	inline deoglSPBlockUBO &GetUBOProbeIndex() const{ return pUBOProbeIndex; }
	inline deoglSPBlockUBO &GetUBOProbePosition() const{ return pUBOProbePosition; }
	inline deoglSPBlockUBO &GetUBORayDirection() const{ return pUBORayDirection; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateUBOParameter();
	void pCreateUBOProbeIndex();
	void pCreateUBOProbePosition();
	void pCreateUBORayDirection();
};

#endif
