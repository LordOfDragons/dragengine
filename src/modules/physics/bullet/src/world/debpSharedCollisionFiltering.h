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

#ifndef _DEBPSHAREDCOLLISIONFILTERING_H_
#define _DEBPSHAREDCOLLISIONFILTERING_H_

class debpCollisionObject;
class debpCollider;
class debpHTSector;
class debpTouchSensor;
class debpWorld;

class btCollisionObject;


/**
 * \brief Shared collision filter functions.
 */
class debpSharedCollisionFiltering{
private:
	debpWorld &pWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared collision filtering. */
	debpSharedCollisionFiltering( debpWorld &world );
	
	/** \brief Clean up shared collision filtering. */
	~debpSharedCollisionFiltering();
	/*@}*/
	
	
	
	/** \name Filtering */
	/*@{*/
	/** \brief Check if collision body can hit collision body. */
	bool CanBodyHitBody( const btCollisionObject &body0, const btCollisionObject &body1 ) const;
	
	/** \brief Check if collision object can hit collision object. */
	bool CanCObjHitCObj( const debpCollisionObject &colObj0, const debpCollisionObject &colObj1 ) const;
	
	
	/** \brief Check if collider can hit collision object. */
	bool CanColliderHitCObj( const debpCollisionObject &colObj0, const debpCollider &collider0,
		const debpCollisionObject &colObj1 ) const;
	
	/** \brief Check if height terrain can hit with collision object. */
	bool CanHTerrainHitCObj( const debpHTSector &htsector, const debpCollisionObject &colObj1 ) const;
	
	/** \brief Check if touch sensor can hit collision object. */
	bool CanTSensorHitCObj( const debpTouchSensor &touchSensor, const debpCollisionObject &colObj1 ) const;
	
	
	/** \brief Check if two colliders can collide. */
	bool CanColliderHitCollider( const debpCollisionObject &colObj0, const debpCollider &collider0,
		const debpCollisionObject &colObj1, const debpCollider &collider1 ) const;
	/*@}*/
};

#endif
