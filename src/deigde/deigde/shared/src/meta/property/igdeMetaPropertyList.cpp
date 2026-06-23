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

#include "igdeMetaPropertyList.h"
#include "undo/igdeMetaPropertyListUndo.h"
#include "widget/igdeMetaPropertyListWidget.h"
#include "../igdeMetaContext.h"
#include "../../environment/igdeEnvironment.h"
#include "../../localization/igdeTranslationManager.h"
#include "../../gui/igdeUIHelper.h"
#include "../../undo/igdeUndoSystem.h"

#include <dragengine/common/collection/decTList.h>


struct sSortEntry{
	deObject::Ref entry{};
	int index = -1;
};


// Class igdeMetaPropertyList::Listener
/////////////////////////////////////////

void igdeMetaPropertyList::Listener::OnActiveChanged(
igdeMetaPropertyList*, const ContextRef&){
}

void igdeMetaPropertyList::Listener::OnSelectionChanged(
igdeMetaPropertyList*, const ContextRef&){
}

void igdeMetaPropertyList::Listener::OnObjectItemInfoChanged(
igdeMetaPropertyList*, const ContextRef&){
}


// Class igdeMetaPropertyList::ActionDuplicate
////////////////////////////////////////////////

igdeMetaPropertyList::ActionDuplicate::ActionDuplicate(igdeMetaPropertyList &property,
	igdeWidget &owner, const ContextRef &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.Duplicate",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiDuplicate),
	"@Igde.MetaPropertyList.Action.Duplicate.ToolTip"),
pPropertyList(property){
}

void igdeMetaPropertyList::ActionDuplicate::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyList.IsValid(context)){
		return;
	}
	
	const auto oldValuedData = pPropertyList.GetPropertyValue(context);
	igdeMetaPropertyList::List copiedObjects;
	auto newValue = oldValuedData;
	
	if(pPropertyList.GetMultiSelection()){
		const auto selection = pPropertyList.GetSelection(context);
		if(selection.IsEmpty()){
			return;
		}
		
		selection.Visit([&](const deObject::Ref &object){
			const auto copiedObject = pPropertyList.CopyObject(context, newValue, object);
			if(copiedObject){
				newValue.Add(copiedObject);
				copiedObjects.Add(copiedObject);
			}
		});
		
	}else{
		const auto active = pPropertyList.GetActiveObject(context);
		if(!active){
			return;
		}
		
		const auto copiedObject = pPropertyList.CopyObject(context, newValue, active);
		if(copiedObject){
			newValue.Add(copiedObject);
			copiedObjects.Add(copiedObject);
		}
	}
	
	if(newValue == oldValuedData){
		return;
	}
	
	pPropertyList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyList));
	
	if(copiedObjects.IsNotEmpty()){
		pPropertyList.SetActiveObject(context, copiedObjects.First());
		
		if(pPropertyList.GetMultiSelection()){
			pPropertyList.SetSelection(context, copiedObjects);
		}
	}
}

void igdeMetaPropertyList::ActionDuplicate::Update(){
	const auto &context = GetContext();
	SetEnabled(pPropertyList.IsValid(context) && pPropertyList.GetActiveObject(context));
}


// Class igdeMetaPropertyList::ActionRemove
/////////////////////////////////////////////

igdeMetaPropertyList::ActionRemove::ActionRemove(igdeMetaPropertyList &property,
	igdeWidget &owner, const ContextRef &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.Remove",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
	"@Igde.MetaPropertyList.Action.Remove.ToolTip"),
pPropertyList(property){
}

void igdeMetaPropertyList::ActionRemove::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyList.IsValid(context)){
		return;
	}
	
	if(pPropertyList.GetMultiSelection()){
		const auto selection = pPropertyList.GetSelection(context);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pPropertyList.GetPropertyValue(context);
		selection.Visit([&](const deObject::Ref &data){
			newValue.Remove(data);
		});
		
		pPropertyList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyList));
		
	}else{
		const auto active = pPropertyList.GetActiveObject(context);
		if(!active){
			return;
		}
		
		auto newValue = pPropertyList.GetPropertyValue(context);
		newValue.Remove(active);
		
		pPropertyList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyList));
	};
}

void igdeMetaPropertyList::ActionRemove::Update(){
	const auto &context = GetContext();
	SetEnabled(pPropertyList.IsValid(context) && pPropertyList.GetActiveObject(context));
}


// Class igdeMetaPropertyList::ActionRemoveAll
////////////////////////////////////////////////

igdeMetaPropertyList::ActionRemoveAll::ActionRemoveAll(igdeMetaPropertyList &property,
	igdeWidget &owner, const ContextRef &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.RemoveAll",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiDelete),
	"@Igde.MetaPropertyList.Action.RemoveAll.ToolTip"),
pPropertyList(property){
}

void igdeMetaPropertyList::ActionRemoveAll::OnAction(){
	const auto &context = GetContext();
	if(pPropertyList.IsValid(context) && pPropertyList.GetActiveObject(context)){
		pPropertyList.ChangePropertyValue(context, {}, BuildUndoInfo(pPropertyList));
	}
}

