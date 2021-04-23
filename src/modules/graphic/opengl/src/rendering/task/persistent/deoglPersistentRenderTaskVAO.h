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

#ifndef _DEOGLPERSISTENTRENDERTASKVAO_H_
#define _DEOGLPERSISTENTRENDERTASKVAO_H_

#include "../../../deoglBasics.h"

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

class deoglQuickSorter;
class deoglPersistentRenderTaskInstance;
class deoglSharedSPB;
class deoglSharedSPBRTIGroup;
class deoglVAO;


/**
 * Persistent render task VAO.
 */
class deoglPersistentRenderTaskVAO{
private:
	deoglVAO *pVAO;
	void *pOwner;
	decPointerList pInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create persistent render task vao. */
	deoglPersistentRenderTaskVAO( deoglVAO *vao );
	
	/** Clean up persistent render task vao. */
	~deoglPersistentRenderTaskVAO();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** VAO. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	
	/** Owner object or NULL. */
	inline void *GetOwner() const{ return pOwner; }
	
	/** Set owner object or NULL. */
	void SetOwner( void *owner );
	
	
	
	/** Total amount of points. */
	int GetTotalPointCount() const;
	
	/** Total amount of sub-instances. */
	int GetTotalSubInstanceCount() const;
	
	
	
	/** Number of instances. */
	int GetInstanceCount() const;
	
	/** Instance at index. */
	deoglPersistentRenderTaskInstance *GetInstanceAt( int index ) const;
	
	/** Instance with shared sub instance spb. */
	deoglPersistentRenderTaskInstance *GetInstanceWith( deoglSharedSPBRTIGroup *group ) const;
	
	/** Add instance. */
	deoglPersistentRenderTaskInstance *AddInstance( deoglSharedSPB *spb = NULL,
		deoglSharedSPBRTIGroup *group = NULL );
	
	/** Remove all instances. */
	void RemoveAllInstances();
	
	
	
	/** Remove elements owned by owner. */
	void RemoveOwnedBy( void *owner );
	/*@}*/
};

#endif
