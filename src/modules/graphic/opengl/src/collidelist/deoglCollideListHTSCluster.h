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

#ifndef _DEOGLCOLLIDELISTHTSCLUSTER_H_
#define _DEOGLCOLLIDELISTHTSCLUSTER_H_

#include "../occlusiontest/deoglOcclusionTestListener.h"

#include <dragengine/common/math/decMath.h>

class deoglCollideListHTSector;
class deoglOcclusionTest;


/**
 * Collide List Height Terrain Sector.
 */
class deoglCollideListHTSCluster : public deoglOcclusionTestListener{
private:
	deoglCollideListHTSector &pSector;
	decPoint pCoordinates;
	int pIndex;
	
	bool pCulled;
	int pCascadeMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create cluster. */
	deoglCollideListHTSCluster( deoglCollideListHTSector &sector );
	
	/** Clean up cluster. */
	~deoglCollideListHTSCluster();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear. */
	void Clear();
	
	/** Cluster coordinates. */
	inline const decPoint &GetCoordinates() const{ return pCoordinates; }
	
	/** Set cluster coordinates. */
	void SetCoordinates( const decPoint &coordinates );
	
	/** Cluster index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** Component is culled. */
	inline bool GetCulled() const{ return pCulled; }
	
	/** Set component is culled. */
	void SetCulled( bool culled );
	
	/** Cascade mask. */
	inline int GetCascadeMask() const{ return pCascadeMask; }
	
	/** Set cascade mask. */
	void SetCascadeMask( int mask );
	
	/** Start occlusion test. */
	void StartOcclusionTest( deoglOcclusionTest &occlusionTest, const decVector &offset );
	
	/** Occlusion test finished with a result of invisible for the element. */
	virtual void OcclusionTestInvisible();
	/*@}*/
};

#endif
