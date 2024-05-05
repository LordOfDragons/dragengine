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
