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

#ifndef _DEOALMOVRAYHITSCLOSEST_H_
#define _DEOALMOVRAYHITSCLOSEST_H_

#include <dragengine/common/math/decMath.h>

#include "../../../model/octree/deoalModelOctreeVisitor.h"
#include "../../../model/octree/deoalModelRTOctree.h"
#include "../../../model/octree/deoalModelRTBVH.h"

class deoalAComponent;
class deoalAModel;
class deoalModelFace;
class deoalRayTraceResult;


// #define MOVRAYHITSCLOSEST_DO_TIMIING 1

#ifdef MOVRAYHITSCLOSEST_DO_TIMIING
#include <dragengine/common/utils/decTimer.h>
#endif


/**
 * \brief Model octree visitor closest hit face.
 * 
 * Visits octree with box enclosing ray. Closest face hit by ray is stored.
 */
class deoalMOVRayHitsClosest : public deoalModelOctreeVisitor{
private:
	deoalAComponent &pComponent;
	const deoalAModel &pModel;
	decVector pRayOrigin;
	decVector pRayDirection;
	decVector pRayTarget;
	
	bool pFrontFacing;
	float pLimitDistance;
	
	decVector pInvRayDirection;
	bool pCheckAxisX;
	bool pCheckAxisY;
	bool pCheckAxisZ;
	
	float pRayLength;
	decVector pRayBoxMin;
	decVector pRayBoxMax;
	
	bool pHasResult;
	float pResultDistance;
	decVector pResultPoint;
	const decVector *pResultNormal;
	int pResultFace;
	
#ifdef MOVRAYHITSCLOSEST_DO_TIMIING
public:
	decTimer timer;
	float timing;
	int timingCount;
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalMOVRayHitsClosest(deoalAComponent &component, const deoalAModel &model);
	
	/** \brief Clean up visitor. */
	virtual ~deoalMOVRayHitsClosest();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Ray origin. */
	inline const decVector &GetRayOrigin() const{ return pRayOrigin; }
	
	/** \brief Ray direction. */
	inline const decVector &GetRayDirection() const{ return pRayDirection; }
	
	/** \brief Ray length. */
	inline float GetRayLength() const{ return pRayLength; }
	
	/** \brief Ray box minimum extend. */
	inline const decVector &GetRayBoxMin() const{ return pRayBoxMin; }
	
	/** \brief Ray box maximum extend. */
	inline const decVector &GetRayBoxMax() const{ return pRayBoxMax; }
	
	/** \brief Set test ray. */
	void SetRay(const decVector &origin, const decVector &direction);
	
	/** \brief Find front facing hits otherwise back facing hits. */
	inline bool GetFrontFacing() const{ return pFrontFacing; }
	
	/** \brief Set to find front facing hits or back facing hits. */
	void SetFrontFacing(bool frontFacing);
	
	/** \brief Limit distance. */
	inline float GetLimitDistance() const{ return pLimitDistance; }
	
	/** \brief Set limit distance. */
	void SetLimitDistance(float limitDistance);
	
	
	
	/** \brief Result is set. */
	inline bool GetHasResult() const{ return pHasResult; }
	
	/** \brief Result distance. */
	inline float GetResultDistance() const{ return pResultDistance; }
	
	/** \brief Result point. */
	inline const decVector GetResultPoint() const{ return pResultPoint; }
	
	/** \brief Result normal. */
	inline const decVector &GetResultNormal() const{ return *pResultNormal; }
	
	/** \brief Result face index. */
	inline int GetResultFace() const{ return pResultFace; }
	
	
	
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
	void pVisitNode(const deoalModelOctree &node);
	void pVisitNode(const deoalModelRTOctree &octree, const deoalModelRTOctree::sNode &node);
	void pVisitNode(const deoalModelRTBVH &bvh, const deoalModelRTBVH::sNode &node);
	bool pRayHitsBox(const decVector &center, const decVector &halfExtends, float &closestDistance);
	void pSetResult(float distance, const decVector &point, const decVector &normal, int face);
};

#endif
