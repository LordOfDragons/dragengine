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

#ifndef _DEOGLGI_H_
#define _DEOGLGI_H_

#include "deoglGIBVHShared.h"
#include "deoglGITraceRays.h"
#include "deoglGIMaterials.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBSingleUse.h"

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
	
	deoglSPBSingleUse::Ref pUBOParameterSingleUse;
	deoglSPBlockUBO::Ref pUBOParameter;
	
	deoglSPBSingleUse::Ref pUBOProbeIndexSingleUse;
	deoglSPBlockUBO::Ref pUBOProbeIndex;
	
	deoglSPBSingleUse::Ref pUBOProbePositionSingleUse;
	deoglSPBlockUBO::Ref pUBOProbePosition;
	
	deoglSPBSingleUse::Ref pUBORayDirectionSingleUse;
	deoglSPBlockUBO::Ref pUBORayDirection;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination. */
	deoglGI(deoglRenderThread &renderThread);
	
	/** Clean up global illumination. */
	~deoglGI();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	/** BVH Shared. */
	inline deoglGIBVHShared &GetBVHShared(){return pBVHShared;}
	inline const deoglGIBVHShared &GetBVHShared() const{return pBVHShared;}
	
	/** Trace rays. */
	inline deoglGITraceRays &GetTraceRays(){return pTraceRays;}
	inline const deoglGITraceRays &GetTraceRays() const{return pTraceRays;}
	
	/** Materials. */
	inline deoglGIMaterials &GetMaterials(){return pMaterials;}
	inline const deoglGIMaterials &GetMaterials() const{return pMaterials;}
	
	/** UBO. */
	deoglSPBlockUBO &NextUBOParameter();
	inline deoglSPBlockUBO &GetUBOParameter() const{return pUBOParameter;}
	
	deoglSPBlockUBO &NextUBOProbeIndex();
	inline deoglSPBlockUBO &GetUBOProbeIndex() const{return pUBOProbeIndex;}
	
	deoglSPBlockUBO &NextUBOProbePosition();
	inline deoglSPBlockUBO &GetUBOProbePosition() const{return pUBOProbePosition;}
	
	deoglSPBlockUBO &NextUBORayDirection();
	inline deoglSPBlockUBO &GetUBORayDirection() const{return pUBORayDirection;}
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateUBOParameter();
	void pCreateUBOProbeIndex();
	void pCreateUBOProbePosition();
	void pCreateUBORayDirection();
};

#endif
