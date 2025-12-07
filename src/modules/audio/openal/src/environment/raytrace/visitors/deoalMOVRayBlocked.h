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

#ifndef _DEOALMOVRAYBLOCKED_H_
#define _DEOALMOVRAYBLOCKED_H_

#include <dragengine/common/math/decMath.h>

#include "../../../model/octree/deoalModelOctreeVisitor.h"
#include "../../../model/octree/deoalModelRTOctree.h"
#include "../../../model/octree/deoalModelRTBVH.h"

class deoalAComponent;
class deoalAModel;
class deoalModelFace;


/**
 * \brief Model octree visitor detecting blocked ray.
 * 
 * Similar to deoalMOVRayHitsFaces but stops visiting at the first found hit and does not
 * store hit parameters.
 */
class deoalMOVRayBlocked : public deoalModelOctreeVisitor{
private:
	deoalAComponent &pComponent;
	decVector pRayOrigin;
	decVector pRayDirection;
	decVector pRayTarget;
	
	decVector pInvRayDirection;
	bool pCheckAxisX;
	bool pCheckAxisY;
	bool pCheckAxisZ;
	
	float pRayLength;
	decVector pRayBoxMin;
	decVector pRayBoxMax;
	
	bool pBlocked;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalMOVRayBlocked(deoalAComponent &component);
	
	/** \brief Clean up visitor. */
	virtual ~deoalMOVRayBlocked();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Ray origin. */
	inline const decVector &GetRayOrigin() const{return pRayOrigin;}
	
	/** \brief Ray direction. */
	inline const decVector &GetRayDirection() const{return pRayDirection;}
	
	/** \brief Ray length. */
	inline float GetRayLength() const{return pRayLength;}
	
	/** \brief Ray box minimum extend. */
	inline const decVector &GetRayBoxMin() const{return pRayBoxMin;}
	
	/** \brief Ray box maximum extend. */
	inline const decVector &GetRayBoxMax() const{return pRayBoxMax;}
	
	/** \brief Set test ray. */
	void SetRay(const decVector &origin, const decVector &direction);
	
	
	
	/** \brief Ray is blocked. */
	inline bool GetBlocked() const{return pBlocked;}
	
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
	virtual void VisitNode(deoalModelOctree &node);
	
	/** \brief Visit octree node. */
	virtual void VisitNode(deoalOctree *node, int intersection);
	
	/** \brief Visit optimized ray-trace octree node. */
	void VisitOctree(const deoalModelRTOctree &octree);
	
	/** \brief Visit optimized ray-trace BVH node. */
	void VisitBVH(const deoalModelRTBVH &bvh);
	/*@}*/
	
	
	
protected:
	void pVisitNode(const deoalModelRTOctree &octree, const deoalModelRTOctree::sNode &node);
	void pVisitNode(const deoalModelRTBVH &bvh, const deoalModelRTBVH::sNode &node);
	bool pRayHitsBox(const decVector &center, const decVector &halfExtends);
};

#endif
