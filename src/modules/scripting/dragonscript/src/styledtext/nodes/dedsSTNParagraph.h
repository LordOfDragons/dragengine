/* 
 * Drag[en]gine DragonScript Script Module
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
#ifndef _DEDSSTNPARAGRAPH_H_
#define _DEDSSTNPARAGRAPH_H_

// includes
#include "dedsStyledTextNode.h"

// predefinitions



/**
 * @brief Styled Text Node Paragraph.
 * Paragraph node in a styled text object storing a list of child nodes.
 */
class dedsSTNParagraph : public dedsStyledTextNode{
private:
	dedsStyledTextNode **pNodes;
	int pNodeCount;
	int pNodeSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new container node. */
	dedsSTNParagraph();
	/** Cleans up the styled text node. */
	virtual ~dedsSTNParagraph();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of child nodes. */
	inline int GetNodeCount() const{ return pNodeCount; }
	/** Retrieves the child node at the given index. */
	dedsStyledTextNode *GetNodeAt( int index ) const;
	/** Retrieves the index of a child node or -1 if not found. */
	int IndexOfNode( dedsStyledTextNode *node ) const;
	/** Determines if a child node exists in the container. */
	bool HasNode( dedsStyledTextNode *node ) const;
	/** Adds a child node. */
	void AddNode( dedsStyledTextNode *node );
	/** Removes a child node. */
	void RemoveNode( dedsStyledTextNode *node );
	/** Removes all child nodes. */
	void RemoveAllNodes();
	/*@}*/
};

// end of include only once
#endif
