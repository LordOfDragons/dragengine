/* 
 * Drag[en]gine Basic Network Module
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

// include only once
#ifndef _DEBNSTATELINKLIST_H_
#define _DEBNSTATELINKLIST_H_

// predefintions
class debnStateLink;



/**
 * @brief State Link List.
 * Stores a list of state link pointers. The pointers are not held. The
 * list is organized as a fill up and empty list. State link pointers
 * can be added quickly and processed in linear order using indices.
 * Furthermore the list can be cleared. No individual removing of state
 * link pointers is possible.
 */
class debnStateLinkList{
private:
	debnStateLink **pLinks;
	int pLinkCount;
	int pLinkSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new state link list object. */
	debnStateLinkList();
	/** Cleans up the state link list object. */
	~debnStateLinkList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	/** Retrieves the link at the given index. */
	debnStateLink *GetLinkAt( int index ) const;
	/** Retrieves the index of the link or -1 if not found. */
	int IndexOfLink( debnStateLink *link ) const;
	/** Determines if the link exists in the list. */
	bool HasLink( debnStateLink *link ) const;
	/** Adds a link. */
	void AddLink( debnStateLink *link );
	/** Removes a link. */
	void RemoveLink( debnStateLink *link );
	/** Removes a link if existing in the list. */
	void RemoveLinkIfExisting( debnStateLink *link );
	/** Removes all links. */
	void RemoveAllLinks();
	/*@}*/
};

// end of include only once
#endif
