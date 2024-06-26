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

#ifndef _DEOGLGICASCADE_H_
#define _DEOGLGICASCADE_H_

#include <stdint.h>
#include <dragengine/common/math/decMath.h>

class deoglGIState;
class deoglSPBlockUBO;
class deoglDCollisionFrustum;
class deoglGIBVH;


/**
 * Global illumination cascade.
 */
class deoglGICascade{
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
	deoglGIState &pGIState;
	const int pIndex;
	
	decVector pOffset;
	decVector pProbeSpacing;
	decVector pProbeSpacingInv;
	decVector pFieldSize;
	decVector pFieldOrigin;
	decVector pPositionClamp;
	decVector pDynamicHalfEnlarge;
	decVector pStaticHalfEnlarge;
	bool pFillUpUpdatesWithExpensiveProbes;
	int pSkyShadowCascade;
	
	float pMaxDetectionRange;
	decVector pDetectionBox;
	
	float pMaxProbeDistance;
	
	decDVector pPosition;
	decDVector pLastRefPosition;
	decPoint3 pGridCoordShift;
	
	sProbe *pProbes;
	uint16_t *pAgedProbes;
	bool pHasInvalidProbesInsideView;
	bool pRequiresFullUpdateInsideView;
	
	uint32_t *pClearProbes;
	int pClearProbeCount;
	bool pHasClearProbes;
	
	uint16_t *pUpdateProbes;
	int pUpdateProbeCount;
	
	uint16_t *pRayCacheProbes;
	int pRayCacheProbeCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination cascade. */
	deoglGICascade( deoglGIState &giState, int index, const decVector &probeSpacing,
		const decVector &offset );
	
	/** Clean up global illumination cascade. */
	~deoglGICascade();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** GI State. */
	inline deoglGIState &GetGIState() const{ return pGIState; }
	
	/** Cascade index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Offset. */
	inline const decVector &GetOffset() const{ return pOffset; }
	
	/** Probe spacing. */
	inline const decVector &GetProbeSpacing() const{ return pProbeSpacing; }
	
	/** Probe spacing inverse. */
	inline const decVector &GetProbeSpacingInverse() const{ return pProbeSpacingInv; }
	
	/** Probe field origin. */
	inline const decVector &GetFieldOrigin() const{ return pFieldOrigin; }
	
	/** Size of probe field. */
	inline const decVector &GetFieldSize() const{ return pFieldSize; }
	
	/** Position clamp. */
	inline const decVector &GetPositionClamp() const{ return pPositionClamp; }
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Grid coordinate shift (wrapping around). Shift is in the range from 0 to probeCount-1. */
	inline const decPoint3 &GetGridCoordShift() const{ return pGridCoordShift; }
	
	/** Detection box. */
	inline const decVector &GetDetectionBox() const{ return pDetectionBox; }
	
	/** Max probe distance. */
	inline float GetMaxProbeDistance() const{ return pMaxProbeDistance; }
	
	/** Fill up update slots with expensive probes on large position changes. */
	inline bool GetFillUpUpdatesWithExpensiveProbes() const{ return pFillUpUpdatesWithExpensiveProbes; }
	
	/** Fill up update slots with expensive probes on large position changes. */
	void SetFillUpUpdatesWithExpensiveProbes( bool fillUp );
	
	/** Index of cascade to use for sky shadow casting. */
	inline int GetSkyShadowCascade() const{ return pSkyShadowCascade; }
	
	/** Set index of cascade to use for sky shadow casting. */
	void SetSkyShadowCascade( int cascade );
	
	
	
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
	
	
	
	/** Probe position. */
	decVector ProbePosition( int index ) const;
	
	
	
	/** Probes inside view with epfValid cleared are present. */
	inline bool HasInvalidProbesInsideView() const{ return pHasInvalidProbesInsideView; }
	
	/** Cascade requires full update of all inside view probes. */
	inline bool GetRequiresFullUpdateInsideView() const{ return pRequiresFullUpdateInsideView; }
	
	/** Set if cascade requires full update of all inside view probes. */
	void SetRequiresFullUpdateInsideView( bool requiresUpdate );
	
	
	
	/** Clear clear probes. */
	void ClearClearProbes();
	
	/** Has clear probes. */
	inline bool HasClearProbes() const{ return pHasClearProbes; }
	
	/** Prepare clear probes UBO. */
	void PrepareUBOClearProbes( deoglSPBlockUBO &ubo ) const;
	
	
	
	/** Count of probes to update. */
	inline int GetUpdateProbeCount() const{ return pUpdateProbeCount; }
	
	/** Index of probe if in list of update probes otherwise -1. */
	int IndexOfUpdateProbe( int probeIndex ) const;
	
	/** Count of probes to ray cache update. */
	inline int GetRayCacheProbeCount() const{ return pRayCacheProbeCount; }
	
	
	
	/** Invalid all probes. */
	void Invalidate();
	
	/** Invalidate area. */
	void InvalidateArea( const decDVector &minExtend, const decDVector &maxExtend, bool hard );
	
	/** Dynamic area changed. */
	void TouchDynamicArea( const decDVector &minExtend, const decDVector &maxExtend );
	
	/** Validate ray caches marked for update. */
	void ValidatedRayCaches();
	
	/** Invalidate all ray caches. */
	void InvalidateAllRayCaches();
	
	/** Update position. */
	void UpdatePosition( const decDVector &position );
	
	/** Find probes to update. */
	void FindProbesToUpdate( const deoglDCollisionFrustum &frustum );
	
	/** Prepare ray cache probes. */
	void PrepareRayCacheProbes();
	
	
	
	/** Calculate UBO self shadow bias value. */
	float CalcUBOSelfShadowBias() const;
	
	/** Update parameters UBO. */
	void UpdateUBOParameters( deoglSPBlockUBO &ubo, int probeCount, const deoglGIBVH &bvh ) const;
	
	/** Update probe positions UBO. */
	void UpdateUBOProbePosition( deoglSPBlockUBO &ubo ) const;
	
	/** Update probe indices UBO. */
	void UpdateUBOProbeIndices( deoglSPBlockUBO &ubo ) const;
	
	/** Update probe indices UBO from ray cache indices. */
	void UpdateUBOProbeIndicesRayCache( deoglSPBlockUBO &ubo ) const;
	
	/** Update probe positions UBO from ray cache indices. */
	void UpdateUBOProbePositionRayCache( deoglSPBlockUBO &ubo ) const;
	
	/** Update probe offsets from probe offset shader result. */
	void UpdateProbeOffsetFromShader( const char *data );
	
	/** Update probe extends from probe extend shader result. */
	void UpdateProbeExtendsFromShader( const char *data );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pInitProbes();
	void pFindProbesToUpdateFullUpdateInsideView();
	void pFindProbesToUpdateRegular();
	void pAddUpdateProbes( uint8_t mask, uint8_t flags, int &lastIndex,
		int &remainingMatchCount, int maxUpdateCount );
	void pUpdateUBOProbePosition( deoglSPBlockUBO &ubo, const uint16_t *indices, int count ) const;
	void pUpdateUBOProbeIndices( deoglSPBlockUBO &ubo, const uint16_t *indices, int count ) const;
	void pUpdateHasProbeFlags();
};

#endif
