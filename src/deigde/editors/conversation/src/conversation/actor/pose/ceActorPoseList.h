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

#ifndef _CEACTORPOSELIST_H_
#define _CEACTORPOSELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class ceActorPose;



/**
 * \brief Actor pose list.
 */
class ceActorPoseList{
private:
	decObjectOrderedSet pGestures;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create pose list. */
	ceActorPoseList();
	
	/** \brief Create pose list. */
	ceActorPoseList( const ceActorPoseList &pose );
	
	/** \brief Clean up pose list. */
	~ceActorPoseList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of poses. */
	int GetCount() const;
	
	/** \brief Gesture at index. */
	ceActorPose *GetAt( int index ) const;
	
	/** \brief Named pose or \em NULL if absent. */
	ceActorPose *GetNamed( const char *name ) const;
	
	/** \brief Index of pose or -1 if absent. */
	int IndexOf( ceActorPose *pose ) const;
	
	/** \brief Index of named pose or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Gesture is present. */
	bool Has( ceActorPose *pose ) const;
	
	/** \brief Named pose is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add pose. */
	void Add( ceActorPose *pose );
	
	/** \brief Remove pose. */
	void Remove( ceActorPose *pose );
	
	/** \brief Remove all poses. */
	void RemoveAll();
	
	/** \brief Set from another list. */
	ceActorPoseList &operator=( const ceActorPoseList &list );
	/*@}*/
};

#endif
