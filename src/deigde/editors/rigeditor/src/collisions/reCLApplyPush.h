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

#ifndef _RECLAPPLYPUSH_H_
#define _RECLAPPLYPUSH_H_

#include "reCLHitList.h"

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>

class decLayerMask;
class deBasePhysicsWorld;
class reRig;
class deCollider;



/**
 * \brief Collider lister applying a push to the closest hit shape in the simulation collider.
 */
class reCLApplyPush : public deBaseScriptingCollider{
private:
	reRig *pRig;
	
	decDVector pRayPosition;
	decDVector pRayDirection;
	
	decVector pPush;
	
	deCollider *pCollider;
	
	deCollider *pClosestCollider;
	int pClosestBone;
	float pClosestDistance;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision listener. */
	reCLApplyPush(reRig *rig);
	/** Cleans up the collision listener. */
	virtual ~reCLApplyPush();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the ray position. */
	inline const decDVector &GetRayPosition() const{return pRayPosition;}
	/** \brief Retrieves the ray direction. */
	inline const decDVector &GetRayDirection() const{return pRayDirection;}
	
	/** Sets the ray. */
	void SetRay(const decDVector &position, const decVector &direction);
	/** Sets the push. */
	void SetPush(const decVector &push);
	/** Sets the collider. */
	void SetCollider(deCollider *collider);
	
	/** \brief Reset visitor. */
	void Reset();
	/** \brief Apply push to the closest shape if any. */
	void ApplyPush();
	
	/**
	 * \brief Helper method applying push to the closest shape if any in the world.
	 * \details Calls Reset(), then RayHits on the world with the provided
	 *          layer maks and then ApplyPush().
	 */
	void ApplyPushIn(deBasePhysicsWorld &phyWorld, const decLayerMask &layerMask);
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
	 * Determines if this collider can be hit by the given collider.
	 */
	virtual bool CanHitCollider(deCollider *owner, deCollider *collider);
	/**
	 * Notifies the scripts that the properties of this collider have changed and
	 * that the attached element has to update. This is usually called after the
	 * collision detection but can also be called multiple times.
	 */
	virtual void ColliderChanged(deCollider *owner);
};

#endif
