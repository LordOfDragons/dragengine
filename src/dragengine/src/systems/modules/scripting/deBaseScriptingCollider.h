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

#ifndef _DEBASESCRIPTINGCOLLIDER_H_
#define _DEBASESCRIPTINGCOLLIDER_H_

#include "../../../dragengine_export.h"

class deRigConstraint;
class deColliderConstraint;
class deCollisionInfo;
class deCollider;
class deComponent;


/**
 * \brief Scripting Module Collider Peer.
 */
class DE_DLL_EXPORT deBaseScriptingCollider{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseScriptingCollider();
	
	/** \brief Clean up peer. */
	virtual ~deBaseScriptingCollider();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/**
	 * \brief Determines the response for a given collision.
	 * 
	 * If deCollider::ertCustom is set the info object has to be updated
	 * by the scripting module with the response to the collision. In
	 * all other cases the info object should not be modified.
	 */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	
	/**
	 * \brief Determines if a collider can be hit.
	 * 
	 * If owner is NULL a ray collision is tested. Otherwise a collider
	 * versus collider collision is tested. If a collision is possible
	 * between the ray and a collider or two colliders true has to be
	 * returned otherwise false.
	 */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	
	/**
	 * \brief The properties of the collider changed.
	 * 
	 * This is called by the physics module after the collision detection.
	 * This can possibly be called multiple times during collision detection.
	 */
	virtual void ColliderChanged( deCollider *owner );
	
	/** \brief A collider constraint has broken. */
	virtual void ColliderConstraintBroke( deCollider *owner, int index, deColliderConstraint *constraint );
	
	/** \brief A collider rig constraint has broken. */
	virtual void RigConstraintBroke( deCollider *owner, int bone, int index, deRigConstraint *constraint );
	/*@}*/
};

#endif
