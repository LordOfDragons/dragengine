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

#ifndef _DETOUCHSENSOR_H_
#define _DETOUCHSENSOR_H_

#include "../deResource.h"
#include "../../common/shape/decShapeList.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decCollisionFilter.h"
#include "../../common/collection/decObjectOrderedSet.h"

class deTouchSensorManager;
class deBasePhysicsTouchSensor;
class deBaseScriptingTouchSensor;
class deBaseScriptingCollider;
class decShape;
class deWorld;
class deCollider;


/**
 * \brief Touch Sensor Resource.
 *
 * Touch sensors detect the presence of colliders in a set of
 * collision shapes. The touch sensor groups in fact multiple
 * sensors in one object. This grouping improves performance
 * since the shapes share a close spatial relationship which
 * reduces greatly the number of collision tests required.
 * Whenever a collider enters or leaves one of the
 * shapes a notification is send to the scripting module peer.
 * The touch sensor itself does not store the colliders and
 * faces in contact with any of the shapes. This is
 * the responsability of the scripting module peer. The reason
 * for this design choice is that game scripts usually require
 * only a subset of the encountered colliders for
 * further processing. In addition actual game actions tend to
 * be linked to colliders entering or leaving a
 * given location in space. Iterating a list each frame update
 * would be too time consuming in large worlds. Like colliders
 * touch sensors too use a layer mask and group number to
 * determine what colliders can be tracked.
 */
class DE_DLL_EXPORT deTouchSensor : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deTouchSensor> Ref;
	
	
private:
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pDirection;
	
	decCollisionFilter pCollisionFilter;
	decObjectOrderedSet pIgnoreColliders;
	
	bool pTrackEnterLeave;
	bool pEnabled;
	
	decShapeList pShape;
	
	deBasePhysicsTouchSensor *pPeerPhysics;
	deBaseScriptingTouchSensor *pPeerScripting;
	
	deWorld *pParentWorld;
	deTouchSensor *pLLWorldPrev;
	deTouchSensor *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create touch sensor. */
	deTouchSensor( deTouchSensorManager *manager );
	
protected:
	/**
	 * \brief Clean up lumimeter.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deTouchSensor();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter( const decCollisionFilter &collisionFilter );
	
	/** \brief Touch sensor tracks objects entering and leaving shapes. */
	inline bool GetTrackEnterLeave() const{ return pTrackEnterLeave; }
	
	/** \brief Set if touch sensor tracks objects entering and leaving shapes. */
	void SetTrackEnterLeave( bool trackEnterLeave );
	
	/** \brief Touch sensor is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if touch sensor is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Shape. */
	inline const decShapeList &GetShape() const{ return pShape; }
	
	/** \brief Set shape. */
	void SetShape( const decShapeList &shape );
	
	
	
	/** \brief Touch sensor contains colliders. */
	bool IsEmpty() const;
	
	/** \brief Number of colliders in touch sensor. */
	int GetColliderCount() const;
	
	/**
	 * \brief Collider at index in touch sensor.
	 * \param collider Index of the collider retrieve from the shape.
	 */
	deCollider *GetColliderAt( int collider ) const;
	
	/**
	 * \brief Notify scripting module peer collider entered touch sensor.
	 * \param collider Collider entering the shape.
	 */
	void NotifyColliderEntered( deCollider *collider );
	
	/**
	 * \brief Notify scripting module peer collider left touch sensor.
	 * \param collider Collider entering the shape.
	 */
	void NotifyColliderLeft( deCollider *collider );
	/*@}*/
	
	
	
	/** \name Ignore colliders */
	/*@{*/
	/** \brief Number of colliders to ignore. */
	int GetIgnoreColliderCount() const;
	
	/**
	 * \brief Collider to ignore at index.
	 * \throws deeInvalidParam \em index is less than 0.
	 * \throws deeInvalidParam \em index is greater or equal than GetIgnoreColliderCount()-1.
	 */
	deCollider *GetIgnoreColliderAt( int index ) const;
	
	/** \brief Collider to ignore is present. */
	bool HasIgnoreCollider( deCollider *collider ) const;
	
	/**
	 * \brief Add collider to ignore.
	 * \throws deeInvalidParam \em collider is present.
	 */
	void AddIgnoreCollider( deCollider *collider );
	
	/**
	 * \brief Remove collider to ignore.
	 * \throws deeInvalidParam \em collider is absent.
	 */
	void RemoveIgnoreCollider( deCollider *collider );
	
	/** \brief Remove all colliders to ignore. */
	void RemoveAllIgnoreColliders();
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/** \brief Test if a point is located inside the touch sensor. */
	bool PointInside( const decDVector &point );
	
	/**
	 * \brief Visit all touching elements with listener.
	 * 
	 * To stop testing set StopTesting in the provided collision information object to true.
	 */
	void AllHits( deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test ray for collision with the element in the given shape.
	 * 
	 * For each collision the collisionResponse function the given listener is called.
	 * To stop testing set StopTesting in the provided collision information object to true.
	 * The distance parameter in the collision response represents the actual distance to the
	 * ray origin along the ray direction.
	 */
	void RayHits( const decDVector &rayOrigin, const decVector &rayDirection,
	deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test collider for collision with scene elements.
	 * 
	 * For each collision the collisionResponse function of the listener assigned to the
	 * collider is called. To stop testing set StopTesting in the provided collision
	 * information object to true.
	 */
	void ColliderHits( deCollider *collider, deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test moving collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	void ColliderMoveHits( deCollider *collider, const decVector &displacement,
	deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test rotating collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	void ColliderRotateHits( deCollider *collider, const decVector &rotation,
	deBaseScriptingCollider *listener );
	
	/**
	 * \brief Test moving and rotating collider for collision with scene elements.
	 * 
	 * For each collision starting with the earliest the collisionResponse function of the
	 * listener assigned to the collider is called. To stop testing set StopTesting in the
	 * provided collision information object to true.
	 */
	void ColliderMoveRotateHits( deCollider *collider, const decVector &displacement,
	const decVector &rotation, deBaseScriptingCollider *listener );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Physics system peer. */
	inline deBasePhysicsTouchSensor *GetPeerPhysics() const{ return pPeerPhysics; }
	
	/** \brief Set physics system peer. */
	void SetPeerPhysics( deBasePhysicsTouchSensor *peer );
	
	/** \brief Scripting system peer. */
	inline deBaseScriptingTouchSensor *GetPeerScripting() const{ return pPeerScripting; }
	
	/** \brief Set scripting system peer. */
	void SetPeerScripting( deBaseScriptingTouchSensor *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous touch sensor in the parent world linked list. */
	inline deTouchSensor *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next touch sensor in the parent world linked list. */
	void SetLLWorldPrev( deTouchSensor *touchSensor );
	
	/** \brief Next touch sensor in the parent world linked list. */
	inline deTouchSensor *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next touch sensor in the parent world linked list. */
	void SetLLWorldNext( deTouchSensor *touchSensor );
	/*@}*/
};

#endif
