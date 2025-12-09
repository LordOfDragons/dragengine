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

#ifndef _DEDSCOLLISIONTESTER_H_
#define _DEDSCOLLISIONTESTER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decCollisionFilter.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class deScriptingDragonScript;

class deColliderVolume;
class decShapeList;

class dsValue;
class dsRealObject;



/**
 * \brief Collision tester.
 */
class dedsCollisionTester : public deBaseScriptingCollider, public deObject{
private:
	deScriptingDragonScript &pDS;
	
	deWorld::Ref pWorld;
	deTouchSensor::Ref pTouchSensor;
	
	deCollider::Ref pCollider;
	
	decCollisionFilter pCollisionFilter;
	
	dsValue *pColliderListener;
	bool pHasColliderListener;
	
	decObjectOrderedSet pListIgnoreColliders;
	
	bool pHasCollision;
	float pHitDistance;
	decVector pHitNormal;
	deCollider::Ref pHitCollider;
	int pHitBone;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new collision tester. */
	dedsCollisionTester(deScriptingDragonScript &ds);
	
	/** \brief Create copy of a collision tester. */
	dedsCollisionTester(const dedsCollisionTester &collisionTester);
	
	/** \brief Clean up the collision tester. */
	virtual ~dedsCollisionTester();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief World to use for collision testing or null if not set. */
	inline const deWorld::Ref &GetWorld() const{ return pWorld; }
	
	/** \brief Set world to use for collision testing or null if not set. */
	void SetWorld(deWorld *world);
	
	/** \brief Touch sensor to use for testing collisions or null if not set. */
	inline const deTouchSensor::Ref &GetTouchSensor() const{ return pTouchSensor; }
	
	/** \brief Set touch sensor to use for testing collisions or null if not set. */
	void SetTouchSensor(deTouchSensor *touchSensor);
	
	
	
	/** \brief Set collision testing to use a ray test. */
	void SetCollisionRay();
	
	/** \brief Set collision testing to use a collision volume. */
	void SetCollisionShape(const decShapeList &shapeList);
	
	/** \brief Collider or null if not set. */
	inline const deCollider::Ref &GetCollider() const{ return pCollider; }
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter(const decCollisionFilter &filter);
	
	/** \brief Collider listener used to customize can hit collider or null if not used. */
	dsRealObject *GetColliderListener() const;
	
	/** \brief Set collider listener used to customize can hit collider or null if not used. */
	void SetColliderListener(dsRealObject *object);
	
	/** \brief List of colliders to ignore. */
	inline decObjectOrderedSet &GetListIgnoreColliders(){ return pListIgnoreColliders; }
	inline const decObjectOrderedSet &GetListIgnoreColliders() const{ return pListIgnoreColliders; }
	
	
	
	/** \brief Determine if a collision has been found. */
	inline bool HasCollision() const{ return pHasCollision; }
	
	/** \brief Hit distance. Valid if hasCollision() returns true. */
	inline float GetHitDistance() const{ return pHitDistance; }
	
	/** \brief Hit normal. Valid if hasCollision() returns true. */
	inline const decVector &GetHitNormal() const{ return pHitNormal; }
	
	/** \brief Hit collider or null if none. Valid if hasCollision() returns true. */
	inline const deCollider::Ref &GetHitCollider() const{ return pHitCollider; }
	
	/** \brief Hit collider bone or -1 if none. Valid if hasCollision() returns true. */
	inline int GetHitBone() const{ return pHitBone; }
	
	
	
	/**
	 * \brief Reset collision result.
	 * \details After this call hasCollision() returns false and collision results are undefined.
	 */
	void Reset();
	
	/** \brief Test ray collision. */
	void RayHits(const decDVector &position, const decVector &direction);
	
	/** \brief Test collider hits collision. */
	void ColliderHits(const decDVector &position);
	
	/** \brief Test collider hits collision. */
	void ColliderHits(const decDVector &position, const decQuaternion &orientation);
	
	/** \brief Test collider move hits collision. */
	void ColliderMoveHits(const decDVector &position, const decVector &direction);
	
	/** \brief Test collider move hits collision. */
	void ColliderMoveHits(const decDVector &position, const decQuaternion &orientation,
	const decVector &direction);
	
	/** \brief Test collider rotate hits collision. */
	void ColliderRotateHits(const decDVector &position, const decVector &rotation);
	
	/** \brief Test collider rotate hits collision. */
	void ColliderRotateHits(const decDVector &position, const decQuaternion &orientation,
	const decVector &rotation);
	
	/** \brief Test collider move rotate hits collision. */
	void ColliderMoveRotateHits(const decDVector &position, const decVector &direction,
	const decVector &rotation);
	
	/** \brief Test collider move rotate hits collision. */
	void ColliderMoveRotateHits(const decDVector &position, const decQuaternion &orientation,
	const decVector &direction, const decVector &rotation);
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/**
	 * Determines the response for a given collision. If you specified ertCustom
	 * you have to update the info object with the response to the collision. In
	 * all other cases you do must not modify the info object.
	 */
	virtual void CollisionResponse(deCollider *owner, deCollisionInfo *info);
	
	/**
	 * Determines if a collider can be hit. If this peer is used with a collider
	 * then the script is asked to determine if the two collider can hit each
	 * other. If this peer is used in a ray test owner is NULL and the script
	 * is asked to determine if the ray can hit the collider.
	 * \param owner Collider this peer belongs to or NULL if a ray test is done.
	 * \param collider Collider to test.
	 * \return True if the owner/ray can hit the given collider.
	 */
	virtual bool CanHitCollider(deCollider *owner, deCollider *collider);
	/*@}*/
	
private:
	void pCleanUp();
	void pCopyCollider(deCollider *collider);
};

#endif
