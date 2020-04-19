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

#ifndef _DEBPGHOSTOBJECT_H_
#define _DEBPGHOSTOBJECT_H_

#include "debpCollisionObject.h"

#include <dragengine/common/math/decMath.h>

class debpBulletShape;
class btGhostObject;
class debpCollisionWorld;



/**
 * \brief Implements a ghost object in the bullet physics system.
 * \details This object takes care of creating the bullet ghost object and updating
 *          its state. Furthermore a reference is kept to the dragengine object owning
 *          the ghost object. A dragengine object can own more than one ghost object.
 */
class debpGhostObject : public debpCollisionObject{
private:
	debpCollisionWorld *pDynWorld;
	btGhostObject *pGhostObject;
	debpBulletShape *pShape;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	decDMatrix pMatrix;
	bool pEnabled;
	bool pDirtyMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new bullet ghost object. */
	debpGhostObject();
	/** \brief Cleans up the bullet ghost object. */
	virtual ~debpGhostObject();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the ghost object. */
	inline btGhostObject *GetGhostObject() const{ return pGhostObject; }
	/** \brief Retrieves the dynamics world or NULL if not assigned to one yet. */
	inline debpCollisionWorld *GetDynamicsWorld() const{ return pDynWorld; }
	/** \brief Sets the dynamics world or NULL if not assigned to one yet. */
	void SetDynamicsWorld( debpCollisionWorld *dynWorld );
	/** \brief Retrieves the collision shape or NULL if not set. */
	inline debpBulletShape *GetShape() const{ return pShape; }
	/** \brief Sets the collision shape or NULL if not set. */
	void SetShape( debpBulletShape *shape );
	
	/** \brief Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** \brief Sets the position. */
	void SetPosition( const decDVector &position );
	/** \brief Retrieves the orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	/** \brief Sets the orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Determines if the ghost object is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	/** \brief Sets if the ghost object is enabled. */
	void SetEnabled( bool enabled );
	/** \brief Retrieves the ghost object matrix. */
	const decDMatrix &GetMatrix();
	
	/** \brief Update dynamic world AABB if dirty. */
	void UpdateAABB();
	/*@}*/
	
private:
	void pCleanUp();
	void pCreateGhostObject();
	void pFreeGhostObject();
	void pUpdateTransform();
};

#endif
