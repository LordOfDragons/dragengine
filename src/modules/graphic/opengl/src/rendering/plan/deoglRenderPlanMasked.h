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

#ifndef _DEOGLRENDERPLANMASKED_H_
#define _DEOGLRENDERPLANMASKED_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderPlan;
class deoglRComponent;

/**
 * Stores a masked render plan. A masked render plan is used to render
 * views into an world render call using a mask to limit the touched
 * pixels. A render plan masked consists of a render plan itself as
 * well as information on how to obtain the mask.
 */
class deoglRenderPlanMasked{
private:
	deoglRenderPlan *pPlan;
	
	deoglRComponent *pComponent;
	int pComponentTexture;
	
	bool pUseClipPlane;
	decVector pClipNormal;
	float pClipDistance;
	decVector pClipNormalStereo;
	float pClipDistanceStereo;
	
	int pStencilMask;
	int pParentStencilMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create masked render plan. */
	deoglRenderPlanMasked();
	
	/** Clean up masked render plan. */
	~deoglRenderPlanMasked();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render plan. */
	inline deoglRenderPlan *GetPlan() const{ return pPlan; }
	
	/** Set render plan. */
	void SetPlan(deoglRenderPlan *plan);
	
	/** Component or nuullptr. */
	inline deoglRComponent *GetComponent() const{ return pComponent; }
	
	/** Component texture. */
	inline int GetComponentTexture() const{ return pComponentTexture; }
	
	/** Set component and texture or nullptr if unused. */
	void SetComponent(deoglRComponent *component, int texture);
	
	/** Clip plane is used. */
	inline bool GetUseClipPlane() const{ return pUseClipPlane; }
	
	/** Set if clip plane is used. */
	void SetUseClipPlane(bool useClipPlane);
	
	/** Clipping plane normal. */
	inline const decVector &GetClipNormal() const{ return pClipNormal; }
	
	/** Set clipping plane normal. */
	void SetClipNormal(const decVector &normal);
	
	/** Clipping plane distance. */
	inline float GetClipDistance() const{ return pClipDistance; }
	
	/** Set clipping plane distance. */
	void SetClipDistance(float distance);
	
	/** Stereo clipping plane normal. */
	inline const decVector &GetClipNormalStereo() const{ return pClipNormalStereo; }
	
	/** Set stereo clipping plane normal. */
	void SetClipNormalStereo(const decVector &normal);
	
	/** Stereo clipping plane distance. */
	inline float GetClipDistanceStereo() const{ return pClipDistanceStereo; }
	
	/** Set stereo clipping plane distance. */
	void SetClipDistanceStereo(float distance);
	
	/** Stencil mask. */
	inline int GetStencilMask() const{ return pStencilMask; }
	
	/** Set stencil mask. */
	void SetStencilMask(int mask);
	
	/** Parent stencil mask. */
	inline int GetParentStencilMask() const{ return pParentStencilMask; }
	
	/** Set parent stencil mask. */
	void SetParentStencilMask(int mask);
	/*@}*/
};

#endif
