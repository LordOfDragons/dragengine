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

#ifndef _DEOGLRENDERLIGHTBASE_H_
#define _DEOGLRENDERLIGHTBASE_H_

#include "../deoglRenderBase.h"
#include "../../collidelist/deoglCollideList.h"

class deoglComponentSet;
class deoglRenderPlan;
class deoglGIState;
class deoglRenderPlanLight;


/**
 * Base class for light renderer classes.
 */
class deoglRenderLightBase : public deoglRenderBase{
private:
	deoglCollideList pColList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderLightBase(deoglRenderThread &renderThread);
	
	/** Clean up renderer. */
	~deoglRenderLightBase() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Collider list. */
	inline deoglCollideList &GetCollideList(){ return pColList; }
	inline const deoglCollideList &GetCollideList() const{ return pColList; }
	
	/** Populate collider list with components from a component list. */
	void AddComponentsToColliderList(const deoglComponentSet &list);
	
	/** Transform color by matrix. */
	decColor TransformColor(const decMatrix &matrix, const decColor &color) const;
	
	/** Restore frame buffer configuration. */
	void RestoreFBO(deoglRenderPlan &plan);
	
	/** Restore texture bindings from the deferred rendering pass using linear interpolation. */
	void RestoreDRTexturesSmooth();
	
	/** Restore depth texture binding from the deferred rendering pass using linear interpolation. */
	void RestoreDRTextureDepthSmooth();
	
	/** Restore GI Rays frame buffer configuration. */
	void RestoreFBOGITraceRays(deoglGIState &giState);
	
	
	
	/** Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfoSolid();
	
	/** Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfoTransparent();
	/*@}*/
	
	
	
protected:
	int pPipelineModifiers(const deoglRenderPlanLight &planLight, bool solid, bool hasAmbient) const;
};

#endif
