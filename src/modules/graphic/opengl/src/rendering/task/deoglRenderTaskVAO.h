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

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

#include "../../deoglBasics.h"

class deoglQuickSorter;
class deoglRenderTaskInstance;
class deoglSharedSPB;
class deoglRenderTaskSharedVAO;
class deoglRenderTaskSharedInstance;


/**
 * Render Task VAO.
 */
class deoglRenderTaskVAO{
private:
	const deoglRenderTaskSharedVAO *pVAO;
	
	decPointerList pInstances;
	int pInstanceCount;
	
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
	
	
	
	/** Number of render task instances. */
	inline int GetInstanceCount() const{ return pInstanceCount; }
	
	/** Render task instance at index. */
	deoglRenderTaskInstance *GetInstanceAt( int index ) const;
	
	/** Add render task instance. */
	deoglRenderTaskInstance *AddInstance( deoglRenderTaskSharedInstance *instance );
	
	/** Sort instances by distance. */
	void SortInstancesByDistance( deoglQuickSorter &sorter, const decDVector &position,
		const decDVector &direction, double posDotDir );
	/*@}*/
};

#endif
