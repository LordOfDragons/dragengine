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

#ifndef _DEBPTOUCHSENSOR_H_
#define _DEBPTOUCHSENSOR_H_

#include "../shape/debpShapeList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerOrderedSet.h>
#include <dragengine/systems/modules/physics/deBasePhysicsTouchSensor.h>

class debpCollider;
class deTouchSensor;
class debpWorld;
class dePhysicsBullet;
class debpGhostObject;
class deDebugDrawer;
class deDebugDrawerShape;



/**
 * \brief Bullet touch sensor peer.
 */
class debpTouchSensor : public deBasePhysicsTouchSensor{
private:
	dePhysicsBullet &pBullet;
	deTouchSensor &pTouchSensor;
	
	debpWorld *pParentWorld;
	decDVector pMinExtend;
	decDVector pMaxExtend;
	decDMatrix pMatrix;
	decDMatrix pInvMatrix;
	
	decDVector pBasicMinExtend;
	decDVector pBasicMaxExtend;
	
	bool pDirtyMatrix;
	bool pDirtyExtends;
	
	debpShapeList pShape;
	decPointerOrderedSet pTouchingColliders;
	decPointerOrderedSet pLeavingColliders;
	debpGhostObject *pGhostObject;
	
	deDebugDrawer *pDebugDrawer;
	deDebugDrawerShape *pDDSShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	debpTouchSensor( dePhysicsBullet &bullet, deTouchSensor &touchSensor );
	
	/** \brief Clean up peer. */
	virtual ~debpTouchSensor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline dePhysicsBullet &GetBullet() const{ return pBullet; }
	
	/** \brief Touch sensor. */
	inline deTouchSensor &GetTouchSensor() const{ return pTouchSensor; }
	
	
	
	/** \brief Shape. */
	inline const debpShapeList &GetShape() const{ return pShape; }
	
	/** \brief Retrieves the ghost object. */
	inline debpGhostObject *GetGhostObject() const{ return pGhostObject; }
	
	/** \brief Retrieves the list of touching colliders. */
	inline decPointerOrderedSet &GetTouchingColliders(){ return pTouchingColliders; }
	inline const decPointerOrderedSet &GetTouchingColliders() const{ return pTouchingColliders; }
	
	/** \brief Retrieves the list of leaving colliders. */
	inline decPointerOrderedSet &GetLeavingColliders(){ return pLeavingColliders; }
	inline const decPointerOrderedSet &GetLeavingColliders() const{ return pLeavingColliders; }
	
	
	
	/** \brief Parent world or \em NULL. */
	inline debpWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL. */
	void SetParentWorld( debpWorld *parentWorld );
	
	/** \brief Minimum extend. */
	const decDVector &GetMinimumExtend();
	
	/** \brief Maximum extend. */
	const decDVector &GetMaximumExtend();
	
	/** \brief Matrix. */
	const decDMatrix &GetMatrix();
	
	/** \brief Inverse matrix. */
	const decDMatrix &GetInverseMatrix();
	
	/** \brief Apply accumulated changes. */
	void ApplyChanges();
	
	/** \brief Mark ghost object dirty to force an update. */
	void MarkGhostObjectDirty();
	
	/** \brief Touch sensor and collider collide. */
	bool Collides( const debpCollider &collider ) const;
	
	/** \brief Touch sensor and collider do not collide. */
	bool CollidesNot( const debpCollider &collider ) const;
	
	/** \brief Determines if a collider touches the shape. */
	bool TestCollider( debpCollider *collider );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Layer mask changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Ignore colliders changed. */
	virtual void IgnoreCollidersChanged();
	
	/** \brief Track enter/leave changed. */
	virtual void TrackEnterLeaveChanged();
	
	/** \brief Enabled changed. */
	virtual void EnabledChanged();
	
	/** \brief Touch sensor contains no colliders. */
	virtual bool IsEmpty();
	
	/** \brief Number of colliders in touch sensor. */
	virtual int GetColliderCount();
	
	/**
	 * \brief Collider at index in touch sensor.
	 * \param collider Index of the collider retrieve.
	 */
	virtual deCollider *GetColliderAt( int collider );
	
	/** \brief Shape changed. */
	virtual void ShapeChanged();
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/** \brief Test if a point is located inside the collider. */
	virtual bool PointInside( const decDVector &point );
	
	/**
	 * \brief Visit all touching elements with listener.
	 * 
	 * To stop testing set StopTesting in the provided collision information object to true.
	 */
	virtual void AllHits( deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test ray for collision with the element in the given shape.
	 * 
	 * For each collision the collisionResponse function the given listener is called.
	 * To stop testing set StopTesting in the provided collision information object to true.
	 * The distance parameter in the collision response represents the actual distance to the
	 * ray origin along the ray direction.
	 */
	virtual void RayHits( const decDVector &rayOrigin, const decVector &rayDirection,
	deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test collider for collision with scene elements.
	 * 
	 * For each collision the collisionResponse function of the listener assigned to the
	 * collider is called. To stop testing set StopTesting in the provided collision
	 * information object to true.
	 */
	virtual void ColliderHits( deCollider *collider, deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test moving collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	virtual void ColliderMoveHits( deCollider *collider, const decVector &displacement,
	deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test rotating collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	virtual void ColliderRotateHits( deCollider *collider, const decVector &rotation,
	deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test moving and rotating collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	virtual void ColliderMoveRotateHits( deCollider *collider, const decVector &displacement,
	const decVector &rotation, deBaseScriptingCollider *listener );
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Debug drawer or \em NULL if not activated .*/
	inline deDebugDrawer *GetDebugDrawer() const{ return pDebugDrawer; }
	
	/** \brief Debug drawer shape or \em NULL if not ativated. */
	inline deDebugDrawerShape *GetDDSShape() const{ return pDDSShape; }
	
	/** \brief Update debug drawer if developer mode is enabled. */
	void UpdateDebugDrawer();
	
	/**
	 * \brief Update debug drawer shape shape.
	 * \details Called after creating debug drawer or if the collider subclass requires an update.
	 */
	void UpdateDDSShape();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateMatrix();
	void pUpdateExtends();
	void pCalculateBasicExtends();
	void pClearTracking();
};

#endif
