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

#ifndef _DEOGLRENDERPLANLIST_H_
#define _DEOGLRENDERPLANLIST_H_

class deoglRenderPlan;



/**
 * @brief Render Plan List.
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
	inline int GetCount() const{ return pPlanCount; }
	/** Retrieves the index of the render plan or -1 if not in the list. */
	int IndexOf( deoglRenderPlan *plan ) const;
	/** Determines if the render plan exists. */
	bool Has( deoglRenderPlan *plan ) const;
	/** Retrieves the render plan at the given index. */
	deoglRenderPlan *GetAt( int index ) const;
	/** Adds a render plan. */
	void Add( deoglRenderPlan *plan );
	/** Adds a render plan if missing in the list. Returns true if added. */
	bool AddIfMissing( deoglRenderPlan *plan );
	/** Removes render plan. */
	void Remove( deoglRenderPlan *plan );
	/** Removes render plan if existing in the list. Returns true if removed. */
	bool RemoveIfExisting( deoglRenderPlan *plan );
	/** Removes occlusion mesh from the given index. */
	void RemoveFrom( int index );
	/** Removes all render plans. */
	void RemoveAll();
	/*@}*/
	
private:
	void pAdd( deoglRenderPlan *plan );
};

#endif
