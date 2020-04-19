/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DEDSCOLLIDER_H_
#define _DEDSCOLLIDER_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class deScriptingDragonScript;
class dsValue;
class dsRealObject;



/**
 * \brief Collider peer.
 */
class dedsCollider : public deBaseScriptingCollider{
private:
	deScriptingDragonScript &pDS;
	deCollider *pCollider;
	
	dsValue *pValOwner;
	
	bool pEnableCanHitCallback;
	
	dsValue *pValCB;
	bool pHasCB;
	
	dsValue *pValCBBreaking;
	bool pHasCBBreaking;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedsCollider( deScriptingDragonScript &ds, deCollider *collider );
	
	/** \brief Clean up peer. */
	virtual ~dedsCollider();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Can hit collider callback is enabled. */
	inline bool GetEnableCanHitCallback() const{ return pEnableCanHitCallback; }
	
	/** \brief Owner object or \em NULL. */
	dsRealObject *GetOwner() const;
	
	/** \brief Set owner object or \em NULL. */
	void SetOwner( dsRealObject *object );
	
	/** \brief Set if can hit collider callback is enabled. */
	void SetEnableCanHitCallback( bool enable );
	
	/** \brief Callback object or \em NULL. */
	dsRealObject *GetCallback() const;
	
	/** \brief Set callback object or \em NULL. */
	void SetCallback( dsRealObject *object );
	
	/** \brief Breaking callback object or \em NULL. */
	dsRealObject *GetCallbackBreaking() const;
	
	/** \brief Set breaking callback object or \em NULL. */
	void SetCallbackBreaking( dsRealObject *object );
	
	
	
	/** \brief Collision response. */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	
	/** \brief Can hit collider query. */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	
	/** \brief Collider changed. */
	virtual void ColliderChanged( deCollider *owner );
	
	
	
	/** \brief Collider constraint has broken. */
	virtual void ColliderConstraintBroke( deCollider *owner,
		int index, deColliderConstraint *constraint );
	
	/** \brief Collider rig constraint has broken. */
	virtual void RigConstraintBroke( deCollider *owner,
		int bone, int index, deRigConstraint *constraint );
	/*@}*/
};

#endif
