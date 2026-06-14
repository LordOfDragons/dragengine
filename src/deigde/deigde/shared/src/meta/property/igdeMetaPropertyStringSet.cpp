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
	const igdeMetaContext::Ref &context, igdeWidget &owner) :
igdeAction("@Igde.MetaPropertyList.Action.Add",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
	"@Igde.MetaPropertyList.Action.Add.ToolTip"),
pProperty(property),
pContext(context),
pOwner(owner){
}

void igdeMetaPropertyStringSet::ActionAdd::OnAction(){
	if(!pProperty.IsValid(pContext)){
		return;
	}
	
	const decStringSet candidates = pProperty.GetValidStrings(pContext);
	if(candidates.IsEmpty()){
		return;
	}
	
	auto &environment = pOwner.GetEnvironment();
	auto dialog = igdeDialogSetSelect::Ref::New(environment,
		"@Igde.MetaPropertyList.Action.Dialog.AddEntries.Title",
		"@Igde.MetaPropertyList.Action.Dialog.AddEntries.Message", candidates);
	
	auto iconPresent = environment.GetStockIcon(igdeEnvironment::esiSmallPlus);
	auto iconAbsent = environment.GetStockIcon(igdeEnvironment::esiSmallMinus);
	
	const auto oldValue = pProperty.GetPropertyValue(pContext);
	dialog->MarkItems(oldValue, iconPresent, iconAbsent);
	if(!dialog->Run(&pOwner)){
		return;
	}
	
	const auto newValue = oldValue + dialog->GetSelection();
	if(newValue == oldValue){
		return;
	}
	
	const auto &tm = environment.GetTranslationManager();
	pProperty.ChangePropertyValue(pContext, newValue,
		tm.TranslateIf(pProperty.GetUndoInfoOrLabel()).ToUTF8()
			+ ": " + tm.TranslateIf(GetText()).ToUTF8());
}

void igdeMetaPropertyStringSet::ActionAdd::Update(){
	SetEnabled(pProperty.IsValid(pContext));
}


// Class igdeMetaPropertyStringSet::ActionRemove
//////////////////////////////////////////////////

