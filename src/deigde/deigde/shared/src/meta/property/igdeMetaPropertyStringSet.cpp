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

#include "igdeMetaPropertyStringSet.h"
#include "undo/igdeMetaPropertyStringSetUndo.h"
#include "widget/igdeMetaPropertyStringSetWidget.h"
#include "../igdeMetaContext.h"
#include "../../gui/igdeUIHelper.h"
#include "../../gui/igdeCommonDialogs.h"
#include "../../gui/dialog/igdeDialogSetSelect.h"
#include "../../environment/igdeEnvironment.h"
#include "../../localization/igdeTranslationManager.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyStringSet::Listener
//////////////////////////////////////////////

void igdeMetaPropertyStringSet::Listener::OnActiveChanged(
igdeMetaPropertyStringSet*, const igdeMetaContext::Ref&){
}

void igdeMetaPropertyStringSet::Listener::OnSelectionChanged(
igdeMetaPropertyStringSet*, const igdeMetaContext::Ref&){
}


// Class igdeMetaPropertyStringSet::ActionAdd
///////////////////////////////////////////////

igdeMetaPropertyStringSet::ActionAdd::ActionAdd(igdeMetaPropertyStringSet &property,
	igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.Add",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
	"@Igde.MetaPropertyList.Action.Add.ToolTip"),
pPropertyStringSet(property){
}

void igdeMetaPropertyStringSet::ActionAdd::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringSet.IsValid(context)){
		return;
	}
	
	const decStringSet candidates = pPropertyStringSet.GetAllowedStrings(context);
	if(candidates.IsEmpty()){
		return;
	}
	
	auto &owner = GetOwner();
	auto &environment = GetEnvironment();
	auto dialog = igdeDialogSetSelect::Ref::New(environment,
		"@Igde.MetaPropertyList.Action.Dialog.AddEntries.Title",
		"@Igde.MetaPropertyList.Action.Dialog.AddEntries.Message", candidates);
	
	auto iconPresent = environment.GetStockIcon(igdeEnvironment::esiSmallPlus);
	auto iconAbsent = environment.GetStockIcon(igdeEnvironment::esiSmallMinus);
	
	const auto oldValue = pPropertyStringSet.GetPropertyValue(context);
	dialog->MarkItems(oldValue, iconPresent, iconAbsent);
	if(!dialog->Run(&owner)){
		return;
	}
	
	const auto newValue = oldValue + dialog->GetSelection();
	if(newValue == oldValue){
		return;
	}
	
	pPropertyStringSet.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringSet));
}

void igdeMetaPropertyStringSet::ActionAdd::Update(){
	SetEnabled(pPropertyStringSet.IsValid(GetContext()));
}


// Class igdeMetaPropertyStringSet::ActionRemove
//////////////////////////////////////////////////

igdeMetaPropertyStringSet::ActionRemove::ActionRemove(igdeMetaPropertyStringSet &property,
	igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.Remove",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"@Igde.MetaPropertyList.Action.Remove.ToolTip"),
pPropertyStringSet(property){
}

void igdeMetaPropertyStringSet::ActionRemove::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringSet.IsValid(context)){
		return;
	}
	
	if(pPropertyStringSet.GetMultiSelection()){
		const auto selection = pPropertyStringSet.GetSelection(context);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pPropertyStringSet.GetPropertyValue(context);
		selection.Visit([&](const decString &string){
			newValue.Remove(string);
		});
		
		pPropertyStringSet.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringSet));
		
	}else{
		const auto active = pPropertyStringSet.GetActiveString(context);
		if(!active){
			return;
		}
		
		auto newValue = pPropertyStringSet.GetPropertyValue(context);
		newValue.Remove(active->GetData());
		
		pPropertyStringSet.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringSet));
	};
}

void igdeMetaPropertyStringSet::ActionRemove::Update(){
	const auto &context = GetContext();
	SetEnabled(pPropertyStringSet.IsValid(context) && pPropertyStringSet.GetActiveString(context));
}


// Class igdeMetaPropertyStringSet::ActionRemoveAll
/////////////////////////////////////////////////////

igdeMetaPropertyStringSet::ActionRemoveAll::ActionRemoveAll(
	igdeMetaPropertyStringSet &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.RemoveAll",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiDelete),
	"@Igde.MetaPropertyList.Action.RemoveAll.ToolTip"),
pPropertyStringSet(property){
}

void igdeMetaPropertyStringSet::ActionRemoveAll::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringSet.IsValid(context) || pPropertyStringSet.GetPropertyValue(context).IsEmpty()){
		return;
	}
	
	pPropertyStringSet.ChangePropertyValue(context, {}, BuildUndoInfo(pPropertyStringSet));
}

void igdeMetaPropertyStringSet::ActionRemoveAll::Update(){
	const auto &context = GetContext();
	SetEnabled(pPropertyStringSet.IsValid(context) && pPropertyStringSet.GetPropertyValue(context).IsNotEmpty());
}


// Class igdeMetaPropertyStringSet::ActionExportAsText
////////////////////////////////////////////////////////

igdeMetaPropertyStringSet::ActionExportAsText::ActionExportAsText(
	igdeMetaPropertyStringSet &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.ExportAsText",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
	"@Igde.MetaPropertyList.Action.ExportAsText.ToolTip"),
pPropertyStringSet(property){
}

void igdeMetaPropertyStringSet::ActionExportAsText::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringSet.IsValid(context)){
		return;
	}
	
	auto text = DEJoin(decStringList(pPropertyStringSet.GetPropertyValue(context)).GetSortedAscending(), "\n");
	igdeCommonDialogs::GetMultilineString(GetOwner(),
		"@Igde.MetaPropertyList.Dialog.ExportAsText.Title",
		"@Igde.MetaPropertyList.Dialog.ExportAsText.Message", text);
}

