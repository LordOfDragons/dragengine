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

#ifndef _DEOALWOVPREPARERAYTRACE_H_
#define _DEOALWOVPREPARERAYTRACE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/collection/decPointerList.h>

#include "deoalWorldOctreeVisitor.h"

class deoalAWorld;
class deoalRTWorldBVH;


/**
 * \brief World octree visitor prepare for ray-tracing.
 * 
 * This is a two stage visitor due to deoalAComponent potentially altering the extends and thus
 * the octree node. This potential altering of the extends is allowed to avoid transforming
 * components outside the audible range which improves performance. Allowing this forces us
 * though to first collect all affected components and then to prepare them after visiting.
 * This way changes to the octree doe not affect the ongoing visiting. This also means the
 * visitor should be allocated once and then reused to avoid allocating the array each time.
 */
class deoalWOVPrepareRayTrace : public deoalWorldOctreeVisitor{
private:
	decDVector pCenter;
	double pRadius;
	double pRadiusSquared;
	decLayerMask pLayerMask;
	deoalRTWorldBVH *pRTWorldBVH;
	
	decPointerList pComponents;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalWOVPrepareRayTrace();
	
	/** \brief Clean up visitor. */
	virtual ~deoalWOVPrepareRayTrace();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Center. */
	inline const decDVector &GetCenter() const{ return pCenter; }
	
	/** \brief Set center. */
	void SetCenter(const decDVector &center);
	
	/** \brief Radius. */
	inline double GetRadius() const{ return pRadius; }
	
	/** \brief Set radius. */
	void SetRadius(double radius);
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	/** \brief Ray trace world BVH or NULL. */
	inline deoalRTWorldBVH *GetRTWorldBVH() const{ return pRTWorldBVH; }
	
	/** \brief Set ray trace world bch or NULL. */
	void SetRTWorldBVH(deoalRTWorldBVH *bvh);
	
	
	
	/**
	 * \brief Visit world with visitor.
	 * 
	 * Always use this method instead of doing visiting on your own. Clears first the array
	 * of components, then visits the world and then prepare the collected components.
	 */
	void Visit(deoalAWorld &world);
	
	
	
	/** \brief Visits components affecting sound hit by ray. */
	virtual void VisitNode(deoalDOctree *node, int intersection);
	/*@}*/
	
	
	
private:
	void pPrepareRTWorldBVH(deoalRTWorldBVH &bvh);
};

#endif
