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

#ifndef _DEBPTOUCHSENSOR_H_
#define _DEBPTOUCHSENSOR_H_

#include "../shape/debpShapeList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerOrderedSet.h>
#include <dragengine/systems/modules/physics/deBasePhysicsTouchSensor.h>
#include <dragengine/src/resources/debug/deDebugDrawer.h>

class debpCollider;
class deTouchSensor;
class debpWorld;
class dePhysicsBullet;
class debpGhostObject;
class deDebugDrawerShape;



/**
 * Bullet touch sensor peer.
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
	
	deDebugDrawer::Ref pDebugDrawer;
	deDebugDrawerShape *pDDSShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	debpTouchSensor(dePhysicsBullet &bullet, deTouchSensor &touchSensor);
	
	/** Clean up peer. */
	virtual ~debpTouchSensor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline dePhysicsBullet &GetBullet() const{ return pBullet; }
	
	/** Touch sensor. */
	inline deTouchSensor &GetTouchSensor() const{ return pTouchSensor; }
	
	
	
	/** Shape. */
	inline const debpShapeList &GetShape() const{ return pShape; }
	
	/** Retrieves the ghost object. */
	inline debpGhostObject *GetGhostObject() const{ return pGhostObject; }
	
	/** Retrieves the list of touching colliders. */
	inline decPointerOrderedSet &GetTouchingColliders(){ return pTouchingColliders; }
	inline const decPointerOrderedSet &GetTouchingColliders() const{ return pTouchingColliders; }
	
	/** Retrieves the list of leaving colliders. */
	inline decPointerOrderedSet &GetLeavingColliders(){ return pLeavingColliders; }
	inline const decPointerOrderedSet &GetLeavingColliders() const{ return pLeavingColliders; }
	
	
	
	/** Parent world or \em NULL. */
	inline debpWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world or \em NULL. */
	void SetParentWorld(debpWorld *parentWorld);
	
	/** Minimum extend. */
	const decDVector &GetMinimumExtend();
	
	/** Maximum extend. */
	const decDVector &GetMaximumExtend();
	
	/** Matrix. */
	const decDMatrix &GetMatrix();
	
	/** Inverse matrix. */
	const decDMatrix &GetInverseMatrix();
	
	/** Apply accumulated changes. */
	void ApplyChanges();
	
	/** Mark ghost object dirty to force an update. */
	void MarkGhostObjectDirty();
	
	/** Touch sensor and collider collide. */
	bool Collides(const debpCollider &collider) const;
	
	/** Touch sensor and collider do not collide. */
	bool CollidesNot(const debpCollider &collider) const;
	
	/** Determines if a collider touches the shape. */
	bool TestCollider(debpCollider *collider);
	
	/** Remove collider immediately. For use by debpCollider only during cleaning up. */
	void RemoveColliderImmediately(debpCollider *collider);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Orientation changed. */
	virtual void OrientationChanged();
	
	/** Layer mask changed. */
	virtual void CollisionFilterChanged();
	
	/** Ignore colliders changed. */
	virtual void IgnoreCollidersChanged();
	
	/** Track enter/leave changed. */
	virtual void TrackEnterLeaveChanged();
	
	/** Enabled changed. */
	virtual void EnabledChanged();
	
	/** Touch sensor contains no colliders. */
	virtual bool IsEmpty();
	
	/** Number of colliders in touch sensor. */
	virtual int GetColliderCount();
	
	/**
	 * Collider at index in touch sensor.
	 * \param collider Index of the collider retrieve.
	 */
	virtual deCollider *GetColliderAt(int collider);
	
	/** Shape changed. */
	virtual void ShapeChanged();
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/** Test if a point is located inside the collider. */
	virtual bool PointInside(const decDVector &point);
	
	/**
	 * Visit all touching elements with listener.
	 * 
	 * To stop testing set StopTesting in the provided collision information object to true.
	 */
	virtual void AllHits(deBaseScriptingCollider *listener);
	
	/**
	 * Test ray for collision with the element in the given shape.
	 * 
	 * For each collision the collisionResponse function the given listener is called.
	 * To stop testing set StopTesting in the provided collision information object to true.
	 * The distance parameter in the collision response represents the actual distance to the
	 * ray origin along the ray direction.
	 */
	virtual void RayHits(const decDVector &rayOrigin, const decVector &rayDirection,
	deBaseScriptingCollider *listener);
	
	/**
	 * Test collider for collision with scene elements.
	 * 
	 * For each collision the collisionResponse function of the listener assigned to the
	 * collider is called. To stop testing set StopTesting in the provided collision
	 * information object to true.
	 */
	virtual void ColliderHits(deCollider *collider, deBaseScriptingCollider *listener);
	
	/**
	 * Test moving collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	virtual void ColliderMoveHits(deCollider *collider, const decVector &displacement,
	deBaseScriptingCollider *listener);
	
	/**
	 * Test rotating collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	virtual void ColliderRotateHits(deCollider *collider, const decVector &rotation,
	deBaseScriptingCollider *listener);
	
	/**
	 * Test moving and rotating collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	virtual void ColliderMoveRotateHits(deCollider *collider, const decVector &displacement,
	const decVector &rotation, deBaseScriptingCollider *listener);
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** Debug drawer or \em NULL if not activated .*/
	inline const deDebugDrawer::Ref &GetDebugDrawer() const{ return pDebugDrawer; }
	
	/** Debug drawer shape or \em NULL if not ativated. */
	inline deDebugDrawerShape *GetDDSShape() const{ return pDDSShape; }
	
	/** Update debug drawer if developer mode is enabled. */
	void UpdateDebugDrawer();
	
	/**
	 * Update debug drawer shape shape.
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
	void pColliderHitsLocked(deCollider &collider, deBaseScriptingCollider &listener);
};

#endif
