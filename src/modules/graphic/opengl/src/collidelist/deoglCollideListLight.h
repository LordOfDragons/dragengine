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
	~deoglCollideListLight() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Clear collide list. */
	void Clear();
	
	/** Light. */
	inline deoglRLight *GetLight() const{ return pLight; }
	
	/** Set light. */
	void SetLight(deoglRLight *light);
	
	
	
	/** Light is culled. */
	inline bool GetCulled() const{ return pCulled; }
	
	/** Set light is culled. */
	void SetCulled(bool visible);
	
	/** Camera is inside light volume. */
	inline bool GetCameraInside() const{ return pCameraInside; }
	
	/** Camera is inside ccclusion query box. */
	inline bool GetCameraInsideOccQueryBox() const{ return pCameraInsideOccQueryBox; }
	
	/** Test if camera is inside light volume and occlusion query box. */
	void TestInside(const deoglRenderPlan &plan);
	
	/** Start occlusion test. */
	void StartOcclusionTest(deoglOcclusionTest &occlusionTest, const decDVector &cameraPosition);
	
	/** Occlusion test finished with a result of invisible for the element. */
	void OcclusionTestInvisible() override;
	
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
	void SetOcclusionQueryValid(bool valid);
	/*@}*/
};

#endif
