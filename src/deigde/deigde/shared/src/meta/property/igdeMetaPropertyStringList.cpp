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

#include "igdeMetaPropertyStringList.h"
#include "undo/igdeMetaPropertyStringListUndo.h"
#include "widget/igdeMetaPropertyStringListWidget.h"
#include "../igdeMetaContext.h"
#include "../../gui/igdeUIHelper.h"
#include "../../gui/igdeCommonDialogs.h"
#include "../../gui/dialog/igdeDialogSetSelect.h"
#include "../../environment/igdeEnvironment.h"
#include "../../localization/igdeTranslationManager.h"
#include "../../undo/igdeUndoSystem.h"

#include <dragengine/common/collection/decTList.h>


struct sSortEntry{
	decString entry;
	int index;
};


// Class igdeMetaPropertyStringList::Listener
///////////////////////////////////////////////

void igdeMetaPropertyStringList::Listener::OnActiveChanged(
igdeMetaPropertyStringList*, const igdeMetaContext::Ref&){
}

void igdeMetaPropertyStringList::Listener::OnSelectionChanged(
igdeMetaPropertyStringList*, const igdeMetaContext::Ref&){
}


// Class igdeMetaPropertyStringList::ActionAdd
////////////////////////////////////////////////

igdeMetaPropertyStringList::ActionAdd::ActionAdd(igdeMetaPropertyStringList &property,
		igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.Add",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
	"@Igde.MetaPropertyList.Action.Add.ToolTip"),
pPropertyStringList(property){
}

void igdeMetaPropertyStringList::ActionAdd::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringList.IsValid(context)){
		return;
	}
	
	const decStringSet candidates = pPropertyStringList.GetAllowedStrings(context);
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
	
	const auto oldValue = pPropertyStringList.GetPropertyValue(context);
	decStringSet oldValueSet(oldValue);
	dialog->MarkItems(oldValueSet, iconPresent, iconAbsent);
	if(!dialog->Run(&owner)){
		return;
	}
	
	auto newValue = oldValue;
	dialog->GetSelection().Visit([&](const decString &string){
		if(!newValue.Has(string)){
			newValue.Add(string);
		}
	});
	
	if(newValue == oldValue){
		return;
	}
	
	pPropertyStringList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringList));
}

void igdeMetaPropertyStringList::ActionAdd::Update(){
	SetEnabled(pPropertyStringList.IsValid(GetContext()));
}


// Class igdeMetaPropertyStringList::ActionRemove
///////////////////////////////////////////////////

igdeMetaPropertyStringList::ActionRemove::ActionRemove(igdeMetaPropertyStringList &property,
		igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.Remove",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"@Igde.MetaPropertyList.Action.Remove.ToolTip"),
pPropertyStringList(property){
}

void igdeMetaPropertyStringList::ActionRemove::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringList.IsValid(context)){
		return;
	}
	
	if(pPropertyStringList.GetMultiSelection()){
		const auto selection = pPropertyStringList.GetSelection(context);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pPropertyStringList.GetPropertyValue(context);
		selection.Visit([&](const decString &string){
			const int index = newValue.IndexOf(string);
			if(index != -1){
				newValue.RemoveFrom(index);
			}
		});
		
		pPropertyStringList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringList));
		
	}else{
		const auto active = pPropertyStringList.GetActiveString(context);
		if(!active){
			return;
		}
		
		auto newValue = pPropertyStringList.GetPropertyValue(context);
		const int index = newValue.IndexOf(active->GetData());
		if(index != -1){
			newValue.RemoveFrom(index);
		}
		
		pPropertyStringList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringList));
	};
}

void igdeMetaPropertyStringList::ActionRemove::Update(){
	const auto &context = GetContext();
	SetEnabled(pPropertyStringList.IsValid(context) && pPropertyStringList.GetActiveString(context));
}


// Class igdeMetaPropertyStringList::ActionRemoveAll
///////////////////////////////////////////////////////

