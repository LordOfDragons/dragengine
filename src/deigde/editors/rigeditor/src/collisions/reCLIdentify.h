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

// include only once
#ifndef _RECLIDENTIFY_H_
#define _RECLIDENTIFY_H_

// includes
#include "dragengine/systems/modules/scripting/deBaseScriptingCollider.h"
#include "dragengine/common/math/decMath.h"

// predefinitions
class reRig;
class reRigBone;
class reRigShape;
class deCollider;
class deColliderVolume;



/**
 * @brief Identify Collision Listener.
 * Identifies the first hit element.
 */
class reCLIdentify : public deBaseScriptingCollider{
private:
	reRig *pRig;
	reRigBone *pBone;
	reRigShape *pShape;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new collision listener. */
	reCLIdentify( reRig *rig );
	/** Cleans up the collision listener. */
	virtual ~reCLIdentify();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Resets the visitor. */
	void Reset();
	/** Retrieves the bone or NULL. */
	inline reRigBone *GetBone() const{ return pBone; }
	/** Retrieves the shape or NULL. */
	inline reRigShape *GetShape() const{ return pShape; }
	/** Determines if a bone is hit. */
	bool HasBone() const;
	/** Determines if a shape is hit. */
	bool HasShape() const;
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/**
	 * Determines the response for a given collision. If you specified ertCustom
	 * you have to update the info object with the response to the collision. In
	 * all other cases you do must not modify the info object.
	 */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	/**
	 * Determines if this collider can be hit by the given collider.
	 */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	/**
	 * Notifies the scripts that the properties of this collider have changed and
	 * that the attached element has to update. This is usually called after the
	 * collision detection but can also be called multiple times.
	 */
	virtual void ColliderChanged( deCollider *owner );
	
private:
	reRigBone *pGetBoneFromCollider( deCollider *collider ) const;
	reRigShape *pGetShapeFromCollider( deColliderVolume *collider ) const;
};

// end of include only once
#endif
