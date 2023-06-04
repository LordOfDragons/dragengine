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

#ifndef _AEWPVIEWLISTENER_H_
#define _AEWPVIEWLISTENER_H_

#include "../../animator/aeAnimatorNotifier.h"

class aeWPView;



/**
 * View panel listener.
 */
class aeWPViewListener : public aeAnimatorNotifier{
private:
	aeWPView &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	aeWPViewListener( aeWPView &panel );
	
	/** Clean up listener. */
	virtual ~aeWPViewListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Rig changed. */
	virtual void RigChanged( aeAnimator *animator );
	
	/** Animator changed. */
	virtual void AnimatorChanged( aeAnimator *animator );
	
	/** Model changed. */
	virtual void ModelChanged( aeAnimator *animator );
	
	/** The environment component changed. */
	virtual void EnvObjectChanged( aeAnimator *animator );
	
	/** Sky changed. */
	virtual void SkyChanged( aeAnimator *animator );
	
	/** Animation changed. */
	virtual void AnimationChanged( aeAnimator *animator );
	
	/** Playback changed. */
	virtual void PlaybackChanged( aeAnimator *animator );
	
	/** View changed. */
	virtual void ViewChanged( aeAnimator *animator );
	
	/** Camera changed. */
	virtual void CameraChanged( aeAnimator *animator );
	
	/** Camera view changed. */
	virtual void CameraViewChanged( aeAnimator *animator );
	
	/** Active attachment changed. */
	virtual void ActiveAttachmentChanged( aeAnimator *animator, aeAttachment *attachment );
	
	/** Attachment changed. */
	virtual void AttachmentChanged( aeAnimator *animator, aeAttachment *attachment );
	
	/** Attachment count changed added. */
	virtual void AttachmentStructureChanged( aeAnimator *animator );
	/*@}*/
};

#endif
