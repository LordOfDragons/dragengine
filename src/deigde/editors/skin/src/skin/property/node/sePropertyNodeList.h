/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEPROPERTYNODELIST_H_
#define _SEPROPERTYNODELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class sePropertyNode;



/**
 * \brief Texture property node list.
 */
class sePropertyNodeList{
private:
	decObjectOrderedSet pNodes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node list. */
	sePropertyNodeList();
	
	/** \brief Create copy of node. */
	sePropertyNodeList( const sePropertyNodeList &node );
	
	/** \brief Clean up node. */
	~sePropertyNodeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of nodes. */
	int GetCount() const;
	
	/** \brief Node at index. */
	sePropertyNode *GetAt( int index ) const;
	
	/** \brief Node is present. */
	bool Has( sePropertyNode *node ) const;
	
	/** \brief Index of node or -1 if absent. */
	int IndexOf( sePropertyNode *node ) const;
	
	/** \brief Add node. */
	void Add( sePropertyNode *node );
	
	/** \brief Add node if absent. */
	void AddIfAbsent( sePropertyNode *node );
	
	/** \brief Insert node at index. */
	void Insert( int index, sePropertyNode *node );
	
	/** \brief Remove node. */
	void Remove( sePropertyNode *node );
	
	/** \brief Remove node if present. */
	void RemoveIfPresent( sePropertyNode *node );
	
	/** \brief Remove all nodes. */
	void RemoveAll();
	
	/** \brief Move node to position. */
	void Move( sePropertyNode *node, int index );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	
	/** \brief Copy list. */
	sePropertyNodeList &operator=( const sePropertyNodeList &list );
	/*@}*/
};

#endif
