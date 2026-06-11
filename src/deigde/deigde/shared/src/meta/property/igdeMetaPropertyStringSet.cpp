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
#include "../../gui/igdeUIHelper.h"
#include "../../environment/igdeEnvironment.h"
#include "../../undo/igdeUndoSystem.h"


// Class igdeMetaPropertyStringSet::Listener
//////////////////////////////////////////////

void igdeMetaPropertyStringSet::Listener::OnActiveChanged(igdeMetaPropertyStringSet*, const igdeMetaContext::Ref&){
}

void igdeMetaPropertyStringSet::Listener::OnSelectionChanged(igdeMetaPropertyStringSet*, const igdeMetaContext::Ref&){
}


// Class igdeMetaPropertyStringSet::ActionRemove
//////////////////////////////////////////////////

igdeMetaPropertyStringSet::ActionRemove::ActionRemove(igdeMetaPropertyStringSet &property,
	const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
igdeAction("@Igde.MetaPropertyList.Action.Remove",
	environment.GetStockIcon(igdeEnvironment::esiMinus),
	"@Igde.MetaPropertyList.Action.Remove.ToolTip"),
pProperty(property),
pContext(context){
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
		pProperty.ChangePropertyValue(pContext, newValue);
		
	}else{
		const auto active = pProperty.GetActiveString(pContext);
		if(!active){
			return;
		}
		
		auto newValue = pProperty.GetPropertyValue(pContext);
		newValue.Remove(active);
		pProperty.ChangePropertyValue(pContext, newValue);
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
pContext(context){
}

void igdeMetaPropertyStringSet::ActionRemoveAll::OnAction(){
	if(pProperty.IsValid(pContext) && pProperty.GetActiveString(pContext)){
		pProperty.ChangePropertyValue(pContext, {});
	}
}

void igdeMetaPropertyStringSet::ActionRemoveAll::Update(){
	SetEnabled(pProperty.IsValid(pContext) && pProperty.GetActiveString(pContext));
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
const igdeMetaContext::Ref &context, const decStringSet &newValue){
	if(context && context->GetUndoSystem()){
		auto undo = igdeMetaPropertyStringSetUndo::Ref::New(*this, context, newValue);
		undo->Redo();
		context->GetUndoSystem()->Add(undo);
		return undo;
		
	}else{
		SetPropertyValue(context, newValue);
		return {};
	}
}

decStringSet igdeMetaPropertyStringSet::GetSelection(const igdeMetaContext::Ref &context) const{
	auto active = GetActiveString(context);
	return active ? decStringSet(devctag, active) : decStringSet();
}

void igdeMetaPropertyStringSet::SetSelection(const igdeMetaContext::Ref&, const decStringSet&){
}

void igdeMetaPropertyStringSet::AddContextMenuEntriesAdd(igdeMenuCascade&, const igdeMetaContext::Ref&, igdeWidget&){
}

igdeAction::Ref igdeMetaPropertyStringSet::CreateButtonAction(TargetButton, const igdeMetaContext::Ref&, igdeWidget&){
	return {};
}

igdeMetaPropertyWidget::Ref igdeMetaPropertyStringSet::CreateWidget(const igdeMetaContext::Ref &context){
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
}

igdeAction::Ref igdeMetaPropertyStringSet::CreateDefaultButtonAction(TargetButton target,
	const igdeMetaContext::Ref &context, igdeWidget &owner){
	switch(target){
	case TargetButton::remove:
		return ActionRemove::Ref::New(*this, context, owner.GetEnvironment());
		
	default:
		return {};
	}
}
