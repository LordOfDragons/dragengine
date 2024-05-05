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
