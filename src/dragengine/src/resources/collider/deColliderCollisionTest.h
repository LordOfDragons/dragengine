/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECOLLIDERCOLLISIONTEST_H_
#define _DECOLLIDERCOLLISIONTEST_H_

#include "deColliderReference.h"
#include "../component/deComponentReference.h"
#include "../sensor/deTouchSensorReference.h"
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
class deColliderCollisionTest : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deColliderCollisionTest> Ref;
	
	
	
private:
	deTouchSensorReference pTouchSensor;
	deColliderReference pCollider;
	decCollisionFilter pCollisionFilter;
	
	deComponentReference pComponent;
	decString pBone;
	decVector pOrigin;
	decVector pDirection;
	bool pLocalDirection;
	bool pEnabled;
	
	decObjectList pCollisionInfo;
	decDVector pTestOrigin;
	decDVector pTestDirection;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create collider collision test. */
	deColliderCollisionTest();
	
	/** \brief Create copy of a collider collision test. */
	deColliderCollisionTest( const deColliderCollisionTest &collisionTester );
	
protected:
	/**
	 * \brief Clean up collider collision test.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deColliderCollisionTest();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Touch sensor to use for testing or NULL to use the parent collider world. */
	inline deTouchSensor *GetTouchSensor() const{ return pTouchSensor; }
	
	/** \brief Set touch sensor to use for testing or NULL to use the parent collider world. */
	void SetTouchSensor( deTouchSensor *touchSensor );
	
	
	
	/** \brief Collider or NULL to test using a ray. */
	inline deCollider *GetCollider() const{ return pCollider; }
	
	/** \brief Set collider or NULL to test using a ray. */
	void SetCollider( deCollider *collider );
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter( const decCollisionFilter &filter );
	
	
	
	/** \brief Component to use to retrieve bone position from or NULL if not used. */
	inline deComponent *GetComponent() const{ return pComponent; }
	
	/** \brief Set component to use to retrieve bone position from or NULL if not used. */
	void SetComponent( deComponent *component );
	
	/** \brief Bone to apply origin to for testing or empty string to use parent collider position. */
	inline const decString &GetBone() const{ return pBone; }
	
	/** \brief Set bone to apply origin to for testing or empty string to use parent collider position. */
	void SetBone( const char *name );
	
	/** \brief Cast origin. */
	inline const decVector &GetOrigin() const{ return pOrigin; }
	
	/** \brief Set cast origin. */
	void SetOrigin( const decVector &origin );
	
	/** \brief Cast direction. */
	inline const decVector &GetDirection() const{ return pDirection; }
	
	/** \brief Set cast direction. */
	void SetDirection( const decVector &direction );
	
	/** \brief Local direction. */
	inline bool GetLocalDirection() const{ return pLocalDirection; }
	
	/** \brief Set if direction is local. */
	void SetLocalDirection( bool localDirection );
	
	/** \brief Collision test is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if collision test is enabled. */
	void SetEnabled( bool enabled );
	
	
	
	/** \brief Number of collision information. */
	int GetCollisionInfoCount() const;
	
	/**
	 * \brief Get collision information at index.
	 * 
	 * \warning Do not change the content of this object. It is potentially shared.
	 */
	deCollisionInfo *GetCollisionInfoAt( int index ) const;
	
	/** \brief Add collision information. */
	void AddCollisionInfo( deCollisionInfo *info );
	
	/** \brief Remove all collision information. */
	void RemoveAllCollisionInfo();
	
	
	
	/** \brief Test origin used by physics module. */
	inline const decDVector &GetTestOrigin() const{ return pTestOrigin; }
	
	/** \brief Set test origin used by physics module. */
	void SetTestOrigin( const decDVector &testOrigin );
	
	/** \brief Test direction used by physics module. */
	inline const decDVector &GetTestDirection() const{ return pTestDirection; }
	
	/** \brief Set test direction used by physics module. */
	void SetTestDirection( const decDVector &testDirection );
	/*@}*/
	
	
	
	/** \name Operator */
	/*@{*/
	/** \brief Copy a collider collision test to this collider collision test. */
	deColliderCollisionTest &operator=( const deColliderCollisionTest &collisionTest );
	/*@}*/
};

#endif
