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

#ifndef _DEOALRAYCACHEOCTREE_H_
#define _DEOALRAYCACHEOCTREE_H_

#include "../octree/deoalOctree.h"

#include <dragengine/common/collection/decPointerList.h>

class deoalRayCacheRay;



/**
 * \brief OpenAL world octree.
 */
class deoalRayCacheOctree : public deoalOctree{
private:
	decPointerList pRays;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create world octree. */
	deoalRayCacheOctree(const decVector &center, const decVector &halfSize);
	
	/** \brief Clean up world octree. */
	virtual ~deoalRayCacheOctree();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create octree for octant. */
	virtual deoalOctree *CreateOctree(int octant) const;
	
	/** \brief Clear content of node. */
	virtual void ClearNodeContent();
	
	/** \brief Clear all rays from tree. */
	void ClearRays();
	
	/** \brief Add ray into octree. */
	void InsertRayIntoTree(deoalRayCacheRay *ray, int maxDepth);
	/*@}*/
	
	
	
	/** \name Rays */
	/*@{*/
	/** \brief Number of rays. */
	int GetRayCount() const;
	
	/** \brief Ray at index. */
	deoalRayCacheRay *GetRayAt(int index) const;
	
	/** \brief Index of ray or -1 if absent. */
	int IndexOfRay(deoalRayCacheRay *ray) const;
	
	/** \brief Add ray. */
	void AddRay(deoalRayCacheRay *ray);
	
	/** \brief Remove ray. */
	void RemoveRay(deoalRayCacheRay *ray);
	
	/** \brief Remove all rays. */
	void RemoveAllRays();
	/*@}*/
	
	
	
private:
	deoalRayCacheOctree *pGetNodeFor(const decVector &position, int maxDepth);
};

#endif
