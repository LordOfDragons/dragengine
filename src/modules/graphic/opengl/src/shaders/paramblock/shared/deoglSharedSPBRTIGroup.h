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

#ifndef _DEOGLSHAREDSPBRTIGROUP_H_
#define _DEOGLSHAREDSPBRTIGROUP_H_

#include "deoglSharedSPBRTIGroupList.h"

#include <dragengine/deObject.h>

class deoglSharedSPB;
class deoglSharedSPBRTIGroupList;
class deoglRenderTaskSharedInstance;


/**
 * OpenGL shared SPB render task instance group.
 * 
 * Stores a deoglRenderTaskInstanceGroup used to group instances in render tasks.
 * The shared spb is used to find the matching group and is not reference counted.
 * 
 * If the last reference to this object is released the object removes itself from the
 * parent group list.
 */
class deoglSharedSPBRTIGroup : public deObject{
public:
	typedef deTObjectReference<deoglSharedSPBRTIGroup> Ref;
	
	
	
private:
	const deoglSharedSPBRTIGroupList::Ref pParent;
	deoglSharedSPB &pSharedSPB;
	int pTextureCount;
	deoglRenderTaskSharedInstance *pRTSInstance;
	unsigned int pUniqueKey;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared SPB render task instance group. */
	deoglSharedSPBRTIGroup( deoglSharedSPBRTIGroupList *parent,
		deoglSharedSPB &sharedSPB, int textureCount );
	
protected:
	/** Clean up shared SPB render task instance group. */
	virtual ~deoglSharedSPBRTIGroup();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Parent list. */
	inline deoglSharedSPBRTIGroupList &GetParent() const{ return pParent; }
	
	/** Shared SPB. */
	inline deoglSharedSPB &GetSharedSPB() const{ return pSharedSPB; }
	
	/** Render task shared instance. */
	inline deoglRenderTaskSharedInstance *GetRTSInstance() const{ return pRTSInstance; }
	
	/** Count of following textures covered by the group. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** Unique key for use with dictionaries. */
	inline unsigned int GetUniqueKey() const{ return pUniqueKey; }
	/*@}*/
};

#endif
