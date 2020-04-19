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
