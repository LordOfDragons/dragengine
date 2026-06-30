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

#include "aeMCPController.h"
#include "../../animator/link/aeLink.h"
#include "../../gui/aeWindowMain.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/meta/property/undo/igdeMetaPropertyListUndo.h>
#include <deigde/meta/property/undo/igdeMetaPropertyStringUndo.h>

#include <dragengine/common/collection/decTList.h>


// Class aeMCPControllers
///////////////////////////

namespace {

class cUndoSetControllers : public igdeMetaPropertyListUndo{
public:
	struct sLinkRemoveController{
		aeLink::Ref link;
		aeController::Ref controller;
	};
	
private:
	decTList<sLinkRemoveController> pLinksRemoveControllers;
	
public:
	cUndoSetControllers(aeMCPControllers &property, const igdeMetaContext::Ref &context,
	const igdeMetaPropertyList::List &newValue, const char *undoInfo, const char *undoInfoLong) :
	igdeMetaPropertyListUndo(property, context, newValue, undoInfo, undoInfoLong)
	{
		const auto &animator = property.Animator(context);
		const auto oldControllers = property.ConvertList(GetOldValue());
		const auto newControllers = property.ConvertList(GetNewValue());
		const auto &links = animator.GetLinks();
		
		oldControllers.Visit([&](const aeController::Ref &controller){
			if(newControllers.Has(controller)){
				return;
			}
			
			links.Visit([&](const aeLink::Ref &link){
				if(link->GetController() == controller){
					pLinksRemoveControllers.Add({link, controller});
				}
			});
		});
	}
	
	inline const decTList<sLinkRemoveController> &GetLinksRemoveControllers() const{ return pLinksRemoveControllers; }
	
	void Undo() override{
		igdeMetaPropertyListUndo::Undo();
		pLinksRemoveControllers.Visit([&](const sLinkRemoveController &each){
			each.link->SetController(each.controller, false);
		});
	}
	
	void Redo() override{
		pLinksRemoveControllers.Visit([&](const sLinkRemoveController &each){
			each.link->SetController(nullptr, false);
		});
		igdeMetaPropertyListUndo::Redo();
	}
};

class cActionControllerAdd : public igdeMetaProperty::Action{
	aeMCPControllers &pPropertyController;
	
public:
	cActionControllerAdd(aeMCPControllers &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
		igdeMetaProperty::Action(owner, context, "@Animator.Action.Controller.Add",
			owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
			"@Animator.Action.Controller.Add.ToolTip"),
		pPropertyController(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyController.IsValid(context)){
			return;
		}
		
		decString name(Translate("Animator.DefaultName.Controller").ToUTF8());
		if(!igdeCommonDialogs::GetString(GetOwner(), "@Animator.Dialog.AddController.Title",
		"@Animator.Dialog.AddController.Name", name)){
			return;
		}
		
		auto list = pPropertyController.GetStorage(context).GetValue();
		if(list.HasNamed(name)){
			igdeCommonDialogs::Error(GetOwner(), "@Animator.Dialog.AddController.Title",
				"@Animator.Dialog.AddController.ErrorNameExists");
			return;
		}
		
		list.Add(aeController::Ref::New(pPropertyController.Animator(context).GetWindowMain(), name));
		pPropertyController.ChangePropertyValueType(context, list, BuildUndoInfo(pPropertyController));
	}
};

}

igdeMetaPropertyListUndo::Ref aeMCPControllers::ChangePropertyValue(const ContextRef &context,
const List &newValue, const char *undoInfo, const char *undoInfoLong){
	const auto undo = deTObjectReference<cUndoSetControllers>::New(*this, context, newValue, undoInfo, undoInfoLong);
	
	if(newValue.HasMatching([&](const deObject::Ref &a){
		return newValue.HasMatching([&](const deObject::Ref &b){
			return a != b && a.StaticCast<aeController>()->GetName() == b.StaticCast<aeController>()->GetName();
		});
	})){
		igdeCommonDialogs::Error(WindowMain(context), undo->GetShortInfo(),
			"@Animator.WPController.Dialog.SetControllerName.DuplicateName");
		return {};
	}
	
	const auto &linksRemoveControllers = undo->GetLinksRemoveControllers();
	if(linksRemoveControllers.IsNotEmpty()){
		decStringList names;
		linksRemoveControllers.Visit([&](const cUndoSetControllers::sLinkRemoveController &each){
			names.Add(each.link->GetName());
		});
		names.SortAscending();
		auto strNames = names.GetCount() > 5 ? DEJoin(names.GetHead(5), ", ") + ", ..." : DEJoin(names, ", ");
		
		if(igdeCommonDialogs::QuestionFormat(WindowMain(context), igdeCommonDialogs::ebsYesNo,
		undo->GetShortInfo(), "@Animator.Dialog.RemoveController.Message",
		names.GetCount(), strNames.GetString()) != igdeCommonDialogs::ebYes){
			return {};
		}
	}
	
	if(context->GetUndoSystem() && GetCanUndo()){
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		undo->Redo();
		return {};
	}
}

