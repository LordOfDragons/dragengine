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

#ifndef _RECLOSESTELEMENT_H_
#define _RECLOSESTELEMENT_H_

#include "../rig/reRig.h"
#include "../rig/bone/reRigBone.h"
#include "../rig/shape/reRigShape.h"
#include "../rig/constraint/reRigConstraint.h"
#include "../rig/push/reRigPush.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



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
	reRigBone::Ref pHitSimBone;
	reRigBone::Ref pHitBone;
	reRigShape::Ref pHitShape;
	reRigConstraint::Ref pHitConstraint;
	reRigPush::Ref pHitPush;
	bool pHasHit;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a object. */
	reCLClosestElement(reRig &rig);
	/** \brief Cleans up the object. */
	virtual ~reCLClosestElement();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Sets if the simulation rig is tested. */
	void SetTestSimRig(bool test);
	/** \brief Sets if bones are tested. */
	void SetTestBones(bool test);
	/** \brief Sets if shapes are tested. */
	void SetTestShapes(bool test);
	/** \brief Sets if constraints are tested. */
	void SetTestConstraints(bool test);
	/** \brief Sets if pushes are tested. */
	void SetTestPushes(bool test);
	
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
	/** \brief Retrieves the hit simulation bone or nullptr. */
	inline const reRigBone::Ref &GetHitSimBone() const{ return pHitSimBone; }
	/** \brief Retrieves the hit bone or nullptr. */
	inline const reRigBone::Ref &GetHitBone() const{ return pHitBone; }
	/** \brief Retrieves the hit shape or nullptr. */
	inline const reRigShape::Ref &GetHitShape() const{ return pHitShape; }
	/** \brief Retrieves the hit constraint or nullptr. */
	inline const reRigConstraint::Ref &GetHitConstraint() const{ return pHitConstraint; }
	/** \brief Retrieves the hit push or nullptr. */
	inline const reRigPush::Ref &GetHitPush() const{ return pHitPush; }
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
	void pInitResult(deCollisionInfo *info);
};

#endif
