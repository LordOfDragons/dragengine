/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEGDCSPEAKERLIST_H_
#define _IGDEGDCSPEAKERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCSpeaker;



/**
 * \brief Game definition class speaker list.
 */
class DE_DLL_EXPORT igdeGDCSpeakerList{
private:
	decObjectOrderedSet pSpeakers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCSpeakerList();
	
	/** \brief Create copy of a game definition class speaker list. */
	igdeGDCSpeakerList( const igdeGDCSpeakerList &list );
	
	/** \brief Clean up list. */
	~igdeGDCSpeakerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of speakers. */
	int GetCount() const;
	
	/** \brief Speaker by index. */
	igdeGDCSpeaker *GetAt( int index ) const;
	
	/** \brief Index of an speaker or -1 if not found. */
	int IndexOf( igdeGDCSpeaker *speaker ) const;
	
	/** \brief Determines if an speaker exists. */
	bool Has( igdeGDCSpeaker *speaker ) const;
	
	/** \brief Adds an speaker. */
	void Add( igdeGDCSpeaker *speaker );
	
	/** \brief Inserts a new speaker. */
	void InsertAt( igdeGDCSpeaker *speaker, int index );
	
	/** \brief Moves an speaker to a new position. */
	void MoveTo( igdeGDCSpeaker *speaker, int index );
	
	/** \brief Removes an speaker. */
	void Remove( igdeGDCSpeaker *speaker );
	
	/** \brief Removes all speakers. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCSpeakerList &list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCSpeakerList &operator=( const igdeGDCSpeakerList &list );
	/*@}*/
};

#endif
