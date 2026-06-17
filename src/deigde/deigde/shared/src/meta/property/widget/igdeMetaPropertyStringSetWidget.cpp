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

#include "igdeMetaPropertyStringSetWidget.h"
#include "../undo/igdeMetaPropertyStringSetUndo.h"
#include "../../igdeMetaContextItemInfo.h"
#include "../../../clipboard/igdeClipboard.h"
#include "../../../gui/igdeUIHelper.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"


namespace {

class cListener : public igdeListBoxListener{
	igdeMetaPropertyStringSetWidget &pWidget;
	
public:
	explicit cListener(igdeMetaPropertyStringSetWidget &widget) :
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


class ActionCopy : public igdeAction{
protected:
	igdeMetaPropertyStringSetWidget &pWidget;
	bool pSelection;
	
public:
	ActionCopy(igdeMetaPropertyStringSetWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Copy",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"@Igde.Action.Copy.ToolTip"),
	pWidget(widget), pSelection(false){
	}
	
	~ActionCopy() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyStringSet();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		const auto values = pSelection ? property.GetSelection(context)
			: property.GetPropertyValue(context);
		
		if(values.IsNotEmpty()){
			clipboard->Set(igdeMetaPropertyStringSet::ClipboardData::Ref::New(values));
		}
	}
};

class ActionCopySelection : public ActionCopy{
public:
	ActionCopySelection(igdeMetaPropertyStringSetWidget &widget,
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
	igdeMetaPropertyStringSetWidget &pWidget;
	bool pAppend;
	
public:
	ActionPaste(igdeMetaPropertyStringSetWidget &widget, const igdeMetaContext::Ref &context,
		igdeEnvironment &environment) :
	igdeAction("@Igde.Action.Paste",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"@Igde.Action.Paste.ToolTip"),
	pWidget(widget), pAppend(false){
	}
	
	~ActionPaste() override = default;
	
	void OnAction() override{
		auto &property = pWidget.GetPropertyStringSet();
		const auto &context = pWidget.GetContext();
		if(!property.IsValid(context)){
			return;
		}
		
		const auto clipboard = context->GetClipboard();
		if(!clipboard){
			return;
		}
		
		const auto clip = clipboard->GetWithTypeName(igdeMetaPropertyStringSet::ClipboardData::TypeName).
			DynamicCast<igdeMetaPropertyStringSet::ClipboardData>();
		if(!clip){
			return;
		}
		
		const auto oldValue = property.GetPropertyValue(context);
		const auto newValue = pAppend ? oldValue + clip->GetData() : clip->GetData();
		if(oldValue == newValue){
			return;
		}
		
		const auto &tm = pWidget.GetEnvironment().GetTranslationManager();
		property.ChangePropertyValue(context, newValue,
			tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(GetText()).ToUTF8());
	}
	
	void Update() override{
		if(pWidget.GetPropertyStringSet().IsValid(pWidget.GetContext())){
			const auto cb = pWidget.GetContext()->GetClipboard();
			SetEnabled(cb && cb->HasWithTypeName(igdeMetaPropertyStringSet::ClipboardData::TypeName));
			
		}else{
			SetEnabled(false);
		}
	}
};

class ActionPasteAppend : public ActionPaste{
public:
	ActionPasteAppend(igdeMetaPropertyStringSetWidget &widget,
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
	igdeMetaPropertyStringSetWidget &pWidget;
	
public:
	cActionResetToDefault(igdeMetaPropertyStringSetWidget &widget) :
	igdeAction("@Igde.MetaProperty.Action.ResetToDefault",
		widget.GetButtonContextMenu()->GetEnvironment().GetStockIcon(igdeEnvironment::esiUndo),
		"@Igde.MetaProperty.Action.ResetToDefault.ToolTip"),
	pWidget(widget){
	}
	
	~cActionResetToDefault() override = default;
	
	void OnAction() override{
		const auto &context = pWidget.GetContext();
		auto &property = pWidget.GetPropertyStringSet();
		if(!property.IsValid(context) || property.GetPropertyValue(context).IsEmpty()){
			return;
		}
		
		const auto &tm = pWidget.GetEnvironment().GetTranslationManager();
		property.ChangePropertyValue(context, {},
			tm.TranslateIf(property.GetUndoInfoOrLabel()).ToUTF8()
				+ ": " + tm.TranslateIf(GetText()).ToUTF8());
	}
};

}


// Class igdeMetaPropertyStringSetWidget::PropertyListener
////////////////////////////////////////////////////////////

igdeMetaPropertyStringSetWidget::PropertyListener::PropertyListener(igdeMetaPropertyStringSetWidget &widget) :
pWidget(widget){
}

igdeMetaPropertyStringSetWidget::PropertyListener::~PropertyListener() = default;

void igdeMetaPropertyStringSetWidget::PropertyListener::OnValueChanged(
igdeMetaPropertyStringSet*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.Update();
	}
}

void igdeMetaPropertyStringSetWidget::PropertyListener::OnActiveChanged(
igdeMetaPropertyStringSet*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.SelectActiveString();
	}
}

void igdeMetaPropertyStringSetWidget::PropertyListener::OnSelectionChanged(
igdeMetaPropertyStringSet*, const igdeMetaContext::Ref &context){
	if(pWidget.GetContext() == context){
		pWidget.RestoreSelection();
	}
}


// Class igdeMetaPropertyStringSetWidget
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyStringSetWidget::igdeMetaPropertyStringSetWidget(igdeMetaPropertyStringSet &property) :
igdeMetaPropertyWidget(property),
pPropertyStringSet(property),
pPropertyListener(PropertyListener::Ref::New(*this)),
pRows(property.GetRows())
{
	property.GetListeners().Add(pPropertyListener);
}

igdeMetaPropertyStringSetWidget::~igdeMetaPropertyStringSetWidget(){
	Drop();
	pPropertyStringSet.GetListeners().Remove(pPropertyListener);
}


// Management
///////////////

void igdeMetaPropertyStringSetWidget::Create(igdeContainer &container, igdeUIHelper &helper, bool noLabel){
	DEASSERT_NULL(pListBox)
	
	pListener = deTObjectReference<cListener>::New(*this);
	helper.ListBox(pRows, pPropertyStringSet.GetDescription(), pListBox, pListener);
	pListBox->SetDefaultSorter();
	pListBox->SetSelectionMode(pPropertyStringSet.GetMultiSelection()
		? igdeListBox::esmMultiple : igdeListBox::esmSingle);
	
	auto buttons = igdeContainerFlow::Ref::New(helper.GetEnvironment(),
		igdeContainerFlow::eaY, igdeContainerFlow::esNone);
	pAddButton(igdeMetaPropertyStringSet::TargetButton::add, helper, buttons);
	pAddButton(igdeMetaPropertyStringSet::TargetButton::remove, helper, buttons);
	if(buttons->GetChildren().IsEmpty()){
		buttons.Clear();
	}
	
	WrapEditWidget(container, helper, noLabel, pListBox, buttons);
	
	UpdateMatchable(container);
}

void igdeMetaPropertyStringSetWidget::Drop(){
	if(pListBox){
		if(pListener){
			pListBox->RemoveListener(pListener);
		}
		pRows = pListBox->GetRows();
	}
	
	pListener.Clear();
	pListBox.Clear();
	pButtonActions.RemoveAll();
	igdeMetaPropertyWidget::Drop();
}

void igdeMetaPropertyStringSetWidget::Update(){
	if(!pListBox){
		return;
	}
	
	const auto &context = GetContext();
	const bool valid = pPropertyStringSet.IsValid(context);
	RunWithPreventUpdate([&]{
		pListBox->RemoveAllItems();
		if(valid){
			const auto set = pPropertyStringSet.GetPropertyValue(context);
			igdeMetaContextItemInfo info;
			set.Visit([&](const decString &string){
				pPropertyStringSet.GetStringItemInfo(context, string, info);
				auto item = igdeListItem::Ref::New(info.GetText(), info.GetIcon(), info.GetDescription());
				item->SetRefData(igdeTMetaData<decString>::Ref::New(string));
				pListBox->AddItem(item);
			});
			pListBox->SortItems();
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

void igdeMetaPropertyStringSetWidget::SelectActiveString(){
	if(!pListBox || !pPropertyStringSet.IsValid(GetContext())){
		return;
	}
	
	const auto active = pPropertyStringSet.GetActiveString(GetContext());
	if(active){
		pListBox->SetSelection(pListBox->GetItems().IndexOfMatching([&](const igdeListItem &item){
			return item.GetRefData().DynamicCast<igdeTMetaData<decString>>()->GetData() == active->GetData();
		}));
	}
	pListBox->MakeSelectionVisible();
}

void igdeMetaPropertyStringSetWidget::StoreActiveString(){
	if(!pListBox || !pPropertyStringSet.IsValid(GetContext())){
		return;
	}
	
	const auto data = pListBox->GetSelectedItemRefData();
	pPropertyStringSet.SetActiveString(GetContext(), data
		? igdeMetaPropertyStringSet::StringRef::New(
			data.DynamicCast<igdeTMetaData<decString>>()->GetData())
		: igdeMetaPropertyStringSet::StringRef());
}

void igdeMetaPropertyStringSetWidget::AddContextMenuEntries(igdeMenuCascade &menu){
	igdeMetaPropertyWidget::AddContextMenuEntries(menu);
	
	auto &helper = menu.GetEnvironment().GetUIHelper();
	const auto &context = GetContext();
	
	if(menu.GetChildren().IsNotEmpty()){
		helper.MenuSeparator(menu);
	}
	
	if(context && context->GetClipboard()){
		helper.MenuCommand(menu, deTObjectReference<ActionCopy>::New(*this, context, helper.GetEnvironment()));
		helper.MenuCommand(menu, deTObjectReference<ActionCopySelection>::New(*this, context, helper.GetEnvironment()));
		helper.MenuCommand(menu, deTObjectReference<ActionPaste>::New(*this, context, helper.GetEnvironment()));
		helper.MenuCommand(menu, deTObjectReference<ActionPasteAppend>::New(*this, context, helper.GetEnvironment()));
		helper.MenuSeparator(menu);
	}
	
	helper.MenuCommand(menu, deTObjectReference<cActionResetToDefault>::New(*this));
}

void igdeMetaPropertyStringSetWidget::StoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple
	|| !GetContext() || !pPropertyStringSet.IsValid(GetContext())){
		return;
	}
	
	decStringSet selection;
	pListBox->GetItems().Visit([&](const igdeListItem &item){
		if(item.GetSelected()){
			selection.Add(item.GetRefData().DynamicCast<igdeTMetaData<decString>>()->GetData());
		}
	});
	pPropertyStringSet.SetSelection(GetContext(), selection);
}

void igdeMetaPropertyStringSetWidget::RestoreSelection(){
	if(!pListBox || pListBox->GetSelectionMode() != igdeListBox::esmMultiple
	|| !GetContext() || !pPropertyStringSet.IsValid(GetContext())){
		return;
	}
	
	RunWithPreventUpdate([&]{
		const auto selection = pPropertyStringSet.GetSelection(GetContext());
		pListBox->GetItems().VisitIndexed([&](int index, igdeListItem &item){
			const bool selected = selection.Has(item.GetRefData().DynamicCast<igdeTMetaData<decString>>()->GetData());
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

void igdeMetaPropertyStringSetWidget::OnContextChanged(){
	Update();
	SelectActiveString();
	RestoreSelection();
	
	pButtonActions.Visit([&](igdeMetaProperty::Action &action){
		action.SetContext(GetContext());
	});
}


// Private Functions
//////////////////////

void igdeMetaPropertyStringSetWidget::pAddButton(igdeMetaPropertyStringSet::TargetButton target,
igdeUIHelper &helper, igdeContainer &container){
	igdeButton::Ref button;
	helper.Button(button, {});
	button->SetStyle(igdeButton::ebsToolBar);
	auto action = pPropertyStringSet.CreateButtonAction(target, button);
	if(!action){
		return;
	}
	
	action->SetContext(GetContext());
	button->SetAction(action);
	container.AddChild(button);
	pButtonActions.Add(action);
}
