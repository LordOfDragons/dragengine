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

#ifndef _RECLSELECT_H_
#define _RECLSELECT_H_

#include "reCLHitList.h"

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>

class reRig;
class reRigBone;
class reRigShape;
class reRigConstraint;
class reRigPush;
class deCollider;
class deColliderVolume;



/**
 * \brief Select elements collision listener.
 * \details The first element hit is selected. If multi select is enabled the hit
 *          element selection is toggled. Otherwise all other elements are
 *          unselected and this one selected.
 */
class reCLSelect : public deBaseScriptingCollider{
private:
	reRig *pRig;
	bool pToggleSelection;
	bool pCanSelectBones;
	bool pCanSelectShapes;
	bool pCanSelectConstraints;
	bool pCanSelectPushes;
	reCLHitList pHitList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new collision listener. */
	reCLSelect( reRig *rig );
	/** \brief Cleans up the collision listener. */
	virtual ~reCLSelect();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Sets if selection is toggled instead of switched. */
	void SetToggleSelection( bool toggle );
	/** \brief Sets if rig bones can be (un)selected. */
	void SetCanSelectBones( bool canSelect );
	/** \brief Sets if rig shapes can be (un)selected. */
	void SetCanSelectShapes( bool canSelect );
	/** \brief Sets if rig constraints can be (un)selected. */
	void SetCanSelectConstraints( bool canSelect );
	/** \brief Sets if rig pushes can be (un)selected. */
	void SetCanSelectPushes( bool canSelect );
	
	/** \brief Resets the visitor. */
	void Reset();
	/** \brief Runs the action on the visited object. */
	void RunAction();
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/**
	 * \brief Determines the response for a given collision.
	 * \details If you specified ertCustom you have to update the info object
	 *          with the response to the collision. In all other cases you do
	 *          must not modify the info object.
	 */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	/**
	 * \brief Determines if a collider can be hit.
	 * \details If this peer is used with a collider then the script is asked to
	 *          determine if the two collider can hit each other. If this peer is
	 *          used in a ray test owner is NULL and the script is asked to
	 *          determine if the ray can hit the collider.
	 * \param owner Collider this peer belongs to or NULL if a ray test is done.
	 * \param collider Collider to test.
	 * \return True if the owner/ray can hit the given collider.
	 */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	/**
	 * \brief Notifies the scripts that the properties of this collider have changed
	 *        and that the attached element has to update.
	 * \details This is usually called after the collision detection but can also be
	 *          called multiple times.
	 */
	virtual void ColliderChanged( deCollider *owner );
	/*@}*/
	
private:
	reRigBone *pGetBoneFromCollider( deColliderVolume *collider ) const;
	reRigShape *pGetShapeFromCollider( deColliderVolume *collider ) const;
	reRigConstraint *pGetConstraintFromCollider( deColliderVolume *collider ) const;
	reRigPush *pGetPushFromCollider( deColliderVolume *collider ) const;
};

#endif
