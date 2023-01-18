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

#ifndef _DEOGLRAYTRACEFIELD_H_
#define _DEOGLRAYTRACEFIELD_H_

#include "../texture/texture2d/deoglTexture.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObjectReference.h>

class deoglFramebuffer;
class deoglRenderThread;
class deoglROcclusionMesh;


/**
 * Ray trace field.
 * 
 * Stores a grid of ray trace probes. Each probe stores a set of ray traces all with the
 * same direction in the same order. This field can be used to speed up ray tracing for
 * global illumination calculations not requiring high precisionn.
 */
class deoglRayTraceField{
public:
	/** UBO Parameters. */
	enum eUBOParameter{
		eupProbeCount,
		eupProbesPerLine,
		eupProbeSize,
		eupProbeStride,
		eupResolution,
		eupClamp,
		eupSpacing,
		eupOrigin,
		eupSize
	};
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	decVector pOrigin;
	decVector pSize;
	decPoint3 pResolution;
	decPoint3 pCoordClamp;
	decVector pSpacing;
	decVector pInverseSpacing;
	int pProbeSize;
	int pProbesPerLine;
	int pProbeCount;
	
	deoglTexture pTexRays;
	deoglFramebuffer *pFBORays;
	deoglSPBlockUBO::Ref pUBO;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create ray trace field. */
	deoglRayTraceField( deoglRenderThread &renderThread );
	
	/** Clean up ray trace field. */
	~deoglRayTraceField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Field origin. */
	inline const decVector &GetOrigin() const{ return pOrigin; }
	
	/** Field size. */
	inline const decVector &GetSize() const{ return pSize; }
	
	/** Count of probes along each axis. */
	inline const decPoint3 &GetResolution() const{ return pResolution; }
	
	/** Coordinate clamping. */
	inline const decPoint3 &GetCoordClamp() const{ return pCoordClamp; }
	
	/** Spacing between probes in each direction. */
	inline const decVector &GetSpacing() const{ return pSpacing; }
	
	/** Size of single probe image. */
	inline int GetProbeSize() const{ return pProbeSize; }
	
	/** Count of probes per line. */
	inline int GetProbesPerLine() const{ return pProbesPerLine; }
	
	/**
	 * Init field parameters.
	 * 
	 * The method adds a border around the object of roughly 1m size.
	 * This border ensures tracing picks up valid resoluts from outside the object.
	 */
	void Init( const decVector &minExtend, const decVector &maxExtend );
	
	
	
	/** Probe coordinate closest to position. */
	decPoint3 Position2Coord( const decVector &position ) const;
	
	/** Position closest to probe coordinate. */
	decVector Coord2Position( const decPoint3 &coord ) const;
	
	
	
	/** Ray texture. */
	inline deoglTexture &GetTextureRays(){ return pTexRays; }
	inline const deoglTexture &GetTextureRays() const{ return pTexRays; }
	
	/** Ray fbo or NULL. */
	inline deoglFramebuffer *GetFBORays(){ return pFBORays; }
	
	/** UBO. */
	inline deoglSPBlockUBO &GetUBO() const{ return pUBO; }
	
	
	
	/** Render field. */
	void RenderField( deoglROcclusionMesh &occlusionMesh );
	
	/** Drop rays FBO. */
	void DropFBO();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pPrepareUBOState();
	void pPrepareRayTexFBO();
};

#endif
