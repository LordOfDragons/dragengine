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

#include "igdeMetaPropertyObjectSet.h"
#include "undo/igdeMetaPropertyObjectSetUndo.h"
#include "widget/igdeMetaPropertyObjectSetWidget.h"
#include "../igdeMetaContext.h"
#include "../../gui/igdeUIHelper.h"
#include "../../gui/igdeCommonDialogs.h"
#include "../../gui/dialog/igdeDialogSetSelect.h"
#include "../../environment/igdeEnvironment.h"
#include "../../localization/igdeTranslationManager.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyObjectSet::Listener
//////////////////////////////////////////////

void igdeMetaPropertyObjectSet::Listener::OnActiveChanged(
igdeMetaPropertyObjectSet*, const igdeMetaContext::Ref&){
}

void igdeMetaPropertyObjectSet::Listener::OnSelectionChanged(
igdeMetaPropertyObjectSet*, const igdeMetaContext::Ref&){
}


// Class igdeMetaPropertyObjectSet::ActionAdd
///////////////////////////////////////////////

igdeMetaPropertyObjectSet::ActionAdd::ActionAdd(igdeMetaPropertyObjectSet &property,
	igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.Add",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
	"@Igde.MetaPropertyList.Action.Add.ToolTip"),
pPropertyObjectSet(property){
}

void igdeMetaPropertyObjectSet::ActionAdd::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyObjectSet.IsValid(context)){
		return;
	}
	
	const auto candidates = pPropertyObjectSet.GetValidObjects(context);
	if(candidates.IsEmpty()){
		return;
	}
	
	decStringSet candidateNames;
	decTObjectDictionary<deObject> candidateMap;
	igdeMetaContextItemInfo info;
	candidates.Visit([&](const deObject::Ref &object){
		pPropertyObjectSet.GetObjectItemInfo(context, object, info);
		candidateNames.Add(info.GetText());
		candidateMap.SetAt(info.GetText(), object);
	});
	
	auto &owner = GetOwner();
	auto &environment = GetEnvironment();
	auto dialog = igdeDialogSetSelect::Ref::New(environment,
		"@Igde.MetaPropertyList.Action.Dialog.AddEntries.Title",
		"@Igde.MetaPropertyList.Action.Dialog.AddEntries.Message", candidateNames);
	
	auto iconPresent = environment.GetStockIcon(igdeEnvironment::esiSmallPlus);
	auto iconAbsent = environment.GetStockIcon(igdeEnvironment::esiSmallMinus);
	
	const auto oldValue = pPropertyObjectSet.GetPropertyValue(context);
	decStringSet oldValueNames;
	oldValue.Visit([&](const deObject::Ref &object){
		pPropertyObjectSet.GetObjectItemInfo(context, object, info);
		oldValueNames.Add(info.GetText());
	});
	
	dialog->MarkItems(oldValueNames, iconPresent, iconAbsent);
	if(!dialog->Run(&owner)){
		return;
	}
	
	auto newValue = oldValue;
	dialog->GetSelection().Visit([&](const decString &name){
		if(const auto object = candidateMap.GetAtOrDefault(name)){
			newValue.Add(object);
		}
	});
	
	if(newValue == oldValue){
		return;
	}
	
	pPropertyObjectSet.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyObjectSet));
}

void igdeMetaPropertyObjectSet::ActionAdd::Update(){
	SetEnabled(pPropertyObjectSet.IsValid(GetContext()));
}


// Class igdeMetaPropertyObjectSet::ActionRemove
//////////////////////////////////////////////////

igdeMetaPropertyObjectSet::ActionRemove::ActionRemove(igdeMetaPropertyObjectSet &property,
	igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.Remove",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"@Igde.MetaPropertyList.Action.Remove.ToolTip"),
pPropertyObjectSet(property){
}

void igdeMetaPropertyObjectSet::ActionRemove::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyObjectSet.IsValid(context)){
		return;
	}
	
	if(pPropertyObjectSet.GetMultiSelection()){
		const auto selection = pPropertyObjectSet.GetSelection(context);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pPropertyObjectSet.GetPropertyValue(context);
		selection.Visit([&](const deObject::Ref &data){
			newValue.Remove(data);
		});
		
		pPropertyObjectSet.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyObjectSet));
		
	}else{
		const auto active = pPropertyObjectSet.GetActiveObject(context);
		if(!active){
			return;
		}
		
		auto newValue = pPropertyObjectSet.GetPropertyValue(context);
		newValue.Remove(active);
		
		pPropertyObjectSet.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyObjectSet));
	};
}

