/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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
	deoalRayCache( const decVector &minExtend, const decVector &maxExtend );
	
	/** \brief Clean up model ray cache. */
	~deoalRayCache();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Find matching ray. */
	deoalRayCacheRay *FindRay( const decVector &origin, const decVector &direction );
	
	/** \brief Cache ray if not present or longer than present cached ray. */
	void CacheRay( const decVector &origin, const decVector &direction, float length,
		const deoalRayTraceHitElementList &elements );
	
	/** \brief Cache ray if not present or longer than present cached ray. */
	void CacheRay( const decVector &origin, const decVector &direction, float length,
		const deoalRayTraceResult &result );
	
	/** \brief Cache ray if not present or longer than present cached ray. */
	void CacheRay( const decVector &origin, const decVector &direction, float length,
		const deoalRayTraceResult &result, int elementCount );
	
	
	
	/** \brief Debug. */
	void DebugPrint( deoalAudioThread &audioThread, const char *source );
	/*@}*/
	
	
	
private:
	void pLockRead();
	void pUnlockRead();
	deoalRayCacheRay *pFindRay( const decVector &origin, const decVector &direction ) const;
	void pCacheRay( deoalRayCacheRay *ray );
};

#endif
