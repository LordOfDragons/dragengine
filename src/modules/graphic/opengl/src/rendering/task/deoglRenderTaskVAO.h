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

#ifndef _DEOGLRENDERTASKVAO_H_
#define _DEOGLRENDERTASKVAO_H_

#include <dragengine/common/math/decMath.h>

#include "../../deoglBasics.h"

class deoglQuickSorter;
class deoglRenderTaskInstance;
class deoglSharedSPB;
class deoglRenderTask;
class deoglRenderTaskSharedVAO;
class deoglRenderTaskSharedInstance;


/**
 * Render Task VAO.
 */
class deoglRenderTaskVAO{
private:
	const deoglRenderTaskSharedVAO *pVAO;
	
	deoglRenderTaskInstance *pRootInstance;
	deoglRenderTaskInstance *pTailInstance;
	int pInstanceCount;
	
	deoglRenderTaskVAO *pNextVAO;
	
	deoglRenderTaskVAO *pLLNext;
	
	deoglRenderTaskInstance **pHasInstance;
	int pHasInstanceCount;
	int pHasInstanceSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task vao. */
	deoglRenderTaskVAO();
	
	/** Clean up render task vao. */
	~deoglRenderTaskVAO();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset vao. */
	void Reset();
	
	
	
	/** Total amount of points. */
	int GetTotalPointCount() const;
	
	/** Total amount of sub-instances. */
	int GetTotalSubInstanceCount() const;
	
	
	
	/** VAO. */
	inline const deoglRenderTaskSharedVAO *GetVAO() const{ return pVAO; }
	
	/** Set VAO. */
	void SetVAO( const deoglRenderTaskSharedVAO *vao );
	
	
	
	/** Root render task instance or NULL. */
	inline deoglRenderTaskInstance *GetRootInstance() const{ return pRootInstance; }
	
	/** Number of render task instances. */
	inline int GetInstanceCount() const{ return pInstanceCount; }
	
	/** Add render task instance. */
	deoglRenderTaskInstance *AddInstance( deoglRenderTask &task, deoglRenderTaskSharedInstance *instance );
	
	/** Sort instances by distance. */
	void SortInstancesByDistance( deoglQuickSorter &sorter, const decDVector &position,
		const decDVector &direction, double posDotDir );
	
	/** Next vao to render or NULL. */
	inline deoglRenderTaskVAO *GetNextVAO() const{ return pNextVAO; }
	
	/** Set next vao to render or NULL. */
	void SetNextVAO( deoglRenderTaskVAO *vao );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** Next vao in parent render task VAO pool or NULL. */
	inline deoglRenderTaskVAO *GetLLNext() const{ return pLLNext; }
	
	/** Set next vao in parent render task VAO pool or NULL. */
	void SetLLNext( deoglRenderTaskVAO *vao );
	/*@}*/
};

#endif
