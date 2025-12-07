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

#ifndef _DEBPBULLETSHAPECOLLISION_H_
#define _DEBPBULLETSHAPECOLLISION_H_

#include <dragengine/common/math/decMath.h>

#include <LinearMath/btScalar.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

class dePhysicsBullet;
struct btDbvtNode;
class debpDCollisionVolume;
class btSphereShape;
class debpDCollisionSphere;
class btBoxShape;
class debpDCollisionBox;
class btCollisionObject;
class btTransform;
struct btCollisionObjectWrapper;
class btConvexShape;


/**
 * \brief Drag[en]gine collision detection using bullet collision shapes.
 * 
 * Replaces bullet collision detection with faster dragengine collision detection where
 * possible. Also implements some bullet collision tests which are missing.
 */
class debpBulletShapeCollision{
private:
	dePhysicsBullet &pBullet;
	btAlignedObjectArray<const btDbvtNode*> pRayTestStacks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create collision test instance. */
	debpBulletShapeCollision(dePhysicsBullet &bullet);
	
	/** \brief Clean up collision test instance. */
	~debpBulletShapeCollision();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Tests for collision between two shapes.
	 * 
	 * This is the basic entry point call. Determines the actual collision test type
	 * and processes the collision using either drag[en]gine accelerated collision
	 * testing or slower bullet collision testing where not possible.
	 */
	void ShapeCast(const btConvexShape *castShape, const btTransform &castFromTrans,
	const btTransform &castToTrans, const btCollisionObjectWrapper *colObjWrap,
	btCollisionWorld::ConvexResultCallback &resultCallback, btScalar allowedPenetration);
	
	/**
	 * \brief Test for collision between two a convex shape and a compound shape.
	 * 
	 * Tests each child with an individual call to castShape.
	 */
	void ShapeCastCompound(const btConvexShape *castShape, const btTransform &castFromTrans,
	const btTransform &castToTrans, const btCollisionObjectWrapper *colObjWrap,
	btCollisionWorld::ConvexResultCallback &resultCallback, btScalar allowedPenetration);
	
	/**
	 * \brief Test for collision between two convex shapes.
	 * 
	 * Uses drag[en]gine accelerated collision if possible or else falling back to
	 * slower bullet collision testing.
	 */
	void ShapeCastConvex(const btConvexShape *castShape, const btTransform &castFromTrans,
	const btTransform &castToTrans, const btCollisionObjectWrapper *colObjWrap,
	btCollisionWorld::ConvexResultCallback &resultCallback, btScalar allowedPenetration);
	
	
	
	/**
	 * \brief Test if point is inside collision object.
	 * 
	 * In bullet some collision shape classes have a method implemented like isInside.
	 * Unfortunately no such virtual method is available in the basic btCollisionShape class.
	 * This method provide this functionality by checking the type of the shape and calling
	 * the appropriate method simulating it if absent.
	 */
	bool IsPointInside(const btCollisionObject &shape, const btVector3 &position) const;
	
	/**
	 * \brief Test if point is inside collision shape.
	 * 
	 * In bullet some collision shape classes have a method implemented like isInside.
	 * Unfortunately no such virtual method is available in the basic btCollisionShape class.
	 * This method provide this functionality by checking the type of the shape and calling
	 * the appropriate method simulating it if absent.
	 */
	bool IsPointInside(const btCollisionShape &shape, const btVector3 &position) const;
	/*@}*/
	
	
	
	/** \brief Determine if a start to end matrix transformation contains rotation. */
	bool HasTransformRotation(const btTransform &castFromTrans,
		const btTransform &castToTrans) const;
	
	/** \brief Create box collision volume from bullet box shape. */
	void BoxVolumeFromBoxShape(debpDCollisionBox &boxVolume, btBoxShape &boxShape,
		const btTransform &shapeTransform) const;
	
	/** \brief Create sphere collision volume from bullet sphere shape. */
	void SphereVolumeFromSphereShape(debpDCollisionSphere &sphereVolume,
		btSphereShape &sphereShape, const btTransform &shapeTransform) const;
	
	/** \brief Test for collision using two collision volumes. */
	void VolumeCastVolume(debpDCollisionVolume *castVolume, debpDCollisionVolume *hitVolume,
		const btCollisionObjectWrapper *colObjWrap, const btTransform &castFromTrans,
		const btTransform &castToTrans, btCollisionWorld::ConvexResultCallback &resultCallback) const;
};

#endif
