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

#include "igdeMetaPropertyObjectSetWidget.h"
#include "state/igdeMetaPropertyWidgetStateList.h"
#include "../undo/igdeMetaPropertyObjectSetUndo.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListener : public igdeListBoxListener{
	igdeMetaPropertyObjectSetWidget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertyObjectSetWidget &widget) :
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
	igdeMetaPropertyObjectSetWidget &pWidget;
	bool pSelection;
	
public:
	ActionCopy(igdeMetaPropertyObjectSetWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget), pSelection(false){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyObjectSet();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		igdeMetaPropertyObjectSet::Set values;
		if(pSelection){
			if(property.GetMultiSelection()){
				values = property.GetSelection(context);
				
			}else{
				auto activeObject = property.GetActiveObject(context);
				if(activeObject){
					values.Add(activeObject);
				}
			}
			
		}else{
			values = property.GetPropertyValue(context);
		}
		
		if(values.IsNotEmpty()){
			clipboard->Set(igdeMetaPropertyObjectSet::ClipboardData::Ref::New(property, std::move(values)));
		}
	}
};

class ActionCopySelection : public ActionCopy{
public:
	ActionCopySelection(igdeMetaPropertyObjectSetWidget &widget,
		const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
	ActionCopy(widget, context, environment)
	{
		SetText("@Igde.MetaPropertyList.Action.CopySelection");
		SetDescription("@Igde.MetaPropertyList.Action.CopySelection.ToolTip");
		pSelection = true;
	}
	
	~ActionCopySelection() override = default;
};


class ActionPaste : public igdeAction{
protected:
	igdeMetaPropertyObjectSetWidget &pWidget;
	bool pAppend;
	
public:
	ActionPaste(igdeMetaPropertyObjectSetWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pWidget(widget), pAppend(false){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyObjectSet();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		const auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		const auto clip = clipboard->GetWithTypeName(property.GetClipboardDataTypeName()).
			DynamicCast<igdeMetaPropertyObjectSet::ClipboardData>();
		if(!clip){
			return;
		}
		
		const auto oldValue = property.GetPropertyValue(context);
		const auto newValue = pAppend ? oldValue + clip->GetData() : clip->GetData();
		if(oldValue == newValue){
			return;
		}
		
		property.ChangePropertyValue(context, newValue, property.RealUndoInfo(context, *this));
	}
	
	void Update() override{
		if(pWidget.GetPropertyObjectSet().IsValid(pWidget.GetContext())){
			const auto cb = pWidget.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(pWidget.GetPropertyObjectSet().GetClipboardDataTypeName()));
			
		}else{
			SetEnabled(false);
		}
	}
};

class ActionPasteAppend : public ActionPaste{
public:
	ActionPasteAppend(igdeMetaPropertyObjectSetWidget &widget,
		const igdeMetaContext::Ref &context, igdeEnvironment &environment) :
	ActionPaste(widget, context, environment)
	{
		SetText("@Igde.MetaPropertyList.Action.PasteAppend");
		SetDescription("@Igde.MetaPropertyList.Action.PasteAppend.ToolTip");
		pAppend = true;
	}
	
	~ActionPasteAppend() override = default;
};


class cActionResetToDefault : public igdeAction{
	igdeMetaPropertyObjectSetWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyObjectSetWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pWidget(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyObjectSet();
		if(!property.IsValid(context) || property.GetPropertyValue(context).IsEmpty()){
			return;
		}
		
		property.ChangePropertyValue(context, {}, property.RealUndoInfo(context, *this));
	}
};

}


// Class igdeMetaPropertyObjectSetWidget::PropertyListener
////////////////////////////////////////////////////////////

igdeMetaPropertyObjectSetWidget::PropertyListener::PropertyListener(igdeMetaPropertyObjectSetWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyObjectSetWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyObjectSetWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyObjectSet*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}

void igdeMetaPropertyObjectSetWidget::PropertyListener::OnActiveChanged(
igdeMetaPropertyObjectSet*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.SelectActiveObject();
	}
}

void igdeMetaPropertyObjectSetWidget::PropertyListener::OnSelectionChanged(
igdeMetaPropertyObjectSet*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.RestoreSelection();
	}
}


