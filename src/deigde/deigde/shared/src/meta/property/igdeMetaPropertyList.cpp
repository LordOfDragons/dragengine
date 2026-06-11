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
#include "../../environment/igdeEnvironment.h"
#include "../../gui/igdeUIHelper.h"
#include "../../undo/igdeUndoSystem.h"

#include <dragengine/common/collection/decTList.h>


struct sSortEntry{
	deObject::Ref entry{};
	int index = -1;
};


// Class igdeMetaPropertyList::Listener
/////////////////////////////////////////

void igdeMetaPropertyList::Listener::OnActiveChanged(igdeMetaPropertyList*, const igdeMetaContext::Ref&){
}

void igdeMetaPropertyList::Listener::OnSelectionChanged(igdeMetaPropertyList*, const igdeMetaContext::Ref&){
}


// Class igdeMetaPropertyList::ActionRemove
/////////////////////////////////////////////

igdeMetaPropertyList::ActionRemove::ActionRemove(igdeMetaPropertyList &property,
	const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
igdeAction("@Igde.MetaPropertyList.Action.Remove",
	environment.GetStockIcon(igdeEnvironment::esiMinus),
	"@Igde.MetaPropertyList.Action.Remove.ToolTip"),
pProperty(property),
pContext(context){
}

void igdeMetaPropertyList::ActionRemove::OnAction(){
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
		pProperty.ChangePropertyValue(pContext, newValue);
		
	}else{
		const auto active = pProperty.GetActiveObject(pContext);
		if(!active){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValue(pContext);
		newValue.Remove(active);
		pProperty.ChangePropertyValue(pContext, newValue);
	};
}

void igdeMetaPropertyList::ActionRemove::Update(){
	SetEnabled(pProperty.IsValid(pContext) && pProperty.GetActiveObject(pContext));
}


// Class igdeMetaPropertyList::ActionRemoveAll
////////////////////////////////////////////////

igdeMetaPropertyList::ActionRemoveAll::ActionRemoveAll(igdeMetaPropertyList &property,
	const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
igdeAction("@Igde.MetaPropertyList.Action.RemoveAll",
	environment.GetStockIcon(igdeEnvironment::esiDelete),
	"@Igde.MetaPropertyList.Action.RemoveAll.ToolTip"),
pProperty(property),
pContext(context){
}

void igdeMetaPropertyList::ActionRemoveAll::OnAction(){
	if(pProperty.IsValid(pContext) && pProperty.GetActiveObject(pContext)){
		pProperty.ChangePropertyValue(pContext, {});
	}
}

void igdeMetaPropertyList::ActionRemoveAll::Update(){
	SetEnabled(pProperty.IsValid(pContext) && pProperty.GetActiveObject(pContext));
}


// Class igdeMetaPropertyList::ActionMoveUp
/////////////////////////////////////////////

igdeMetaPropertyList::ActionMoveUp::ActionMoveUp(igdeMetaPropertyList &property,
	const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
igdeAction("@Igde.MetaPropertyList.Action.MoveUp",
	environment.GetStockIcon(igdeEnvironment::esiUp),
	"@Igde.MetaPropertyList.Action.MoveUp.ToolTip"),
pProperty(property),
pContext(context),
pTop(false){
}

void igdeMetaPropertyList::ActionMoveUp::OnAction(){
	if(!pProperty.IsValid(pContext)){
		return;
	}
	
	if(pProperty.GetMultiSelection()){
		const auto selection = pProperty.GetSelection(pContext);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValue(pContext);
		
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
			pProperty.ChangePropertyValue(pContext, newValue);
		}
		
	}else{
		const auto active = pProperty.GetActiveObject(pContext);
		if(!active){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValue(pContext);
		const int index = newValue.IndexOf(active);
		if(index == 0){
			return;
		}
		
		newValue.MoveIndex(index, pTop ? 0 : index - 1);
		pProperty.ChangePropertyValue(pContext, newValue);
	};
}

void igdeMetaPropertyList::ActionMoveUp::Update(){
	bool enabled = false;
	if(pProperty.IsValid(pContext)){
		if(pProperty.GetMultiSelection()){
			const auto selection = pProperty.GetSelection(pContext);
			if(selection.IsNotEmpty()){
				const auto list = pProperty.GetPropertyValue(pContext);
				
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
			const auto active = pProperty.GetActiveObject(pContext);
			enabled = active && pProperty.GetPropertyValue(pContext).First() != active;
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
	const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
ActionMoveUp(property, context, environment){
	SetText("@Igde.MetaPropertyList.Action.MoveTop");
	SetDescription("@Igde.MetaPropertyList.Action.MoveTop.ToolTip");
	SetIcon(environment.GetStockIcon(igdeEnvironment::esiStrongUp));
	SetTop(true);
}


// Class igdeMetaPropertyList::ActionMoveDown
///////////////////////////////////////////////

igdeMetaPropertyList::ActionMoveDown::ActionMoveDown(igdeMetaPropertyList &property,
	const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
igdeAction("@Igde.MetaPropertyList.Action.MoveDown",
	environment.GetStockIcon(igdeEnvironment::esiDown),
	"@Igde.MetaPropertyList.Action.MoveDown.ToolTip"),
pProperty(property),
pContext(context),
pBottom(false){
}

void igdeMetaPropertyList::ActionMoveDown::OnAction(){
	if(!pProperty.IsValid(pContext)){
		return;
	}
	
	if(pProperty.GetMultiSelection()){
		const auto selection = pProperty.GetSelection(pContext);
		if(selection.IsEmpty()){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValue(pContext);
		
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
			pProperty.ChangePropertyValue(pContext, newValue);
		}
		
	}else{
		const auto active = pProperty.GetActiveObject(pContext);
		if(!active){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValue(pContext);
		const int index = newValue.IndexOf(active);
		const int bottomIndex = newValue.GetCount() - 1;
		if(index == bottomIndex){
			return;
		}
		
		newValue.MoveIndex(index, pBottom ? bottomIndex : index + 1);
		pProperty.ChangePropertyValue(pContext, newValue);
	};
}

void igdeMetaPropertyList::ActionMoveDown::Update(){
	bool enabled = false;
	if(pProperty.IsValid(pContext)){
		if(pProperty.GetMultiSelection()){
			const auto selection = pProperty.GetSelection(pContext);
			if(selection.IsNotEmpty()){
				const auto list = pProperty.GetPropertyValue(pContext);
				
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
			const auto active = pProperty.GetActiveObject(pContext);
			enabled = active && pProperty.GetPropertyValue(pContext).Last() != active;
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
	const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
ActionMoveDown(property, context, environment){
	SetText("@Igde.MetaPropertyList.Action.MoveBottom");
	SetDescription("@Igde.MetaPropertyList.Action.MoveBottom.ToolTip");
	SetIcon(environment.GetStockIcon(igdeEnvironment::esiStrongDown));
	SetBottom(true);
}


// Class igdeMetaPropertyList
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyList::igdeMetaPropertyList(
	const char *id, const char *name, const char *description) :
igdeMetaProperty(id, name, description),
pRows(4),
pMultiSelection(false){
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

void igdeMetaPropertyList::NotifyValueChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnValueChanged(this, context);
	});
}

void igdeMetaPropertyList::NotifyActiveChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnActiveChanged(this, context);
	});
}

void igdeMetaPropertyList::NotifySelectionChanged(const igdeMetaContext::Ref &context){
	pListeners.Notify([&](Listener &listener){
		listener.OnSelectionChanged(this, context);
	});
}


igdeMetaPropertyListUndo::Ref igdeMetaPropertyList::ChangePropertyValue(
const igdeMetaContext::Ref &context, const List &newValue){
	if(context && context->GetUndoSystem()){
		auto undo = igdeMetaPropertyListUndo::Ref::New(*this, context, newValue);
		undo->Redo();
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

igdeMetaPropertyList::List igdeMetaPropertyList::GetSelection(const igdeMetaContext::Ref &context) const{
	auto active = GetActiveObject(context);
	return active ? List(devctag, active) : List();
}

void igdeMetaPropertyList::SetSelection(const igdeMetaContext::Ref &context, const List &selection){
}

igdeAction::Ref igdeMetaPropertyList::CreateButtonAction(
TargetButton, const igdeMetaContext::Ref&, igdeWidget&){
	return {};
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyList::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertyListWidget::Ref::New(*this, context);
}

void igdeMetaPropertyList::AddDefaultContextMenuEntries(igdeMenuCascade &menu,
const igdeMetaContext::Ref &context, igdeWidget &owner){
	auto &helper = menu.GetEnvironment().GetUIHelper();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	helper.MenuCommand(menu, ActionRemove::Ref::New(*this, context, owner.GetEnvironment()));
	helper.MenuCommand(menu, ActionRemoveAll::Ref::New(*this, context, owner.GetEnvironment()));
	
	helper.MenuSeparator(menu);
	helper.MenuCommand(menu, ActionMoveUp::Ref::New(*this, context, owner.GetEnvironment()));
	helper.MenuCommand(menu, ActionMoveDown::Ref::New(*this, context, owner.GetEnvironment()));
	helper.MenuCommand(menu, ActionMoveTop::Ref::New(*this, context, owner.GetEnvironment()));
	helper.MenuCommand(menu, ActionMoveBottom::Ref::New(*this, context, owner.GetEnvironment()));
}

igdeAction::Ref igdeMetaPropertyList::CreateDefaultButtonAction(TargetButton target,
	const igdeMetaContext::Ref &context, igdeWidget &owner){
	switch(target){
	case TargetButton::remove:
		return ActionRemove::Ref::New(*this, context, owner.GetEnvironment());
		
	case TargetButton::moveUp:
		return ActionMoveUp::Ref::New(*this, context, owner.GetEnvironment());
		
	case TargetButton::moveDown:
		return ActionMoveDown::Ref::New(*this, context, owner.GetEnvironment());
		
	default:
		return {};
	}
}
