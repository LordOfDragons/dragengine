/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPDEBUGINFORMATIONLIST_H_
#define _DEBPDEBUGINFORMATIONLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class debpDebugInformation;



/**
 * \brief List of debug information for performance profiling.
 */
class debpDebugInformationList{
private:
	decObjectOrderedSet pChildren;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create debug information list. */
	debpDebugInformationList();
	
	/** \brief Clean up debug information list. */
	virtual ~debpDebugInformationList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of children. */
	int GetCount() const;
	
	/** \brief Child at index. */
	debpDebugInformation *GetAt( int index ) const;
	
	/** \brief Add child. */
	void Add( debpDebugInformation *child );
	
	/** \brief Remove child. */
	void Remove( debpDebugInformation *child );
	
	/** \brief Remove child if present. */
	void RemoveIfPresent( debpDebugInformation *child );
	
	/** \brief Remove all children. */
	void RemoveAll();
	/*@}*/
};

#endif
