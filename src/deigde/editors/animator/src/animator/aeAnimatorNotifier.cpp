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

#include "aeAnimatorNotifier.h"



// Class aeAnimatorNotifier
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeAnimatorNotifier::aeAnimatorNotifier(){
}

aeAnimatorNotifier::~aeAnimatorNotifier(){
}



// Notifications
//////////////////

void aeAnimatorNotifier::StateChanged( aeAnimator* ){
}

void aeAnimatorNotifier::UndoChanged( aeAnimator* ){
}

void aeAnimatorNotifier::AnimatorChanged( aeAnimator* ){
}

void aeAnimatorNotifier::ViewChanged( aeAnimator* ){
}

void aeAnimatorNotifier::ModelChanged( aeAnimator *animator ){
	ViewChanged( animator );
}

void aeAnimatorNotifier::SkyChanged( aeAnimator *animator ){
	ViewChanged( animator );
}

void aeAnimatorNotifier::EnvObjectChanged( aeAnimator *animator ){
	ViewChanged( animator );
}

void aeAnimatorNotifier::RigChanged( aeAnimator *animator ){
	AnimatorChanged( animator );
}

void aeAnimatorNotifier::AnimationChanged( aeAnimator *animator ){
	AnimatorChanged( animator );
}

void aeAnimatorNotifier::PlaybackChanged( aeAnimator* ){
}

void aeAnimatorNotifier::LocomotionChanged( aeAnimator* ){
}



void aeAnimatorNotifier::ActiveControllerChanged( aeAnimator*, aeController* ){
}

void aeAnimatorNotifier::ControllerChanged( aeAnimator*, aeController* ){
}

void aeAnimatorNotifier::ControllerNameChanged( aeAnimator*, aeController* ){
}

void aeAnimatorNotifier::ControllerValueChanged( aeAnimator*, aeController* ){
}

void aeAnimatorNotifier::ControllerStructureChanged( aeAnimator* ){
}



void aeAnimatorNotifier::ActiveLinkChanged( aeAnimator*, aeLink* ){
}

void aeAnimatorNotifier::LinkChanged( aeAnimator*, aeLink* ){
}

void aeAnimatorNotifier::LinkNameChanged( aeAnimator*, aeLink* ){
}

void aeAnimatorNotifier::LinkStructureChanged( aeAnimator* ){
}



void aeAnimatorNotifier::ActiveRuleChanged( aeAnimator*, aeRule* ){
}

void aeAnimatorNotifier::RuleChanged( aeAnimator*, aeRule* ){
}

void aeAnimatorNotifier::RuleNameChanged( aeAnimator*, aeRule* ){
}

void aeAnimatorNotifier::RuleStructureChanged( aeAnimator* ){
}



void aeAnimatorNotifier::ActiveAttachmentChanged( aeAnimator*, aeAttachment* ){
}

void aeAnimatorNotifier::AttachmentChanged( aeAnimator*, aeAttachment* ){
}

void aeAnimatorNotifier::AttachmentStructureChanged( aeAnimator* ){
}



void aeAnimatorNotifier::CameraChanged( aeAnimator* ){
}

void aeAnimatorNotifier::CameraViewChanged( aeAnimator* ){
}
