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

#ifndef _DEOALRAYCACHE_H_
#define _DEOALRAYCACHE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/threading/deMutex.h>


class deoalRayCacheRay;
class deoalRayTraceHitElementList;
class deoalRayTraceResult;
class deoalRayCacheOctree;
class deoalAudioThread;


/**
 * \brief Model ray cache.
 */
class deoalRayCache{
private:
	
	deMutex pMutexWrite;
	deMutex pMutexRead;
	int pReadCount;
	
	float pRange;
	float pSpread;
	float pSpreadDot;
	int pOctreeDepth;
	
	deoalRayCacheRay *pRootRay;
	deoalRayCacheRay *pTailRay;
	int pRayCount;
	
	deoalRayCacheOctree *pOctree;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model ray cache. */
	deoalRayCache(const decVector &minExtend, const decVector &maxExtend);
	
	/** \brief Clean up model ray cache. */
	~deoalRayCache();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Find matching ray. */
	deoalRayCacheRay *FindRay(const decVector &origin, const decVector &direction);
	
	/** \brief Cache ray if not present or longer than present cached ray. */
	void CacheRay(const decVector &origin, const decVector &direction, float length,
		const deoalRayTraceHitElementList &elements);
	
	/** \brief Cache ray if not present or longer than present cached ray. */
	void CacheRay(const decVector &origin, const decVector &direction, float length,
		const deoalRayTraceResult &result);
	
	/** \brief Cache ray if not present or longer than present cached ray. */
	void CacheRay(const decVector &origin, const decVector &direction, float length,
		const deoalRayTraceResult &result, int elementCount);
	
	
	
	/** \brief Debug. */
	void DebugPrint(deoalAudioThread &audioThread, const char *source);
	/*@}*/
	
	
	
private:
	void pLockRead();
	void pUnlockRead();
	deoalRayCacheRay *pFindRay(const decVector &origin, const decVector &direction) const;
	void pCacheRay(deoalRayCacheRay *ray);
};

#endif
