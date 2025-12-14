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
public:
	typedef deTObjectReference<aeWPRuleListener> Ref;
	
	
private:
	aeWPRule &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	aeWPRuleListener(aeWPRule &panel);
	
	/** Clean up listener. */
	~aeWPRuleListener() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Model changed. */
	void ModelChanged(aeAnimator *animator) override;
	
	/** Rig changed. */
	void RigChanged(aeAnimator *animator) override;
	
	/** Animation changed. */
	void AnimationChanged(aeAnimator *animator) override;
	
	/** Controller named changed. */
	void ControllerNameChanged(aeAnimator *animator, aeController *controller) override;
	
	/** Controller count or order changed. */
	void ControllerStructureChanged(aeAnimator *animator) override;
	
	/** Link name changed. */
	void LinkNameChanged(aeAnimator *animator, aeLink *link) override;
	
	/** Link count or order changed. */
	void LinkStructureChanged(aeAnimator *animator) override;
	
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
