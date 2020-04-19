/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEHTVRSLOT_H_
#define _MEHTVRSLOT_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class meHTVRLink;
class meHTVRule;



/**
 * \brief Height Terrain Vegetation Rule Slot.
 */
class meHTVRSlot{
private:
	bool pIsInput;
	decObjectOrderedSet pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create input slot. */
	meHTVRSlot();
	
	/** \brief Clean up input slot. */
	~meHTVRSlot();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Is input slot. */
	inline bool GetIsInput() const{ return pIsInput; }
	
	/** \brief Set is input slot. */
	void SetIsInput( bool isInput );
	
	/** \brief Number of links. */
	int GetLinkCount() const;
	
	/** \brief Link at the given index. */
	meHTVRLink *GetLinkAt( int index ) const;
	
	/** \brief Index of the link or -1 if not found. */
	int IndexOfLink( meHTVRLink *link ) const;
	
	/** \brief Link exists. */
	bool HasLink( meHTVRLink *link ) const;
	
	/** \brief Add link. */
	void AddLink( meHTVRLink *link );
	
	/** \brief Remove link. */
	void RemoveLink( meHTVRLink *link );
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	/** \brief Link with the given source node and slot. */
	bool HasLinkWithSource( meHTVRule *rule, int slot ) const;
	
	/** \brief Link with the given source node and slot or NULL if not found. */
	meHTVRLink *GetLinkWithSource( meHTVRule *rule, int slot ) const;
	
	/** \brief Link with the given destination node and slot. */
	bool HasLinkWithDestination( meHTVRule *rule, int slot ) const;
	
	/** \brief Link with the given destination node and slot or NULL if not found. */
	meHTVRLink *GetLinkWithDestination( meHTVRule *rule, int slot ) const;
	/*@}*/
};

#endif
