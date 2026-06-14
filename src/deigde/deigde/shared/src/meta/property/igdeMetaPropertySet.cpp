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
	const igdeMetaContext::Ref &context, igdeWidget &owner) :
igdeAction("@Igde.MetaPropertyList.Action.Add",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
	"@Igde.MetaPropertyList.Action.Add.ToolTip"),
pProperty(property),
pContext(context),
pOwner(owner){
}

void igdeMetaPropertySet::ActionAdd::OnAction(){
	if(!pProperty.IsValid(pContext)){
		return;
	}
	
	const igdeMetaPropertySet::Set candidates = pProperty.GetValidObjects(pContext);
	if(candidates.IsEmpty()){
		return;
	}
	
	decStringSet candidateNames;
	decTObjectDictionary<deObject> candidateMap;
	igdeMetaContextItemInfo info;
	candidates.Visit([&](const deObject::Ref &object){
		pProperty.GetObjectItemInfo(pContext, object, info);
		candidateNames.Add(info.GetText());
		candidateMap.SetAt(info.GetText(), object);
	});
	
	auto &environment = pOwner.GetEnvironment();
	auto dialog = igdeDialogSetSelect::Ref::New(environment,
		"@Igde.MetaPropertyList.Action.Dialog.AddEntries.Title",
		"@Igde.MetaPropertyList.Action.Dialog.AddEntries.Message", candidateNames);
	
	auto iconPresent = environment.GetStockIcon(igdeEnvironment::esiSmallPlus);
	auto iconAbsent = environment.GetStockIcon(igdeEnvironment::esiSmallMinus);
	
	const auto oldValue = pProperty.GetPropertyValue(pContext);
	decStringSet oldValueNames;
	oldValue.Visit([&](const deObject::Ref &object){
		pProperty.GetObjectItemInfo(pContext, object, info);
		oldValueNames.Add(info.GetText());
	});
	
	dialog->MarkItems(oldValueNames, iconPresent, iconAbsent);
	if(!dialog->Run(&pOwner)){
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
	
	const auto &tm = environment.GetTranslationManager();
	pProperty.ChangePropertyValue(pContext, newValue,
		tm.TranslateIf(pProperty.GetUndoInfoOrLabel()).ToUTF8()
			+ ": " + tm.TranslateIf(GetText()).ToUTF8());
}

void igdeMetaPropertySet::ActionAdd::Update(){
	SetEnabled(pProperty.IsValid(pContext));
}


// Class igdeMetaPropertySet::ActionRemove
////////////////////////////////////////////

igdeMetaPropertySet::ActionRemove::ActionRemove(igdeMetaPropertySet &property,
	const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
igdeAction("@Igde.MetaPropertyList.Action.Remove",
	environment.GetStockIcon(igdeEnvironment::esiMinus),
	"@Igde.MetaPropertyList.Action.Remove.ToolTip"),
pProperty(property),
pContext(context),
pEnvironment(environment){
}

void igdeMetaPropertySet::ActionRemove::OnAction(){
	if(!pProperty.IsValid(pContext)){
		return;
	}
	
	if(pProperty.GetMultiSelection()){
		const auto selection = pProperty.GetSelection(pContext);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValue(pContext);
		selection.Visit([&](const deObject::Ref &data){
			newValue.Remove(data);
		});
		
		const auto &tm = pEnvironment.GetTranslationManager();
		pProperty.ChangePropertyValue(pContext, newValue,
			tm.TranslateIf(pProperty.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(GetText()).ToUTF8());
		
	}else{
		const auto active = pProperty.GetActiveObject(pContext);
		if(!active){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValue(pContext);
		newValue.Remove(active);
		
		const auto &tm = pEnvironment.GetTranslationManager();
		pProperty.ChangePropertyValue(pContext, newValue,
			tm.TranslateIf(pProperty.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(GetText()).ToUTF8());
	};
}

void igdeMetaPropertySet::ActionRemove::Update(){
	SetEnabled(pProperty.IsValid(pContext) && pProperty.GetActiveObject(pContext));
}


// Class igdeMetaPropertySet::ActionRemoveAll
///////////////////////////////////////////////

igdeMetaPropertySet::ActionRemoveAll::ActionRemoveAll(igdeMetaPropertySet &property,
	const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
igdeAction("@Igde.MetaPropertyList.Action.RemoveAll",
	environment.GetStockIcon(igdeEnvironment::esiDelete),
	"@Igde.MetaPropertyList.Action.RemoveAll.ToolTip"),
pProperty(property),
pContext(context),
pEnvironment(environment){
}

void igdeMetaPropertySet::ActionRemoveAll::OnAction(){
	if(pProperty.IsValid(pContext) && pProperty.GetActiveObject(pContext)){
		const auto &tm = pEnvironment.GetTranslationManager();
		pProperty.ChangePropertyValue(pContext, {},
			tm.TranslateIf(pProperty.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(GetText()).ToUTF8());
	}
}

void igdeMetaPropertySet::ActionRemoveAll::Update(){
	SetEnabled(pProperty.IsValid(pContext) && pProperty.GetActiveObject(pContext));
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

igdeAction::Ref igdeMetaPropertySet::CreateButtonAction(
TargetButton, const igdeMetaContext::Ref&, igdeWidget&){
	return {};
}

igdeMetaPropertyWidget::Ref igdeMetaPropertySet::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertySetWidget::Ref::New(*this, context);
}

void igdeMetaPropertySet::AddDefaultContextMenuEntries(igdeMenuCascade &menu,
const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, ActionRemove::Ref::New(*this, context, owner.GetEnvironment()));
	helper.MenuCommand(menu, ActionRemoveAll::Ref::New(*this, context, owner.GetEnvironment()));
}

igdeAction::Ref igdeMetaPropertySet::CreateDefaultButtonAction(TargetButton target,
	const igdeMetaContext::Ref &context, igdeWidget &owner){
	switch(target){
	case TargetButton::remove:
		return ActionRemove::Ref::New(*this, context, owner.GetEnvironment());
		
	default:
		return {};
	}
}
