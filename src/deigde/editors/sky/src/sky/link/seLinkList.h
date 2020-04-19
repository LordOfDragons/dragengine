/* 
 * Drag[en]gine IGDE Sky Editor
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
 * \brief Link list.
 */
class seLinkList{
private:
	decObjectOrderedSet pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link list. */
	seLinkList();
	
	/** \brief Clean up link list. */
	~seLinkList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of links. */
	int GetCount() const;
	
	/** \brief Link at index. */
	seLink *GetAt( int index ) const;
	
	/** \brief Index of link or -1 if absent. */
	int IndexOf( seLink *link ) const;
	
	/** \brief Link is absent. */
	bool Has( seLink *link ) const;
	
	/** \brief Add link. */
	void Add( seLink *link );
	
	/** \brief Insert link. */
	void InsertAt( seLink *link, int index );
	
	/** \brief Move link. */
	void MoveTo( seLink *link, int index );
	
	/** \brief Remove link. */
	void Remove( seLink *link );
	
	/** \brief Remove all links. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	seLinkList &operator=( const seLinkList &list );
	
	/** \brief Add list. */
	seLinkList &operator+=( const seLinkList &list );
	/*@}*/
};

#endif
