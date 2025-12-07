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

#ifndef _DEBASEPHASICSTOUCHSENSOR_H_
#define _DEBASEPHASICSTOUCHSENSOR_H_

#include "../../../common/math/decMath.h"

class decShapeList;
class deCollider;
class deBaseScriptingCollider;


/**
 * \brief Physics Module Touch Sensor Peer.
 *
 * Touch sensor peer for the pyhsics module.
 */
class DE_DLL_EXPORT deBasePhysicsTouchSensor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBasePhysicsTouchSensor();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsTouchSensor();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Ignore colliders changed. */
	virtual void IgnoreCollidersChanged();
	
	/** \brief Track enter/leave changed. */
	virtual void TrackEnterLeaveChanged();
	
	/** \brief Enabled changed. */
	virtual void EnabledChanged();
	
	/** \brief Shape changed. */
	virtual void ShapeChanged();
	
	
	
	/** \brief Touch sensor contains no colliders. */
	virtual bool IsEmpty();
	
	/** \brief Number of colliders in touch sensor. */
	virtual int GetColliderCount();
	
	/**
	 * \brief Collider at index in touch sensor.
	 * \param collider Index of the collider retrieve.
	 */
	virtual deCollider *GetColliderAt(int collider);
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/** \brief Test if a point is located inside the collider. */
	virtual bool PointInside(const decDVector &point);
	
	/**
	 * \brief Visit all touching elements with listener.
	 * 
	 * To stop testing set StopTesting in the provided collision information object to true.
	 */
	virtual void AllHits(deBaseScriptingCollider *listener);
	
	/**
	 * \brief Test ray for collision with the element in the given shape.
	 * 
	 * For each collision the collisionResponse function the given listener is called.
	 * To stop testing set StopTesting in the provided collision information object to true.
	 * The distance parameter in the collision response represents the actual distance to the
	 * ray origin along the ray direction.
	 */
	virtual void RayHits(const decDVector &rayOrigin, const decVector &rayDirection,
		deBaseScriptingCollider *listener);
	
	/**
	 * \brief Test collider for collision with scene elements.
	 * 
	 * For each collision the collisionResponse function of the listener assigned to the
	 * collider is called. To stop testing set StopTesting in the provided collision
	 * information object to true.
	 */
	virtual void ColliderHits(deCollider *collider, deBaseScriptingCollider *listener);
	
	/**
	 * \brief Test moving collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	virtual void ColliderMoveHits(deCollider *collider, const decVector &displacement,
		deBaseScriptingCollider *listener);
	
	/**
	 * \brief Test rotating collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	virtual void ColliderRotateHits(deCollider *collider, const decVector &rotation,
		deBaseScriptingCollider *listener);
	
	/**
	 * \brief Test moving and rotating collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	virtual void ColliderMoveRotateHits(deCollider *collider, const decVector &displacement,
		const decVector &rotation, deBaseScriptingCollider *listener);
	/*@}*/
};

#endif
