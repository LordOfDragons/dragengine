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
	debpColliderCollisionTest( debpCollider &parentCollider, deColliderCollisionTest &collisionTest );
	
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
	void RayHits( const decDVector &position, const decVector &direction );
	
	/** \brief Test collider hits collision. */
	void ColliderHits( const decDVector &position );
	
	/** \brief Test collider hits collision. */
	void ColliderHits( const decDVector &position, const decQuaternion &orientation );
	
	/** \brief Test collider move hits collision. */
	void ColliderMoveHits( const decDVector &position, const decVector &direction );
	
	/** \brief Test collider move hits collision. */
	void ColliderMoveHits( const decDVector &position, const decQuaternion &orientation,
	const decVector &direction );
	
	/** \brief Test collider rotate hits collision. */
	void ColliderRotateHits( const decDVector &position, const decVector &rotation );
	
	/** \brief Test collider rotate hits collision. */
	void ColliderRotateHits( const decDVector &position, const decQuaternion &orientation,
	const decVector &rotation );
	
	/** \brief Test collider move rotate hits collision. */
	void ColliderMoveRotateHits( const decDVector &position, const decVector &direction,
	const decVector &rotation );
	
	/** \brief Test collider move rotate hits collision. */
	void ColliderMoveRotateHits( const decDVector &position, const decQuaternion &orientation,
	const decVector &direction, const decVector &rotation );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/**
	 * Determines the response for a given collision. If you specified ertCustom
	 * you have to update the info object with the response to the collision. In
	 * all other cases you do must not modify the info object.
	 */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	
	/**
	 * Determines if a collider can be hit. If this peer is used with a collider
	 * then the script is asked to determine if the two collider can hit each
	 * other. If this peer is used in a ray test owner is NULL and the script
	 * is asked to determine if the ray can hit the collider.
	 * \param owner Collider this peer belongs to or NULL if a ray test is done.
	 * \param collider Collider to test.
	 * \return True if the owner/ray can hit the given collider.
	 */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	/*@}*/
};

#endif
