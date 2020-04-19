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