igdeMetaPropertyStringList::ActionRemoveAll::ActionRemoveAll(
		igdeMetaPropertyStringList &property, igdeWidget &owner,
		const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.RemoveAll",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiDelete),
	"@Igde.MetaPropertyList.Action.RemoveAll.ToolTip"),
pPropertyStringList(property){
}

void igdeMetaPropertyStringList::ActionRemoveAll::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringList.IsValid(context) || pPropertyStringList.GetPropertyValue(context).IsEmpty()){
		return;
	}
	
	pPropertyStringList.ChangePropertyValue(context, {}, BuildUndoInfo(pPropertyStringList));
}

void igdeMetaPropertyStringList::ActionRemoveAll::Update(){
	const auto &context = GetContext();
	SetEnabled(pPropertyStringList.IsValid(context)
		&& pPropertyStringList.GetPropertyValue(context).IsNotEmpty());
}


// Class igdeMetaPropertyStringList::ActionMoveUp
////////////////////////////////////////////////////

igdeMetaPropertyStringList::ActionMoveUp::ActionMoveUp(igdeMetaPropertyStringList &property,
		igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.MoveUp",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
	"@Igde.MetaPropertyList.Action.MoveUp.ToolTip"),
pPropertyStringList(property),
pTop(false){
}

void igdeMetaPropertyStringList::ActionMoveUp::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringList.IsValid(context)){
		return;
	}
	
	if(pPropertyStringList.GetMultiSelection()){
		const auto selection = pPropertyStringList.GetSelection(context);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pPropertyStringList.GetPropertyValue(context);
		
		decTList<sSortEntry> sortedSelection(selection.GetCount());
		selection.Visit([&](const decString &string){
			sortedSelection.Add({string, newValue.IndexOf(string)});
		});
		sortedSelection.Sort([](const sSortEntry &a, const sSortEntry &b){
			return DECompare(a.index, b.index);
		});
		
		const int distance = pTop ? sortedSelection.First().index : 1;
		bool anyMoved = false;
		
		sortedSelection.VisitIndexed([&](int index, const sSortEntry &entry){
			const int newIndex = decMath::max(entry.index - distance, index);
			if(newIndex != entry.index){
				newValue.Move(entry.index, newIndex);
				anyMoved = true;
			}
		});
		
		if(anyMoved){
			pPropertyStringList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringList));
		}
		
	}else{
		const auto active = pPropertyStringList.GetActiveString(context);
		if(!active){
			return;
		}
		
		auto newValue = pPropertyStringList.GetPropertyValue(context);
		const int index = newValue.IndexOf(active->GetData());
		if(index == 0){
			return;
		}
		
		newValue.Move(index, pTop ? 0 : index - 1);
		
		pPropertyStringList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringList));
	};
}

void igdeMetaPropertyStringList::ActionMoveUp::Update(){
	const auto &context = GetContext();
	bool enabled = false;
	
	if(pPropertyStringList.IsValid(context)){
		if(pPropertyStringList.GetMultiSelection()){
			const auto selection = pPropertyStringList.GetSelection(context);
			if(selection.IsNotEmpty()){
				const auto &list = pPropertyStringList.GetPropertyValue(context);
				
				decTList<sSortEntry> sortedSelection(selection.GetCount());
				selection.Visit([&](const decString &string){
					sortedSelection.Add({string, list.IndexOf(string)});
				});
				sortedSelection.Sort([](const sSortEntry &a, const sSortEntry &b){
					return DECompare(a.index, b.index);
				});
				sortedSelection.VisitWhileIndexed([&](int index, const sSortEntry &entry){
					enabled = entry.index != index;
					return !enabled;
				});
			}
			
		}else{
			const auto active = pPropertyStringList.GetActiveString(context);
			enabled = active && pPropertyStringList.GetPropertyValue(context).First() != active->GetData();
		}
	}
	
	SetEnabled(enabled);
}

