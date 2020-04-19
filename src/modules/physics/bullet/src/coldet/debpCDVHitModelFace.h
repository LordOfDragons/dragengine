/* 
 * Drag[en]gine Bullet Physics Module
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
#ifndef _DEBPCDVHITMODELFACE_H_
#define _DEBPCDVHITMODELFACE_H_

// includes
#include <dragengine/common/math/decMath.h>
#include "debpCollisionDetection.h"
#include "octree/debpDOctreeVisitor.h"

// predefinitions
class debpShape;
class debpComponent;
class debpCollider;
class debpColliderVolume;
class debpColliderComponent;



/**
 * @brief Hit Model Face Visitor.
 * Visitor for the collision detection class to test for collision of
 * one or more shapes with faces of a model.
 */
class debpCDVHitModelFace : public debpDOctreeVisitor{
private:
	debpCollisionDetection *pColDet;
	
	debpColliderVolume *pColliderVolume;
	debpColliderComponent *pColliderComponent;
	debpShape *pShape;
	
	debpComponent *pComponent;
	
	debpCollisionResult pResult;
	bool pHasCollision;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new CLASS. */
	debpCDVHitModelFace( debpCollisionDetection *coldet );
	/** Cleans up the CLASS. */
	virtual ~debpCDVHitModelFace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Resets the visitor. */
	void Reset();
	/** Sets the component to test. */
	void SetComponent( debpComponent *component );
	/** Set the shape to test with. */
	void SetTestShape( debpShape *shape );
	/** Sets the collider to test with. */
	void SetTestCollider( debpCollider *collider );
	
	/** Retrieves the result. */
	inline debpCollisionResult &GetResult(){ return pResult; }
	/** Determines if a collision has been found. */
	inline bool HasCollision() const{ return pHasCollision; }
	/*@}*/
	
	/** @name Visiting */
	/*@{*/
	/** Visit a node. */
	virtual void VisitNode( debpDOctree *node, int intersection );
	/*@}*/
};

// end of include only once
#endif