igdeMetaPropertyStringSet::ActionRemove::ActionRemove(igdeMetaPropertyStringSet &property,
	const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
igdeAction("@Igde.MetaPropertyList.Action.Remove",
	environment.GetStockIcon(igdeEnvironment::esiMinus),
	"@Igde.MetaPropertyList.Action.Remove.ToolTip"),
pProperty(property),
pContext(context),
pEnvironment(environment){
}

void igdeMetaPropertyStringSet::ActionRemove::OnAction(){
	if(!pProperty.IsValid(pContext)){
		return;
	}
	
	if(pProperty.GetMultiSelection()){
		const auto selection = pProperty.GetSelection(pContext);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValue(pContext);
		selection.Visit([&](const decString &string){
			newValue.Remove(string);
		});
		
		const auto &tm = pEnvironment.GetTranslationManager();
		pProperty.ChangePropertyValue(pContext, newValue,
			tm.TranslateIf(pProperty.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(GetText()).ToUTF8());
		
	}else{
		const auto active = pProperty.GetActiveString(pContext);
		if(!active){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValue(pContext);
		newValue.Remove(active->GetData());
		
		const auto &tm = pEnvironment.GetTranslationManager();
		pProperty.ChangePropertyValue(pContext, newValue,
			tm.TranslateIf(pProperty.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(GetText()).ToUTF8());
	};
}

void igdeMetaPropertyStringSet::ActionRemove::Update(){
	SetEnabled(pProperty.IsValid(pContext) && pProperty.GetActiveString(pContext));
}


// Class igdeMetaPropertyStringSet::ActionRemoveAll
/////////////////////////////////////////////////////

igdeMetaPropertyStringSet::ActionRemoveAll::ActionRemoveAll(
	igdeMetaPropertyStringSet &property, const igdeMetaContext::Ref &context,
	igdeEnvironment &environment) :
igdeAction("@Igde.MetaPropertyList.Action.RemoveAll",
	environment.GetStockIcon(igdeEnvironment::esiDelete),
	"@Igde.MetaPropertyList.Action.RemoveAll.ToolTip"),
pProperty(property),
pContext(context),
pEnvironment(environment){
}

void igdeMetaPropertyStringSet::ActionRemoveAll::OnAction(){
	if(!pProperty.IsValid(pContext) || pProperty.GetPropertyValue(pContext).IsEmpty()){
		return;
	}
	
	const auto &tm = pEnvironment.GetTranslationManager();
	pProperty.ChangePropertyValue(pContext, {},
		tm.TranslateIf(pProperty.GetUndoInfoOrLabel()).ToUTF8()
			+ ": " + tm.TranslateIf(GetText()).ToUTF8());
}

void igdeMetaPropertyStringSet::ActionRemoveAll::Update(){
	SetEnabled(pProperty.IsValid(pContext) && pProperty.GetPropertyValue(pContext).IsNotEmpty());
}


// Class igdeMetaPropertyStringSet::ActionExportAsText
////////////////////////////////////////////////////////

igdeMetaPropertyStringSet::ActionExportAsText::ActionExportAsText(
	igdeMetaPropertyStringSet &property, const igdeMetaContext::Ref &context, igdeWidget &owner) :
igdeAction("@Igde.MetaPropertyList.Action.ExportAsText",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
	"@Igde.MetaPropertyList.Action.ExportAsText.ToolTip"),
pProperty(property),
pContext(context),
pOwner(owner){
}

void igdeMetaPropertyStringSet::ActionExportAsText::OnAction(){
	if(!pProperty.IsValid(pContext)){
		return;
	}
	
	auto text = DEJoin(decStringList(pProperty.GetPropertyValue(pContext)).GetSortedAscending(), "\n");
	igdeCommonDialogs::GetMultilineString(pOwner, "@Igde.MetaPropertyList.Dialog.ExportAsText.Title",
		"@Igde.MetaPropertyList.Dialog.ExportAsText.Message", text);
}

void igdeMetaPropertyStringSet::ActionExportAsText::Update(){
	SetEnabled(pProperty.IsValid(pContext));
}


// Class igdeMetaPropertyStringSet::ActionImportFromText
//////////////////////////////////////////////////////////

igdeMetaPropertyStringSet::ActionImportFromText::ActionImportFromText(
	igdeMetaPropertyStringSet &property, const igdeMetaContext::Ref &context, igdeWidget &owner) :
igdeAction("@Igde.MetaPropertyList.Action.ImportFromText",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
	"@Igde.MetaPropertyList.Action.ImportFromText.ToolTip"),
pProperty(property),
pContext(context),
pOwner(owner){
}

void igdeMetaPropertyStringSet::ActionImportFromText::OnAction(){
	if(!pProperty.IsValid(pContext)){
		return;
	}
	
	decString text;
	while(true){
		if(!igdeCommonDialogs::GetMultilineString(pOwner,
		"@Igde.MetaPropertyList.Dialog.ImportFromText.Title",
		"@Igde.MetaPropertyList.Dialog.ImportFromText.Message", text)){
			return;
		}
		break;
	}
	
	const auto oldValue = pProperty.GetPropertyValue(pContext);
	const auto newValue = oldValue + decStringSet(text.Split('\n').Collect([](const decString &line){
		return !line.GetTrimmed().IsEmpty();
	}));
	
	if(newValue != oldValue){
		const auto &tm = pOwner.GetEnvironment().GetTranslationManager();
		pProperty.ChangePropertyValue(pContext, newValue,
			tm.TranslateIf(pProperty.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(GetText()).ToUTF8());
	}
}

void igdeMetaPropertyStringSet::ActionImportFromText::Update(){
	SetEnabled(pProperty.IsValid(pContext));
}


// Class igdeMetaPropertyStringSet
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyStringSet::igdeMetaPropertyStringSet(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description),
pRows(4),
pMultiSelection(false){
}

igdeMetaPropertyStringSet::~igdeMetaPropertyStringSet() = default;


// Management
///////////////

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

decStringSet igdeMetaPropertyStringSet::GetValidStrings(const igdeMetaContext::Ref&) const{
	return {};
}

igdeAction::Ref igdeMetaPropertyStringSet::CreateButtonAction(
TargetButton target, const igdeMetaContext::Ref &context, igdeWidget &owner){
	return CreateDefaultButtonAction(target, context, owner);
}

void igdeMetaPropertyStringSet::AddContextMenuEntries(igdeMenuCascade &contextMenu,
const ContextRef &context, igdeWidget &owner){
	AddDefaultContextMenuEntries(contextMenu, context, owner);
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyStringSet::CreateWidget(
const igdeMetaContext::Ref &context){
	return igdeMetaPropertyStringSetWidget::Ref::New(*this, context);
}

void igdeMetaPropertyStringSet::AddDefaultContextMenuEntries(igdeMenuCascade &menu,
const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, ActionRemove::Ref::New(*this, context, owner.GetEnvironment()));
	helper.MenuCommand(menu, ActionRemoveAll::Ref::New(*this, context, owner.GetEnvironment()));
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, ActionExportAsText::Ref::New(*this, context, owner));
	helper.MenuCommand(menu, ActionImportFromText::Ref::New(*this, context, owner));
}

igdeAction::Ref igdeMetaPropertyStringSet::CreateDefaultButtonAction(TargetButton target,
	const igdeMetaContext::Ref &context, igdeWidget &owner){
	switch(target){
	case TargetButton::add:
		return ActionAdd::Ref::New(*this, context, owner);
		
	case TargetButton::remove:
		return ActionRemove::Ref::New(*this, context, owner.GetEnvironment());
		
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
