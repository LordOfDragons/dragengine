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
public:
	typedef deTObjectReference<aeWPViewListener> Ref;
	
	
private:
	aeWPView &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	aeWPViewListener(aeWPView &panel);
	
	/** Clean up listener. */
protected:
	~aeWPViewListener() override;
public:
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Rig changed. */
	void RigChanged(aeAnimator *animator) override;
	
	/** Animator changed. */
	void AnimatorChanged(aeAnimator *animator) override;
	
	/** Model changed. */
	void ModelChanged(aeAnimator *animator) override;
	
	/** The environment component changed. */
	void EnvObjectChanged(aeAnimator *animator) override;
	
	/** Sky changed. */
	void SkyChanged(aeAnimator *animator) override;
	
	/** Animation changed. */
	void AnimationChanged(aeAnimator *animator) override;
	
	/** Playback changed. */
	void PlaybackChanged(aeAnimator *animator) override;
	
	/** View changed. */
	void ViewChanged(aeAnimator *animator) override;
	
	/** Camera changed. */
	void CameraChanged(aeAnimator *animator) override;
	
	/** Camera view changed. */
	void CameraViewChanged(aeAnimator *animator) override;
	
	/** Active attachment changed. */
	void ActiveAttachmentChanged(aeAnimator *animator, aeAttachment *attachment) override;
	
	/** Attachment changed. */
	void AttachmentChanged(aeAnimator *animator, aeAttachment *attachment) override;
	
	/** Attachment count changed added. */
	void AttachmentStructureChanged(aeAnimator *animator) override;
	/*@}*/
};

#endif
