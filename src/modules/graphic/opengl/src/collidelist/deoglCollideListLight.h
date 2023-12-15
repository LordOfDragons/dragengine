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

#ifndef _DEOGLCOLLIDELISTLIGHT_H_
#define _DEOGLCOLLIDELISTLIGHT_H_

#include "../occlusiontest/deoglOcclusionTestListener.h"

#include <dragengine/common/math/decMath.h>

class deoglRLight;
class deoglOcclusionTest;
class deoglRenderPlan;
class deoglOcclusionQuery;



/**
 * Light in collide list.
 */
class deoglCollideListLight : public deoglOcclusionTestListener{
private:
	deoglRLight *pLight;
	bool pCulled;
	bool pCameraInside;
	bool pCameraInsideOccQueryBox;
	
	deoglOcclusionQuery *pOcclusionQuery;
	bool pOccQueryValid;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create collide list light. */
	deoglCollideListLight();
	
	/** Clean up collide list light. */
	~deoglCollideListLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear collide list. */
	void Clear();
	
	/** Light. */
	inline deoglRLight *GetLight() const{ return pLight; }
	
	/** Set light. */
	void SetLight( deoglRLight *light );
	
	
	
	/** Light is culled. */
	inline bool GetCulled() const{ return pCulled; }
	
	/** Set light is culled. */
	void SetCulled( bool visible );
	
	/** Camera is inside light volume. */
	inline bool GetCameraInside() const{ return pCameraInside; }
	
	/** Camera is inside ccclusion query box. */
	inline bool GetCameraInsideOccQueryBox() const{ return pCameraInsideOccQueryBox; }
	
	/** Test if camera is inside light volume and occlusion query box. */
	void TestInside( const deoglRenderPlan &plan );
	
	/** Start occlusion test. */
	void StartOcclusionTest( deoglOcclusionTest &occlusionTest, const decDVector &cameraPosition );
	
	/** Occlusion test finished with a result of invisible for the element. */
	virtual void OcclusionTestInvisible();
	
	/** Has occlusion query. */
	inline bool HasOcclusionQuery() const{ return pOcclusionQuery != nullptr; }
	
	/** Occlusion query. */
	deoglOcclusionQuery &GetOcclusionQuery();
	
	/**
	 * Light is hidden according to the last time the occlusion query has been run.
	 * Deals with various special cases preventing the caller to know or care about them.
	 */
	bool IsHiddenByOccQuery() const;
	
	/** Set if occlusion query is valid. */
	void SetOcclusionQueryValid( bool valid );
	/*@}*/
};

#endif
