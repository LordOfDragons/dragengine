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

#include "aeMCPRuleSubAnimator.h"
#include "../../animator/aeAnimator.h"


// aeMCPRuleSubAnimatorConnectionTarget
/////////////////////////////////////////

decStringSet aeMCPRuleSubAnimatorConnectionTarget::GetPropertyAllowedStrings(const ContextRef &context) const{
	decStringSet strings;
	const auto rule = Owner(context).parentRule;
	if(rule){
		const auto &subAnimator = rule->GetSubAnimator();
		if(subAnimator){
			subAnimator->GetControllers().Visit([&](const deAnimatorController &controller){
				strings.Add(controller.GetName());
			});
		}
	}
	return strings;
}


// aeMCPRuleSubAnimatorConnectionController
/////////////////////////////////////////////

void aeMCPRuleSubAnimatorConnectionController::GetObjectItemInfoType(const ContextRef &context,
const ObjectTypeRef &controller, igdeMetaContextItemInfo &info) const{
	info.SetAll(controller->mpName.GetValue());
}

aeMCPRuleSubAnimatorConnectionController::ObjectTypeList
aeMCPRuleSubAnimatorConnectionController::GetPropertyAllowedObjectsType(const ContextRef &context) const{
	const auto r = Owner(context).parentRule;
	return r ? ObjectTypeList::New(r->GetAnimatorRef().mpControllers.GetValue()) : ObjectTypeList::New();
}


// aeMCPRuleSubAnimatorConnections
////////////////////////////////////

namespace{

class cActionConnectionAdd : public igdeMetaProperty::Action{
	aeMCPRuleSubAnimatorConnections &pProperty;
	
public:
	cActionConnectionAdd(aeMCPRuleSubAnimatorConnections &property, igdeWidget &owner,
		const igdeMetaContext::Ref &context) :
	Action(owner, context, "@Igde.MetaPropertyList.Action.Add",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Igde.MetaPropertyList.Action.Add.ToolTip"),
	pProperty(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pProperty.IsValid(context)){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValueType(context);
		auto connection = aeRuleSubAnimator::Connection::Ref::New(pProperty.Owner(context).GetWindowMain());
		newValue.Add(connection);
		pProperty.ChangePropertyValueType(context, newValue, BuildUndoInfo(pProperty));
		pProperty.SetActiveObjectType(context, connection);
	}
};

}

void aeMCPRuleSubAnimatorConnections::GetObjectItemInfoType(const ContextRef &context,
const ObjectTypeRef &connection, igdeMetaContextItemInfo &info) const{
	info.SetAll(decString::Formatted("{} -> {}", connection->mpController
			? connection->mpController->mpName.GetValue() : decString(),
		connection->mpTarget.GetValue()));
}

void aeMCPRuleSubAnimatorConnections::AddContextMenuEntries(igdeMenuCascade &menu,
const ContextRef &context, igdeWidget &owner){
	auto &environment = menu.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, deTObjectReference<cActionConnectionAdd>::New(*this, owner, context));
}
