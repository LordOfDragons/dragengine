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
#ifndef _DEBNSTATELINKMANAGER_H_
#define _DEBNSTATELINKMANAGER_H_

// predefintions
class debnStateLink;
class debnState;



/**
 * @brief State Link Manager.
 * Manages a list of established network state links.
 */
class debnStateLinkManager{
private:
	debnStateLink *pHeadLink;
	debnStateLink *pTailLink;
	int pLinkCount;
	int pNextIdentifier;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new state link manager object. */
	debnStateLinkManager();
	/** Cleans up the state link manager object. */
	~debnStateLinkManager();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	/** Retrieves the head link or NULL. */
	inline debnStateLink *GetHeadLink() const{ return pHeadLink; }
	/** Retrieves the link with the given identifier or NULL if not found. */
	debnStateLink *GetLinkWithIdentifier( int identifier ) const;
	/** Retrieves the link with the given network state or NULL if not found. */
	debnStateLink *GetLinkWithNetworkState( debnState *networkState ) const;
	/** Add a link. */
	void AddLink( debnStateLink *link );
	/** Removes a link. */
	void RemoveLink( debnStateLink *link );
	/** Removes all links. */
	void RemoveAllLinks();
	
	/** Retrieves a new free identifier. */
	int GetFreeIdentifier();
	/*@}*/
	
private:
};

// end of include only once
#endif
