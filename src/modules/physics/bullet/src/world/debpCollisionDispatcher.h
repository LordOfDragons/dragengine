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

#ifndef _DEBPCOLLISIONDISPATCHER_H_
#define _DEBPCOLLISIONDISPATCHER_H_

#include <dragengine/common/math/decMath.h>

#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"

class btCollisionConfiguration;
class debpSharedCollisionFiltering;



/**
 * \brief Collision dispatcher.
 * 
 * Enhances the CollisionDispatcher to determine the need for collision detection and
 * response using the callback functions build into the scripting peers.
 */
class debpCollisionDispatcher : public btCollisionDispatcher{
private:
	debpSharedCollisionFiltering &pCollisionFiltering;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dispatcher. */
	debpCollisionDispatcher( debpSharedCollisionFiltering &collisionFiltering,
		btCollisionConfiguration *collisionConfiguration );
	
	/** \brief Clean up dispatcher. */
	virtual ~debpCollisionDispatcher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Collision objects can collide. */
// 	virtual bool needsCollision( const btCollisionObject *body0, const btCollisionObject *body1 );
	
	/** \brief Collision objects need collision response. */
// 	virtual bool needsResponse( const btCollisionObject *body0, const btCollisionObject *body1 );
	/*@}*/
};

#endif
