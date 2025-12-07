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

#ifndef _DEOALRTWORLDOCTREEVISITOR_H_
#define _DEOALRTWORLDOCTREEVISITOR_H_

#include "deoalRTWorldOctree.h"



/**
 * \brief Ray trace world octree visitor.
 */
class deoalRTWorldOctreeVisitor{
private:
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
	
// 	decVector pRayNormDir;
// 	float pSphereDot;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deoalRTWorldOctreeVisitor();
	
	/** \brief Clean up visitor. */
	virtual ~deoalRTWorldOctreeVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Ray origin. */
	inline const decVector &GetRayOrigin() const{return pRayOrigin;}
	
	/** \brief Ray direction. */
	inline const decVector &GetRayDirection() const{return pRayDirection;}
	
	/** \brief Ray direction has X axis component not equal to 0. */
	inline bool GetCheckAxisX() const{return pCheckAxisX;}
	
	/** \brief Ray direction has Y axis component not equal to 0. */
	inline bool GetCheckAxisY() const{return pCheckAxisY;}
	
	/** \brief Ray direction has Z axis component not equal to 0. */
	inline bool GetCheckAxisZ() const{return pCheckAxisZ;}
	
	/**
	 * \brief Inverted ray direction.
	 * 
	 * Returns vector 1/rayDirection for all components.
	 */
	inline const decVector &GetInvRayDirection() const{return pInvRayDirection;}
	
	/** \brief Ray length. */
	inline float GetRayLength() const{return pRayLength;}
	
	/** \brief Ray box minimum extend. */
	inline const decVector &GetRayBoxMin() const{return pRayBoxMin;}
	
	/** \brief Ray box maximum extend. */
	inline const decVector &GetRayBoxMax() const{return pRayBoxMax;}
	
	/** \brief Set test ray. */
	void SetRay(const decVector &origin, const decVector &direction);
	
	/** \brief Set test ray. */
	void SetRay(const deoalRTWorldOctree &octree, const decDVector &origin, const decDVector &direction);
	
	
	
	/** \brief Visit optimized ray-trace octree node. */
	void VisitOctree(const deoalRTWorldOctree &octree);
	
	/** \brief Visit component. */
	virtual void VisitComponent(const deoalRTWorldOctree::sVisitComponent &rtcomponent);
	/*@}*/
	
	
	
protected:
	void pVisitNode(const deoalRTWorldOctree &octree, const deoalRTWorldOctree::sVisitNode &node);
	bool pRayHitsBox(const decVector &center, const decVector &halfExtends) const;
// 	bool pRayHitsSphere( const decVector &center, float radiusSquared ) const;
	bool pRayHitsBox(const decVector &center, const decVector &halfExtends, float &closestDistance) const;
};

#endif
