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

#ifndef _AEWINDOWMAINLISTENER_H_
#define _AEWINDOWMAINLISTENER_H_

#include "../animator/aeAnimatorNotifier.h"

class aeWindowMain;



/**
 * Main window listener.
 */
class aeWindowMainListener : public aeAnimatorNotifier{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<aeWindowMainListener> Ref;
	
	

private:
	aeWindowMain &pWindowMain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	aeWindowMainListener(aeWindowMain &windowMain);
	
	/** Clean up listener. */
	~aeWindowMainListener() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Changed or saved state changed. */
	void StateChanged(aeAnimator *animator) override;
	
	/** Undos changed. */
	void UndoChanged(aeAnimator *animator) override;
	
	/** Animator changed. */
	void AnimatorChanged(aeAnimator *animator) override;
	
	/** View changed. */
	void ViewChanged(aeAnimator *animator) override;
	
	/** Model changed. */
	void ModelChanged(aeAnimator *animator) override;
	
	/** Animation changed. */
	void AnimationChanged(aeAnimator *animator) override;
	
	/** Controller changed. */
	void ControllerChanged(aeAnimator *animator, aeController *controller) override;
	
	/** Controller count or order changed. */
	void ControllerStructureChanged(aeAnimator *animator) override;
	
	/** Active rule changed. */
	void ActiveRuleChanged(aeAnimator *animator, aeRule *rule) override;
	
	/** Rule changed. */
	void RuleChanged(aeAnimator *animator, aeRule *rule) override;
	
	/** Rule name changed. */
	void RuleNameChanged(aeAnimator *animator, aeRule *rule) override;
	
	/** Rule count or order changed. */
	void RuleStructureChanged(aeAnimator *animator) override;
	/*@}*/
};

#endif
