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

#ifndef _AECLCLOSESTHIT_H_
#define _AECLCLOSESTHIT_H_

#include "aeElementVisitor.h"

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>

class igdeGizmo;
class igdeEnvironment;


/**
 * Closest Hit Visitor.
 */
class aeCLClosestHit : public deBaseScriptingCollider, public aeElementVisitor{
private:
	float pHitDistance;
	decVector pHitNormal;
	bool pHasHit;
	deCollider *pHitCollider;
	int pHitBone;
	int pHitShape;
	
	igdeGizmo *pHitGizmo;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	aeCLClosestHit();
	
	/** Clean up visitor. */
	~aeCLClosestHit() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset visitor. */
	void Reset();
	
	/** Element has been hit. */
	inline bool GetHasHit() const{ return pHasHit; }
	
	/** Hit distance. */
	inline float GetHitDistance() const{ return pHitDistance; }
	
	/** Hit normal. */
	inline const decVector &GetHitNormal() const{ return pHitNormal; }
	
	/** Hit collider or nullptr. */
	inline deCollider *GetHitCollider() const{ return pHitCollider; }
	
	/** Hit bone index or -1. */
	inline int GetHitBone() const{ return pHitBone; }
	
	/** Hit shape index or -1. */
	inline int GetHitShape() const{ return pHitShape; }
	
	
	
	/** Identify hit element. */
	void IdentifyHitElement(igdeEnvironment &environment);
	
	/** Hit gizmo or nullptr. */
	inline igdeGizmo *GetHitGizmo() const{ return pHitGizmo; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	void CollisionResponse(deCollider *owner, deCollisionInfo *info) override;
	bool CanHitCollider(deCollider *owner, deCollider *collider) override;
	void ColliderChanged(deCollider *owner) override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	void VisitGizmo(igdeGizmo *gizmo) override;
	/*@}*/
};

#endif
