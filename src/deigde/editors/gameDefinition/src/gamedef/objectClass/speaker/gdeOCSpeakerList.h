/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEOCSPEAKERLIST_H_
#define _GDEOCSPEAKERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCSpeaker;



/**
 * \brief Object class speaker list.
 */
class gdeOCSpeakerList{
private:
	decObjectOrderedSet pSpeakers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class speaker list. */
	gdeOCSpeakerList();
	
	/** \brief Create copy of object class speaker list. */
	gdeOCSpeakerList( const gdeOCSpeakerList &list );
	
	/** \brief Clean up object class speaker list. */
	~gdeOCSpeakerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class speakers. */
	int GetCount() const;
	
	/** \brief Object class speaker at position. */
	gdeOCSpeaker *GetAt( int index ) const;
	
	/** \brief Index of named object class speaker or -1 if absent. */
	int IndexOf( gdeOCSpeaker *speaker ) const;
	
	/** \brief Object class speaker is present. */
	bool Has( gdeOCSpeaker *speaker ) const;
	
	/** \brief Add object class speaker. */
	void Add( gdeOCSpeaker *speaker );
	
	/** \brief Remove object class speaker. */
	void Remove( gdeOCSpeaker *speaker );
	
	/** \brief Remove all object class speakers. */
	void RemoveAll();
	
	/** \brief Set from another object class speaker list. */
	gdeOCSpeakerList &operator=( const gdeOCSpeakerList &list );
	/*@}*/
};

#endif
