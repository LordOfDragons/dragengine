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

#include "igdeMetaPropertySetWidget.h"
#include "state/igdeMetaPropertyWidgetStateList.h"
#include "../undo/igdeMetaPropertySetUndo.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListener : public igdeListBoxListener{
	igdeMetaPropertySetWidget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertySetWidget &widget) :
	pWidget(widget){
	}
	
	~cListener() override = default;
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(!pWidget.GetPreventUpdate()){
			pWidget.StoreActiveObject();
		}
	}
	
	void OnItemSelected(igdeListBox *listBox, int index) override{
		if(!pWidget.GetPreventUpdate()){
			pWidget.StoreSelection();
		}
	}
	
	void OnItemDeselected(igdeListBox *listBox, int index) override{
		if(!pWidget.GetPreventUpdate()){
			pWidget.StoreSelection();
		}
	}
	
	void OnDoubleClickItem(igdeListBox *listBox, int index) override{
	}
	
	void AddContextMenuEntries(igdeListBox *listBox, igdeMenuCascade &menu) override{
		pWidget.AddContextMenuEntries(menu);
	}
};


class ActionCopy : public igdeAction{
protected:
	igdeMetaPropertySetWidget &pWidget;
	bool pSelection;
	
public:
	ActionCopy(igdeMetaPropertySetWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertySet();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		igdeMetaPropertySet::Set values, copiedValues;
		if(property.GetMultiSelection()){
			values = property.GetSelection(context);
			
		}else{
			auto activeObject = property.GetActiveObject(context);
			if(activeObject){
				values.Add(activeObject);
			}
		}
		
		values.Visit([&](const deObject::Ref &object){
			const auto copiedObject = property.CopyObject(context, copiedValues, object);
			if(copiedObject){
				copiedValues.Add(copiedObject);
			}
		});
		
		if(copiedValues.IsEmpty()){
			return;
		}
		
		clipboard->Set(igdeMetaPropertySet::ClipboardData::Ref::New(property, std::move(copiedValues)));
	}
	
	void Update() override{
		auto &property = pWidget.GetPropertySet();
		const auto &context = pWidget.GetContext();
		SetEnabled(property.IsValid(context) && context->GetClipboard()
			&& !property.GetActiveObject(context).IsNotNull());
	}
};


class ActionCut : public igdeMetaPropertySet::ActionRemove{
protected:
	igdeAction::Ref pActionCopy;
	
public:
	ActionCut(igdeMetaPropertySetWidget &widget, const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
	igdeMetaPropertySet::ActionRemove(widget.GetPropertySet(), widget.GetButtonContextMenu(), context),
	pActionCopy(deTObjectReference<ActionCopy>::New(widget, context, environment))
	{
		SetText("@Igde.Action.Cut");
		SetIcon(environment.GetStockIcon(igdeEnvironment::esiCut));
		SetDescription("@Igde.Action.Cut.ToolTip");
	}
	
	~ActionCut() override = default;
	
	void OnAction() override{
		pActionCopy->OnAction();
		igdeMetaPropertySet::ActionRemove::OnAction();
	}
	
	void Update() override{
		igdeMetaPropertySet::ActionRemove::Update();
	}
};


class ActionPaste : public igdeAction{
protected:
	igdeMetaPropertySetWidget &pWidget;
	bool pAppend;
	
public:
	ActionPaste(igdeMetaPropertySetWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pWidget(widget){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertySet();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		const auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		const auto clip = clipboard->GetWithTypeName(property.GetClipboardDataTypeName()).
			DynamicCast<igdeMetaPropertySet::ClipboardData>();
		if(!clip){
			return;
		}
		
		const auto oldData = property.GetPropertyValue(context);
		igdeMetaPropertySet::Set pastedObjects;
		auto newData = oldData;
		
		clip->GetData().Visit([&](const deObject::Ref &object){
			const auto copiedObject = property.CopyObject(context, newData, object);
			if(copiedObject){
				newData.Add(copiedObject);
			}
		});
		
		if(oldData == newData){
			return;
		}
		
		property.ChangePropertyValue(context, newData, property.RealUndoInfo(context, *this));
		
		if(pastedObjects.IsNotEmpty()){
			property.SetActiveObject(context, pastedObjects.GetAt(0));
			
			if(property.GetMultiSelection()){
				property.SetSelection(context, pastedObjects);
			}
		}
	}
	
	void Update() override{
		if(pWidget.GetPropertySet().IsValid(pWidget.GetContext())){
			const auto cb = pWidget.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pWidget.GetPropertySet().GetClipboardDataTypeName()));
			return;
		}
		SetEnabled(false);
	}
};


