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
	void SetBlocked(bool blocked);
	
	
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
	virtual void VisitNode(deoalWorldOctree &node);
	
	/** \brief Visit octree node. */
	virtual void VisitNode(deoalDOctree *node, int intersection);
	
	/** \brief Visit component. */
	virtual void VisitComponent(deoalAComponent *component);
	/*@}*/
};

#endif