// Class igdeMetaPropertyObjectSetWidget
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyObjectSetWidget::igdeMetaPropertyObjectSetWidget(igdeMetaPropertyObjectSet &property) :
igdeMetaPropertyWidget(property),
pPropertyObjectSet(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyObjectSetWidget::~igdeMetaPropertyObjectSetWidget(){
	Drop();
	pPropertyObjectSet.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyObjectSetWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pListBox)
	
	auto state = pPropertyObjectSet.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
	if(!state){
		state = deTObjectReference<igdeMetaPropertyWidgetStateList>::New();
		state->rows = pPropertyObjectSet.GetRows();
		pPropertyObjectSet.SetWidgetState(state);
	}
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ListBox(state->rows, pPropertyObjectSet.GetDescription(), pListBox, pListener);
	pListBox->SetDefaultSorter();
	pListBox->SetSelectionMode(pPropertyObjectSet.GetMultiSelection()
		? igdeListBox::esmMultiple : igdeListBox::esmSingle);
	
	auto buttons = igdeContainerFlow::Ref::New(helper.GetEnvironment(),
		igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	pAddButton(igdeMetaPropertyObjectSet::TargetButton::add, helper, buttons);
	pAddButton(igdeMetaPropertyObjectSet::TargetButton::remove, helper, buttons);
	if(buttons->GetChildren().IsEmpty()){
		buttons.Clear();
	}
	
	WrapEditWidget(container, helper, noLabel, pListBox, buttons);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyObjectSetWidget::Drop(){
	if(pListBox){
		auto state = pPropertyObjectSet.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
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

void igdeMetaPropertyObjectSetWidget::Update(){
	if(!pListBox){
		return;
	}
	
	const auto &context = GetContext();
	const bool valid = pPropertyObjectSet.IsValid(context);
	RunWithPreventUpdate([&]{
		pListBox->RemoveAllItems();
		if(valid){
			const auto set = pPropertyObjectSet.GetPropertyValue(context);
			igdeMetaContextItemInfo info;
			set.Visit([&](const deObject::Ref &object){
				pPropertyObjectSet.GetObjectItemInfo(context, object, info);
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

void igdeMetaPropertyObjectSetWidget::SelectActiveObject(){
	if(!pListBox || !pPropertyObjectSet.IsValid(GetContext())){
		return;
	}
	
	pListBox->SetSelectionWithRefData(pPropertyObjectSet.GetActiveObject(GetContext()));
	pListBox->MakeSelectionVisible();
}

void igdeMetaPropertyObjectSetWidget::StoreActiveObject(){
	if(!pListBox || !pPropertyObjectSet.IsValid(GetContext())){
		return;
	}
	
	pPropertyObjectSet.SetActiveObject(GetContext(), pListBox->GetSelectedItemRefData());
}

void igdeMetaPropertyObjectSetWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	const auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, env));
		helper.MenuCommand(menu, deTObjectReference<ActionCopySelection>::New(*this, context, env));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, env));
		helper.MenuCommand(menu, deTObjectReference<ActionPasteAppend>::New(*this, context, env));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyObjectSetWidget::StoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple
	|| !GetContext() || !pPropertyObjectSet.IsValid(GetContext())){
		return;
	}
	
	igdeMetaPropertyObjectSet::Set selection;
	pListBox->GetItems().Visit([&](const igdeListItem &item){
		if(item.GetSelected()){
			selection.Add(item.GetRefData());
		}
	});
	pPropertyObjectSet.SetSelection(GetContext(), selection);
}

void igdeMetaPropertyObjectSetWidget::RestoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple
	|| !GetContext() || !pPropertyObjectSet.IsValid(GetContext())){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto selection = pPropertyObjectSet.GetSelection(GetContext());
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

void igdeMetaPropertyObjectSetWidget::OnContextChanged(){
	Update();
	SelectActiveObject();
	RestoreSelection();
	
	pButtonActions.Visit([&](igdeMetaProperty::Action &action){
		action.SetContext(GetContext());
	});
}


// Private Functions
//////////////////////

void igdeMetaPropertyObjectSetWidget::pAddButton(igdeMetaPropertyObjectSet::TargetButton target,
igdeUIHelper &helper, igdeContainer &container){
	igdeButton::Ref button;
	helper.Button(button, {});
	button->SetStyle(igdeButton::ebsToolBar);
	auto action = pPropertyObjectSet.CreateButtonAction(target, button);
	if(!action){
		return;
	}
	
	action->SetContext(GetContext());
	button->SetAction(action);
	container.AddChild(button);
	pButtonActions.Add(action);
}
