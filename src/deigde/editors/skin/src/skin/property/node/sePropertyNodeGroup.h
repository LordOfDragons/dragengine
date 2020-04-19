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

#ifndef _SEPROPERTYNODEGROUP_H_
#define _SEPROPERTYNODEGROUP_H_

#include "sePropertyNode.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>



/**
 * \brief Texture property group node.
 */
class sePropertyNodeGroup : public sePropertyNode{
private:
	decObjectOrderedSet pNodes;
	seProperty *pProperty;
	bool pActiveGroup;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node. */
	sePropertyNodeGroup( deEngine &engine );
	
	/** \brief Create copy of node. */
	sePropertyNodeGroup( const sePropertyNodeGroup &node );
	
	/** \brief Clean up node. */
	virtual ~sePropertyNodeGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent property or \em NULL if not present. */
	virtual seProperty *GetProperty() const;
	
	/** \brief Set parent property or \em NULL. */
	void SetProperty( seProperty *property );
	
	/** \brief Node group is the active one. */
	inline bool GetActiveGroup() const{ return pActiveGroup; }
	
	/** \brief Set if node group is the active one. */
	void SetActiveGroup( bool active );
	
	
	
	/** \brief Number of nodes. */
	int GetNodeCount() const;
	
	/** \brief Node at index. */
	sePropertyNode *GetNodeAt( int index ) const;
	
	/** \brief Node is present. */
	bool HasNode( sePropertyNode *node ) const;
	
	/** \brief Index of node or -1 if absent. */
	int IndexOfNode( sePropertyNode *node ) const;
	
	/** \brief Add node. */
	void AddNode( sePropertyNode *node );
	
	/** \brief Insert node at index. */
	void InsertNode( int index, sePropertyNode *node );
	
	/** \brief Remove node. */
	void RemoveNode( sePropertyNode *node );
	
	/** \brief Remove all nodes. */
	void RemoveAllNodes();
	
	/** \brief Move node to position. */
	void MoveNode( sePropertyNode *node, int index );
	
	
	
	/** \brief Create copy of node. */
	virtual sePropertyNode *Copy() const;
	
	/** \brief Update resources. */
	virtual void UpdateResources();
	/*@}*/
};

#endif
