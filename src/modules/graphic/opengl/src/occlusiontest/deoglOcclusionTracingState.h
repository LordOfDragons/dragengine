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

#ifndef _DEOGLOCCLUSIONTRACINGSTATE_H_
#define _DEOGLOCCLUSIONTRACINGSTATE_H_

#include "../framebuffer/deoglFramebuffer.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>

class deoglOcclusionTracing;
class deoglRWorld;


/**
 * \brief Occlusion tracing state.
 * 
 * Stores the state of one occlusion trace. Typically every camera owns one tracing state.
 */
class deoglOcclusionTracingState{
public:
	/** \brief Probe parameters. */
	struct sProbe{
		decPoint3 coord; //<! Grid coordinates
		decVector position; //<! Local position
		int index; //<! Grid index
		float blendFactor; //<! 1-hysteresis modified
		int age; //<! Age in update runs since probe has been last updated
		bool valid; //<! Probe has been updated at least once
	};
	
	
private:
	deoglOcclusionTracing &pTracing;
	
	decDVector pPosition;
	sProbe *pProbes;
	decPoint pSampleImageSize;
	
	deoglCollideList pCollideList;
	
	decTimer pTimerUpdateProbe;
	float pElapsedUpdateProbe;
	float pUpdateProbeInterval;
	sProbe **pUpdateProbes;
	int pUpdateProbeCount;
	
	deoglTexture pTexProbeOcclusion;
	deoglTexture pTexProbeDistance;
	deoglFramebuffer pFBOProbeOcclusion;
	deoglFramebuffer pFBOProbeDistance;
	bool pClearMaps;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create occlusion tracing state. */
	deoglOcclusionTracingState( deoglOcclusionTracing &tracing );
	
	/** \brief Clean up occlusion tracing state. */
	~deoglOcclusionTracingState();
	/*@}*/
	
	
	
	/** @name Management */
	/*@{*/
	/** \brief Tracing. */
	inline deoglOcclusionTracing &GetTracing() const{ return pTracing; }
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Size of sample image. */
	inline const decPoint &GetSampleImageSize() const{ return pSampleImageSize; }
	
	/** \brief World position closest to grid. */
	decDVector WorldClosestGrid( const decDVector &position ) const;
	
	/** \brief Count of probes to update. */
	inline int GetUpdateProbeCount() const{ return pUpdateProbeCount; }
	
	/** \brief Occlusion probe texture. */
	inline deoglTexture &GetTextureProbeOcclusion(){ return pTexProbeOcclusion; }
	inline const deoglTexture &GetTextureProbeOcclusion() const{ return pTexProbeOcclusion; }
	
	/** \brief Distance probe texture. */
	inline deoglTexture &GetTextureProbeDistance(){ return pTexProbeDistance; }
	inline const deoglTexture &GetTextureProbeDistance() const{ return pTexProbeDistance; }
	
	/** \brief Probe fbo occlusion. */
	inline deoglFramebuffer &GetFBOProbeOcclusion(){ return pFBOProbeOcclusion; }
	
	/** \brief Probe fbo distance. */
	inline deoglFramebuffer &GetFBOProbeDistance(){ return pFBOProbeDistance; }
	
	/** \brief Update. */
	void Update( deoglRWorld &world, const decDVector &position );
	
	/** \brief Invalid all probes. */
	void Invalidate();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pInitProbes();
	void pUpdatePosition( const decDVector &position );
	void pTraceProbes();
	void pAgeProbes();
	void pFindProbesToUpdate();
	void pPrepareUBOState();
	void pPrepareProbeTexturesAndFBO();
};

#endif
