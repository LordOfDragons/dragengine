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

#ifndef _DEOGLGISTATE_H_
#define _DEOGLGISTATE_H_

#include "deoglGIRays.h"
#include "deoglGIInstances.h"
#include "../collidelist/deoglCollideList.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>

class deoglRenderThread;
class deoglRComponent;
class deoglRWorld;
class deoglSPBlockUBO;
class deoglDCollisionFrustum;


/**
 * Global illumination state.
 * 
 * Stores the state of global illumination. Typically every camera owns one state.
 */
class deoglGIState{
public:
	/** Probe flags. */
	enum eProbeFlags{
		epfSmoothUpdate = 0x1,
		epfValid = 0x2, //<! Probe has been updated at least once
		epfRayLimitsValid = 0x4, //<! Ray-Tracing distance limits are valid
		epfRayCacheValid = 0x8, //<! Ray-Tracing ray cache is valid
		epfInsideView = 0x10
	};
	
	/** Probe parameters. */
	struct sProbe{
		decPoint3 coord; //<! Grid coordinates
		decPoint3 shiftedCoord;
		decVector position;
		decVector offset;
		decVector minExtend;
		decVector maxExtend;
		uint16_t index; //<! Grid index
		uint8_t flags;
		uint8_t countOffsetMoved;
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	decVector pProbeSpacing;
	decVector pProbeSpacingInv;
	decPoint3 pProbeCount;
	decPoint3 pGridCoordClamp;
	decVector pFieldSize;
	decVector pFieldOrigin;
	decVector pPositionClamp;
	int pStrideProbeCount;
	int pRealProbeCount;
	
	float pMaxDetectionRange;
	decVector pDetectionBox;
	deoglCollideList pCollideList;
	deoglCollideList pCollideListFiltered;
	
	int pIrradianceMapSize;
	int pDistanceMapSize;
	float pMaxProbeDistance;
	float pDepthSharpness;
	float pHysteresis;
	float pNormalBias;
	float pEnergyPreservation;
	
	int pSizeTexIrradiance;
	int pSizeTexDistance;
	decVector2 pIrradianceMapScale;
	decVector2 pDistanceMapScale;
	
	decDVector pPosition;
	sProbe *pProbes;
	decPoint pSampleImageSize;
	decPoint3 pGridCoordShift;
	
	uint16_t *pAgedProbes;
	uint16_t *pUpdateProbes;
	int pUpdateProbeCount;
	uint16_t *pRayLimitProbes;
	int pRayLimitProbeCount;
	uint16_t *pRayCacheProbes;
	int pRayCacheProbeCount;
	
	uint16_t *pClearProbes;
	int pClearProbeCount;
	bool pHasClearProbes;
	deObjectReference pUBOClearProbes;
	
	deoglTexture pTexProbeIrradiance;
	deoglTexture pTexProbeDistance;
	deoglTexture pTexProbeOffset;
	deoglFramebuffer pFBOProbeIrradiance;
	deoglFramebuffer pFBOProbeDistance;
	deoglFramebuffer pFBOProbeOffset;
	deoglPixelBuffer *pPixBufProbeOffset;
	bool pClearMaps;
	bool pProbesHaveMoved;
	
	GLuint pVBOProbeExtends;
	GLfloat *pVBOProbeExtendsData;
	bool pProbesExtendsChanged;
	
	deoglGIInstances pInstances;
	deoglGIRays pRays;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination state. */
	deoglGIState( deoglRenderThread &renderThread );
	
	/** Clean up global illumination state. */
	~deoglGIState();
	/*@}*/
	
	
	
	/** @name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Probe spacing. */
	inline const decVector &GetProbeSpacing() const{ return pProbeSpacing; }
	
	/** Probe spacing inverse. */
	inline const decVector &GetProbeSpacingInverse() const{ return pProbeSpacingInv; }
	
	/** Probe count. */
	inline const decPoint3 &GetProbeCount() const{ return pProbeCount; }
	
	/** Grid coord clamp. */
	inline const decPoint3 &GetGridCoordClamp() const{ return pGridCoordClamp; }
	
	/** Probe field origin. */
	inline const decVector &GetFieldOrigin() const{ return pFieldOrigin; }
	
	/** Size of probe field. */
	inline const decVector &GetFieldSize() const{ return pFieldSize; }
	
	/** Position clamp. */
	inline const decVector &GetPositionClamp() const{ return pPositionClamp; }
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Size of sample image. */
	inline const decPoint &GetSampleImageSize() const{ return pSampleImageSize; }
	
	/**
	 * Grid coordinate shift (wrapping around).
	 * 
	 * Shift is in the range from 0 to probeCount-1.
	 */
	inline const decPoint3 &GetGridCoordShift() const{ return pGridCoordShift; }
	
	
	
	/** Detection box. */
	inline const decVector &GetDetectionBox() const{ return pDetectionBox; }
	
	/** Collide list. */
	inline deoglCollideList &GetCollideList(){ return pCollideList; }
	inline const deoglCollideList &GetCollideList() const{ return pCollideList; }
	
	/** Filtered collide list. */
	inline const deoglCollideList &GetCollideListFiltered() const{ return pCollideListFiltered; }
	
	/** Find content affecting GI state. */
	void FindContent( deoglRWorld &world );
	
	/** Filter occlusion meshes into filtered collide list. */
	void FilterOcclusionMeshes();
	
	/** Filter components into filtered collide list. */
	void FilterComponents();
	
	
	
	/** Grid coordinates for probe index. */
	decPoint3 ProbeIndex2GridCoord( int index ) const;
	
	/** Probe index for grid coordinates. */
	int GridCoord2ProbeIndex( const decPoint3 &coord ) const;
	
