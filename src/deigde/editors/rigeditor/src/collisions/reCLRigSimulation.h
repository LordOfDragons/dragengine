/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _RECLRIGSIMULATION_H_
#define _RECLRIGSIMULATION_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class reRig;



/**
 * \brief Rig simulation collision listener.
 * 
 * Collision listenere used for simulating a rig. Collisions with selection
 * colliders are not allowed. All other collisions are allowed. Important
 * to know is that the rig handed to this class is not held.
 * 
 */
class reCLRigSimulation : public deBaseScriptingCollider{
private:
	reRig &pRig;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create collision listener. */
	reCLRigSimulation( reRig &rig );
	
	/** \brief Clean up collision listener. */
	virtual ~reCLRigSimulation();
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
	 * Determines if this collider can be hit by the given collider.
	 */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	
	/**
	 * Notifies the scripts that the properties of this collider have changed and
	 * that the attached element has to update. This is usually called after the
	 * collision detection but can also be called multiple times.
	 */
	virtual void ColliderChanged( deCollider *owner );
};

#endif