class cActionResetToDefault : public igdeAction{
	igdeMetaPropertySetWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertySetWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pWidget(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertySet();
		if(!property.IsValid(context) || property.GetPropertyValue(context).IsEmpty()){
			return;
		}
		
		property.ChangePropertyValue(context, {}, property.RealUndoInfo(context, *this));
	}
};

}


// Class igdeMetaPropertySetWidget::PropertyListener
//////////////////////////////////////////////////////

igdeMetaPropertySetWidget::PropertyListener::PropertyListener(igdeMetaPropertySetWidget &widget) :
pWidget(widget){
}

igdeMetaPropertySetWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertySetWidget::PropertyListener::OnValueChanged(
igdeMetaPropertySet*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}

void igdeMetaPropertySetWidget::PropertyListener::OnActiveChanged(
igdeMetaPropertySet*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.SelectActiveObject();
	}
}

void igdeMetaPropertySetWidget::PropertyListener::OnSelectionChanged(
igdeMetaPropertySet*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.RestoreSelection();
	}
}

void igdeMetaPropertySetWidget::PropertyListener::OnObjectItemInfoChanged(
igdeMetaPropertySet*, const igdeMetaContext::Ref &context){
	if(!context || pWidget.GetContext() == context){
		pWidget.UpdateItemInfo();
	}
}


