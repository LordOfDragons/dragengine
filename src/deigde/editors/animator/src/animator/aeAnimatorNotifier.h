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

#ifndef _AEANIMATORNOTIFIER_H_
#define _AEANIMATORNOTIFIER_H_

#include <dragengine/deObject.h>

class aeAnimator;
class aeController;
class aeLink;
class aeRule;
class aeAttachment;



/**
 * Animator Notifier.
 *
 * Notifier for changes in a animator.
 */
class aeAnimatorNotifier : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<aeAnimatorNotifier> Ref;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new animator notifier. */
	aeAnimatorNotifier();
	/** Cleans up the animator notifier. */
protected:
	virtual ~aeAnimatorNotifier();
public:
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/** Changed or saved state changed. */
	virtual void StateChanged(aeAnimator *animator);
	/** Undos changed. */
	virtual void UndoChanged(aeAnimator *animator);
	/** Animator changed. */
	virtual void AnimatorChanged(aeAnimator *animator);
	/** View changed. */
	virtual void ViewChanged(aeAnimator *animator);
	/** Model changed. */
	virtual void ModelChanged(aeAnimator *animator);
	/** Sky changed. */
	virtual void SkyChanged(aeAnimator *animator);
	/** The environment object changed. */
	virtual void EnvObjectChanged(aeAnimator *animator);
	
	/** Rig changed. */
	virtual void RigChanged(aeAnimator *animator);
	
	/** Animation changed. */
	virtual void AnimationChanged(aeAnimator *animator);
	/** Playback changed. */
	virtual void PlaybackChanged(aeAnimator *animator);
	/** Locomotion changed. */
	virtual void LocomotionChanged(aeAnimator *animator);
	
	/** Active controller changed. */
	virtual void ActiveControllerChanged(aeAnimator *animator, aeController *controller);
	/** Controller changed. */
	virtual void ControllerChanged(aeAnimator *animator, aeController *controller);
	/** Controller name changed. */
	virtual void ControllerNameChanged(aeAnimator *animator, aeController *controller);
	/** Controller value changed. */
	virtual void ControllerValueChanged(aeAnimator *animator, aeController *controller);
	/** Controller count or order changed. */
	virtual void ControllerStructureChanged(aeAnimator *animator);
	
	/** Active link changed. */
	virtual void ActiveLinkChanged(aeAnimator *animator, aeLink *link);
	/** Link changed. */
	virtual void LinkChanged(aeAnimator *animator, aeLink *link);
	/** Link name changed. */
	virtual void LinkNameChanged(aeAnimator *animator, aeLink *link);
	/** Link count or order changed. */
	virtual void LinkStructureChanged(aeAnimator *animator);
	
	/** Active rule changed. */
	virtual void ActiveRuleChanged(aeAnimator *animator, aeRule *rule);
	/** Rule changed. */
	virtual void RuleChanged(aeAnimator *animator, aeRule *rule);
	/** Rule name changed. */
	virtual void RuleNameChanged(aeAnimator *animator, aeRule *rule);
	/** Rule count or order changed. */
	virtual void RuleStructureChanged(aeAnimator *animator);
	
	/** Active attachment changed. */
	virtual void ActiveAttachmentChanged(aeAnimator *animator, aeAttachment *attachment);
	/** Attachment changed. */
	virtual void AttachmentChanged(aeAnimator *animator, aeAttachment *attachment);
	/** Attachment count changed added. */
	virtual void AttachmentStructureChanged(aeAnimator *animator);
	
	/** Camera changed. */
	virtual void CameraChanged(aeAnimator *animator);
	/** Camera view changed. */
	virtual void CameraViewChanged(aeAnimator *animator);
	/*@}*/
};

// end of include only once
#endif
