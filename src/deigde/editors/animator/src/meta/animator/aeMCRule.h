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

#ifndef _AEMCRULE_H_
#define _AEMCRULE_H_

#include <deigde/meta/igdeMetaContext.h>

#include <dragengine/common/exceptions.h>

class aeWindowMain;
class aeRule;
class aeRuleAnimation;
class aeRuleAnimationDifference;
class aeRuleAnimationSelect;
class aeRuleBoneTransformator;
class aeRuleForeignState;
class aeRuleGroup;
class aeRuleInverseKinematic;
class aeRuleLimit;
class aeRuleMirror;
class aeRuleStateManipulator;
class aeRuleStateSnapshot;
class aeRuleSubAnimator;
class aeRuleTrackTo;


/**
 * Rule meta context.
 */
class aeMCRule : public igdeMetaContext{
public:
	using Ref = deTObjectReference<aeMCRule>;
	
private:
	aeWindowMain &pWindowMain;
	aeRule *pRule;
	deTObjectReference<aeRule> pGuardRule;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule meta context. */
	aeMCRule(aeWindowMain &windowMain, aeRule *rule, bool guardRule = false);
	
protected:
	/**
	 * Clean up object.
	 */
	~aeMCRule() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Window main. */
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Rule. */
	inline aeRule *GetRule() const{ return pRule; }
	
	/** Rule reference. */
	aeRule &GetRuleRef() const;
	
	/** Capture context. */
	virtual igdeMetaContext::Ref Capture() const;
	
	/** Environment. */
	igdeEnvironment &GetEnvironment() const override;
	
	/** Undo system or nullptr to apply actions immediately. */
	igdeUndoSystem *GetUndoSystem() const override;
	
	/** \brief Clipboard or nullptr if not supported. */
	igdeClipboard *GetClipboard() const override;
	/*@}*/
};


class aeMCRuleAnimation : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleAnimation>;
	
	aeMCRuleAnimation(aeWindowMain &windowMain, aeRuleAnimation *rule, bool guardRule = false);
	aeRuleAnimation *GetRuleType() const;
	aeRuleAnimation &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleAnimation() override;
};


class aeMCRuleAnimationDifference : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleAnimationDifference>;
	
	aeMCRuleAnimationDifference(aeWindowMain &windowMain, aeRuleAnimationDifference *rule, bool guardRule = false);
	aeRuleAnimationDifference *GetRuleType() const;
	aeRuleAnimationDifference &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleAnimationDifference() override;
};


class aeMCRuleAnimationSelect : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleAnimationSelect>;
	
	aeMCRuleAnimationSelect(aeWindowMain &windowMain, aeRuleAnimationSelect *rule, bool guardRule = false);
	aeRuleAnimationSelect *GetRuleType() const;
	aeRuleAnimationSelect &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleAnimationSelect() override;
};


class aeMCRuleBoneTransformator : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleBoneTransformator>;
	
	aeMCRuleBoneTransformator(aeWindowMain &windowMain, aeRuleBoneTransformator *rule, bool guardRule = false);
	aeRuleBoneTransformator *GetRuleType() const;
	aeRuleBoneTransformator &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleBoneTransformator() override;
};


class aeMCRuleForeignState : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleForeignState>;
	
	aeMCRuleForeignState(aeWindowMain &windowMain, aeRuleForeignState *rule, bool guardRule = false);
	aeRuleForeignState *GetRuleType() const;
	aeRuleForeignState &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleForeignState() override;
};


class aeMCRuleGroup : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleGroup>;
	
	aeMCRuleGroup(aeWindowMain &windowMain, aeRuleGroup *rule, bool guardRule = false);
	aeRuleGroup *GetRuleType() const;
	aeRuleGroup &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleGroup() override;
};


class aeMCRuleInverseKinematic : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleInverseKinematic>;
	
	aeMCRuleInverseKinematic(aeWindowMain &windowMain, aeRuleInverseKinematic *rule, bool guardRule = false);
	aeRuleInverseKinematic *GetRuleType() const;
	aeRuleInverseKinematic &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleInverseKinematic() override;
};


class aeMCRuleLimit : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleLimit>;
	
	aeMCRuleLimit(aeWindowMain &windowMain, aeRuleLimit *rule, bool guardRule = false);
	aeRuleLimit *GetRuleType() const;
	aeRuleLimit &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleLimit() override;
};


class aeMCRuleMirror : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleMirror>;
	
	aeMCRuleMirror(aeWindowMain &windowMain, aeRuleMirror *rule, bool guardRule = false);
	aeRuleMirror *GetRuleType() const;
	aeRuleMirror &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleMirror() override;
};


class aeMCRuleStateManipulator : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleStateManipulator>;
	
	aeMCRuleStateManipulator(aeWindowMain &windowMain, aeRuleStateManipulator *rule, bool guardRule = false);
	aeRuleStateManipulator *GetRuleType() const;
	aeRuleStateManipulator &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleStateManipulator() override;
};


class aeMCRuleStateSnapshot : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleStateSnapshot>;
	
	aeMCRuleStateSnapshot(aeWindowMain &windowMain, aeRuleStateSnapshot *rule, bool guardRule = false);
	aeRuleStateSnapshot *GetRuleType() const;
	aeRuleStateSnapshot &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleStateSnapshot() override;
};


class aeMCRuleSubAnimator : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleSubAnimator>;
	
	aeMCRuleSubAnimator(aeWindowMain &windowMain, aeRuleSubAnimator *rule, bool guardRule = false);
	aeRuleSubAnimator *GetRuleType() const;
	aeRuleSubAnimator &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleSubAnimator() override;
};


class aeMCRuleTrackTo : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleTrackTo>;
	
	aeMCRuleTrackTo(aeWindowMain &windowMain, aeRuleTrackTo *rule, bool guardRule = false);
	aeRuleTrackTo *GetRuleType() const;
	aeRuleTrackTo &GetRuleTypeRef() const;
	igdeMetaContext::Ref Capture() const override;
	
protected:
	~aeMCRuleTrackTo() override;
};

#endif
