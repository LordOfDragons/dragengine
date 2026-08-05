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

#include "igdeMetaPropertyUndoHistoryWidget.h"
#include "state/igdeMetaPropertyWidgetStateList.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cActionUndo : public igdeMetaProperty::Action{
	igdeMetaPropertyUndoHistoryWidget &pWidget;
	
public:
	cActionUndo(igdeMetaPropertyUndoHistoryWidget &widget, igdeWidget &owner,
		const igdeMetaContext::Ref &context = {}) :
	Action(owner, context, "@Igde.Action.Undo",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.Action.Undo.ToolTip"),
	pWidget(widget){
	}
	
	~cActionUndo() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyUndoHistory();
		if(!property.IsValid(context)){
			return;
		}
		
		auto undoSystem = property.GetPropertyValue(context);
		if(undoSystem && undoSystem->GetUndoableCount() > 0){
			undoSystem->Undo();
		}
	}
	
	void Update() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyUndoHistory();
		if(!property.IsValid(context)){
			SetEnabled(false);
			return;
		}
		
		const auto undoSystem = property.GetPropertyValue(context);
		SetEnabled(undoSystem && undoSystem->GetUndoableCount() > 0);
	}
};

class cActionRedo : public igdeMetaProperty::Action{
	igdeMetaPropertyUndoHistoryWidget &pWidget;
	
public:
	cActionRedo(igdeMetaPropertyUndoHistoryWidget &widget, igdeWidget &owner,
		const igdeMetaContext::Ref &context = {}) :
	Action(owner, context, "@Igde.Action.Redo",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiRedo),
		"@Igde.Action.Redo.ToolTip"),
	pWidget(widget){
	}
	
	~cActionRedo() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyUndoHistory();
		if(!property.IsValid(context)){
			return;
		}
		
		auto undoSystem = property.GetPropertyValue(context);
		if(undoSystem && undoSystem->GetRedoableCount() > 0){
			undoSystem->Redo();
		}
	}
	
	void Update() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyUndoHistory();
		if(!property.IsValid(context)){
			SetEnabled(false);
			return;
		}
		
		const auto undoSystem = property.GetPropertyValue(context);
		SetEnabled(undoSystem && undoSystem->GetRedoableCount() > 0);
	}
};

class cActionClear : public igdeMetaProperty::Action{
	igdeMetaPropertyUndoHistoryWidget &pWidget;
	
public:
	cActionClear(igdeMetaPropertyUndoHistoryWidget &widget, igdeWidget &owner,
		const igdeMetaContext::Ref &context = {}) :
	Action(owner, context, "@Igde.Action.ClearUndo",
		owner.GetEnvironment().GetStockIcon(igdeEnvironment::esiDelete),
		"@Igde.Action.ClearUndo.ToolTip"),
	pWidget(widget){
	}
	
	~cActionClear() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyUndoHistory();
		if(!property.IsValid(context)){
			return;
		}
		
		auto undoSystem = property.GetPropertyValue(context);
		if(undoSystem && undoSystem->GetUndos().IsNotEmpty()){
			undoSystem->RemoveAll();
		}
	}
	
	void Update() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyUndoHistory();
		if(!property.IsValid(context)){
			SetEnabled(false);
			return;
		}
		
		const auto undoSystem = property.GetPropertyValue(context);
		SetEnabled(undoSystem && undoSystem->GetUndos().IsNotEmpty());
	}
};

}


// Class igdeMetaPropertyListWidget::PropertyListener
///////////////////////////////////////////////////////

igdeMetaPropertyUndoHistoryWidget::PropertyListener::PropertyListener(
	igdeMetaPropertyUndoHistoryWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyUndoHistoryWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyUndoHistoryWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyUndoHistory*, const igdeMetaContext::Ref&){
	pWidget.Update();
}


// Class igdeMetaPropertyUndoHistoryWidget
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyUndoHistoryWidget::igdeMetaPropertyUndoHistoryWidget(
	igdeMetaPropertyUndoHistory &property) :
