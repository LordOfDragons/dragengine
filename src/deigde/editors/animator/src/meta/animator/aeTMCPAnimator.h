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

#ifndef _AETMCPANIMATOR_H_
#define _AETMCPANIMATOR_H_

#include <algorithm>

#include "aeMCAnimator.h"


/**
 * Animator meta context property template.
 */
template <typename T>
class aeTMCPAnimator : public T{
public:
	template <typename... A>
	aeTMCPAnimator(const char *id, const char *name, const char *description, A&&... args) : T(id, name, description, std::forward<A>(args)...) {}
	
protected:
	virtual ~aeTMCPAnimator() override{}
	
public:
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCAnimator>()->CaptureAnimator();
	}
	
	bool IsValid(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCAnimator>()->GetAnimator().IsNotNull();
	}
	
	inline aeWindowMain &WindowMain(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCAnimator>()->GetWindowRef();
	}
	
	inline aeAnimator &Animator(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCAnimator>()->GetAnimator();
	}
};


/**
 * Animator controller meta context property template.
 */
template <typename T>
class aeTMCPAnimatorController : public T{
public:
	template <typename... A>
	aeTMCPAnimatorController(const char *id, const char *name, const char *description, A&&... args) : T(id, name, description, std::forward<A>(args)...) {}
	
protected:
	virtual ~aeTMCPAnimatorController() override{}
	
public:
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCAnimator>()->CaptureController();
	}
	
	bool IsValid(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCAnimator>()->GetActiveController().IsNotNull();
	}
	
	inline aeWindowMain &WindowMain(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCAnimator>()->GetWindowRef();
	}
	
	inline aeAnimator &Animator(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCAnimator>()->GetAnimator();
	}
	
	inline aeController &Controller(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCAnimator>()->GetActiveController();
	}
};


/**
 * Animator link meta context property template.
 */
template <typename T>
class aeTMCPAnimatorLink : public T{
public:
	template <typename... A>
	aeTMCPAnimatorLink(const char *id, const char *name, const char *description, A&&... args) : T(id, name, description, std::forward<A>(args)...) {}
	
protected:
	virtual ~aeTMCPAnimatorLink() override{}
	
public:
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCAnimator>()->CaptureLink();
	}
	
	bool IsValid(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCAnimator>()->GetActiveLink().IsNotNull();
	}
	
	inline aeAnimator &Animator(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCAnimator>()->GetAnimator();
	}
	
	inline aeLink &Link(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCAnimator>()->GetActiveLink();
	}
};


/**
 * Animator rule meta context property template.
 */
template <typename T>
class aeTMCPAnimatorRule : public T{
public:
	template <typename... A>
	aeTMCPAnimatorRule(const char *id, const char *name, const char *description, A&&... args) : T(id, name, description, std::forward<A>(args)...) {}
	
protected:
	virtual ~aeTMCPAnimatorRule() override{}
	
public:
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCAnimator>()->CaptureRule();
	}
	
	bool IsValid(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCAnimator>()->GetActiveRule().IsNotNull();
	}
	
	inline aeAnimator &Animator(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCAnimator>()->GetAnimator();
	}
	
	inline aeRule &Rule(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCAnimator>()->GetActiveRule();
	}
};

#endif