void igdeMetaPropertyStringSet::ActionExportAsText::Update(){
	SetEnabled(pPropertyStringSet.IsValid(GetContext()));
}


// Class igdeMetaPropertyStringSet::ActionImportFromText
//////////////////////////////////////////////////////////

igdeMetaPropertyStringSet::ActionImportFromText::ActionImportFromText(
	igdeMetaPropertyStringSet &property, igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.ImportFromText",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
	"@Igde.MetaPropertyList.Action.ImportFromText.ToolTip"),
pPropertyStringSet(property){
}

void igdeMetaPropertyStringSet::ActionImportFromText::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringSet.IsValid(context)){
		return;
	}
	
	decString text;
	while(true){
		if(!igdeCommonDialogs::GetMultilineString(GetOwner(),
		"@Igde.MetaPropertyList.Dialog.ImportFromText.Title",
		"@Igde.MetaPropertyList.Dialog.ImportFromText.Message", text)){
			return;
		}
		break;
	}
	
	const auto oldValue = pPropertyStringSet.GetPropertyValue(context);
	const auto newValue = oldValue + decStringSet(text.Split('\n').Collect([](const decString &line){
		return !line.GetTrimmed().IsEmpty();
	}));
	
	if(newValue != oldValue){
		pPropertyStringSet.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringSet));
	}
}

void igdeMetaPropertyStringSet::ActionImportFromText::Update(){
	SetEnabled(pPropertyStringSet.IsValid(GetContext()));
}


// Class igdeMetaPropertyStringSet
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyStringSet::igdeMetaPropertyStringSet(const char *id, const char *name,
	const char *description) :
igdeMetaProperty(id, name, description),
pRows(4),
pMultiSelection(false)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyStringSet::igdeMetaPropertyStringSet(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag),
pRows(4),
pMultiSelection(false)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyStringSet::~igdeMetaPropertyStringSet() = default;


// Management
///////////////

void igdeMetaPropertyStringSet::SetDefaultValue(const decStringSet &value){
	pDefaultValue = value;
}

void igdeMetaPropertyStringSet::SetRows(int rows){
	pRows = decMath::max(rows, 1);
}

void igdeMetaPropertyStringSet::SetMultiSelection(bool multiSelection){
	pMultiSelection = multiSelection;
}

void igdeMetaPropertyStringSet::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyStringSet::NotifyActiveChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnActiveChanged(this, context);
	});
}

void igdeMetaPropertyStringSet::NotifySelectionChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnSelectionChanged(this, context);
	});
}


igdeMetaPropertyStringSetUndo::Ref igdeMetaPropertyStringSet::ChangePropertyValue(
const igdeMetaContext::Ref &context, const decStringSet &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyStringSetUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

decStringSet igdeMetaPropertyStringSet::GetSelection(const igdeMetaContext::Ref &context) const{
	auto active = GetActiveString(context);
	return active ? decStringSet(devctag, active->GetData()) : decStringSet();
}

void igdeMetaPropertyStringSet::SetSelection(const igdeMetaContext::Ref&, const decStringSet&){
}

void igdeMetaPropertyStringSet::GetStringItemInfo(const ContextRef &context,
const decString &string, igdeMetaContextItemInfo &info) const{
	info.SetAll(string);
}

decStringSet igdeMetaPropertyStringSet::GetAllowedStrings(const igdeMetaContext::Ref&) const{
	return {};
}

igdeMetaProperty::Action::Ref igdeMetaPropertyStringSet::CreateButtonAction(
TargetButton target, igdeWidget &owner){
	return CreateDefaultButtonAction(target, owner);
}

void igdeMetaPropertyStringSet::AddContextMenuEntries(igdeMenuCascade &contextMenu,
const ContextRef &context, igdeWidget &owner){
	AddDefaultContextMenuEntries(contextMenu, context, owner);
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyStringSet::CreateWidget(){
	return igdeMetaPropertyStringSetWidget::Ref::New(*this);
}

void igdeMetaPropertyStringSet::AddDefaultContextMenuEntries(igdeMenuCascade &menu,
const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, ActionRemove::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionRemoveAll::Ref::New(*this, owner, context));
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, ActionExportAsText::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionImportFromText::Ref::New(*this, owner, context));
}

igdeMetaPropertyStringSet::Action::Ref igdeMetaPropertyStringSet::CreateDefaultButtonAction(
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


// igdeMetaPropertyStringSetStorage
/////////////////////////////////////

igdeMetaPropertyStringSetStorage::igdeMetaPropertyStringSetStorage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyStringSet(id, name, description){
}

igdeMetaPropertyStringSetStorage::igdeMetaPropertyStringSetStorage(
	const char *id, const char *translationTag) :
igdeMetaPropertyStringSet(id, translationTag){
}

igdeMetaPropertyStringSetStorage::~igdeMetaPropertyStringSetStorage() = default;

decStringSet igdeMetaPropertyStringSetStorage::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyStringSetStorage::SetPropertyValue(
const igdeMetaContext::Ref &context, const decStringSet &value){
	GetStorage(context).SetValue(value);
}

igdeMetaPropertyStringSet::StringRef igdeMetaPropertyStringSetStorage::GetActiveString(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetActive();
}

void igdeMetaPropertyStringSetStorage::SetActiveString(
const igdeMetaContext::Ref &context, const StringRef &activeString){
	GetStorage(context).SetActive(activeString);
}

decStringSet igdeMetaPropertyStringSetStorage::GetSelection(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetSelection();
}

void igdeMetaPropertyStringSetStorage::SetSelection(
const igdeMetaContext::Ref &context, const decStringSet &selection){
	GetStorage(context).SetSelection(selection);
}
