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

#ifndef _DECOLLIDERCOLLISIONTEST_H_
#define _DECOLLIDERCOLLISIONTEST_H_

#include "deCollider.h"
#include "../component/deComponent.h"
#include "../sensor/deTouchSensor.h"
#include "../../deObject.h"
#include "../../common/collection/decObjectList.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decCollisionFilter.h"
#include "../../common/string/decString.h"

class deCollisionInfo;


/**
 * \brief Collider collision test.
 * 
 * The collision information instance is created by the constructor. It is not changed for
 * the lifetime of the object. See GetHasCollision() to determine if the collision
 * information object contains valid collision information.
 * 
 * If ray testing is used (collider is NULL) all CanHitCollider calls are send to the
 * parent collider CanHitCollider call.
 * 
 * The parent collider is ignored during collision tests. It is therefore not required to
 * explicitly exclude it from CanHitCollider call.
 */
class DE_DLL_EXPORT deColliderCollisionTest : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deColliderCollisionTest> Ref;
	
	
private:
	deTouchSensor::Ref pTouchSensor;
	deCollider::Ref pCollider;
	decCollisionFilter pCollisionFilter;
	
	deComponent::Ref pComponent;
	decString pBone;
	decVector pOrigin;
	decQuaternion pOrientation;
	decVector pDirection;
	bool pLocalDirection;
	bool pEnabled;
	
	decObjectList pCollisionInfo;
	decDVector pTestOrigin;
	decQuaternion pTestOrientation;
	decDVector pTestDirection;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create collider collision test. */
	deColliderCollisionTest();
	
	/** \brief Create copy of a collider collision test. */
	deColliderCollisionTest(const deColliderCollisionTest &collisionTester);
	
protected:
	/**
	 * \brief Clean up collider collision test.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deColliderCollisionTest() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Touch sensor to use for testing or NULL to use the parent collider world. */
	inline const deTouchSensor::Ref &GetTouchSensor() const{ return pTouchSensor; }
	
	/** \brief Set touch sensor to use for testing or NULL to use the parent collider world. */
	void SetTouchSensor(deTouchSensor *touchSensor);
	
	
	
	/** \brief Collider or NULL to test using a ray. */
	inline const deCollider::Ref &GetCollider() const{ return pCollider; }
	
	/** \brief Set collider or NULL to test using a ray. */
	void SetCollider(deCollider *collider);
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter(const decCollisionFilter &filter);
	
	
	
	/** \brief Component to use to retrieve bone position from or NULL if not used. */
	inline const deComponent::Ref &GetComponent() const{ return pComponent; }
	
	/** \brief Set component to use to retrieve bone position from or NULL if not used. */
	void SetComponent(deComponent *component);
	
	/** \brief Bone to apply origin to for testing or empty string to use parent collider position. */
	inline const decString &GetBone() const{ return pBone; }
	
	/** \brief Set bone to apply origin to for testing or empty string to use parent collider position. */
	void SetBone(const char *name);
	
	/** \brief Cast origin. */
	inline const decVector &GetOrigin() const{ return pOrigin; }
	
	/** \brief Set cast origin. */
	void SetOrigin(const decVector &origin);
	
	/**
	 * \brief Cast orientation.
	 * \version 1.16
	 */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/**
	 * \brief Set cast orientation.
	 * \version 1.16
	 */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Cast direction. */
	inline const decVector &GetDirection() const{ return pDirection; }
	
	/** \brief Set cast direction. */
	void SetDirection(const decVector &direction);
	
	/** \brief Local direction. */
	inline bool GetLocalDirection() const{ return pLocalDirection; }
	
	/** \brief Set if direction is local. */
	void SetLocalDirection(bool localDirection);
	
	/** \brief Collision test is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if collision test is enabled. */
	void SetEnabled(bool enabled);
	
	
	
	/** \brief Number of collision information. */
	int GetCollisionInfoCount() const;
	
	/**
	 * \brief Get collision information at index.
	 * 
	 * \warning Do not change the content of this object. It is potentially shared.
	 */
	deCollisionInfo *GetCollisionInfoAt(int index) const;
	
	/** \brief Add collision information. */
	void AddCollisionInfo(deCollisionInfo *info);
	
	/** \brief Remove all collision information. */
	void RemoveAllCollisionInfo();
	
	
	
	/** \brief Test origin used by physics module. */
	inline const decDVector &GetTestOrigin() const{ return pTestOrigin; }
	
	/** \brief Set test origin used by physics module. */
	void SetTestOrigin(const decDVector &origin);
	
	/**
	 * \brief Test orientation used by physics module.
	 * \version 1.16
	 */
	inline const decQuaternion &GetTestOrientation() const{ return pTestOrientation; }
	
	/**
	 * \brief Set test orientation used by physics module.
	 * \version 1.16
	 */
	void SetTestOrientation(const decQuaternion &orientation);
	
	/** \brief Test direction used by physics module. */
	inline const decDVector &GetTestDirection() const{ return pTestDirection; }
	
	/** \brief Set test direction used by physics module. */
	void SetTestDirection(const decDVector &direction);
	/*@}*/
	
	
	
	/** \name Operator */
	/*@{*/
	/** \brief Copy a collider collision test to this collider collision test. */
	deColliderCollisionTest &operator=(const deColliderCollisionTest &collisionTest);
	/*@}*/
};

#endif