aeMCPControllers::ObjectTypeRef aeMCPControllers::CopyObjectType(const ContextRef &context,
const aeController::List &existingObjects, const ObjectTypeRef &object) const{
	auto copied = aeController::Ref::New(WindowMain(context), *object);
	copied->GetMPName().SetValue(Animator(context).uniqueNameController.Generate(copied->GetMPName()), false);
	return copied;
}

igdeMetaProperty::Action::Ref aeMCPControllers::CreateButtonAction(TargetButton target, igdeWidget &owner){
	switch(target){
	case TargetButton::add:
		return deTObjectReference<cActionControllerAdd>::New(*this, owner);
		
	default:
		return CreateDefaultButtonAction(target, owner);
	}
}

void aeMCPControllers::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	const auto &windowMain = Animator(context).GetWindowMain();
	auto &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, windowMain.GetActionControllerAdd());
	AddDefaultContextMenuEntries(menu, context, owner);
}


// Class aeMCPControllerName
//////////////////////////////

igdeMetaPropertyStringUndo::Ref aeMCPControllerName::ChangePropertyValue(
const ContextRef &context, const char *newValue, const char *undoInfo, const char *undoInfoLong){
	decString name(newValue);
	auto animator = Controller(context).GetAnimator();
	if(animator){
		name = animator->uniqueNameController.Generate(name, GetPropertyValue(context));
	}
	return igdeMetaPropertyStringStorage::ChangePropertyValue(context, name, undoInfo, undoInfoLong);
}


// Class aeMCPControllerMaximumValue
//////////////////////////////////////

namespace {

class cActionMaximumFromMove : public igdeMetaProperty::Action{
	aeMCPControllerMaximumValue &pPropertyMaximum;
	
public:
	cActionMaximumFromMove(aeMCPControllerMaximumValue &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
		igdeMetaProperty::Action(owner, context, "@Animator.WPController.SetFromMove",
			nullptr, "@Animator.WPController.SetFromMove.ToolTip"),
		pPropertyMaximum(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyMaximum.IsValid(context)){
			return;
		}
		
		auto &controller = pPropertyMaximum.Controller(context);
		if(!controller.GetAnimator()){
			return;
		}
		
		const auto &engAnimator = controller.GetAnimator()->GetEngineAnimator();
		if(!engAnimator){
			return;
		}
		
		const auto &engAnimation = engAnimator->GetAnimation();
		if(!engAnimation){
			return;
		}
		
		decStringList names;
		engAnimation->GetMoves().Visit([&](const deAnimationMove &move){
			names.Add(move.GetName());
		});
		if(names.IsEmpty()){
			return;
		}
		
		names.SortAscending();
		int selection = 0;
		if(!igdeCommonDialogs::SelectString(GetOwner(),
		"@Animator.WPController.Dialog.SetRangeFromMove.Title",
		"@Animator.WPController.Dialog.SetRangeFromMove.Message", names, selection)){
			return;
		}
		
		pPropertyMaximum.ChangePropertyValue(GetContext(),
			engAnimation->GetMoves()[engAnimation->FindMove(names[selection])]->GetPlaytime(),
			BuildUndoInfo(pPropertyMaximum));
	}
};

}

void aeMCPControllerMaximumValue::AddContextMenuEntries(igdeMenuCascade &menu, const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, deTObjectReference<cActionMaximumFromMove>::New(*this, owner, context));
}


// Class aeMCPControllerCurrentValue
//////////////////////////////////////

