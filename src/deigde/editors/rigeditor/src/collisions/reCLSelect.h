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

#ifndef _RECLSELECT_H_
#define _RECLSELECT_H_

#include "reCLHitListEntry.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

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
	reRig &pRig;
	bool pToggleSelection;
	bool pCanSelectBones;
	bool pCanSelectShapes;
	bool pCanSelectConstraints;
	bool pCanSelectPushes;
	reCLHitListEntry::List pHitList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new collision listener. */
	reCLSelect(reRig &rig);
	/** \brief Cleans up the collision listener. */
	virtual ~reCLSelect();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Sets if selection is toggled instead of switched. */
	void SetToggleSelection(bool toggle);
	/** \brief Sets if rig bones can be (un)selected. */
	void SetCanSelectBones(bool canSelect);
	/** \brief Sets if rig shapes can be (un)selected. */
	void SetCanSelectShapes(bool canSelect);
	/** \brief Sets if rig constraints can be (un)selected. */
	void SetCanSelectConstraints(bool canSelect);
	/** \brief Sets if rig pushes can be (un)selected. */
	void SetCanSelectPushes(bool canSelect);
	
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
	virtual void CollisionResponse(deCollider *owner, deCollisionInfo *info);
	/**
	 * \brief Determines if a collider can be hit.
	 * \details If this peer is used with a collider then the script is asked to
	 *          determine if the two collider can hit each other. If this peer is
	 *          used in a ray test owner is nullptr and the script is asked to
	 *          determine if the ray can hit the collider.
	 * \param owner Collider this peer belongs to or nullptr if a ray test is done.
	 * \param collider Collider to test.
	 * \return True if the owner/ray can hit the given collider.
	 */
	virtual bool CanHitCollider(deCollider *owner, deCollider *collider);
	/**
	 * \brief Notifies the scripts that the properties of this collider have changed
	 *        and that the attached element has to update.
	 * \details This is usually called after the collision detection but can also be
	 *          called multiple times.
	 */
	virtual void ColliderChanged(deCollider *owner);
	/*@}*/
	
private:
	reRigBone *pGetBoneFromCollider(deColliderVolume *collider) const;
	reRigShape *pGetShapeFromCollider(deColliderVolume *collider) const;
	reRigConstraint *pGetConstraintFromCollider(deColliderVolume *collider) const;
	reRigPush *pGetPushFromCollider(deColliderVolume *collider) const;
};

#endif
