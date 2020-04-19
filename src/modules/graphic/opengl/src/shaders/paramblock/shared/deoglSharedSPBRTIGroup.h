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

#include <dragengine/deObject.h>

#include "../../../rendering/task/deoglRenderTaskInstanceGroup.h"

class deoglSharedSPB;
class deoglSharedSPBRTIGroupList;


/**
 * \brief OpenGL shared SPB render task instance group.
 * 
 * Stores a deoglRenderTaskInstanceGroup used to group instances in render tasks.
 * The shared spb is used to find the matching group and is not reference counted.
 * 
 * If the last reference to this object is released the object removes itself from the
 * parent group list.
 */
class deoglSharedSPBRTIGroup : public deObject{
public:
	deoglSharedSPBRTIGroupList &pParent;
	deoglSharedSPB &pSharedSPB;
	deoglRenderTaskInstanceGroup pGroup;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared SPB render task instance group. */
	deoglSharedSPBRTIGroup( deoglSharedSPBRTIGroupList &parent, deoglSharedSPB &sharedSPB );
	
	/** \brief Clean up shared SPB render task instance group. */
	virtual ~deoglSharedSPBRTIGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent list. */
	inline deoglSharedSPBRTIGroupList &GetParent() const{ return pParent; }
	
	/** \brief Shared SPB. */
	inline deoglSharedSPB &GetSharedSPB() const{ return pSharedSPB; }
	
	/** \brief Render task instance group. */
	inline deoglRenderTaskInstanceGroup &GetGroup(){ return pGroup; }
	/*@}*/
};

#endif
