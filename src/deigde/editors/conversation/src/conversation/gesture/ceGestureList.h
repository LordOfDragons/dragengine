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

#ifndef _CEGESTURELIST_H_
#define _CEGESTURELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceGesture;



/**
 * \brief Gesture List.
 */
class ceGestureList{
private:
	decObjectOrderedSet pGestures;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new gesture list. */
	ceGestureList();
	/** Cleans up the gesture list. */
	~ceGestureList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of gestures. */
	int GetCount() const;
	/** Retrieves the gesture at the given position. */
	ceGesture *GetAt( int index ) const;
	/** Retrieves the named gesture or NULL if not found. */
	ceGesture *GetNamed( const char *name ) const;
	/** Retrieves the index of a gesture or -1 if not found. */
	int IndexOf( ceGesture *gesture ) const;
	/** Retrieves the index of the named gesture or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Determines if a gesture exists. */
	bool Has( ceGesture *gesture ) const;
	/** Determines if a named gesture exists. */
	bool HasNamed( const char *name ) const;
	/** Adds a gesture. */
	void Add( ceGesture *gesture );
	/** Removes a gesture. */
	void Remove( ceGesture *gesture );
	/** Removes all gestures. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	ceGestureList &operator=( const ceGestureList &list );
	/*@}*/
};

#endif
