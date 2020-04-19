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

#ifndef _RERIGNOTIFIER_H_
#define _RERIGNOTIFIER_H_

#include <dragengine/deObject.h>
#include <dragengine/resources/collider/deColliderConstraint.h>

class reRig;
class reRigPush;
class reRigBone;
class reRigShape;
class reRigConstraint;



/**
 * @brief Rig Notifier.
 * Notifier for changes in a rig.
 */
class reRigNotifier : public deObject{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig notifier. */
	reRigNotifier();
	/** Cleans up the rig notifier. */
	virtual ~reRigNotifier();
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/** Element or work mode changed. */
	virtual void ModeChanged( reRig *rig );
	/** Changed or saved state changed. */
	virtual void StateChanged( reRig *rig );
	/** Undos changed. */
	virtual void UndoChanged( reRig *rig );
	
	/** The bone structure of the rig changed. */
	virtual void StructureChanged( reRig *rig );
	/** Rig parameters changed. */
	virtual void RigChanged( reRig *rig );
	/** The resource changed. */
	virtual void ResourceChanged( reRig *rig );
	/** The sky changed. */
	virtual void SkyChanged( reRig *rig );
	/** The environment object changed. */
	virtual void EnvObjectChanged( reRig *rig );
	/** The view changed. */
	virtual void ViewChanged( reRig *rig );
	
	/** The camera changed. */
	virtual void CameraChanged( reRig *rig );
	/** The camera view changed. */
	virtual void CameraViewChanged( reRig *rig );
	
	/** Bone count changed. */
	virtual void BoneCountChanged( reRig *rig );
	/** A bone changed. */
	virtual void BoneChanged( reRig *rig, reRigBone *bone );
	/** A bone changed selection state. */
	virtual void BoneSelectedChanged( reRig *rig, reRigBone *bone );
	/** All bones have been deselected. */
	virtual void AllBonesDeselected( reRig *rig );
	/** The active bone changed. */
	virtual void ActiveBoneChanged( reRig *rig );
	
	/** Shape count changed. */
	virtual void ShapeCountChanged( reRig *rig );
	/** A shape changed. */
	virtual void ShapeChanged( reRig *rig, reRigShape *shape );
	/** A shape changed selection state. */
	virtual void ShapeSelectedChanged( reRig *rig, reRigShape *shape );
	/** All shapes have been deselected. */
	virtual void AllShapesDeselected( reRig *rig );
	/** The active shape changed. */
	virtual void ActiveShapeChanged( reRig *rig );
	
	/** Constraint count changed. */
	virtual void ConstraintCountChanged( reRig *rig );
	/** A constraint changed. */
	virtual void ConstraintChanged( reRig *rig, reRigConstraint *constraint );
	/** A constraint degree of freedom changed. */
	virtual void ConstraintDofChanged( reRig *rig, reRigConstraint *constraint,
		deColliderConstraint::eDegreesOfFreedom dof );
	/** A constraint changed selection state. */
	virtual void ConstraintSelectedChanged( reRig *rig, reRigConstraint *constraint );
	/** All constraints have been deselected. */
	virtual void AllConstraintsDeselected( reRig *rig );
	/** The active constraint changed. */
	virtual void ActiveConstraintChanged( reRig *rig );
	
	/** Push count changed. */
	virtual void PushCountChanged( reRig *rig );
	/** A push changed. */
	virtual void PushChanged( reRig *rig, reRigPush *push );
	/** A push changed selection state. */
	virtual void PushSelectedChanged( reRig *rig, reRigPush *push );
	/** All pushes have been deselected. */
	virtual void AllPushesDeselected( reRig *rig );
	/** The active push changed. */
	virtual void ActivePushChanged( reRig *rig );
	/*@}*/
};

#endif
