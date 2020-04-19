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

#include "reRigNotifier.h"



// Class reRigNotifier
////////////////////////

// Constructor, destructor
////////////////////////////

reRigNotifier::reRigNotifier(){
}

reRigNotifier::~reRigNotifier(){
}



// Notifications
//////////////////

void reRigNotifier::ModeChanged( reRig *rig ){
}

void reRigNotifier::StateChanged( reRig *rig ){
}

void reRigNotifier::UndoChanged( reRig *rig ){
}



void reRigNotifier::StructureChanged( reRig *rig ){
}

void reRigNotifier::RigChanged( reRig *rig ){
}

void reRigNotifier::ResourceChanged( reRig *rig ){
}

void reRigNotifier::SkyChanged( reRig *rig ){
}

void reRigNotifier::EnvObjectChanged( reRig *rig ){
}

void reRigNotifier::ViewChanged( reRig *rig ){
}



void reRigNotifier::CameraChanged( reRig *rig ){
}

void reRigNotifier::CameraViewChanged( reRig *rig ){
}



void reRigNotifier::BoneCountChanged( reRig *rig ){
}

void reRigNotifier::BoneChanged( reRig *rig, reRigBone *bone ){
}

void reRigNotifier::BoneSelectedChanged( reRig *rig, reRigBone *bone ){
}

void reRigNotifier::AllBonesDeselected( reRig *rig ){
}

void reRigNotifier::ActiveBoneChanged( reRig *rig ){
}



void reRigNotifier::ShapeCountChanged( reRig *rig ){
}

void reRigNotifier::ShapeChanged( reRig *rig, reRigShape *shape ){
}

void reRigNotifier::ShapeSelectedChanged( reRig *rig, reRigShape *shape ){
}

void reRigNotifier::AllShapesDeselected( reRig *rig ){
}

void reRigNotifier::ActiveShapeChanged( reRig *rig ){
}



void reRigNotifier::ConstraintCountChanged( reRig *rig ){
}

void reRigNotifier::ConstraintChanged( reRig *rig, reRigConstraint *constraint ){
}

void reRigNotifier::ConstraintDofChanged( reRig *rig, reRigConstraint *constraint,
deColliderConstraint::eDegreesOfFreedom dof ){
}

void reRigNotifier::ConstraintSelectedChanged( reRig *rig, reRigConstraint *constraint ){
}

void reRigNotifier::AllConstraintsDeselected( reRig *rig ){
}

void reRigNotifier::ActiveConstraintChanged( reRig *rig ){
}



void reRigNotifier::PushCountChanged( reRig *rig ){
}

void reRigNotifier::PushChanged( reRig *rig, reRigPush *push ){
}

void reRigNotifier::PushSelectedChanged( reRig *rig, reRigPush *push ){
}

void reRigNotifier::AllPushesDeselected( reRig *rig ){
}

void reRigNotifier::ActivePushChanged( reRig *rig ){
}
