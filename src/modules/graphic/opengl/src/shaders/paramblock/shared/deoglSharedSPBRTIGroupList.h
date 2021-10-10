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

#ifndef _DEOGLSHAREDSPBRTIGROUPLIST_H_
#define _DEOGLSHAREDSPBRTIGROUPLIST_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerList.h>

class deoglRenderThread;
class deoglSharedSPB;
class deoglSharedSPBRTIGroup;



/**
 * OpenGL shared SPB render task instance group list.
 */
class deoglSharedSPBRTIGroupList : public deObject{
public:
	typedef deTObjectReference<deoglSharedSPBRTIGroupList> Ref;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	decPointerList pGroups;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared SPB render task instance group list. */
	deoglSharedSPBRTIGroupList( deoglRenderThread &renderThread );
	
protected:
	/** Clean up shared SPB list. */
	virtual ~deoglSharedSPBRTIGroupList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Count of groups. */
	int GetCount() const;
	
	/** Group at index. */
	deoglSharedSPBRTIGroup *GetAt( int index ) const;
	
	/**
	 * Group group with shared SPB or NULL if not found. Caller obtains reference to the group.
	 * Release reference if not used anymore. Group is removed from this list once all
	 * references are released.
	 */
	deoglSharedSPBRTIGroup *GetWith( deoglSharedSPB &sharedSPB, int textureCount = 1 ) const;
	
	/**
	 * Group group with shared SPB. If group does not exist creates it first. Caller obtains
	 * reference to the group. Release reference if not used anymore. Group is removed from
	 * this list once all references are released.
	 */
	deoglSharedSPBRTIGroup *GetOrAddWith( deoglSharedSPB &sharedSPB, int textureCount = 1 );
	
	/**
	 * Add group with shared SPB. Call only after GetWith returned NULL. If group does not
	 * exist creates it first. Caller obtains reference to the group. Release reference if
	 * not used anymore. Group is removed from this list once all references are released.
	 */
	deoglSharedSPBRTIGroup *AddWith( deoglSharedSPB &sharedSPB, int textureCount = 1 );
	
	
	
	/**
	 * Remove group.
	 * 
	 * \warning For use by deoglSharedSPBRTIGroup only.
	 */
	void Remove( deoglSharedSPBRTIGroup *group );
	/*@}*/
};

#endif
