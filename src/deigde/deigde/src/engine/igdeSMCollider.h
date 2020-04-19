/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDESMCOLLIDER_H_
#define _IGDESMCOLLIDER_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



/**
 * \brief IGDE Script Module Collider Peer.
 *
 * Collider peer for the IGDRE Script Module. If a delegee is assigned all events are
 * forwarded to the delegee. Optionally a user pointer can be assigned.
 */
class igdeSMCollider : public deBaseScriptingCollider{
private:
	deBaseScriptingCollider *pDelegee;
	void *pUserPointer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create collider listener. */
	igdeSMCollider();
	
	/** \brief Clean up collider listener. */
	virtual ~igdeSMCollider();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Delegee or \em NULL. */
	inline deBaseScriptingCollider *GetDelegee() const{ return pDelegee; }
	
	/** \brief Set delegee or \em NULL. */
	void SetDelegee( deBaseScriptingCollider *delegee );
	
	/** \brief User pointer. */
	inline void *GetUserPointer() const{ return pUserPointer; }
	
	/** \brief Set user pointer. */
	void SetUserPointer( void *userPointer );
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
	
	/**
	 * Notifies the scripts that the properties of this collider have changed and
	 * that the attached element has to update. This is usually called after the
	 * collision detection but can also be called multiple times.
	 */
	virtual void ColliderChanged( deCollider *owner );
	/*@}*/
};

#endif
