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

#include "igdeMetaPropertyStringListWidget.h"
#include "state/igdeMetaPropertyWidgetStateList.h"
#include "../undo/igdeMetaPropertyStringListUndo.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListener : public igdeListBoxListener{
	igdeMetaPropertyStringListWidget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertyStringListWidget &widget) :
	pWidget(widget){
	}
	
	~cListener() override = default;
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(!pWidget.GetPreventUpdate()){
			pWidget.StoreActiveString();
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
	igdeMetaPropertyStringListWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyStringListWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pWidget(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyStringList();
		if(!property.IsValid(context) || property.GetPropertyValue(context).IsEmpty()){
			return;
		}
		
		property.ChangePropertyValue(context, {}, property.RealUndoInfo(context, *this));
	}
};

}


// Class igdeMetaPropertyStringListWidget::PropertyListener
/////////////////////////////////////////////////////////////

igdeMetaPropertyStringListWidget::PropertyListener::PropertyListener(
		igdeMetaPropertyStringListWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyStringListWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyStringListWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyStringList*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}

void igdeMetaPropertyStringListWidget::PropertyListener::OnActiveChanged(
igdeMetaPropertyStringList*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.SelectActiveString();
	}
}

void igdeMetaPropertyStringListWidget::PropertyListener::OnSelectionChanged(
igdeMetaPropertyStringList*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.RestoreSelection();
	}
}


// Class igdeMetaPropertyStringListWidget
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyStringListWidget::igdeMetaPropertyStringListWidget(
		igdeMetaPropertyStringList &property) :
