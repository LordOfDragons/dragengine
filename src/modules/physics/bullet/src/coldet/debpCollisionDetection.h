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

#ifndef _DEBPCOLLISIONDETECTION_H_
#define _DEBPCOLLISIONDETECTION_H_

#include "debpSweepCollisionTest.h"
#include "debpBulletShapeCollision.h"
#include "../shape/debpShapeTransform.h"

#include <dragengine/common/math/decMath.h>

class decCollisionFilter;
class dePhysicsBullet;
class debpWorld;
class debpCollider;
class debpColliderVolume;
class debpColliderComponent;
class debpColliderRig;
class debpCollisionObject;
class debpShape;
class debpHTSector;
class debpHeightTerrain;
class debpHTSector;
class debpComponent;
class deModelFace;

class deBaseScriptingCollider;
class deCollisionInfo;
class debpDCollisionBox;

class btSphereShape;



/**
 * @brief Bullet Collision Detection Result.
 *
 * Result of a collision detection. Depending on the conducted collision test
 * one or more parameters are set. All other parameters are not touched to
 * avoid loosing speed on unused memory juggling. All collision test methods
 * leave the result untouched if no collision has been detected. Only if a
 * collision has been detected and the method returns true the appropriate
 * values in the result are set.
 */
struct debpCollisionResult{
	// the normal of the collision plane
	decDVector normal;
	// the distance to the collision measure relative to the displacement length.
	// therefore 0 equals no displacement and 1 the full displacement vector
	float distance;
	
	// the bone of the testing collider component colliding
	int bone1;
	// the shape of the testing collider volume colliding
	int shape1;
	
	// the hit collider
	debpCollider *collider;
	// the bone in the hit collider component colliding
	int bone2;
	// the shape in the hit collider volume colliding
	int shape2;
	
	// the hit face
	int face;
	// the hit height terrain sector
	debpHTSector *htsector;
};



/**
 * @brief Bullet Collision Detection.
 *
 * Various collision detection routines.
 */
class debpCollisionDetection{
private:
	dePhysicsBullet &pBullet;
	deCollisionInfo *pColInfo;
	debpShapeTransform pShape1;
	debpShapeTransform pShape2;
	debpSweepCollisionTest pRayHackShape;
	debpBulletShapeCollision pShapeCollision;
	
	btSphereShape *pPointTestShape;
	btCollisionObject *pPointTestBulletColObj;
	
	
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new object. */
	debpCollisionDetection( dePhysicsBullet &bullet );
	/** Cleans up the object. */
	~debpCollisionDetection();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Shared collision info. */
	inline deCollisionInfo *GetCollisionInfo(){ return pColInfo; }
	
	/** \brief Bullet shape collision. */
	inline debpBulletShapeCollision &GetBulletShapeCollision(){ return pShapeCollision; }
	
	/** \brief Ray hack shape. */
	inline debpSweepCollisionTest &GetRayHackShape(){ return pRayHackShape; }
	/*@}*/
	
	/** @name Collision Detection */
	/*@{*/
	/**
	 * \brief Test a point for collision with colliders.
	 * 
	 * For each collision the collisionResponse function the given listener is called. To
	 * stop testing set StopTesting in the provided collision information object to true.
	 */
	void PointHits( const decDVector &point, debpWorld &world,
	const decCollisionFilter &collisionFilter, deBaseScriptingCollider &listener );
	
	
	/**
	 * Tests a ray for collisions with world elements. For each hit the listener is invoked and
	 * the collision detection stopped if requested by the user.
	 */
	void RayHits( const decDVector &origin, const decDVector &direction, debpWorld &world,
	const decCollisionFilter &collisionFilter, deBaseScriptingCollider &listener );
	
	/**
	 * Tests a collider for collisions with colliders in a world. For each hit the
	 * listener is invoked and the collision detection stopped if requested by the user.
	 */
	void ColliderHits( debpCollider *collider, debpWorld *world, deBaseScriptingCollider *listener );
	
	/**
	 * Tests a moving collider for collisions with colliders in a world. For each hit the
	 * listener is invoked and the collision detection stopped if requested by the user.
	 */
	void ColliderMoveHits( debpCollider *collider, const decDVector &displacement,
		debpWorld *world, deBaseScriptingCollider *listener );
	
	/**
	 * Tests a rotating collider for collisions with colliders in a world. For each hit the
	 * listener is invoked and the collision detection stopped if requested by the user.
	 */
	void ColliderRotateHits( debpCollider *collider, const decVector &rotation,
		debpWorld *world, deBaseScriptingCollider *listener );
	