igdeMetaPropertyWidget(property),
pPropertyUndoHistory(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyUndoHistoryWidget::~igdeMetaPropertyUndoHistoryWidget(){
	Drop();
	pPropertyUndoHistory.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyUndoHistoryWidget::Create(Builder &builder, bool noLabel){
	DEASSERT_NULL(pListBox);
	
	auto &helper = builder.GetHelper();
	helper.ListBox(pPropertyUndoHistory.GetRows(), pPropertyUndoHistory.GetDescription(), pListBox, {});
	pListBox->SetSelectionMode(igdeListBox::esmSingle);
	pListBox->SetAutoTranslateItems(true);
	pListBox->SetEnabled(false);
	OnActivate();
	
	auto buttons = igdeContainerFlow::Ref::New(helper.GetEnvironment(),
		igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	pAddButton(deTObjectReference<cActionUndo>::New(*this, pListBox), helper, buttons);
	pAddButton(deTObjectReference<cActionRedo>::New(*this, pListBox), helper, buttons);
	
	WrapEditWidget(builder, noLabel, pListBox, buttons);
	
	UpdateMatchable();
	SetContext(builder.GetContext());
}

void igdeMetaPropertyUndoHistoryWidget::Drop(){
	OnDeactivate();
	
	pButtonActions.RemoveAll();
	pListBox.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyUndoHistoryWidget::Update(){
	if(!pListBox){
		return;
	}
	
	const auto &context = GetContext();
	const bool valid = pPropertyUndoHistory.IsValid(context);
	RunWithPreventUpdate([&]{
		pListBox->RemoveAllItems();
		if(valid){
			const auto undoSystem = pPropertyUndoHistory.GetPropertyValue(context);
			if(undoSystem){
				auto &environment = pListBox->GetEnvironment();
				auto iconUndo = environment.GetStockIcon(igdeEnvironment::esiUndo);
				auto iconRedo = environment.GetStockIcon(igdeEnvironment::esiRedo);
				const int redoCount = undoSystem->GetRedoableCount();
				
				undoSystem->GetUndos().VisitIndexed([&](int i, const igdeUndo &undo){
					pListBox->AddItem(undo.GetShortInfo(), i < redoCount ? iconRedo : iconUndo);
				});
				
				if(undoSystem->GetUndos().IsNotEmpty()){
					pListBox->SetSelection(decMath::min(redoCount, undoSystem->GetUndos().GetCount() - 1));
				}
			}
		}
		pListBox->SetEnabled(valid);
	});
	
	pButtonActions.Visit([&](igdeMetaProperty::Action &action){
		action.Update();
	});
	igdeMetaPropertyWidget::Update();
}

void igdeMetaPropertyUndoHistoryWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	auto &owner = GetButtonContextMenu();
	helper.MenuCommand(menu, deTObjectReference<cActionUndo>::New(*this, owner, context));
	helper.MenuCommand(menu, deTObjectReference<cActionRedo>::New(*this, owner, context));
	helper.MenuSeparator(menu);
	
	helper.MenuCommand(menu, deTObjectReference<cActionClear>::New(*this, owner, context));
}

bool igdeMetaPropertyUndoHistoryWidget::IsPropertyValid() const{
	return pPropertyUndoHistory.IsValid(GetContext());
}

void igdeMetaPropertyUndoHistoryWidget::OnActivate(){
	auto state = pPropertyUndoHistory.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
	if(!state){
		state = deTObjectReference<igdeMetaPropertyWidgetStateList>::New();
		state->rows = pPropertyUndoHistory.GetRows();
		pPropertyUndoHistory.SetWidgetState(state);
	}
	
	if(pListBox){
		pListBox->SetRows(state->rows);
	}
}

void igdeMetaPropertyUndoHistoryWidget::OnDeactivate(){
	if(!pListBox){
		return;
	}
	
	auto state = pPropertyUndoHistory.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
	if(!state){
		state = deTObjectReference<igdeMetaPropertyWidgetStateList>::New();
		pPropertyUndoHistory.SetWidgetState(state);
	}
	
	state->rows = pListBox->GetRows();
}


// Protected Functions
////////////////////////

void igdeMetaPropertyUndoHistoryWidget::OnContextChanged(){
	Update();
	
	auto &context = GetContext();
	pButtonActions.Visit([&](igdeMetaProperty::Action &action){
		action.SetContext(context);
	});
}


// Private Functions
//////////////////////

void igdeMetaPropertyUndoHistoryWidget::pAddButton(const igdeMetaProperty::Action::Ref &action,
igdeUIHelper &helper, igdeContainer &container){
	igdeButton::Ref button;
	helper.Button(button, action);
	button->SetStyle(igdeButton::ebsToolBar);
	container.AddChild(button);
	pButtonActions.Add(action);
}
