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

#ifndef _DEOALRAYCACHERAY_H_
#define _DEOALRAYCACHERAY_H_

#include <dragengine/common/math/decMath.h>


class deoalRayCacheOctree;
class deoalRayCacheRayHit;
class deoalRayTraceHitElementList;
class deoalRayTraceResult;


/**
 * \brief Model ray cache ray.
 */
class deoalRayCacheRay{
	decVector pOrigin;
	decVector pDirection;
	float pLength;
	
	deoalRayCacheRayHit *pHits;
	int pHitCount;
	
	deoalRayCacheOctree *pOctreeNode;
	
	deoalRayCacheRay *pLLPrev;
	deoalRayCacheRay *pLLNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model ray cache ray. */
	deoalRayCacheRay(const decVector &origin, const decVector &direction,
		float length, const deoalRayTraceHitElementList &elements);
	
	/** \brief Create model ray cache ray. */
	deoalRayCacheRay(const decVector &origin, const decVector &direction,
		float length, const deoalRayTraceResult &result);
	
	/** \brief Create model ray cache ray. */
	deoalRayCacheRay(const decVector &origin, const decVector &direction,
		float length, const deoalRayTraceResult &result, int elementCount);
	
	/** \brief Clean up model ray cache ray. */
	~deoalRayCacheRay();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Origin. */
	inline const decVector &GetOrigin() const{return pOrigin;}
	
	/** \brief Normalized direction. */
	inline const decVector &GetDirection() const{return pDirection;}
	
	/** \brief Length. */
	inline float GetLength() const{return pLength;}
	
	/** \brief Number of hits. */
	inline int GetHitCount() const{return pHitCount;}
	
	/** \brief Hit at index. */
	const deoalRayCacheRayHit &GetHitAt(int index) const;
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/** \brief Octree node. */
	inline deoalRayCacheOctree *GetOctreeNode() const{return pOctreeNode;}
	
	/** \brief Set octree node. */
	void SetOctreeNode(deoalRayCacheOctree *node);
	
	/** \brief Previous ray in cache. */
	inline deoalRayCacheRay *GetLLPrev() const{return pLLPrev;}
	
	/** \brief Set previous ray in cache. */
	void SetLLPrev(deoalRayCacheRay *ray);
	
	/** \brief Next ray in cache. */
	inline deoalRayCacheRay *GetLLNext() const{return pLLNext;}
	
	/** \brief Set next ray in cache. */
	void SetLLNext(deoalRayCacheRay *ray);
	/*@}*/
};

#endif
