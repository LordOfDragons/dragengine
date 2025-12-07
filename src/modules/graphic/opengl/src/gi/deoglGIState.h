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

#ifndef _DEOGLGISTATE_H_
#define _DEOGLGISTATE_H_

#include "deoglGIBVH.h"
#include "deoglGIRayCache.h"
#include "deoglGIInstances.h"
#include "deoglGIAreaTracker.h"
#include "../collidelist/deoglCollideList.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>

class deoglGICascade;
class deoglRenderThread;
class deoglRComponent;
class deoglRWorld;
class deoglDCollisionFrustum;


#define GI_MAX_CASCADES 5


/**
 * Global illumination state.
 * 
 * Stores the state of global illumination. Typically every camera owns one state.
 */
class deoglGIState{
private:
	deoglRenderThread &pRenderThread;
	
	decVector pSize;
	deoglRWorld *pWorld;
	decLayerMask pLayerMask;
	int pGIImportance;
	
	decPoint3 pProbeCount;
	decPoint3 pGridCoordClamp;
	int pStrideProbeCount;
	int pRealProbeCount;
	
	deoglGIAreaTracker pAreaTracker;
	
	int pIrradianceMapSize;
	int pDistanceMapSize;
	float pDepthSharpness;
	float pHysteresis;
	float pNormalBias;
	float pIrradianceGamma;
	float pSelfShadowBias;
	
	int pSizeTexIrradiance;
	int pSizeTexDistance;
	decVector2 pIrradianceMapScale;
	decVector2 pDistanceMapScale;
	
	deoglGICascade **pCascades;
	int pCascadeCount;
	int pActiveCascade;
	int pLastFrameCascade;
	
	int *pCascaceUpdateCycle;
	int pCascaceUpdateCycleCount;
	int pCascaceUpdateCycleIndex;
	int pCameraForceToneMapAdaptionCount;
	
	decPoint pSampleImageSize;
	
	deoglSPBlockUBO::Ref pUBOClearProbes;
	
	deoglArrayTexture pTexProbeIrradiance;
	deoglArrayTexture pTexProbeDistance;
	deoglArrayTexture pTexProbeOffset;
	bool pClearMaps;
	deoglSPBlockSSBO::Ref pPBProbeDynamicStates;
	deoglSPBlockSSBO::Ref pPBProbeOffsets;
	bool pProbesHaveMoved;
	
	deoglSPBlockSSBO::Ref pPBProbeExtends;
	bool pProbesExtendsChanged;
	
	deoglGIInstances pInstances;
	deoglGIRayCache pRayCache;
	deoglGIBVH pBVHStatic;
	deoglGIBVH pBVHDynamic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination state. */
	deoglGIState(deoglRenderThread &renderThread, const decVector &size);
	
	/** Clean up global illumination state. */
	~deoglGIState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	/** Size of GI state. */
	inline const decVector &GetSize() const{return pSize;}
	
	/** World. */
	inline deoglRWorld *GetWorld() const{return pWorld;}
	
	/** Set world. */
	void SetWorld(deoglRWorld *world);
	
	/** Layer mask. */
	inline const decLayerMask &GetLayerMask() const{return pLayerMask;}
	
	/** Set layer mask. Invalidates all if changed. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	/** GI importance. */
	inline int GetGIImportance() const{return pGIImportance;}
	
	/** Set size of GI state invalidating certain volumes and probes. */
// 	void SetSize( const decVector &size );
	
	/** Probe count. */
	inline const decPoint3 &GetProbeCount() const{return pProbeCount;}
	
	/** Grid coord clamp. */
	inline const decPoint3 &GetGridCoordClamp() const{return pGridCoordClamp;}
	
	/** Probe stride. */
	inline int GetStrideProbeCount() const{return pStrideProbeCount;}
	
	/** Real probe count. */
	inline int GetRealProbeCount() const{return pRealProbeCount;}
	
	
	
	/** Cascade count. */
	inline int GetCascadeCount() const{return pCascadeCount;}
	
	/** Cascade at index. */
	deoglGICascade &GetCascadeAt(int index) const;
	
	/** Activate cascade. */
	deoglGICascade &GetActiveCascade() const;
	
	/** Last frame cascade. */
	deoglGICascade &GetLastCascade() const;
	
	/** Cascade to use for sky shadow casting based on the active cascade. */
	deoglGICascade &GetSkyShadowCascade() const;
	
	/** Camera should force tone mapping adaption. */
	bool CameraForceToneMapAdaption() const;
	
	
	
	/** Size of sample image. */
	inline const decPoint &GetSampleImageSize() const{return pSampleImageSize;}
	
	
	
	/** Irradiance map size. */
	inline int GetIrradianceMapSize() const{return pIrradianceMapSize;}
	
	/** Distance map size. */
	inline int GetDistanceMapSize() const{return pDistanceMapSize;}
	
	/** Irradiance map scale. */
	inline const decVector2 &GetIrradianceMapScale() const{return pIrradianceMapScale;}
	
	/** Distance map scale. */
	inline const decVector2 &GetDistanceMapScale() const{return pDistanceMapScale;}
	
