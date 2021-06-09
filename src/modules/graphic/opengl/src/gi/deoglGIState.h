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

#include "deoglGIRayCache.h"
#include "deoglGIInstances.h"
#include "deoglGIAreaTracker.h"
#include "../collidelist/deoglCollideList.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/texture2d/deoglTexture.h"

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
		epfDisabled = 0x2,
		epfNearGeometry = 0x4,
		epfValid = 0x8, //<! Probe has been updated at least once
		epfRayCacheValid = 0x10, //<! Ray-Tracing ray cache is valid
		epfInsideView = 0x20,
		epfDynamicDisable = 0x40
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
	
	decVector pSize;
	int pCascadeCount;
	deoglRWorld *pWorld;
	decLayerMask pLayerMask;
	
	decVector pProbeSpacing;
	decVector pProbeSpacingInv;
	decPoint3 pProbeCount;
	decPoint3 pGridCoordClamp;
	decVector pFieldSize;
	decVector pFieldOrigin;
	decVector pPositionClamp;
	decVector pDynamicHalfEnlarge;
	int pStrideProbeCount;
	int pRealProbeCount;
	
	float pMaxDetectionRange;
	decVector pDetectionBox;
	deoglGIAreaTracker pAreaTracker;
	
	int pIrradianceMapSize;
	int pDistanceMapSize;
	float pMaxProbeDistance;
	float pDepthSharpness;
	float pHysteresis;
	float pNormalBias;
	float pEnergyPreservation;
	float pIrradianceGamma;
	float pSelfShadowBias;
	
	int pSizeTexIrradiance;
	int pSizeTexDistance;
	decVector2 pIrradianceMapScale;
	decVector2 pDistanceMapScale;
	
	decDVector pPosition;
	decDVector pLastRefPosition;
	sProbe *pProbes;
	decPoint pSampleImageSize;
	decPoint3 pGridCoordShift;
	
	int pActiveCascade;
	uint16_t *pAgedProbes;
	uint16_t *pUpdateProbes;
	int pUpdateProbeCount;
	uint16_t *pRayCacheProbes;
	int pRayCacheProbeCount;
	
	uint32_t *pClearProbes;
	int pClearProbeCount;
	bool pHasClearProbes;
	deObjectReference pUBOClearProbes;
	
	deoglArrayTexture pTexProbeIrradiance;
	deoglArrayTexture pTexProbeDistance;
	deoglArrayTexture pTexProbeOffset;
	deoglTexture pTexProbeState;
	deoglFramebuffer pFBOProbeIrradiance;
	deoglFramebuffer pFBOProbeDistance;
	deoglFramebuffer pFBOProbeOffset;
	deoglFramebuffer pFBOProbeState;
	bool pClearMaps;
	GLuint pVBOProbeOffsets;
	GLfloat *pVBOProbeOffsetsData;
	bool pProbesHaveMoved;
	
	GLuint pVBOProbeExtends;
	GLfloat *pVBOProbeExtendsData;
	bool pProbesExtendsChanged;
	
	deoglGIInstances pInstances;
	deoglGIRayCache pRayCache;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination state. */
	deoglGIState( deoglRenderThread &renderThread, const decVector &size, int cascadeCount );
	
	/** Clean up global illumination state. */
	~deoglGIState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Size of GI state. */
	inline const decVector &GetSize() const{ return pSize; }
	
	/** World. */
	inline deoglRWorld *GetWorld() const{ return pWorld; }
	
	/** Set world. */
	void SetWorld( deoglRWorld *world );
	
	/** Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set layer mask. Invalidates all if changed. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** Set size of GI state invalidating certain volumes and probes. */
