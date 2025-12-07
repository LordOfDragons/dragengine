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
	aeWPViewListener(aeWPView &panel);
	
	/** Clean up listener. */
	virtual ~aeWPViewListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Rig changed. */
	virtual void RigChanged(aeAnimator *animator);
	
	/** Animator changed. */
	virtual void AnimatorChanged(aeAnimator *animator);
	
	/** Model changed. */
	virtual void ModelChanged(aeAnimator *animator);
	
	/** The environment component changed. */
	virtual void EnvObjectChanged(aeAnimator *animator);
	
	/** Sky changed. */
	virtual void SkyChanged(aeAnimator *animator);
	
	/** Animation changed. */
	virtual void AnimationChanged(aeAnimator *animator);
	
	/** Playback changed. */
	virtual void PlaybackChanged(aeAnimator *animator);
	
	/** View changed. */
	virtual void ViewChanged(aeAnimator *animator);
	
	/** Camera changed. */
	virtual void CameraChanged(aeAnimator *animator);
	
	/** Camera view changed. */
	virtual void CameraViewChanged(aeAnimator *animator);
	
	/** Active attachment changed. */
	virtual void ActiveAttachmentChanged(aeAnimator *animator, aeAttachment *attachment);
	
	/** Attachment changed. */
	virtual void AttachmentChanged(aeAnimator *animator, aeAttachment *attachment);
	
	/** Attachment count changed added. */
	virtual void AttachmentStructureChanged(aeAnimator *animator);
	/*@}*/
};

#endif
