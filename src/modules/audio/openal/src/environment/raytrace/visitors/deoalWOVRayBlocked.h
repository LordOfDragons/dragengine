/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALWOVRAYBLOCKED_H_
#define _DEOALWOVRAYBLOCKED_H_

#include <dragengine/common/math/decMath.h>

#include "../../../world/octree/deoalWOVRayTrace.h"



/**
 * \brief World octree visitor checking if ray is blocked.
 * 
 * Similar to deoalWOVRayHitsElement but stops at the first hit and does not store hit parameters.
 */
class deoalWOVRayBlocked : public deoalWOVRayTrace{
private:
	bool pBlocked;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalWOVRayBlocked();
	
	/** \brief Clean up visitor. */
	virtual ~deoalWOVRayBlocked();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Ray is blocked. */
	inline bool GetBlocked() const{ return pBlocked; }
	
	/** \brief Set if ray is blocked. */
	void SetBlocked( bool blocked );
	
	
	/**
	 * \brief Visit octree node.
	 * 
	 * Visitor is responsible to check if the node and children thereof have to be
	 * visited. This allows to apply special checks not handled by deoalDOctree.
	 * 
	 * To visit world call VisitNode(*world.GetOctree()).
	 * 
	 * The default implementation calls VisitNode(deoalDOctree*,int) then visits child nodes.
	 */
	virtual void VisitNode( deoalWorldOctree &node );
	
	/** \brief Visit octree node. */
	virtual void VisitNode( deoalDOctree *node, int intersection );
	
	/** \brief Visit component. */
	virtual void VisitComponent( deoalAComponent *component );
	/*@}*/
};

#endif
