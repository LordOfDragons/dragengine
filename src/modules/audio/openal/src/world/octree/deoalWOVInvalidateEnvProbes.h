/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	deoalWOVInvalidateEnvProbes(const decDVector &minExtend,
		const decDVector &maxExtend, const decLayerMask &layerMask);
	
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
	void SetExtends(const decDVector &minExtend, const decDVector &maxExtend);
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	
	
	/** \brief Visits components affecting sound hit by ray. */
	virtual void VisitNode(deoalDOctree *node, int intersection);
	/*@}*/
};

#endif