void igdeMetaPropertyObjectSet::ActionRemove::Update(){
	const auto &context = GetContext();
	SetEnabled(pPropertyObjectSet.IsValid(context) && pPropertyObjectSet.GetActiveObject(context));
}


// Class igdeMetaPropertyObjectSet::ActionRemoveAll
/////////////////////////////////////////////////////

igdeMetaPropertyObjectSet::ActionRemoveAll::ActionRemoveAll(
	igdeMetaPropertyObjectSet &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.RemoveAll",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiDelete),
	"@Igde.MetaPropertyList.Action.RemoveAll.ToolTip"),
pPropertyObjectSet(property){
}

void igdeMetaPropertyObjectSet::ActionRemoveAll::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyObjectSet.IsValid(context) || pPropertyObjectSet.GetPropertyValue(context).IsEmpty()){
		return;
	}
	
	pPropertyObjectSet.ChangePropertyValue(context, {}, BuildUndoInfo(pPropertyObjectSet));
}

void igdeMetaPropertyObjectSet::ActionRemoveAll::Update(){
	const auto &context = GetContext();
	SetEnabled(pPropertyObjectSet.IsValid(context) && pPropertyObjectSet.GetPropertyValue(context).IsNotEmpty());
}


// Class igdeMetaPropertyObjectSet
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyObjectSet::igdeMetaPropertyObjectSet(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description),
pRows(4),
pMultiSelection(false){
}

igdeMetaPropertyObjectSet::~igdeMetaPropertyObjectSet() = default;


// Management
///////////////

void igdeMetaPropertyObjectSet::SetDefaultValue(const Set &value){
	pDefaultValue = value;
}

void igdeMetaPropertyObjectSet::SetRows(int rows){
	pRows = decMath::max(rows, 1);
}

void igdeMetaPropertyObjectSet::SetMultiSelection(bool multiSelection){
	pMultiSelection = multiSelection;
}

void igdeMetaPropertyObjectSet::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyObjectSet::NotifyActiveChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnActiveChanged(this, context);
	});
}

void igdeMetaPropertyObjectSet::NotifySelectionChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnSelectionChanged(this, context);
	});
}


igdeMetaPropertyObjectSetUndo::Ref igdeMetaPropertyObjectSet::ChangePropertyValue(
const igdeMetaContext::Ref &context, const Set &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyObjectSetUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyObjectSet::Set igdeMetaPropertyObjectSet::GetSelection(
const igdeMetaContext::Ref &context) const{
	auto active = GetActiveObject(context);
	return active ? Set(devctag, active) : Set();
}

void igdeMetaPropertyObjectSet::SetSelection(const igdeMetaContext::Ref&, const Set&){
}

igdeMetaPropertyObjectSet::Set igdeMetaPropertyObjectSet::GetValidObjects(
const igdeMetaContext::Ref&) const{
	return {};
}

igdeMetaProperty::Action::Ref igdeMetaPropertyObjectSet::CreateButtonAction(
TargetButton target, igdeWidget &owner){
	return CreateDefaultButtonAction(target, owner);
}

void igdeMetaPropertyObjectSet::AddContextMenuEntries(igdeMenuCascade &contextMenu,
const ContextRef &context, igdeWidget &owner){
	AddDefaultContextMenuEntries(contextMenu, context, owner);
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyObjectSet::CreateWidget(){
	return igdeMetaPropertyObjectSetWidget::Ref::New(*this);
}

void igdeMetaPropertyObjectSet::AddDefaultContextMenuEntries(igdeMenuCascade &menu,
const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, ActionRemove::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionRemoveAll::Ref::New(*this, owner, context));
}

igdeMetaPropertyObjectSet::Action::Ref igdeMetaPropertyObjectSet::CreateDefaultButtonAction(
TargetButton target, igdeWidget &owner){
	switch(target){
	case TargetButton::add:
		return ActionAdd::Ref::New(*this, owner);
		
	case TargetButton::remove:
		return ActionRemove::Ref::New(*this, owner);
		
	default:
		return {};
	}
}
