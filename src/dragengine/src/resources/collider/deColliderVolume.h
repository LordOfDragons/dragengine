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

#ifndef _DECOLLIDERVOLUME_H_
#define _DECOLLIDERVOLUME_H_

#include "deCollider.h"
#include "../../common/shape/decShapeList.h"


/**
 * \brief Volume collider.
 *
 * Colliders are used by the physics module to provide collision
 * detection and optional collision response. After the collision
 * detection stage is done each Collider contains the new state
 * of motion. The user can then update the game state depending
 * on the changes. The scripting peer receives all relevant events
 * during a collision detection run. The Collider is defined by a
 * list of Collision Volume objects. The final test volume is
 * considered to be the boolean union of all the volumes.
 */
class deColliderVolume : public deCollider{
private:
	decShapeList pShapes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create volume collider. */
	deColliderVolume( deColliderManager *manager );
	
protected:
	/**
	 * \brief Clean up volume collider.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deColliderVolume();
	/*@}*/
	
	
	
public:
	/** \name Shapes */
	/*@{*/
	/** \brief Shapes. */
	inline const decShapeList &GetShapes() const{ return pShapes; }
	
	/** \brief Set shapes. */
	void SetShapes( const decShapeList &shapes );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit collider. */
	virtual void Visit( deColliderVisitor &visitor );
	/*@}*/
};

#endif
