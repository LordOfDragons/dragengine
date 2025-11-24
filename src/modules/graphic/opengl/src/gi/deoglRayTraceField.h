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

#ifndef _DEOGLRAYTRACEFIELD_H_
#define _DEOGLRAYTRACEFIELD_H_

#include "../framebuffer/deoglFramebuffer.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObjectReference.h>

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
	deoglFramebuffer::Ref pFBORays;
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
	inline const deoglFramebuffer::Ref &GetFBORays(){ return pFBORays; }
	
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