// Class igdeMetaPropertySetWidget
////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertySetWidget::igdeMetaPropertySetWidget(igdeMetaPropertySet &property) :
igdeMetaPropertyWidget(property),
pPropertySet(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertySetWidget::~igdeMetaPropertySetWidget(){
	Drop();
	pPropertySet.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertySetWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pListBox)
	
	auto state = pPropertySet.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
	if(!state){
		state = igdeMetaPropertyWidgetStateList::Ref::New();
		state->rows = pPropertySet.GetRows();
		pPropertySet.SetWidgetState(state);
	}
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ListBox(state->rows, pPropertySet.GetDescription(), pListBox, pListener);
	pListBox->SetDefaultSorter();
	pListBox->SetSelectionMode(pPropertySet.GetMultiSelection()
		? igdeListBox::esmMultiple : igdeListBox::esmSingle);
	
	auto buttons = igdeContainerFlow::Ref::New(helper.GetEnvironment(),
		igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	pAddButton(igdeMetaPropertySet::TargetButton::add, helper, buttons);
	pAddButton(igdeMetaPropertySet::TargetButton::remove, helper, buttons);
	if(buttons->GetChildren().IsEmpty()){
		buttons.Clear();
	}
	
	WrapEditWidget(container, helper, noLabel, pListBox, buttons);
	
	UpdateMatchable(container);
}

void igdeMetaPropertySetWidget::Drop(){
	if(pListBox){
		auto state = pPropertySet.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
		if(state){
			state->rows = pListBox->GetRows();
		}
		
		if(pListener){
			pListBox->RemoveListener(pListener);
		}
	}
	
	pListener.Clear();
	pListBox.Clear();
	pButtonActions.RemoveAll();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertySetWidget::Update(){
	if(!pListBox){
		return;
	}
	
	const auto &context = GetContext();
	const bool valid = pPropertySet.IsValid(context);
	RunWithPreventUpdate([&]{
		pListBox->RemoveAllItems();
		if(valid){
			const auto objects = pPropertySet.GetPropertyValue(context);
			igdeMetaContextItemInfo info;
			objects.Visit([&](const deObject::Ref &object){
				pPropertySet.GetObjectItemInfo(context, object, info);
				auto item = igdeListItem::Ref::New(info.GetText(), info.GetIcon(), info.GetDescription());
				item->SetRefData(object);
				pListBox->AddItem(item);
			});
			pListBox->SortItems();
		}
		pListBox->SetEnabled(valid);
	});
	
	SelectActiveObject();
	RestoreSelection();
	
	// the list automatically selects the first item if the list is not empty. this potentially
	// changes the active object without notification due to RunWithPreventUpdate(). store the
	// current selection and active object to properly synchronize if anything changed
	StoreSelection();
	StoreActiveObject();
}

void igdeMetaPropertySetWidget::UpdateItemInfo(){
	if(!pListBox){
		return;
	}
	
	const auto &context = GetContext();
	const bool valid = pPropertySet.IsValid(context);
	if(!valid){
		return;
	}
	
	igdeMetaContextItemInfo info;
	bool requiresSorting = true;
	
	pListBox->GetItems().VisitIndexed([&](int index, igdeListItem &item){
		pPropertySet.GetObjectItemInfo(context, item.GetRefData(), info);
		
		const auto &text = info.GetText();
		const auto &icon = info.GetIcon();
		const auto &description = info.GetDescription();
		if(item.GetText() == text && item.GetIcon() == icon && item.GetDescription() == description){
			return;
		}
		
		requiresSorting |= item.GetText() != text;
		
		item.SetText(text);
		item.SetIcon(icon);
		item.SetDescription(description);
		pListBox->ItemChangedAt(index);
	});
	
	if(requiresSorting){
		pListBox->SortItems();
	}
}

void igdeMetaPropertySetWidget::SelectActiveObject(){
	if(!pListBox || !pPropertySet.IsValid(GetContext())){
		return;
	}
	
	pListBox->SetSelectionWithRefData(pPropertySet.GetActiveObject(GetContext()));
	pListBox->MakeSelectionVisible();
}

void igdeMetaPropertySetWidget::StoreActiveObject(){
	if(!pListBox || !pPropertySet.IsValid(GetContext())){
		return;
	}
	
	pPropertySet.SetActiveObject(GetContext(), pListBox->GetSelectedItemRefData());
}

void igdeMetaPropertySetWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, env));
		helper.MenuCommand(menu, deTObjectReference<ActionCut>::New(*this, context, env));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, env));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertySetWidget::StoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple
	|| !pPropertySet.IsValid(GetContext())){
		return;
	}
	
	igdeMetaPropertySet::Set newValue;
	pListBox->GetItems().Visit([&](const igdeListItem &item){
		if(item.GetSelected()){
			newValue.Add(item.GetRefData());
		}
	});
	GetPropertySet().ChangePropertyValue(GetContext(), newValue);
}

void igdeMetaPropertySetWidget::RestoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple
	|| !pPropertySet.IsValid(GetContext())){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto selection = pPropertySet.GetSelection(GetContext());
		pListBox->GetItems().VisitIndexed([&](int index, igdeListItem &item){
			const bool selected = selection.Has(item.GetRefData());
			if(item.GetSelected() != selected){
				if(selected){
					pListBox->SelectItem(index);
					
				}else{
					pListBox->DeselectItem(index);
				}
			}
		});
	});
}



// Protected Functions
////////////////////////

void igdeMetaPropertySetWidget::OnContextChanged(){
	Update();
	SelectActiveObject();
	RestoreSelection();
	
	pButtonActions.Visit([&](igdeMetaProperty::Action &action){
		action.SetContext(GetContext());
	});
}


// Private Functions
//////////////////////

void igdeMetaPropertySetWidget::pAddButton(igdeMetaPropertySet::TargetButton target,
igdeUIHelper &helper, igdeContainer &container){
	igdeButton::Ref button;
	helper.Button(button, {});
	button->SetStyle(igdeButton::ebsToolBar);
	auto action = pPropertySet.CreateButtonAction(target, button);
	if(!action){
		return;
	}
	
	action->SetContext(GetContext());
	button->SetAction(action);
	container.AddChild(button);
	pButtonActions.Add(action);
}
