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

#ifndef _CEFACEPOSELIST_H_
#define _CEFACEPOSELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceFacePose;



/**
 * \brief Face Pose List.
 */
class ceFacePoseList{
private:
	decObjectOrderedSet pFacePoses;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new face pose list. */
	ceFacePoseList();
	/** Cleans up the face pose list. */
	~ceFacePoseList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of face poses. */
	int GetCount() const;
	/** Retrieves the face pose at the given position. */
	ceFacePose *GetAt( int index ) const;
	/** Retrieves the named face pose or NULL if not found. */
	ceFacePose *GetNamed( const char *name ) const;
	/** Retrieves the index of a face pose or -1 if not found. */
	int IndexOf( ceFacePose *facePose ) const;
	/** Retrieves the index of the named face pose or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a face pose exists. */
	bool Has( ceFacePose *facePose ) const;
	/** Determines if a named face pose exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a face pose. */
	void Add( ceFacePose *facePose );
	/** Removes a face pose. */
	void Remove( ceFacePose *facePose );
	/** Removes all face poses. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceFacePoseList &operator=( const ceFacePoseList &list );
	
	/** \brief Append face poses if absent. */
	ceFacePoseList &operator+=( const ceFacePoseList &list );
	/*@}*/
};

#endif
