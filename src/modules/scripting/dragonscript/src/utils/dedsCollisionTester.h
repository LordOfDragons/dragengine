/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DEDSCOLLISIONTESTER_H_
#define _DEDSCOLLISIONTESTER_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decCollisionFilter.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class deScriptingDragonScript;

class deCollider;
class deColliderVolume;
class decShapeList;
class deTouchSensor;
class deWorld;

class dsValue;
class dsRealObject;



/**
 * \brief Collision tester.
 */
class dedsCollisionTester : public deBaseScriptingCollider, public deObject{
private:
	deScriptingDragonScript &pDS;
	
	deWorld *pWorld;
	deTouchSensor *pTouchSensor;
	
	deCollider *pCollider;
	
	decCollisionFilter pCollisionFilter;
	
	dsValue *pColliderListener;
	bool pHasColliderListener;
	
	decObjectOrderedSet pListIgnoreColliders;
	
	bool pHasCollision;
	float pHitDistance;
	decVector pHitNormal;
	deCollider *pHitCollider;
	int pHitBone;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new collision tester. */
	dedsCollisionTester( deScriptingDragonScript &ds );
	
	/** \brief Create copy of a collision tester. */
	dedsCollisionTester( const dedsCollisionTester &collisionTester );
	
	/** \brief Clean up the collision tester. */
	virtual ~dedsCollisionTester();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief World to use for collision testing or null if not set. */
	inline deWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world to use for collision testing or null if not set. */
	void SetWorld( deWorld *world );
	
	/** \brief Touch sensor to use for testing collisions or null if not set. */
	inline deTouchSensor *GetTouchSensor() const{ return pTouchSensor; }
	
	/** \brief Set touch sensor to use for testing collisions or null if not set. */
	void SetTouchSensor( deTouchSensor *touchSensor );
	
	
	
	/** \brief Set collision testing to use a ray test. */
	void SetCollisionRay();
	
	/** \brief Set collision testing to use a collision volume. */
	void SetCollisionShape( const decShapeList &shapeList );
	
	/** \brief Collider or null if not set. */
	inline deCollider *GetCollider() const{ return pCollider; }
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter( const decCollisionFilter &filter );
	
	/** \brief Collider listener used to customize can hit collider or null if not used. */
	dsRealObject *GetColliderListener() const;
	
	/** \brief Set collider listener used to customize can hit collider or null if not used. */
	void SetColliderListener( dsRealObject *object );
	
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
	inline deCollider *GetHitCollider() const{ return pHitCollider; }
	
	/** \brief Hit collider bone or -1 if none. Valid if hasCollision() returns true. */
	inline int GetHitBone() const{ return pHitBone; }
	
	
	
	/**
	 * \brief Reset collision result.
	 * \details After this call hasCollision() returns false and collision results are undefined.
	 */
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
	
private:
	void pCleanUp();
	void pCopyCollider( deCollider *collider );
};

#endif