void igdeMetaPropertyList::ActionRemoveAll::Update(){
	const auto &context = GetContext();
	SetEnabled(pPropertyList.IsValid(context) && pPropertyList.GetActiveObject(context));
}


// Class igdeMetaPropertyList::ActionMoveUp
/////////////////////////////////////////////

igdeMetaPropertyList::ActionMoveUp::ActionMoveUp(igdeMetaPropertyList &property,
	igdeWidget &owner, const ContextRef &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.MoveUp",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp),
	"@Igde.MetaPropertyList.Action.MoveUp.ToolTip"),
pPropertyList(property),
pTop(false){
}

void igdeMetaPropertyList::ActionMoveUp::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyList.IsValid(context)){
		return;
	}
	
	if(pPropertyList.GetMultiSelection()){
		const auto selection = pPropertyList.GetSelection(context);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pPropertyList.GetPropertyValue(context);
		
		decTList<sSortEntry> sortedSelection(selection.GetCount());
		selection.Visit([&](const deObject::Ref &data){
			sortedSelection.Add({data, newValue.IndexOf(data)});
		});
		sortedSelection.Sort([](const sSortEntry &a, const sSortEntry &b){
			return DECompare(a.index, b.index);
		});
		
		const int distance = pTop ? sortedSelection.First().index : 1;
		bool anyMoved = false;
		
		sortedSelection.VisitIndexed([&](int index, const sSortEntry &entry){
			const int newIndex = decMath::max(entry.index - distance, index);
			if(newIndex != entry.index){
				newValue.MoveIndex(entry.index, newIndex);
				anyMoved = true;
			}
		});
		
		if(anyMoved){
			pPropertyList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyList));
		}
		
	}else{
		const auto active = pPropertyList.GetActiveObject(context);
		if(!active){
			return;
		}
		
		auto newValue = pPropertyList.GetPropertyValue(context);
		const int index = newValue.IndexOf(active);
		if(index == 0){
			return;
		}
		
		newValue.MoveIndex(index, pTop ? 0 : index - 1);
		
		pPropertyList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyList));
	};
}

void igdeMetaPropertyList::ActionMoveUp::Update(){
	const auto &context = GetContext();
	bool enabled = false;
	
	if(pPropertyList.IsValid(context)){
		if(pPropertyList.GetMultiSelection()){
			const auto selection = pPropertyList.GetSelection(context);
			if(selection.IsNotEmpty()){
				const auto list = pPropertyList.GetPropertyValue(context);
				
				decTList<sSortEntry> sortedSelection(selection.GetCount());
				selection.Visit([&](const deObject::Ref &data){
					sortedSelection.Add({data, list.IndexOf(data)});
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
			const auto active = pPropertyList.GetActiveObject(context);
			enabled = active && pPropertyList.GetPropertyValue(context).First() != active;
		}
	}
	
	SetEnabled(enabled);
}

void igdeMetaPropertyList::ActionMoveUp::SetTop(bool top){
	pTop = top;
}


// Class igdeMetaPropertyList::ActionMoveTop
//////////////////////////////////////////////

igdeMetaPropertyList::ActionMoveTop::ActionMoveTop(igdeMetaPropertyList &property,
	igdeWidget &owner, const ContextRef &context) :
ActionMoveUp(property, owner, context)
{
	SetText("@Igde.MetaPropertyList.Action.MoveTop");
	SetDescription("@Igde.MetaPropertyList.Action.MoveTop.ToolTip");
	SetIcon(owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongUp));
	SetTop(true);
}


// Class igdeMetaPropertyList::ActionMoveDown
///////////////////////////////////////////////

igdeMetaPropertyList::ActionMoveDown::ActionMoveDown(igdeMetaPropertyList &property,
	igdeWidget &owner, const ContextRef &context) :
Action(owner, context, "@Igde.MetaPropertyList.Action.MoveDown",
	owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown),
	"@Igde.MetaPropertyList.Action.MoveDown.ToolTip"),
pPropertyList(property),
pBottom(false){
}

void igdeMetaPropertyList::ActionMoveDown::OnAction(){
	const auto &context = GetContext();
	if(!pPropertyList.IsValid(context)){
		return;
	}
	
	if(pPropertyList.GetMultiSelection()){
		const auto selection = pPropertyList.GetSelection(context);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pPropertyList.GetPropertyValue(context);
		
		decTList<sSortEntry> sortedSelection(selection.GetCount());
		selection.Visit([&](const deObject::Ref &data){
			sortedSelection.Add({data, newValue.IndexOf(data)});
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
				newValue.MoveIndex(entry.index, newIndex);
				anyMoved = true;
			}
		});
		
		if(anyMoved){
			pPropertyList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyList));
		}
		
	}else{
		const auto active = pPropertyList.GetActiveObject(context);
		if(!active){
			return;
		}
		
		auto newValue = pPropertyList.GetPropertyValue(context);
		const int index = newValue.IndexOf(active);
		const int bottomIndex = newValue.GetCount() - 1;
		if(index == bottomIndex){
			return;
		}
		
		newValue.MoveIndex(index, pBottom ? bottomIndex : index + 1);
		
		pPropertyList.ChangePropertyValue(context, newValue, BuildUndoInfo(pPropertyList));
	};
}

