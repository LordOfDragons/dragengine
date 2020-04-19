/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SELINKLIST_H_
#define _SELINKLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seLink;



/**
 * \brief Link List.
 */
class seLinkList{
private:
	decObjectOrderedSet pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link list. */
	seLinkList();
	
	/** \brief Create copy of link list. */
	seLinkList( const seLinkList &copy );
	
	/** \brief Clean up link list. */
	~seLinkList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of links. */
	int GetCount() const;
	
	/**
	 * \brief Link at index.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	seLink *GetAt( int index ) const;
	
	/**
	 * \brief Index of link or -1 if not present in the list.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	int IndexOf( seLink *link ) const;
	
	/**
	 * \brief Determine if a link is present in the list.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	bool Has( seLink *link ) const;
	
	/**
	 * \brief Add link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 */
	void Add( seLink *link );
	
	/**
	 * \brief Insert link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em index is less than 0 or greater than GetCount()-1.
	 */
	void Insert( seLink *link, int index );
	
	/**
	 * \brief Move link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em from is less than 0 or greater than GetCount()-1.
	 * \throws deeInvalidParam \em to is less than 0 or greater than GetCount().
	 */
	void Move( seLink *link, int index );
	
	/**
	 * \brief Remove link.
	 * \throws deeInvalidParam \em link is \em NULL.
	 * \throws deeInvalidParam \em link is not present in the list.
	 */
	void Remove( seLink *link );
	
	/** \brief Remove all links. */
	void RemoveAll();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy link list to this link list. */
	seLinkList &operator=( const seLinkList &list );
	
	/** \brief Add links from link list if not present in this link list. */
	seLinkList &operator+=( const seLinkList &list );
	/*@}*/
};

#endif
