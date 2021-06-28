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

#ifndef _DEOGLCOLLIDELISTPROPFIELDCLUSTER_H_
#define _DEOGLCOLLIDELISTPROPFIELDCLUSTER_H_

#include "../occlusiontest/deoglOcclusionTestListener.h"

#include <dragengine/common/math/decMath.h>

class deoglCollideListPropFieldType;
class deoglOcclusionTest;
class deoglPropFieldCluster;


/**
 * Collide list prop field type cluster.
 */
class deoglCollideListPropFieldCluster : public deoglOcclusionTestListener{
private:
	deoglCollideListPropFieldType &pType;
	deoglPropFieldCluster *pCluster;
	
	bool pCulled;
	int pCascadeMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create cluster. */
	deoglCollideListPropFieldCluster( deoglCollideListPropFieldType &type );
	
	/** Clean up cluster. */
	~deoglCollideListPropFieldCluster();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear. */
	void Clear();
	
	/** Cluster. */
	inline deoglPropFieldCluster *GetCluster() const{ return pCluster; }
	
	/** Set cluster. */
	void SetCluster( deoglPropFieldCluster *cluster );
	
	
	
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
