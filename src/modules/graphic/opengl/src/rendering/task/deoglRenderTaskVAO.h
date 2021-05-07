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
class deoglRenderTaskSharedVAO;
class deoglRenderTaskInstanceGroup;


/**
 * \brief Render Task VAO.
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
	/** \brief Create render task vao. */
	deoglRenderTaskVAO();
	
	/** \brief Clean up render task vao. */
	~deoglRenderTaskVAO();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Reset vao. */
	void Reset();
	
	
	
	/** \brief Total amount of points. */
	int GetTotalPointCount() const;
	
	/** \brief Total amount of sub-instances. */
	int GetTotalSubInstanceCount() const;
	
	
	
	/** \brief VAO. */
	inline const deoglRenderTaskSharedVAO *GetVAO() const{ return pVAO; }
	
	/** \brief Set VAO. */
	void SetVAO( const deoglRenderTaskSharedVAO *vao );
	
	
	
	/** \brief Root render task instance or \em NULL. */
	inline deoglRenderTaskInstance *GetRootInstance() const{ return pRootInstance; }
	
	/** \brief Number of render task instances. */
	inline int GetInstanceCount() const{ return pInstanceCount; }
	
	/** \brief Add render task instance. */
	void AddInstance( deoglRenderTaskInstance *instance );
	
	/** \brief Find instance with group (slow call) or \em NULL. */
	deoglRenderTaskInstance *GetInstanceWith( deoglRenderTaskInstanceGroup *group );
	
	/** \brief Instance for group instance index or \em NULL if absent. */
	deoglRenderTaskInstance *GetInstanceForIndex( int index );
	
	/** \brief Sort instances by distance. */
	void SortInstancesByDistance( deoglQuickSorter &sorter, const decDVector &position,
		const decDVector &direction, double posDotDir );
	
	/** \brief Next vao to render or \em NULL. */
	inline deoglRenderTaskVAO *GetNextVAO() const{ return pNextVAO; }
	
	/** \brief Set next vao to render or \em NULL. */
	void SetNextVAO( deoglRenderTaskVAO *vao );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Next vao in parent render task VAO pool or \em NULL. */
	inline deoglRenderTaskVAO *GetLLNext() const{ return pLLNext; }
	
	/** \brief Set next vao in parent render task VAO pool or \em NULL. */
	void SetLLNext( deoglRenderTaskVAO *vao );
	/*@}*/
};

#endif
