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

#ifndef _DEDSCOLLIDERLISTENERCLOSEST_H_
#define _DEDSCOLLIDERLISTENERCLOSEST_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class deScriptingDragonScript;

class dsValue;
class dsRealObject;

class deCollisionInfo;
class deCollider;



/**
 * \brief Collider listener closest hit.
 * 
 * Tests for moving collisions storing the parameters of the closest
 * collision using wrapped collider. Once finished the result is
 * send to the wrapped collider.
 * 
 */
class dedsColliderListenerClosest : public deBaseScriptingCollider{
private:
	deScriptingDragonScript &pDS;
	
	dsValue *pListener;
	bool pHasListener;
	
	deCollider *pOwner;
	bool pHasCollision;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new CLASS. */
	dedsColliderListenerClosest( deScriptingDragonScript &ds );
	/** Cleans up the CLASS. */
	virtual ~dedsColliderListenerClosest();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Sets the wrapped listener. */
	void SetListener( dsRealObject *object );
	/** Reset. */
	void Reset();
	/**
	 * Notifies the listener about the found collision. If a collision is found
	 * the collisionResponse method on the listener is called. If not collision
	 * has been found no action is taken.
	 */
	void NotifyListener();
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
};

#endif
