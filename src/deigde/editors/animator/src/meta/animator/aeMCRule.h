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
	aeMCRule(aeWindowMain &windowMain, aeRule *rule);
	
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
	Ref Capture() const;
	
	/** Undo system or nullptr to apply actions immediately. */
	igdeUndoSystem *GetUndoSystem() const override;
	
	/** \brief Clipboard or nullptr if not supported. */
	igdeClipboard *GetClipboard() const override;
	/*@}*/
};


class aeMCRuleAnimation : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleAnimation>;
	
	aeMCRuleAnimation(aeWindowMain &windowMain, aeRuleAnimation *rule);
	aeRuleAnimation *GetRuleType() const;
	aeRuleAnimation &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleAnimation() override;
};


class aeMCRuleAnimationDifference : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleAnimationDifference>;
	
	aeMCRuleAnimationDifference(aeWindowMain &windowMain, aeRuleAnimationDifference *rule);
	aeRuleAnimationDifference *GetRuleType() const;
	aeRuleAnimationDifference &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleAnimationDifference() override;
};


class aeMCRuleAnimationSelect : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleAnimationSelect>;
	
	aeMCRuleAnimationSelect(aeWindowMain &windowMain, aeRuleAnimationSelect *rule);
	aeRuleAnimationSelect *GetRuleType() const;
	aeRuleAnimationSelect &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleAnimationSelect() override;
};


class aeMCRuleBoneTransformator : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleBoneTransformator>;
	
	aeMCRuleBoneTransformator(aeWindowMain &windowMain, aeRuleBoneTransformator *rule);
	aeRuleBoneTransformator *GetRuleType() const;
	aeRuleBoneTransformator &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleBoneTransformator() override;
};


class aeMCRuleForeignState : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleForeignState>;
	
	aeMCRuleForeignState(aeWindowMain &windowMain, aeRuleForeignState *rule);
	aeRuleForeignState *GetRuleType() const;
	aeRuleForeignState &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleForeignState() override;
};


class aeMCRuleGroup : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleGroup>;
	
	aeMCRuleGroup(aeWindowMain &windowMain, aeRuleGroup *rule);
	aeRuleGroup *GetRuleType() const;
	aeRuleGroup &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleGroup() override;
};


class aeMCRuleInverseKinematic : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleInverseKinematic>;
	
	aeMCRuleInverseKinematic(aeWindowMain &windowMain, aeRuleInverseKinematic *rule);
	aeRuleInverseKinematic *GetRuleType() const;
	aeRuleInverseKinematic &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleInverseKinematic() override;
};


class aeMCRuleLimit : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleLimit>;
	
	aeMCRuleLimit(aeWindowMain &windowMain, aeRuleLimit *rule);
	aeRuleLimit *GetRuleType() const;
	aeRuleLimit &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleLimit() override;
};


class aeMCRuleMirror : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleMirror>;
	
	aeMCRuleMirror(aeWindowMain &windowMain, aeRuleMirror *rule);
	aeRuleMirror *GetRuleType() const;
	aeRuleMirror &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleMirror() override;
};


class aeMCRuleStateManipulator : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleStateManipulator>;
	
	aeMCRuleStateManipulator(aeWindowMain &windowMain, aeRuleStateManipulator *rule);
	aeRuleStateManipulator *GetRuleType() const;
	aeRuleStateManipulator &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleStateManipulator() override;
};


class aeMCRuleStateSnapshot : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleStateSnapshot>;
	
	aeMCRuleStateSnapshot(aeWindowMain &windowMain, aeRuleStateSnapshot *rule);
	aeRuleStateSnapshot *GetRuleType() const;
	aeRuleStateSnapshot &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleStateSnapshot() override;
};


class aeMCRuleSubAnimator : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleSubAnimator>;
	
	aeMCRuleSubAnimator(aeWindowMain &windowMain, aeRuleSubAnimator *rule);
	aeRuleSubAnimator *GetRuleType() const;
	aeRuleSubAnimator &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleSubAnimator() override;
};


class aeMCRuleTrackTo : public aeMCRule{
public:
	using Ref = deTObjectReference<aeMCRuleTrackTo>;
	
	aeMCRuleTrackTo(aeWindowMain &windowMain, aeRuleTrackTo *rule);
	aeRuleTrackTo *GetRuleType() const;
	aeRuleTrackTo &GetRuleTypeRef() const;
	
protected:
	~aeMCRuleTrackTo() override;
};

#endif
