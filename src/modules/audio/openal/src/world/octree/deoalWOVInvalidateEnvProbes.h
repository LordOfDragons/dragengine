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

#ifndef _DEOALWOVINVALIDATEENVPROBES_H_
#define _DEOALWOVINVALIDATEENVPROBES_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

#include "deoalWorldOctreeVisitor.h"



/**
 * \brief World octree visitor invalidating environment probes.
 * 
 * For best performance store an instance of this visitor aside and reuse it. This is because
 * a temporary pointer list is kept inside the visitor to avoid allocating memory all the time.
 * This list is required since environment probes can not be removed while visiting a node is
 * in progress. For this reason first the environment probes to remove are collected in this
 * list and then removed. This is done for each node which is safe.
 */
class deoalWOVInvalidateEnvProbes : public deoalWorldOctreeVisitor{
private:
	decDVector pMinExtend;
	decDVector pMaxExtend;
	decLayerMask pLayerMask;
	decPointerList pRemoveEnvProbes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalWOVInvalidateEnvProbes();
	
	/** \brief Create visitor. */
	deoalWOVInvalidateEnvProbes( const decDVector &minExtend,
		const decDVector &maxExtend, const decLayerMask &layerMask );
	
	/** \brief Clean up visitor. */
	virtual ~deoalWOVInvalidateEnvProbes();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Minimum extend. */
	inline const decDVector &GetMinExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decDVector &GetMaxExtend() const{ return pMaxExtend; }
	
	/** \brief Set extends. */
	void SetExtends( const decDVector &minExtend, const decDVector &maxExtend );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** \brief Visits components affecting sound hit by ray. */
	virtual void VisitNode( deoalDOctree *node, int intersection );
	/*@}*/
};

#endif
