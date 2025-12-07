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

#ifndef _DEARANIMATOR_H_
#define _DEARANIMATOR_H_

#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>

class dearAnimation;
class deDEAnimator;
class deAnimator;



/**
 * DEAnimator Animator Module Class.
 * Animator module.
 */
class dearAnimator : public deBaseAnimatorAnimator{
private:
	deDEAnimator &pModule;
	deAnimator &pAnimator;
	dearAnimation *pAnimation;
	unsigned int pUpdateTracker;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create animator peer. */
	dearAnimator(deDEAnimator &module, deAnimator &animator);
	
	/** Clean up animator. */
	virtual ~dearAnimator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deDEAnimator &GetModule(){ return pModule; }
	inline const deDEAnimator &GetModule() const{ return pModule; }
	
	/** Animator. */
	inline deAnimator &GetAnimator(){ return pAnimator; }
	inline const deAnimator &GetAnimator() const{ return pAnimator; }
	
	/** Current update tracker state. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	
	/** Animation or nullptr. */
	inline dearAnimation *GetAnimation() const{ return pAnimation; }
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/** Rig changed. */
	virtual void RigChanged();
	
	/** Animation changed. */
	virtual void AnimationChanged();
	
	/** List of bones changed. */
	virtual void BonesChanged();
	
	/** List of vertex position sets changed. */
	virtual void VertexPositionSetsChanged();
	
	/** Controllers added or removed. */
	virtual void ControllerCountChanged();
	
	/** Links added, removed or changed. */
	virtual void LinksChanged();
	
	/** Rules added, removed or changed. */
	virtual void RulesChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