void igdeMetaPropertyStringList::ActionMoveUp::SetTop(bool top){
	pTop = top;
}


// Class igdeMetaPropertyStringList::ActionMoveTop
/////////////////////////////////////////////////////

igdeMetaPropertyStringList::ActionMoveTop::ActionMoveTop(igdeMetaPropertyStringList &property,
		igdeWidget &owner, const igdeMetaContext::Ref &context) :
ActionMoveUp(property, owner, context)
{
	SetText("@Igde.MetaPropertyList.Action.MoveTop");
	SetDescription("@Igde.MetaPropertyList.Action.MoveTop.ToolTip");
	SetIcon(owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongUp));
	SetTop(true);
}


// Class igdeMetaPropertyStringList::ActionMoveDown
/////////////////////////////////////////////////////

igdeMetaPropertyStringList::ActionMoveDown::ActionMoveDown(igdeMetaPropertyStringList &property,
		igdeWidget &owner, const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.MoveDown",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
	"@Igde.MetaPropertyList.Action.MoveDown.ToolTip"),
pPropertyStringList(property),
pBottom(false){
}

void igdeMetaPropertyStringList::ActionMoveDown::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringList.IsValid(context)){
		return;
	}
	
	if(pPropertyStringList.GetMultiSelection()){
		const auto selection = pPropertyStringList.GetSelection(context);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pPropertyStringList.GetPropertyValue(context);
		
		decTList<sSortEntry> sortedSelection(selection.GetCount());
		selection.Visit([&](const decString &string){
			sortedSelection.Add({string, newValue.IndexOf(string)});
		});
		sortedSelection.Sort([](const sSortEntry &a, const sSortEntry &b){
			return DECompare(b.index, a.index);
		});
		
		const int bottomIndex = newValue.GetCount() - 1;
		const int distance = pBottom ? bottomIndex - sortedSelection.First().index : 1;
		bool anyMoved = false;
		
		sortedSelection.VisitIndexed([&](int index, const sSortEntry &entry){
			const int newIndex = decMath::min(entry.index + distance, bottomIndex);
			if(newIndex != entry.index){
				newValue.Move(entry.index, newIndex);
				anyMoved = true;
			}
		});
		
		if(anyMoved){
			pPropertyStringList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringList));
		}
		
	}else{
		const auto active = pPropertyStringList.GetActiveString(context);
		if(!active){
			return;
		}
		
		auto newValue = pPropertyStringList.GetPropertyValue(context);
		const int index = newValue.IndexOf(active->GetData());
		const int bottomIndex = newValue.GetCount() - 1;
		if(index == bottomIndex){
			return;
		}
		
		newValue.Move(index, pBottom ? bottomIndex : index + 1);
		
		pPropertyStringList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringList));
	};
}

void igdeMetaPropertyStringList::ActionMoveDown::Update(){
	const auto &context = GetContext();
	bool enabled = false;
	
	if(pPropertyStringList.IsValid(context)){
		if(pPropertyStringList.GetMultiSelection()){
			const auto selection = pPropertyStringList.GetSelection(context);
			if(selection.IsNotEmpty()){
				const auto &list = pPropertyStringList.GetPropertyValue(context);
				
				decTList<sSortEntry> sortedSelection(selection.GetCount());
				selection.Visit([&](const decString &string){
					sortedSelection.Add({string, list.IndexOf(string)});
				});
				sortedSelection.Sort([](const sSortEntry &a, const sSortEntry &b){
					return DECompare(b.index, a.index);
				});
				const int bottomIndex = list.GetCount() - 1;
				sortedSelection.VisitWhileIndexed([&](int index, const sSortEntry &entry){
					enabled = entry.index != bottomIndex - index;
					return !enabled;
				});
			}
			
		}else{
			const auto active = pPropertyStringList.GetActiveString(context);
			enabled = active && pPropertyStringList.GetPropertyValue(context).Last() != active->GetData();
		}
	}
	
	SetEnabled(enabled);
}

