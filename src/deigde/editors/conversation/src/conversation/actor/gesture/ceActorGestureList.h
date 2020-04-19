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

#ifndef _CEACTORGESTURELIST_H_
#define _CEACTORGESTURELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceActorGesture;



/**
 * \brief Actor gesture list.
 */
class ceActorGestureList{
private:
	decObjectOrderedSet pGestures;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create gesture list. */
	ceActorGestureList();
	
	/** \brief Create gesture list. */
	ceActorGestureList( const ceActorGestureList &gesture );
	
	/** \brief Clean up gesture list. */
	~ceActorGestureList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of gestures. */
	int GetCount() const;
	
	/** \brief Gesture at index. */
	ceActorGesture *GetAt( int index ) const;
	
	/** \brief Named gesture or \em NULL if absent. */
	ceActorGesture *GetNamed( const char *name ) const;
	
	/** \brief Index of gesture or -1 if absent. */
	int IndexOf( ceActorGesture *gesture ) const;
	
	/** \brief Index of named gesture or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Gesture is present. */
	bool Has( ceActorGesture *gesture ) const;
	
	/** \brief Named gesture is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add gesture. */
	void Add( ceActorGesture *gesture );
	
	/** \brief Remove gesture. */
	void Remove( ceActorGesture *gesture );
	
	/** \brief Remove all gestures. */
	void RemoveAll();
	
	/** \brief Set from another list. */
	ceActorGestureList &operator=( const ceActorGestureList &list );
	/*@}*/
};

#endif
