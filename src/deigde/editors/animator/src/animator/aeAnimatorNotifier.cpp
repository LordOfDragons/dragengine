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

void aeAnimatorNotifier::StateChanged(aeAnimator*){
}

void aeAnimatorNotifier::UndoChanged(aeAnimator*){
}

void aeAnimatorNotifier::AnimatorChanged(aeAnimator*){
}

void aeAnimatorNotifier::ViewChanged(aeAnimator*){
}

void aeAnimatorNotifier::ModelChanged(aeAnimator *animator){
	ViewChanged(animator);
}

void aeAnimatorNotifier::SkyChanged(aeAnimator *animator){
	ViewChanged(animator);
}

void aeAnimatorNotifier::EnvObjectChanged(aeAnimator *animator){
	ViewChanged(animator);
}

void aeAnimatorNotifier::RigChanged(aeAnimator *animator){
	AnimatorChanged(animator);
}

void aeAnimatorNotifier::AnimationChanged(aeAnimator *animator){
	AnimatorChanged(animator);
}

void aeAnimatorNotifier::PlaybackChanged(aeAnimator*){
}

void aeAnimatorNotifier::LocomotionChanged(aeAnimator*){
}



void aeAnimatorNotifier::ActiveControllerChanged(aeAnimator*, aeController*){
}

void aeAnimatorNotifier::ControllerChanged(aeAnimator*, aeController*){
}

void aeAnimatorNotifier::ControllerNameChanged(aeAnimator*, aeController*){
}

void aeAnimatorNotifier::ControllerValueChanged(aeAnimator*, aeController*){
}

void aeAnimatorNotifier::ControllerStructureChanged(aeAnimator*){
}



void aeAnimatorNotifier::ActiveLinkChanged(aeAnimator*, aeLink*){
}

void aeAnimatorNotifier::LinkChanged(aeAnimator*, aeLink*){
}

void aeAnimatorNotifier::LinkNameChanged(aeAnimator*, aeLink*){
}

void aeAnimatorNotifier::LinkStructureChanged(aeAnimator*){
}



void aeAnimatorNotifier::ActiveRuleChanged(aeAnimator*, aeRule*){
}

void aeAnimatorNotifier::RuleChanged(aeAnimator*, aeRule*){
}

void aeAnimatorNotifier::RuleNameChanged(aeAnimator*, aeRule*){
}

void aeAnimatorNotifier::RuleStructureChanged(aeAnimator*){
}



void aeAnimatorNotifier::ActiveAttachmentChanged(aeAnimator*, aeAttachment*){
}

void aeAnimatorNotifier::AttachmentChanged(aeAnimator*, aeAttachment*){
}

void aeAnimatorNotifier::AttachmentStructureChanged(aeAnimator*){
}



void aeAnimatorNotifier::CameraChanged(aeAnimator*){
}

void aeAnimatorNotifier::CameraViewChanged(aeAnimator*){
}
