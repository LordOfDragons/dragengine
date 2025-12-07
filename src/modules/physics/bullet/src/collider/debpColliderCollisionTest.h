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

#ifndef _DEBPCOLLIDERCOLLISIONTEST_H_
#define _DEBPCOLLIDERCOLLISIONTEST_H_

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class deColliderCollisionTest;
class debpCollider;



/**
 * \brief Collider collision test object.
 */
class debpColliderCollisionTest : public deBaseScriptingCollider{
private:
	debpCollider &pParentCollider;
	deColliderCollisionTest &pCollisionTest;
	
	decObjectList pCollisionInfo;
	int pCollisionInfoCount;
	bool pSortByDistance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a collider attachment. */
	debpColliderCollisionTest(debpCollider &parentCollider, deColliderCollisionTest &collisionTest);
	
	/** \brief Clean up the collider attachment. */
	virtual ~debpColliderCollisionTest();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the collider collision test. */
	inline deColliderCollisionTest &GetCollisionTest(){ return pCollisionTest; }
	inline const deColliderCollisionTest &GetCollisionTest() const{ return pCollisionTest; }
	
	/** \brief Collider collision test changed. */
	void CollisionTestChanged();
	
	/** \brief Update collision test. */
	void Update();
	
	/** \brief Set collider collision test to test result. */
	void SetCollisionTestResult();
	
	
	
	/** \brief Reset collision result. */
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
};

#endif