	/** Depth sharpness. */
	inline float GetDepthSharpness() const{return pDepthSharpness;}
	
	/** Hysteresis. */
	inline float GetHysteresis() const{return pHysteresis;}
	
	/** Normal bias. */
	inline float GetNormalBias() const{return pNormalBias;}
	
	/** Irradiance gamma. */
	inline float GetIrradianceGamma() const{return pIrradianceGamma;}
	
	/** Self shadow bias. */
	inline float GetSelfShadowBias() const{return pSelfShadowBias;}
	
	
	
	/** Clear probes UBO. */
	inline deoglSPBlockUBO &GetUBOClearProbes() const{return pUBOClearProbes;}
	
	/** Prepare clear probes UBO. */
	void PrepareUBOClearProbes() const;
	
	
	
	/** Irradiance probe texture. */
	inline deoglArrayTexture &GetTextureProbeIrradiance(){return pTexProbeIrradiance;}
	inline const deoglArrayTexture &GetTextureProbeIrradiance() const{return pTexProbeIrradiance;}
	
	/** Distance probe texture. */
	inline deoglArrayTexture &GetTextureProbeDistance(){return pTexProbeDistance;}
	inline const deoglArrayTexture &GetTextureProbeDistance() const{return pTexProbeDistance;}
	
	/** Probe offset texture. */
	inline deoglArrayTexture &GetTextureProbeOffset(){return pTexProbeOffset;}
	inline const deoglArrayTexture &GetTextureProbeOffset() const{return pTexProbeOffset;}
	
	/** Probe dynamic states parameter block. */
	inline const deoglSPBlockSSBO::Ref &GetPBProbeDynamicStates() const{return pPBProbeDynamicStates;}
	
	/** Probe offset parameter block. */
	inline const deoglSPBlockSSBO::Ref &GetPBProbeOffsets() const{return pPBProbeOffsets;}
	
	/** Probe extends feedback parameter block. */
	inline const deoglSPBlockSSBO::Ref &GetPBProbeExtends() const{return pPBProbeExtends;}
	
	/** Activate next cascade to use for upcoming Update() call. */
	void ActivateNextCascade();
	
	/** Update. */
	void Update(const decDVector &cameraPosition, const deoglDCollisionFrustum &frustum);
	
	/** Prepare UBO state. */
	void PrepareUBOState() const;
	
	/** Prepare UBO state for ray cache rendering. */
	void PrepareUBOStateRayCache() const;
	
	/** Invalid all probes. */
	void Invalidate();
	
	/** Probe moved. */
	void ProbesMoved();
	
	/** Invalidate area. */
	void InvalidateArea(const decDVector &minExtend, const decDVector &maxExtend, bool hard);
	
	/** Dynamic area changed. */
	void TouchDynamicArea(const decDVector &minExtend, const decDVector &maxExtend);
	
	/** Validate ray caches marked for update. */
	void ValidatedRayCaches();
	
	/** Notification component entered world. */
	void ComponentEnteredWorld(deoglRComponent *component);
	
	/** Notification component changed layer mask. */
	void ComponentChangedLayerMask(deoglRComponent *component);
	
	/** Notification component became visible. */
	void ComponentBecameVisible(deoglRComponent *component);
	
	/** Notification component changed gi importance. */
	void ComponentChangedGIImportance(deoglRComponent *component);
	
	/** Start read back information for the next frame update. */
	void StartReadBack();
	
	
	
	/** Instances. */
	inline deoglGIInstances &GetInstances(){return pInstances;}
	inline const deoglGIInstances &GetInstances() const{return pInstances;}
	
	/** Rays. */
	inline deoglGIRayCache &GetRayCache(){return pRayCache;}
	inline const deoglGIRayCache &GetRayCache() const{return pRayCache;}
	
	/** Static BVH. */
	inline deoglGIBVH &GetBVHStatic(){return pBVHStatic;}
	inline const deoglGIBVH &GetBVHStatic() const{return pBVHStatic;}
	
	/** Dynamic BVH. */
	inline deoglGIBVH &GetBVHDynamic(){return pBVHDynamic;}
	inline const deoglGIBVH &GetBVHDynamic() const{return pBVHDynamic;}
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pInitCascades();
	void pInitCascadeUpdateCycle();
	void pInitUBOClearProbes();
	void pInvalidateAllRayCaches();
	void pFindContent(const decDVector &position);
	void pTrackInstanceChanges();
	void pUpdateProbeOffsetFromShader(deoglGICascade &cascade);
	void pUpdateProbeExtendsFromShader(deoglGICascade &cascade);
	void pPrepareTraceProbes(deoglGICascade &cascade, const deoglDCollisionFrustum &frustum);
	void pFindProbesToUpdate(deoglGICascade &cascade, const deoglDCollisionFrustum &frustum);
	void pPrepareRayCacheProbes(deoglGICascade &cascade);
	void pPrepareProbeTexturesAndFBO();
	void pPrepareProbeVBO();
	void pPrepareUBORayDirections() const;
};

#endif
