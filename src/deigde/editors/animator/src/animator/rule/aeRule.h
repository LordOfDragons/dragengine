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

#ifndef _AERULE_H_
#define _AERULE_H_

#include "../link/aeLink.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/meta/igdeMetaContext.h>
#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyObjectSet.h>
#include <deigde/meta/property/igdeMetaPropertyStringSet.h>

class aeWindowMain;
class aeAnimator;
class aeRuleGroup;
class deAnimatorRule;
class deAnimatorRuleVisitor;
class igdeTranslationManager;

/**
 * Animator rule.
 */
class aeRule : public deObject{
public:
	/** Type holding strong reference. */
	using Ref = deTObjectReference<aeRule>;
	using List = decTObjectOrderedSet<aeRule>;
	
	using MetaContext = igdeMetaContextType<aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRule *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
	template <typename R>
	class MetaPropertyTarget : public R::MetaProperty<igdeMetaPropertyObjectSetStorage<aeLink>> {
	public:
		template <typename... A>
		MetaPropertyTarget(A&&... args) : R::MetaProperty<igdeMetaPropertyObjectSetStorage<aeLink>>(std::forward<A>(args)...){
			this->SetRows(3);
			this->SetMultiSelection(true);
		}
		
		void GetObjectItemInfoType(const igdeMetaContext::Ref&,
		const aeLink::Ref &link, igdeMetaContextItemInfo &info) const override{
			info.SetAll(link->mpName);
		}
		
		igdeMetaPropertyObjectSetStorage<aeLink>::SetType GetValidObjectsType(const igdeMetaContext::Ref &context) const override{
			return igdeMetaPropertyObjectSetStorage<aeLink>::SetType(this->Owner(context).GetAnimatorRef().mpLinks.GetValue());
		}
		
		void AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner) override{
			auto &helper = menu.GetEnvironment().GetUIHelper();
			helper.MenuCommand(menu, igdeMetaPropertyObjectSet::ActionAdd::Ref::New(*this, owner, context));
			this->AddDefaultContextMenuEntries(menu, context, owner);
		}
		
	protected:
		~MetaPropertyTarget() override = default;
	};
	
	
private:
	aeWindowMain &pWindowMain;
	aeAnimator *pAnimator;
	aeRuleGroup *pParentGroup;
	deAnimatorRule *pEngRule;
	int pIndex;
	
	MetaContext::Ref pMetaContext;
	deAnimatorRuleVisitorIdentify::eRuleTypes pType;
	
public:
	igdeMetaPropertyStringStorage::Storage mpName;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRule::eBlendModes>::Storage mpBlendMode;
	igdeMetaPropertyFloatStorage::Storage mpBlendFactor;
	igdeMetaPropertyBooleanStorage::Storage mpInvertBlendFactor;
	igdeMetaPropertyBooleanStorage::Storage mpEnabled;
	igdeMetaPropertyStringSetStorage::Storage mpAffectedBones;
	igdeMetaPropertyStringSetStorage::Storage mpAffectedVps;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetBlendFactor;
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRule() = delete;
	aeRule(const aeRule&) = delete;
	
	/** Create a new animator rule. */
	aeRule(aeWindowMain &windowMain, const MetaContext::Ref &metaContext,
		deAnimatorRuleVisitorIdentify::eRuleTypes type, const char *name);
	
	/** Create a copy of an animator rule. */
	aeRule(const MetaContext::Ref &metaContext, const aeRule &copy);
	
	/** Clean up the animator rule. */
	~aeRule() override;
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Retrieve the parent animator. */
	aeAnimator *GetAnimator() const;
	/** Set the parent animator. */
	void SetAnimator(aeAnimator *animator);
	
	/** Parent animator throwing exception if nullptr. */
	aeAnimator &GetAnimatorRef() const;
	
	igdeEnvironment &GetEnvironment() const;
	igdeUndoSystem *GetUndoSystem() const;
	
	/** Meta context. */
	inline const MetaContext::Ref &GetMetaContext() const{ return pMetaContext; }
	
	/** Retrieve the engine animator rule or nullptr. */
	inline deAnimatorRule *GetEngineRule() const{ return pEngRule; }
	/** Set the engine animator rule or nullptr. */
	void SetEngineRule(deAnimatorRule *rule);
	/** Create an engine animator rule. */
	virtual deAnimatorRule::Ref CreateEngineRule() = 0;
	/** Init the given engine rule with the default rule properties. */
	void InitEngineRule(deAnimatorRule *engRule) const;
	
	/** Retrieve the rule type. */
	inline deAnimatorRuleVisitorIdentify::eRuleTypes GetType() const{ return pType; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Sets the index. */
	void SetIndex(int index);
	
	/** Retrieve the parent group or nullptr if there is none. */
	inline aeRuleGroup *GetParentGroup() const{ return pParentGroup; }
	/** Set the parent group or nullptr if there is none. */
	void SetParentGroup(aeRuleGroup *group);
	
	/** Update Component and Animation. */
	virtual void UpdateCompAnim();
	/** Update targets. */
	virtual void UpdateTargets();
	/** Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage(aeLink *link) const;
	
	/** Notify the engine that the rule changed. */
	void NotifyRuleChanged();
	
	/** Create a copy of this rule. */
	virtual aeRule::Ref CreateCopy() const = 0;
	
	/** Parent animator changed. */
	virtual void OnParentAnimatorChanged();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	aeRule &operator=(const aeRule &copy) = delete;
	/*@}*/
	
	/** \name Helper */
	/*@{*/
	/** Create a new rule from a rule type. */
	static aeRule::Ref CreateRuleFromType(aeWindowMain &windowMain,
		deAnimatorRuleVisitorIdentify::eRuleTypes type,
		const igdeTranslationManager &tm);
	/*@}*/
	
	
protected:
	void pInitCopy(const aeRule &copy);
	
	void pUpdateEngineTarget(deAnimatorControllerTarget &target,
		const igdeMetaPropertyObjectSetStorage<aeLink>::Storage &storage) const;
};

#endif
