/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLCOLLIDELISTHTSECTOR_H_
#define _DEOGLCOLLIDELISTHTSECTOR_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

class deoglHTViewSector;
class deoglOcclusionTest;
class deoglCollideListHTSCluster;


/**
 * Collide List Height Terrain Sector.
 */
class deoglCollideListHTSector{
private:
	deoglHTViewSector *pSector;
	
	decPointerList pClusters;
	int pClusterCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create height terrain sector. */
	deoglCollideListHTSector();
	
	/** Clean up height terrain sector. */
	~deoglCollideListHTSector();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear sector. */
	void Clear();
	
	/** Start occlusion test. */
	void StartOcclusionTest( deoglOcclusionTest &occlusionTest, const decDVector &referencePosition );
	
	
	
	/** Sector. */
	inline deoglHTViewSector *GetSector() const{ return pSector; }
	
	/** Set sector. */
	void SetSector( deoglHTViewSector *sector );
	
	/** Count of clusters. */
	inline int GetClusterCount() const{ return pClusterCount; }
	
	/** Cluster at index. */
	deoglCollideListHTSCluster &GetClusterAt( int index ) const;
	
	/** Add cluster. */
	deoglCollideListHTSCluster *AddCluster( const decPoint &coordinates );
	
	/** Remove all clusters. */
	void RemoveAllClusters();
	
	/** Remove culled clusters. */
	void RemoveCulledClusters();
	/*@}*/
};

#endif