	/**
	 * Tests a moving and rotating collider for collisions with colliders in a world. For each hit the
	 * listener is invoked and the collision detection stopped if requested by the user.
	 */
	void ColliderMoveRotateHits( debpCollider *collider, const decVector &displacement,
		const decVector &rotation, debpWorld *world, deBaseScriptingCollider *listener );
	
#if 0
	/**
	 * Determines if a ray hits a height terrain.
	 * \deprecated Can be deleted.
	 */
	bool RayHitsHeightTerrain( const decDVector &origin, const decDVector &direction, debpHTSector *sector,
		bool testHiddenFaces, debpCollisionResult &result );
#endif
	
#if 0
	/**
	 * Determines if a shape hits a height terrain.
	 * \deprecated Can be deleted.
	 */
	bool ShapeHitsHeightTerrain( debpShape *shape, debpHTSector *sector, debpCollisionResult &result );
#endif
	
	/**
	 * Determines if a collider hits a height terrain. If the height terrain is hit the shape1
	 * parameter is set in the result.
	 * \deprecated Still used by debpCollisionWorld::HitContactCallback::process and debpTSShape::ColliderHits()
	 */
	bool ColliderHitsHeightTerrain( debpCollider *collider, debpHTSector *sector, debpCollisionResult &result );
	
#if 0
	/**
	 * Determines if a collider move hits a height terrain. If the height terrain is hit the
	 * distance, normal and shape1 parameter are set in the result.
	 * \deprecated Can be deleted.
	 */
	bool ColliderMoveHitsHeightTerrain( debpCollider *collider, const decDVector &displacement,
		debpHTSector *sector, debpCollisionResult &result );
#endif
	
#if 0
	/**
	 * Determines if a collider rotate hits a height terrain. If the height terrain is hit the
	 * distance, normal and shape1 parameter are set in the result.
	 * \deprecated Can be deleted.
	 */
	bool ColliderRotateHitsHeightTerrain( debpCollider *collider, const decVector &rotation,
		debpHTSector *sector, debpCollisionResult &result );
#endif
	
#if 0
	/**
	 * Determines if a ray hits a collider. The ray is supposed to be transformed relative to the
	 * world. If the collider is hit the shape2 and bone2 parameters are set in the result.
	 */
	bool RayHitsCollider( const decDVector &origin, const decDVector &direction, debpCollider *collider,
		debpCollisionResult &result );
#endif
	
	/**
	 * Determines if a collider hits another collider. The collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the shape1, shape2, bone1 and
	 * bone2 parameters are set in the result.
	 */
	bool ColliderHitsCollider( debpCollider *collider1, debpCollider *collider2, debpCollisionResult &result );
	
#if 0
	/**
	 * Determines if a shape hits a collider. The shape and collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the shape2 and bone2 parameters
	 * are set in the result.
	 */
	bool ShapeHitsCollider( debpShape *shape, debpCollider *collider, debpCollisionResult &result );
#endif
	
#if 0
	/**
	 * Determines if a collider move hits another collider. The collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the shape1, shape2, bone1 and
	 * bone2 parameters are set in the result.
	 */
	bool ColliderMoveHitsCollider( debpCollider *collider1, const decDVector &displacement,
		debpCollider *collider2, debpCollisionResult &result );
#endif
	
	/**
	 * Determines if a shape hits a model face. The shape is supposed to be transformed relative to the
	 * component owning the model.
	 */
	bool ShapeHitsModelFace( debpShape &shape, debpComponent &component, int face );
	
	/**
	 * Determines if a shape move hits a model face. The shape is supposed to be transformed relative
	 * to the model. If the face is hit the normal and distance parameter of the result are set.
	 */
	bool ShapeMoveHitsModelFace( debpShape &shape, const decDVector &displacement,
		const debpComponent &component, int face, debpCollisionResult &result );
	
	/**
	 * Determines if a collider hits a triangle. The collider shapes are supposed to be transformed
	 * relative to the triangle. If the triangle is hit the shape1 parameter of the result are set.
	 */
	bool ColliderHitsTriangle( debpCollider *collider, const decDVector &p1, const decDVector &p2,
		const decDVector &p3, debpCollisionResult &result );
	
#if 0
	/**
	 * Determines if a collider move hits a triangle. The collider shapes are supposed to be
	 * transformed relative to the triangle. If the triangle is hit the shape1, normal and distance
	 * parameter of the result are set.
	 */
	bool ColliderMoveHitsTriangle( debpCollider *collider, const decDVector &displacement,
		const decDVector &p1, const decDVector &p2, const decDVector &p3, debpCollisionResult &result );
#endif
	