void igdeMetaPropertyList::ActionMoveDown::Update(){
	const auto &context = GetContext();
	bool enabled = false;
	
	if(pPropertyList.IsValid(context)){
		if(pPropertyList.GetMultiSelection()){
			const auto selection = pPropertyList.GetSelection(context);
			if(selection.IsNotEmpty()){
				const auto list = pPropertyList.GetPropertyValue(context);
				
				decTList<sSortEntry> sortedSelection(selection.GetCount());
				selection.Visit([&](const deObject::Ref &data){
					sortedSelection.Add({data, list.IndexOf(data)});
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
			const auto active = pPropertyList.GetActiveObject(context);
			enabled = active && pPropertyList.GetPropertyValue(context).Last() != active;
		}
	}
	
	SetEnabled(enabled);
}

void igdeMetaPropertyList::ActionMoveDown::SetBottom(bool bottom){
	pBottom = bottom;
}


// Class igdeMetaPropertyList::ActionMoveBottom
/////////////////////////////////////////////////

igdeMetaPropertyList::ActionMoveBottom::ActionMoveBottom(igdeMetaPropertyList &property,
	igdeWidget &owner, const ContextRef &context) :
ActionMoveDown(property, owner, context)
{
	SetText("@Igde.MetaPropertyList.Action.MoveBottom");
	SetDescription("@Igde.MetaPropertyList.Action.MoveBottom.ToolTip");
	SetIcon(owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiStrongDown));
	SetBottom(true);
}


// Class igdeMetaPropertyList
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyList::igdeMetaPropertyList(const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyList::igdeMetaPropertyList(const char *id, const char *translationTag) :
igdeMetaProperty(id, translationTag)
{
	SetClipboardDataTypeName(ClipboardData::TypeName);
}

igdeMetaPropertyList::~igdeMetaPropertyList() = default;


// Management
///////////////

void igdeMetaPropertyList::SetRows(int rows){
	pRows = decMath::max(rows, 1);
}

void igdeMetaPropertyList::SetMultiSelection(bool multiSelection){
	pMultiSelection = multiSelection;
}

void igdeMetaPropertyList::SetSorted(bool sorted){
	pSorted = sorted;
}

void igdeMetaPropertyList::NotifyValueChanged(const ContextRef &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyList::NotifyActiveChanged(const ContextRef &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnActiveChanged(this, context);
	});
}

void igdeMetaPropertyList::NotifySelectionChanged(const ContextRef &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnSelectionChanged(this, context);
	});
}

void igdeMetaPropertyList::NotifyObjectItemInfoChanged(const ContextRef &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnObjectItemInfoChanged(this, context);
	});
}


igdeMetaPropertyListUndo::Ref igdeMetaPropertyList::ChangePropertyValue(
const ContextRef &context, const List &newValue,
const char *undoInfo, const char *undoInfoLong){
	if(context->GetUndoSystem()){
		const auto undo = igdeMetaPropertyListUndo::Ref::New(
			*this, context, newValue, undoInfo, undoInfoLong);
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyList::List igdeMetaPropertyList::GetSelection(const ContextRef &context) const{
	auto active = GetActiveObject(context);
	return active ? List(devctag, active) : List();
}

void igdeMetaPropertyList::SetSelection(const ContextRef &context, const List &selection){
}

igdeMetaProperty::Action::Ref igdeMetaPropertyList::CreateButtonAction(TargetButton, igdeWidget&){
	return {};
}

void igdeMetaPropertyList::AddContextMenuEntries(igdeMenuCascade &contextMenu,
const ContextRef &context, igdeWidget &owner){
	AddDefaultContextMenuEntries(contextMenu, context, owner);
}

igdeMetaPropertyList::ObjectRef igdeMetaPropertyList::CopyObject(
const ContextRef &context, const List &existingObjects, const ObjectRef &object) const{
	return {};
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyList::CreateWidget(){
	return igdeMetaPropertyListWidget::Ref::New(*this);
}

void igdeMetaPropertyList::AddDefaultContextMenuEntries(igdeMenuCascade &menu,
const ContextRef &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, ActionDuplicate::Ref::New(*this, owner, context));
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, ActionRemove::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionRemoveAll::Ref::New(*this, owner, context));
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, ActionMoveUp::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionMoveDown::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionMoveTop::Ref::New(*this, owner, context));
	helper.MenuCommand(menu, ActionMoveBottom::Ref::New(*this, owner, context));
}

igdeMetaProperty::Action::Ref igdeMetaPropertyList::CreateDefaultButtonAction(
TargetButton target, igdeWidget &owner){
	switch(target){
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
