/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESKINPROPERTYNODEGROUP_H_
#define _DESKINPROPERTYNODEGROUP_H_

#include "deSkinPropertyNode.h"

#include "../../../../common/collection/decPointerList.h"


/**
 * \brief Skin property group node for constructed texture property.
 * 
 * Contains a list of nodes applied from first to last.
 */
class deSkinPropertyNodeGroup : public deSkinPropertyNode{
private:
	decPointerList pNodes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property node. */
	deSkinPropertyNodeGroup();
	
	/** \brief Clean up constructed property node. */
	virtual ~deSkinPropertyNodeGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of nodes. */
	int GetNodeCount() const;
	
	/** \brief Node at index. */
	deSkinPropertyNode *GetNodeAt( int index ) const;
	
	/** \brief Add node. */
	void AddNode( deSkinPropertyNode *node );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits node. */
	virtual void Visit( deSkinPropertyNodeVisitor &visitor );
	/*@}*/
};

#endif
