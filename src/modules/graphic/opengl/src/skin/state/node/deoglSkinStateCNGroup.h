/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSKINSTATECNGROUP_H_
#define _DEOGLSKINSTATECNGROUP_H_

#include <dragengine/common/collection/decObjectList.h>

#include "deoglSkinStateConstructedNode.h"

class deSkinPropertyNodeGroup;


/**
 * Skin state constructed node.
 */
class deoglSkinStateCNGroup : public deoglSkinStateConstructedNode{
public:
	typedef deTObjectReference<deoglSkinStateCNGroup> Ref;
	
	
	
private:
	decObjectList pNodes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state constructed node. */
	deoglSkinStateCNGroup( deSkinPropertyNodeGroup &node );
	
	/** Create skin state constructed node. */
	deoglSkinStateCNGroup( const deoglSkinStateCNGroup &node );
	
protected:
	/** Clean up skin state mapped. */
	virtual ~deoglSkinStateCNGroup() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Count of child nodes. */
	int GetNodeCount() const;
	
	/** Child node at index. */
	deoglSkinStateConstructedNode *GetNodeAt( int index ) const;
	
	
	
	/** Update. */
	virtual void Update( deoglSkinState &state ) override;
	
	/** Create copy. */
	virtual deoglSkinStateConstructedNode::Ref Copy() const override;
	/*@}*/
};

#endif
