/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEWOSOFORCEFIELD_H_
#define _IGDEWOSOFORCEFIELD_H_

#include "igdeWOSubObject.h"
#include "../../../triggersystem/igdeTriggerExpressionReference.h"

#include <dragengine/resources/collider/deColliderReference.h>
#include <dragengine/resources/forcefield/deForceFieldReference.h>


class deColliderAttachment;
class igdeGDCForceField;


/**
 * \brief Object wrapper sub object.
 */
class igdeWOSOForceField : public igdeWOSubObject{
private:
	const igdeGDCForceField &pGDForceField;
	deForceFieldReference pForceField;
	bool pAddedToWorld;
	deColliderReference pAttachedToCollider;
	deColliderAttachment *pAttachment;
	igdeTriggerExpressionReference pTriggerEnabled;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSOForceField( igdeWObject &wrapper, const igdeGDCForceField &gdForceField, const decString &prefix );
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSOForceField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief ForceField resource. */
	inline deForceField *GetForceField() const{ return pForceField; }
	
	/** \brief Update parameters. */
	virtual void UpdateParameters();
	
	/** \brief Init triggers. */
	virtual void InitTriggers();
	
	/** \brief Update trigger. */
	virtual void UpdateTriggers();
	
	/** \brief Update visibility. */
	virtual void UpdateVisibility();
	
	/** \brief Layer masks changed. */
	virtual void UpdateLayerMasks();
	
	/** \brief Collision filter changed. */
	virtual void UpdateCollisionFilter();
	
	/** \brief All sub components finished loading. */
	virtual void OnAllSubObjectsFinishedLoading();
	
	/** \brief Visit. */
	virtual void Visit( igdeWOSOVisitor &visitor );
	
	/** \brief For internal use only. */
	void AsyncLoadFinished( bool success );
	/*@}*/
	
	
	
protected:
	void AttachToCollider();
	void DetachFromCollider();
	
	
	
private:
	void pUpdateForceField();
	void pDestroyForceField();
};

#endif
