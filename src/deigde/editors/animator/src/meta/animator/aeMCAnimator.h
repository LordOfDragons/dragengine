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

#include "../../animator/controller/aeController.h"
#include "../../animator/link/aeLink.h"
#include "../../animator/rule/aeRule.h"

#include <deigde/meta/igdeMetaContext.h>

#include <dragengine/deTWeakObjectReference.h>


class aeWindowMain;
class aeAnimator;


/**
 * Animator meta context.
 * 
 * Animator is stored as weak reference since animator has igdeUndoSystem member which would
 * result in reference cycle.
 */
class aeMCAnimator : public igdeMetaContext{
public:
	using Ref = deTObjectReference<aeMCAnimator>;
	
private:
	aeWindowMain *pWindow;
	deTWeakObjectReference<aeAnimator> pAnimator;
	aeController::Ref pController;
	aeLink::Ref pLink;
	aeRule::Ref pRule;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create animator meta context. */
	explicit aeMCAnimator(aeWindowMain *window);
	
protected:
	/**
	 * Clean up object.
	 */
	virtual ~aeMCAnimator() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Window or nullptr. */
	inline aeWindowMain *GetWindow() const{ return pWindow; }
	
	/** Animator or null. */
	deTObjectReference<aeAnimator> GetAnimator() const;
	
	/** Active controller of active animator or nullptr. */
	const aeController::Ref &GetActiveController() const;
	
	/** Active link of active animator or nullptr. */
	const aeLink::Ref &GetActiveLink() const;
	
	/** Active rule of active animator or nullptr. */
	const aeRule::Ref &GetActiveRule() const;
	
	/** Capture context for animator properties. */
	Ref CaptureAnimator() const;
	
	/** Capture context for controller properties. */
	Ref CaptureController() const;
	
	/** Capture context for link properties. */
	Ref CaptureLink() const;
	
	/** Capture context for rule properties. */
	Ref CaptureRule() const;
	
	/** Undo system or nullptr to apply actions immediately. */
	igdeUndoSystem *GetUndoSystem() const override;
	/*@}*/
};

#endif
