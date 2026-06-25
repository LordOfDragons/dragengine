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

#include "igdeMetaPropertyListWidget.h"
#include "state/igdeMetaPropertyWidgetStateList.h"
#include "../undo/igdeMetaPropertyListUndo.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListener : public igdeListBoxListener{
	igdeMetaPropertyListWidget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertyListWidget &widget) :
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


class cActionResetToDefault : public igdeAction{
	igdeMetaPropertyListWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyListWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pWidget(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyList();
		if(!property.IsValid(context) || property.GetPropertyValue(context).IsEmpty()){
			return;
		}
		
		property.ChangePropertyValue(context, {}, property.RealUndoInfo(context, *this));
	}
};

}


// Class igdeMetaPropertyListWidget::PropertyListener
///////////////////////////////////////////////////////

igdeMetaPropertyListWidget::PropertyListener::PropertyListener(igdeMetaPropertyListWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyListWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyListWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyList*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}

void igdeMetaPropertyListWidget::PropertyListener::OnActiveChanged(
igdeMetaPropertyList*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.SelectActiveObject();
	}
}

void igdeMetaPropertyListWidget::PropertyListener::OnSelectionChanged(
igdeMetaPropertyList*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.RestoreSelection();
	}
}

void igdeMetaPropertyListWidget::PropertyListener::OnObjectItemInfoChanged(
igdeMetaPropertyList*, const igdeMetaContext::Ref &context){
	if(!context || pWidget.GetContext() == context){
		pWidget.UpdateItemInfo();
	}
}


// Class igdeMetaPropertyListWidget
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyListWidget::igdeMetaPropertyListWidget(igdeMetaPropertyList &property) :
igdeMetaPropertyWidget(property),
pPropertyList(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyListWidget::~igdeMetaPropertyListWidget(){
	Drop();
	pPropertyList.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyListWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pListBox);
	
	auto state = pPropertyList.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
	if(!state){
		state = igdeMetaPropertyWidgetStateList::Ref::New();
		state->rows = pPropertyList.GetRows();
		pPropertyList.SetWidgetState(state);
	}
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ListBox(state->rows, pPropertyList.GetDescription(), pListBox, pListener);
	pListBox->SetSelectionMode(pPropertyList.GetMultiSelection()
		? igdeListBox::esmMultiple : igdeListBox::esmSingle);
	if(pPropertyList.GetSorted()){
		pListBox->SetDefaultSorter();
	}
	
	auto buttons = igdeContainerFlow::Ref::New(helper.GetEnvironment(),
		igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	pAddButton(igdeMetaPropertyList::TargetButton::add, helper, buttons);
	pAddButton(igdeMetaPropertyList::TargetButton::remove, helper, buttons);
	pAddButton(igdeMetaPropertyList::TargetButton::moveUp, helper, buttons);
	pAddButton(igdeMetaPropertyList::TargetButton::moveDown, helper, buttons);
	if(buttons->GetChildren().IsEmpty()){
		buttons.Clear();
	}
	
	WrapEditWidget(container, helper, noLabel, pListBox, buttons);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyListWidget::Drop(){
	if(pListBox){
		auto state = pPropertyList.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
		if(state){
			state->rows = pListBox->GetRows();
		}
		
		if(pListener){
			pListBox->RemoveListener(pListener);
		}
	}
	
	pButtonActions.RemoveAll();
	pListener.Clear();
	pListBox.Clear();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyListWidget::Update(){
	if(!pListBox){
		return;
	}
	
	const auto &context = GetContext();
	const bool valid = pPropertyList.IsValid(context);
	RunWithPreventUpdate([&]{
		pListBox->RemoveAllItems();
		if(valid){
			const auto objects = pPropertyList.GetPropertyValue(context);
			igdeMetaContextItemInfo info;
			objects.Visit([&](const deObject::Ref &object){
				pPropertyList.GetObjectItemInfo(context, object, info);
				auto item = igdeListItem::Ref::New(info.GetText(), info.GetIcon(), info.GetDescription());
				item->SetRefData(object);
				pListBox->AddItem(item);
			});
			if(pPropertyList.GetSorted()){
				pListBox->SortItems();
			}
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

void igdeMetaPropertyListWidget::UpdateItemInfo(){
	if(!pListBox){
		return;
	}
	
	const auto &context = GetContext();
	const bool valid = pPropertyList.IsValid(context);
	if(!valid){
		return;
	}
	
	igdeMetaContextItemInfo info;
	bool requiresSorting = true;
	
	pListBox->GetItems().VisitIndexed([&](int index, igdeListItem &item){
		pPropertyList.GetObjectItemInfo(context, item.GetRefData(), info);
		
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
	
	if(pPropertyList.GetSorted() && requiresSorting){
		pListBox->SortItems();
	}
}

void igdeMetaPropertyListWidget::SelectActiveObject(){
	if(!pListBox || !pPropertyList.IsValid(GetContext())){
		return;
	}
	
	pListBox->SetSelectionWithRefData(pPropertyList.GetActiveObject(GetContext()));
	pListBox->MakeSelectionVisible();
}

void igdeMetaPropertyListWidget::StoreActiveObject(){
	if(!pListBox || !pPropertyList.IsValid(GetContext())){
		return;
	}
	
	pPropertyList.SetActiveObject(GetContext(), pListBox->GetSelectedItemRefData());
}

void igdeMetaPropertyListWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, igdeMetaPropertyList::ActionCopy::Ref::New(
			pPropertyList, GetButtonContextMenu(), context));
		helper.MenuCommand(menu, igdeMetaPropertyList::ActionCut::Ref::New(
			pPropertyList, GetButtonContextMenu(), context));
		helper.MenuCommand(menu, igdeMetaPropertyList::ActionPaste::Ref::New(
			pPropertyList, GetButtonContextMenu(), context));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyListWidget::StoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple
	|| !pPropertyList.IsValid(GetContext())){
		return;
	}
	
	igdeMetaPropertyList::List newValue;
	pListBox->GetItems().Visit([&](const igdeListItem &item){
		if(item.GetSelected()){
			newValue.Add(item.GetRefData());
		}
	});
	pPropertyList.ChangePropertyValue(GetContext(), newValue);
}

void igdeMetaPropertyListWidget::RestoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple
	|| !pPropertyList.IsValid(GetContext())){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto selection = pPropertyList.GetSelection(GetContext());
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

void igdeMetaPropertyListWidget::OnContextChanged(){
	Update();
	SelectActiveObject();
	RestoreSelection();
	
	auto &context = GetContext();
	pButtonActions.Visit([&](igdeMetaProperty::Action &action){
		action.SetContext(context);
	});
}


// Private Functions
//////////////////////

void igdeMetaPropertyListWidget::pAddButton(igdeMetaPropertyList::TargetButton target,
igdeUIHelper &helper, igdeContainer &container){
	igdeButton::Ref button;
	helper.Button(button, {});
	button->SetStyle(igdeButton::ebsToolBar);
	auto action = pPropertyList.CreateButtonAction(target, button);
	if(!action){
		return;
	}
	
	action->SetContext(GetContext());
	button->SetAction(action);
	container.AddChild(button);
	pButtonActions.Add(action);
}
