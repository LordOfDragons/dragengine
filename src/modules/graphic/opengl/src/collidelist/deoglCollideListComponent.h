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