	/**
	 * Determines if a collider move hits a model face. The collider shapes are supposed to be
	 * transformed relative to the model. If the face is hit the shape1, normal and distance
	 * parameter of the result are set.
	 */
	bool ColliderMoveHitsModelFace( debpCollider *collider, const decDVector &displacement,
		const debpComponent &component, int face, debpCollisionResult &result );
	
#if 0
	/**
	 * Calculates the bounding box of the shapes of a moving collider.
	 * @warning This method is supposed to be moved later on into the collider itself.
	 */
	void GetColliderMoveBoundingBox( debpCollider &collider, const decDVector &displacement, debpDCollisionBox &box );
#endif
	
#if 0
	/**
	 * Determines if a ray hits a collider component. The ray is supposed to be transformed relative to the
	 * world. If the collider is hit the bone2 parameter is set in the result.
	 */
	bool RayHitsColliderComponent( const decDVector &origin, const decDVector &direction, debpColliderComponent &collider,
		debpCollisionResult &result );
#endif
	
#if 0
	/**
	 * Determines if a ray hits a collider rigged. The ray is supposed to be transformed relative to the
	 * world. If the collider is hit the bone2 parameter is set in the result.
	 */
	bool RayHitsColliderRig( const decDVector &origin, const decDVector &direction, debpColliderRig &collider,
		debpCollisionResult &result );
#endif
	
#if 0
	/**
	 * Determines if a shape hits a collider volume. The shape and collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the shape2 parameters is set in the
	 * result.
	 */
	bool ShapeHitsColliderVolume( debpShape &shape, debpColliderVolume &collider, debpCollisionResult &result );
#endif
	
#if 0
	/**
	 * Determines if a shape hits a collider component. The shape and collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the bone2 parameters is set in the result.
	 */
	bool ShapeHitsColliderComponent( debpShape &shape, debpColliderComponent &collider, debpCollisionResult &result );
#endif
	
#if 0
	/**
	 * Determines if a shape hits a collider rigged. The shape and collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the bone2 parameters is set in the result.
	 */
	bool ShapeHitsColliderRig( debpShape &shape, debpColliderRig &collider, debpCollisionResult &result );
#endif
	
	/**
	 * Determines if a collider volume hits another collider volume. The collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the shape1 and shape2 parameters are set
	 * in the result.
	 */
	bool ColliderVolumeHitsColliderVolume( debpColliderVolume &collider1, debpColliderVolume &collider2,
		debpCollisionResult &result );
	
	/**
	 * Determines if a collider volume hits a collider component. The collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the shape1 and bone2 parameters are set
	 * in the result.
	 */
	bool ColliderVolumeHitsColliderComponent( debpColliderVolume &collider1, debpColliderComponent &collider2,
		debpCollisionResult &result );
	
	/**
	 * Determines if a collider volume hits a collider rigged. The collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the shape1 and bone2 parameters are set
	 * in the result.
	 */
	bool ColliderVolumeHitsColliderRig( debpColliderVolume &collider1, debpColliderRig &collider2,
		debpCollisionResult &result );
	
	/**
	 * Determines if a collider component hits another collider component. The collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the bone1 and bone2 parameters are set in the result.
	 */
	bool ColliderComponentHitsColliderComponent( debpColliderComponent &collider1, debpColliderComponent &collider2,
		debpCollisionResult &result );
	
	/**
	 * Determines if a collider component hits a collider rigged. The collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the bone1 and bone2 parameters are set in the result.
	 */
	bool ColliderComponentHitsColliderRig( debpColliderComponent &collider1, debpColliderRig &collider2,
		debpCollisionResult &result );
	
#if 0
	/**
	 * Determines if a collider volume move hits a collider component. The collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the shape1 and bone2 parameters are set
	 * in the result.
	 */
	bool ColliderVolumeMoveHitsColliderComponent( debpColliderVolume &collider1, const decDVector &displacement,
		debpColliderComponent &collider2, debpCollisionResult &result );
#endif
	
#if 0
	/**
	 * Determines if a collider volume move hits a collider rigged. The collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the shape1 and bone2 parameters are set
	 * in the result.
	 */
	bool ColliderVolumeMoveHitsColliderRig( debpColliderVolume &collider1, const decDVector &displacement,
		debpColliderRig &collider2, debpCollisionResult &result );
#endif
	
#if 0
	/**
	 * Determines if a collider component move hits another collider component. The collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the bone1 and bone2 parameters are set in the result.
	 */
	bool ColliderComponentMoveHitsColliderComponent( debpColliderComponent &collider1, const decDVector &displacement,
		debpColliderComponent &collider2, debpCollisionResult &result );
#endif
	
#if 0
	/**
	 * Determines if a collider component move hits a collider rigged. The collider shapes are supposed to be
	 * transformed relative to the world. If the collider is hit the bone1 and bone2 parameters are set in the result.
	 */
	bool ColliderComponentMoveHitsColliderRig( debpColliderComponent &collider1, const decDVector &displacement,
		debpColliderRig &collider2, debpCollisionResult &result );
#endif
	/*@}*/
};

#endif
