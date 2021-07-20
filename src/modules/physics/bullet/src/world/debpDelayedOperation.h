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

#ifndef _DEBPDELAYEDOPERATION_H_
#define _DEBPDELAYEDOPERATION_H_

#include <dragengine/common/collection/decPointerOrderedSet.h>

class btCollisionObject;
class btCollisionShape;
class btRigidBody;

class debpCollisionObject;
class debpCollisionWorld;


/**
 * \brief Delayed operations for world objects.
 * 
 * During collision testing and processing objects can be added to world, removed from
 * the world or changed in a way bullet can not deal with. For example removing objects
 * while bullet is internally looping over the world can result in segmentation faults
 * or other problems. If such a problematic call is conducted the world is put into a
 * locked state where potentially dangerous changes have to be registered to be applied
 * once the locked state is left. The user is responsible to check if the world is
 * currently locked (state stored in this class) and thus operations have to be put
 * into the delayed operations instead. The world locks and unlocks the delayed operation
 * object. Locks are cummulative so only after all locks are released the pending
 * operations are applied.
 */
class debpDelayedOperation{
private:
	debpCollisionWorld &pWorld;
	int pLockCount;
	decPointerOrderedSet pListAddRigidBody;
	decPointerOrderedSet pListRemoveRigidBody;
	decPointerOrderedSet pListAddCollisionObject;
	decPointerOrderedSet pListRemoveCollisionObject;
	decPointerOrderedSet pListClearFromBroadphase;
	btCollisionShape *pDummyShape;
	debpCollisionObject *pDummyCollisionObject;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create delayed operation object. */
	debpDelayedOperation( debpCollisionWorld &world );
	
	/** \brief Clean up delayed operation object. */
	~debpDelayedOperation();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief World is locked. */
	inline bool IsLocked() const{ return pLockCount > 0; }
	
	/**
	 * \brief Lock parent world.
	 * 
	 * The world can be locked multiple times. Each time the lock count is increased
	 * by one. The lock is fully released once the lock count reaches 0 again. Make
	 * sure each lock is paired with a matching unlock.
	 */
	void Lock();
	
	/**
	 * \brief Unlock parent world.
	 * 
	 * The world can be locked multiple times. Each time the lock count is increased
	 * by one. The lock is fully released once the lock count reaches 0 again. Make
	 * sure each lock is paired with a matching unlock. Once a lock count of 0 is
	 * reached all delayed operations are applied.
	 */
	void Unlock();
	
	
	
	/**
	 * \brief Add rigid body to be added to the world.
	 * 
	 * The pointer to the rigid body is not held by the delayed operation object.
	 */
	void AddRigidBody( btRigidBody *rigidBody );
	
	/**
	 * \brief Remove rigid body from the world.
	 * 
	 * The pointer to the rigid body is taken over by the delayed operation object.
	 * Do not delete the object after handing it over. If the rigid body has been
	 * already added to the list of physics bodies to be added to the world the physics
	 * body is removed from the list and deleted without listing it for later removal.
	 * the collision shape is replaced with a temporary one to avoid problems. If the
	 * shape is not shared deleted it before handing it over.
	 */
	void RemoveRigidBody( btRigidBody *rigidBody );
	
	/**
	 * \brief Add collision object to be added to the world.
	 * 
	 * The pointer to the collision object is not held by the delayed operation object.
	 */
	void AddCollisionObject( btCollisionObject *collisionObject );
	
	/**
	 * \brief Remove collision object from the world.
	 * 
	 * The pointer to the collision object is taken over by the delayed operation object.
	 * Do not delete the object after handing it over. If the collision object has been
	 * already added to the list of physics bodies to be added to the world the physics
	 * body is removed from the list and deleted without listing it for later removal.
	 * the collision shape is replaced with a temporary one to avoid problems. If the
	 * shape is not shared deleted it before handing it over.
	 */
	void RemoveCollisionObject( btCollisionObject *collisionObject );
	
	/**
	 * \brief Clear collision object from broadphase.
	 * 
	 * The pointer to the collision object is not held by the delayed operation object.
	 */
	void AddClearFromBroadphase( btCollisionObject *collisionObject );
	/*@}*/
	
	
	
private:
	void pApplyPendingOperations();
	void pClearFromBroadphase();
	void pRemoveCollisionObjects();
	void pRemoveRigidBodies();
	void pAddRigidBodies();
	void pAddCollisionObjects();
};

#endif
