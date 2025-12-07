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

#ifndef _DEOGLRENDERPLANLIST_H_
#define _DEOGLRENDERPLANLIST_H_

class deoglRenderPlan;



/**
 * Render Plan List.
 * List of pointers to render plans. The ordering of the render plans in the list is not defined and can
 * change when render plans are removed. This allows to remove render plans with constant time requirements.
 */
class deoglRenderPlanList{
private:
	deoglRenderPlan **pPlans;
	int pPlanCount;
	int pPlanSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new render plan list. */
	deoglRenderPlanList();
	/** Cleans up the render plan list. */
	~deoglRenderPlanList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of render plans. */
	inline int GetCount() const{return pPlanCount;}
	/** Retrieves the index of the render plan or -1 if not in the list. */
	int IndexOf(deoglRenderPlan *plan) const;
	/** Determines if the render plan exists. */
	bool Has(deoglRenderPlan *plan) const;
	/** Retrieves the render plan at the given index. */
	deoglRenderPlan *GetAt(int index) const;
	/** Adds a render plan. */
	void Add(deoglRenderPlan *plan);
	/** Adds a render plan if missing in the list. Returns true if added. */
	bool AddIfMissing(deoglRenderPlan *plan);
	/** Removes render plan. */
	void Remove(deoglRenderPlan *plan);
	/** Removes render plan if existing in the list. Returns true if removed. */
	bool RemoveIfExisting(deoglRenderPlan *plan);
	/** Removes occlusion mesh from the given index. */
	void RemoveFrom(int index);
	/** Removes all render plans. */
	void RemoveAll();
	/*@}*/
	
private:
	void pAdd(deoglRenderPlan *plan);
};

#endif
