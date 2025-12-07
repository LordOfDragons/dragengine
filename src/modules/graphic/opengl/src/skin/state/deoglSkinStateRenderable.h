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

#ifndef _DEOGLSKINSTATERENDERABLE_H_
#define _DEOGLSKINSTATERENDERABLE_H_

class deoglRenderPlan;
class deoglRenderThread;
class deoglTexture;
class deoglRSkin;
class deoalSkinState;
class deoglRDynamicSkin;


/**
 * Skin State Renderable.
 * 
 * Stores the host renderable linked to this skin renderable and additional
 * objects required for masked rendering if required.
 */
class deoglSkinStateRenderable{
private:
	deoglSkinState &pSkinState;
	int pIndex;
	int pHostRenderable;
	deoglRenderPlan *pPlan;
	deoglTexture *pTexture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state renderable. */
	deoglSkinStateRenderable(deoglSkinState &skinState, int index);
	
	/** Clean up skin state renderable. */
	~deoglSkinStateRenderable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Host renderable index or -1 if not used. */
	inline int GetHostRenderable() const{return pHostRenderable;}
	
	/** Set host renderable index or -1 if not used. */
	void SetHostRenderable(int hostRenderable);
	
	/** Render plan or NULL if not using masked rendering. */
	inline deoglRenderPlan *GetPlan() const{return pPlan;}
	
	/** Set render plan or NULL if not using masked rendering. */
	void SetPlan(deoglRenderPlan *plan);
	
	/** Temporary texture or NULL if not existing. */
	inline deoglTexture *GetTexture() const{return pTexture;}
	
	/** Set temporary texture or NULL if not existing. */
	void SetTexture(deoglTexture *texture);
	
	/** Clear renderable to be used for another one. */
	void Clear();
	
	
	
	/** Add render plans. */
	void AddRenderPlans(deoglRenderPlan &plan);
	/*@}*/
};

#endif
