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

#ifndef _DEBPSWEEPCOLLISIONTEST_H_
#define _DEBPSWEEPCOLLISIONTEST_H_

#include "../world/debpCollisionWorld.h"

#include "LinearMath/btTransform.h"

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/shape/decShapeVisitor.h>

class debpCollisionDetection;
class debpCollider;
class debpGhostObject;
class decShapeList;
class btConvexShape;
class btGhostObject;



/**
 * \brief Bullet collision shape sweep collision testing.
 * \details Stores a list of convex collision shapes usable for sweep testing.
 *          The list is populated using shape visiting. Once created the object
 *          can be used to carry out sweep collision tests on btCollisionWorld.
 */
class debpSweepCollisionTest : protected decShapeVisitor{
public:
	/**
	* \brief Convex shape in a sweep collision test object.
	* \author Plüss Roland
	* \version 1.0
	* \date 2014
	*/
	class cShape{
	private:
		btConvexShape *pShape;
		btTransform pTransform;
		
	public:
		/** \name Constructors and Destructors */
		/*@{*/
		/** \brief Creates a new collision test shape. */
		cShape(btConvexShape *shape, const btTransform &transform);
		/** \brief Cleans up the shape. */
		~cShape();
		/*@}*/
		
		/** \name Management */
		/*@{*/
		/** \brief Retrieves the convex shape. */
		inline const btConvexShape *GetShape() const{ return pShape; }
		/** \brief Retrieves the transform. */
		inline const btTransform &GetTransform() const{ return pTransform; }
		/*@}*/
	};
	
private:
	debpCollisionDetection &pColDet;
	decPointerList pShapeList;
	decVector pScale;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sweep collision test. */
	debpSweepCollisionTest(debpCollisionDetection &coldet);
	
	/** \brief Clean up the visitor. */
	virtual ~debpSweepCollisionTest();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the list of cShape pointers. */
	inline const decPointerList &GetShapeList() const{ return pShapeList; }
	
	/** \brief Add a shape from a decShape. */
	void AddShape(decShape &shape, const decVector &scale);
	/** \brief Adds all shapes from a decShapeList. */
	void AddShapes(const decShapeList &list, const decVector &scale);
	/** \brief Remove all shapes. */
	void RemoveAllShapes();
	
	/** \brief Script callback safe sweep test for collision in a world. */
	void SweepTest(debpCollisionWorld &world, const btTransform &from, const btTransform &to,
		debpCollisionWorld::ConvexResultCallback &resultCallback);
	
	/** \brief Script callback safe Sweep test for collision in a ghost object. */
	void SweepTest(debpGhostObject &ghostObject, const btTransform &from, const btTransform &to,
		debpCollisionWorld::ConvexResultCallback &resultCallback);
	
	/** \brief Script callback safe sweep test for collision against collider. */
	void SweepTest(debpCollider &collider, const btTransform &from, const btTransform &to,
		debpCollisionWorld::ConvexResultCallback &resultCallback);
	/*@}*/
	
	
	
protected:
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void VisitShape(decShape &shape);
	
	/** \brief Visit sphere shape. */
	virtual void VisitShapeSphere(decShapeSphere &sphere);
	
	/** \brief Visit box shape. */
	virtual void VisitShapeBox(decShapeBox &box);
	
	/** \brief Visit cylinder shape. */
	virtual void VisitShapeCylinder(decShapeCylinder &cylinder);
	
	/** \brief Visit capsule shape. */
	virtual void VisitShapeCapsule(decShapeCapsule &capsule);
	
	/** \brief Visit hull shape. */
	virtual void VisitShapeHull(decShapeHull &hull);
	/*@}*/
	
	
	
private:
	void SweepTest(btCollisionObject &collisionObject, const btVector3 &castShapeAabbMin, 
		const btVector3 &castShapeAabbMax, const btTransform &rfrom, const btTransform &rto,
		const btConvexShape &castShape, debpCollisionWorld::ConvexResultCallback &resultCallback);
};

#endif