void igdeMetaPropertyStringList::ActionMoveDown::SetBottom(bool bottom){
	pBottom = bottom;
}


// Class igdeMetaPropertyStringList::ActionMoveBottom
////////////////////////////////////////////////////////

igdeMetaPropertyStringList::ActionMoveBottom::ActionMoveBottom(
		igdeMetaPropertyStringList &property, igdeWidget &owner,
		const igdeMetaContext::Ref &context) :
ActionMoveDown(property, owner, context)
{
	SetText("@Igde.MetaPropertyList.Action.MoveBottom");
	SetDescription("@Igde.MetaPropertyList.Action.MoveBottom.ToolTip");
	SetIcon(owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongDown));
	SetBottom(true);
}


// Class igdeMetaPropertyStringList::ActionExportAsText
///////////////////////////////////////////////////////////

igdeMetaPropertyStringList::ActionExportAsText::ActionExportAsText(
		igdeMetaPropertyStringList &property, igdeWidget &owner,
		const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.ExportAsText",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave),
	"@Igde.MetaPropertyList.Action.ExportAsText.ToolTip"),
pPropertyStringList(property){
}

void igdeMetaPropertyStringList::ActionExportAsText::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringList.IsValid(context)){
		return;
	}
	
	decString text;
	pPropertyStringList.GetPropertyValue(context).Visit([&](const decString &string){
		if(!text.IsEmpty()){
			text += "\n";
		}
		text += string;
	});
	
	igdeCommonDialogs::GetMultilineString(GetOwner(),
		"@Igde.MetaPropertyList.Dialog.ExportAsText.Title",
		"@Igde.MetaPropertyList.Dialog.ExportAsText.Message", text);
}

void igdeMetaPropertyStringList::ActionExportAsText::Update(){
	SetEnabled(pPropertyStringList.IsValid(GetContext()));
}


// Class igdeMetaPropertyStringList::ActionImportFromText
///////////////////////////////////////////////////////////

igdeMetaPropertyStringList::ActionImportFromText::ActionImportFromText(
		igdeMetaPropertyStringList &property, igdeWidget &owner,
		const igdeMetaContext::Ref &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.ImportFromText",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen),
	"@Igde.MetaPropertyList.Action.ImportFromText.ToolTip"),
pPropertyStringList(property){
}

void igdeMetaPropertyStringList::ActionImportFromText::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyStringList.IsValid(context)){
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
	
	auto newValue = pPropertyStringList.GetPropertyValue(context);
	text.Split('\n').Visit([&](const decString &line){
		const decString trimmed(line.GetTrimmed());
		if(!trimmed.IsEmpty() && !newValue.Has(trimmed)){
			newValue.Add(trimmed);
		}
	});
	
	if(newValue != pPropertyStringList.GetPropertyValue(context)){
		pPropertyStringList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyStringList));
	}
}

void igdeMetaPropertyStringList::ActionImportFromText::Update(){
	SetEnabled(pPropertyStringList.IsValid(GetContext()));
}


// Class igdeMetaPropertyStringList
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyStringList::igdeMetaPropertyStringList(const char *id, const char *name,
	const char *description) :
igdeMetaProperty(id, name, description),
pRows(4),
pMultiSelection(false)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyStringList::igdeMetaPropertyStringList(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag),
pRows(4),
pMultiSelection(false)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyStringList::~igdeMetaPropertyStringList() = default;


// Management
///////////////

void igdeMetaPropertyStringList::SetDefaultValue(const decStringList &value){
	pDefaultValue = value;
}

void igdeMetaPropertyStringList::SetRows(int rows){
	pRows = decMath::max(rows, 1);
}

void igdeMetaPropertyStringList::SetMultiSelection(bool multiSelection){
	pMultiSelection = multiSelection;
}

void igdeMetaPropertyStringList::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyStringList::NotifyActiveChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnActiveChanged(this, context);
	});
}

void igdeMetaPropertyStringList::NotifySelectionChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnSelectionChanged(this, context);
	});
}


