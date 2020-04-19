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

#ifndef _DEOGLRENDERTASKINSTANCEGROUP_H_
#define _DEOGLRENDERTASKINSTANCEGROUP_H_



/**
 * \brief Group of instances for render tasks.
 * 
 * Instance group structs are used to group instances for render taks. A group of instances
 * is represented as one deoglRenderTaskInstance with each member of the group as a sub
 * instance. This allows adding instances to render task with O(1) complexity.
 * 
 * This class is only used by the render task to provide the grouping support in O(1)
 * complexity. No additional information is carried. The owner of this struct is responsible
 * to set up the deoglRenderTaskInstance with the appropriate information. All group members
 * share the same information then.
 */
class deoglRenderTaskInstanceGroup{
private:
	unsigned int pTrackingNumber;
	int pIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render task instance group. */
	deoglRenderTaskInstanceGroup();
	
	
	
	/** \brief Tracking number. */
	inline unsigned int GetTrackingNumber() const{ return pTrackingNumber; }
	
	/** \brief Set tracking number. */
	void SetTrackingNumber( unsigned int trackingNumber );
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex( int index );
};

#endif
