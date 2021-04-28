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

#ifndef _DEOGLCOLLIDELISTCOMPONENT_H_
#define _DEOGLCOLLIDELISTCOMPONENT_H_

#include "../occlusiontest/deoglOcclusionTestListener.h"

#include <dragengine/common/math/decMath.h>

class deoglRComponent;
class deoglOcclusionTest;



/**
 * Keeps track of a component in a collide list. Stores the component as well
 * as the used lod level. It is possible for the component to be NULL. This
 * allows the collide list to clear a colider list component not in use
 * to avoid costly allocations. The component object is only a pointer and is
 * not referenced. Care has to be taken to clear collide lists of a component
 * is no more existing.
 */
class deoglCollideListComponent : public deoglOcclusionTestListener{
private:
	deoglRComponent *pComponent;
	int pLODLevel;
	bool pCulled;
	int pCascadeMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create collide list component. */
	deoglCollideListComponent();
	
	/** Clean up collide list component. */
	~deoglCollideListComponent();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear collide list. */
	void Clear();
	
	/** Component. */
	inline deoglRComponent *GetComponent() const{ return pComponent; }
	
	/** Set component. */
	void SetComponent( deoglRComponent *component );
	
	/** LOD level to use. */
	inline int GetLODLevel() const{ return pLODLevel; }
	
	/** Set LOD level to use. */
	void SetLODLevel( int lodLevel );
	
	/** Set LOD level to use to maximum LOD level. */
	void SetLODLevelMax();
	
	
	
	/** Component is culled. */
	inline bool GetCulled() const{ return pCulled; }
	
	/** Set component is culled. */
	void SetCulled( bool visible );
	
	/** Cascade mask. */
	inline int GetCascadeMask() const{ return pCascadeMask; }
	
	/** Set cascade mask. */
	void SetCascadeMask( int mask );
	
	/** Start occlusion test. */
	void StartOcclusionTest( deoglOcclusionTest &occlusionTest, const decDVector &cameraPosition );
	
	/** Occlusion test finished with a result of invisible for the element. */
	virtual void OcclusionTestInvisible();
	/*@}*/
};

#endif
