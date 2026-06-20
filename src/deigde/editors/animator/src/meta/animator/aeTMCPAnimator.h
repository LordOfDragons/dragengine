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
#include "aeMCController.h"
#include "aeMCLink.h"
#include "aeMCRule.h"


/**
 * Animator meta context property template.
 */
template <typename T>
class aeTMCPAnimator : public T{
public:
	template <typename... A>
	aeTMCPAnimator(const char *id, const char *name, const char *description, A&&... args) :
		T(id, name, description, std::forward<A>(args)...) {}
	
protected:
	virtual ~aeTMCPAnimator() override{}
	
public:
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCAnimator>()->Capture();
	}
	
	bool IsValid(const igdeMetaContext::Ref &context) const override{
		const auto c = context.DynamicCast<aeMCAnimator>();
		return c && !c->IsDisposed() && c->GetAnimator() != nullptr;
	}
	
	inline aeWindowMain &WindowMain(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCAnimator>()->GetWindowMain();
	}
	
	inline aeAnimator &Animator(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCAnimator>()->GetAnimatorRef();
	}
};


/**
 * Animator controller meta context property template.
 */
template <typename T>
class aeTMCPAnimatorController : public T{
public:
	template <typename... A>
	aeTMCPAnimatorController(const char *id, const char *name, const char *description, A&&... args) :
		T(id, name, description, std::forward<A>(args)...) {}
	
protected:
	virtual ~aeTMCPAnimatorController() override{}
	
public:
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCController>()->Capture();
	}
	
	bool IsValid(const igdeMetaContext::Ref &context) const override{
		const auto c = context.DynamicCast<aeMCController>();
		return c && !c->IsDisposed() && c->GetController() != nullptr;
	}
	
	inline aeWindowMain &WindowMain(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCController>()->GetWindowMain();
	}
	
	inline aeController &Controller(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCController>()->GetControllerRef();
	}
};


/**
 * Animator link meta context property template.
 */
template <typename T>
class aeTMCPAnimatorLink : public T{
public:
	template <typename... A>
	aeTMCPAnimatorLink(const char *id, const char *name, const char *description, A&&... args) :
		T(id, name, description, std::forward<A>(args)...) {}
	
protected:
	virtual ~aeTMCPAnimatorLink() override{}
	
public:
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCLink>()->Capture();
	}
	
	bool IsValid(const igdeMetaContext::Ref &context) const override{
		const auto c = context.DynamicCast<aeMCLink>();
		return c && !c->IsDisposed() && c->GetLink() != nullptr;
	}
	
	inline aeLink &Link(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCLink>()->GetLinkRef();
	}
};


/**
 * Animator rule meta context property template.
 */
template <typename T>
class aeTMCPAnimatorRule : public T{
public:
	template <typename... A>
	aeTMCPAnimatorRule(const char *id, const char *name, const char *description, A&&... args) :
		T(id, name, description, std::forward<A>(args)...) {}
	
protected:
	virtual ~aeTMCPAnimatorRule() override{}
	
public:
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<aeMCRule>()->Capture();
	}
	
	inline aeWindowMain &WindowMain(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCRule>()->GetWindowMain();
	}
	
	bool IsValid(const igdeMetaContext::Ref &context) const override{
		const auto c = context.DynamicCast<aeMCRule>();
		return c && !c->IsDisposed() && c->GetRule() != nullptr;
	}
	
	inline aeRule &Rule(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<aeMCRule>()->GetRuleRef();
	}
};

template <typename T, typename C, typename R>
class aeTMCPAnimatorRuleType : public aeTMCPAnimatorRule<T>{
public:
	template <typename... A>
	aeTMCPAnimatorRuleType(const char *id, const char *name, const char *description, A&&... args) :
		aeTMCPAnimatorRule<T>(id, name, description, std::forward<A>(args)...) {}
	
protected:
	virtual ~aeTMCPAnimatorRuleType() override{}
	
public:
	igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
		return context.DynamicCast<C>()->Capture();
	}
	
	bool IsValid(const igdeMetaContext::Ref &context) const override{
		const auto c = context.DynamicCast<C>();
		return c && !c->IsDisposed() && c->GetRule() != nullptr;
	}
	
	inline R &RuleType(const igdeMetaContext::Ref &context) const{
		return context.DynamicCast<C>()->GetRuleTypeRef();
	}
};

template <typename T>
using aeTMCPAnimatorRuleAnimation = aeTMCPAnimatorRuleType<T, aeMCRuleAnimation, aeRuleAnimation>;

template <typename T>
using aeTMCPAnimatorRuleAnimationDifference = aeTMCPAnimatorRuleType<T, aeMCRuleAnimationDifference, aeRuleAnimationDifference>;

template <typename T>
using aeTMCPAnimatorRuleAnimationSelect = aeTMCPAnimatorRuleType<T, aeMCRuleAnimationSelect, aeRuleAnimationSelect>;

template <typename T>
using aeTMCPAnimatorRuleBoneTransformator = aeTMCPAnimatorRuleType<T, aeMCRuleBoneTransformator, aeRuleBoneTransformator>;

template <typename T>
using aeTMCPAnimatorRuleForeignState = aeTMCPAnimatorRuleType<T, aeMCRuleForeignState, aeRuleForeignState>;

template <typename T>
using aeTMCPAnimatorRuleGroup = aeTMCPAnimatorRuleType<T, aeMCRuleGroup, aeRuleGroup>;

template <typename T>
using aeTMCPAnimatorRuleInverseKinematic = aeTMCPAnimatorRuleType<T, aeMCRuleInverseKinematic, aeRuleInverseKinematic>;

template <typename T>
using aeTMCPAnimatorRuleLimit = aeTMCPAnimatorRuleType<T, aeMCRuleLimit, aeRuleLimit>;

template <typename T>
using aeTMCPAnimatorRuleMirror = aeTMCPAnimatorRuleType<T, aeMCRuleMirror, aeRuleMirror>;

template <typename T>
using aeTMCPAnimatorRuleStateManipulator = aeTMCPAnimatorRuleType<T, aeMCRuleStateManipulator, aeRuleStateManipulator>;

template <typename T>
using aeTMCPAnimatorRuleStateSnapshot = aeTMCPAnimatorRuleType<T, aeMCRuleStateSnapshot, aeRuleStateSnapshot>;

template <typename T>
using aeTMCPAnimatorRuleSubAnimator = aeTMCPAnimatorRuleType<T, aeMCRuleSubAnimator, aeRuleSubAnimator>;

template <typename T>
using aeTMCPAnimatorRuleTrackTo = aeTMCPAnimatorRuleType<T, aeMCRuleTrackTo, aeRuleTrackTo>;

#endif
