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
	dedsCollider(deScriptingDragonScript &ds, deCollider *collider);
	
	/** \brief Clean up peer. */
	~dedsCollider() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Can hit collider callback is enabled. */
	inline bool GetEnableCanHitCallback() const{ return pEnableCanHitCallback; }
	
	/** \brief Owner object or \em NULL. */
	dsRealObject *GetOwner() const;
	
	/** \brief Set owner object or \em NULL. */
	void SetOwner(dsRealObject *object);
	
	/** \brief Set if can hit collider callback is enabled. */
	void SetEnableCanHitCallback(bool enable);
	
	/** \brief Callback object or \em NULL. */
	dsRealObject *GetCallback() const;
	
	/** \brief Set callback object or \em NULL. */
	void SetCallback(dsRealObject *object);
	
	/** \brief Breaking callback object or \em NULL. */
	dsRealObject *GetCallbackBreaking() const;
	
	/** \brief Set breaking callback object or \em NULL. */
	void SetCallbackBreaking(dsRealObject *object);
	
	
	
	/** \brief Collision response. */
	void CollisionResponse(deCollider *owner, deCollisionInfo *info) override;
	
	/** \brief Can hit collider query. */
	bool CanHitCollider(deCollider *owner, deCollider *collider) override;
	
	/** \brief Collider changed. */
	void ColliderChanged(deCollider *owner) override;
	
	
	
	/** \brief Collider constraint has broken. */
	void ColliderConstraintBroke(deCollider *owner,
		int index, deColliderConstraint *constraint) override;
	
	/** \brief Collider rig constraint has broken. */
	void RigConstraintBroke(deCollider *owner,
		int bone, int index, deRigConstraint *constraint) override;
	/*@}*/
};

#endif
