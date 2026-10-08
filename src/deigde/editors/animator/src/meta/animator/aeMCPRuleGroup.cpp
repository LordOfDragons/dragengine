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

#include "aeMCPRuleGroup.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/rule/aeRuleGroup.h"


namespace {

class UndoRules : public igdeMetaPropertyListUndo{
private:
	aeLink::List pAddLinks;
	aeController::List pAddControllers;
	
public:
	using Ref = deTObjectReference<UndoRules>;
	
	UndoRules(igdeMetaPropertyList &property, const igdeMetaContext::Ref &context,
	const igdeMetaPropertyList::List &newValue, const char *undoInfo = nullptr,
	const char *undoInfoLong = nullptr) :
		igdeMetaPropertyListUndo(property, context, newValue, undoInfo, undoInfoLong)
	{
		pProcessLinks();
	}
	
	void Undo() override{
		igdeMetaPropertyListUndo::Undo();
		if(pAddLinks.IsNotEmpty()){
			auto &animator = GetContext().DynamicCast<aeRuleGroup::MetaContext>()->GetOwnerRef().GetAnimatorRef();
			animator.mpLinks.SetValue(animator.mpLinks.GetValue() - pAddLinks);
		}
		if(pAddControllers.IsNotEmpty()){
			auto &animator = GetContext().DynamicCast<aeRuleGroup::MetaContext>()->GetOwnerRef().GetAnimatorRef();
			animator.mpControllers.SetValue(animator.mpControllers.GetValue() - pAddControllers);
		}
	}
	
	void Redo() override{
		if(pAddControllers.IsNotEmpty()){
			auto &animator = GetContext().DynamicCast<aeRuleGroup::MetaContext>()->GetOwnerRef().GetAnimatorRef();
			animator.mpControllers.SetValue(animator.mpControllers.GetValue() + pAddControllers);
		}
		if(pAddLinks.IsNotEmpty()){
			auto &animator = GetContext().DynamicCast<aeRuleGroup::MetaContext>()->GetOwnerRef().GetAnimatorRef();
			animator.mpLinks.SetValue(animator.mpLinks.GetValue() + pAddLinks);
		}
		igdeMetaPropertyListUndo::Redo();
	}
	
protected:
	void pProcessLinks(){
		const auto &animator = GetContext().DynamicCast<aeRuleGroup::MetaContext>()->GetOwnerRef().GetAnimatorRef();
		decTDictionary<aeLink::Ref,aeLink::Ref> addLinks;
		decTDictionary<aeController::Ref,aeController::Ref> addControllers;
		
		GetNewValue().Visit([&](const deObject::Ref &object){
			object.DynamicCast<aeRule>()->EnsureValidLinks(animator, addLinks, addControllers);
		});
		
		addLinks.Visit([&](const aeLink::Ref&, const aeLink::Ref &value){
			pAddLinks.Add(value);
		});
		addControllers.Visit([&](const aeController::Ref&, const aeController::Ref &value){
			pAddControllers.Add(value);
		});
	}
};

}

	
void aeMCPRuleGroupRules::GetObjectItemInfoType(const ContextRef&, const ObjectTypeRef &rule,
igdeMetaContextItemInfo &info) const{
	info.SetAll(decString::Formatted("{0}: {1}", rule->GetIndex(), rule->mpName.GetValue()));
}

aeMCPRuleGroupRules::ObjectTypeRef aeMCPRuleGroupRules::CopyObjectType(
const ContextRef &context, const aeRule::List &existingObjects, const ObjectTypeRef &object) const{
	auto &rule = Owner(context);
	auto copied = object->CreateCopy();
	copied->mpName.SetValue(rule.uniqueNameRule.Generate(
		[&](const decString &name){
			return existingObjects.NoneMatching([&](const aeRule &existing){
				return existing.mpName == name;
			});
		}, copied->mpName), false);
	return copied;
}

igdeMetaPropertyListUndo::Ref aeMCPRuleGroupRules::ChangePropertyValue(const ContextRef &context,
const List &newValue, const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem() && GetCanUndo()){
		const auto undo = UndoRules::Ref::New(*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}
