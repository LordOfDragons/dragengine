/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEPOSELIST_H_
#define _CEPOSELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class cePose;



/**
 * \brief Pose List.
 */
class cePoseList{
private:
	decObjectOrderedSet pPoses;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new pose list. */
	cePoseList();
	/** Cleans up the pose list. */
	~cePoseList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of poses. */
	int GetCount() const;
	/** Retrieves the pose at the given position. */
	cePose *GetAt( int index ) const;
	/** Retrieves the named pose or NULL if not found. */
	cePose *GetNamed( const char *name ) const;
	/** Retrieves the index of a pose or -1 if not found. */
	int IndexOf( cePose *pose ) const;
	/** Retrieves the index of the named pose or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a pose exists. */
	bool Has( cePose *pose ) const;
	/** Determines if a named pose exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a pose. */
	void Add( cePose *pose );
	/** Removes a pose. */
	void Remove( cePose *pose );
	/** Removes all poses. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	cePoseList &operator=( const cePoseList &list );
	/*@}*/
};

#endif
