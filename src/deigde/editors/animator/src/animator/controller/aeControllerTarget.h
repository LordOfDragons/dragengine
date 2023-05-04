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

#ifndef _AECONTROLLERTARGET_H_
#define _AECONTROLLERTARGET_H_

#include "../link/aeLinkList.h"

class aeLink;
class aeAnimator;
class deAnimatorControllerTarget;



/**
 * Links for a controller target.
 */
class aeControllerTarget{
private:
	aeLinkList pLinks;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new controller target. */
	aeControllerTarget();
	/** Create a copy of a controller target. */
	aeControllerTarget( const aeControllerTarget &copy );
	/** Cleans up the controller target. */
	~aeControllerTarget();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of links. */
	int GetLinkCount() const;
	/** Retrieves the link at the given index. */
	aeLink *GetLinkAt( int index ) const;
	/** Retrieves the index of the link or -1 if not found. */
	int IndexOfLink( aeLink *link ) const;
	/** Determines if the link exists. */
	bool HasLink( aeLink *link ) const;
	/** Adds a new link. */
	void AddLink( aeLink *link );
	/** Removes a link. */
	void RemoveLink( aeLink *link );
	/** Removes all links. */
	void RemoveAllLinks();
	
	/** Updates the given engine target. */
	void UpdateEngineTarget( aeAnimator *animator, deAnimatorControllerTarget &target ) const;
	
	/** Add links to a list of links. */
	void AddLinksToList( aeLinkList &list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another controller target to this controller target. */
	aeControllerTarget &operator=( const aeControllerTarget &copy );
	/*@}*/
};

#endif