igdeMetaPropertyStringListUndo::Ref igdeMetaPropertyStringList::ChangePropertyValue(
const igdeMetaContext::Ref &context, const decStringList &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyStringListUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

decStringList igdeMetaPropertyStringList::GetSelection(const igdeMetaContext::Ref &context) const{
	auto active = GetActiveString(context);
	return active ? decStringList(devctag, active->GetData()) : decStringList();
}

void igdeMetaPropertyStringList::SetSelection(const igdeMetaContext::Ref&, const decStringList&){
}

void igdeMetaPropertyStringList::GetStringItemInfo(const ContextRef &context,
const decString &string, igdeMetaContextItemInfo &info) const{
	info.SetAll(string);
}

decStringSet igdeMetaPropertyStringList::GetAllowedStrings(const igdeMetaContext::Ref&) const{
	return {};
}

igdeMetaProperty::Action::Ref igdeMetaPropertyStringList::CreateButtonAction(
TargetButton target, igdeWidget &owner){
	return CreateDefaultButtonAction(target, owner);
}

void igdeMetaPropertyStringList::AddContextMenuEntries(igdeMenuCascade &contextMenu,
const ContextRef &context, igdeWidget &owner){
	AddDefaultContextMenuEntries(contextMenu, context, owner);
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyStringList::CreateWidget(){
	return igdeMetaPropertyStringListWidget::Ref::New(*this);
}

void igdeMetaPropertyStringList::AddDefaultContextMenuEntries(igdeMenuCascade &menu,
const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, ActionRemove::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionRemoveAll::Ref::New(*this, owner, context));
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, ActionMoveUp::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionMoveDown::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionMoveTop::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionMoveBottom::Ref::New(*this, owner, context));
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, ActionExportAsText::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionImportFromText::Ref::New(*this, owner, context));
}

igdeMetaPropertyStringList::Action::Ref igdeMetaPropertyStringList::CreateDefaultButtonAction(
TargetButton target, igdeWidget &owner){
	switch(target){
	case TargetButton::add:
		return ActionAdd::Ref::New(*this, owner);
		
	case TargetButton::remove:
		return ActionRemove::Ref::New(*this, owner);
		
	case TargetButton::moveUp:
		return ActionMoveUp::Ref::New(*this, owner);
		
	case TargetButton::moveDown:
		return ActionMoveDown::Ref::New(*this, owner);
		
	default:
		return {};
	}
}


// igdeMetaPropertyStringListStorage
///////////////////////////////////////

igdeMetaPropertyStringListStorage::igdeMetaPropertyStringListStorage(
	const char *id, const char *name, const char *description) :
igdeMetaPropertyStringList(id, name, description){
}

igdeMetaPropertyStringListStorage::igdeMetaPropertyStringListStorage(
	const char *id, const char *translationTag) :
igdeMetaPropertyStringList(id, translationTag){
}

igdeMetaPropertyStringListStorage::~igdeMetaPropertyStringListStorage() = default;

decStringList igdeMetaPropertyStringListStorage::GetPropertyValue(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetValue();
}

void igdeMetaPropertyStringListStorage::SetPropertyValue(
const igdeMetaContext::Ref &context, const decStringList &value){
	GetStorage(context).SetValue(value);
}

igdeMetaPropertyStringList::StringRef igdeMetaPropertyStringListStorage::GetActiveString(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetActive();
}

void igdeMetaPropertyStringListStorage::SetActiveString(
const igdeMetaContext::Ref &context, const StringRef &activeString){
	GetStorage(context).SetActive(activeString);
}

decStringList igdeMetaPropertyStringListStorage::GetSelection(
const igdeMetaContext::Ref &context) const{
	return GetStorage(context).GetSelection();
}

void igdeMetaPropertyStringListStorage::SetSelection(
const igdeMetaContext::Ref &context, const decStringList &selection){
	GetStorage(context).SetSelection(selection);
}
