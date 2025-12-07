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

#ifndef _DEOALWOVRAYTRACE_H_
#define _DEOALWOVRAYTRACE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

#include "deoalWorldOctreeVisitor.h"

class deoalAComponent;



/**
 * \brief World octree visitor supporting ray tracing.
 * 
 * Extends basic visitor with support to store ray tracing properties. Visited components are
 * checked for intersection with ray before being send to the respective visit function.
 * 
 * Only components affecting sound are ray-checked and visited. To check also other elements
 * overwrite VisitNode and implement visiting other elements if required.
 */
class deoalWOVRayTrace : public deoalWorldOctreeVisitor{
private:
	decDVector pRayOrigin;
	decDVector pRayDirection;
	decDVector pRayTarget;
	decLayerMask pLayerMask;
	
	decDVector pInvRayDirection;
	bool pCheckAxisX;
	bool pCheckAxisY;
	bool pCheckAxisZ;
	
	double pRayLength;
	decDVector pRayBoxMin;
	decDVector pRayBoxMax;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalWOVRayTrace();
	
	/** \brief Clean up visitor. */
	virtual ~deoalWOVRayTrace();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Ray origin. */
	inline const decDVector &GetRayOrigin() const{ return pRayOrigin; }
	
	/** \brief Ray direction. */
	inline const decDVector &GetRayDirection() const{ return pRayDirection; }
	
	/** \brief Ray direction has X axis component not equal to 0. */
	inline bool GetCheckAxisX() const{ return pCheckAxisX; }
	
	/** \brief Ray direction has Y axis component not equal to 0. */
	inline bool GetCheckAxisY() const{ return pCheckAxisY; }
	
	/** \brief Ray direction has Z axis component not equal to 0. */
	inline bool GetCheckAxisZ() const{ return pCheckAxisZ; }
	
	/**
	 * \brief Inverted ray direction.
	 * 
	 * Returns vector 1/rayDirection for all components.
	 */
	inline const decDVector &GetInvRayDirection() const{ return pInvRayDirection; }
	
	/** \brief Ray length. */
	inline double GetRayLength() const{ return pRayLength; }
	
	/** \brief Ray box minimum extend. */
	inline const decDVector &GetRayBoxMin() const{ return pRayBoxMin; }
	
	/** \brief Ray box maximum extend. */
	inline const decDVector &GetRayBoxMax() const{ return pRayBoxMax; }
	
	/** \brief Set test ray. */
	void SetRay(const decDVector &origin, const decDVector &direction);
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	
	
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
	
	/** \brief Visits components affecting sound hit by ray. */
	virtual void VisitNode(deoalDOctree *node, int intersection);
	/*@}*/
	
	
	
protected:
	bool pRayHitsBox(const decDVector &center, const decDVector &halfExtends);
};

#endif
