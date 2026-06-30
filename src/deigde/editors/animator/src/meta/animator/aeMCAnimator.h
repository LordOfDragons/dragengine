/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _AEMCANIMATOR_H_
#define _AEMCANIMATOR_H_

#include <deigde/meta/igdeMetaContext.h>

class aeAnimator;
class aeAnimatorLocomotionLeg;
class aeWindowMain;


/**
 * Animator meta context.
 */
class aeMCAnimator : public igdeMetaContext{
public:
	using Ref = deTObjectReference<aeMCAnimator>;
	
private:
	aeWindowMain &pWindowMain;
	aeAnimator *pAnimator;
	deTObjectReference<aeAnimator> pGuardAnimator;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create animator meta context. */
	explicit aeMCAnimator(aeWindowMain &windowMain, aeAnimator *animator);
	
protected:
	aeMCAnimator(aeWindowMain &windowMain, aeAnimator *animator, const char *identifier);
	
	/**
	 * Clean up object.
	 */
	virtual ~aeMCAnimator() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Window main. */
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** Animator reference. */
	aeAnimator &GetAnimatorRef() const;
	
	/** Capture context. */
	Ref Capture() const;
	
	/** Environment. */
	igdeEnvironment &GetEnvironment() const override;
	
	/** Undo system or nullptr to apply actions immediately. */
	igdeUndoSystem *GetUndoSystem() const override;
	
	/** \brief Clipboard or nullptr if not supported. */
	igdeClipboard *GetClipboard() const override;
	/*@}*/
};


class aeMCAnimatorController : public aeMCAnimator{
public:
	using Ref = deTObjectReference<aeMCAnimatorController>;
	aeMCAnimatorController(aeWindowMain &windowMain, aeAnimator *animator);
	~aeMCAnimatorController() override;
};

class aeMCAnimatorLink : public aeMCAnimator{
public:
	using Ref = deTObjectReference<aeMCAnimatorLink>;
	aeMCAnimatorLink(aeWindowMain &windowMain, aeAnimator *animator);
	~aeMCAnimatorLink() override;
};

class aeMCAnimatorRule : public aeMCAnimator{
public:
	using Ref = deTObjectReference<aeMCAnimatorRule>;
	aeMCAnimatorRule(aeWindowMain &windowMain, aeAnimator *animator);
	~aeMCAnimatorRule() override;
};

class aeMCAnimatorPlayground : public aeMCAnimator{
public:
	using Ref = deTObjectReference<aeMCAnimatorPlayground>;
	aeMCAnimatorPlayground(aeWindowMain &windowMain, aeAnimator *animator);
	~aeMCAnimatorPlayground() override;
};

class aeMCAnimatorView : public aeMCAnimator{
public:
	using Ref = deTObjectReference<aeMCAnimatorView>;
	aeMCAnimatorView(aeWindowMain &windowMain, aeAnimator *animator);
	~aeMCAnimatorView() override;
};

class aeMCAnimatorAttachment : public aeMCAnimator{
public:
	using Ref = deTObjectReference<aeMCAnimatorAttachment>;
	aeMCAnimatorAttachment(aeWindowMain &windowMain, aeAnimator *animator);
	~aeMCAnimatorAttachment() override;
};

class aeMCAnimatorLocomotionLeg : public igdeMetaContext{
public:
	using Ref = deTObjectReference<aeMCAnimatorLocomotionLeg>;
	
private:
	aeWindowMain &pWindowMain;
	aeAnimatorLocomotionLeg *pLeg;
	deTObjectReference<aeAnimatorLocomotionLeg> pGuardLeg;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create locomotion leg meta context. */
	explicit aeMCAnimatorLocomotionLeg(aeWindowMain &windowMain, aeAnimatorLocomotionLeg *leg);
	
protected:
	/**
	 * Clean up object.
	 */
	virtual ~aeMCAnimatorLocomotionLeg() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Window main. */
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Leg. */
	inline aeAnimatorLocomotionLeg *GetLeg() const{ return pLeg; }
	
	/** Leg reference. */
	aeAnimatorLocomotionLeg &GetLegRef() const;
	
	/** Capture context. */
	Ref Capture() const;
	
	/** Environment. */
	igdeEnvironment &GetEnvironment() const override;
	
	/** Undo system or nullptr to apply actions immediately. */
	igdeUndoSystem *GetUndoSystem() const override;
	
	/** \brief Clipboard or nullptr if not supported. */
	igdeClipboard *GetClipboard() const override;
	/*@}*/
};

#endif
