/* 
 * Drag[en]gine IGDE Animator Editor
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
class aeCLClosestHit : public deBaseScriptingCollider, aeElementVisitor{
private:
	float pHitDistance;
	decVector pHitNormal;
	bool pHasHit;
	deCollider *pHitCollider;
	int pHitShape;
	
	igdeGizmo *pHitGizmo;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	aeCLClosestHit();
	
	/** Clean up visitor. */
	virtual ~aeCLClosestHit() override;
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
	
	/** Hit shape index or -1. */
	inline int GetHitShape() const{ return pHitShape; }
	
	
	
	/** Identify hit element. */
	void IdentifyHitElement( igdeEnvironment &environment );
	
	/** Hit gizmo or nullptr. */
	inline igdeGizmo *GetHitGizmo() const{ return pHitGizmo; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info ) override;
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider ) override;
	virtual void ColliderChanged( deCollider *owner ) override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	virtual void visitGizmo( igdeGizmo *gizmo ) override;
	/*@}*/
};

#endif
