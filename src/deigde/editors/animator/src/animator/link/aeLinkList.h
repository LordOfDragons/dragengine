/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AELINKLIST_H_
#define _AELINKLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class aeLink;



/**
 * \brief Link List.
 */
class aeLinkList{
private:
	decObjectOrderedSet pLinks;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new link list. */
	aeLinkList();
	
	/** \brief Create copy of a link list. */
	aeLinkList( const aeLinkList &copy );
	
	/** \brief Clean up link list. */
	~aeLinkList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Number of links. */
	int GetCount() const;
	
	/**
	 * \brief Link at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	aeLink *GetAt( int index ) const;
	
	/** \brief Named link. */
	aeLink *GetNamed( const char *name ) const;
	
	/**
	 * \brief Index of link or -1 if not present in the list.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	int IndexOf( aeLink *link ) const;
	
	/**
	 * \brief Determine if a link is present in the list.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	bool Has( aeLink *link ) const;
	
	/**
	 * \brief Named link is present in the list.
	 */
	bool HasNamed( const char *name ) const;
	
	/**
	 * \brief Add link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	void Add( aeLink *link );
	
	/**
	 * \brief Insert link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert( aeLink *link, int index );
	
	/**
	 * \brief Move link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move( aeLink *link, int index );
	
	/**
	 * \brief Remove link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em link is not present in the list.
	 */
	void Remove( aeLink *link );
	
	/** \brief Remove all links. */
	void RemoveAll();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy link list to this link list. */
	aeLinkList &operator=( const aeLinkList &list );
	
	/** \brief Add links from link list if not present in this link list. */
	aeLinkList &operator+=( const aeLinkList &list );
	/*@}*/
};

#endif