// 	void SetSize( const decVector &size );
	
	/** Cascade count. */
	inline int GetCascadeCount() const{ return pCascadeCount; }
	
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
	
	/** Irradiance gamma. */
	inline float GetIrradianceGamma() const{ return pIrradianceGamma; }
	
	/** Self shadow bias. */
	inline float GetSelfShadowBias() const{ return pSelfShadowBias; }
	
	/** Calculate UBO self shadow bias value. */
	float CalcUBOSelfShadowBias() const;
	
	/** Count of probes to update. */
	inline int GetUpdateProbeCount() const{ return pUpdateProbeCount; }
	
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
	inline deoglArrayTexture &GetTextureProbeIrradiance(){ return pTexProbeIrradiance; }
	inline const deoglArrayTexture &GetTextureProbeIrradiance() const{ return pTexProbeIrradiance; }
	
	/** Distance probe texture. */
	inline deoglArrayTexture &GetTextureProbeDistance(){ return pTexProbeDistance; }
	inline const deoglArrayTexture &GetTextureProbeDistance() const{ return pTexProbeDistance; }
	
	/** Probe offset texture. */
	inline deoglArrayTexture &GetTextureProbeOffset(){ return pTexProbeOffset; }
	inline const deoglArrayTexture &GetTextureProbeOffset() const{ return pTexProbeOffset; }
	
	/** Probe state texture. */
	inline deoglTexture &GetTextureProbeState(){ return pTexProbeState; }
	inline const deoglTexture &GetTextureProbeState() const{ return pTexProbeState; }
	
	/** Probe fbo irradiance. */
	inline deoglFramebuffer &GetFBOProbeIrradiance(){ return pFBOProbeIrradiance; }
	
	/** Probe fbo distance. */
	inline deoglFramebuffer &GetFBOProbeDistance(){ return pFBOProbeDistance; }
	
	/** Probe offset fbo. */
	inline deoglFramebuffer &GetFBOProbeOffset(){ return pFBOProbeOffset; }
	
	/** Probe state fbo. */
	inline deoglFramebuffer &GetFBOProbeState(){ return pFBOProbeState; }
	
	/** Probe offset feedback VBO. */
	inline GLuint GetVBOProbeOffsets() const{ return pVBOProbeOffsets; }
	
	/** Probe extends feedback VBO. */
	inline GLuint GetVBOProbeExtends() const{ return pVBOProbeExtends; }
	
	/** Update. */
	void Update( const decDVector &cameraPosition, const deoglDCollisionFrustum &frustum );
	
	/** Prepare UBO state. */
	void PrepareUBOState() const;
	
	/** Prepare UBO state for ray cache rendering. */
	void PrepareUBOStateRayCache() const;
	
	/** Invalid all probes. */
	void Invalidate();
	
	/** Probe moved. */
	void ProbesMoved();
	
	/** Update probe offsets from probe offset shader result. */
	void UpdateProbeOffsetFromShader();
	
	/** Update probe extends from probe extends shader result. */
	void UpdateProbeExtendsFromShader();
	
	/** Invalidate area. */
	void InvalidateArea( const decDVector &minExtend, const decDVector &maxExtend );
	
	/** Dynamic area count increment. */
	void TouchDynamicArea( const decDVector &minExtend, const decDVector &maxExtend );
	
	/** Validate ray caches marked for update. */
	void ValidatedRayCaches();
	
	/** Notification component entered world. */
	void ComponentEnteredWorld( deoglRComponent *component );
	
	/** Notification component changed layer mask. */
	void ComponentChangedLayerMask( deoglRComponent *component );
	
	
	
	/** Instances. */
	inline deoglGIInstances &GetInstances(){ return pInstances; }
	inline const deoglGIInstances &GetInstances() const{ return pInstances; }
	
	/** Rays. */
	inline deoglGIRayCache &GetRayCache(){ return pRayCache; }
	inline const deoglGIRayCache &GetRayCache() const{ return pRayCache; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pInitProbes();
	void pInitUBOClearProbes();
	void pInvalidateAllRayCaches();
	void pFindContent();
	void pTrackInstanceChanges();
	void pUpdatePosition( const decDVector &position );
	void pPrepareTraceProbes( const deoglDCollisionFrustum &frustum );
	void pFindProbesToUpdate( const deoglDCollisionFrustum &frustum );
	void pAddUpdateProbes( uint8_t mask, uint8_t flags, int &lastIndex,
		int &remainingMatchCount, int maxUpdateCount );
	void pPrepareRayCacheProbes();
	void pPrepareProbeTexturesAndFBO();
	void pPrepareProbeVBO();
	void pPrepareUBOParameters( int probeCount ) const;
	void pPrepareUBORayDirections() const;
};

#endif
