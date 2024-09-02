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
