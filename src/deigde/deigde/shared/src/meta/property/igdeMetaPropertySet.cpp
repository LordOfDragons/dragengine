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

#include "igdeMetaPropertySet.h"
#include "undo/igdeMetaPropertySetUndo.h"
#include "widget/igdeMetaPropertySetWidget.h"
#include "../igdeMetaContextItemInfo.h"
#include "../../gui/igdeUIHelper.h"
#include "../../gui/dialog/igdeDialogSetSelect.h"
#include "../../environment/igdeEnvironment.h"
#include "../../undo/igdeUndoSystem.h"
#include "../../localization/igdeTranslationManager.h"


// Class igdeMetaPropertySet::Listener
////////////////////////////////////////

void igdeMetaPropertySet::Listener::OnActiveChanged(
igdeMetaPropertySet*, const igdeMetaContext::Ref&){
}

void igdeMetaPropertySet::Listener::OnSelectionChanged(
igdeMetaPropertySet*, const igdeMetaContext::Ref&){
}


// Class igdeMetaPropertySet::ActionAdd
/////////////////////////////////////////

igdeMetaPropertySet::ActionAdd::ActionAdd(igdeMetaPropertySet &property,
	igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.Add",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
	"@Igde.MetaPropertyList.Action.Add.ToolTip"),
pPropertySet(property){
}

void igdeMetaPropertySet::ActionAdd::OnAction(){
	const auto &context = GetContext();
	if(!pPropertySet.IsValid(context)){
		return;
	}
	
	const igdeMetaPropertySet::Set candidates = pPropertySet.GetValidObjects(context);
	if(candidates.IsEmpty()){
		return;
	}
	
	decStringSet candidateNames;
	decTObjectDictionary<deObject> candidateMap;
	igdeMetaContextItemInfo info;
	candidates.Visit([&](const deObject::Ref &object){
		pPropertySet.GetObjectItemInfo(context, object, info);
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
	
	const auto oldValue = pPropertySet.GetPropertyValue(context);
	decStringSet oldValueNames;
	oldValue.Visit([&](const deObject::Ref &object){
		pPropertySet.GetObjectItemInfo(context, object, info);
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
	
	pPropertySet.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertySet));
}

void igdeMetaPropertySet::ActionAdd::Update(){
	SetEnabled(pPropertySet.IsValid(GetContext()));
}


// Class igdeMetaPropertySet::ActionRemove
////////////////////////////////////////////

igdeMetaPropertySet::ActionRemove::ActionRemove(igdeMetaPropertySet &property,
	igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.Remove",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"@Igde.MetaPropertyList.Action.Remove.ToolTip"),
pPropertySet(property){
}

void igdeMetaPropertySet::ActionRemove::OnAction(){
	const auto &context = GetContext();
	if(!pPropertySet.IsValid(context)){
		return;
	}
	
	if(pPropertySet.GetMultiSelection()){
		const auto selection = pPropertySet.GetSelection(context);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pPropertySet.GetPropertyValue(context);
		selection.Visit([&](const deObject::Ref &data){
			newValue.Remove(data);
		});
		
		pPropertySet.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertySet));
		
	}else{
		const auto active = pPropertySet.GetActiveObject(context);
		if(!active){
			return;
		}
		
		auto newValue = pPropertySet.GetPropertyValue(context);
		newValue.Remove(active);
		
		pPropertySet.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertySet));
	};
}

void igdeMetaPropertySet::ActionRemove::Update(){
	const auto &context = GetContext();
	SetEnabled(pPropertySet.IsValid(context) && pPropertySet.GetActiveObject(context));
}


// Class igdeMetaPropertySet::ActionRemoveAll
///////////////////////////////////////////////

igdeMetaPropertySet::ActionRemoveAll::ActionRemoveAll(igdeMetaPropertySet &property,
	igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.RemoveAll",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiDelete),
	"@Igde.MetaPropertyList.Action.RemoveAll.ToolTip"),
pPropertySet(property){
}

void igdeMetaPropertySet::ActionRemoveAll::OnAction(){
	const auto &context = GetContext();
	if(pPropertySet.IsValid(context) && pPropertySet.GetActiveObject(context)){
		pPropertySet.ChangePropertyValue(context, {}, BuildUndoInfo(pPropertySet));
	}
}

void igdeMetaPropertySet::ActionRemoveAll::Update(){
	const auto &context = GetContext();
	SetEnabled(pPropertySet.IsValid(context) && pPropertySet.GetActiveObject(context));
}


// Class igdeMetaPropertySet
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertySet::igdeMetaPropertySet(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description),
pRows(4),
pMultiSelection(false){
}

igdeMetaPropertySet::~igdeMetaPropertySet() = default;


// Management
///////////////

void igdeMetaPropertySet::SetDefaultValue(const Set &value){
	pDefaultValue = value;
}

void igdeMetaPropertySet::SetRows(int rows){
	pRows = decMath::max(rows, 1);
}

void igdeMetaPropertySet::SetMultiSelection(bool multiSelection){
	pMultiSelection = multiSelection;
}

void igdeMetaPropertySet::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertySet::NotifyActiveChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnActiveChanged(this, context);
	});
}

void igdeMetaPropertySet::NotifySelectionChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnSelectionChanged(this, context);
	});
}


igdeMetaPropertySetUndo::Ref igdeMetaPropertySet::ChangePropertyValue(
const igdeMetaContext::Ref &context, const Set &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertySetUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertySet::Set igdeMetaPropertySet::GetSelection(
const igdeMetaContext::Ref &context) const{
	auto active = GetActiveObject(context);
	return active ? Set(devctag, active) : Set();
}

void igdeMetaPropertySet::SetSelection(const igdeMetaContext::Ref&, const Set&){
}

igdeMetaPropertySet::Set igdeMetaPropertySet::GetValidObjects(
const igdeMetaContext::Ref &context) const{
	return {};
}

igdeMetaPropertySet::ObjectRef igdeMetaPropertySet::CopyObject(const ContextRef &context,
const Set &existingObjects, const ObjectRef &object) const{
	return {};
}

igdeMetaProperty::Action::Ref igdeMetaPropertySet::CreateButtonAction(TargetButton, igdeWidget&){
	return {};
}

igdeMetaPropertyWidget::Ref igdeMetaPropertySet::CreateWidget(){
	return igdeMetaPropertySetWidget::Ref::New(*this);
}

void igdeMetaPropertySet::AddDefaultContextMenuEntries(igdeMenuCascade &menu,
const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, ActionRemove::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionRemoveAll::Ref::New(*this, owner, context));
}

igdeMetaPropertySet::Action::Ref igdeMetaPropertySet::CreateDefaultButtonAction(
TargetButton target, igdeWidget &owner){
	switch(target){
	case TargetButton::remove:
		return ActionRemove::Ref::New(*this, owner);
		
	default:
		return {};
	}
}
