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

#include "reWindowMain.h"
#include "reWindowMainListener.h"



// Class reWindowMainListener
///////////////////////////////

// Constructor, destructor
////////////////////////////

reWindowMainListener::reWindowMainListener( reWindowMain &window ) :
pWindow( window ){
}

reWindowMainListener::~reWindowMainListener(){
}



// Management
///////////////

void reWindowMainListener::ModeChanged( reRig *rig ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::StateChanged( reRig *rig ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::UndoChanged( reRig *rig ){
	pWindow.UpdateAllActions();
}



void reWindowMainListener::BoneSelectedChanged( reRig *rig, reRigBone *bone ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::AllBonesDeselected( reRig *rig ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::ActiveBoneChanged( reRig *rig ){
	pWindow.UpdateAllActions();
}



void reWindowMainListener::ShapeCountChanged( reRig *rig ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::ShapeSelectedChanged( reRig *rig, reRigShape *shape ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::AllShapesDeselected( reRig *rig ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::ActiveShapeChanged( reRig *rig ){
	pWindow.UpdateAllActions();
}



void reWindowMainListener::ConstraintCountChanged( reRig *rig ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::AllConstraintsDeselected( reRig *rig ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::ConstraintSelectedChanged( reRig *rig, reRigConstraint *constraint ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::ActiveConstraintChanged( reRig *rig ){
	pWindow.UpdateAllActions();
}



void reWindowMainListener::PushCountChanged( reRig *rig ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::AllPushesDeselected( reRig *rig ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::PushSelectedChanged( reRig *rig, reRigPush *push ){
	pWindow.UpdateAllActions();
}

void reWindowMainListener::ActivePushChanged( reRig *rig ){
	pWindow.UpdateAllActions();
}
