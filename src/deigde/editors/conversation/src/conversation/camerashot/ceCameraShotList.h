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

#ifndef _CECAMERASHOTLIST_H_
#define _CECAMERASHOTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceCameraShot;



/**
 * \brief Camera Shot List.
 */
class ceCameraShotList{
private:
	decObjectOrderedSet pShots;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new camera shot list. */
	ceCameraShotList();
	/** Cleans up the camera shot list. */
	~ceCameraShotList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of camera shots. */
	int GetCount() const;
	/** Retrieves the camera shot at the given position. */
	ceCameraShot *GetAt( int index ) const;
	/** Retrieves the named camera shot or NULL if not found. */
	ceCameraShot *GetNamed( const char *name ) const;
	/** Retrieves the index of a camera shot or -1 if not found. */
	int IndexOf( ceCameraShot *cameraShot ) const;
	/** Retrieves the index of the named camera shot or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a camera shot exists. */
	bool Has( ceCameraShot *cameraShot ) const;
	/** Determines if a named camera shot exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a camera shot. */
	void Add( ceCameraShot *cameraShot );
	/** Removes a camera shot. */
	void Remove( ceCameraShot *cameraShot );
	/** Removes all camera shots. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceCameraShotList &operator=( const ceCameraShotList &list );
	
	/** \brief Append camera shots if absent. */
	ceCameraShotList &operator+=( const ceCameraShotList &list );
	/*@}*/
};

#endif
