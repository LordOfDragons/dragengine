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
class deoglRComponentLOD;
class deoglOcclusionTest;



/**
 * Component in collide list.
 */
class deoglCollideListComponent : public deoglOcclusionTestListener{
private:
	deoglRComponent *pComponent;
	int pLODLevel;
	deoglRComponentLOD *pComponentLOD;
	bool pCulled;
	int pCascadeMask;
	int pCubeFaceMask;
	int pSpecialFlags;
	
	
	
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
	
	/**
	 * Set component.
	 * \note For use by deoglCollideList only.
	 */
	void SetComponent( deoglRComponent *component );
	
	/** LOD level to use. */
	inline int GetLODLevel() const{ return pLODLevel; }
	
	/** Set LOD level to use. */
	void SetLODLevel( int lodLevel );
	
	/** Set LOD level to use to maximum LOD level. */
	void SetLODLevelMax();
	
	/** Component LOD. */
	inline deoglRComponentLOD &GetComponentLOD() const{ return *pComponentLOD; }
	
	
	
	/** Component is culled. */
	inline bool GetCulled() const{ return pCulled; }
	
	/** Set component is culled. */
	void SetCulled( bool culled );
	
	/** Cascade mask. */
	inline int GetCascadeMask() const{ return pCascadeMask; }
	
	/** Set cascade mask. */
	void SetCascadeMask( int mask );
	
	/**
	 * Calculate for each cube map face if component is potentially visible. Optimizes rendering
	 * cube maps by not rendering components on cube map faces where they are not visible
	 */
	void UpdateCubeFaceMask( const decDVector &cubePosition );
	
	/** Component cube face mask. */
	inline int GetCubeFaceMask() const{ return pCubeFaceMask; }
	
	/** Component is visible on cube map face. Valid after calling UpdateCubeFaceVisibility() */
	bool GetCubeFaceMaskAt( int face ) const;
	
	/** Special flags. */
	inline int GetSpecialFlags() const{ return pSpecialFlags; }
	
	/** Set special flags. */
	void SetSpecialFlags( int flags );
	
	/** Start occlusion test. */
	void StartOcclusionTest( deoglOcclusionTest &occlusionTest, const decDVector &referencePosition );
	
	/** Occlusion test finished with a result of invisible for the element. */
	virtual void OcclusionTestInvisible();
	/*@}*/
};

#endif
