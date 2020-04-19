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

#ifndef _RECLOSESTELEMENT_H_
#define _RECLOSESTELEMENT_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>

class reRig;
class reRigBone;
class reRigShape;
class reRigConstraint;
class reRigPush;
class deCollider;



/**
 * \brief Collision listener storing the closest hit element.
 */
class reCLClosestElement : public deBaseScriptingCollider{
private:
	reRig &pRig;
	
	bool pTestSimRig;
	bool pTestBones;
	bool pTestShapes;
	bool pTestConstraints;
	bool pTestPushes;
	
	float pHitDistance;
	decVector pHitNormal;
	bool pHitSimRig;
	reRigBone *pHitSimBone;
	reRigBone *pHitBone;
	reRigShape *pHitShape;
	reRigConstraint *pHitConstraint;
	reRigPush *pHitPush;
	bool pHasHit;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a object. */
	reCLClosestElement( reRig &rig );
	/** \brief Cleans up the object. */
	virtual ~reCLClosestElement();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Sets if the simulation rig is tested. */
	void SetTestSimRig( bool test );
	/** \brief Sets if bones are tested. */
	void SetTestBones( bool test );
	/** \brief Sets if shapes are tested. */
	void SetTestShapes( bool test );
	/** \brief Sets if constraints are tested. */
	void SetTestConstraints( bool test );
	/** \brief Sets if pushes are tested. */
	void SetTestPushes( bool test );
	
	/** \brief Resets the visitor. */
	void Reset();
	
	/** \brief Determines if an element has been hit. */
	inline bool GetHasHit() const{ return pHasHit; }
	/** \brief Retrieves the hit distance. */
	inline float GetHitDistance() const{ return pHitDistance; }
	/** \brief Retrieves the hit normal. */
	inline const decVector &GetHitNormal() const{ return pHitNormal; }
	/** \brief Determines if the simulation rig itself is hit. */
	inline bool GetHitSimRig() const{ return pHitSimRig; }
	/** \brief Retrieves the hit simulation bone or NULL. */
	inline reRigBone *GetHitSimBone() const{ return pHitSimBone; }
	/** \brief Retrieves the hit bone or NULL. */
	inline reRigBone *GetHitBone() const{ return pHitBone; }
	/** \brief Retrieves the hit shape or NULL. */
	inline reRigShape *GetHitShape() const{ return pHitShape; }
	/** \brief Retrieves the hit constraint or NULL. */
	inline reRigConstraint *GetHitConstraint() const{ return pHitConstraint; }
	/** \brief Retrieves the hit push or NULL. */
	inline reRigPush *GetHitPush() const{ return pHitPush; }
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
	void pInitResult( deCollisionInfo *info );
};

#endif