igdeMetaPropertyWidget(property),
pPropertyStringList(property),
pPropertyListener(PropertyListener::Ref::New(*this))
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyStringListWidget::~igdeMetaPropertyStringListWidget(){
	Drop();
	pPropertyStringList.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyStringListWidget::Create(igdeContainer &container,
		igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pListBox)
	
	auto state = pPropertyStringList.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
	if(!state){
		state = igdeMetaPropertyWidgetStateList::Ref::New();
		state->rows = pPropertyStringList.GetRows();
		pPropertyStringList.SetWidgetState(state);
	}
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ListBox(state->rows, pPropertyStringList.GetDescription(), pListBox, pListener);
	pListBox->SetSelectionMode(pPropertyStringList.GetMultiSelection()
		? igdeListBox::esmMultiple : igdeListBox::esmSingle);
	
	auto buttons = igdeContainerFlow::Ref::New(helper.GetEnvironment(),
		igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	pAddButton(igdeMetaPropertyStringList::TargetButton::add, helper, buttons);
	pAddButton(igdeMetaPropertyStringList::TargetButton::remove, helper, buttons);
	pAddButton(igdeMetaPropertyStringList::TargetButton::moveUp, helper, buttons);
	pAddButton(igdeMetaPropertyStringList::TargetButton::moveDown, helper, buttons);
	if(buttons->GetChildren().IsEmpty()){
		buttons.Clear();
	}
	
	WrapEditWidget(container, helper, noLabel, pListBox, buttons);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyStringListWidget::Drop(){
	if(pListBox){
		auto state = pPropertyStringList.GetWidgetState().DynamicCast<igdeMetaPropertyWidgetStateList>();
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

void igdeMetaPropertyStringListWidget::Update(){
	if(!pListBox){
		return;
	}
	
	const auto &context = GetContext();
	const bool valid = pPropertyStringList.IsValid(context);
	RunWithPreventUpdate([&]{
		pListBox->RemoveAllItems();
		if(valid){
			const auto &list = pPropertyStringList.GetPropertyValue(context);
			igdeMetaContextItemInfo info;
			list.Visit([&](const decString &string){
				pPropertyStringList.GetStringItemInfo(context, string, info);
				auto item = igdeListItem::Ref::New(info.GetText(), info.GetIcon(), info.GetDescription());
				item->SetRefData(igdeTMetaData<decString>::Ref::New(string));
				pListBox->AddItem(item);
			});
		}
		pListBox->SetEnabled(valid);
	});
	
	SelectActiveString();
	RestoreSelection();
	
	// the list automatically selects the first item if the list is not empty. this potentially
	// changes the active object without notification due to RunWithPreventUpdate(). store the
	// current selection and active object to properly synchronize if anything changed
	StoreSelection();
	StoreActiveString();
}

void igdeMetaPropertyStringListWidget::SelectActiveString(){
	if(!pListBox || !pPropertyStringList.IsValid(GetContext())){
		return;
	}
	
	const auto active = pPropertyStringList.GetActiveString(GetContext());
	if(active){
		pListBox->SetSelection(pListBox->GetItems().IndexOfMatching([&](const igdeListItem &item){
			return item.GetRefData().DynamicCast<igdeTMetaData<decString>>()->GetData() == active->GetData();
		}));
	}
	pListBox->MakeSelectionVisible();
}

void igdeMetaPropertyStringListWidget::StoreActiveString(){
	if(!pListBox || !pPropertyStringList.IsValid(GetContext())){
		return;
	}
	
	const auto data = pListBox->GetSelectedItemRefData();
	pPropertyStringList.SetActiveString(GetContext(), data
		? igdeMetaPropertyStringList::StringRef::New(
			data.DynamicCast<igdeTMetaData<decString>>()->GetData())
		: igdeMetaPropertyStringList::StringRef());
}

void igdeMetaPropertyStringListWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &env = menu.GetEnvironment();
	auto &helper = env.GetUIHelper();
	const auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, igdeMetaPropertyStringList::ActionCopy::Ref::New(
			pPropertyStringList, GetButtonContextMenu(), context));
		helper.MenuCommand(menu, igdeMetaPropertyStringList::ActionCopySelection::Ref::New(
			pPropertyStringList, GetButtonContextMenu(), context));
		helper.MenuCommand(menu, igdeMetaPropertyStringList::ActionCut::Ref::New(
			pPropertyStringList, GetButtonContextMenu(), context));
		helper.MenuCommand(menu, igdeMetaPropertyStringList::ActionPaste::Ref::New(
			pPropertyStringList, GetButtonContextMenu(), context));
		helper.MenuCommand(menu, igdeMetaPropertyStringList::ActionPasteAppend::Ref::New(
			pPropertyStringList, GetButtonContextMenu(), context));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyStringListWidget::StoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple
		|| !GetContext() || !pPropertyStringList.IsValid(GetContext())){
		return;
	}
	
	decStringList selection;
	pListBox->GetItems().Visit([&](const igdeListItem &item){
		if(item.GetSelected()){
			selection.Add(item.GetRefData().DynamicCast<igdeTMetaData<decString>>()->GetData());
		}
	});
	pPropertyStringList.SetSelection(GetContext(), selection);
}

void igdeMetaPropertyStringListWidget::RestoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple
		|| !GetContext() || !pPropertyStringList.IsValid(GetContext())){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto selection = pPropertyStringList.GetSelection(GetContext());
		pListBox->GetItems().VisitIndexed([&](int index, igdeListItem &item){
			const bool selected = selection.Has(
				item.GetRefData().DynamicCast<igdeTMetaData<decString>>()->GetData());
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

void igdeMetaPropertyStringListWidget::OnContextChanged(){
	Update();
	SelectActiveString();
	RestoreSelection();
	
	pButtonActions.Visit([&](igdeMetaProperty::Action &action){
		action.SetContext(GetContext());
	});
}


// Private Functions
//////////////////////

void igdeMetaPropertyStringListWidget::pAddButton(igdeMetaPropertyStringList::TargetButton target,
igdeUIHelper &helper, igdeContainer &container){
	igdeButton::Ref button;
	helper.Button(button, {});
	button->SetStyle(igdeButton::ebsToolBar);
	auto action = pPropertyStringList.CreateButtonAction(target, button);
	if(!action){
		return;
	}
	
	action->SetContext(GetContext());
	button->SetAction(action);
	container.AddChild(button);
	pButtonActions.Add(action);
}