	/** Probe index for grid coordinates. */
	decVector Grid2Local( const decPoint3 &coord ) const;
	
	
	
	/** Grid coordinates closest to world position unclamped. */
	decPoint3 World2Grid( const decDVector &position ) const;
	
	/** World position closest to grid. */
	decDVector Grid2World( const decPoint3 &grid ) const;
	
	/** World coordinate of closest grid location. */
	decDVector WorldClosestGrid( const decDVector &position ) const;
	
	/** Local grid coordinates to shifted grid coordinates. */
	decPoint3 LocalGrid2ShiftedGrid( const decPoint3 &coord ) const;
	
	/** Shifted grid coordinates to local grid coordinates. */
	decPoint3 ShiftedGrid2LocalGrid( const decPoint3 &coord ) const;
	
	
	
	/** Irradiance map size. */
	inline int GetIrradianceMapSize() const{ return pIrradianceMapSize; }
	
	/** Distance map size. */
	inline int GetDistanceMapSize() const{ return pDistanceMapSize; }
	
	/** Irradiance map scale. */
	inline const decVector2 &GetIrradianceMapScale() const{ return pIrradianceMapScale; }
	
	/** Distance map scale. */
	inline const decVector2 &GetDistanceMapScale() const{ return pDistanceMapScale; }
	
	/** Normal bias. */
	inline float GetNormalBias() const{ return pNormalBias; }
	
	/** Energy preservation. */
	inline float GetEnergyPreservation() const{ return pEnergyPreservation; }
	
	/** Count of probes to update. */
	inline int GetUpdateProbeCount() const{ return pUpdateProbeCount; }
	
	/** Count of probes to ray limit update. */
	inline int GetRayLimitProbeCount() const{ return pRayLimitProbeCount; }
	
	/** Count of probes to ray cache update. */
	inline int GetRayCacheProbeCount() const{ return pRayCacheProbeCount; }
	
	
	
	/** Clear clear probes. */
	void ClearClearProbes();
	
	/** Has clear probes. */
	inline bool HasClearProbes() const{ return pHasClearProbes; }
	
	/** Clear probes UBO. */
	inline deoglSPBlockUBO &GetUBOClearProbes() const{ return ( deoglSPBlockUBO& )( deObject& )pUBOClearProbes; }
	
	/** Prepare clear probes UBO. */
	void PrepareUBOClearProbes() const;
	
	
	
	/** Irradiance probe texture. */
	inline deoglTexture &GetTextureProbeIrradiance(){ return pTexProbeIrradiance; }
	inline const deoglTexture &GetTextureProbeIrradiance() const{ return pTexProbeIrradiance; }
	
	/** Distance probe texture. */
	inline deoglTexture &GetTextureProbeDistance(){ return pTexProbeDistance; }
	inline const deoglTexture &GetTextureProbeDistance() const{ return pTexProbeDistance; }
	
	/** Probe offset texture. */
	inline deoglTexture &GetTextureProbeOffset(){ return pTexProbeOffset; }
	inline const deoglTexture &GetTextureProbeOffset() const{ return pTexProbeOffset; }
	
	/** Probe fbo irradiance. */
	inline deoglFramebuffer &GetFBOProbeIrradiance(){ return pFBOProbeIrradiance; }
	
	/** Probe fbo distance. */
	inline deoglFramebuffer &GetFBOProbeDistance(){ return pFBOProbeDistance; }
	
	/** Probe offset fbo. */
	inline deoglFramebuffer &GetFBOProbeOffset(){ return pFBOProbeOffset; }
	
	/** Probe extends feedback VBO. */
	inline GLuint GetVBOProbeExtends() const{ return pVBOProbeExtends; }
	
	/** Update. */
	void Update( deoglRWorld &world, const decDVector &cameraPosition,
		const deoglDCollisionFrustum &frustum );
	
	/** Prepare UBO state. */
	void PrepareUBOState() const;
	
	/** Prepare UBO state for ray limit rendering. */
	void PrepareUBOStateRayLimit() const;
	
	/** Prepare UBO state for ray cache rendering. */
	void PrepareUBOStateRayCache() const;
	
	/** Invalid all probes. */
	void Invalidate();
	
	/** Probe moved. */
	void ProbesMoved();
	
	/** Update probe offsets from probe offset texture. */
	void UpdateProbeOffsetFromTexture();
	
	/** Update probe extends from probe extends VBO. */
	void UpdateProbeExtendsFromVBO();
	
	/** Invalidate area. */
	void InvalidateArea( const decDVector &minExtend, const decDVector &maxExtend );
	
	/** Validate ray caches marked for update. */
	void ValidatedRayCaches();
	
	
	
	/** Instances. */
	inline deoglGIInstances &GetInstances(){ return pInstances; }
	inline const deoglGIInstances &GetInstances() const{ return pInstances; }
	
	/** Rays. */
	inline deoglGIRays &GetRays(){ return pRays; }
	inline const deoglGIRays &GetRays() const{ return pRays; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pInitProbes();
	void pInitUBOClearProbes();
	void pInvalidateAllRayLimits();
	void pInvalidateAllRayCaches();
	void pTrackInstanceChanges();
	void pSyncTrackedInstances();
	void pUpdatePosition( const decDVector &position );
	void pPrepareTraceProbes( const deoglDCollisionFrustum &frustum );
	void pFindProbesToUpdate( const deoglDCollisionFrustum &frustum );
	void pAddUpdateProbe( sProbe &probe );
	void pPrepareRayLimitProbes();
	void pPrepareRayCacheProbes();
	void pPrepareProbeTexturesAndFBO();
	void pPrepareProbeVBO();
	void pPrepareUBOParameters( int probeCount ) const;
	void pPrepareUBORayDirections() const;
};

#endif
