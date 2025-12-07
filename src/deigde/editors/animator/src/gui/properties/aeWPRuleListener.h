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

#ifndef _AEWPRULELISTENER_H_
#define _AEWPRULELISTENER_H_

#include "../../animator/aeAnimatorNotifier.h"

class aeWPRule;



/**
 * Rule panel listener.
 */
class aeWPRuleListener : public aeAnimatorNotifier{
private:
	aeWPRule &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	aeWPRuleListener(aeWPRule &panel);
	
	/** Clean up listener. */
	virtual ~aeWPRuleListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Model changed. */
	virtual void ModelChanged(aeAnimator *animator);
	
	/** Rig changed. */
	virtual void RigChanged(aeAnimator *animator);
	
	/** Animation changed. */
	virtual void AnimationChanged(aeAnimator *animator);
	
	/** Controller named changed. */
	virtual void ControllerNameChanged(aeAnimator *animator, aeController *controller);
	
	/** Controller count or order changed. */
	virtual void ControllerStructureChanged(aeAnimator *animator);
	
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
	/*@}*/
};

#endif