namespace {

class cActionValueSetToLower : public igdeMetaProperty::Action{
	aeMCPControllerCurrentValue &pPropertyCurrent;
	
public:
	cActionValueSetToLower(aeMCPControllerCurrentValue &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
		igdeMetaProperty::Action(owner, context, "@Animator.WPController.ValueSetToLower",
			owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallLeft),
			"@Animator.WPController.ValueSetToLower.ToolTip"),
		pPropertyCurrent(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyCurrent.IsValid(context)){
			return;
		}
		
		auto &controller = pPropertyCurrent.Controller(context);
		pPropertyCurrent.ChangePropertyValue(context, controller.GetMPMinimumValue(), BuildUndoInfo(pPropertyCurrent));
	}
};

class cActionValueSetToUpper : public igdeMetaProperty::Action{
	aeMCPControllerCurrentValue &pPropertyCurrent;
	
public:
	cActionValueSetToUpper(aeMCPControllerCurrentValue &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
		igdeMetaProperty::Action(owner, context, "@Animator.WPController.ValueSetToUpper",
			owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallRight),
			"@Animator.WPController.ValueSetToUpper.ToolTip"),
		pPropertyCurrent(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyCurrent.IsValid(context)){
			return;
		}
		
		auto &controller = pPropertyCurrent.Controller(context);
		pPropertyCurrent.ChangePropertyValue(context, controller.GetMPMaximumValue(), BuildUndoInfo(pPropertyCurrent));
	}
};

class cActionValueSetToCenter : public igdeMetaProperty::Action{
	aeMCPControllerCurrentValue &pPropertyCurrent;
	
public:
	cActionValueSetToCenter(aeMCPControllerCurrentValue &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
		igdeMetaProperty::Action(owner, context, "@Animator.WPController.ValueSetToCenter",
			owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
			"@Animator.WPController.ValueSetToCenter.ToolTip"),
		pPropertyCurrent(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyCurrent.IsValid(context)){
			return;
		}
		
		auto &controller = pPropertyCurrent.Controller(context);
		pPropertyCurrent.ChangePropertyValue(context,
			(controller.GetMPMinimumValue() + controller.GetMPMaximumValue()) / 2.0f,
			BuildUndoInfo(pPropertyCurrent));
	}
};

class cActionValueSetRandom : public igdeMetaProperty::Action{
	aeMCPControllerCurrentValue &pPropertyCurrent;
	
public:
	cActionValueSetRandom(aeMCPControllerCurrentValue &property, igdeWidget &owner, const igdeMetaContext::Ref &context = {}) :
		igdeMetaProperty::Action(owner, context, "@Animator.WPController.ValueSetRandom",
			nullptr, "@Animator.WPController.ValueSetRandom.ToolTip"),
		pPropertyCurrent(property){}
	
	void OnAction() override{
		const auto &context = GetContext();
		if(!pPropertyCurrent.IsValid(context)){
			return;
		}
		
		auto &controller = pPropertyCurrent.Controller(context);
		pPropertyCurrent.ChangePropertyValue(context,
			decMath::random(controller.GetMPMinimumValue(), controller.GetMPMaximumValue()),
			BuildUndoInfo(pPropertyCurrent));
	}
};

}

deTObjectReference<igdeMetaPropertyFloatUndo> aeMCPControllerCurrentValue::ChangePropertyValue(
const ContextRef &context, float newValue, const char *undoInfo, const char *undoInfoLong){
	if(Controller(context).GetMPFrozen()){
		return {};
	}
	return aeTMCPAnimatorController::ChangePropertyValue(context, newValue, undoInfo, undoInfoLong);
}

void aeMCPControllerCurrentValue::AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner){
	auto &helper = contextMenu.GetEnvironment().GetUIHelper();
	if(contextMenu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(contextMenu);
	}
	helper.MenuCommand(contextMenu, deTObjectReference<cActionValueSetToLower>::New(*this, owner, context));
	helper.MenuCommand(contextMenu, deTObjectReference<cActionValueSetToUpper>::New(*this, owner, context));
	helper.MenuCommand(contextMenu, deTObjectReference<cActionValueSetToCenter>::New(*this, owner, context));
	helper.MenuCommand(contextMenu, deTObjectReference<cActionValueSetRandom>::New(*this, owner, context));
}
