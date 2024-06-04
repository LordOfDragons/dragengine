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
