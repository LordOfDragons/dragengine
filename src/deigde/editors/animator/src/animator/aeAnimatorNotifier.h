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

#ifndef _AEANIMATORNOTIFIER_H_
#define _AEANIMATORNOTIFIER_H_

#include <dragengine/deObject.h>

class aeAnimator;
class aeController;
class aeLink;
class aeRule;
class aeAttachment;



/**
 * @brief Animator Notifier.
 *
 * Notifier for changes in a animator.
 */
class aeAnimatorNotifier : public deObject{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new animator notifier. */
	aeAnimatorNotifier();
	/** Cleans up the animator notifier. */
	virtual ~aeAnimatorNotifier();
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/** Changed or saved state changed. */
	virtual void StateChanged( aeAnimator *animator );
	/** Undos changed. */
	virtual void UndoChanged( aeAnimator *animator );
	/** Animator changed. */
	virtual void AnimatorChanged( aeAnimator *animator );
	/** View changed. */
	virtual void ViewChanged( aeAnimator *animator );
	/** Model changed. */
	virtual void ModelChanged( aeAnimator *animator );
	/** Sky changed. */
	virtual void SkyChanged( aeAnimator *animator );
	/** The environment object changed. */
	virtual void EnvObjectChanged( aeAnimator *animator );
	
	/** \brief Rig changed. */
	virtual void RigChanged( aeAnimator *animator );
	
	/** Animation changed. */
	virtual void AnimationChanged( aeAnimator *animator );
	/** Playback changed. */
	virtual void PlaybackChanged( aeAnimator *animator );
	/** Locomotion changed. */
	virtual void LocomotionChanged( aeAnimator *animator );
	
	/** Active controller changed. */
	virtual void ActiveControllerChanged( aeAnimator *animator, aeController *controller );
	/** Controller changed. */
	virtual void ControllerChanged( aeAnimator *animator, aeController *controller );
	/** Controller name changed. */
	virtual void ControllerNameChanged( aeAnimator *animator, aeController *controller );
	/** Controller value changed. */
	virtual void ControllerValueChanged( aeAnimator *animator, aeController *controller );
	/** Controller count or order changed. */
	virtual void ControllerStructureChanged( aeAnimator *animator );
	
	/** Active link changed. */
	virtual void ActiveLinkChanged( aeAnimator *animator, aeLink *link );
	/** Link changed. */
	virtual void LinkChanged( aeAnimator *animator, aeLink *link );
	/** \brief Link name changed. */
	virtual void LinkNameChanged( aeAnimator *animator, aeLink *link );
	/** Link count or order changed. */
	virtual void LinkStructureChanged( aeAnimator *animator );
	
	/** Active rule changed. */
	virtual void ActiveRuleChanged( aeAnimator *animator, aeRule *rule );
	/** Rule changed. */
	virtual void RuleChanged( aeAnimator *animator, aeRule *rule );
	/** Rule name changed. */
	virtual void RuleNameChanged( aeAnimator *animator, aeRule *rule );
	/** Rule count or order changed. */
	virtual void RuleStructureChanged( aeAnimator *animator );
	
	/** Active attachment changed. */
	virtual void ActiveAttachmentChanged( aeAnimator *animator, aeAttachment *attachment );
	/** Attachment changed. */
	virtual void AttachmentChanged( aeAnimator *animator, aeAttachment *attachment );
	/** Attachment count changed added. */
	virtual void AttachmentStructureChanged( aeAnimator *animator );
	
	/** Camera changed. */
	virtual void CameraChanged( aeAnimator *animator );
	/** Camera view changed. */
	virtual void CameraViewChanged( aeAnimator *animator );
	/*@}*/
};

// end of include only once
#endif
