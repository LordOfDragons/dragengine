/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSASTNODELIST_H_
#define _DEOGLSASTNODELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deoglSASTNode;



/**
 * @brief Shader Abstract Syntax Tree Declaration List.
 */
class deoglSASTNodeList{
private:
	decObjectOrderedSet pNodes;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new shader abstraxt syntax tree node list. */
	deoglSASTNodeList();
	/** Cleans up the shader abstraxt syntax tree node list. */
	~deoglSASTNodeList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of nodes. */
	int GetCount() const;
	/** Retrieves the index of the node or -1 if not in the list. */
	int IndexOf( deoglSASTNode *node ) const;
	/** Determines if the node exists. */
	bool Has( deoglSASTNode *node ) const;
	/** Retrieves the node at the given index. */
	deoglSASTNode *GetAt( int index ) const;
	/** Adds a node. */
	void Add( deoglSASTNode *node );
	/** Removes a node. */
	void Remove( deoglSASTNode *node );
	/** Removes all nodes. */
	void RemoveAll();
	/*@}*/
};

#endif
